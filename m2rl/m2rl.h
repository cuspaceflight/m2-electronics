/* M2RL Driver
 * 2015 Adam Greig, Cambridge University Spaceflight
 */

#ifndef M2RL_H
#define M2RL_H

#include "ch.h"
#include "m2telem.h"

msg_t m2rl_thread(void* arg);
void m2rl_send_packet(TelemPacket* pkt);
void m2rl_send_buffer(void* buf, size_t n);

#endif /* M2RL_H */
