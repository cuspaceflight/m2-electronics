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
  if (chTimeNow() - last_sbd > 30000) {
    /* Only send an Iridium SBD message every 30s */
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

void dispatch_init()
{
  static sbp_msg_callback_t est_state_cb;
  sbp_register_callback(&sbp_state, 0x22, &est_state_callback, 0, &est_state_cb);
}

msg_t dispatch_thread(void* arg)
{
    (void)arg;
    while(TRUE)
        chThdSleepMilliseconds(1000);
}


