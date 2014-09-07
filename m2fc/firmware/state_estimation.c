/*
 * State estimation and sensor fusion
 * M2FC
 * 2014 Adam Greig, Cambridge University Spaceflight
 */

#include <math.h>
#include "state_estimation.h"
#include "microsd.h"
#include "time_utils.h"

/* Kalman filter state and covariance storage */
static float x[3]    = { 0.0f, 0.0f, 0.0f};
static float p[3][3] = {{0.0f, 0.0f, 0.0f},
                        {0.0f, 0.0f, 0.0f},
                        {0.0f, 0.0f, 0.0f}};
static uint32_t t_clk = 0;

/* Locks to protect the global shared Kalman state */
static BinarySemaphore kalman_lock;

/* Constants from the US Standard Atmosphere 1976 */
const float Rs = 8.31432f;
const float g0 = 9.80665f;
const float M = 0.0289644f;
const float Lb[7] = {
    -0.0065f, 0.0f, 0.001, 0.0028f, 0.0f, -0.0028f, -0.002f};
const float Pb[7] = {
    101325.0f, 22632.10f, 5474.89f, 868.02f, 110.91f, 66.94f, 3.96f};
const float Tb[7] = {
    288.15f, 216.65, 216.65, 228.65, 270.65, 270.65, 214.65};
const float Hb[7] = {
    0.0f, 11000.0f, 20000.0f, 32000.0f, 47000.0f, 51000.0f, 71000.0f};

volatile uint8_t state_estimation_trust_barometer = 1;

void state_estimation_update_accel(float accel, float r);
float state_estimation_pressure_to_altitude(float pressure);
float state_estimation_p2a_nonzero_lapse(float p, int b);
float state_estimation_p2a_zero_lapse(float p, int b);

void state_estimation_new_pressure(float pressure)
{
    float y, r, s_inv, k[3];
    float res = 6.5f;
    float h = state_estimation_pressure_to_altitude(pressure);
    float hd = state_estimation_pressure_to_altitude(pressure + res);

    /* If there was an error (couldn't find suitable altitude band) for this
     * pressure, just don't use it. It's probably wrong. */
    if(h == -9999.0f || hd == -9999.0f)
        return;

    /* TODO: validate choice of r */
    r = (h - hd) * (h - hd);

    /* Discard data when mission control believes we are transonic. */
    if(!state_estimation_trust_barometer)
        return;

    /* Acquire lock */
    chBSemWait(&kalman_lock);

    y = h - x[0];

    s_inv = 1.0f / (p[0][0] + r);

    k[0] = p[0][0] * s_inv;
    k[1] = p[1][0] * s_inv;
    k[2] = p[2][0] * s_inv;

    x[0] += k[0] * y;

    microsd_log_f(0xD2, &pressure, &x[0]);

    p[0][0] -= k[0] * p[0][0];
    p[0][1] -= k[0] * p[0][1];
    p[0][2] -= k[0] * p[0][2];
    p[1][0] -= k[1] * p[0][0];
    p[1][1] -= k[1] * p[0][1];
    p[1][2] -= k[1] * p[0][2];
    p[2][0] -= k[2] * p[0][0];
    p[2][1] -= k[2] * p[0][1];
    p[2][2] -= k[2] * p[0][2];

    /* Release lock */
    chBSemSignal(&kalman_lock);
}

float state_estimation_pressure_to_altitude(float pressure)
{
    int b;
    for(b = 0; b < 6; b++) {
        if(pressure <= Pb[b] && pressure > Pb[b+1]) {
            if(Lb[b] == 0.0f) {
                return state_estimation_p2a_zero_lapse(pressure, b);
            } else {
                return state_estimation_p2a_nonzero_lapse(pressure, b);
            }
        }
    }

    return -9999.0f;
}

/*
 * Convert a pressure and an atmospheric level b into an altitude.
 * Reverses the standard equation for non-zero lapse regions,
 * P = Pb (Tb / (Tb + Lb(h - hb)))^(M g0 / R* Lb)
 */
float state_estimation_p2a_nonzero_lapse(float pressure, int b)
{
    float lb = Lb[b];
    float hb = Hb[b];
    float pb = Pb[b];
    float tb = Tb[b];

    return hb + tb/lb * (powf(pressure/pb, (-Rs*lb)/(g0*M)) - 1.0f);
}

/* Convert a pressure and an atmospheric level b into an altitude.
 * Reverses the standard equation for zero-lapse regions,
 * P = Pb exp( -g0 M (h-hb) / R* Tb)
 */
float state_estimation_p2a_zero_lapse(float pressure, int b)
{
    float hb = Hb[b];
    float pb = Pb[b];
    float tb = Tb[b];

    return hb + (Rs * tb)/(g0 * M) * (logf(pressure) - logf(pb));
}

void state_estimation_new_lg_accel(float lg_accel)
{
    if(fabsf(lg_accel) > 150.0f) {
        /* The low-g accelerometer is limited to +-16g, so
         * we'll just discard anything above 150m/s (15.3g).
         */
        return;
    }

    /* TODO: validate choice of r */
    state_estimation_update_accel(lg_accel, 0.1385f);
}

/*
 * Run the Kalman update step for a new observation of the acceleration.
 */
void state_estimation_new_hg_accel(float hg_accel)
{
    /* TODO: validate choice of r */
    state_estimation_update_accel(hg_accel, 0.9617f);
}

/* Run the Kalman update for a single acceleration value.
 * Called internally from the hg and lg functions after
 * preprocessing.
 */
void state_estimation_update_accel(float a, float r)
{
    float y, s_inv, k[3];

    /* Acquire lock */
    chBSemWait(&kalman_lock);

    y = a - x[2];

    s_inv = 1.0f / (p[2][2] + r);

    k[0] = p[0][2] * s_inv;
    k[1] = p[1][2] * s_inv;
    k[2] = p[2][2] * s_inv;

    x[2] += k[2] * y;
    microsd_log_f(0xD3, &a, &x[2]);

    p[0][0] -= k[0] * p[2][0];
    p[0][1] -= k[0] * p[2][1];
    p[0][2] -= k[0] * p[2][2];
    p[1][0] -= k[1] * p[2][0];
    p[1][1] -= k[1] * p[2][1];
    p[1][2] -= k[1] * p[2][2];
    p[2][0] -= k[2] * p[2][0];
    p[2][1] -= k[2] * p[2][1];
    p[2][2] -= k[2] * p[2][2];

    /* Release lock */
    chBSemSignal(&kalman_lock);
}

/*
 * Run the Kalman prediction step and return the latest state estimate.
 */
state_estimate_t state_estimation_get_state()
{
    float q, dt;
    state_estimate_t x_out;

    /* TODO Determine this q-value */
    q = 200.0;

    /* Acquire lock */
    chBSemWait(&kalman_lock);

    /* Find elapsed time */
    dt = time_seconds_since(&t_clk);

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
    p[2][2] += dt * q;

    x_out.h = x[0];
    x_out.v = x[1];
    x_out.a = x[2];

    microsd_log_f(0xD0, &x[0], &x[1]);
    microsd_log_f(0xD1, &x[2], &x[2]);
    
    /* Release lock */
    chBSemSignal(&kalman_lock);

    return x_out;
}

void state_estimation_init()
{
    chBSemInit(&kalman_lock, FALSE);
}
