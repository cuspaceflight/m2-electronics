/*
 * HMC5883L Driver 
 * Cambridge University Spaceflight
 */

#ifndef HMC5883L_H
#define HMC5883L_H

#include "ch.h"
#include "hal.h"

/* The main thread*/
msg_t hmc5883l_thread(void *arg) ;

/* Interrupt callbacks for EXTI. */
void hmc5883l_wakeup(EXTDriver *extp, expchannel_t channel);

#endif /* HMC5883L_H */
