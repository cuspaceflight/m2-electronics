/*
 * M2FC Shell Commands
 * M2FC
 * 2014 Adam Greig, Cambridge University Spaceflight
 */
#include "m2fc_shell.h"
#include "hal.h"
#include "chprintf.h"
#include "ff.h"

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

  chprintf(chp, "name          addr    stack prio refs     state   time\r\n");
  chprintf(chp, "------------------------------------------------------\r\n");
  tp = chRegFirstThread();
  do {
    chprintf(chp, "%10s %.8lx %.8lx %4lu %4lu %9s %lu\r\n",
            chRegGetThreadName(tp),
            (uint32_t)tp, (uint32_t)tp->p_ctx.r13,
            (uint32_t)tp->p_prio, (uint32_t)(tp->p_refs - 1),
            states[tp->p_state], (uint32_t)tp->p_time);
    tp = chRegNextThread(tp);
  } while (tp != NULL);
}

static void cmd_rt(BaseSequentialStream *chp, int argc, char *argv[]) {
    (void)argv;
    if(argc > 0) {
        chprintf(chp, "Usage: rt\r\n");
        chprintf(chp, "Prints current realtime clock ticks and frequency\r\n");
    }
    halrtcnt_t t = halGetCounterValue();
    halclock_t f = halGetCounterFrequency();
    chprintf(chp, "Current real time clock ticks: %u\r\n", t);
    chprintf(chp, "Real time clock frequency: %u\r\n", f);
}

static FATFS SDC_FS;
static bool_t fs_ready = FALSE;

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

void m2fc_shell_run()
{
    static const ShellCommand commands[] = {
        {"mem", cmd_mem},
        {"threads", cmd_threads},
        {"rt", cmd_rt},
        {"mount", cmd_mount},
        {"unmount", cmd_unmount},
        {"hello", cmd_hello},
        {NULL, NULL}
    };
    static const ShellConfig shell_cfg = {
      (BaseSequentialStream *)&SD2,
      commands
    };
    shellInit();
    sdStart(&SD2, NULL);
    shellCreate(&shell_cfg, 2048, NORMALPRIO);
}

