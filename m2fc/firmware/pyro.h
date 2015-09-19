/*
 * Pyrotechnic channel driver
 * M2FC
 * 2014 Adam Greig, Cambridge University Spaceflight
 */

#ifndef PYRO_H
#define PYRO_H

#include <ch.h>
#include <stdbool.h>

typedef enum {PYRO_1, PYRO_2, PYRO_3} pyro_channel;

/* Check the pyro channel `channel` for continuity, returns TRUE or FALSE. */
bool pyro_continuity(pyro_channel channel);

/* Check that all pyros that should be installed are installed. */
bool pyro_continuities(void);

/* Fire the pyro channels `channel` for `conf.pyro_firetime` milliseconds.
 * Blocks.
 */
void pyro_fire(bool ch1, bool ch2, bool ch3);

/* Fire the drogue or main chute, selecting the appropriate pyro channels from
 * the configuration.
 * Fires for conf.pyro_firetime ms, blocks.
 */
void pyro_fire_drogue(void);
void pyro_fire_main(void);

/* Checks pyro continuities continuously */
msg_t pyro_continuity_thread(void *arg);

#endif /* PYRO_H */
