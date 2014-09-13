/*
 * Mission state machine
 * M2FC
 * 2014 Adam Greig, Cambridge University Spaceflight
 */

#ifndef MISSION_H
#define MISSION_H

#include "ch.h"
#include "state_estimation.h"

typedef enum {
    STATE_PAD = 0, STATE_IGNITION, STATE_POWERED_ASCENT, STATE_FREE_ASCENT,
    STATE_APOGEE, STATE_DROGUE_DESCENT, STATE_RELEASE_MAIN,
    STATE_MAIN_DESCENT, STATE_LAND, STATE_LANDED, NUM_STATES
} state_t;

struct instance_data {
    int32_t t_launch;
    int32_t t_apogee;
    state_estimate_t state;
};

typedef struct instance_data instance_data_t;

msg_t mission_thread(void *arg);

#endif /* MISSION_H */
