/*
 * MicroSD logging
 * M2FC
 * 2014 Adam Greig, Cambridge University Spaceflight
 */
#ifndef MICROSD_H
#define MICROSD_H

#include "ch.h"
#include "ff.h"

typedef struct {
    uint32_t rt;
    char thread[10];
    char data[41];
} microsd_message __attribute__((aligned(sizeof(stkalign_t))));

void microsd_mem_init(void);
void microsd_log(char* data);
msg_t microsd_thread(void* arg);

#endif /* MICROSD_H */
