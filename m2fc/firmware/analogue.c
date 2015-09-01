/*
 * ADC Driver (ADC1, ADC2, ADC3)
 * M2FC
 * 2015 Eivind Roson Eide, Cambridge University Spaceflight
 */

#include "analogue.h"

#include <string.h>

#include "microsd.h"
#include "config.h"
#include "state_estimation.h"
#include "chprintf.h"


#define ADC_NUM_CHANNELS   2            
#define ADC_BUF_DEPTH      1024         


//Don't think I will need this
#define SG1_PIN GPIOA_STRAIN_1          //PA0 = ADC IN0    
#define SG2_PIN GPIOA_STRAIN_2          //PA1 = ADC IN1
#define SG3_PIN GPIOA_STRAIN_3          //PA2 = ADC IN2
#define TC1_PIN GPIOC_THERMO_1          //PC1 = ADC IN11
#define TC2_PIN GPIOC_THERMO_2          //PC2 = ADC IN12    
#define TC3_PIN GPIOC_THERMO_3          //PC3 = ADC IN13


#define SG1_CHN     ADC_CHANNEL_IN0          //PA0 = ADC IN0    
#define SG2_CHN     ADC_CHANNEL_IN1          //PA1 = ADC IN1
#define SG3_CHN     ADC_CHANNEL_IN2          //PA2 = ADC IN2
#define TC1_CHN     ADC_CHANNEL_IN10         //PC1 = ADC IN11
#define TC2_CHN     ADC_CHANNEL_IN11         //PC2 = ADC IN12    
#define TC3_CHN     ADC_CHANNEL_IN12         //PC3 = ADC IN13

#define ADC_MICROSD_CHN_SG   CHAN_SENS_SG 
#define ADC_MICROSD_CHN_TC   CHAN_SENS_TC 



static void gpt_adc_trigger(GPTDriver *gpt_ptr);
static void adccallback(ADCDriver *adcDriverpointer, adcsample_t *buffer, size_t n);
static void adcerrorcallback(ADCDriver *adcDriverpointer, adcerror_t err);
static void save_results(void);


//Create arrays with the sample data from ADCs
static adcsample_t samples1[ADC_NUM_CHANNELS * ADC_BUF_DEPTH];
static adcsample_t samples2[ADC_NUM_CHANNELS * ADC_BUF_DEPTH];
static adcsample_t samples3[ADC_NUM_CHANNELS * ADC_BUF_DEPTH];

static Thread *tp = NULL;

/* 
 * Configure a GPT object 
 */ 

static const GPTConfig gpt_adc_config = 
{ 
     40000,  /* timer clock: 40khz */
     gpt_adc_trigger,  /* Timer callback function */
     0
};


static volatile int numberOfBuffersReady = 0;  /*This number keeps track on how many times the full buffer callback has been called since last save. */

/*
* ADC conversion group.
* Mode:        Continuous, 1024 samples of 2 channels,
* 
* What does this mean:?
* TIMER 3 triggers adc1, 
* 
* !!!TODO!!! the others should be triggered by adc1
* 
* Channels:    {IN0, IN11} (1)
*              {IN1, IN12} (2)
*              {IN2, IN13} (3)
*/

static const ADCConversionGroup adcConGrp1 = {
    TRUE,
    ADC_NUM_CHANNELS,
    adccallback,
    adcerrorcallback,
    /* cr1   */
    0,                                           /*I believe all the bits should be set to 0 */
    /*cr2*/
    ADC_CR2_EXTSEL_SRC(8) | ADC_CR2_EXTEN_0,     /*this lines is supposed to clock the ADC to timer 3 TRGO event*/
    /* smpr1 */
    ADC_SMPR1_SMP_AN11(2) |         /*The following lines will set all of them to 40 cycles per conversion (28 + 12) */
    ADC_SMPR1_SMP_AN12(2) |
    ADC_SMPR1_SMP_AN13(2), 
    /* smpr2*/  
    ADC_SMPR2_SMP_AN0(2) |
    ADC_SMPR2_SMP_AN1(2) |     
    ADC_SMPR2_SMP_AN2(2),                                
    /* sqr1 */
    ADC_SQR1_NUM_CH(2),
    /* sqr2 */
    0,
    /* sqr3 */
    ADC_SQR3_SQ1_N(SG1_CHN) | ADC_SQR3_SQ2_N(TC1_CHN) 
}; 

static const ADCConversionGroup adcConGrp2 = {   
    TRUE,               /*Should probably define this to be called when the others are called? */
    ADC_NUM_CHANNELS,
    adccallback,
    adcerrorcallback,
    /* cr1   */
    0,                                           /*I believe all the bits should be set to 0*/
    /*cr2*/
    ADC_CR2_EXTSEL_SRC(8) | ADC_CR2_EXTEN_0,     /*this lines is supposed to clock the ADC to timer 3 TRGO event*/
    /* smpr1 */
    ADC_SMPR1_SMP_AN11(2) |         /*The following lines will set all of them to 40 cycles per conversion (28 + 12) */
    ADC_SMPR1_SMP_AN12(2) |
    ADC_SMPR1_SMP_AN13(2), 
    /* smpr2*/  
    ADC_SMPR2_SMP_AN0(2) |
    ADC_SMPR2_SMP_AN1(2) |     
    ADC_SMPR2_SMP_AN2(2),                                
    /* sqr1 */
    ADC_SQR1_NUM_CH(2),
    /* sqr2 */
    0,
    /* sqr3 */
    ADC_SQR3_SQ1_N(SG2_CHN) | ADC_SQR3_SQ2_N(TC2_CHN) 
}; 

