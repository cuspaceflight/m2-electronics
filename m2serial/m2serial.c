/* M2Serial
 * 2015 Adam Greig, Cambridge University Spaceflight
 */

#include "m2serial.h"
#include <stdbool.h>
#include "hal.h"
#include "m2status.h"

static bool recv_systemstatus(SystemStatus* status);

Mutex uart_mtx;
void(*m2serial_shell)(BaseSequentialStream*) = NULL;

SerialDriver *M2SerialSD;

msg_t m2serial_thread(void* arg)
{
    SystemStatus status;
    (void)arg;
    while(M2SerialSD == NULL) chThdSleepMilliseconds(100);

    chMtxInit(&uart_mtx);
    chMtxLock(&uart_mtx);
    sdStart(M2SerialSD, NULL);
    chMtxUnlock();

    while(true) {
        if(recv_systemstatus(&status)) {
            m2status_rx_systemstatus(&status);
        }
    }
}

static uint16_t compute_crc(uint8_t *buf, size_t len)
{
    size_t i, j;
    uint16_t crc = 0xFFFF;
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
    return crc ^ 0xFFFF;
}

void m2serial_send_buffer(void* inbuf, size_t n)
{
    /* Start with 7E
     * Escape all 7E and 7D to 7D 5E and 7D 5D
     * Append CRC16 checksum
     * Transmit
     */
    size_t i;
    uint8_t* buf = (uint8_t*)inbuf;
    uint16_t chk = compute_crc(buf, n);

    chMtxLock(&uart_mtx);

    sdPut(M2SerialSD, 0x7E);
    for(i=0; i<n; i++) {
        if(buf[i] == 0x7D || buf[i] == 0x7E) {
            sdPut(M2SerialSD, 0x7D);
            sdPut(M2SerialSD, buf[i] ^ 0x20);
        } else {
            sdPut(M2SerialSD, buf[i]);
        }
    }
    sdPut(M2SerialSD, (chk & 0xFF));
    sdPut(M2SerialSD, (chk & 0xFF00) >> 8);

    chMtxUnlock();
}

static bool recv_systemstatus(SystemStatus* status)
{
    size_t i;
    uint16_t chk, rx_chk_l, rx_chk_h;

    /* Await start symbol */
    while(sdGet(M2SerialSD) != 0x7E);

    /* Read in all the bytes in a SystemStatus */
    for(i=0; i<sizeof(SystemStatus); i++) {
        uint8_t b = sdGet(M2SerialSD);

        /* Handle escaped characters */
        if(b == 0x7D) {
            b = sdGet(M2SerialSD) ^ 0x20;
        } else if(b == 0x7E) {
            /* 7E7E7E will start a shell, otherwise we've de-synced and should
             * restart. */
            if(i == 0 && sdGet(M2SerialSD) == 0x7E && m2serial_shell != NULL) {
                chMtxLock(&uart_mtx);
                m2serial_shell((BaseSequentialStream*)M2SerialSD);
                chMtxUnlock();
            } else {
                return false;
            }
        }

        /* Save this byte in the SystemStatus */
        ((uint8_t*)status)[i] = b;
    }

    /* Read in checksum */
    rx_chk_l = sdGet(M2SerialSD);
    rx_chk_h = sdGet(M2SerialSD);

    /* Compute local checksum */
    chk = compute_crc((uint8_t*)status, sizeof(SystemStatus));

    /* Compare checksums */
    if(((chk & 0xFF) == rx_chk_l) && (((chk >> 8) & 0xFF) == rx_chk_h)) {
        return true;
    } else {
        return false;
    }
}
