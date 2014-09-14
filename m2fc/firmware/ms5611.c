/*
 * MS5611-01BA03 Driver
 * M2FC
 * 2014 Adam Greig, Cambridge University Spaceflight
 */

#include "ms5611.h"
#include "microsd.h"

#include "hal.h"
#include "chprintf.h"

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
 * Resets the MS5611. Sends 0x1E, waits 5ms.
 */
static void ms5611_reset()
{
    uint8_t adr = 0x1E;
    spiSelect(&MS5611_SPID);
    spiSend(&MS5611_SPID, 1, (void*)&adr);
    chThdSleepMilliseconds(5);
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
    int32_t t0;

    /* Start conversion */
    spiSelect(&MS5611_SPID);
    spiSend(&MS5611_SPID, 1, (void*)&adr);
    
    /*
     * Wait for conversion to complete. There doesn't appear to be any way
     * to do this without timing it, unfortunately.
     *
     * If we just watch the clock we'll consume enormous CPU time for little
     * gain. Instead we'll sleep for "roughly" 1ms and then wait until at least
     * the desired 0.6ms have passed.
     */
    t0 = halGetCounterValue();
    chThdSleepMilliseconds(1);
    while(halGetCounterValue() - t0 < US2RTT(600)) {
        chThdYield();
    }

    /* Deassert CS */
    spiUnselect(&MS5611_SPID);

    /* Read ADC result */
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
    uint16_t d0, d7;
    ms5611_read_u16(0xA0, &d0);
    ms5611_read_u16(0xA2, &(cal_data->c1));
    ms5611_read_u16(0xA4, &(cal_data->c2));
    ms5611_read_u16(0xA6, &(cal_data->c3));
    ms5611_read_u16(0xA8, &(cal_data->c4));
    ms5611_read_u16(0xAA, &(cal_data->c5));
    ms5611_read_u16(0xAC, &(cal_data->c6));
    ms5611_read_u16(0xAE, &d7);

    microsd_log_u16(CHAN_CAL_BARO1,
                    d0, cal_data->c1, cal_data->c2, cal_data->c3);
    microsd_log_u16(CHAN_CAL_BARO2,
                    cal_data->c4, cal_data->c5, cal_data->c6, d7);
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

    microsd_log_s32(CHAN_IMU_BARO, *pressure, *temperature);
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

    chRegSetThreadName("MS5611");

    spiStart(&MS5611_SPID, &spi_cfg);
    ms5611_init(&cal_data);

    while (TRUE) {
        ms5611_read(&cal_data, &temperature, &pressure);
        state_estimation_new_pressure((float)pressure);
    }

    return (msg_t)NULL;
}
