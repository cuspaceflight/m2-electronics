/*
 * MicroSD logging
 * M2FC
 * 2014 Adam Greig, Cambridge University Spaceflight
 */

#include <string.h>
#include "microsd.h"
#include "hal.h"
#include "chprintf.h"

static MemoryPool microsd_mp;
static microsd_message microsd_mp_b[256];

static Mailbox microsd_mb;
static msg_t microsd_mb_q[256];

static void microsd_open_file(FIL* fp);
static void microsd_unmount(FIL* fp);
static void microsd_write_line(FIL* fp, char* line);

void microsd_mem_init()
{
    chPoolInit(&microsd_mp, sizeof(microsd_message), NULL);
    chPoolLoadArray(&microsd_mp, (void*)microsd_mp_b, 256);

    chMBInit(&microsd_mb, microsd_mb_q, 256);
}

void microsd_log(const char* data)
{
    microsd_message *msg;
    msg = (microsd_message*)chPoolAlloc(&microsd_mp);
    msg->rt = halGetCounterValue();
    memcpy(msg->thread, chRegGetThreadName(chThdSelf()), 10);
    memcpy(msg->data, data, 41);
    chMBPost(&microsd_mb, (msg_t)msg, TIME_IMMEDIATE);
}

static void microsd_open_file(FIL* fp)
{
    uint16_t file_idx = 0;
    char fname[16];
    FRESULT err;
    BYTE mode = FA_WRITE | FA_CREATE_NEW;

    chsnprintf(fname, 16, "log_%05d.txt", file_idx);
    err = f_open(fp, fname, mode);
    while(err != FR_OK) {
        /*
        chprintf((BaseSequentialStream*)&SD2,
                 "Could not open file '%s', err %d\r\n", fname, err);
        */
        file_idx++;
        chsnprintf(fname, 16, "log_%05d.txt", file_idx);
        err = f_open(fp, fname, mode);
    }
}

static void microsd_unmount(FIL* fp)
{
    f_close(fp);
    sdcDisconnect(&SDCD1);
}

static void microsd_write_line(FIL* fp, char* line)
{
    int n;
    n = f_puts(line, fp);
    if(n == -1) {
        /* TODO: be sad */
        while(1);
    }
    f_sync(fp);
}

msg_t microsd_thread(void* arg)
{
    static FATFS fs;
    static FIL fp;
    static FRESULT err;
    msg_t status, msgp;
    microsd_message *msg;
    char logline[64];

    (void)arg;

    chRegSetThreadName("MicroSD");

    sdcStart(&SDCD1, NULL);

    chThdSleepMilliseconds(5);

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

    while(TRUE) {
        status = chMBFetch(&microsd_mb, &msgp, 1000);
        if(status != RDY_OK) {
            continue;
        }

        msg = (microsd_message*)msgp;
        chsnprintf(logline, 64, "%10u,%10s,%s\n",
                   msg->rt, msg->thread, msg->data);
        microsd_write_line(&fp, logline);
        chPoolFree(&microsd_mp, (msg_t*)msgp);
    }

    microsd_unmount(&fp);
}

/*
static FRESULT scan_files(BaseSequentialStream *chp, char *path) {
  FRESULT res;
  FILINFO fno;
  DIR dir;
  int i;
  char *fn;

#if _USE_LFN
  fno.lfname = 0;
  fno.lfsize = 0;
#endif
  res = f_opendir(&dir, path);
  if (res == FR_OK) {
    i = strlen(path);
    for (;;) {
      res = f_readdir(&dir, &fno);
      if (res != FR_OK || fno.fname[0] == 0)
        break;
      if (fno.fname[0] == '.')
        continue;
      fn = fno.fname;
      if (fno.fattrib & AM_DIR) {
        path[i++] = '/';
        strcpy(&path[i], fn);
        res = scan_files(chp, path);
        if (res != FR_OK)
          break;
        path[--i] = 0;
      }
      else {
        chprintf(chp, "%s/%s\r\n", path, fn);
      }
    }
  }
  return res;
}


static void cmd_tree(BaseSequentialStream *chp, int argc, char *argv[]) {
  FRESULT err;
  uint32_t clusters;
  FATFS *fsp;

  (void)argv;
  if (argc > 0) {
    chprintf(chp, "Usage: tree\r\n");
    return;
  }
  if (!fs_ready) {
    chprintf(chp, "File System not mounted\r\n");
    return;
  }
  err = f_getfree("/", &clusters, &fsp);
  if (err != FR_OK) {
    chprintf(chp, "FS: f_getfree() failed\r\n");
    return;
  }
  chprintf(chp,
           "FS: %lu free clusters, %lu sectors per cluster, %lu bytes free\r\n",
           clusters, (uint32_t)SDC_FS.csize,
           clusters * (uint32_t)SDC_FS.csize * (uint32_t)MMCSD_BLOCK_SIZE);
  fbuff[0] = 0;
  scan_files(chp, (char *)fbuff);
}

*/
