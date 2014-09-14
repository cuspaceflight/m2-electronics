#ifndef TEST_CH_H
#define TEST_CH_H

#include <stdint.h>

#define TRUE 1
#define FALSE !TRUE

typedef char BinarySemaphore;
typedef int msg_t;

void chBSemWait(BinarySemaphore* bsp);
void chBSemInit(BinarySemaphore* bsp, int initial);
void chBSemSignal(BinarySemaphore* bsp);
void chSetTimeNow(int32_t t);
int32_t chTimeNow(void);
void chThdSleepMilliseconds(int ms);
void chRegSetThreadName(const char* name);

#endif /* TEST_CH_H */
