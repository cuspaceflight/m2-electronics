/*
 * Datalogging
 * M2FC/Badger3
 * 2015 Chi Pham, Cambridge University Spaceflight
 * 2015 Adam Greig, Cambridge University Spaceflight
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "hal.h"
#include "microsd.h"
#include "datalogging.h"
#include "config.h"
#include "chprintf.h"
#include "m2status.h"

/* ------------------------------------------------------------------------- */

#define LOG_MEMPOOL_ITEMS 3072  // 1K
#define LOG_CACHE_SIZE    16384 // 16KB

static void mem_init(void);
static void _log(TelemPacket *packet);

/* ------------------------------------------------------------------------- */
/* STATIC VARIABLES */
/* ------------------------------------------------------------------------- */

/* memory pool for allocating space for incoming data to be queued. */
static MemoryPool log_mempool;

/* mailbox (queue) for storing/accessing data asynchronously. Will be storing
 * pointers to data (packets).
 */
static Mailbox log_mailbox;

/* data cache to ensure SD writes are done LOG_CACHE_SIZE bytes at a time. */
static volatile char log_cache[LOG_CACHE_SIZE];

/* statically allocated memory used for the memory pool */
static volatile char mempool_buffer[LOG_MEMPOOL_ITEMS * sizeof(TelemPacket)]
                     __attribute__((aligned(sizeof(stkalign_t))))
                     __attribute__((section(".ccm")));

/* statically allocated memory used for the queue in mailbox */
static volatile msg_t mailbox_buffer[LOG_MEMPOOL_ITEMS]
                      __attribute__((section(".ccm")));

static uint8_t log_location = 0;

/* ------------------------------------------------------------------------- */
/* MAIN THREAD FUNCTIONS */
/* ------------------------------------------------------------------------- */

/* Main datalogging thread. Continuously checks for data added through the
 * logging functions, and persists it to an SD card/sends some of it to radio.
 */
msg_t datalogging_thread(void* arg)
{
    static const int packet_size = sizeof(TelemPacket);
    volatile char* cache_ptr = log_cache; // pointer to keep track of cache

    SDFS file_system;        // struct that encapsulates file system state
    SDFILE file;             // file struct thing
    msg_t mailbox_res;       // mailbox fetch result
    intptr_t data_msg;       // buffer to store the fetched mailbox item
    SDRESULT write_res;      // result of writing data to file system
    SDRESULT open_res;       // result of re-opening the log file
    (void)arg;

    /* initialise stuff */
    m2status_datalogging_status(STATUS_WAIT);
    chRegSetThreadName("Datalogging");
    mem_init();
    while (microsd_open_file_inc(&file, "log", "bin", &file_system) != FR_OK);

    if(conf.location == CFG_M2FC_NOSE)
        log_c(M2T_CH_SYS_INIT, "M2FCNOSE");
    else if(conf.location == CFG_M2FC_BODY)
        log_c(M2T_CH_SYS_INIT, "M2FCBODY");
    log_c(M2T_CH_SYS_VERSION, conf.version);

    m2status_datalogging_status(STATUS_OK);

    while (true) {
        m2status_datalogging_status(STATUS_OK);

        /* Block waiting for a message to be available */
        mailbox_res = chMBFetch(&log_mailbox, (msg_t*)&data_msg, TIME_INFINITE);

        /* Mailbox was reset while waiting/fetch failed ... try again! */
        if (mailbox_res != RDY_OK || data_msg == 0) continue;

        /* Put packet in the static cache and free it from the memory pool */
        memcpy((void*)cache_ptr, (void*)data_msg, packet_size);
        chPoolFree(&log_mempool, (void*)data_msg);

        /* If the cache is full, write it all to the sd card */
        if(cache_ptr + packet_size >= log_cache + LOG_CACHE_SIZE) {
            write_res = microsd_write(&file, (char*)log_cache, LOG_CACHE_SIZE);

            /* If the write failed, keep attempting to re-open the log file
             * and write the data out when we succeed.
             */
            while (write_res != FR_OK) {
                m2status_datalogging_status(STATUS_ERR_WRITING);
                microsd_close_file(&file);
                open_res = microsd_open_file_inc(&file, "log", "bin",
                                                 &file_system);
                if(open_res == FR_OK) {
                    write_res = microsd_write(&file, (char*)log_cache,
                                              LOG_CACHE_SIZE);
                }
            }

            /* reset cache pointer to beginning of cache */
            cache_ptr = log_cache;
        } else {
            cache_ptr += packet_size;
        }
    }
}

/* Initialise memory management structures used to keep the data temporarily
 * in memory.
 */
static void mem_init(void)
{
    chMBInit(&log_mailbox, (msg_t*)mailbox_buffer, LOG_MEMPOOL_ITEMS);
    chPoolInit(&log_mempool, sizeof(TelemPacket), NULL);

    /* fill the memory pool with statically allocated bits of memory
     * ie. prevent dynamic core memory allocation (which cannot be freed), we
     * just want the "bookkeeping" that memory pools provide
     */
    chPoolLoadArray(&log_mempool, (void*)mempool_buffer, LOG_MEMPOOL_ITEMS);

    log_location = 2 - conf.location;
}

