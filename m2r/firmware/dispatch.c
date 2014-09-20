/*
 * Grand Central Dispatch
 * M2R
 * 2014 Adam Greig, Cambridge University Spaceflight
 */

#include <math.h>
#include "ch.h"
#include "chprintf.h"

#include "dispatch.h"
#include "audio_data.h"

volatile m2r_state_t m2r_state;
#include "rockblock.h"

void dispatch_pvt(const ublox_pvt_t pvt)
{
  static systime_t last_sbd = 0;
  if (chTimeElapsedSince(last_sbd) > S2ST(30)) {
    /* Only send an Iridium SBD message every 30s */
    send_sbd_posn(&pvt);
    last_sbd = chTimeNow();
  }
}

#define LEN(x) (sizeof(x)/sizeof(x[0]))
uint8_t * const number[10] = {
  zero, one, two, three, four, five, six, seven, eight, nine
};
uint16_t const number_lens[10] = {
  LEN(zero), LEN(one), LEN(two), LEN(three), LEN(four),
  LEN(five), LEN(six), LEN(seven), LEN(eight), LEN(nine)
};

/* alt is in meters AMSL */
void say_altitude(float alt, uint8_t **samples, uint16_t *lens)
{
    int32_t kft_agl = lroundf((3.2808399f * alt - 4000.0f) / 1000.0f);

    if (kft_agl <= 0) {
        /* below 1000 ft, don't report altitude */
        samples[0] = 0;
        return;
    }

    char buff[4] = {0};
    chsnprintf(buff, 4, "%-3d", kft_agl);

    uint8_t i = 0;
    while (buff[i] && buff[i] != ' ') {
        samples[i] = number[buff[i] - '0'];
        lens[i] = number_lens[buff[i] - '0'];
        i++;
    }
    samples[i] = kft;
    lens[i++] = LEN(kft);
    samples[i] = 0;
    lens[i] = 0;
}

msg_t dispatch_thread(void* arg)
{
    (void)arg;
    while(TRUE)
        chThdSleepMilliseconds(1000);
}
