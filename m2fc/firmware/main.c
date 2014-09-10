/*
 * M2FC
 * 2014 Adam Greig, Cambridge University Spaceflight
 */

#include <stdio.h>
#include <string.h>

#include <ch.h>
#include <hal.h>

#include "ff.h"

#include "ms5611.h"
#include "adxl3x5.h"
#include "pyro.h"
#include "microsd.h"
#include "m2fc_shell.h"
#include "mission.h"
#include "state_estimation.h"


static WORKING_AREA(waMS5611, 512);
static WORKING_AREA(waADXL345, 512);
static WORKING_AREA(waADXL375, 512);
static WORKING_AREA(waMission, 1024);
static WORKING_AREA(waThreadHB, 128);
static WORKING_AREA(waMicroSD, 512);

static msg_t ThreadHeartbeat(void *arg) {
    (void)arg;
    chRegSetThreadName("heartbeat");

    while (TRUE) {
        palSetPad(GPIOA, GPIOA_LED_STATUS);
        palSetPad(GPIOC, GPIOC_LED_C);
        palClearPad(GPIOC, GPIOC_LED_A);
        IWDG->KR = 0xAAAA;
        chThdSleepMilliseconds(500);

        palClearPad(GPIOA, GPIOA_LED_STATUS);
        palClearPad(GPIOC, GPIOC_LED_C);
        palClearPad(GPIOC, GPIOC_LED_A);
        IWDG->KR = 0xAAAA;
        chThdSleepMilliseconds(500);
    }

    return (msg_t)NULL;
}

int main(void) {
    halInit();
    chSysInit();
    chRegSetThreadName("main");

    state_estimation_init();

    chThdCreateStatic(waMicroSD, sizeof(waMicroSD), HIGHPRIO,
                      microsd_thread, NULL);

    chThdCreateStatic(waMission, sizeof(waMission), NORMALPRIO,
                      mission_thread, NULL);

    chThdCreateStatic(waMS5611, sizeof(waMS5611), NORMALPRIO,
                      ms5611_thread, NULL);

    chThdCreateStatic(waADXL345, sizeof(waADXL345), NORMALPRIO,
                      adxl345_thread, NULL);

    chThdCreateStatic(waADXL375, sizeof(waADXL375), NORMALPRIO,
                      adxl375_thread, NULL);

    chThdCreateStatic(waThreadHB, sizeof(waThreadHB), NORMALPRIO,
                      ThreadHeartbeat, NULL);

    /* Configure and enable the watchdog timer */
    IWDG->KR = 0x5555;
    IWDG->PR = 3;
    IWDG->KR = 0xCCCC;

    m2fc_shell_run();

    chThdSetPriority(LOWPRIO);
    chThdSleep(TIME_INFINITE);

    return 0;
}
