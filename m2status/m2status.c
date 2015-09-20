#include "m2status.h"

SystemStatus M2FCBodyStatus = {0};
SystemStatus M2FCNoseStatus = {0};
SystemStatus M2RStatus = {0};

SystemStatus* LocalStatus = NULL;

msg_t m2status_thread(void* arg)
{
    (void)arg;
}

void m2status_rx_packet(TelemPacket packet)
{
    (void)packet;
}

void m2status_set_sg(int16_t sg1, int16_t sg3, int16_t sg3)
{
    LocalStatus->latest_timestamps.sg = halGetCounterValue();
    LocalStatus->latest.sg1 = sg1;
    LocalStatus->latest.sg2 = sg2;
    LocalStatus->latest.sg3 = sg3;
}

void m2status_set_tc(int16_t tc1, int16_t tc2, int16_t tc3)
{
    LocalStatus->latest_timestamps.tc = halGetCounterValue();
    LocalStatus->latest.tc1 = tc1;
    LocalStatus->latest.tc2 = tc2;
    LocalStatus->latest.tc3 = tc3;
}

void m2status_set_lga(int16_t x, int16_t y, int16_t z)
{
    LocalStatus->latest_timestamps.lga = halGetCounterValue();
    LocalStatus->latest.lga_x = x;
    LocalStatus->latest.lga_y = y;
    LocalStatus->latest.lga_z = z;
}

void m2status_set_hga(int16_t x, int16_t y, int16_t z)
{
    LocalStatus->latest_timestamps.hga = halGetCounterValue();
    LocalStatus->latest.hga_x = x;
    LocalStatus->latest.hga_y = y;
    LocalStatus->latest.hga_z = z;
}

void m2status_set_baro(int32_t p, int32_t t)
{
    LocalStatus->latest_timestamps.baro = halGetCounterValue();
    LocalStatus->latest.baro_p = p;
    LocalStatus->latest.baro_t = t;
}

void m2status_set_gyro(int16_t x, int16_t y, int16_t z)
{
    LocalStatus->latest_timestamps.gyro = halGetCounterValue();
    LocalStatus->latest.gyro_x = x;
    LocalStatus->latest.gyro_y = y;
    LocalStatus->latest.gyro_z = z;
}

void m2status_set_magno(int16_t x, int16_t y, int16_t z)
{
    LocalStatus->latest_timestamps.magno = halGetCounterValue();
    LocalStatus->latest.magno_x = x;
    LocalStatus->latest.magno_y = y;
    LocalStatus->latest.magno_z = z;
}

void m2status_set_se_pred(float dt, float h, float v, float a)
{
    LocalStatus->latest_timestamps.se_predict = halGetCounterValue();
    LocalStatus->latest.se_dt = t;
    LocalStatus->latest.se_h = h;
    LocalStatus->latest.se_v = v;
    LocalStatus->latest.se_a = a;
}

void m2status_set_se_pressure(float sensor, float estimate)
{
    LocalStatus->latest_timestamps.se_pressure = halGetCounterValue();
    LocalStatus->latest.se_p_s = sensor;
    LocalStatus->latest.se_p_e = estimate;
}

void m2status_set_se_accel(float sensor, float estimate)
{
    LocalStatus->latest_timestamps.se_accel = halGetCounterValue();
    LocalStatus->latest.se_a_s = sensor;
    LocalStatus->latest.se_a_e = estimate;
}

void m2status_set_mc(int32_t state)
{
    LocalStatus->latest_timestamps.mc = halGetCounterValue();
    LocalStatus->latest.mc_state = state;
}

void m2status_set_pyro_c(int16_t py1, int16_t py2, int16_t py3)
{
    LocalStatus->latest_timestamps.pyro_c = halGetCounterValue();
    LocalStatus->latest.pyro_c_1 = py1;
    LocalStatus->latest.pyro_c_2 = py2;
    LocalStatus->latest.pyro_c_3 = py3;
}

void m2status_set_pyro_f(int16_t py1, int16_t py2, int16_t py3)
{
    LocalStatus->latest_timestamps.pyro_f = halGetCounterValue();
    LocalStatus->latest.pyro_f_1 = py1;
    LocalStatus->latest.pyro_f_2 = py2;
    LocalStatus->latest.pyro_f_3 = py3;
}

void m2status_set_battery(int16_t battery_mv)
{
    LocalStatus->latest_timestamps.battery = halGetCounterValue();
    LocalStatus->latest.battery = battery_mv;
}

void m2status_set_gps_time(uint8_t year_l, uint8_t year_h, uint8_t month,
                           uint8_t day, uint8_t hour, uint8_t min, uint8_t sec,
                           uint8_t valid)
{
    LocalStatus->latest_timestamps.gps_t = halGetCounterValue();
    LocalStatus->latest.gps_t_year_l = year_l;
    LocalStatus->latest.gps_t_year_h = year_h;
    LocalStatus->latest.gps_t_month = month;
    LocalStatus->latest.gps_t_day = day;
    LocalStatus->latest.gps_t_hour = hour;
    LocalStatus->latest.gps_t_min = min;
    LocalStatus->latest.gps_t_sec = sec;
    LocalStatus->latest.gps_t_valid = valid;
}

void m2status_set_gps_pos(int32_t lat, int32_t lng)
{
    LocalStatus->latest_timestamps.gps_p = halGetCounterValue();
    LocalStatus->latest.gps_lat = lat;
    LocalStatus->latest.gps_lng = lng;
}

void m2status_set_gps_alt(int32_t alt, int32_t alt_msl)
{
    LocalStatus->latest_timestamps.gps_a = halGetCounterValue();
    LocalStatus->latest.gps_alt = alt;
    LocalStatus->latest.gps_alt_msl = alt_msl;
}

void m2status_set_gps_status(int8_t fix_type, int8_t flags, int8_t num_sv)
{
    LocalStatus->latest_timestamps.gps_s = halGetCounterValue();
    LocalStatus->latest.gps_fix_type = fix_type;
    LocalStatus->latest.gps_flags = flags;
    LocalStatus->latest.gps_num_sv = num_sv;
}

