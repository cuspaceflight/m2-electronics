#include <stdbool.h>
#include <stdio.h>
#include "hal.h"
#include "microsd.h"
#include "config.h"

/* ------------------------------------------------------------------------- */

#define BUFFER_SIZE 128 // estimated max line length in config

static bool read_int(SDFILE* file, const char* name, unsigned int* attribute);
static bool read_float(SDFILE* file, const char* name, float* attribute);
static bool read_bool(SDFILE* file, const char* name, bool* attribute);

/* ------------------------------------------------------------------------- */

#ifndef M2FC_FIRMWARE_VERSION
#define M2FC_FIRMWARE_VERSION "UNKNOWN "
#endif

/* External config. Some sane defaults are provided. */
config_t conf = {
    .version = M2FC_FIRMWARE_VERSION, .config_loaded = 0,
    .location = CFG_M2FC_NOSE, .accel_axis = 3, .pyro_firetime = 5000,
    .pyro_1 = CFG_PYRO_DROGUE, .pyro_2 = CFG_PYRO_DROGUE,
    .pyro_3 = CFG_PYRO_DROGUE, .ignition_accel = 30,
    .burnout_time = 5200, .apogee_time = 55000, .main_altitude = 1450,
    .main_time = 30000, .landing_time = 300000, .read_analogue = false
};

/* ------------------------------------------------------------------------- */

/* read an integer attribute with config name <name> into <attribute>.
 * sscanf version:
 * char format[BUFFER_SIZE];
 * sprintf(format, "%s=%%lf", name);
 * sscanf(buffer, format, attribute) which returns 1 if succesful
 */
static bool read_int(SDFILE* file, const char* name, unsigned int* attribute)
{
    char buffer[BUFFER_SIZE];
    char format[BUFFER_SIZE];
    SDRESULT status;

    status = microsd_gets(file, buffer, BUFFER_SIZE);
    if (status != FR_OK) return false;

    sprintf(format, "%s=%%u", name);
    return sscanf(buffer, format, attribute) == 1;
}

/* read a float attribute with config name <name> into <attribute>.
 */
static bool read_float(SDFILE* file, const char* name, float* attribute)
{
    char buffer[BUFFER_SIZE];
    char format[BUFFER_SIZE];
    double placeholder;
    int n_matched;
    SDRESULT status;

    status = microsd_gets(file, buffer, BUFFER_SIZE);
    if (status != FR_OK) return false;

    sprintf(format, "%s=%%lf", name);
    n_matched = sscanf(buffer, format, &placeholder);
    *attribute = (float)placeholder;
    return n_matched == 1;
}


/* read a bool attribute (aka int) with config name <name> into <attribute>.
 */
static bool read_bool(SDFILE* file, const char* name, bool* attribute)
{
    unsigned int res = 0;
    if (!read_int(file, name, &res)) return false;
    *attribute = res != 0;
    return true;
}

/* returns true if succesfully read ALL the attributes.
 * strict in terms of ordering, no spaces, etc.
 */
bool read_config(SDFILE* file)
{
    conf.config_loaded =
        read_int(file, "accel_axis", &conf.accel_axis) &&
        read_int(file, "pyro_firetime", &conf.pyro_firetime) &&
        read_int(file, "pyro_1", &conf.pyro_1) &&
        read_int(file, "pyro_2", &conf.pyro_2) &&
        read_int(file, "pyro_3", &conf.pyro_3) &&
        read_int(file, "ignition_accel", &conf.ignition_accel) &&
        read_int(file, "burnout_time", &conf.burnout_time) &&
        read_int(file, "apogee_time", &conf.apogee_time) &&
        read_int(file, "main_altitude", &conf.main_altitude) &&
        read_int(file, "main_time", &conf.main_time) &&
        read_int(file, "landing_time", &conf.landing_time) &&
        read_bool(file, "read_analogue", &conf.read_analogue);

    (void)read_float;

    return conf.config_loaded;
}

/* Sanity check the config we read to ensure it's at least reasonable and
 * consistent.
 */
bool check_config()
{
    bool ok = true;

    /* Basic sanity checks */
    ok &= conf.location == CFG_M2FC_BODY || conf.location == CFG_M2FC_NOSE;
    ok &= conf.accel_axis < 3;
    ok &= conf.pyro_firetime < 100000;
    ok &= conf.pyro_1 < 3;
    ok &= conf.pyro_2 < 3;
    ok &= conf.pyro_3 < 3;
    ok &= conf.ignition_accel < 1000;
    ok &= conf.burnout_time < 1000000;
    ok &= conf.apogee_time < 10000000;
    ok &= conf.main_altitude < 100000;
    ok &= conf.main_time < 10000000;
    ok &= conf.landing_time < 10000000;

    /* Check pyro consistency */
    if(conf.location == CFG_M2FC_BODY)
        ok &= (conf.pyro_1 + conf.pyro_2 + conf.pyro_3 == 1);
    else if(conf.location == CFG_M2FC_NOSE)
        ok &= (conf.pyro_1 + conf.pyro_2 + conf.pyro_3 == 3);

    return ok;
}


/* Initialises config. Returns false if some error occurred.
 */
bool config_init()
{
    SDFILE file;
    SDFS sd;
    bool success = false;

    /* Read Location from the solder jumper */
    if(palReadPad(GPIOD, GPIOD_SJ1) == PAL_LOW)
        conf.location = CFG_M2FC_NOSE;
    else
        conf.location = CFG_M2FC_BODY;

    /* Read other configuration from the microSD card */
    SDRESULT open = microsd_open_file(&file, "config.txt", FA_READ, &sd);
    if (open == FR_OK) {
        success = read_config(&file);
        success &= check_config();
    }

    microsd_close_file(&file);
    return success;
}
