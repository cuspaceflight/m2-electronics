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

static WORKING_AREA(waThreadHB, 128);
static WORKING_AREA(waThreadUblox, 4096);
static WORKING_AREA(waThreadRadio, 2048);
static WORKING_AREA(waM2Serial, 1024);
static WORKING_AREA(waM2Status, 1024);
/*static WORKING_AREA(waThreadSBP, 2048);*/
static WORKING_AREA(waDispatch, 1024);

static msg_t ThreadHeartbeat(void *arg) {
    (void)arg;
    chRegSetThreadName("heartbeat");

    while (TRUE) {
        palSetPad(GPIOB, GPIOB_LED_STATUS);
        /*palSetPad(GPIOB, GPIOB_BUZZER);*/
        IWDG->KR = 0xAAAA;

        if(/*M2FCBodyStatus.m2fcbody == STATUS_OK &&*/
           M2FCNoseStatus.m2fcnose == STATUS_OK)
        {
            chThdSleepMilliseconds(50);
        } else {
            chThdSleepMilliseconds(500);
        }

        palClearPad(GPIOB, GPIOB_LED_STATUS);
        /*palClearPad(GPIOB, GPIOB_BUZZER);*/
        IWDG->KR = 0xAAAA;
        chThdSleepMilliseconds(500);
    }
    return (msg_t)NULL;
}

int main(void) {
    halInit();
    chSysInit();
    chRegSetThreadName("main");

    LocalStatus = &M2RStatus;

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
    M2SerialSD = &SD2;
    chThdCreateStatic(waM2Serial, sizeof(waM2Serial), HIGHPRIO,
                      m2serial_thread, NULL);

    chThdCreateStatic(waM2Status, sizeof(waM2Status), HIGHPRIO,
                      m2status_thread, NULL);

    chThdCreateStatic(waThreadUblox, sizeof(waThreadUblox), NORMALPRIO,
                      ublox_thread, NULL);

    chThdCreateStatic(waThreadRadio, sizeof(waThreadRadio), NORMALPRIO,
                      radio_thread, NULL);

    chThdCreateStatic(waDispatch, sizeof(waDispatch), NORMALPRIO,
                      dispatch_thread, NULL);

    chThdSetPriority(LOWPRIO);
    chThdSleep(TIME_INFINITE);

    return 0;
}
