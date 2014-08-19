/*
 * MS5611-01BA03 Driver
 * M2FC
 * 2014 Adam Greig, Cambridge University Spaceflight
 */

#include "ms5611.h"
#define MS5611_SPID        SPID3
#define MS5611_SPI_CS_PORT GPIOD
#define MS5611_SPI_CS_PIN  GPIOD_BARO_CS

void ms5611_reset(void);
void ms5611_read_cal(MS5611CalData* cal_data);
void ms5611_read_u16(uint8_t adr, uint16_t* c);
void ms5611_read_s24(uint8_t adr, int32_t* d);

void ms5611_init(MS5611CalData* cal_data)
{
    ms5611_reset();
    ms5611_read_cal(cal_data);
}

void ms5611_read(MS5611CalData* cal_data,
                 int32_t* temperature, int32_t* pressure)
{
    int32_t d1, d2;
    int64_t off, sens, off2, sens2, t2, dt;
    ms5611_read_s24(0x42, &d1);
    ms5611_read_s24(0x52, &d2);

    dt = (int64_t)d2 - (int64_t)cal_data->c5 * (1<<8);
    *temperature = 2000 + (dt * (int64_t)cal_data->c6) / (1<<23);
    
    off = (int64_t)cal_data->c2 * (1<<16) + \
          ((int64_t)cal_data->c4 * dt) / (1<<7);

    sens = (int64_t)cal_data->c1 * (1<<15) + \
           ((int64_t)cal_data->c3 * dt) / (1<<8);

    if (*temperature < 2000) {
        t2 = dt * dt / (1<<31);
        off2 = 5 * ((*temperature - 2000) * (*temperature - 2000))/2;
        sens2 = 5 * ((*temperature - 2000) * (*temperature - 2000))/4;

        if (*temperature < -1500) {
            off2 += 7 * ((*temperature + 1500) * (*temperature + 1500));
            sens2 += 11 * ((*temperature + 1500) * (*temperature + 1500)) / 2;
        }
    } else {
        t2 = 0;
        off2 = 0;
        sens2 = 0;
    }

    *temperature -= t2;
    off -= off2;
    sens -= sens2;
    *pressure = ((d1 * sens) / (1<<21) - off) / (1<<15);
}

void ms5611_reset()
{
    uint8_t adr = 0x1E;
    spiSelect(&MS5611_SPID);
    spiSend(&MS5611_SPID, 1, (void*)&adr);
    chThdSleepMilliseconds(3);
    spiUnselect(&MS5611_SPID);
    chThdSleepMilliseconds(1);
}

void ms5611_read_cal(MS5611CalData* cal_data)
{
    ms5611_read_u16(0xA2, &(cal_data->c1));
    ms5611_read_u16(0xA4, &(cal_data->c2));
    ms5611_read_u16(0xA6, &(cal_data->c3));
    ms5611_read_u16(0xA8, &(cal_data->c4));
    ms5611_read_u16(0xAA, &(cal_data->c5));
    ms5611_read_u16(0xAC, &(cal_data->c6));
}

void ms5611_read_u16(uint8_t adr, uint16_t* c)
{
    uint8_t rx[2];
    spiSelect(&MS5611_SPID);
    spiSend(&MS5611_SPID, 1, (void*)&adr);
    spiReceive(&MS5611_SPID, 2, (void*)rx);
    spiUnselect(&MS5611_SPID);
    chThdSleepMilliseconds(1);

    *c = rx[0] << 8 | rx[1];
}

void ms5611_read_s24(uint8_t adr, int32_t* d)
{
    uint8_t adc_adr = 0x00, rx[3];

    spiSelect(&MS5611_SPID);
    spiSend(&MS5611_SPID, 1, (void*)&adr);
    chThdSleepMilliseconds(30);
    spiUnselect(&MS5611_SPID);

    chThdSleepMilliseconds(1);
    spiSelect(&MS5611_SPID);
    spiSend(&MS5611_SPID, 1, (void*)&adc_adr);
    spiReceive(&MS5611_SPID, 3, (void*)rx);
    spiUnselect(&MS5611_SPID);
    chThdSleepMilliseconds(1);

    *d = rx[0] << 16 | rx[1] << 8 | rx[2];
}

msg_t ms5611_thread(void *arg)
{
    (void)arg;

    static const SPIConfig spi_cfg = {
        NULL,
        MS5611_SPI_CS_PORT,
        MS5611_SPI_CS_PIN,
        SPI_CR1_BR_0 | SPI_CR1_BR_1 | SPI_CR1_BR_2 | SPI_CR1_CPOL | SPI_CR1_CPHA
    };

    static MS5611CalData cal_data;

    static int32_t temperature, pressure;

    spiStart(&MS5611_SPID, &spi_cfg);

    ms5611_init(&cal_data);

    while (TRUE) {
        ms5611_read(&cal_data, &temperature, &pressure);
        chThdSleepMilliseconds(1000);
    }

    return (msg_t)NULL;
}
