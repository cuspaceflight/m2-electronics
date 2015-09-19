#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <float.h>
#include <math.h>

typedef struct {
    uint32_t timestamp;
    uint8_t metadata;
    uint8_t channel;
    uint16_t checksum;
    uint64_t data;
} Packet __attribute__((aligned));

int main(int argc, char* argv[]) {
    FILE *f;
    FILE *fo;
    size_t fsize;
    size_t npackets;

    double last_t_s = 0.0f;
    double this_t_s;
    double dt;
    double dt_sum;
    double dt_sum_sq;
    double dt_mean;
    double dt_std;
    double dt_min = DBL_MAX;;
    double dt_max = DBL_MIN;
    double t0 = 0.0f;
    double tN;

    int channels[255];
    int i;

    uint64_t this_timestamp = 0;
    uint64_t last_timestamp = 0;
    uint64_t t_correction = 0;

    Packet packet;

    if(argc != 2) {
        printf("Usage: %s <logfile>", argv[0]);
        return 1;
    }

    for(i=0; i<255; i++) channels[i] = 0;

    f = fopen(argv[1], "r");
    fo = fopen("out_times.bin", "w");
    fseek(f, 0, SEEK_END);
    fsize = ftell(f);
    npackets = fsize / 16;
    rewind(f);

    for(i=0; i<npackets; i++) {
        fread(&packet, sizeof(Packet), 1, f);
        if(packet.timestamp < last_timestamp)
            t_correction += 0xFFFFFFFF;
        last_timestamp = packet.timestamp;
        this_timestamp = packet.timestamp + t_correction;
        this_t_s = this_timestamp / 168e6f;
        if(t0 == 0.0f) {
            t0 = this_t_s;
            last_t_s = this_t_s;
        }
        dt = this_t_s - last_t_s;
        fwrite(&dt, sizeof(double), 1, fo);
        last_t_s = this_t_s;
        dt_sum += dt;
        dt_sum_sq += dt*dt;
        if(dt < dt_min && i > 1024) dt_min = dt;
        if(dt > dt_max && i > 1024) dt_max = dt;
        tN = this_t_s;
        channels[packet.channel] += 1;

        if(dt > 0.5f) {
            printf("[%d] more than 0.5s since last packet\n", i);
        }
    }

    dt_mean = dt_sum / (double)npackets;
    dt_std = sqrt(dt_sum_sq / (double)npackets - dt_mean*dt_mean);

#if 0
    printf("Log Analysis Result: Channels\n");
    printf("====================================================\n");

    for(i=0; i<255; i++) {
        printf("    %02X: % 15d\n", i, channels[i]);
    }
#endif

    printf("Log Analysis Result: Timing\n");
    printf("====================================================\n");
    
    printf("Num Packets: %ld\n", npackets);
    printf("Total Time: %.2f sec\n", tN - t0);
    printf("dt:\n");
    printf("    sum:      %.4f  sec\n", dt_sum);
    printf("    mean:     %.4f msec\n", 1000.0f * dt_mean);
    printf("    min:      %.6f msec\n", 1000.0f * dt_min);
    printf("    max:      %.6f msec\n", 1000.0f * dt_max);
    printf("    std.dev.: %.4f msec\n", 1000.0f * dt_std);

    fclose(f);
    fclose(fo);
}
