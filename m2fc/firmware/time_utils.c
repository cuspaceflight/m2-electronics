/*
 * Time utilities
 * M2FC
 * 2014 Adam Greig, Cambridge University Spaceflight
 */

#include "time_utils.h"
#include "hal.h"

uint32_t time_ticks_since(uint32_t *t0)
{
    uint32_t t1 = halGetCounterValue();
    uint32_t tp = *t0;
    if(t1 < tp) {
        *t0 = t1;
        return t1 + ((0xffffffff - tp) + 1);
    } else {
        *t0 = t1;
        return t1 - tp;
    }
}

float time_seconds_since(uint32_t *t0)
{
    return (float)time_ticks_since(t0) / (float)halGetCounterFrequency();
}
