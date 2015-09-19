#include "m2telem.h"

extern uint16_t compute_crc(uint8_t *buf, size_t len)
{
    size_t i, j;
    uint16_t crc = 0x0000;
    for(i=0; i<len; i++) {
        crc ^= (uint16_t) buf[i] << 8;
        for(j=0; j<8; j++) {
            if(crc & 0x8000) {
                crc = (crc << 1) ^ 0x1021;
            } else {
                crc <<= 1;
            }
        }
    }
    return crc;
}

void m2telem_write_checksum(TelemPacket *packet)
{
    uint16_t crc = compute_crc((uint8_t*)packet, 14);
    packet->checksum = crc;
}

bool m2telem_check_checksum(TelemPacket *packet)
{
    uint16_t crc = compute_crc((uint8_t*)packet, 14);
    return packet->checksum == crc;
}

void m2telem_enframe(TelemPacket* pkt, uint8_t* buf, size_t* buf_len)
{
    int pkt_idx, buf_idx;
    uint8_t* pkt_p = (uint8_t*)pkt;

    buf[0] = 0x7E;
    for(pkt_idx=0, buf_idx=1; pkt_idx<16; pkt_idx++, buf_idx++) {
        uint8_t val = pkt_p[pkt_idx];
        if(val != 0x7E && val != 0x7D) {
            buf[buf_idx] = val;
        } else {
            buf[buf_idx] = 0x7D;
            buf_idx++;
            buf[buf_idx] = val ^ 0x20;
        }
    }

    *buf_len = buf_idx;
}


#define DEFRAME_STATE_START   (0)
#define DEFRAME_STATE_NORMAL  (1)
#define DEFRAME_STATE_ESCAPED (2)
bool m2telem_deframe(uint8_t* buf, size_t buf_len,
                     DeframeState* state,
                     TelemPacket* pkt)
{
    size_t buf_idx, pkt_idx;
    uint8_t* pkt_p = (uint8_t*)pkt;
    bool ret = false;

    /* Process each byte of the input buffer. */
    for(buf_idx=0; buf_idx < buf_len; buf_idx++) {
        uint8_t val = buf[buf_idx];

        /* Always go to the START state when we see a 0x7E. */
        if(val == 0x7E) {
            state->s = DEFRAME_STATE_START;
        }

        switch(state->s) {
            case DEFRAME_STATE_START:
                state->idx = 0;
                if(val == 0x7E) {
                    state->s = DEFRAME_STATE_NORMAL;
                }
                break;

            case DEFRAME_STATE_NORMAL:
                if(val != 0x7D) {
                    state->buf[state->idx] = val;
                    state->idx++;
                } else {
                    state->s = DEFRAME_STATE_ESCAPED;
                }
                break;

            case DEFRAME_STATE_ESCAPED:
                state->buf[state->idx] = val ^ 0x20;
                state->idx++;
                state->s = DEFRAME_STATE_NORMAL;
                break;
        }

        /* If we've read in a whole packet, copy it to the output packet
         * and set the return flag to true.
         */
        if(state->idx == 16) {
            for(pkt_idx=0; pkt_idx<16; pkt_idx++) {
                pkt_p[pkt_idx] = state->buf[pkt_idx];
            }
            state->s = DEFRAME_STATE_START;
            ret = true;
        }
    }

    return ret;
}
