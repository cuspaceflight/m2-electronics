
/*
 * ADC Driver (ADC1, ADC2, ADC3)
 * M2FC
 * 2015 Eivind Roson Eide, Cambridge University Spaceflight
 */


#include "ch.h"  // Do I need these?
#include "hal.h" // Do I need these?


msg_t ADC_read_SGs_and_TCs (void *args);

static void gpt_adc_trigger(GPTDriver *gpt_ptr);
static void adccallback(ADCDriver *adcDriverpointer, adcsample_t *buffer, size_t n);
static void adc_error_callback(ADCDriver *adcDriverpointer, adcerror_t err);
