/*
 * Grand Central Dispatch
 * M2R
 * 2014 Adam Greig, Cambridge University Spaceflight
 */

#include "dispatch.h"

volatile m2r_state_t m2r_state;

void dispatch_pvt(const ublox_pvt_t pvt)
{
}

msg_t dispatch_thread(void* arg)
{
    (void)arg;
    while(TRUE)
        chThdSleepMilliseconds(1000);
}
