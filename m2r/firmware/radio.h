/*
 * Radio transmission controller
 * M2R
 * 2014 Adam Greig, Cambridge University Spaceflight
 */

#ifndef RADIO_H
#define RADIO_H

#include "ch.h"
#include <stdint.h>

bool_t radio_enqueue(char* msg);
msg_t radio_thread(void* arg);
void radio_say(uint8_t* buf, uint16_t len);

#endif /* RADIO_H */
