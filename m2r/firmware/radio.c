/*
 * Radio transmission controller
 * M2R
 * 2014 Adam Greig, Cambridge University Spaceflight
 */

#include "radio.h"
#include "hal.h"
#include "chprintf.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "dispatch.h"

#define PI 3.14159f
#define BAUD (50.0f)
#define T_BIT (1.0f / BAUD)
#define FS (8000.0f)
#define SAMPLES_PER_BIT 160 //T_BIT * FS
#define F_MARK  (1000.0f)
#define F_SPACE (1350.0f)
#define MSGLEN 128
static uint8_t mark_buf[SAMPLES_PER_BIT];
static uint8_t space_buf[SAMPLES_PER_BIT];

static void radio_generate_buffers()
{
    uint32_t i;
    for(i=0; i<SAMPLES_PER_BIT; i++) {
        float t = ((float)i / (float)SAMPLES_PER_BIT) * T_BIT;
        float mark  = sinf(2.0f * PI * F_MARK * t);
        float space = sinf(2.0f * PI * F_SPACE * t);
        mark_buf[i]  = (uint8_t)((mark  * 127.0f) + 127.0f);
        space_buf[i] = (uint8_t)((space * 127.0f) + 127.0f);
    }
}

static void radio_make_telem_string(char* buf, size_t len)
{
    chsnprintf(buf, len,
             "AD6AM %02d:%02d:%02d %d,%d (%d, %d) "
             "%.0fm (%.0fm) %.0fm/s (%.0fm/s)\n",
             m2r_state.hour, m2r_state.minute, m2r_state.second,
             m2r_state.lat, m2r_state.lng, m2r_state.gps_valid,
             m2r_state.gps_num_sats, m2r_state.imu_height,
             m2r_state.imu_max_height, m2r_state.imu_velocity,
             m2r_state.imu_max_velocity);
}

uint8_t *radio_fm_sampbuf;
uint8_t radio_fm_sampidx = 0;
uint16_t radio_fm_samplen = 0;
bool_t radio_fm_rtty = true;
uint8_t radio_fm_bitidx = 0;
uint8_t radio_fm_bitbuf[10];
uint8_t radio_fm_byteidx = 0;
uint8_t radio_fm_bytebuf[MSGLEN];
uint8_t radio_fm_audioidx = 0;
uint8_t* radio_fm_audioqueue[16];
uint16_t radio_fm_audioqueuelens[16];
static void radio_fm_timer(GPTDriver *gptd)
{
    uint8_t i, byte;
    (void)gptd;
    /* Write next sample to the DAC */
    DAC->DHR12R1 = (uint32_t)radio_fm_sampbuf[radio_fm_sampidx] << 1;
    radio_fm_sampidx++;

    /* If we just wrote the last sample... */
    if(radio_fm_sampidx == radio_fm_samplen) {
        radio_fm_sampidx = 0;

        palTogglePad(GPIOB, GPIOB_LED_RADIO);

        /* If we're transmitting RTTY... */
        if(radio_fm_rtty) {
            radio_fm_bitidx++;
            
            /* If we're now also transmitted all bits... */
            if(radio_fm_bitidx == 10) {
                radio_fm_bitidx = 0;
                radio_fm_byteidx++;
                if(radio_fm_bytebuf[radio_fm_byteidx] == 0x00) {
                    /* End of message */
                    radio_fm_byteidx = 0;
                    radio_make_telem_string(radio_fm_bytebuf, 128);
                } else {
                    /* START */
                    radio_fm_bitbuf[0] = 1;
                    /* Data ASCII7 */
                    byte = radio_fm_bytebuf[radio_fm_byteidx];
                    for(i=0; i<7; i++)
                        radio_fm_bitbuf[i + 1] = byte & (1 << i);
                    /* STOPs */
                    radio_fm_bitbuf[8] = 1;
                    radio_fm_bitbuf[9] = 1;
                }
            }

            /* Set new bit buffer */
            if(radio_fm_bitbuf[radio_fm_bitidx])
                radio_fm_sampbuf = mark_buf;
            else
                radio_fm_sampbuf = space_buf;


        /* If we're transmitting audio... */
        } else {
            /* Need to load next audio file to play. */
            radio_fm_audioidx++;
            if(radio_fm_audioqueue[radio_fm_audioidx] == 0x00) {
                /* End of message */
                radio_fm_sampidx = 0;
                radio_fm_audioidx = 0;
            } else {
                radio_fm_sampbuf = radio_fm_audioqueue[
                    radio_fm_audioidx];
                radio_fm_samplen = radio_fm_audioqueuelens[
                    radio_fm_audioidx];
            }
        }
    }
}

uint8_t radio_ssb_bitidx = 0;
uint8_t radio_ssb_bitbuf[10];
uint8_t radio_ssb_byteidx = 0;
uint8_t radio_ssb_bytebuf[MSGLEN];
static void radio_ssb_timer(GPTDriver *gptd)
{
    uint8_t i, byte;
    (void)gptd;

    if(radio_ssb_bitbuf[radio_ssb_bitidx]) {
        DAC->DHR8R2 = 100;
    } else {
        DAC->DHR8R2 = 183;
    }

    radio_ssb_bitidx++;

    if(radio_ssb_bitidx == 10) {
        radio_ssb_bitidx = 0;
        radio_ssb_byteidx++;
        if(radio_ssb_bytebuf[radio_ssb_byteidx] == 0x00) {
            /* End of message */
            radio_ssb_byteidx = 0;
            radio_make_telem_string(radio_ssb_bytebuf, 128);
        } else {
            /* START bit */
            radio_ssb_bitbuf[0] = 1;
            /* Data bits, 7bit ASCII */
            byte = radio_ssb_bytebuf[radio_ssb_byteidx];
            for(i=0; i<7; i++)
                radio_ssb_bitbuf[i + 1] = byte & (1<<i);
            /* Stop bits */
            radio_ssb_bitbuf[8] = 1;
            radio_ssb_bitbuf[9] = 1;
        }
    }
}

static const GPTConfig gptcfg1 = {
    32000,
    radio_fm_timer,
    0
};

msg_t radio_thread(void* arg)
{
    chRegSetThreadName("Radio");

    /* Compute the sine waves for AFSK */
    radio_generate_buffers();

    /* Enable DAC */
    RCC->APB1ENR |= (1<<29);
    DAC->CR = DAC_CR_EN1 | DAC_CR_BOFF1;

    /* Enable 8kHz FM Radio timer */
    gptStart(&GPTD2, &gptcfg1);
    gptStartContinuous(&GPTD2, 4);

    strncpy(radio_fm_bytebuf, "AD6AM MARTLET 2 FM INITIALISE ", 128);

    while(TRUE) {
        chThdSleepMilliseconds(100);
    }
}
