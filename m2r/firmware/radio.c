/*
 * Radio transmission controller
 * M2R
 * 2014 Adam Greig, Cambridge University Spaceflight
 */

#include "radio.h"
#include "hal.h"
#include <stdlib.h>
#include <math.h>

static char radio_queue_buf[256];
static char radio_tx_buf[256];
static char radio_rs232_buf[11];

const float baud = 50.0f;
const float t_bit = 1.0f / baud;
const float fs = 8000.0f;
const uint32_t samples_per_bit = t_bit * fs;

static uint8_t mark_buf[samples_per_bit];
static uint8_t space_buf[samples_per_bit];
static uint8_t *dac_buf;
static uint32_t dac_idx;

const float f_mark  = 1300.0f;
const float f_space = 1000.0f;

#define PI 3.14159f

#if 0
#define DAC1 (uint32_t*)0x40007400
#define DAC!_CR (DAC1+0x00)
#define DAC1_DHR8R1 (DAC1+0x10)
#endif

void generate_buffers(void);


void generate_buffers()
{
    uint32_t i;
    for(i=0; i<samples_per_bit; i++) {
        float t = ((float)i / samples_per_bit) * t_bit;
        float mark  = sinf(2.0f * PI * f_mark * t);
        float space = sinf(2.0f * PI * f_space * t);
        mark_buf[i]  = (uint8_t)((mark  * 2047.0f) + 2047.0f;
        space_buf[i] = (uint8_t)((space * 2047.0f) + 2047.0f;
    }
}

bool_t radio_enqueue(char* msg)
{
    strncpy(radio_queue_buf, msg, 256);
}

void radio_timer()
{
    DAC->DHR8R1 = dac_buf[dac_idx++];
    if(dac_idx == samples_per_bit) {
        dac_idx = 0;
        if(dac_buf == mark_buf)
            dac_buf = space_buf;
        else
            dac_buf = mark_buf;
    }
}

static const GPTConfig gptcfg = {
    8000,
    radio_timer
};

msg_t radio_thread(void* arg)
{
    chRegSetThreadName("Radio");

    radio_enqueue("AD6AM ");
    strncpy(radio_tx_buf, radio_queue_buf, 256);

    dac_buf = mark_buf;
    dac_idx = 0;

    /* Enable DAC */
    DAC->CR = DAC_CR_EN1;

    gptStart(&GPTD2, &gptcfg);

    while(TRUE) {
        chThdSleepMilliseconds(100);
        /*strncpy(radio_tx_buf, radio_queue_buf, 256);*/
    }
}
