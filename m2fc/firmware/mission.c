/*
 * Mission state machine
 * M2FC
 * 2014 Adam Greig, Cambridge University Spaceflight
 */

#include <math.h>
#include "mission.h"
#include "state_estimation.h"
#include "pyro.h"
#include "microsd.h"
#include "config.h"

typedef enum {
    STATE_PAD, STATE_IGNITION, STATE_POWERED_ASCENT, STATE_FREE_ASCENT,
    STATE_APOGEE, STATE_DROGUE_DESCENT, STATE_RELEASE_MAIN,
    STATE_MAIN_DESCENT, STATE_LAND, STATE_LANDED, NUM_STATES
} state_t;

struct instance_data {
    int32_t t_launch;
    int32_t t_apogee;
    state_estimate_t state;
};

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

state_t do_state_pad(instance_data_t *data)
{
    state_estimation_trust_barometer = 1;
    if(fabsf(data->state.v) > IGNITION_VELOCITY)
        return STATE_IGNITION;
    else
        return STATE_PAD;
}

state_t do_state_ignition(instance_data_t *data)
{
    state_estimation_trust_barometer = 0;
    data->t_launch = chTimeNow();
    return STATE_POWERED_ASCENT;
}

state_t do_state_powered_ascent(instance_data_t *data)
{
    state_estimation_trust_barometer = 0;
    if(fabsf(data->state.a) < BURNOUT_ACCELERATION)
        return STATE_FREE_ASCENT;
    else if(chTimeNow() - data->t_launch > BURNOUT_TIMER)
        return STATE_FREE_ASCENT;
    else
        return STATE_POWERED_ASCENT;
}

state_t do_state_free_ascent(instance_data_t *data)
{
    state_estimation_trust_barometer = 1;
    if(fabsf(data->state.a) < APOGEE_ACCELERATION)
        return STATE_APOGEE;
    else if(chTimeNow() - data->t_launch > APOGEE_TIMER)
        return STATE_APOGEE;
    else
        return STATE_FREE_ASCENT;
}

state_t do_state_apogee(instance_data_t *data)
{
    state_estimation_trust_barometer = 1;
    data->t_apogee = chTimeNow();
    pyro_fire_drogue();
    return STATE_DROGUE_DESCENT;
}

state_t do_state_drogue_descent(instance_data_t *data)
{
    state_estimation_trust_barometer = 1;
    if(fabsf(data->state.h) < MAIN_DEPLOY_ALTITUDE)
        return STATE_RELEASE_MAIN;
    else if(chTimeNow() - data->t_apogee > MAIN_DEPLOY_TIMER)
        return STATE_RELEASE_MAIN;
    else
        return STATE_DROGUE_DESCENT;
}

state_t do_state_release_main(instance_data_t *data)
{
    (void)data;
    state_estimation_trust_barometer = 1;
    pyro_fire_main();
    return STATE_MAIN_DESCENT;
}

state_t do_state_main_descent(instance_data_t *data)
{
    state_estimation_trust_barometer = 1;
    if(fabsf(data->state.v) < LANDED_VELOCITY)
        return STATE_LAND;
    else if(chTimeNow() - data->t_apogee > LANDED_TIMER)
        return STATE_LAND;
    else
        return STATE_MAIN_DESCENT;
}

state_t do_state_land(instance_data_t *data)
{
    state_estimation_trust_barometer = 1;
    (void)data;
    return STATE_LANDED;
}

state_t do_state_landed(instance_data_t *data)
{
    state_estimation_trust_barometer = 1;
    (void)data;
    return STATE_LANDED;
}

msg_t mission_thread(void* arg)
{
    int32_t csi, nsi;
    (void)arg;
    state_t cur_state = STATE_PAD;
    state_t new_state;
    instance_data_t data;
    data.t_launch = -1;
    data.t_apogee = -1;

    while(1) {
        /* Run Kalman prediction step */
        data.state = state_estimation_get_state();

        /* Run state machine current state function */
        new_state = run_state(cur_state, &data);

        /* Log changes in state */
        if(new_state != cur_state) {
            csi = (int32_t)cur_state;
            nsi = (int32_t)new_state;
            microsd_log_s32(0xC0, &csi, &nsi);
        }

        cur_state = new_state;

        /* Tick the state machine about every millisecond */
        chThdSleepMilliseconds(1);
    }
}
