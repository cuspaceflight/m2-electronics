/* M2RL Driver
 * 2015 Adam Greig, Cambridge University Spaceflight
 */

#include "m2rl.h"

msg_t m2rl_thread(void* arg)
{
    (void)arg;
    while(true) {
        chThdSleepMilliseconds(1000);
    }
}

void m2rl_send_packet(TelemPacket* pkt)
{
    (void)pkt;
}

void m2rl_send_buffer(void* buf, size_t n)
{
    (void)buf;
    (void)n;
}
