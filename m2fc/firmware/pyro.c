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

/* Fire the pyro channel `channel` for `duration_ms` milliseconds.
 * Non-blocking.
 */
static VirtualTimer vt1, vt2, vt3;
void pyro_fire(uint8_t channel, uint16_t duration_ms)
{
    uint8_t pad;

    if(channel == PYRO_1) {
        pad = GPIOE_PYRO_1_F;
        chVTReset(&vt1);
        chVTSet(&vt1, MS2ST(duration_ms), pyro_off_1, NULL);
        microsd_log_s16(CHAN_PYRO_F, 1, 0, 0, 0);
    } else if(channel == PYRO_2) {
        pad = GPIOE_PYRO_2_F;
        chVTReset(&vt2);
        chVTSet(&vt2, MS2ST(duration_ms), pyro_off_2, NULL);
        microsd_log_s16(CHAN_PYRO_F, 0, 1, 0, 0);
    } else if(channel == PYRO_3) {
        pad = GPIOE_PYRO_3_F;
        chVTReset(&vt3);
        chVTSet(&vt3, MS2ST(duration_ms), pyro_off_3, NULL);
        microsd_log_s16(CHAN_PYRO_F, 0, 0, 1, 0);
    } else {
        return;
    }

    palSetPad(GPIOE, pad);
}

/*
 * Turn off a pyro channel. Called by the virtual timer.
 */
void pyro_off_1(void* arg)
{
    (void)arg;
    palClearPad(GPIOE, GPIOE_PYRO_1_F);
}
void pyro_off_2(void* arg)
{
    (void)arg;
    palClearPad(GPIOE, GPIOE_PYRO_2_F);
}
void pyro_off_3(void* arg)
{
    (void)arg;
    palClearPad(GPIOE, GPIOE_PYRO_3_F);
}

/* Fire all the right drogue deployment pyros */
void pyro_fire_drogue()
{
    if(m2fc_location == M2FC_BODY) {
        if(PYRO_DROGUE_BODY_1)
            pyro_fire(PYRO_1, PYRO_FIRETIME);
        if(PYRO_DROGUE_BODY_2)
            pyro_fire(PYRO_2, PYRO_FIRETIME);
        if(PYRO_DROGUE_BODY_3)
            pyro_fire(PYRO_3, PYRO_FIRETIME);
    } else if(m2fc_location == M2FC_NOSE) {
        if(PYRO_DROGUE_NOSE_1)
            pyro_fire(PYRO_1, PYRO_FIRETIME);
        if(PYRO_DROGUE_NOSE_2)
            pyro_fire(PYRO_2, PYRO_FIRETIME);
        if(PYRO_DROGUE_NOSE_3)
            pyro_fire(PYRO_3, PYRO_FIRETIME);
    }
}

/* Fire all the right main chute deployment pyros */
void pyro_fire_main()
{
    if(m2fc_location == M2FC_BODY) {
        if(PYRO_MAIN_BODY_1)
            pyro_fire(PYRO_1, PYRO_FIRETIME);
        if(PYRO_MAIN_BODY_2)
            pyro_fire(PYRO_2, PYRO_FIRETIME);
        if(PYRO_MAIN_BODY_3)
            pyro_fire(PYRO_3, PYRO_FIRETIME);
    } else if(m2fc_location == M2FC_NOSE) {
        if(PYRO_MAIN_NOSE_1)
            pyro_fire(PYRO_1, PYRO_FIRETIME);
        if(PYRO_MAIN_NOSE_2)
            pyro_fire(PYRO_2, PYRO_FIRETIME);
        if(PYRO_MAIN_NOSE_3)
            pyro_fire(PYRO_3, PYRO_FIRETIME);
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
