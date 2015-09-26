/*
 * Pyrotechnic channel driver
 * M2FC
 * 2014 Adam Greig, Cambridge University Spaceflight
 */

#include "pyro.h"
#include "config.h"
#include "datalogging.h"
#include "m2status.h"
#include <hal.h>

void pyro_off_1(void* arg);
void pyro_off_2(void* arg);
void pyro_off_3(void* arg);

/* Check the pyro channel `channel` for continuity, returns TRUE or FALSE. */
bool_t pyro_continuity(pyro_channel channel)
{
    uint8_t pad;
    if(channel == PYRO_1) {
        pad = GPIOE_PYRO_1_C;
    } else if(channel == PYRO_2) {
        pad = GPIOE_PYRO_2_C;
    } else if(channel == PYRO_3) {
        pad = GPIOE_PYRO_3_C;
    } else {
        return false;
    }

    if(palReadPad(GPIOE, pad) == PAL_LOW) {
        return true;
    } else {
        return false;
    }
}

/* Based on config options, check continuity of all pyros that should be
 * installed.
 */
bool_t pyro_continuities()
{
    bool_t ok = TRUE;
    int16_t p1, p2, p3;
    
    p1 = (int16_t)pyro_continuity(PYRO_1);
    p2 = (int16_t)pyro_continuity(PYRO_2);
    p3 = (int16_t)pyro_continuity(PYRO_3);

    if(conf.pyro_1) ok &= p1;
    if(conf.pyro_2) ok &= p2;
    if(conf.pyro_3) ok &= p3;

    log_i16(M2T_CH_PYRO_CONT, p1, p2, p3, 0);
    m2status_set_pyro_c(p1, p2, p3);

    return ok;
}

/*
 * Fire the pyro channels ch1/ch2/ch3 with 10ms on/off pulses for the
 * configured total duration.
 */
void pyro_fire(bool ch1, bool ch2, bool ch3)
{
    unsigned int i;
    m2status_set_pyro_f(ch1, ch2, ch3);
    for(i=0; i<conf.pyro_firetime / 20; i++) {
        if(ch1)
            palSetPad(GPIOE, GPIOE_PYRO_1_F);
        if(ch2)
            palSetPad(GPIOE, GPIOE_PYRO_2_F);
        if(ch3)
            palSetPad(GPIOE, GPIOE_PYRO_3_F);

        log_i16(M2T_CH_PYRO_FIRE, ch1, ch2, ch3, 0);
        chThdSleepMilliseconds(10);

        if(ch1)
            palClearPad(GPIOE, GPIOE_PYRO_1_F);
        if(ch2)
            palClearPad(GPIOE, GPIOE_PYRO_2_F);
        if(ch3)
            palClearPad(GPIOE, GPIOE_PYRO_3_F);

        chThdSleepMilliseconds(10);
    }
}


/* Fire all the right drogue deployment pyros */
void pyro_fire_drogue()
{
    pyro_fire(conf.pyro_1 == CFG_PYRO_DROGUE,
              conf.pyro_2 == CFG_PYRO_DROGUE,
              conf.pyro_3 == CFG_PYRO_DROGUE);
}

/* Fire all the right main chute deployment pyros */
void pyro_fire_main()
{
    pyro_fire(conf.pyro_1 == CFG_PYRO_MAIN,
              conf.pyro_2 == CFG_PYRO_MAIN,
              conf.pyro_3 == CFG_PYRO_MAIN);
}

msg_t pyro_continuity_thread(void *arg) {
    (void)arg;
    m2status_pyro_status(STATUS_WAIT);
    chRegSetThreadName("Pyros");

    while(TRUE) {
        if(pyro_continuities()) {
            m2status_pyro_status(STATUS_OK);
            palSetPad(GPIOA, GPIOA_LED_PYROS);
            chThdSleepMilliseconds(10);
            palClearPad(GPIOA, GPIOA_LED_PYROS);
            chThdSleepMilliseconds(990);
        } else {
            /* TODO: report sadness up the chain */
            m2status_pyro_status(STATUS_ERR);
            palSetPad(GPIOA, GPIOA_LED_PYROS);
            chThdSleepMilliseconds(100);
            palClearPad(GPIOA, GPIOA_LED_PYROS);
            chThdSleepMilliseconds(100);
        }
    }
}
