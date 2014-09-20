/*
 * M2FC
 * 2014 Adam Greig, Cambridge University Spaceflight
 */

#include <stdio.h>
#include <string.h>

#include "ch.h"
#include "hal.h"

#include "sbp_io.h"
#include "ublox.h"
#include "radio.h"

static WORKING_AREA(waThreadHB, 128);
static WORKING_AREA(waThreadUblox, 2048);
static WORKING_AREA(waThreadRadio, 1024);
static WORKING_AREA(waThreadSBP, 1024);

static msg_t ThreadHeartbeat(void *arg) {
    (void)arg;
    chRegSetThreadName("heartbeat");

    while (TRUE) {
        palSetPad(GPIOB, GPIOB_LED_STATUS);
        //palSetPad(GPIOB, GPIOB_MTX2_EN);
        IWDG->KR = 0xAAAA;
        chThdSleepMilliseconds(500);

        palClearPad(GPIOB, GPIOB_LED_STATUS);
        //palClearPad(GPIOB, GPIOB_MTX2_EN);
        IWDG->KR = 0xAAAA;
        chThdSleepMilliseconds(500);
    }

    return (msg_t)NULL;
}

int main(void) {
    halInit();
    chSysInit();
    chRegSetThreadName("main");
    sdInit();

    rockblock_init();
    dispatch_init();

    rockblock_init();

    chThdCreateStatic(waThreadHB, sizeof(waThreadHB), LOWPRIO,
                      ThreadHeartbeat, NULL);

    chThdCreateStatic(waThreadUblox, sizeof(waThreadUblox), NORMALPRIO,
                      ublox_thread, NULL);

    chThdCreateStatic(waThreadRadio, sizeof(waThreadRadio), NORMALPRIO,
                      radio_thread, NULL);

    chThdCreateStatic(waThreadSBP, sizeof(waThreadSBP), NORMALPRIO,
                      sbp_thread, NULL);

    /* Configure and enable the watchdog timer */
    /*
    IWDG->KR = 0x5555;
    IWDG->PR = 3;
    IWDG->KR = 0xCCCC;
    */

    chThdSetPriority(LOWPRIO);
    chThdSleep(TIME_INFINITE);

    return 0;
}
