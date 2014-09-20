/*
 * RockBLOCK transmission controller
 * M2R
 * 2014 Adam Greig, Cambridge University Spaceflight
 */

#ifndef ROCKBLOCK_H
#define ROCKBLOCK_H

#include "ch.h"
#include "hal.h"
#include "ublox.h"

#define ROCKBLOCK_PORT SD1

void rockblock_init(void);
void send_sbd_posn(const ublox_pvt_t *pvt);

bool_t rb_enqueue(char* msg);
msg_t rb_thread(void* arg);

#endif /* ROCKBLOCK_H */
