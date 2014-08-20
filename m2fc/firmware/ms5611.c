/*
 * MS5611-01BA03 Driver
 * M2FC
 * 2014 Adam Greig, Cambridge University Spaceflight
 */

#include "ms5611.h"
#include "microsd.h"

#include "hal.h"

#define MS5611_SPID        SPID3
#define MS5611_SPI_CS_PORT GPIOD
#define MS5611_SPI_CS_PIN  GPIOD_BARO_CS

static void ms5611_reset(void);
static void ms5611_read_u16(uint8_t adr, uint16_t* c);
static void ms5611_read_s24(uint8_t adr, int32_t* d);
static void ms5611_init(MS5611CalData* cal_data);
static void ms5611_read_cal(MS5611CalData* cal_data);
static void ms5611_read(MS5611CalData* cal_data,
                        int32_t* temperature, int32_t* pressure);

/*
 * Resets the MS5611. Sends 0x1E, waits 3ms.
 */
static void ms5611_reset()
{
    uint8_t adr = 0x1E;
    spiSelect(&MS5611_SPID);
    spiSend(&MS5611_SPID, 1, (void*)&adr);
    chThdSleepMilliseconds(3);
    spiUnselect(&MS5611_SPID);
}

/*
 * Reads a uint16 from the MS5611 address `adr`, stores it to `c`.
 */
static void ms5611_read_u16(uint8_t adr, uint16_t* c)
{
    uint8_t rx[2];
    spiSelect(&MS5611_SPID);
    spiSend(&MS5611_SPID, 1, (void*)&adr);
    spiReceive(&MS5611_SPID, 2, (void*)rx);
    spiUnselect(&MS5611_SPID);

    *c = rx[0] << 8 | rx[1];
}

/*
 * Reads an int24 from the MS5611 address `adr`, stores it to `d`.
 */
static void ms5611_read_s24(uint8_t adr, int32_t* d)
{
    uint8_t adc_adr = 0x00, rx[3];

    spiSelect(&MS5611_SPID);
    spiSend(&MS5611_SPID, 1, (void*)&adr);
    spiUnselect(&MS5611_SPID);

    chThdSleepMilliseconds(1);

    spiSelect(&MS5611_SPID);
    spiSend(&MS5611_SPID, 1, (void*)&adc_adr);
    spiReceive(&MS5611_SPID, 3, (void*)rx);
    spiUnselect(&MS5611_SPID);

    *d = rx[0] << 16 | rx[1] << 8 | rx[2];
}

/*
 * Reads MS5611 calibration data, writes it to `cal_data`.
 */
static void ms5611_read_cal(MS5611CalData* cal_data)
{
    ms5611_read_u16(0xA2, &(cal_data->c1));
    ms5611_read_u16(0xA4, &(cal_data->c2));
    ms5611_read_u16(0xA6, &(cal_data->c3));
    ms5611_read_u16(0xA8, &(cal_data->c4));
    ms5611_read_u16(0xAA, &(cal_data->c5));
    ms5611_read_u16(0xAC, &(cal_data->c6));
}

/*
 * Initialise the MS5611.
 * Sends a RESET and then reads in the calibration data.
 *
 * Call this once system startup, before attempting ms5611_read.
 *
 * cal_data should be a pointer to some memory to store the calibration in.
 */
static void ms5611_init(MS5611CalData* cal_data)
{
    ms5611_reset();
    ms5611_read_cal(cal_data);
}

/*
 * Read and compensate a temperature and pressure from the MS5611.
 *
 * `cal_data` is previously read calibration data.
 * `temperature` and `pressure` are written to.
 *
 * `temperature` is in centidegrees Celcius,
 * `pressure` is in Pascals.
 */
static void ms5611_read(MS5611CalData* cal_data,
                        int32_t* temperature, int32_t* pressure)
{
    int32_t d1, d2;
    int64_t off, sens, dt;
    ms5611_read_s24(0x40, &d1);
    ms5611_read_s24(0x50, &d2);

    dt = (int64_t)d2 - (int64_t)cal_data->c5 * (1<<8);
    *temperature = 2000 + (dt * (int64_t)cal_data->c6) / (1<<23);
    
    off = (int64_t)cal_data->c2 * (1<<16) + \
          ((int64_t)cal_data->c4 * dt) / (1<<7);

    sens = (int64_t)cal_data->c1 * (1<<15) + \
           ((int64_t)cal_data->c3 * dt) / (1<<8);

    *pressure = ((d1 * sens) / (1<<21) - off) / (1<<15);
}

/*
 * MS5611 main thread.
 * Resets the MS5611, reads cal data, then reads a pressure and temperature
 * in a loop.
 */
msg_t ms5611_thread(void *arg)
{
    (void)arg;

    static const SPIConfig spi_cfg = {
        NULL,
        MS5611_SPI_CS_PORT,
        MS5611_SPI_CS_PIN,
        SPI_CR1_BR_1 | SPI_CR1_CPOL | SPI_CR1_CPHA
    };

    static MS5611CalData cal_data;
    static int32_t temperature, pressure;
    char log[41];

    chRegSetThreadName("MS5611");

    spiStart(&MS5611_SPID, &spi_cfg);
    ms5611_init(&cal_data);

    microsd_log("Init complete");

    while (TRUE) {
        ms5611_read(&cal_data, &temperature, &pressure);
        chsnprintf(log, 41, "%d,%d", temperature, pressure);
        microsd_log(log);
    }

    return (msg_t)NULL;
}
