/*
 * Pyrotechnic channel driver
 * M2FC
 * 2014 Adam Greig, Cambridge University Spaceflight
 */

#include "pyro.h"
#include "config.h"
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
    } else if(channel == PYRO_2) {
        pad = GPIOE_PYRO_2_F;
        chVTReset(&vt2);
        chVTSet(&vt2, MS2ST(duration_ms), pyro_off_2, NULL);
    } else if(channel == PYRO_3) {
        pad = GPIOE_PYRO_3_F;
        chVTReset(&vt3);
        chVTSet(&vt3, MS2ST(duration_ms), pyro_off_3, NULL);
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

void pyro_fire_drogue()
{
    /* TODO: Check this is the nosecone m2fc */
    pyro_fire(PYRO_1, PYRO_FIRETIME);
    pyro_fire(PYRO_2, PYRO_FIRETIME);
    pyro_fire(PYRO_3, PYRO_FIRETIME);
}

void pyro_fire_main()
{
    /* TODO: Check this is the body tube m2fc */
    pyro_fire(PYRO_1, PYRO_FIRETIME);
}