/* ------------------------------------------------------------------------- */
/* LOGGING FUNCTIONS */
/* ------------------------------------------------------------------------- */

/* log 8 characters. truncates data */
void log_c(uint8_t channel, const char* c)
{
    TelemPacket pkt = {
        .timestamp = halGetCounterValue(), .metadata = 0, .channel = channel,
        .checksum = 0, .c = {c[0], c[1], c[2], c[3], c[4], c[5], c[6], c[7]}};
    _log(&pkt);
}

/* log one signed 64-bit integer */
void log_i64(uint8_t channel, int64_t a)
{
    TelemPacket pkt = {
        .timestamp = halGetCounterValue(), .channel = channel,
        .metadata = 0, .checksum = 0, .i64 = a};
    _log(&pkt);
}

/* log one unsigned 64-bit integer */
void log_u64(uint8_t channel, uint64_t a)
{
    TelemPacket pkt = {
        .timestamp = halGetCounterValue(), .channel = channel,
        .metadata = 0, .checksum = 0, .u64 = a};
    _log(&pkt);
}

/* log two signed 32-bit integers */
void log_i32(uint8_t channel, int32_t a, int32_t b)
{
    TelemPacket pkt = {
        .timestamp = halGetCounterValue(), .channel = channel,
        .metadata = 0, .checksum = 0, .i32 = {a, b}};
    _log(&pkt);
}

/* log two unsigned 32-bit integers */
void log_u32(uint8_t channel, uint32_t a, uint32_t b)
{
    TelemPacket pkt = {
        .timestamp = halGetCounterValue(), .channel = channel,
        .metadata = 0, .checksum = 0, .u32 = {a, b}};
    _log(&pkt);
}

/* log four signed 16-bit integers */
void log_i16(uint8_t channel,
    int16_t a, int16_t b, int16_t c, int16_t d)
{
    TelemPacket pkt = {
        .timestamp = halGetCounterValue(), .channel = channel,
        .metadata = 0, .checksum = 0, .i16 = {a, b, c, d}};
    _log(&pkt);
}

/* log four unsigned 16-bit integers */
void log_u16(uint8_t channel,
    uint16_t a, uint16_t b, uint16_t c, uint16_t d)
{
    TelemPacket pkt = {
        .timestamp = halGetCounterValue(), .channel = channel,
        .metadata = 0, .checksum = 0, .u16 = {a, b, c, d}};
    _log(&pkt);
}

/* log eight signed 8-bit integers */
void log_i8(uint8_t channel,
    int8_t a, int8_t b, int8_t c, int8_t d,
    int8_t e, int8_t f, int8_t g, int8_t h)
{
    TelemPacket pkt = {
        .timestamp = halGetCounterValue(), .channel = channel,
        .metadata = 0, .checksum = 0, .i8 = {a, b, c, d, e, f, g, h}};
    _log(&pkt);
}

/* log eight unsigned 8-bit integers */
void log_u8(uint8_t channel,
    uint8_t a, uint8_t b, uint8_t c, uint8_t d,
    uint8_t e, uint8_t f, uint8_t g, uint8_t h)
{
    TelemPacket pkt = {
        .timestamp = halGetCounterValue(), .channel = channel,
        .metadata = 0, .checksum = 0, .u8 = {a, b, c, d, e, f, g, h}};
    _log(&pkt);
}

/* log two 32-bit single precision floats */
void log_f(uint8_t channel, float a, float b)
{
    TelemPacket pkt = {
        .timestamp = halGetCounterValue(), .channel = channel,
        .metadata = 0, .checksum = 0, .f = {a, b}};
    _log(&pkt);
}

/* log one 64-bit double precision float */
void log_d(uint8_t channel, double a)
{
    TelemPacket pkt = {
        .timestamp = halGetCounterValue(), .channel = channel,
        .metadata = 0, .checksum = 0, .d = a};
    _log(&pkt);
}

/* Allocate and post a formatted packet containing metadata + data to mailbox.
 * Use counter arrays to determine if this data should be sampled to radio.
 * (it's called _log because log conflicts with a library function)
 */
static void _log(TelemPacket *packet)
{
    void* msg;
    msg_t retval;
    packet->metadata = log_location;
    m2telem_write_checksum(packet);

    /* allocate space for the packet and copy it into a mailbox message */
    msg = chPoolAlloc(&log_mempool);
    if (msg == NULL) return;
    memcpy(msg, (void*)packet, sizeof(TelemPacket));

    /* put it in the mailbox buffer */
    retval = chMBPost(&log_mailbox, (intptr_t)msg, TIME_IMMEDIATE);
    if (retval != RDY_OK) {
        chPoolFree(&log_mempool, msg);
        return;
    }
}
