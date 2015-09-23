/* M2Serial
 * 2015 Adam Greig, Cambridge University Spaceflight
 */

#include "m2serial.h"
#include <stdbool.h>
#include "hal.h"
#include "m2status.h"

Mutex uart_mtx;
void(*m2serial_shell)(BaseSequentialStream*) = NULL;

msg_t m2serial_thread(void* arg)
{
    static uint8_t buf[32];
    static DeframeState df;
    static TelemPacket pkt;
    size_t n;
    bool got_packet;
    int n_escapes = 0;
    int i;

    chMtxInit(&uart_mtx);
    chMtxLock(&uart_mtx);
    sdStart(&SD1, NULL);
    chMtxUnlock();

    while(true) {
        n = sdAsynchronousRead(&SD1, buf, sizeof(buf));
        got_packet = m2telem_deframe(buf, n, &df, &pkt);
        if(got_packet) {
            m2status_rx_packet(&pkt);
        }

        /* If 0x7D 7D 7D 7D is transmitted, run a shell. */
        for(i=0; i<n; i++) {
            if(buf[i] == 0x7D)
                n_escapes++;
            else
                n_escapes = 0;

            if(n_escapes == 4) {
                if(m2serial_shell != NULL)
                    m2serial_shell(&SD1);
                n_escapes = 0;
                break;
            }
        }
    }
}

void m2serial_send_packet(TelemPacket* packet)
{
    chMtxLock(&uart_mtx);
    static uint8_t buf[33];
    size_t buf_len;
    m2telem_enframe(packet, buf, &buf_len);
    sdWrite(&SD1, buf, buf_len);
    chMtxUnlock();
}
