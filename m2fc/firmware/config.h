/*
 * Configuration access and parsing.
 * M2FC / Badger 3
 * 2015 Chi Pham, Cambridge University Spaceflight
 * 2015 Adam Greig, Cambridge University Spaceflight
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <stdbool.h>
#include "ch.h"

#define CFG_M2FC_NOSE 0
#define CFG_M2FC_BODY 1

#define CFG_PYRO_UNUSED 0
#define CFG_PYRO_DROGUE 1
#define CFG_PYRO_MAIN   2

/* Exposes functions for reading config file from sd card.
 * Usage: config MUST be initialised with config_init, then macros can be used
 * to access config.
 *
 * NOTE: to add a config option, you have to add it the following places:
 *   - in the config_t type defined in here
 *   - in the read_config function of config.c
 */

/* ------------------------------------------------------------------------- */

/* Read config from the file on the sd card specified by <path>.
 * If none/error occurred, return false, if succeeded return true.
 */
bool config_init(void);

/* Sanity check the loaded config.
 */
bool check_config(void);

//-----------------------------------------------------------------------------

/* See docs/config.md for descriptions */
typedef struct config_ {
    char version[8];
    bool config_loaded;
    unsigned int location;
    unsigned int accel_axis;
    unsigned int pyro_firetime, pyro_1, pyro_2, pyro_3;
    unsigned int ignition_accel;
    unsigned int burnout_time;
    unsigned int apogee_time;
    unsigned int main_altitude, main_time;
    unsigned int landing_time;
    bool use_adc, use_magno, use_gyro;
} config_t;

/* This is the global configuration that can be accessed from any file.
 * In practice, it should ONLY be written to from config.c.
 * Use the macros above to get the config values.
 */
extern config_t conf;

msg_t config_thread(void* arg);

#endif  /* CONFIG_H */
