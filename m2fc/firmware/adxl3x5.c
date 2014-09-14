/*
 * ADXL3x5 Driver (ADXL345, ADXL375)
 * M2FC
 * 2014 Adam Greig, Cambridge University Spaceflight
 */

#include <stdlib.h>
#include "adxl3x5.h"
#include "microsd.h"
#include "config.h"
#include "state_estimation.h"

#include "chprintf.h"

#define ADXL345_SPID         SPID2
#define ADXL345_SPI_CS_PORT  GPIOB
#define ADXL345_SPI_CS_PIN   GPIOB_LG_ACCEL_CS

#define ADXL375_SPID         SPID1
#define ADXL375_SPI_CS_PORT  GPIOA
#define ADXL375_SPI_CS_PIN   GPIOA_HG_ACCEL_CS

static uint8_t adxl3x5_read_u8(SPIDriver* SPID, uint8_t adr);
static void adxl3x5_write_u8(SPIDriver* SPID, uint8_t adr, uint8_t val);
static void adxl3x5_read_accel(SPIDriver* SPID, int16_t* accels);
static void adxl3x5_init(SPIDriver* SPID, uint8_t x, int16_t *axis, int16_t *g);
static void adxl3x5_sad(const uint8_t n);
static float adxl3x5_accels_to_axis(int16_t *accels, int16_t axis, int16_t g);

static Thread *tp345 = NULL;
static Thread *tp375 = NULL;

/* Helper for sad moments */
static void adxl3x5_sad(const uint8_t n)
{
    /* TODO: report sadness up the chain */
    uint8_t i;
    while(1) {
        for(i=0; i<n; i++) {
            palSetPad(GPIOA, GPIOA_LED_SENSORS);
            chThdSleepMilliseconds(100);
            palClearPad(GPIOA, GPIOA_LED_SENSORS);
            chThdSleepMilliseconds(200);
        }
        chThdSleepMilliseconds(800);
    }
}

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
 * Sets registers for 800Hz operation in high power mode,
 * enables measurement, and runs a self test to verify device performance.
 */
static void adxl3x5_init(SPIDriver* SPID, uint8_t x, int16_t *axis, int16_t *g)
{
    uint8_t devid;
    uint16_t i, j;
    int32_t accels_sum[3], accels_delta[3];
    int16_t accels_cur[3], accels_test_avg[3], accels_notest_avg[3];
    const uint16_t n_discard_samples = 30;
    const uint16_t n_test_samples = 100;

    devid = adxl3x5_read_u8(SPID, 0x00);
    if(devid != 0xE5) {
        adxl3x5_sad(2);
    }

    /* BW_RATE: Set high power mode and 800Hz ODR */
    adxl3x5_write_u8(SPID, 0x2C, 0x0D);

    /* DATA_FORMAT: Full resolution, maximum range */
    adxl3x5_write_u8(SPID, 0x31, (1<<3) | (1<<1) | (1<<0));

    /* POWER_CTL: Enter MEASURE mode */
    adxl3x5_write_u8(SPID, 0x2D, (1<<3));

    /* Read current accelerations */
    /* First discard some samples to allow settling to new settings */
    for(i=0; i<n_discard_samples; i++) {
        adxl3x5_read_accel(SPID, accels_cur);
        chThdSleepMilliseconds(1);
    }
    /* Zero the sums */
    for(j=0; j<3; j++)
        accels_sum[j] = 0;
    /* Now read and sum 0.1s worth of samples */
    for(i=0; i<n_test_samples; i++) {
        adxl3x5_read_accel(SPID, accels_cur);
        for(j=0; j<3; j++)
            accels_sum[j] += accels_cur[j];
        chThdSleepMilliseconds(1);
    }
    /* Average the summed values */
    for(j=0; j<3; j++)
        accels_notest_avg[j] = (int16_t)(accels_sum[j] / n_test_samples);

    /* DATA_FORMAT: Self test, full resolution, maximum range */
    adxl3x5_write_u8(SPID, 0x31, (1<<7) | (1<<3) | (1<<1) | (1<<0));

    /* Read current accelerations, should have self-test values */
    /* First discard some samples to allow settling to new settings */
    for(i=0; i<n_discard_samples; i++) {
        adxl3x5_read_accel(SPID, accels_cur);
        chThdSleepMilliseconds(1);
    }
    /* Zero the sums */
    for(j=0; j<3; j++)
        accels_sum[j] = 0;
    /* Now read and sum 0.1s worth of samples */
    for(i=0; i<n_test_samples; i++) {
        adxl3x5_read_accel(SPID, accels_cur);
        for(j=0; j<3; j++)
            accels_sum[j] += accels_cur[j];
        chThdSleepMilliseconds(1);
    }
    /* Average the summed values */
    for(j=0; j<3; j++)
        accels_test_avg[j] = (int16_t)(accels_sum[j] / n_test_samples);

    /* Compute the delta between self-test and no-self-test averages */
    for(j=0; j<3; j++)
        accels_delta[j] = accels_test_avg[j] - accels_notest_avg[j];

    if(x == 4) {
        /* ADXL345 self test parameters at 3.3V operation:
         * X min 0.354g, Y min -0.354g, Z min 0.441g
         * At full resolution and +-16g, corresponds to:
         * X min 93LSB, Y min -93LSB, Z min 112LSB
         * (Using 265LSB/g for X and Y and 256LSB/g for Z)
         */
        if(accels_delta[0] < 93  ||
           accels_delta[1] > -93 ||
           accels_delta[2] < 112) {
            adxl3x5_sad(3);
        }
    } else if(x == 7) {
        /* ADXL375 self test parameters:
         * Z typ 6.4g
         * Sensitivity is typically 20.5LSB/g
         * So typical change is 131LSB
         * Let's be OK with anything above 100LSB.
         */
        if(accels_delta[2] < 100) {
            adxl3x5_sad(4);
        }
    }

    /* The thrust axis comes from the config file now,
     * because 1g on the high-g accel was not very
     * distinguished from the other axes.
     */
    *axis = ACCEL_THRUST_AXIS;
    *g = accels_notest_avg[ACCEL_THRUST_AXIS];

    /* DATA_FORMAT: Full resolution, maximum range (no self test) */
    adxl3x5_write_u8(SPID, 0x31, (1<<3) | (1<<1) | (1<<0));

    /* BW_RATE: Set high power mode and 3200Hz ODR */
    /* DISABLED for now, seems to cause trouble */
    /*adxl3x5_write_u8(SPID, 0x2C, 0x0F);*/

    /* INT_ENABLE: Enable DATA READY interrupt on INT1 */
    adxl3x5_write_u8(SPID, 0x2E, (1<<7));

    /* Discard some samples to allow it to settle after turning off test */
    for(i=0; i<n_discard_samples; i++) {
        adxl3x5_read_accel(SPID, accels_cur);
        chThdSleepMilliseconds(1);
    }
}

