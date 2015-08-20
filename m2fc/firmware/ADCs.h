
/*
 * ADC Driver (ADC1, ADC2, ADC3)
 * M2FC
 * 2015 Eivind Roson Eide, Cambridge University Spaceflight
 */

#ifndef _ADCs_H_
#define _ADCs_H_


#include "ch.h"  // Do I need these?
#include "hal.h" // Do I need these?
#include <stdlib.h>

#include <adc_lld.h>
#include <gpt_lld.h>

#include "microsd.h"
#include "config.h"
#include "state_estimation.h"
#include "chprintf.h"



msg_t ADC_read_SGs_and_TCs (void *args);

static void gpt_adc_trigger(GPTDriver *gpt_ptr);
static void adccallback(ADCDriver *adcDriverpointer, adcsample_t *buffer, size_t n);
static void adcerrorcallback(ADCDriver *adcDriverpointer, adcerror_t err);
static void saveResults(size_t n);

#endif // #ifndef _ADCs_H_
