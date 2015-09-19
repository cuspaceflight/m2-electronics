/*
 * Datalogging
 * M2FC/Badger3
 * 2015 Chi Pham, Cambridge University Spaceflight
 * 2015 Adam Greig, Cambridge University Spaceflight
 */

#ifndef DATALOGGING_H
#define DATALOGGING_H

#include "m2telem.h"

/* Main datalogging thread that handles writing the data persistently.
 * It periodically fetches the data that is being logged by the below functions
 * and saves them to the microsd card.
 */
msg_t datalogging_thread(void* arg);

/* log 8 characters */
void log_c(uint8_t channel, const char* data);

/* log one signed 64-bit integer */
void log_i64(uint8_t channel, int64_t data);

/* log one unsigned 64-bit integer */
void log_u64(uint8_t channel, uint64_t data);

/* log two signed 32-bit integers */
void log_i32(uint8_t channel, int32_t data_a, int32_t data_b);

/* log two unsigned 32-bit integers */
void log_u32(uint8_t channel, uint32_t data_a, uint32_t data_b);

/* log four signed 16-bit integers */
void log_i16(uint8_t channel, int16_t data_a, int16_t data_b,
    int16_t data_c, int16_t data_d);

/* log four unsigned 16-bit integers */
void log_u16(uint8_t channel, uint16_t data_a, uint16_t data_b,
    uint16_t data_c, uint16_t data_d);

/* log eight signed 8-bit integers */
void log_i8(uint8_t channel,
    int8_t data_a, int8_t data_b, int8_t data_c, int8_t data_d,
    int8_t data_e, int8_t data_f, int8_t data_g, int8_t data_h);

/* log eight unsigned 8-bit integers */
void log_u8(uint8_t channel,
    uint8_t data_a, uint8_t data_b, uint8_t data_c, uint8_t data_d,
    uint8_t data_e, uint8_t data_f, uint8_t data_g, uint8_t data_h);

/* log two 32-bit single precision floats */
void log_f(uint8_t channel, float data_a, float data_b);

/* log one 64-bit double precision float */
void log_d(uint8_t channel, double data);

#endif /* DATALOGGING_H */
