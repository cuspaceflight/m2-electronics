/*
 * Grand Central Dispatch
 * M2R
 * 2014 Adam Greig, Cambridge University Spaceflight
 */

#include "dispatch.h"

void dispatch_pvt(const ublox_pvt_t pvt)
{
    int32_t lat = pvt.lat;
    int32_t lng = pvt.lon;
}

msg_t dispatch_thread(void* arg)
{
    (void)arg;
    while(TRUE)
        chThdSleepMilliseconds(1000);
}
