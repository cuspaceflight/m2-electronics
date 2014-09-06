/*
 * Mission state machine
 * M2FC
 * 2014 Adam Greig, Cambridge University Spaceflight
 */

#include "mission.h"

typedef enum {
    STATE_PAD, STATE_IGNITION, STATE_POWERED_ASCENT, STATE_FREE_ASCENT,
    STATE_APOGEE, STATE_DROGUE_DESCENT, STATE_RELEASE_MAIN,
    STATE_MAIN_DESCENT, STATE_LAND, STATE_LANDED, NUM_STATES
} state_t;

typedef struct instance_data instance_data_t;
typedef state_t state_func_t(instance_data_t *data);

state_t do_state_pad(instance_data_t *data);
state_t do_state_ignition(instance_data_t *data);
state_t do_state_powered_ascent(instance_data_t *data);
state_t do_state_free_ascent(instance_data_t *data);
state_t do_state_apogee(instance_data_t *data);
state_t do_state_drogue_descent(instance_data_t *data);
state_t do_state_release_main(instance_data_t *data);
state_t do_state_main_descent(instance_data_t *data);
state_t do_state_land(instance_data_t *data);
state_t do_state_landed(instance_data_t *data);

state_func_t* const state_table[NUM_STATES] = {
    do_state_pad, do_state_ignition, do_state_powered_ascent,
    do_state_free_ascent, do_state_apogee, do_state_drogue_descent,
    do_state_release_main, do_state_main_descent, do_state_land,
    do_state_landed
};

state_t run_state(state_t cur_state, instance_data_t *data) {
    return state_table[cur_state](data);
};

msg_t mission_tick(void* arg)
{
    (void)arg;
    state_t cur_state = STATE_PAD;
    instance_data_t data;

    while(1) {
        cur_state = run_state(cur_state, &data);
    }
}
