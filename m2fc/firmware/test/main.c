#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

#include "ch.h"
#include "state_estimation.h"
#include "microsd.h"
#include "mission.h"

extern volatile float seconds_to_return;
extern volatile int32_t ch_time_now;

state_t run_state(state_t cur_state, instance_data_t *data);

int main(int argc, char* argv[])
{
    FILE *logfile, *statefile, *sensorfile, *missionfile;
    char msg[16];
    uint32_t last_ts = 0;
    uint64_t last_time = 0, current_time = 0;
    uint64_t time_correction = 0;
    state_t cur_state = STATE_PAD, new_state;
    instance_data_t instance_data;
    instance_data.t_launch = -1;
    instance_data.t_apogee = -1;

    if(argc != 5) {
        printf("Usage: %s <logfile> <state outfile> <sensor outfile>"
               " <mission outfile>\n", argv[0]);
        return 1;
    }

    logfile = fopen(argv[1], "rb");
    statefile = fopen(argv[2], "w");
    sensorfile = fopen(argv[3], "w");
    missionfile = fopen(argv[4], "w");

    microsd_init_files(statefile, sensorfile);

    state_estimation_init();
    state_estimation_trust_barometer = 1;

    while(fread(msg, 1, 16, logfile)) {
        uint32_t *ts;
        uint8_t *channel, *mode;
        int32_t *pressure, *temperature;
        int16_t *x, *y, *z;
        float accel, tf;
        state_estimate_t state;

        ts = (uint32_t*)&msg[0];
        channel = (uint8_t*)&msg[7];
        mode = (uint8_t*)&msg[6];

        (void)mode;

        if((uint64_t)*ts < last_ts) {
            time_correction += 0xFFFFFFFF;
        }
        last_ts = *ts;
        current_time = (uint64_t)*ts + time_correction;
        tf = (float)current_time / 168E6f;

        if(current_time > 15e10) {
            fclose(logfile);
            fclose(statefile);
            fclose(sensorfile);
            fclose(missionfile);
            printf("Log closed at t=%f\n", (float)current_time/168E6f);
            return 0;
        }

        microsd_set_time(tf);
        chSetTimeNow((int32_t)(tf * 1000.0f));

        //if(current_time - last_time > 168000) {
        if(*channel == 0xD0) {
            // Update time that time_seconds_since will return
            seconds_to_return = (float)(current_time - last_time) / 168E6f;
            printf("Running new prediction after %f seconds\n",
                   seconds_to_return);

            // Run prediction step
            state = state_estimation_get_state();
            printf("Got new predicted state %f %f %f\n",
                   state.h, state.v, state.a);
            
            // Run mission state machine
            if(tf > 30.0f) {
                instance_data.state = state;
                new_state = run_state(cur_state, &instance_data);
                if(new_state != cur_state) {
                    fprintf(missionfile, "%f %d\n", tf, new_state);
                    cur_state = new_state;
                }
            }
        }

        last_time = current_time;

        if(*channel == 0x10) {
            x = (int16_t*)&msg[8];
            y = (int16_t*)&msg[10];
            z = (int16_t*)&msg[12];

            printf("\n    ACCELS %"PRId16" %"PRId16" %"PRId16"\n\n",
                   *x, *y, *z);

            (void)x;
            (void)z;

            accel = -(float)*y * 0.03846f - 9.80665f;

            // Submit new LG Accel reading
            printf("Updating with new LG accel %f\n", accel);
            state_estimation_new_lg_accel(accel);

        } else if(*channel == 0x20) {
        } else if(*channel == 0x30) {
            pressure = (int32_t*)&msg[8];
            temperature = (int32_t*)&msg[12];

            (void)temperature;

            // Submit new pressure reading
            printf("Updating with new pressure reading %f\n", (float)*pressure);
            state_estimation_new_pressure((float)*pressure);
        }
    }

    printf("Log closed at t=%f\n", (float)current_time/168E6f);

    fclose(logfile);
    fclose(sensorfile);
    fclose(statefile);
    fclose(missionfile);

    return 0;
}
