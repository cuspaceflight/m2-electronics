/*
 * Grand Central Dispatch
 * M2R
 * 2014 Adam Greig, Cambridge University Spaceflight
 */

#include <math.h>
#include <string.h>
#include "ch.h"
#include "chprintf.h"
#include "dispatch.h"
#include "audio_data.h"
volatile m2r_state_t m2r_state;
#include "rockblock.h"
#include "sbp_io.h"

#include "../../m2fc/firmware/state_estimation.h"

void dispatch_pvt(const ublox_pvt_t pvt)
{
  static systime_t last_sbd = 0;

    m2r_state.hour = pvt.hour;
    m2r_state.minute = pvt.minute;
    m2r_state.second = pvt.second;
    m2r_state.gps_valid = pvt.fix_type;
    m2r_state.gps_num_sats = pvt.num_sv;
    m2r_state.lat = pvt.lat;
    m2r_state.lng = pvt.lon;
    m2r_state.gps_height = pvt.height;

  if (chTimeNow() - last_sbd > 60000) {
    /* Only send an Iridium SBD message every 60s */
    send_sbd_posn(&pvt);
    last_sbd = chTimeNow();
  }
}

#define LEN(x) (sizeof(x)/sizeof(x[0]))
uint8_t * const number[10] = {
  zero, one, two, three, four, five, six, seven, eight, nine
};
uint16_t const number_lens[10] = {
  LEN(zero), LEN(one), LEN(two), LEN(three), LEN(four),
  LEN(five), LEN(six), LEN(seven), LEN(eight), LEN(nine)
};

/* alt is in meters AMSL */
void say_altitude(float alt, uint8_t **samples, uint16_t *lens)
{
    int32_t kft_agl = lroundf((3.2808399f * alt - 4000.0f) / 1000.0f);

    if (kft_agl <= 0) {
        /* below 1000 ft, don't report altitude */
        samples[0] = 0;
        return;
    }

    char buff[4] = {0};
    chsnprintf(buff, 4, "%-3d", kft_agl);

    uint8_t i = 0;
    while (buff[i] && buff[i] != ' ') {
        samples[i] = number[buff[i] - '0'];
        lens[i] = number_lens[buff[i] - '0'];
        i++;
    }
    samples[i] = kft;
    lens[i++] = LEN(kft);
    samples[i] = 0;
    lens[i] = 0;
}

void est_state_callback(u16 sender_id, u8 len, u8 msg[], void *context)
{
  (void)sender_id; (void)context;

  m2r_state.armed = true;

  state_estimate_t est;
  memcpy(&est, msg, len);

  m2r_state.imu_height = est.h;
  m2r_state.imu_velocity = est.v;

  if (m2r_state.imu_height > m2r_state.imu_max_height) {
    m2r_state.imu_max_height = m2r_state.imu_height;
  }
  if (m2r_state.imu_velocity > m2r_state.imu_max_velocity) {
    m2r_state.imu_max_velocity = m2r_state.imu_velocity;
  }
}

void m2fc_mission_callback(u16 sender_id, u8 len, u8 msg[], void *ctx)
{
    (void)sender_id; (void)ctx;
    m2r_state.fc_state = msg[0];
    if(msg[0] == 4) {
        radio_say(drogue, sizeof(drogue));
    } else if(msg[0] == 6) {
        radio_say(main_chute, sizeof(main_chute));
    }
}

void dispatch_init()
{
  static sbp_msg_callbacks_node_t est_state_cb;
  static sbp_msg_callbacks_node_t m2fc_mission_cb;
  m2r_state.armed = false;
  m2r_state.hour = 0;
  m2r_state.minute = 0;
  m2r_state.second = 0;
  m2r_state.gps_valid = 0;
  m2r_state.gps_num_sats = 0;
  m2r_state.lat = 0;
  m2r_state.lng = 0;
  m2r_state.gps_height = 0;
  m2r_state.imu_height = 0.0f;
  m2r_state.imu_max_height = 0.0f;
  m2r_state.imu_velocity = 0.0f;
  m2r_state.imu_max_velocity = 0.0f;
  m2r_state.fc_state = 0;
  sbp_register_callback(&sbp_state, 0x22, &est_state_callback, 0, &est_state_cb);
  sbp_register_callback(&sbp_state, 0x30, &m2fc_mission_callback, 0, &m2fc_mission_cb);
}

msg_t dispatch_thread(void* arg)
{
    (void)arg;
    while(TRUE)
        chThdSleepMilliseconds(1000);
}


