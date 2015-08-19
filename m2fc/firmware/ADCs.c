
/*
 * ADC Driver (ADC1, ADC2, ADC3)
 * M2FC
 * 2015 Eivind Roson Eide, Cambridge University Spaceflight
 */

#include <stdlib.h>
#include "ADCs.h"

#include <adc_lld.h>
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


#define ADC_MICROSD_CHN_SG //---- TODO  ---  ///
#define ADC_MICROSD_CHN_TC //---- TODO  ---  ///


//Create arrays with the sample data from ADCs
static adcsample_t samples1[ADC_GRP_NUM_CHANNELS * ADC_GRP_BUF_DEPTH];
static adcsample_t samples2[ADC_GRP_NUM_CHANNELS * ADC_GRP_BUF_DEPTH];
static adcsample_t samples3[ADC_GRP_NUM_CHANNELS * ADC_GRP_BUF_DEPTH];

/* 
 * Configure a GPT object 
 */ 

static GPTConfig gpt_adc_config = 
{ 
     40000,  // timer clock: 40khz 
     gpt_adc_trigger  // Timer callback function 
};

static numberOfBuffersReady = 0;  //This number keeps track on how many times the full buffer callback has been called since last save.

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

static const ADCConversionGroup adcConGrp = {
    TRUE,
    ADC_NUM_CHANNELS,
    adccallback,
    adcerrorcallback,
    /* cr1   */
    0,                                           //I believe all the bits should be set to 0
    /*cr2*/
    ADC_CR2_EXTSEL_SRC(8) | ADC_CR2_EXTEN_0,     //this lines is supposed to clock the ADC to timer 3 TRGO event
    /* smpr1 */
    ADC_SMPR1_SMP_AN11(2) |         //The following lines will set all of them to 40 cycles per conversion (28 + 12)
    ADC_SMPR1_SMP_AN12(2) |
    ADC_SMPR1_SMP_AN13(2), 
    /* smpr2*/  
    ADC_SMPR2_SMP_AN0(2) |
    ADC_SMPR2_SMP_AN1(2) |     
    ADC_SMPR2_SMP_AN2(2),                                
    /* sqr1 */
    0,
    /* sqr2 */
    0,
    /* sqr3 */
    ADC_SQR1_SQ1_N(SG1) | ADC_SQR1_SQ1_N(TC1) 
}; 

static const ADCConversionGroup adcConGrp2 = {   /Should probably define this to be called when the others are called?
    TRUE,
    ADC_NUM_CHANNELS,
    adccallback,
    adcerrorcallback,
    /* cr1   */
    0,                                           //I believe all the bits should be set to 0
    /*cr2*/
    ADC_CR2_EXTSEL_SRC(8) | ADC_CR2_EXTEN_0,     //this lines is supposed to clock the ADC to timer 3 TRGO event
    /* smpr1 */
    ADC_SMPR1_SMP_AN11(2) |         //The following lines will set all of them to 40 cycles per conversion (28 + 12)
    ADC_SMPR1_SMP_AN12(2) |
    ADC_SMPR1_SMP_AN13(2), 
    /* smpr2*/  
    ADC_SMPR2_SMP_AN0(2) |
    ADC_SMPR2_SMP_AN1(2) |     
    ADC_SMPR2_SMP_AN2(2),                                
    /* sqr1 */
    0,
    /* sqr2 */
    0,
    /* sqr3 */
    ADC_SQR1_SQ1_N(SG2) | ADC_SQR1_SQ1_N(TC2) 
}; 

static const ADCConversionGroup adcConGrp3 = {   /Should probably define this to be called when the others are called?
    TRUE,
    ADC_NUM_CHANNELS,
    adccallback,
    adcerrorcallback,
    /* cr1   */
    0,                                           //I believe all the bits should be set to 0
    /*cr2*/
    ADC_CR2_EXTSEL_SRC(8) | ADC_CR2_EXTEN_0,     //this lines is supposed to clock the ADC to timer 3 TRGO event
    /* smpr1 */
    ADC_SMPR1_SMP_AN11(2) |         //The following lines will set all of them to 40 cycles per conversion (28 + 12)
    ADC_SMPR1_SMP_AN12(2) |
    ADC_SMPR1_SMP_AN13(2), 
    /* smpr2*/  
    ADC_SMPR2_SMP_AN0(2) |
    ADC_SMPR2_SMP_AN1(2) |     
    ADC_SMPR2_SMP_AN2(2),                                
    /* sqr1 */
    0,
    /* sqr2 */
    0,
    /* sqr3 */
    ADC_SQR1_SQ1_N(SG3) | ADC_SQR1_SQ1_N(TC3) 
}; 
 

