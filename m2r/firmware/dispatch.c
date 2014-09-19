/*
 * Grand Central Dispatch
 * M2R
 * 2014 Adam Greig, Cambridge University Spaceflight
 */

#include "dispatch.h"

#include "rockblock.h"

void dispatch_pvt(const ublox_pvt_t pvt)
{
  static systime_t last_sbd = 0;
  if (chTimeElapsedSince(last_sbd) > S2ST(30)) {
    /* Only send an Iridium SBD message every 30s */
    send_sbd_posn(&pvt);
  }
}

msg_t dispatch_thread(void* arg)
{
    (void)arg;
    while(TRUE)
        chThdSleepMilliseconds(1000);
}
