/*
 * Grand Central Dispatch
 * M2R
 * 2014 Adam Greig, Cambridge University Spaceflight
 */

#ifndef DISPATCH_H
#define DISPATCH_H

#include "ch.h"
#include "ublox.h"

void dispatch_pvt(const ublox_pvt_t pvt);
msg_t dispatch_thread(void* arg);

#endif /* DISPATCH_H */
