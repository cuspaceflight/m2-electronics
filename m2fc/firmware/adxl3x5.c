/*
 * ADXL3x5 Driver (ADXL345, ADXL375)
 * M2FC
 * 2014 Adam Greig, Cambridge University Spaceflight
 */

#include "adxl3x5.h"
#include <hal.h>

#define ADXL345_SPID         SPID2
#define ADXL345_SPI_CS_PORT  GPIOB
#define ADXL345_SPI_CS_PIN   GPIOB_LG_ACCEL_CS

#define ADXL375_SPID         SPID1
#define ADXL375_SPI_CS_PORT  GPIOA
#define ADXL375_SPI_CS_PIN   GPIOA_HG_ACCEL_CS

static uint8_t adxl3x5_read_u8(SPIDriver* SPID, uint8_t adr);
static void adxl3x5_write_u8(SPIDriver* SPID, uint8_t adr, uint8_t val);
static void adxl3x5_read_accel(SPIDriver* SPID, int16_t* accels);
static void adxl3x5_init(SPIDriver* SPID, uint8_t x);


/*
 * Read a register at address `adr` on the ADXL3x5 on SPI driver `SPID`.
 * The register's value is returned.
 */
static uint8_t adxl3x5_read_u8(SPIDriver* SPID, uint8_t adr)
{
    uint8_t val;
    adr |= (1<<7);
    adr &= ~(1<<6);
    spiSelect(SPID);
    spiSend(SPID, 1, (void*)&adr);
    spiReceive(SPID, 1, (void*)&val);
    spiUnselect(SPID);

    return val;
}

/*
 * Write a register at address `adr` with value `val` on the ADXL3x5 on SPI
 * driver `SPID`.
 */
static void adxl3x5_write_u8(SPIDriver* SPID, uint8_t adr, uint8_t val)
{
    uint8_t tx[2];
    tx[0] = adr & ~(1<<7 | 1<<6);
    tx[1] = val;

    spiSelect(SPID);
    spiSend(SPID, 2, (void*)tx);
    spiUnselect(SPID);
}

/*
 * Read the current acceleration values from the ADXL on SPI driver `SPID`.
 * The values are stored in `accels` as three int16s.
 */
static void adxl3x5_read_accel(SPIDriver* SPID, int16_t* accels)
{
    uint8_t adr = 0x32 | (1<<6) | (1<<7);

    spiSelect(SPID);
    spiSend(SPID, 1, (void*)&adr);
    spiReceive(SPID, 6, (void*)accels);
    spiUnselect(SPID);
}

/*
 * Initialise the ADXL3x5 device. `x` is a parameter, 4 or 7.
 * Sets registers for 3200Hz operation in high power mode,
 * enables measurement, and runs a self test to verify device performance.
 */
