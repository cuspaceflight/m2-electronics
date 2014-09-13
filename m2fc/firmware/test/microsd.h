#pragma once
#include <stdint.h>
#include <stdio.h>
void microsd_log_f(uint8_t channel, float *a, float *b);
void microsd_log_s32(uint8_t channel, int32_t *a, int32_t *b);
void microsd_init_files(FILE *statefile, FILE *sensorfile);
void microsd_set_time(float t);
