/*
 * ADXL3x5 Driver (ADXL345, ADXL375)
 * M2FC
 * 2014 Adam Greig, Cambridge University Spaceflight
 */

#ifndef ADXL3X5_H
#define ADXL3X5_H

#include "ch.h"
#include "hal.h"

/* The main threads. Run these. */
msg_t adxl345_thread(void *arg);
msg_t adxl375_thread(void *arg);

/* Interrupt callbacks for EXTI. */
void adxl345_wakeup(EXTDriver *extp, expchannel_t channel);
void adxl375_wakeup(EXTDriver *extp, expchannel_t channel);

#endif /* ADXL3X5_H */
