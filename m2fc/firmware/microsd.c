/*
 * MicroSD logging
 * M2FC
 * 2014 Adam Greig, Cambridge University Spaceflight
 */

#include <string.h>
#include <stdio.h>
#include "microsd.h"
#include "hal.h"
#include "chprintf.h"
#include "ff.h"

#define MICROSD_MEMPOOL_SIZE 1024
#define MICROSD_CACHE_SIZE 16384

static MemoryPool microsd_mp;
static volatile char microsd_mp_b[MICROSD_MEMPOOL_SIZE * 16]
                     __attribute__((aligned(sizeof(stkalign_t))));

static Mailbox microsd_mb;
static volatile msg_t microsd_mb_q[MICROSD_MEMPOOL_SIZE];

static volatile char microsd_cache[MICROSD_CACHE_SIZE];

static void microsd_mem_init(void);
static void microsd_open_file(FIL* fp);
static void microsd_unmount(FIL* fp);
static void microsd_write(FIL* fp);

int32_t last_write_time;


void microsd_log_c(uint8_t channel, const char* data)
{
    volatile char *msg;
    msg = (char*)chPoolAlloc(&microsd_mp);
    msg[6] = (char)0;
    msg[7] = (char)channel;
    memcpy((void*)msg, (void*)&halGetCounterValue(), 4);
    memcpy((void*)&msg[8], data, 8);
    chMBPost(&microsd_mb, (msg_t)msg, TIME_IMMEDIATE);
}

void microsd_log_s64(uint8_t channel, int64_t *data)
{
    char *msg;
    msg = (void*)chPoolAlloc(&microsd_mp);
    msg[6] = (char)1;
    msg[7] = (char)channel;
    memcpy(msg, (void*)&halGetCounterValue(), 4);
    memcpy(&msg[8], data, 8);
    chMBPost(&microsd_mb, (msg_t)msg, TIME_IMMEDIATE);
}

void microsd_log_s32(uint8_t channel, int32_t *data_a, int32_t *data_b)
{
    char *msg;
    msg = (void*)chPoolAlloc(&microsd_mp);
    msg[6] = (char)2;
    msg[7] = (char)channel;
    memcpy(msg, (void*)&halGetCounterValue(), 4);
    memcpy(&msg[8],  data_a, 4);
    memcpy(&msg[12], data_b, 4);
    chMBPost(&microsd_mb, (msg_t)msg, TIME_IMMEDIATE);
}

void microsd_log_s16(uint8_t channel, int16_t *data_a, int16_t *data_b,
                                  int16_t *data_c, int16_t *data_d)
{
    char *msg;
    msg = (void*)chPoolAlloc(&microsd_mp);
    msg[6] = (char)3;
    msg[7] = (char)channel;
    memcpy(msg, (void*)&halGetCounterValue(), 4);
    memcpy(&msg[8],  data_a, 2);
    memcpy(&msg[10], data_b, 2);
    memcpy(&msg[12], data_c, 2);
    memcpy(&msg[14], data_d, 2);
    chMBPost(&microsd_mb, (msg_t)msg, TIME_IMMEDIATE);
}

void microsd_log_u16(uint8_t channel, uint16_t *data_a, uint16_t *data_b,
                                      uint16_t *data_c, uint16_t *data_d)
{
    char *msg;
    msg = (void*)chPoolAlloc(&microsd_mp);
    msg[6] = (char)4;
    msg[7] = (char)channel;
    memcpy(msg, (void*)&halGetCounterValue(), 4);
    memcpy(&msg[8],  data_a, 2);
    memcpy(&msg[10], data_b, 2);
    memcpy(&msg[12], data_c, 2);
    memcpy(&msg[14], data_d, 2);
    chMBPost(&microsd_mb, (msg_t)msg, TIME_IMMEDIATE);
}

static void microsd_mem_init()
{
    chPoolInit(&microsd_mp, 16, NULL);
    chPoolLoadArray(&microsd_mp, (void*)microsd_mp_b, MICROSD_MEMPOOL_SIZE);
    chMBInit(&microsd_mb, (msg_t*)microsd_mb_q, MICROSD_MEMPOOL_SIZE);
}

static void microsd_open_file(FIL* fp)
{
    uint16_t file_idx = 0;
    char fname[16];
    FRESULT err;
    BYTE mode = FA_WRITE | FA_CREATE_NEW;

    do {
        file_idx++;
        chsnprintf(fname, 16, "log_%05d.bin", file_idx);
        err = f_open(fp, fname, mode);
    } while(err != FR_OK);
}

static void microsd_unmount(FIL* fp)
{
    f_close(fp);
    sdcDisconnect(&SDCD1);
}

static void microsd_write(FIL* fp)
{
    int32_t t0 = chTimeNow();
    UINT n;
    FRESULT err;

    err = f_write(fp, (void*)microsd_cache, MICROSD_CACHE_SIZE, &n);
    if(err != FR_OK || n != MICROSD_CACHE_SIZE) {
        /* TODO: be sad */
        while(1);
    }
    f_sync(fp);
    last_write_time = chTimeNow() - t0;
}

msg_t microsd_thread(void* arg)
{
    static FATFS fs;
    static FIL fp;
    static FRESULT err;
    msg_t status, msgp;
    char *msg;
    volatile char *cachep = microsd_cache;

    (void)arg;

    microsd_mem_init();
    microsd_log_c(0x00, "CUSFCUSF");

    chRegSetThreadName("MicroSD");

    sdcStart(&SDCD1, NULL);

    
    if(sdcConnect(&SDCD1)) {
        /* TODO: be sad */
        while(1);
    }

    err = f_mount(0, &fs);
    if(err != FR_OK) {
        /* TODO: be sad */
        sdcDisconnect(&SDCD1);
        while(1);
    }

    microsd_open_file(&fp);
    f_lseek(&fp, 1024 * MICROSD_CACHE_SIZE);
    f_lseek(&fp, 0);

    while(TRUE) {
        status = chMBFetch(&microsd_mb, &msgp, TIME_INFINITE);
        if(status != RDY_OK || msgp == 0) {
            continue;
        }

        msg = (char*)msgp;
        memcpy((void*)cachep, msg, 16);
        /*
        chprintf((BaseSequentialStream*)&SD2,
                "About to free %x\r\n", (void*)msg);
        */
        chPoolFree(&microsd_mp, (void*)msg);
        if(cachep + 16 == microsd_cache + MICROSD_CACHE_SIZE) {
            microsd_write(&fp);
            cachep = microsd_cache;
        } else {
            cachep += 16;
        }
    }

    microsd_unmount(&fp);
}
