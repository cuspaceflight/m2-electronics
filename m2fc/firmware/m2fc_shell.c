/*
 * M2FC Shell Commands
 * M2FC
 * 2014 Adam Greig, Cambridge University Spaceflight
 * 2015 Eivind Roson Eide, Cambridge University Spaceflight
 */
#include "m2fc_shell.h"
#include <hal.h>
#include <string.h>
#include "chprintf.h"
#include "hmc5883l.h"
#include "l3g4200d.h" 
#include "ms5611.h"
#include "adxl3x5.h"
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

  chprintf(chp,
    "name        |addr    |stack   |free|prio|refs|state    |time\r\n");
  chprintf(chp,
    "------------|--------|--------|----|----|----|---------|--------\r\n");
  tp = chRegFirstThread();
  do {
    chprintf(chp, "%12s|%.8lx|%.8lx|%4lu|%4lu|%4lu|%9s|%lu\r\n",
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

static void cmd_gyro(BaseSequentialStream *chp, int argc, char *argv[]) {
    (void)argc;	
    (void)argv;

    chprintf(chp, "Gyro readings:\n  x         y         z\n");
    chprintf(chp,"%09d %09d %09d\n", global_gyro[0], global_gyro[1], global_gyro[2]);
    return;
}

static void cmd_baro(BaseSequentialStream *chp, int argc, char *argv[]) {
    (void)argv;
    (void)argc;
    
    chprintf(chp, "Current Pressure: %d\r\n", pressure);
    chprintf(chp, "Current Temperature: %d\r\n", temperature);
}

static void cmd_magno(BaseSequentialStream *chp, int argc, char *argv[]) {
    (void)argv;
    (void)argc;
    chprintf(chp, "Magno readings:\n  x         y         z\n");
    chprintf(chp,"%09d %09d %09d \n", global_magno[0], global_magno[1], global_magno[2]);
}

static void cmd_accel(BaseSequentialStream *chp, int argc, char *argv[]) {
    (void)argc;	
    (void)argv;
    
    chprintf(chp, "Accel readings:\n x     y     z\n");
    chprintf(chp," x: %d, y: %d, z: %d \n", accels[0], accels[1], accels[2]);
    return;
}

static void cmd_pyro(BaseSequentialStream *chp, int argc, char *argv[]) {
    bool p1, p2, p3;
    
    /* Continuity Check */

    p1 = (int16_t)pyro_continuity(PYRO_1);
    p2 = (int16_t)pyro_continuity(PYRO_2);
    p3 = (int16_t)pyro_continuity(PYRO_3);
    
    chprintf(chp, "Results of Continuity Check: 1:%u, 2:%u, 3:%u\n",
             p1, p2, p3);

    if(argc == 1 && strcmp(argv[0], "fire") == 0) {
        int i;
        for(i=5; i>0; i--) {
            chprintf(chp, "Firing pyros in %d...\n", i);
            chThdSleepMilliseconds(1000);
        }
        pyro_fire(true, true, true);
    }
}

void m2fc_shell_run()
{
    static const ShellCommand commands[] = {
        {"mem", cmd_mem},
        {"threads", cmd_threads},
        {"rt", cmd_rt},
        {"magno", cmd_magno},
        {"gyro", cmd_gyro},
        {"baro", cmd_baro},
        {"accel", cmd_accel},
        {"pyro", cmd_pyro},
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
