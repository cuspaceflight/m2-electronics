/*
 * Mission state machine
 * M2FC
 * 2014 Adam Greig, Cambridge University Spaceflight
 */

#ifndef MISSION_H
#define MISSION_H

#include "ch.h"
#include "state_estimation.h"

msg_t mission_thread(void *arg);

#endif /* MISSION_H */
