#include "pyro.h"
#include "hal.h"
#include "ch.h"
#include "m2status.h"

#define PYRO_FIRE_ALTITUDE_M (20000)

bool pyro_cont = false;
bool pyro_fired = false;

bool pyro_got_continuity(void) {
    return palReadPad(GPIOA, GPIOA_PYRO_CONT);
}

void pyro_fire(void) {
    palSetPad(GPIOA, GPIOA_PYRO_FIRE);
    chThdSleepMilliseconds(4000);
    palClearPad(GPIOA, GPIOA_PYRO_FIRE);
    pyro_fired = true;
}

msg_t pyro_thread(void* arg)
{
    (void)arg;

    chRegSetThreadName("Pyro");

    while(true) {
        if(pyro_got_continuity()) {
            palSetPad(GPIOB, GPIOB_LED_PYRO);
            pyro_cont = true;
        } else {
            palClearPad(GPIOB, GPIOB_LED_PYRO);
            pyro_cont = false;
        }

        int altitude_m = M2RStatus.latest.gps_alt / 1000;
        bool gps_locked = M2RStatus.latest.gps_fix_type == 3;
        if(!pyro_fired && gps_locked && altitude_m > PYRO_FIRE_ALTITUDE_M) {
            pyro_fire();
        }

        chThdSleepMilliseconds(500);
    }
}
