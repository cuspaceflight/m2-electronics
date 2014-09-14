/*
 * MicroSD logging
 * M2FC
 * 2014 Adam Greig, Cambridge University Spaceflight
 */
#ifndef MICROSD_H
#define MICROSD_H

#include "ch.h"
#include "ff.h"

#define CHAN_INIT           0x00
#define CHAN_CAL_TFREQ      0x10
#define CHAN_CAL_LGA        0x11
#define CHAN_CAL_HGA        0x12
#define CHAN_CAL_BARO1      0x13
#define CHAN_CAL_BARO2      0x14
#define CHAN_IMU_LGA        0x20
#define CHAN_IMU_HGA        0x21
#define CHAN_IMU_BARO       0x22
#define CHAN_SENS_BAT       0x30
#define CHAN_SENS_SG        0x31
#define CHAN_SENS_TC        0x32
#define CHAN_SM_MISSION     0x40
#define CHAN_SE_P1          0x50
#define CHAN_SE_P2          0x51
#define CHAN_SE_U_P         0x52
#define CHAN_SE_U_A         0x53
#define CHAN_PYRO_C         0x60
#define CHAN_PYRO_F         0x61

/* Log data to the micro SD card.
 * The channel is 0 to 255.
 * Data is either char* (the first 8 bytes will be copied),
 * or a number of integers that add up to 64 bits.
 * The time the function is called is stored against the sample.
 */
void microsd_log_c(uint8_t channel, const char* data);
void microsd_log_s64(uint8_t channel, int64_t data);
void microsd_log_s32(uint8_t channel, int32_t data_a, int32_t data_b);
void microsd_log_s16(uint8_t channel, int16_t data_a, int16_t data_b,
                                      int16_t data_c, int16_t data_d);
void microsd_log_u16(uint8_t channel, uint16_t data_a, uint16_t data_b,
                                      uint16_t data_c, uint16_t data_d);
void microsd_log_f(uint8_t channel, float data_a, float data_b);

/* The MicroSD logging thread. Run me. */
msg_t microsd_thread(void* arg);

#endif /* MICROSD_H */
