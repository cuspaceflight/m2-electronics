/*
 * M2FC
 * 2014 Adam Greig, Cambridge University Spaceflight
 */

#include <ch.h>
#include <hal.h>

#include "ms5611.h"

static WORKING_AREA(waMS5611, 128);
static WORKING_AREA(waThreadHB, 128);
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

    chThdCreateStatic(waThreadHB, sizeof(waThreadHB), NORMALPRIO,
                      ThreadHeartbeat, NULL);

    chThdCreateStatic(waMS5611, sizeof(waMS5611), NORMALPRIO,
                      ms5611_thread, NULL);

    while (TRUE) {
        chThdSleepMilliseconds(500);
    }
}
