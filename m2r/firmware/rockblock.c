/*
 * RockBLOCK transmission controller
 * M2R
 * 2014 Adam Greig, Cambridge University Spaceflight
 */

#include "rockblock.h"
#include "chprintf.h"

void rockblock_init()
{
  const SerialConfig iridium_port_config = {
    .speed = 19200
  };

  sdStart(&ROCKBLOCK_PORT, &iridium_port_config);
}

void send_sbd_posn(const ublox_pvt_t *pvt)
{
  chprintf((BaseSequentialStream *)&ROCKBLOCK_PORT, "AT+SBDD0\r\n");
  chThdSleepMilliseconds(500);
  chprintf((BaseSequentialStream *)&ROCKBLOCK_PORT,
    "AT+SBDWT=M2R: %02d:%02d:%02d 0x%02X %.3f,%.3f,%.3f %.1f\r\n",
    pvt->hour, pvt->minute, pvt->second,
    pvt->fix_type,
    pvt->lat*1e-7, pvt->lon*1e-7, pvt->height*1e-3,
    -pvt->velD*1e-3
  );
  chThdSleepMilliseconds(500);
  chprintf((BaseSequentialStream *)&ROCKBLOCK_PORT, "AT+SBDI\r\n");
}

