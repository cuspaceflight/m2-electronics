/*
 * RockBLOCK transmission controller
 * M2R
 * 2014 Adam Greig, Cambridge University Spaceflight
 */

#include "rockblock.h"
#include "chprintf.h"
#include "dispatch.h"
#include "m2status.h"

#include <string.h>

void rockblock_init()
{
  const SerialConfig iridium_port_config = {
    .speed = 19200
  };

  sdStart(&ROCKBLOCK_PORT, &iridium_port_config);
  sdWrite(&SD1, (uint8_t *)"AT\r\n", 4);

  m2status_rockblock_status(STATUS_OK);
}

void send_sbd_posn()
{

    char txbuf[64];
    char rxbuf[64];
    int n;

    chsnprintf(txbuf, 64, "AT+SBDD0\r\n");
    sdWrite(&SD1, (uint8_t*)txbuf, strlen(txbuf));
    memset(rxbuf, 0, 64);
    n = sdAsynchronousRead(&ROCKBLOCK_PORT, (uint8_t*)rxbuf, 64);
    rxbuf[n] = 0;
    /*printf("%s", rxbuf);*/
    chThdSleepMilliseconds(2000);
    memset(rxbuf, 0, 64);
    n = sdAsynchronousRead(&ROCKBLOCK_PORT, (uint8_t*)rxbuf, 64);
    rxbuf[n] = 0;
    /*printf("%s", rxbuf);*/


    chsnprintf((char*)txbuf, 64,
            "AT+SBDWT=%02d:%02d:%02d %02X %.3f,%.3f,%d\r\n",
            M2RStatus.latest.gps_t_hour,
            M2RStatus.latest.gps_t_min,
            M2RStatus.latest.gps_t_sec,
            M2RStatus.latest.gps_fix_type,
            (double)M2RStatus.latest.gps_lat * (double)1e-7,
            (double)M2RStatus.latest.gps_lng * (double)1e-7,
            M2RStatus.latest.gps_alt);
    sdWrite(&ROCKBLOCK_PORT, (uint8_t*)txbuf, strlen(txbuf));
    memset(rxbuf, 0, 64);
    n = sdAsynchronousRead(&ROCKBLOCK_PORT, (uint8_t*)rxbuf, 64);
    rxbuf[n] = 0;
    /*printf("%s", rxbuf);*/
    chThdSleepMilliseconds(2000);
    memset(rxbuf, 0, 64);
    n = sdAsynchronousRead(&ROCKBLOCK_PORT, (uint8_t*)rxbuf, 64);
    rxbuf[n] = 0;
    /*printf("%s", rxbuf);*/


    chsnprintf((char*)txbuf, 64, "AT-MSSTM\r\n");
    sdWrite(&ROCKBLOCK_PORT, (uint8_t*)txbuf, strlen(txbuf));
    memset(rxbuf, 0, 64);
    n = sdAsynchronousRead(&ROCKBLOCK_PORT, (uint8_t*)rxbuf, 64);
    rxbuf[n] = 0;
    /*printf("%s", rxbuf);*/
    chThdSleepMilliseconds(2000);
    memset(rxbuf, 0, 64);
    n = sdAsynchronousRead(&ROCKBLOCK_PORT, (uint8_t*)rxbuf, 64);
    rxbuf[n] = 0;
    /*printf("%s", rxbuf);*/

    chsnprintf((char*)txbuf, 64, "AT+SBDIX\r\n");
    sdWrite(&ROCKBLOCK_PORT, (uint8_t*)txbuf, strlen(txbuf));
    memset(rxbuf, 0, 64);
    n = sdAsynchronousRead(&ROCKBLOCK_PORT, (uint8_t*)rxbuf, 64);
    rxbuf[n] = 0;
    /*printf("%s", rxbuf);*/
    chThdSleepMilliseconds(5000);
    memset(rxbuf, 0, 64);
    n = sdAsynchronousRead(&ROCKBLOCK_PORT, (uint8_t*)rxbuf, 64);
    rxbuf[n] = 0;
    /*printf("%s", rxbuf);*/

#if 0
    char buf[64];
    chprintf((BaseSequentialStream *)&ROCKBLOCK_PORT, "AT+SBDD0\r\n");
    memset(buf, 0, 64);
    sdAsynchronousRead(&ROCKBLOCK_PORT, buf, 64);
    chThdSleepMilliseconds(500);
    memset(buf, 0, 64);
    sdAsynchronousRead(&ROCKBLOCK_PORT, buf, 64);

    chprintf((BaseSequentialStream *)&ROCKBLOCK_PORT,
            "AT+SBDWT=%02d:%02d:%02d %02X %.3f,%.3f,%d\r\n",
            M2RStatus.latest.gps_t_hour,
            M2RStatus.latest.gps_t_min,
            M2RStatus.latest.gps_t_sec,
            M2RStatus.latest.gps_fix_type,
            (double)M2RStatus.latest.gps_lat * (double)1e-7,
            (double)M2RStatus.latest.gps_lng * (double)1e-7,
            M2RStatus.latest.gps_alt);
    memset(buf, 0, 64);
    sdAsynchronousRead(&ROCKBLOCK_PORT, buf, 64);
    chThdSleepMilliseconds(500);
    memset(buf, 0, 64);
    sdAsynchronousRead(&ROCKBLOCK_PORT, buf, 64);

    chprintf((BaseSequentialStream *)&ROCKBLOCK_PORT, "AT-MSSTM\r\n");
    memset(buf, 0, 64);
    sdAsynchronousRead(&ROCKBLOCK_PORT, buf, 64);
    chThdSleepMilliseconds(500);
    memset(buf, 0, 64);
    sdAsynchronousRead(&ROCKBLOCK_PORT, buf, 64);

    chprintf((BaseSequentialStream *)&ROCKBLOCK_PORT, "AT+SBDIX\r\n");
    memset(buf, 0, 64);
    sdAsynchronousRead(&ROCKBLOCK_PORT, buf, 64);
    chThdSleepMilliseconds(500);
    memset(buf, 0, 64);
    sdAsynchronousRead(&ROCKBLOCK_PORT, buf, 64);
#endif
}

