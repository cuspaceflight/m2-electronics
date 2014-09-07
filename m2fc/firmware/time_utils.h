/*
 * Time utilities
 * M2FC
 * 2014 Adam Greig, Cambridge University Spaceflight
 */

#ifndef TIME_UTILS_H
#define TIME_UTILS_H

#include <stdint.h>
#include "ch.h"

/* Return the number of system clock (168MHz) ticks since the function was last
 * called with the same parameter t0. Updates t0 to current time on call. */
uint32_t time_ticks_since(uint32_t *t0);

/* As time_ticks_since but returns floating point seconds.
 * Also updates t0 for you. */
float time_seconds_since(uint32_t *t0);

#endif /* TIME_UTILS_H */
