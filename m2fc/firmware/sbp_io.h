/*
 * SBP IO
 * 2014 Fergus Noble, Cambridge University Spaceflight
 */

#ifndef SBP_H
#define SBP_H

#include "ch.h"
#include "hal.h"
#include <sbp.h>

#define M2R_PORT SD1

msg_t sbp_thread(void* arg);
u32 m2r_write(u8 *buff, u32 n, void* context);

extern sbp_state_t sbp_state;

#define SBP_SEND(type, x) sbp_send_message(&sbp_state, (type), 0, sizeof(x), (u8*)&(x), &m2r_write);

#endif /* SBP_H */


