/*
 * Time utilities
 * M2FC
 * 2014 Adam Greig, Cambridge University Spaceflight
 */

#include "time_utils.h"

uint32_t time_ticks_since(uint32_t *t0)
{
    uint32_t t1 = halGetCounterValue();
    if(t1 < t0) {
        t0 = t1;
        return 0xffffffff - t0 + t1;
    } else {
        t0 = t1;
        return t1 - t0;
    }
}

float time_seconds_since(uint32_t *t0)
{
    return (float)time_ticks_since(t0) / 168000000.0f;
}
