/*
 * ADC Driver (ADC1, ADC2, ADC3)
 * M2FC
 * 2015 Eivind Roson Eide, Cambridge University Spaceflight
 */

#ifndef ANALOGUE_H
#define ANALOGUE_H


#include "ch.h"
#include "hal.h"
#include <stdlib.h>


msg_t analogue_thread(void *args);

#endif // #ifndef ANALOGUE_H
