/*
 * Grand Central Dispatch
 * M2R
 * 2014 Adam Greig, Cambridge University Spaceflight
 */

#ifndef DISPATCH_H
#define DISPATCH_H

#include "ch.h"
#include "ublox.h"

typedef struct {
    uint8_t hour, minute, second;
    uint8_t gps_valid, gps_num_sats;
    int32_t lat, lng;
    int32_t gps_height;
    float imu_height, imu_max_height;
    float imu_velocity, imu_max_velocity;
} m2r_state_t;

extern volatile m2r_state_t m2r_state;

void dispatch_pvt(const ublox_pvt_t pvt);
msg_t dispatch_thread(void* arg);

void say_altitude(float alt, uint8_t **samples, uint16_t *lens);

#endif /* DISPATCH_H */
