/*
 * M2Status
 * Handle storing and relaying system status, both inside and between the
 * avionics units.
 * 
 * 2015 Adam Greig, Cambridge University Spaceflight
 */

#ifndef M2STATUS_H
#define M2STATUS_H

#include <stdint.h>
#include "ch.h"

typedef enum {
    STATUS_UNKNOWN = 0,
    STATUS_WAIT, STATUS_ERR, STATUS_OK,
    STATUS_ERR_INITIALISING, STATUS_ERR_READING, STATUS_ERR_WRITING,
    STATUS_ERR_SENDING, STATUS_ERR_ALLOCATING,
    STATUS_ERR_CALLBACK_WHILE_ACTIVE, STATUS_ERR_SELFTEST_FAIL,
    STATUS_ERR_INVALID_DEVICE_ID, STATUS_ERR_PERIPHERAL, STATUS_ERR_BAD_INPUT,
} Status;

typedef struct {
    Status m2fcbody, m2fcnose, m2r,
           adc, accel, baro, gyro, magno, pyro, microsd,
           stateestimation, missioncontrol, datalogging, config,
           rockblock, radio, gps;
    struct {
        int16_t sg1, sg2, sg3, tc1, tc2, tc3;
        int16_t lga_x, lga_y, lga_z, hga_x, hga_y, hga_z;
        int32_t baro_p, baro_t;
        int16_t gyro_x, gyro_y, gyro_z;
        int16_t magno_x, magno_y, magno_z;
        float se_dt, se_h, se_v, se_a, se_p_s, se_p_e, se_a_p, se_a_e;
        int32_t mc_state;
        int16_t pyro_c_1, pyro_c_2, pyro_c_3;
        int16_t pyro_f_1, pyro_f_2, pyro_f_3;
        int16_t battery;
        uint8_t gps_t_year_l, gps_t_year_h, gps_t_month, gps_t_day;
        uint8_t gps_t_hour, gps_t_min, gps_t_sec, gps_t_valid;
        int32_t gps_lat, gps_lng, gps_alt, gps_alt_msl;
        int8_t gps_fix_type, gps_flags, gps_num_sv;
        uint8_t cpu_usage, microsd_duty;
    } latest;
    struct {
        uint32_t sg, tc, lga, hga, baro, gyro, magno,
                 se_predict, se_pressure, se_accel, mc,
                 pyro_c, pyro_f, battery, gps_t, gps_p, gps_a, gps_s, stat;
    } latest_timestamps;
} SystemStatus;

/* Store what we know of all the systems in the rocket. */
extern SystemStatus M2FCBodyStatus;
extern SystemStatus M2FCNoseStatus;
extern SystemStatus M2RStatus;

/* Pointer to whichever of the SystemStatus represents "us" */
extern SystemStatus* LocalStatus;

/* Generate M2Telem packets from the current latest states. */
msg_t m2status_thread(void* arg);

/* Update our knowledge of the world by processing a received packet. */
void m2status_rx_packet(TelemPacket packet);

/* Update our knowledge of our own latest readings. */
void m2status_set_sg(int16_t sg1, int16_t sg3, int16_t sg3);
void m2status_set_tc(int16_t tc1, int16_t tc2, int16_t tc3);
void m2status_set_lga(int16_t x, int16_t y, int16_t z);
void m2status_set_hga(int16_t x, int16_t y, int16_t z);
void m2status_set_baro(int32_t p, int32_t t);
void m2status_set_gyro(int16_t x, int16_t y, int16_t z);
void m2status_set_magno(int16_t x, int16_t y, int16_t z);
void m2status_set_se_pred(float dt, float h, float v, float a);
void m2status_set_se_pressure(float sensor, float estimate);
void m2status_set_se_accel(float sensor, float estimate);
void m2status_set_mc(int32_t state);
void m2status_set_pyro_c(int16_t py1, int16_t py2, int16_t py3);
void m2status_set_pyro_f(int16_t py1, int16_t py2, int16_t py3);
void m2status_set_battery(int16_t battery_mv);
void m2status_set_gps_time(uint8_t year_l, uint8_t year_h, uint8_t month,
                           uint8_t day, uint8_t hour, uint8_t min, uint8_t sec,
                           uint8_t valid);
void m2status_set_gps_pos(int32_t lat, int32_t lng);
void m2status_set_gps_alt(int32_t alt, int32_t alt_msl);
void m2status_set_gps_status(int8_t fix_type, int8_t flags, int8_t num_sv);

#endif /* M2STATUS_H */
