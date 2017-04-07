/*
 * M2R Shell Commands
 * M2R
 * 2014 Adam Greig, Cambridge University Spaceflight
 */
#include "m2r_shell.h"
#include <hal.h>
#include "chprintf.h"
#include <stdio.h>
#include "m2status.h"
#include "pyro.h"

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
  uint64_t busy = 0, total = 0;
  Thread *tp;

  (void)argv;
  if (argc > 0) {
    chprintf(chp, "Usage: threads\r\n");
    return;
  }

  chprintf(
    chp, "name      |addr    |stack   |free|prio|refs|state    |time\r\n");
  chprintf(
    chp, "----------|--------|--------|----|----|----|---------|--------\r\n");
  tp = chRegFirstThread();
  do {
    chprintf(chp, "%10s|%.8lx|%.8lx|%4lu|%4lu|%4lu|%9s|%lu\r\n",
            chRegGetThreadName(tp),
            (uint32_t)tp, (uint32_t)tp->p_ctx.r13,
            (uint32_t)tp->p_ctx.r13 - (uint32_t)tp->p_stklimit,
            (uint32_t)tp->p_prio, (uint32_t)(tp->p_refs - 1),
            states[tp->p_state], (uint32_t)tp->p_time);
    if(tp->p_prio != 1) {
        busy += tp->p_time;
    }
    total += tp->p_time;
    tp = chRegNextThread(tp);
  } while (tp != NULL);
  chprintf(chp, "CPU Usage: %ld%%\r\n", busy*100/total);
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

static void cmd_version(BaseSequentialStream *chp, int argc, char* argv[]) {
    (void)argc;
    (void)argv;
    chprintf(chp, "Version: " M2R_FIRMWARE_VERSION "\r\n");
}

static void cmd_pyro_cont(BaseSequentialStream *chp, int argc, char* argv[]) {
    (void)argc;
    (void)argv;
    chprintf(chp, "Pyro continuity: %d, fired: %d\r\n", pyro_cont, pyro_fired);
}

static void cmd_pyro_fire(BaseSequentialStream *chp, int argc, char* argv[]) {
    (void)argc;
    (void)argv;
    chprintf(chp, "Firing...\r\n");
    pyro_fire();
    chprintf(chp, "Fired.\r\n");
}

struct SemihostingVMT {
    _base_sequential_stream_methods
};

static size_t sh_write(void *ip, const uint8_t *bp, size_t n)
{
    (void)ip;
    return fwrite(bp, sizeof(uint8_t), n, stdout);
}

static size_t sh_read(void *ip, uint8_t *bp, size_t n)
{
    (void)ip;
    return fread(bp, sizeof(uint8_t), n, stdin);
}

static msg_t sh_put(void *ip, uint8_t b)
{
    (void)ip;
    return putchar(b);
}

static msg_t sh_get(void *ip)
{
    (void)ip;
    return getchar();
}

static const struct SemihostingVMT vmt = {sh_write, sh_read, sh_put, sh_get};

typedef struct {
    const struct SemihostingVMT *vmt;
} Semihosting;

Semihosting SH = {&vmt};

void m2r_shell_run(BaseSequentialStream *bss)
{
    static const ShellCommand commands[] = {
        {"mem", cmd_mem},
        {"threads", cmd_threads},
        {"rt", cmd_rt},
        {"status", m2status_shell_cmd},
        {"version", cmd_version},
        {"pyro_cont", cmd_pyro_cont},
        {"pyro_fire", cmd_pyro_fire},
        {NULL, NULL}
    };

    static ShellConfig shell_cfg;
    shell_cfg.sc_channel = bss;
    shell_cfg.sc_commands = commands;
    shellInit();
    shellCreate(&shell_cfg, 2048, NORMALPRIO);
}
