/*
 * RockBLOCK transmission controller
 * M2R
 * 2014 Adam Greig, Cambridge University Spaceflight
 */

#ifndef ROCKBLOCK_H
#define ROCKBLOCK_H

#include "ch.h"

bool_t rb_enqueue(char* msg);
msg_t rb_thread(void* arg);

#endif /* ROCKBLOCK_H */
