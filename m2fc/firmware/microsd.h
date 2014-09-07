/*
 * MicroSD logging
 * M2FC
 * 2014 Adam Greig, Cambridge University Spaceflight
 */
#ifndef MICROSD_H
#define MICROSD_H

#include "ch.h"
#include "ff.h"

/* Log data to the micro SD card.
 * The channel is 0 to 255.
 * Data is either char* (the first 8 bytes will be copied),
 * or a number of integers that add up to 64 bits.
 * The time the function is called is stored against the sample.
 */
void microsd_log_c(uint8_t channel, const char* data);
void microsd_log_s64(uint8_t channel, int64_t *data);
void microsd_log_s32(uint8_t channel, int32_t *data_a, int32_t *data_b);
void microsd_log_s16(uint8_t channel, int16_t *data_a, int16_t *data_b,
                                      int16_t *data_c, int16_t *data_d);
void microsd_log_u16(uint8_t channel, uint16_t *data_a, uint16_t *data_b,
                                      uint16_t *data_c, uint16_t *data_d);
void microsd_log_f(uint8_t channel, float *data);

/* The MicroSD logging thread. Run me. */
msg_t microsd_thread(void* arg);

#endif /* MICROSD_H */