/* ISR triggered by the EXTI peripheral when DRDY gets asserted on one
 * of the accelerometers.
 */
void adxl345_wakeup(EXTDriver *extp, expchannel_t channel)
{
    (void)extp;
    (void)channel;

    chSysLockFromIsr();
    if(tp345 != NULL) {
        chSchReadyI(tp345);
        tp345 = NULL;
    }
    chSysUnlockFromIsr();
}

void adxl375_wakeup(EXTDriver *extp, expchannel_t channel)
{
    (void)extp;
    (void)channel;

    chSysLockFromIsr();
    if(tp375 != NULL) {
        chSchReadyI(tp375);
        tp375 = NULL;
    }
    chSysUnlockFromIsr();
}

/* Helper to convert from the three-axis accelerometer readings to a single
 * float in the 'up' direction, compensating for gravity vector and intitial
 * orientation (so long as it is axis-aligned).
 */
static float adxl3x5_accels_to_axis(int16_t *accels, int16_t axis, int16_t g)
{
    float v = (float)(accels[axis] - g);
    return (v / (float)g) * 9.80665f;
}

/*
 * ADXL345 (low-g accelerometer) main thread.
 */
msg_t adxl345_thread(void *arg)
{
    (void)arg;

    const SPIConfig spi_cfg = {
        NULL,
        ADXL345_SPI_CS_PORT,
        ADXL345_SPI_CS_PIN,
        SPI_CR1_BR_1 | SPI_CR1_BR_0 | SPI_CR1_CPOL | SPI_CR1_CPHA
    };
    int16_t accels[3], axis, g;


    chRegSetThreadName("ADXL345");

    spiStart(&ADXL345_SPID, &spi_cfg);
    adxl3x5_init(&ADXL345_SPID, 4, &axis, &g);
    microsd_log_s16(CHAN_CAL_LGA, axis, g, 0, 0);

    while(TRUE) {
        adxl3x5_read_accel(&ADXL345_SPID, accels);
        microsd_log_s16(CHAN_IMU_LGA, accels[0], accels[1], accels[2], 0);
        state_estimation_new_lg_accel(
            adxl3x5_accels_to_axis(accels, axis, g));

        /* Sleep until DRDY */
        chSysLock();
        tp345 = chThdSelf();
        chSchGoSleepTimeoutS(THD_STATE_SUSPENDED, 100);
        chSysUnlock();
    }

    return (msg_t)NULL;
}

/*
 * ADXL375 (high-g accelerometer) main thread.
 */
msg_t adxl375_thread(void *arg)
{
    (void)arg;

    const SPIConfig spi_cfg = {
        NULL,
        ADXL375_SPI_CS_PORT,
        ADXL375_SPI_CS_PIN,
        SPI_CR1_BR_2 | SPI_CR1_CPOL | SPI_CR1_CPHA
    };
    int16_t accels[3], axis, g;

    chRegSetThreadName("ADXL375");

    spiStart(&ADXL375_SPID, &spi_cfg);
    adxl3x5_init(&ADXL375_SPID, 7, &axis, &g);
    microsd_log_s16(CHAN_CAL_HGA, axis, g, 0, 0);

    while(TRUE) {
        adxl3x5_read_accel(&ADXL375_SPID, accels);
        microsd_log_s16(CHAN_IMU_HGA, accels[0], accels[1], accels[2], 0);
        state_estimation_new_hg_accel(
            adxl3x5_accels_to_axis(accels, axis, g));

        /* Sleep until DRDY */
        chSysLock();
        tp375 = chThdSelf();
        chSchGoSleepTimeoutS(THD_STATE_SUSPENDED, 100);
        chSysUnlock();
    }

    return (msg_t)NULL;
}