static void gpt_adc_trigger(GPTDriver *gpt_ptr) { //Don't think I need this
    (void)gpt_ptr;
    //chSysLockFromISR();
    //chSysUnlockFromISR();
}


//Question? Is the buffer some kind of array with type abcsample_t/ uint16_t? Or 

static void adccallback(ADCDriver *adcDriverpointer, adcsample_t *buffer, size_t n) {
    (void)adcDriverpointer;
    (void)buffer; //I hope this doesn't delete everything
    
    if (numberOfBuffersReady < 2)  //Checks if all the buffers are ready. i.e. the two others
    {
        numberOfBuffersReady++;
    }
    
    else
    {
        numberOfBuffersReady = 0;
        saveResults(n);
    }
}

static void saveResults(size_t n)
{
    int16_t SIZE = (int16_t)n;
    int16_t i = 0;
    int16_t j = 0;
    const int16_t MAX_i = 10;
    
  
    while(j < SIZE)
    {
        //read and log all the SG samples
        microsd_log_s16(ADC_MICROSD_CHN_SG, samples1[j], samples2[j], samples3[j], 0); //are these correctly adressed
        
        
        //1 out of 10 samples are then saved to microSD_card with 
        if (i == MAX_i)
        {
            i = 0;  //resets count to 0
            microsd_log_s16(ADC_MICROSD_CHN_TC, samples1[j+1], samples2[j+1], samples3[j+1], 0);  //Check this, do not think this is correct
        }
        
        i ++;
        j += 2;
    }    
}

static void adc_error_callback(ADCDriver *adcDriverpointer, adcerror_t err) {
  (void)adcp;
  (void)err;
  
  //could possibly implement some kind of reset function that turns of the 
}



msg_t ADC_read_SGs_and_TCs (void *args)
{
    (void) args;
    
    const ADCConfig adcconfig = {};
            
    chRegSetThreadName("ADCs");
    
    adcInit();
    adcStart(&ADCD1, &adcconfig);
    adcStart(&ADCD2, &adcconfig);
    adcStart(&ADCD3, &adcconfig);
    
    adcStartConversion(&ADCD1, &adcConGrp1, samples1, ADC_BUF_DEPTH)
    adcStartConversion(&ADCD2, &adcConGrp2, samples2, ADC_BUF_DEPTH)
    adcStartConversion(&ADCD3, &adcConGrp3, samples3, ADC_BUF_DEPTH)
    
    /* 
    * Start the GPT timer 
    * Timer is clocked at 1Mhz (1us). Timer triggers at 1 and calls the callback function 
    */ 
    gptStart(&GPTD1, &gpt_adc_config); 
    gptStartContinuous(&GPTD1, 1); //Is this the way to do it? 
    
    
    
    return (msg_t)NULL;  //What happens when this is reached? Should there be a infinite while loop
} 














#region  HIDE


