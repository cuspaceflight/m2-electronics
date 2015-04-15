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
#include "audio_data.h"

#define PI 3.14159f
#define BAUD (100.0f)
#define T_BIT (1.0f / BAUD)
#define FS (8000.0f)
#define SAMPLES_PER_BIT 80 //T_BIT * FS
#define F_MARK  (1000.0f)
#define F_SPACE (1350.0f)
#define MSGLEN 128
static uint8_t mark_buf[SAMPLES_PER_BIT];
static uint8_t space_buf[SAMPLES_PER_BIT];
static uint8_t zero_buf[SAMPLES_PER_BIT];

static void radio_generate_buffers()
{
    uint32_t i;
    for(i=0; i<SAMPLES_PER_BIT; i++) {
        float t = ((float)i / (float)SAMPLES_PER_BIT) * T_BIT;
        float mark  = sinf(2.0f * PI * F_MARK * t);
        float space = sinf(2.0f * PI * F_SPACE * t);
        mark_buf[i]  = (uint8_t)((mark  * 127.0f) + 127.0f);
        space_buf[i] = (uint8_t)((space * 127.0f) + 127.0f);
        zero_buf[i] = 0;
    }
}

static void radio_make_telem_string(char* buf, size_t len)
{
    chsnprintf(buf, len,
             " AD6AM AD6AM AD6AM %02d:%02d:%02d %.5f,%.5f (%d, %d) "
             "%dm (%dm) %dm/s (%dm/s) %u\n",
             m2r_state.hour, m2r_state.minute, m2r_state.second,
             (float)m2r_state.lat*1e-7f, (float)m2r_state.lng*1e-7f,
             m2r_state.gps_valid,
             m2r_state.gps_num_sats, (int)m2r_state.imu_height,
             (int)m2r_state.imu_max_height, (int)m2r_state.imu_velocity,
             (int)m2r_state.imu_max_velocity, m2r_state.fc_state);
}

uint8_t *radio_fm_sampbuf;
uint16_t radio_fm_sampidx = 0;
uint16_t radio_fm_samplen = 1;
uint8_t radio_fm_rtty = 1;
uint8_t radio_fm_bitidx = 0;
uint8_t radio_fm_bitbuf[11];
uint8_t radio_fm_byteidx = 0;
uint8_t radio_fm_bytebuf[MSGLEN];
uint8_t radio_fm_audioidx = 0;
uint8_t* radio_fm_audioqueue[16];
uint16_t radio_fm_audioqueuelens[16];
static void radio_fm_timer(GPTDriver *gptd)
{
    uint8_t i, byte;
    (void)gptd;
    /* If we just wrote the last sample... */
    if(radio_fm_sampidx >= radio_fm_samplen) {
        radio_fm_sampidx = 0;

        palTogglePad(GPIOB, GPIOB_LED_RADIO);

        /* If we're transmitting RTTY... */
        if(radio_fm_rtty) {

            radio_fm_bitidx++;
            /* If we're now also transmitted all bits... */
            if(radio_fm_bitidx == 11) {
                radio_fm_bitidx = 0;
                radio_fm_byteidx++;
                if(radio_fm_bytebuf[radio_fm_byteidx] == 0x00) {
                    /* End of message */
                    radio_fm_byteidx = 0;
                    radio_make_telem_string(radio_fm_bytebuf, 128);
                    radio_fm_rtty = 0;
                    radio_fm_audioidx = 0;

                } else {
                    /* START */
                    radio_fm_bitbuf[0] = 0;
                    /* Data ASCII7 */
                    byte = radio_fm_bytebuf[radio_fm_byteidx];
                    for(i=0; i<7; i++)
                        radio_fm_bitbuf[i + 1] = (byte & (1 << i)) >> i;
                    /* STOPs */
                    radio_fm_bitbuf[8] = 1;
                    radio_fm_bitbuf[9] = 1;
                    radio_fm_bitbuf[10] = 1;
                }
            }

            /* Set new bit buffer */
            if(radio_fm_bitbuf[radio_fm_bitidx] == 1)
                radio_fm_sampbuf = mark_buf;
            else if(radio_fm_bitbuf[radio_fm_bitidx] == 2)
                radio_fm_sampbuf = zero_buf;
            else if(radio_fm_bitbuf[radio_fm_bitidx] == 0)
                radio_fm_sampbuf = space_buf;
            radio_fm_samplen = SAMPLES_PER_BIT;

        /* If we're transmitting audio... */
        } else {
            /* Need to load next audio file to play. */
            if(radio_fm_audioqueue[radio_fm_audioidx] == 0x00) {
                /* End of message */
                radio_fm_sampidx = 65534;
                radio_fm_audioidx = 0;
                say_altitude(m2r_state.imu_height,
                             &radio_fm_audioqueue,
                             &radio_fm_audioqueuelens);
                radio_fm_rtty = 1;
            }

            radio_fm_sampbuf = radio_fm_audioqueue[
                radio_fm_audioidx];
            radio_fm_samplen = radio_fm_audioqueuelens[
                radio_fm_audioidx];
            radio_fm_audioidx++;
        }
    }

    /* Write next sample to the DAC */
    DAC->DHR8R2 = radio_fm_sampbuf[radio_fm_sampidx];
    radio_fm_sampidx++;

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

void radio_say(u8* buf, u16 len)
{
    radio_fm_audioqueue[0] = buf;
    radio_fm_audioqueue[1] = 0;
    radio_fm_audioqueuelens[0] = len;
    radio_fm_rtty = 0;
}

static const GPTConfig gptcfg1 = {
    32000,
    radio_fm_timer,
    0
};

msg_t radio_thread(void* arg)
{
    chRegSetThreadName("Radio");

    palSetPad(GPIOB, GPIOB_MTX2_EN);

    /* Compute the sine waves for AFSK */
    radio_generate_buffers();

    chsnprintf(radio_fm_bytebuf, MSGLEN, "AD6AM AD6AM MARTLET 2 FM INITIALISE ");

    /* Enable DAC */
    RCC->APB1ENR |= (1<<29);
    DAC->CR = DAC_CR_EN2 | DAC_CR_BOFF2;

    /*while(!m2r_state.armed)*/
        /*chThdSleepMilliseconds(100);*/

    say_altitude(m2r_state.imu_height,
                 &radio_fm_audioqueue,
                 &radio_fm_audioqueuelens);


    /* Enable 8kHz FM Radio timer */
    gptStart(&GPTD2, &gptcfg1);
    gptStartContinuous(&GPTD2, 4);

    while(TRUE) {
        chThdSleepMilliseconds(100);
    }
}
