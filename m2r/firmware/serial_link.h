/*
 * Serial transmit and receive controller
 * M2R
 * 2014 Adam Greig, Cambridge University Spaceflight
 */

#ifndef SERIAL_LINK_H
#define SERIAL_LINK_H

#include "ch.h"

msg_t serial_link_thread(void* arg);
bool_t serial_enqueue(char* msg);

#endif /* SERIAL_LINK_H */
