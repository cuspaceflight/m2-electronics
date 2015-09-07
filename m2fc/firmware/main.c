/*
 * M2FC
 * 2014 Adam Greig, Cambridge University Spaceflight
 */

#include <stdio.h>
#include <string.h>

#include "ch.h"
#include "hal.h"

#include "ff.h"

#include "ms5611.h"
#include "adxl3x5.h"
#include "config.h"
#include "pyro.h"
#include "microsd.h"
#include "m2fc_shell.h"
#include "mission.h"
#include "state_estimation.h"
#include "sbp_io.h"
#include "analogue.h"
#include "l3g4200d.h"
#include "hmc5883l.h"

/*#include "dma1_stream0_mutex.h"*/

/* Create working areas for all threads */
static WORKING_AREA(waMS5611, 512);
static WORKING_AREA(waADXL345, 512);
static WORKING_AREA(waADXL375, 512);
static WORKING_AREA(waMission, 1024);
static WORKING_AREA(waThreadHB, 128);
static WORKING_AREA(waMicroSD, 512);
static WORKING_AREA(waPyros, 128);
static WORKING_AREA(waThreadSBP, 1024);
static WORKING_AREA(waAnalogue, 512);
static WORKING_AREA(waHMC5883L, 512);
static WORKING_AREA(waL3G4200D, 1024);


Mutex dma1_stream0_mutex_object;
Mutex* dma1_stream0_mutex = &dma1_stream0_mutex_object;

/*
 * Heatbeat thread.
 * This thread flashes the everything-is-OK LED once a second,
 * and keeps resetting the watchdog timer for us.
 */
static msg_t ThreadHeartbeat(void *arg) {
    (void)arg;
    chRegSetThreadName("Heartbeat");

    while(TRUE) {
        /* Set the STATUS onboard LED */
        palSetPad(GPIOA, GPIOA_LED_STATUS);
        /* Set the GREEN external LED */
        palSetPad(GPIOC, GPIOC_LED_C);
        palClearPad(GPIOC, GPIOC_LED_A);
        /* Flash them briefly */
        chThdSleepMilliseconds(10);

        /* Turn LEDs off */
        palClearPad(GPIOA, GPIOA_LED_STATUS);
        palClearPad(GPIOC, GPIOC_LED_C);
        palClearPad(GPIOC, GPIOC_LED_A);
        /* Clear watchdog timer */
        IWDG->KR = 0xAAAA;
        chThdSleepMilliseconds(490);
        IWDG->KR = 0xAAAA;
        chThdSleepMilliseconds(500);
    }

    return (msg_t)NULL;
}

/*
 * Set up pin change interrupts for the various sensors that react to them.
 */
static const EXTConfig extcfg = {{
    {EXT_CH_MODE_DISABLED, NULL}, /* Pin 0 - PE0 is the magnetometer DRDY */
    {EXT_CH_MODE_DISABLED, NULL}, /* Pin 1 */
    {EXT_CH_MODE_DISABLED, NULL}, /* Pin 2 */
    {EXT_CH_MODE_DISABLED, NULL}, /* Pin 3 */
    {EXT_CH_MODE_DISABLED, NULL}, /* Pin 4 */
    {EXT_CH_MODE_AUTOSTART | EXT_CH_MODE_RISING_EDGE | EXT_MODE_GPIOC,
        adxl375_wakeup},          /* Pin 5 - PC5 is the HG accel INT1 */
    {EXT_CH_MODE_DISABLED, NULL}, /* Pin 6 */
    {EXT_CH_MODE_DISABLED, NULL}, /* Pin 7 */
    {EXT_CH_MODE_DISABLED, NULL}, /* Pin 8 */
    {EXT_CH_MODE_AUTOSTART | EXT_CH_MODE_RISING_EDGE | EXT_MODE_GPIOD,
        adxl345_wakeup},          /* Pin 9 - PD9 is the LG accel INT1 */
    {EXT_CH_MODE_DISABLED, NULL}, /* Pin 10 */
    {EXT_CH_MODE_DISABLED, NULL}, /* Pin 11 */
    {EXT_CH_MODE_DISABLED, NULL}, /* Pin 12 */
    {EXT_CH_MODE_DISABLED, NULL}, /* Pin 13 */
    {EXT_CH_MODE_DISABLED, NULL}, /* Pin 14 */
    {EXT_CH_MODE_DISABLED, NULL}, /* Pin 15 */
    {EXT_CH_MODE_DISABLED, NULL}, /* 16 - PVD */
    {EXT_CH_MODE_DISABLED, NULL}, /* 17 - RTC Alarm */
    {EXT_CH_MODE_DISABLED, NULL}, /* 18 - USB OTG FS Wakeup */
    {EXT_CH_MODE_DISABLED, NULL}, /* 19 - Ethernet Wakeup */
    {EXT_CH_MODE_DISABLED, NULL}, /* 20 - USB OTG HS Wakeup */
    {EXT_CH_MODE_DISABLED, NULL}, /* 21 - RTC Tamper/Timestamp */
    {EXT_CH_MODE_DISABLED, NULL}  /* 22 - RTC Wakeup */
}};

