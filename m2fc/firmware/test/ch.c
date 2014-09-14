#include "ch.h"

static int32_t ch_time_now = 0;

void chBSemWait(BinarySemaphore* bsp)
{
    (void)bsp;
    return;
}

void chBSemInit(BinarySemaphore* bsp, int initial)
{
    (void)bsp;
    (void)initial;
    return;
}

void chBSemSignal(BinarySemaphore* bsp)
{
    (void)bsp;
    return;
}

int32_t chTimeNow() {
    return ch_time_now;
}

void chSetTimeNow(int32_t t) {
    ch_time_now = t;
}

void chThdSleepMilliseconds(int ms)
{
    (void)ms;
}

void chRegSetThreadName(const char* name)
{
    (void)name;
}
