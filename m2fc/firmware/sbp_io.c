/*
 * SBP IO
 * 2014 Fergus Noble, Cambridge University Spaceflight
 */

#include "ch.h"
#include "hal.h"

#include "sbp_io.h"

sbp_state_t sbp_state;

u32 m2r_read(u8 *buff, u32 n, void* context)
{
  (void)context;
  if(n == 0)
      return 0;
  return sdRead(&M2R_PORT, buff, n);
}

u32 m2r_write(u8 *buff, u32 n, void* context)
{
  (void)context;
  if(n == 0)
      return 0;
  return sdWrite(&M2R_PORT, buff, n);
}

msg_t sbp_thread(void* arg)
{
    (void)arg;

    chRegSetThreadName("sbp");

    sbp_state_init(&sbp_state);

    const SerialConfig m2r_port_config = {
      .speed = 115200,
    };
    sdStart(&M2R_PORT, &m2r_port_config);

    while (TRUE) {
        sbp_process(&sbp_state, &m2r_read);
    }
}


