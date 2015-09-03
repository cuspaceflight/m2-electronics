/*
 * L3G4200D Driver 
 * Cambridge University Spaceflight
 */

#ifndef L3G4200D_H
#define L3G4200D_H

#include "ch.h"
#include "hal.h"

/* The main thread*/
msg_t l3g4200d_thread(void *arg);

extern int16_t global_gyro[3];

/* Interrupt callbacks for EXTI. */
void l3g4200d_wakeup(EXTDriver *extp, expchannel_t channel);

#endif /* L3G4200D_H */
