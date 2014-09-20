/*
 * Pyrotechnic channel driver
 * M2FC
 * 2014 Adam Greig, Cambridge University Spaceflight
 */

#include "pyro.h"
#include "config.h"
#include "microsd.h"
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
        return FALSE;
    }

    if(palReadPad(GPIOE, pad) == PAL_LOW) {
        return TRUE;
    } else {
        return FALSE;
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

    if(m2fc_location == M2FC_BODY) {
        if(PYRO_DROGUE_BODY_1 || PYRO_MAIN_BODY_1)
            ok &= p1;
        if(PYRO_DROGUE_BODY_2 || PYRO_MAIN_BODY_2)
            ok &= p2;
        if(PYRO_DROGUE_BODY_3 || PYRO_MAIN_BODY_3)
            ok &= p3;
    } else if(m2fc_location == M2FC_NOSE) {
        if(PYRO_DROGUE_NOSE_1 || PYRO_MAIN_NOSE_1)
            ok &= p1;
        if(PYRO_DROGUE_NOSE_2 || PYRO_MAIN_NOSE_2)
            ok &= p2;
        if(PYRO_DROGUE_NOSE_3 || PYRO_MAIN_NOSE_3)
            ok &= p3;
    }

    microsd_log_s16(CHAN_PYRO_C, p1, p2, p3, 0);

    return ok;
}

/*
 * Fire the pyro channels ch1/ch2/ch3 for duration_ms milliseconds.
 */
void pyro_fire(uint8_t ch1, uint8_t ch2, uint8_t ch3,
               uint16_t duration_ms)
{
    if(ch1)
        palSetPad(GPIOE, GPIOE_PYRO_1_F);
    if(ch2)
        palSetPad(GPIOE, GPIOE_PYRO_2_F);
    if(ch3)
        palSetPad(GPIOE, GPIOE_PYRO_3_F);

    microsd_log_s16(CHAN_PYRO_F, ch1, ch2, ch3, 0);
    chThdSleepMilliseconds(duration_ms);

    if(ch1)
        palClearPad(GPIOE, GPIOE_PYRO_1_F);
    if(ch2)
        palClearPad(GPIOE, GPIOE_PYRO_2_F);
    if(ch3)
        palClearPad(GPIOE, GPIOE_PYRO_3_F);

    chThdSleepMilliseconds(duration_ms);
}


/* Fire all the right drogue deployment pyros */
void pyro_fire_drogue()
{
    int i;
    for(i=0; i<50; i++) {
        if(m2fc_location == M2FC_BODY) {
            pyro_fire(PYRO_DROGUE_BODY_1,
                      PYRO_DROGUE_BODY_2,
                      PYRO_DROGUE_BODY_3, 9);
        } else if(m2fc_location == M2FC_NOSE) {
            pyro_fire(PYRO_DROGUE_NOSE_1,
                      PYRO_DROGUE_NOSE_2,
                      PYRO_DROGUE_NOSE_3, 9);
        }
    }
}

/* Fire all the right main chute deployment pyros */
void pyro_fire_main()
{
    int i;
    for(i=0; i<50; i++) {
        if(m2fc_location == M2FC_BODY) {
            pyro_fire(PYRO_MAIN_BODY_1,
                      PYRO_MAIN_BODY_2,
                      PYRO_MAIN_BODY_3, 9);
        } else if(m2fc_location == M2FC_NOSE) {
            pyro_fire(PYRO_MAIN_NOSE_1,
                      PYRO_MAIN_NOSE_2,
                      PYRO_MAIN_NOSE_3, 9);
        }
    }
}

msg_t pyro_continuity_thread(void *arg) {
    (void)arg;
    chRegSetThreadName("Pyros");

    while(TRUE) {
        if(pyro_continuities()) {
            palSetPad(GPIOA, GPIOA_LED_PYROS);
            chThdSleepMilliseconds(10);
            palClearPad(GPIOA, GPIOA_LED_PYROS);
            chThdSleepMilliseconds(990);
        } else {
            /* TODO: report sadness up the chain */
            palSetPad(GPIOA, GPIOA_LED_PYROS);
            chThdSleepMilliseconds(100);
            palClearPad(GPIOA, GPIOA_LED_PYROS);
            chThdSleepMilliseconds(100);
        }
    }
}