/*Should probably define this to be called when the others are called? */
static const ADCConversionGroup adcConGrp3 = {   
    TRUE,
    ADC_NUM_CHANNELS,
    adccallback,
    adcerrorcallback,
    
    /* cr1   */
    0,                                           /*I believe all the bits should be set to 0*/
    
    /*cr2*/
    ADC_CR2_EXTSEL_SRC(8) | ADC_CR2_EXTEN_0,     /*this lines is supposed to clock the ADC to timer 3 TRGO event */
    
    /* smpr1 */
    ADC_SMPR1_SMP_AN11(2) |         /*The following lines will set all of them to 40 cycles per conversion (28 + 12) */
    ADC_SMPR1_SMP_AN12(2) |
    ADC_SMPR1_SMP_AN13(2), 
    
    /* smpr2*/  
    ADC_SMPR2_SMP_AN0(2) |
    ADC_SMPR2_SMP_AN1(2) |     
    ADC_SMPR2_SMP_AN2(2),                                
    /* sqr1 */
    ADC_SQR1_NUM_CH(2),
    
    /* sqr2 */
    0,
    
    /* sqr3 */
    ADC_SQR3_SQ1_N(SG3_CHN) | ADC_SQR3_SQ2_N(TC3_CHN) 
}; 
 

static void gpt_adc_trigger(GPTDriver *gpt_ptr) {
    (void)gpt_ptr;
}


/*Question? Is the buffer some kind of array with type abcsample_t/ uint16_t? Or */

static void adccallback(ADCDriver *adcDriverpointer, adcsample_t *buffer, size_t n) {
    (void)adcDriverpointer;
    (void)buffer;   /*I hope this doesn't delete everything */
    (void)n; 
    
    if (numberOfBuffersReady < 2)  /*Checks if all the buffers are ready. i.e. the two others */
    {
        numberOfBuffersReady++;
    }
    
    else
    {
        numberOfBuffersReady = 0;
        chSysLockFromIsr();
        if(tp != NULL && tp->p_state != THD_STATE_READY) {
            chSchReadyI(tp);
        }
        chSysUnlockFromIsr();
    }
}

static void save_results()
{
    int16_t size = (int16_t)(ADC_BUF_DEPTH / ADC_NUM_CHANNELS);
    int16_t i = 0;
    int16_t j = 0;
    const int16_t MAX_i = 10;
    
  
    while(j < size)
    {
        /*read and log all the SG samples */
        microsd_log_s16(ADC_MICROSD_CHN_SG, samples1[j], samples2[j], samples3[j], 0); /*are these correctly adressed */
        
        
        /*1 out of 10 samples are then saved to microSD_card with */
        if (i == MAX_i)
        {
            i = 0;  /*resets count to 0 */
            microsd_log_s16(ADC_MICROSD_CHN_TC, samples1[j+1], samples2[j+1], samples3[j+1], 0);  /*Check this, do not think this is correct */
        }
        
        i ++;
        j += 2;
    }    
}

static void adcerrorcallback(ADCDriver *adcDriverpointer, adcerror_t err) {
  (void)adcDriverpointer;
  (void)err;
  
  /*could possibly implement some kind of reset function that turns of the driver and restarts it */
}



msg_t analogue_thread(void *args)
{
    (void)args;

    chRegSetThreadName("Analogue");

    adcInit();
    adcStart(&ADCD1, NULL);
    adcStart(&ADCD2, NULL);
    adcStart(&ADCD3, NULL);
    gptStart(&GPTD3, &gpt_adc_config); /*What is &GPTD3 ? - General Purpose Timer #3 (aka Timer 3) -AG */
    
    adcStartConversion(&ADCD1, &adcConGrp1, samples1, ADC_BUF_DEPTH);
    adcStartConversion(&ADCD2, &adcConGrp2, samples2, ADC_BUF_DEPTH);
    adcStartConversion(&ADCD3, &adcConGrp3, samples3, ADC_BUF_DEPTH);

    /*
    * Start the GPT timer 
    * Timer is clocked at 1Mhz (1us). Timer triggers at 1 and calls the callback function 
    */
    GPTD3.tim->CR2 |= STM32_TIM_CR2_MMS(2);
    gptStartContinuous(&GPTD3, 2); /*Is this the way to do it? */

    while(true) {
        chSysLock();
        tp = chThdSelf();
        chSchGoSleepS(THD_STATE_SUSPENDED);
        chSysUnlock();
        save_results();
    }

    return (msg_t)NULL;  /*What happens when this is reached? Should there be a infinite while loop */
} 












/* Usefull links: 
 * https://sites.google.com/site/electronicprojs/stm32-chibios-adc-sampling
 * http://www.chibios.org/dokuwiki/doku.php?id=chibios:howtos:createthread
 * http://www.playembedded.org/index.php?lang=en&mod=tut&sec=chi&id=003
 * https://blog.udemy.com/embedded-c-tutorial/
 * http://forum.chibios.org/phpbb/viewtopic.php?f=3&t=1373
 * http://www.scriptoriumdesigns.com/embedded/interrupts.php
 * http://forum.chibios.org/phpbb/viewtopic.php?f=3&t=1373
 * 
 * 
 * Less usefull links
 * http://xmodulo.com/good-ide-for-c-cpp-linux.html
 * 
*/


