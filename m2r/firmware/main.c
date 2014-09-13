/*
 * M2FC
 * 2014 Adam Greig, Cambridge University Spaceflight
 */

#include <stdio.h>
#include <string.h>

#include "ch.h"
#include "hal.h"

static WORKING_AREA(waThreadHB, 128);

static msg_t ThreadHeartbeat(void *arg) {
    (void)arg;
    chRegSetThreadName("heartbeat");

    while (TRUE) {
        palSetPad(GPIOB, GPIOB_LED_STATUS);
        palSetPad(GPIOB, GPIOB_NTX2B_EN);
        IWDG->KR = 0xAAAA;
        chThdSleepMilliseconds(500);

        palClearPad(GPIOB, GPIOB_LED_STATUS);
        palSetPad(GPIOB, GPIOB_NTX2B_EN);
        IWDG->KR = 0xAAAA;
        chThdSleepMilliseconds(500);
    }

    return (msg_t)NULL;
}

int main(void) {
    halInit();
    chSysInit();
    chRegSetThreadName("main");

    chThdCreateStatic(waThreadHB, sizeof(waThreadHB), NORMALPRIO,
                      ThreadHeartbeat, NULL);

    /* Configure and enable the watchdog timer */
    IWDG->KR = 0x5555;
    IWDG->PR = 3;
    IWDG->KR = 0xCCCC;

    chThdSetPriority(LOWPRIO);
    chThdSleep(TIME_INFINITE);

    return 0;
}
