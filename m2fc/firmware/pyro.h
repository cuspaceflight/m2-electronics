/*
 * Pyrotechnic channel driver
 * M2FC
 * 2014 Adam Greig, Cambridge University Spaceflight
 */

#ifndef PYRO_H
#define PYRO_H

#include <ch.h>

typedef enum {PYRO_1, PYRO_2, PYRO_3} pyro_channel;

/* Check the pyro channel `channel` for continuity, returns TRUE or FALSE. */
bool_t pyro_continuity(pyro_channel channel);

/* Check that all pyros that should be installed are installed. */
bool_t pyro_continuities(void);

/* Fire the pyro channel `channel` for `duration_ms` milliseconds.
 * Non-blocking.
 */
void pyro_fire(pyro_channel channel, uint16_t duration_ms);

/* Fire the drogue or main chute, selecting the appropriate pyro channels from
 * the configuration.
 * Fires for PYRO_FIRETIME ms.
 * Does not block.
 */
void pyro_fire_drogue(void);
void pyro_fire_main(void);


/* Checks pyro continuities continuously */
msg_t pyro_continuity_thread(void *arg);

#endif /* PYRO_H */
