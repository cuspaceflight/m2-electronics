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

void dispatch_pvt(const ublox_pvt_t pvt)
{
}

uint8_t * const number[10] = {
  zero, one, two, three, four, five, six, seven, eight, nine
};

/* alt is in meters AMSL */
void say_altitude(float alt, uint8_t **samples)
{
    uint32_t kft_agl = lroundf((3.2808399f * alt - 4000.0f) / 1000.0f);

    if (kft_agl == 0) {
        /* below 1000 ft, don't report altitude */
        return;
    }

    char buff[4] = {0};
    chsnprintf(buff, 4, "%-3d", kft_agl);

    uint8_t i = 0;
    while (buff[i] && buff[i] != ' ') {
        samples[i] = number[buff[i] - '0'];
        i++;
    }
    samples[i++] = kft;
    samples[i] = 0;
}

msg_t dispatch_thread(void* arg)
{
    (void)arg;
    while(TRUE)
        chThdSleepMilliseconds(1000);
}
