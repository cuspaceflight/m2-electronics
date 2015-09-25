/*
 * RockBLOCK transmission controller
 * M2R
 * 2014 Adam Greig, Cambridge University Spaceflight
 */

#include "rockblock.h"
#include "chprintf.h"
#include "dispatch.h"
#include "m2status.h"

void rockblock_init()
{
  const SerialConfig iridium_port_config = {
    .speed = 19200
  };

  sdStart(&ROCKBLOCK_PORT, &iridium_port_config);

  m2status_rockblock_status(STATUS_OK);
}

void send_sbd_posn()
{
  if(!m2r_state.armed)
      return;

  chprintf((BaseSequentialStream *)&ROCKBLOCK_PORT, "AT-MSSTM\r\n");
  chThdSleepMilliseconds(500);
  chprintf((BaseSequentialStream *)&ROCKBLOCK_PORT, "AT+SBDD0\r\n");
  chThdSleepMilliseconds(500);
  chprintf((BaseSequentialStream *)&ROCKBLOCK_PORT,
    "AT+SBDWT=%02d%02d%02d %02X %.3f,%.3f,%d\r\n",
    M2RStatus.latest.gps_t_hour,
    M2RStatus.latest.gps_t_min,
    M2RStatus.latest.gps_t_sec,
    M2RStatus.latest.gps_fix_type,
    (double)M2RStatus.latest.gps_lat * (double)1e-7,
    (double)M2RStatus.latest.gps_lng * (double)1e-7,
    M2RStatus.latest.gps_alt);
  chThdSleepMilliseconds(500);
  chprintf((BaseSequentialStream *)&ROCKBLOCK_PORT, "AT+SBDI\r\n");
}

