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


static WORKING_AREA(waMS5611, 256);
static WORKING_AREA(waADXL345, 256);
static WORKING_AREA(waADXL375, 256);
static WORKING_AREA(waThreadHB, 128);
static WORKING_AREA(waMicroSD, 512);

static msg_t ThreadHeartbeat(void *arg) {
    (void)arg;
    chRegSetThreadName("heartbeat");
    while (TRUE) {
        palSetPad(GPIOA, GPIOA_LED_STATUS);
        chThdSleepMilliseconds(500);
        palClearPad(GPIOA, GPIOA_LED_STATUS);
        chThdSleepMilliseconds(500);
    }

    return (msg_t)NULL;
}

int main(void) {
    halInit();
    chSysInit();
    chRegSetThreadName("main");

    chThdCreateStatic(waMicroSD, sizeof(waMicroSD), HIGHPRIO,
                      microsd_thread, NULL);

    chThdCreateStatic(waMS5611, sizeof(waMS5611), NORMALPRIO,
                      ms5611_thread, NULL);

    chThdCreateStatic(waADXL345, sizeof(waADXL345), NORMALPRIO,
                      adxl345_thread, NULL);

    chThdCreateStatic(waADXL375, sizeof(waADXL375), NORMALPRIO,
                      adxl375_thread, NULL);

    chThdCreateStatic(waThreadHB, sizeof(waThreadHB), NORMALPRIO,
                      ThreadHeartbeat, NULL);

    m2fc_shell_run();

    chThdSetPriority(LOWPRIO);
    chThdSleep(TIME_INFINITE);

    return 0;
}
