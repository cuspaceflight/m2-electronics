#include "microsd.h"
#include <stdio.h>
#include <stdlib.h>

static FILE *sensorfile;
static FILE *statefile;
static float time;

static float last_dt, last_height;

float state_estimation_pressure_to_altitude(float pressure);

void microsd_init_files(FILE* state, FILE* sensor) {
    sensorfile = sensor;
    statefile = state;
}

void microsd_set_time(float t) {
    time = t;
}


void microsd_log_f(uint8_t channel, float* a, float* b)
{
    printf("LOG %x %f %f\n", channel, *a, *b);

    if(channel == 0xD0) {
        last_dt = *a;
        last_height = *b;
    } else if(channel == 0xD1) {
        float velo = *a;
        float accel = *b;

        fprintf(statefile, "%f %f %f %f %f\n", time, last_dt, last_height,
                velo, accel);
    } else if(channel == 0xD2) {
        float height = state_estimation_pressure_to_altitude(*a);
        fprintf(sensorfile, "%f %f nan\n", time, height);
    } else if(channel == 0xD3) {
        fprintf(sensorfile, "%f nan %f\n", time, *a);
    }
}

void microsd_log_s32(uint8_t channel, int32_t *a, int32_t *b)
{
    (void)channel;
    (void)a;
    (void)b;
}
