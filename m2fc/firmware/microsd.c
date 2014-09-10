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

#define MICROSD_MEMPOOL_ITEMS 2048
#define MICROSD_CACHE_SIZE    16384

static MemoryPool microsd_mp;
static volatile char microsd_mp_b[MICROSD_MEMPOOL_ITEMS * 16]
                     __attribute__((aligned(sizeof(stkalign_t))))
                     __attribute__((section(".ccm")));

static Mailbox microsd_mb;
static volatile msg_t microsd_mb_q[MICROSD_MEMPOOL_ITEMS];

static volatile char microsd_cache[MICROSD_CACHE_SIZE];

static void microsd_mem_init(void);
static void microsd_card_deinit(FIL* fp);
static void microsd_card_try_init(FATFS* fs, FIL* fp);
static bool_t microsd_card_init(FATFS* fs, FIL* fp);
static bool_t microsd_open_file(FIL* fp);
static bool_t microsd_write(FIL* fp);


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

void microsd_log_f(uint8_t channel, float *data_a, float *data_b)
{
    char *msg;
    msg = (void*)chPoolAlloc(&microsd_mp);
    msg[6] = (char)5;
    msg[7] = (char)channel;
    memcpy(msg, (void*)&halGetCounterValue(), 4);
    memcpy(&msg[8], data_a, 4);
    memcpy(&msg[12], data_b, 4);
    chMBPost(&microsd_mb, (msg_t)msg, TIME_IMMEDIATE);
}

static void microsd_mem_init()
{
    chPoolInit(&microsd_mp, 16, NULL);
    chPoolLoadArray(&microsd_mp, (void*)microsd_mp_b, MICROSD_MEMPOOL_ITEMS);
    chMBInit(&microsd_mb, (msg_t*)microsd_mb_q, MICROSD_MEMPOOL_ITEMS);
}

/* Attempt to open a log file on the SD card's filesystem.
 * Tries successive files of the form log_00001.bin etc.
 * Returns TRUE on success, FALSE on failure.
 */
static bool_t microsd_open_file(FIL* fp)
{
    uint32_t file_idx = 0;
    char fname[16];
    FRESULT err;
    BYTE mode = FA_WRITE | FA_CREATE_NEW;

    while(1) {
        file_idx++;
        chsnprintf(fname, 16, "log_%05d.bin", file_idx);
        err = f_open(fp, fname, mode);
        if(err == FR_OK) {
            return TRUE;
        } else if(err != FR_EXIST) {
            return FALSE;
        } else if(file_idx > 99999) {
            return FALSE;
        }
    }
}

static bool_t microsd_write(FIL* fp)
{
    UINT n;
    FRESULT err;

    /* Turn on SD CARD LED to indicate writing. */
    palSetPad(GPIOA, GPIOA_LED_SDCARD);

    /* Write the cache to the SD card. */
    err = f_write(fp, (void*)microsd_cache, MICROSD_CACHE_SIZE, &n);

    /* Be sad if it didn't work */
    if(err != FR_OK || n != MICROSD_CACHE_SIZE) {
        return FALSE;
    }

    /* Wait for sync */
    f_sync(fp);

    /* Clear the LED once data is safely on the card. */
    palClearPad(GPIOA, GPIOA_LED_SDCARD);

    return TRUE;
}

static bool_t microsd_card_init(FATFS* fs, FIL* fp)
{
    FRESULT err;

    /* Initialise the SDC interface */
    sdcStart(&SDCD1, NULL);

    /* Attempt to connect to the SD card */
    if(sdcConnect(&SDCD1)) {
        return FALSE;
    }

    /* Attempt to mount the filesystem */
    err = f_mount(0, fs);
    if(err != FR_OK) {
        return FALSE;
    }

    /* Attempt to open the log file */
    if(!microsd_open_file(fp)) {
        return FALSE;
    }

    /* We used to preallocate a large file but hopefully this isn't useful. */
    /*
    err = f_lseek(fp, 128 * MICROSD_CACHE_SIZE);
    if(err != FR_OK) {
        return FALSE;
    }

    err = f_lseek(fp, 0);
    if(err != FR_OK) {
        return FALSE;
    }
    */

    return TRUE;
}

/* Best effort to turn off the SD card entirely. Mainly used if something has
 * broken and we should stop and try again.
 */
static void microsd_card_deinit(FIL* fp)
{
    /* Close open file */
    f_close(fp);
    
    /* Unmount FS */
    f_mount(0, NULL);
    
    /* Disconnect from card */
    sdcDisconnect(&SDCD1);

    /* Disable SDC peripheral */
    sdcStop(&SDCD1);
}

static void microsd_card_try_init(FATFS* fs, FIL* fp)
{
    while(!microsd_card_init(fs, fp)) {
        microsd_card_deinit(fp);
        palSetPad(GPIOA, GPIOA_LED_SDCARD);
        chThdSleepMilliseconds(100);
        palClearPad(GPIOA, GPIOA_LED_SDCARD);
        chThdSleepMilliseconds(100);
    }
}

msg_t microsd_thread(void* arg)
{
    static FATFS fs;
    static FIL fp;
    msg_t status, msgp;
    bool_t write_success;
    char *msg;
    volatile char *cachep = microsd_cache;

    (void)arg;

    microsd_mem_init();
    microsd_log_c(0x00, "CUSFM2FC");

    chRegSetThreadName("MicroSD");

    microsd_card_try_init(&fs, &fp);

    while(TRUE) {
        status = chMBFetch(&microsd_mb, &msgp, TIME_INFINITE);
        if(status != RDY_OK || msgp == 0) {
            continue;
        }

        msg = (char*)msgp;
        memcpy((void*)cachep, msg, 16);
        chPoolFree(&microsd_mp, (void*)msg);
        if(cachep + 16 == microsd_cache + MICROSD_CACHE_SIZE) {
            write_success = microsd_write(&fp);

            if(!write_success) {
                microsd_card_deinit(&fp);
                microsd_card_try_init(&fs, &fp);
            }

            cachep = microsd_cache;
        } else {
            cachep += 16;
        }
    }
}