/*
#region

#region waste

* http://www.st.com/st-web-ui/static/active/en/resource/technical/document/reference_manual/DM00031020.pdf page 374
 * Position, Priority, Type of priority, Acronym, Description, Address
 * 
 * 18 |25 |settable ADC| ADC1, ADC2 and ADC3 global interrupts | 0x0000 0088
 * 
*



/*
CH_IRQ_HANDLER(myIRQ) {
  CH_IRQ_PROLOGUE();
 
  / IRQ handling code, preemptable if the architecture supports it./
 
  chSysLockFromISR();
  / Invocation of some I-Class system APIs, never preemptable./
  chSysUnlockFromISR();
 
  /More IRQ handling code, again preemptable./
 
  CH_IRQ_EPILOGUE();
}


*

#endregion

 //#######NOT CODE###########/////
 
#region Copied for referance from http://forum.chibios.org/phpbb/viewtopic.php?f=4&t=7
//// Copied for referance from http://forum.chibios.org/phpbb/viewtopic.php?f=4&t=7

// ADCConfig structure for stm32 MCUs is empty
static ADCConfig adccfg = {};

// Create buffer to store ADC results. This is
// one-dimensional interleaved array
#define ADC_BUF_DEPTH 2 // depth of buffer
#define ADC_CH_NUM 2    // number of used ADC channels
static adcsample_t samples_buf[ADC_BUF_DEPTH * ADC_CH_NUM]; // results array

// Fill ADCConversionGroup structure fields
static ADCConversionGroup adccg = {
   // this 3 fields are common for all MCUs
      // set to TRUE if need circular buffer, set FALSE otherwise
      TRUE,
      // number of channels
      (uint16_t)(ADC_CH_NUM),
      // callback function, set to NULL for begin
      NULL,
   // Resent fields are stm32 specific. They contain ADC control registers data.
   // Please, refer to ST manual RM0008.pdf to understand what we do.
      // CR1 register content, set to zero for begin
      0,
      // CR2 register content, set to zero for begin
      0,
      // SMRP1 register content, set to zero for begin
      0,
      // SMRP2 register content, set to zero for begin
      0,
      // SQR1 register content. Set channel sequence length
      ((ADC_CH_NUM - 1) << 20),
      // SQR2 register content, set to zero for begin
      0,
      // SQR3 register content. We must select 2 channels
      // For example 15th and 10th channels. Refer to the
      // pinout of your MCU to select other pins you need.
      // On STM32-P103 board they connected to PC15 and PC0 contacts
      // On STM32-103STK board they connected to EXT2-7 contact and joystick
      (15 | (10 << 5)),
};

// Thats all with configuration

int main(int argc, char **argv) {
  (void)argc;
  (void)argv;

  // Setup pins of our MCU as analog inputs
  palSetPadMode(IOPORT3, 5, PAL_MODE_INPUT_ANALOG); // this is 15th channel
  palSetPadMode(IOPORT3, 0, PAL_MODE_INPUT_ANALOG); // this is 10th channel

  // Following 3 functions use previously created configuration
  // to initialize ADC block, start ADC block and start conversion.
  // &ADCD1 is pointer to ADC driver structure, defined in the depths of HAL.
  // Other arguments defined ourself earlier.
  adcInit();
  adcStart(&ADCD1, &adccfg);
  adcStartConversion(&ADCD1, &adccg, &samples_buf[0], ADC_BUF_DEPTH);

  // Thats all. Now your conversion run in background without assistance.

  uint16_t i = 0;

  while (TRUE) {
    i = samples_buf[0] / 2;
  }
  return 0;
}


#endregion

#region


 COPIED FROM http://forum.chibios.org/phpbb/viewtopic.php?f=2&t=939
#define ADC_GRP1_NUM_CHANNELS   2
#define ADC_GRP1_BUF_DEPTH      8

#define ADC_GRP2_NUM_CHANNELS   8
#define ADC_GRP2_BUF_DEPTH      16

static adcsample_t samples1[ADC_GRP1_NUM_CHANNELS * ADC_GRP1_BUF_DEPTH];
static adcsample_t samples2[ADC_GRP2_NUM_CHANNELS * ADC_GRP2_BUF_DEPTH];
//uint8_t iman=100;
/
/ ADC streaming callback.
/
size_t nx = 0, ny = 0;
static void adccallback(ADCDriver *adcp, adcsample_t *buffer, size_t n) {

  (void)adcp;
  if (samples2 == buffer) {
    nx += n;
  }
  else {
    ny += n;
  }
}

static void adcerrorcallback(ADCDriver *adcp, adcerror_t err) {

  (void)adcp;
  (void)err;
}

/
* ADC conversion group.
* Mode:        Linear buffer, 8 samples of 2 channels, SW triggered.
* Channels:    IN7, IN8.
/
static const ADCConversionGroup adcgrpcfg1 = {
  FALSE,
  ADC_GRP1_NUM_CHANNELS,
  NULL,
  adcerrorcallback,
  0,                                                            / CFGR     /
  ADC_TR(0, 4095),                                              / TR1     /
  ADC_CCR_DUAL(1),                                              / CCR       /
  {                                                             / SMPR[2]  /
    0,
    0
  },
  {                                                             / SQR[4]   /
    ADC_SQR1_SQ1_N(ADC_CHANNEL_IN7) | ADC_SQR1_SQ2_N(ADC_CHANNEL_IN8),
    0,
    0,
    0
  },
  {                                                             / SSMPR[2] /
    0,
    0
  },
  {                                                             / SSQR[4]  /
    ADC_SQR1_SQ1_N(ADC_CHANNEL_IN8) | ADC_SQR1_SQ2_N(ADC_CHANNEL_IN7),
    0,
    0,
    0
  }
};

/
* ADC conversion group.
* Mode:        Continuous, 16 samples of 8 channels, SW triggered.
* Channels:    IN7, IN8, IN7, IN8, IN7, IN8, Sensor, VBat/2.
/
static const ADCConversionGroup adcgrpcfg2 = {
  TRUE,
  ADC_GRP2_NUM_CHANNELS,
  adccallback,
  adcerrorcallback,
  0,                                                            / CFGR     /
  ADC_TR(0, 4095),                                              / TR1      /
  ADC_CCR_DUAL(1) | ADC_CCR_TSEN | ADC_CCR_VBATEN,              / CCR      /
  {                                                             / SMPR[2]  /
    ADC_SMPR1_SMP_AN7(ADC_SMPR_SMP_19P5)
    | ADC_SMPR1_SMP_AN8(ADC_SMPR_SMP_19P5),
    ADC_SMPR2_SMP_AN16(ADC_SMPR_SMP_61P5)
    | ADC_SMPR2_SMP_AN17(ADC_SMPR_SMP_61P5),
  },
  {                                                             / SQR[4]   
    ADC_SQR1_SQ1_N(ADC_CHANNEL_IN7)  | ADC_SQR1_SQ2_N(ADC_CHANNEL_IN8) |
    ADC_SQR1_SQ3_N(ADC_CHANNEL_IN7)  | ADC_SQR1_SQ4_N(ADC_CHANNEL_IN8),
    ADC_SQR2_SQ5_N(ADC_CHANNEL_IN7)  | ADC_SQR2_SQ6_N(ADC_CHANNEL_IN8) |
    ADC_SQR2_SQ7_N(ADC_CHANNEL_IN16) | ADC_SQR2_SQ8_N(ADC_CHANNEL_IN17),
    0,
    0
  },
  {                                                             / SSMPR[2] /
    ADC_SMPR1_SMP_AN7(ADC_SMPR_SMP_19P5)
    | ADC_SMPR1_SMP_AN8(ADC_SMPR_SMP_19P5),
    ADC_SMPR2_SMP_AN16(ADC_SMPR_SMP_61P5)
    | ADC_SMPR2_SMP_AN17(ADC_SMPR_SMP_61P5),
  },
  {                                                             / SSQR[4]  /
    ADC_SQR1_SQ1_N(ADC_CHANNEL_IN8)  | ADC_SQR1_SQ2_N(ADC_CHANNEL_IN7) |
    ADC_SQR1_SQ3_N(ADC_CHANNEL_IN8)  | ADC_SQR1_SQ4_N(ADC_CHANNEL_IN7),
    ADC_SQR2_SQ5_N(ADC_CHANNEL_IN8)  | ADC_SQR2_SQ6_N(ADC_CHANNEL_IN7) |
    ADC_SQR2_SQ7_N(ADC_CHANNEL_IN17) | ADC_SQR2_SQ8_N(ADC_CHANNEL_IN16),
    0,
    0
  }
}; 


#endregion
*/

