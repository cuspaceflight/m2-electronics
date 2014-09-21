/*
 * SBP IO
 * 2014 Fergus Noble, Cambridge University Spaceflight
 */

#include "ch.h"
#include "hal.h"

#include "sbp_io.h"

sbp_state_t sbp_state;

u32 m2fc_read(u8 *buff, u32 n, void* context)
{
  (void)context;
  if(n == 0)
      return 0;
  return sdRead(&M2FC_PORT, buff, n);
}

u32 m2fc_write(u8 *buff, u32 n, void* context)
{
  (void)context;
  if(n == 0)
      return 0;
  return sdWrite(&M2FC_PORT, buff, n);
}

msg_t sbp_thread(void* arg)
{
    (void)arg;

    chRegSetThreadName("sbp");

    const SerialConfig m2fc_port_config = {
      .speed = 115200,
    };
    sdStart(&M2FC_PORT, &m2fc_port_config);

    while (TRUE) {
        sbp_process(&sbp_state, &m2fc_read);
    }
}

