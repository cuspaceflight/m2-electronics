/*
 * ADC Driver (ADC1, ADC2, ADC3)
 * M2FC
 * 2015 Eivind Roson Eide, Cambridge University Spaceflight
 */

#ifndef ANALOGUE_H
#define ANALOGUE_H

#include "ch.h"

/*
 * Run the ADCs, sampling the three strain gauges simultaneously and then the
 * three thermocouples simultaneously, both at 20kS/s.
 * The data is then logged to the SD card.
 */
msg_t analogue_thread(void *args);

#endif // #ifndef ANALOGUE_H
