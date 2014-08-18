#include "ch.h"
#include "hal.h"
#include "test.h"

/*
 * This is a periodic thread that does absolutely nothing except flashing
 * a LED.
 */
static WORKING_AREA(waThread1, 128);
static msg_t Thread1(void *arg) {

  (void)arg;
  chRegSetThreadName("blinker");
  while (TRUE) {
    palSetPad(GPIOA, GPIOA_LED_4);       /* Orange.  */
    chThdSleepMilliseconds(500);
    palClearPad(GPIOA, GPIOA_LED_4);     /* Orange.  */
    chThdSleepMilliseconds(500);
  }
}

/*
 * Application entry point.
 */
int main(void) {

  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   */
  halInit();
  chSysInit();

  /*
   * Activates the serial driver 2 using the driver default configuration.
   * PA2(TX) and PA3(RX) are routed to USART2.
   */
  sdStart(&SD2, NULL);

  /*
   * If the user button is pressed after the reset then the test suite is
   * executed immediately before activating the various device drivers in
   * order to not alter the benchmark scores.
   */
  /*if (palReadPad(GPIOA, GPIOA_BUTTON))*/
  TestThread(&SD2);

  /*
   * Creates the example thread.
   */
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);

  /*
   * Normal main() thread activity, in this demo it does nothing except
   * sleeping in a loop and check the button state, when the button is
   * pressed the test procedure is launched with output on the serial
   * driver 2.
   */
  while (TRUE) {
    chThdSleepMilliseconds(500);
  }
}
