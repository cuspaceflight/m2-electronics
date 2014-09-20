/*
 * Mission configuration file
 * M2FC
 * 2014 Adam Greig, Cambridge University Spaceflight
 */

#include "config.h"
#include "hal.h"

config_avionics_t m2fc_location;

void config_read_location()
{
    if(palReadPad(GPIOD, GPIOD_SJ1) == PAL_HIGH)
        m2fc_location = M2FC_NOSE;
    else
        m2fc_location = M2FC_BODY;
}
