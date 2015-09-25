/*
 * M2R
 * 2014 Adam Greig, Cambridge University Spaceflight
 */

#include <stdio.h>
#include <string.h>

#include "ch.h"
#include "hal.h"

#include "sbp_io.h"
#include "ublox.h"
#include "radio.h"
#include "dispatch.h"
#include "rockblock.h"
#include "m2r_shell.h"
#include "m2serial.h"
#include "m2status.h"

void initialise_monitor_handles(void);

static WORKING_AREA(waThreadHB, 128);
static WORKING_AREA(waThreadUblox, 4096);
static WORKING_AREA(waThreadRadio, 2048);
static WORKING_AREA(waM2Serial, 1024);
static WORKING_AREA(waM2Status, 1024);
/*static WORKING_AREA(waThreadSBP, 2048);*/

static msg_t ThreadHeartbeat(void *arg) {
    (void)arg;
    chRegSetThreadName("heartbeat");

    while (TRUE) {
        palSetPad(GPIOB, GPIOB_LED_STATUS);
        IWDG->KR = 0xAAAA;
        chThdSleepMilliseconds(50);

        palClearPad(GPIOB, GPIOB_LED_STATUS);
        IWDG->KR = 0xAAAA;
        chThdSleepMilliseconds(500);
        IWDG->KR = 0xAAAA;
        chThdSleepMilliseconds(400);
    }

    return (msg_t)NULL;
}

int main(void) {
    initialise_monitor_handles();
    halInit();
    chSysInit();
    chRegSetThreadName("main");

    /*sbp_state_init(&sbp_state);*/
    rockblock_init();
    dispatch_init();

    chThdCreateStatic(waThreadHB, sizeof(waThreadHB), LOWPRIO,
                      ThreadHeartbeat, NULL);

    /* Configure and enable the watchdog timer, stopped in debug halt */
    DBGMCU->APB1FZ |= DBGMCU_APB1_FZ_DBG_IWDG_STOP;
    IWDG->KR = 0x5555;
    IWDG->PR = 3;
    IWDG->KR = 0xCCCC;

    m2serial_shell = m2r_shell_run;
    chThdCreateStatic(waM2Serial, sizeof(waM2Serial), HIGHPRIO,
                      m2serial_thread, NULL);

    LocalStatus = &M2RStatus;
    chThdCreateStatic(waM2Status, sizeof(waM2Status), HIGHPRIO,
                      m2status_thread, NULL);

    chThdCreateStatic(waThreadUblox, sizeof(waThreadUblox), NORMALPRIO,
                      ublox_thread, NULL);

    chThdCreateStatic(waThreadRadio, sizeof(waThreadRadio), NORMALPRIO,
                      radio_thread, NULL);

    chThdSetPriority(LOWPRIO);
    chThdSleep(TIME_INFINITE);

    return 0;
}
