/*
 * M2FC
 * 2014 Adam Greig, Cambridge University Spaceflight
 */

#include <ch.h>
#include <hal.h>

#include "ms5611.h"
#include "adxl3x5.h"
#include "pyro.h"
/*#include "microsd.h"*/

#include <stdio.h>
#include <string.h>

#include "chprintf.h"
#include "test.h"
#include "shell.h"

#include "ff.h"

/*
static void cmd_mem(BaseSequentialStream *chp, int argc, char *argv[]) {
    size_t n, size;
    (void)argv;
    if (argc > 0) {
        chprintf(chp, "Usage: mem\r\n");
        return;
    }
    n = chHeapStatus(NULL, &size);
    chprintf(chp, "core free memory : %u bytes\r\n", chCoreStatus());
    chprintf(chp, "heap fragments   : %u\r\n", n);
    chprintf(chp, "heap free total  : %u bytes\r\n", size);
}

static void cmd_threads(BaseSequentialStream *chp, int argc, char *argv[]) {
    static const char *states[] = {THD_STATE_NAMES};
    Thread *tp;
    (void)argv;
    if (argc > 0) {
        chprintf(chp, "Usage: threads\r\n");
        return;
    }
    chprintf(chp, "    addr    stack prio refs     state time\r\n");
    tp = chRegFirstThread();
    do {
        chprintf(chp, "%.8lx %.8lx %4lu %4lu %9s %lu\r\n",
                 (uint32_t)tp, (uint32_t)tp->p_ctx.r13,
                 (uint32_t)tp->p_prio, (uint32_t)(tp->p_refs - 1),
                 states[tp->p_state], (uint32_t)tp->p_time);
        tp = chRegNextThread(tp);
    } while (tp != NULL);
}
*/

static FATFS SDC_FS;
static bool_t fs_ready = FALSE;
static uint8_t fbuff[1024];

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

static void cmd_mem(BaseSequentialStream *chp, int argc, char *argv[]) {
  size_t n, size;

  (void)argv;
  if (argc > 0) {
    chprintf(chp, "Usage: mem\r\n");
    return;
  }
  n = chHeapStatus(NULL, &size);
  chprintf(chp, "core free memory : %u bytes\r\n", chCoreStatus());
  chprintf(chp, "heap fragments   : %u\r\n", n);
  chprintf(chp, "heap free total  : %u bytes\r\n", size);
}

static void cmd_threads(BaseSequentialStream *chp, int argc, char *argv[]) {
  static const char *states[] = {THD_STATE_NAMES};
  Thread *tp;

  (void)argv;
  if (argc > 0) {
    chprintf(chp, "Usage: threads\r\n");
    return;
  }
  chprintf(chp, "    addr    stack prio refs     state time\r\n");
  tp = chRegFirstThread();
  do {
    chprintf(chp, "%.8lx %.8lx %4lu %4lu %9s %lu\r\n",
            (uint32_t)tp, (uint32_t)tp->p_ctx.r13,
            (uint32_t)tp->p_prio, (uint32_t)(tp->p_refs - 1),
            states[tp->p_state], (uint32_t)tp->p_time);
    tp = chRegNextThread(tp);
  } while (tp != NULL);
}

static void cmd_test(BaseSequentialStream *chp, int argc, char *argv[]) {
  Thread *tp;

  (void)argv;
  if (argc > 0) {
    chprintf(chp, "Usage: test\r\n");
    return;
  }
  tp = chThdCreateFromHeap(NULL, 256, chThdGetPriority(),
                           TestThread, chp);
  if (tp == NULL) {
    chprintf(chp, "out of memory\r\n");
    return;
  }
  chThdWait(tp);
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

static void cmd_mount(BaseSequentialStream *chp, int argc, char* argv[]) {
    FRESULT err;
    (void)argc;
    (void)argv;

    if (sdcConnect(&SDCD1)) {
        chprintf(chp, "Failed to connect\r\n");
        return;
    }

    err = f_mount(0, &SDC_FS);
    if(err != FR_OK) {
        chprintf(chp, "Failed to mount\r\n");
        sdcDisconnect(&SDCD1);
        return;
    }

    fs_ready = TRUE;
}

static void cmd_unmount(BaseSequentialStream *chp, int argc, char* argv[]) {
    (void)argc;
    (void)argv;
    sdcDisconnect(&SDCD1);
    fs_ready = FALSE;
    chprintf(chp, "OK\r\n");
}

static void cmd_hello(BaseSequentialStream *chp, int argc, char* argv[]) {
    FIL fsrc;
    FRESULT err;
    int written;
    (void)argc;
    (void)argv;
    /*
     * Open the text file
     */
    err = f_open(&fsrc, "hello.txt", FA_READ | FA_WRITE | FA_CREATE_ALWAYS);
    if (err != FR_OK) {
        chprintf(chp, "FS: f_open(\"hello.txt\") failed.\r\n");
        return;
    } else {
        chprintf(chp, "FS: f_open(\"hello.txt\") succeeded\r\n");
    }
    /*
     * Write text to the file.
     */
    written = f_puts ("Hello World", &fsrc);
    if (written == -1) {
        chprintf(chp, "FS: f_puts(\"Hello World\",\"hello.txt\") failed\r\n");
    } else {
        chprintf(chp, "FS: f_puts(\"Hello World\",\"hello.txt\") succeeded\r\n");
    }
    /*
     * Close the file
     */
    f_close(&fsrc);
}

static const ShellCommand commands[] = {
  {"mem", cmd_mem},
  {"threads", cmd_threads},
  {"test", cmd_test},
  {"tree", cmd_tree},
  {"mount", cmd_mount},
  {"unmount", cmd_unmount},
  {"hello", cmd_hello},
  {NULL, NULL}
};

static const ShellConfig shell_cfg = {
  (BaseSequentialStream *)&SD2,
  commands
};

static WORKING_AREA(waMS5611, 128);
static WORKING_AREA(waADXL345, 128);
static WORKING_AREA(waADXL375, 128);
static WORKING_AREA(waThreadHB, 128);

static msg_t ThreadHeartbeat(void *arg) {
    (void)arg;
    chRegSetThreadName("heartbeat");
    while (TRUE) {
        palSetPad(GPIOA, GPIOA_LED_STATUS);
        chThdSleepMilliseconds(500);
        palClearPad(GPIOA, GPIOA_LED_STATUS);
        chThdSleepMilliseconds(500);
    }

    return (msg_t)NULL;
}

int main(void) {
    halInit();
    chSysInit();

    chThdCreateStatic(waThreadHB, sizeof(waThreadHB), NORMALPRIO,
                      ThreadHeartbeat, NULL);

    /*
    chThdCreateStatic(waMS5611, sizeof(waMS5611), NORMALPRIO,
                      ms5611_thread, NULL);

    chThdCreateStatic(waADXL345, sizeof(waADXL345), NORMALPRIO,
                      adxl345_thread, NULL);

    chThdCreateStatic(waADXL375, sizeof(waADXL375), NORMALPRIO,
                      adxl375_thread, NULL);
    */

    shellInit();
    sdStart(&SD2, NULL);
    sdcStart(&SDCD1, NULL);

    shellCreate(&shell_cfg, 2048, NORMALPRIO);

    chRegSetThreadName("main");
    chThdSetPriority(LOWPRIO);

    while (TRUE) {
        chThdSleepMilliseconds(1000);
    }
}