void test_deployment(void)
{
    int i;

    bool_t cont = true;
    cont &= pyro_continuity(PYRO_1);
    cont &= pyro_continuity(PYRO_2);
    cont &= pyro_continuity(PYRO_3);

    if(!cont) {
        while(1) {
            palClearPad(GPIOC, GPIOC_LED_A);
            palSetPad(GPIOC, GPIOC_LED_C);
            palSetPad(GPIOA, GPIOA_LED_STATUS);
            chThdSleepMilliseconds(100);
            
            palClearPad(GPIOC, GPIOC_LED_A);
            palClearPad(GPIOC, GPIOC_LED_C);
            palClearPad(GPIOA, GPIOA_LED_STATUS);
            chThdSleepMilliseconds(100);
        }
    }

    for(i = 0; i < 10; i++) {
        palClearPad(GPIOC, GPIOC_LED_C);
        palSetPad(GPIOC, GPIOC_LED_A);
        palSetPad(GPIOA, GPIOA_LED_STATUS);
        chThdSleepMilliseconds(1000);
        
        palClearPad(GPIOC, GPIOC_LED_C);
        palClearPad(GPIOC, GPIOC_LED_A);
        palClearPad(GPIOA, GPIOA_LED_STATUS);
        chThdSleepMilliseconds(1000);
    }

    for(i=0; i<5; i++) {
        palClearPad(GPIOC, GPIOC_LED_A);
        palSetPad(GPIOC, GPIOC_LED_C);
        palSetPad(GPIOA, GPIOA_LED_STATUS);
        chThdSleepMilliseconds(200);
        
        palClearPad(GPIOC, GPIOC_LED_A);
        palClearPad(GPIOC, GPIOC_LED_C);
        palClearPad(GPIOA, GPIOA_LED_STATUS);
        chThdSleepMilliseconds(200);
    }

    for(i=0; i<50; i++) {
        pyro_fire(1, 1, 1, 9);
    }

    palClearPad(GPIOC, GPIOC_LED_C);
    palSetPad(GPIOC, GPIOC_LED_A);

    while(1);

}

/*
 * M2FC Main Thread.
 * Starts all the other threads then puts itself to sleep.
 */
int main(void) {
    halInit();
    chSysInit();
    chRegSetThreadName("Main");
    

    /* initilices the mutex*/
    chMtxInit(dma1_stream0_mutex);

    /* Start the heartbeat thread so it will be resetting the watchdog. */
    chThdCreateStatic(waThreadHB, sizeof(waThreadHB), LOWPRIO,
                      ThreadHeartbeat, NULL);


    /* Configure and enable the watchdog timer */
    DBGMCU->APB1FZ |= DBGMCU_APB1_FZ_DBG_IWDG_STOP;
    IWDG->KR = 0x5555;
    IWDG->PR = 3;
    IWDG->KR = 0xCCCC;

    /* Various module initialisation */
    state_estimation_init();
    config_read_location();

    /* Activate the EXTI pin change interrupts */
    extStart(&EXTD1, &extcfg);

    /* Start module threads */
    chThdCreateStatic(waMicroSD, sizeof(waMicroSD), HIGHPRIO,
                      microsd_thread, NULL);

    #ifdef RUN_THREADS
    
    chThdCreateStatic(waMission, sizeof(waMission), NORMALPRIO,
                      mission_thread, NULL);

    chThdCreateStatic(waMS5611, sizeof(waMS5611), NORMALPRIO,
                      ms5611_thread, NULL);

    chThdCreateStatic(waADXL345, sizeof(waADXL345), NORMALPRIO,
                      adxl345_thread, NULL);

    chThdCreateStatic(waADXL375, sizeof(waADXL375), NORMALPRIO,
                      adxl375_thread, NULL);

    chThdCreateStatic(waPyros, sizeof(waPyros), NORMALPRIO,
                      pyro_continuity_thread, NULL);

    chThdCreateStatic(waThreadSBP, sizeof(waThreadSBP), NORMALPRIO,
                      sbp_thread, NULL);
                      
    #endif /*RUN_THREADS*/
    
    #ifdef TEST_MUTEX 
    chThdCreateStatic(waHMC5883L, sizeof(waHMC5883L), NORMALPRIO,
                          hmc5883l_thread, NULL);
    
    chThdCreateStatic(waL3G4200D, sizeof(waL3G4200D), NORMALPRIO,
                          l3g4200d_thread, NULL);
    
    #endif /*TEST_MUTEX*/
    
    chThdCreateStatic(waAnalogue, sizeof(waAnalogue), NORMALPRIO,
                      analogue_thread, NULL);       
      
    /* Cannot enable magno and radio at same time without resolving
     * the DMA channel conflict with a lock etc
     */
     
     /*
    if(USE_MAGNO) {
        chThdCreateStatic(waHMC5883L, sizeof(waHMC5883L), NORMALPRIO,
                          hmc5883l_thread, NULL);
    }
    
    if(USE_GYRO) {
        chThdCreateStatic(waL3G4200D, sizeof(waL3G4200D), NORMALPRIO,
                          l3g4200d_thread, NULL);
    }
    
    */
    
    
              
    /* Start the command shell on the slave serial port */
    m2fc_shell_run();

    /* Let the main thread idle now. */
    chThdSetPriority(LOWPRIO);
    chThdSleep(TIME_INFINITE);

    return 0;
}
