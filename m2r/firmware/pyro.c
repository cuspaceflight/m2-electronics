#include "pyro.h"
#include "hal.h"
#include "ch.h"
#include "m2status.h"

#define PYRO_FIRE_ALTITUDE_M (20000)

bool pyro_cont = false;
bool pyro_fired = false;

bool pyro_got_continuity(void) {
    return !palReadPad(GPIOA, GPIOA_PYRO_CONT);
}

void pyro_fire(void) {
    palSetPad(GPIOA, GPIOA_PYRO_FIRE);
    chThdSleepMilliseconds(3000);
    palClearPad(GPIOA, GPIOA_PYRO_FIRE);
}

msg_t pyro_thread(void* arg)
{
    (void)arg;

    chRegSetThreadName("Pyro");

    while(true) {
        palSetPad(GPIOB, GPIOB_LED_PYRO);
        if(pyro_got_continuity()) {
            pyro_cont = true;
            chThdSleepMilliseconds(100);
        } else {
            pyro_cont = false;
            chThdSleepMilliseconds(1000);
        }
        palClearPad(GPIOB, GPIOB_LED_PYRO);

        int altitude_m = M2RStatus.latest.gps_alt / 1000;
        bool gps_locked = M2RStatus.latest.gps_fix_type == 3;
        if(!pyro_fired && gps_locked && altitude_m > PYRO_FIRE_ALTITUDE_M) {
            pyro_fire();
            pyro_fired = true;
        }

        chThdSleepMilliseconds(500);
    }
}
