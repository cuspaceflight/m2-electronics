/*
 * Radio transmission controller
 * M2R
 * 2014 Adam Greig, Cambridge University Spaceflight
 */

#include "radio.h"
#include "pyro.h"
#include "hal.h"
#include "chprintf.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "m2status.h"

#define MSGLEN 1024

static void radio_make_telem_string(char* buf, size_t len);

uint16_t crc_update(uint16_t crc, uint8_t data);
uint16_t sentence_calc_checksum(char* buf);

uint16_t crc_update(uint16_t crc, uint8_t data) {
    int i;

    crc = crc ^ ((uint16_t)data << 8);
    for (i=0; i<8; i++)
    {
        if (crc & 0x8000)
            crc = (crc << 1) ^ 0x1021;
        else
            crc <<= 1;
    }

    return crc;
}

uint16_t sentence_calc_checksum(char* buf) {
    uint16_t crc = 0xFFFF;
    uint8_t i;
    for(i=0; i<strlen(buf); i++)
        crc = crc_update(crc, buf[i]);
    return crc;
}

uint16_t packet_counter = 0;
static void radio_make_telem_string(char* buf, size_t len)
{
    packet_counter += 1;
    int n = 0;

    double lat = (double)M2RStatus.latest.gps_lat * (double)1e-7f;
    double lng = (double)M2RStatus.latest.gps_lng * (double)1e-7f;
    int alt = M2RStatus.latest.gps_alt / 1000;
    int sv = M2RStatus.latest.gps_num_sv;
    if(sv == 0) {
        lat = 0.0f;
        lng = 0.0f;
        alt = 0;
    }

    n += chsnprintf(buf, len,
                    "$$$M2R,%d,%02d:%02d:%02d,%.5f,%.5f,%d,%d,%d,%d",
                    packet_counter, M2RStatus.latest.gps_t_hour,
                    M2RStatus.latest.gps_t_min, M2RStatus.latest.gps_t_sec,
                    lat, lng, alt, sv, pyro_cont, pyro_fired);

    uint16_t checksum = sentence_calc_checksum(buf+3);
    n += chsnprintf(buf+n, len-n, "*%04X\n", checksum);
}

uint8_t radio_ssb_bitidx = 0;
uint8_t radio_ssb_bitbuf[11];
uint8_t radio_ssb_byteidx = 0;
uint8_t radio_ssb_bytebuf[MSGLEN];
static void radio_ssb_timer(GPTDriver *gptd)
{
    uint8_t i, byte;
    (void)gptd;

    if(radio_ssb_bitbuf[radio_ssb_bitidx]) {
        DAC->DHR8R1 = 100;
    } else {
        DAC->DHR8R1 = 110;
    }

    radio_ssb_bitidx++;

    if(radio_ssb_bitidx == 11) {
        radio_ssb_bitidx = 0;
        radio_ssb_byteidx++;
        if(radio_ssb_bytebuf[radio_ssb_byteidx] == 0x00) {
            /* End of message */
            radio_ssb_byteidx = 0;
            radio_make_telem_string((char*)radio_ssb_bytebuf, MSGLEN);
        } else {
            /* START bit */
            radio_ssb_bitbuf[0] = 0;
            /* Data bits, 7bit ASCII */
            byte = radio_ssb_bytebuf[radio_ssb_byteidx];
            for(i=0; i<7; i++)
                radio_ssb_bitbuf[i + 1] = (byte & (1<<i)) >> i;
            /* Stop bits */
            radio_ssb_bitbuf[8] = 1;
            radio_ssb_bitbuf[9] = 1;
            radio_ssb_bitbuf[10] = 1;

            palSetPad(GPIOB, GPIOB_LED_RADIO);
        }
    } else {
        palClearPad(GPIOB, GPIOB_LED_RADIO);
    }
}

void radio_say(u8* buf, u16 len)
{
    (void)buf;
    (void)len;
}


static const GPTConfig gptcfg1 = {
    32000,
    radio_ssb_timer,
    0
};

msg_t radio_thread(void* arg)
{
    (void)arg;

    m2status_radio_status(STATUS_WAIT);
    chRegSetThreadName("Radio");
    palSetPad(GPIOB, GPIOB_MTX2_EN);

    chsnprintf((char*)radio_ssb_bytebuf, MSGLEN, "M2R M2R SSB INIT\r\n");

    /* Enable DAC */
    RCC->APB1ENR |= (1<<29);
    DAC->CR = DAC_CR_EN1 | DAC_CR_BOFF1;

    /* Enable 50Hz SSB timer */
    gptStart(&GPTD2, &gptcfg1);
    gptStartContinuous(&GPTD2, 640);

    m2status_radio_status(STATUS_OK);

    while(TRUE) {
        chThdSleepMilliseconds(100);
    }
}
