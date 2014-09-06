/*
 * State estimation and sensor fusion
 * M2FC
 * 2014 Adam Greig, Cambridge University Spaceflight
 */

#include <math.h>
#include "state_estimation.h"

static state_estimate_t x = {0f, 0f, 0f};
static float p[3][3] = {{0f, 0f, 0f}, {0f, 0f, 0f}, {0f, 0f, 0f}};
static int32_t t = 0;

void state_estimation_update_accel(float accel, float r);
float state_estimation_pressure_to_altitude(float pressure);

void state_estimation_new_pressure(float pressure)
{
    float h = state_estimation_pressure_to_altitude(pressure);
    float y, r, s_inv, k[3];

    /* TODO: determine r */
    r = 1.0f;

    /* Acquire lock */

    y = h - x[0];

    s_inv = 1f / (p[0][0] + r);

    k[0] = p[0][0] * s_inv;
    k[1] = p[1][0] * s_inv;
    k[2] = p[2][0] * s_inv;

    x[0] += k[0] * y;

    p[0][0] -= k0 * p[0][0];
    p[0][1] -= k0 * p[0][1];
    p[0][2] -= k0 * p[0][2];
    p[1][0] -= k1 * p[0][0];
    p[1][1] -= k1 * p[0][1];
    p[1][2] -= k1 * p[0][2];
    p[2][0] -= k2 * p[0][0];
    p[2][1] -= k2 * p[0][1];
    p[2][2] -= k2 * p[0][2];

    /* Release lock */
}

float state_estimation_pressure_to_altitude(float pressure)
{
}

void state_estimation_new_lg_accel(float lg_accel)
{
    if(abs(lg_accel) > 150.0f) {
        /* The low-g accelerometer is limited to +-16g, so
         * we'll just discard anything above 150m/s (15.3g).
         */
        return;
    }

    /* TODO: determine r */
    state_estimation_update_accel(lg_accel, 0.01f);
}

/*
 * Run the Kalman update step for a new observation of the acceleration.
 */
void state_estimation_new_hg_accel(float hg_accel)
{
    /* TODO: determine r */
    state_estimation_update_accel(hg_accel, 0.1f);
}

/* Run the Kalman update for a single acceleration value.
 * Called internally from the hg and lg functions after
 * preprocessing.
 */
void state_estimation_update_accel(float a, float r)
{
    float y, s_inv, k[3];

    /* Acquire lock */

    y = accel - x[2];

    s_inv = 1f / (p[2][2] + r);

    k[0] = p[0][2] * s_inv;
    k[1] = p[1][2] * s_inv;
    k[2] = p[2][2] * s_inv;

    x[2] += k[2] * y;

    p[0][0] -= k0 * p[2][0];
    p[0][1] -= k0 * p[2][1];
    p[0][2] -= k0 * p[2][2];
    p[1][0] -= k1 * p[2][0];
    p[1][1] -= k1 * p[2][1];
    p[1][2] -= k1 * p[2][2];
    p[2][0] -= k2 * p[2][0];
    p[2][1] -= k2 * p[2][1];
    p[2][2] -= k2 * p[2][2];

    /* Release lock */
}

/*
 * Run the Kalman prediction step and return the latest state estimate.
 */
state_estimate_t state_estimation_get_state()
{
    float new_p[3][3];
    float q, dt;
    int32_t dti, i, j;

    /* TODO Determine this q-value */
    q = 200.0;

    /* Acquire lock */

    /* Find elapsed time */
    /* TODO: Use a higher precision (1MHz) timer */
    /* TODO: Handle wraparound */
    dti = chTimeNow() - t;
    t += dti;
    dt = (float)dti * 0.001; /* TODO: Update constant for new timer */

    /* Update state */
    x[0] += dt * x[1];
    x[1] += dt * x[2];

    /* Update covariance */
    p[0][0] += dt * p[1][0] + dt * p[0][1] + dt * dt * p[1][1];
    p[0][1] += dt * p[1][1] + dt * p[0][2] + dt * dt * p[1][2];
    p[0][2] += dt * p[1][2];
    p[1][0] += dt * p[2][0] + dt * p[1][1] + dt * dt * p[2][1];
    p[1][1] += dt * p[2][1] + dt * p[1][2] + dt * dt * p[2][2];
    p[1][2] += dt * p[2][2];
    p[2][0] += dt * p[2][1];
    p[2][1] += dt * p[2][2];
    p[2][2] += dt * q; /* TODO: Should this be dt * q? */

    /* Release lock */
    return x;
}