/*
 * 
 * #include "ch.h"
#include "hal.h"
#include "adc_in.h"

#define ADC1_NUM_CHANNELS 1
#define ADC1_BUF_DEPTH 1024

adcsample_t adc_buffer[ADC1_NUM_CHANNELS * ADC1_BUF_DEPTH];

static void newAdcBufferCb (ADCDriver *adcp, adcsample_t *buffer, size_t n) {
   palSetPad(GPIOD, GPIOD_LED6);
}

static const ADCConversionGroup adcgrpconf1 = {
   TRUE,
   ADC1_NUM_CHANNELS,
   newAdcBufferCb,
   NULL,
   0,
   ADC_CR2_EXTSEL_SRC(8) | ADC_CR2_EXTEN_0, // Use TIM3_TRGO as source
   0,
   ADC_SMPR2_SMP_AN0(ADC_SAMPLE_15),
   ADC_SQR1_NUM_CH(ADC1_NUM_CHANNELS),
   0,
   ADC_SQR3_SQ1_N(ADC_CHANNEL_IN0)
};

static void sampleRateCb (GPTDriver *gptp) {}

static const GPTConfig sampleRateCfg = {
   8000,      / 8kHz timer clock./
   sampleRateCb  /Timer callback./
};

void startAdcAcquire(void) {
   adcStart (&ADCD1, NULL);
   gptStart (&GPTD3, &sampleRateCfg);
}

void beginAdcAcquire(void) {
   adcStartConversion(&ADCD1, &adcgrpconf1, adc_buffer, ADC1_BUF_DEPTH);
   gptStartContinuous (&GPTD3, 2); // Start ADC/DAC Timer
   GPTD3.tim->SR = 0;
   GPTD3.tim->DIER = 0;
}

void endAdcAcquire(void) {
   adcStopConversion (&ADCD1);
   gptStopTimer (&GPTD3); // Stop ADC/DAC Timer on closing
}

*/

#region

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

#endregion


