#include "time_utils.h"
#include <stdio.h>

volatile float seconds_to_return = 0.0f;

float time_seconds_since(uint32_t* t0)
{
    (void)t0;
    return seconds_to_return;
}