static void adxl3x5_init(SPIDriver* SPID, uint8_t x)
{
    uint8_t devid;
    uint16_t i, j;
    int16_t accels_cur[3], accels_delta[3];
    int16_t accels_test_avg[3], accels_notest_avg[3];

    devid = adxl3x5_read_u8(SPID, 0x00);
    if(devid != 0xE5) {
        /* TODO: be sad */
        while(1);
    }

    /* BW_RATE: Set high power mode and 3200Hz ODR */
    adxl3x5_write_u8(SPID, 0x2C, 0x0F);

    /* POWER_CTL: Enter MEASURE mode */
    adxl3x5_write_u8(SPID, 0x2D, (1<<3));

    /* DATA_FORMAT: Full resolution, maximum range */
    adxl3x5_write_u8(SPID, 0x31, (1<<3) | (1<<1) | (1<<0));

    /* Read current accelerations */
    /* First discard ten samples to allow settling to new settings */
    for(i=0; i<10; i++) {
        adxl3x5_read_accel(SPID, accels_cur);
    }
    /* Zero the no-test averages */
    for(j=0; j<3; j++) {
        accels_notest_avg[j] = 0;
    }
    /* Now read and online-update an average of 320 samples */
    for(i=0; i<320; i++) {
        adxl3x5_read_accel(SPID, accels_cur);
        for(j=0; j<3; j++) {
            accels_delta[j] = accels_cur[j] - accels_notest_avg[j];
            accels_notest_avg[j] += accels_delta[j] / (i+1);
        }
    }

    /* DATA_FORMAT: Self test, full resolution, maximum range */
    adxl3x5_write_u8(SPID, 0x31, (1<<7) | (1<<3) | (1<<1) | (1<<0));

    /* Read current accelerations, should have self-test values */
    /* First discard ten samples to allow settling to new settings */
    for(i=0; i<10; i++) {
        adxl3x5_read_accel(SPID, accels_cur);
    }
    /* Zero the with-test averages */
    for(j=0; j<3; j++) {
        accels_test_avg[j] = 0;
    }
    /* Now read and online-update an average of 320 samples */
    for(i=0; i<320; i++) {
        adxl3x5_read_accel(SPID, accels_cur);
        for(j=0; j<3; j++) {
            accels_delta[j] = accels_cur[j] - accels_test_avg[j];
            accels_test_avg[j] += accels_delta[j] / (i+1);
        }
    }

    /* Compute the delta between self-test and no-self-test averages */
    for(j=0; j<3; j++) {
        accels_delta[j] = accels_test_avg[j] - accels_notest_avg[j];
    }

    if(x == 4) {
        /* ADXL345 self test parameters at 3.3V operation:
         * X min 0.354g, Y min -0.354g, Z min 0.441g
         * At full resolution and +-16g, corresponds to:
         * X min 93LSB, Y min -93LSB, Z min 112LSB
         * (Using 265LSB/g for X and Y and 256LSB/g for Z)
         */
        if(accels_delta[0] < 93  ||
           accels_delta[1] < -93 ||
           accels_delta[2] < 112) {
            /* TODO: be sad */
            while(1);
        }
    } else if(x == 7) {
        /* ADXL375 self test parameters:
         * Z typ 6.4g
         * Sensitivity is typically 20.5LSB/g
         * So typical change is 131LSB
         * Let's be OK with anything above 100LSB.
         */
        if(accels_delta[2] < 100) {
            /* TODO: be sad */
            while(1);
        }
    }

    /* DATA_FORMAT: Full resolution, maximum range */
    adxl3x5_write_u8(SPID, 0x31, (1<<3) | (1<<1) | (1<<0));

}

/*
 * ADXL345 (low-g accelerometer) main thread.
 */
msg_t adxl345_thread(void *arg)
{
    (void)arg;

    static const SPIConfig spi_cfg = {
        NULL,
        ADXL345_SPI_CS_PORT,
        ADXL345_SPI_CS_PIN,
        SPI_CR1_BR_1 | SPI_CR1_BR_0 | SPI_CR1_CPOL | SPI_CR1_CPHA
    };
    static int16_t accels[3];

    spiStart(&ADXL345_SPID, &spi_cfg);
    adxl3x5_init(&ADXL345_SPID, 3);

    while(TRUE) {
        adxl3x5_read_accel(&ADXL345_SPID, accels);
    }

    return (msg_t)NULL;
}

/*
 * ADXL375 (high-g accelerometer) main thread.
 */
msg_t adxl375_thread(void *arg)
{
    (void)arg;

    static const SPIConfig spi_cfg = {
        NULL,
        ADXL375_SPI_CS_PORT,
        ADXL375_SPI_CS_PIN,
        SPI_CR1_BR_2 | SPI_CR1_CPOL | SPI_CR1_CPHA
    };
    static int16_t accels[3];

    spiStart(&ADXL375_SPID, &spi_cfg);
    adxl3x5_init(&ADXL375_SPID, 7);

    while(TRUE) {
        adxl3x5_read_accel(&ADXL375_SPID, accels);
    }

    return (msg_t)NULL;
}
