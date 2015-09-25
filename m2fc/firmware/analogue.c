/*
 * ADC Driver (ADC1, ADC2, ADC3)
 * M2FC
 * 2015 Eivind Roson Eide, Cambridge University Spaceflight
 */

#include <string.h>

#include "analogue.h"
#include "datalogging.h"
#include "config.h"
#include "state_estimation.h"
#include "chprintf.h"
#include "m2status.h"
#include "hal.h"

#define ADC_NUM_CHANNELS   2
#define ADC_BUF_DEPTH      1024

#define SG1_CHN     ADC_CHANNEL_IN0          /* PA0 = ADC IN0  */
#define SG2_CHN     ADC_CHANNEL_IN1          /* PA1 = ADC IN1  */
#define SG3_CHN     ADC_CHANNEL_IN2          /* PA2 = ADC IN2  */
#define TC1_CHN     ADC_CHANNEL_IN10         /* PC1 = ADC IN11 */
#define TC2_CHN     ADC_CHANNEL_IN11         /* PC2 = ADC IN12 */
#define TC3_CHN     ADC_CHANNEL_IN12         /* PC3 = ADC IN13 */

#define ADC_MICROSD_CHN_SG   M2T_CH_ADC_STRAIN
#define ADC_MICROSD_CHN_TC   M2T_CH_ADC_THERMO

static void gpt_adc_trigger(GPTDriver*);
static void adc_call_back(ADCDriver*, adcsample_t*, size_t n);
static void adc_error_call_back(ADCDriver*, adcerror_t);
static void save_results(void);
static void adc_null_callback(ADCDriver *, adcsample_t*, size_t n);

/* Allocate arrays with the sample data from ADCs */
static volatile adcsample_t samples_1[ADC_NUM_CHANNELS * ADC_BUF_DEPTH];
static volatile adcsample_t samples_2[ADC_NUM_CHANNELS * ADC_BUF_DEPTH];
static volatile adcsample_t samples_3[ADC_NUM_CHANNELS * ADC_BUF_DEPTH];

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

/*
 * This number keeps track on how many times the full buffer
 *  callback has been called since last save.
 */
static volatile int number_of_buffers_ready = 0;

/*
* ADC conversion group.
* Mode:        Continuous, 1024 samples of 2 channels,
*
* TIM3 updates triggers the ADC to sample.
*
* Channels:    {IN0, IN11} (ADC1)
*              {IN1, IN12} (ADC2)
*              {IN2, IN13} (ADC3)
*/

static const ADCConversionGroup adc_con_group_1 = {
    TRUE,
    ADC_NUM_CHANNELS,
    adc_call_back,
    adc_error_call_back,
    /* cr1 */
    0,
    /* cr2: Clock the ADC to timer 3 TRGO event*/
    ADC_CR2_EXTSEL_SRC(8) | ADC_CR2_EXTEN_0,
    /* smpr1: set all channels to 40 cycles per conversion (28+12) */
    ADC_SMPR1_SMP_AN11(2)| ADC_SMPR1_SMP_AN12(2)| ADC_SMPR1_SMP_AN13(2),
    /* smpr2: see smpr1 */
    ADC_SMPR2_SMP_AN0(2) | ADC_SMPR2_SMP_AN1(2) | ADC_SMPR2_SMP_AN2(2),
    /* sqr1: set 2 channels in the group */
    ADC_SQR1_NUM_CH(2),
    /* sqr2: no higher channels being sampled */
    0,
    /* sqr3: set the two channels to sample */
    ADC_SQR3_SQ1_N(SG1_CHN) | ADC_SQR3_SQ2_N(TC1_CHN)
};

/* See adc_con_group_1 for documentation  */
static const ADCConversionGroup adc_con_group_2 = {
    TRUE,
    ADC_NUM_CHANNELS,
    adc_null_callback,
    adc_error_call_back,
    /* cr1 */
    0,
    /* cr2: Clock the ADC to timer 3 TRGO event*/
    ADC_CR2_EXTSEL_SRC(8) | ADC_CR2_EXTEN_0,
    /* smpr1: set all channels to 40 cycles per conversion (28+12) */
    ADC_SMPR1_SMP_AN11(2)| ADC_SMPR1_SMP_AN12(2)| ADC_SMPR1_SMP_AN13(2),
    /* smpr2: see smpr1 */
    ADC_SMPR2_SMP_AN0(2) | ADC_SMPR2_SMP_AN1(2) | ADC_SMPR2_SMP_AN2(2),
    /* sqr1: set 2 channels in the group */
    ADC_SQR1_NUM_CH(2),
    /* sqr2: no higher channels being sampled */
    0,
    /* sqr3: set the two channels to sample */
    ADC_SQR3_SQ1_N(SG2_CHN) | ADC_SQR3_SQ2_N(TC2_CHN)
};

/* See adc_con_group_1 for documentation  */
static const ADCConversionGroup adc_con_group_3 = {
    TRUE,
    ADC_NUM_CHANNELS,
    adc_null_callback,
    adc_error_call_back,
    /* cr1 */
    0,
    /* cr2: Clock the ADC to timer 3 TRGO event*/
    ADC_CR2_EXTSEL_SRC(8) | ADC_CR2_EXTEN_0,
    /* smpr1: set all channels to 40 cycles per conversion (28+12) */
    ADC_SMPR1_SMP_AN11(2)| ADC_SMPR1_SMP_AN12(2)| ADC_SMPR1_SMP_AN13(2),
    /* smpr2: see smpr1 */
    ADC_SMPR2_SMP_AN0(2) | ADC_SMPR2_SMP_AN1(2) | ADC_SMPR2_SMP_AN2(2),
    /* sqr1: set 2 channels in the group */
    ADC_SQR1_NUM_CH(2),
    /* sqr2: no higher channels being sampled */
    0,
    /* sqr3: set the two channels to sample */
    ADC_SQR3_SQ1_N(SG3_CHN) | ADC_SQR3_SQ2_N(TC3_CHN)
};

/* Null callback won't actually be used. */
static void gpt_adc_trigger(GPTDriver *gpt_ptr) {
    (void)gpt_ptr;
}
static void adc_null_callback(ADCDriver *d, adcsample_t *b, size_t n) {
    (void)d;
    (void)b;
    (void)n;
}

/* Called on ADC DMA buffer filling up. */
static void adc_call_back(ADCDriver *adc_driver_ptr, adcsample_t *buffer, size_t n) {
    (void)adc_driver_ptr;
    (void)buffer;
    (void)n;

    chSysLockFromIsr();
    if(tp != NULL && tp->p_state != THD_STATE_READY) {
        chSchReadyI(tp);
    } else {
        m2status_adc_status(STATUS_ERR_CALLBACK_WHILE_ACTIVE);
    }
    chSysUnlockFromIsr();
}

static void save_results()
{
    int16_t size = (int16_t)(ADC_BUF_DEPTH);
    int16_t j = 0;

	int16_t sum_TC_samples_1, sum_TC_samples_2, sum_TC_samples_3;

    sum_TC_samples_1 = 0;
    sum_TC_samples_2 = 0;
    sum_TC_samples_3 = 0;

    const int16_t log_interval = 20;

    m2status_set_sg(samples_1[0], samples_2[0], samples_3[0]);
    m2status_set_tc(samples_1[1], samples_2[1], samples_3[1]);

    while(j < size)
    {
        /* Read and log all the SG samples */
        log_i16(ADC_MICROSD_CHN_SG, samples_1[j],
                        samples_2[j], samples_3[j], 0);

        /* Accumulate the TG samples and only save every ten,
         * providing some averaging and lowering data rates.
         */
        sum_TC_samples_1 += samples_1[j + 1];
        sum_TC_samples_2 += samples_2[j + 1];
        sum_TC_samples_3 += samples_3[j + 1];

        if (j % log_interval == 0)
        {
            log_i16(ADC_MICROSD_CHN_TC, sum_TC_samples_1,
                            sum_TC_samples_2, sum_TC_samples_3, 0);

            sum_TC_samples_1 = 0;
            sum_TC_samples_2 = 0;
            sum_TC_samples_3 = 0;
        }

        j += 2;
    }
}

/*
 * adc_error_call_back is called if there is an error during conversion
 *
 * TODO: implement some kind of reset function that turns of the driver and
 * restarts it or something similar in case there is an error.
 */
static void adc_error_call_back(ADCDriver *adc_driver_ptr, adcerror_t err) {
    (void)adc_driver_ptr;
    (void)err;
    m2status_adc_status(STATUS_ERR_PERIPHERAL);
}

msg_t analogue_thread(void *args)
{
    (void)args;

    m2status_adc_status(STATUS_WAIT);
    chRegSetThreadName("Analogue");

    adcInit();
    adcStart(&ADCD1, NULL);
    adcStart(&ADCD2, NULL);
    adcStart(&ADCD3, NULL);
    gptStart(&GPTD3, &gpt_adc_config);

    adcStartConversion(&ADCD1, &adc_con_group_1,
                       (adcsample_t*)samples_1, ADC_BUF_DEPTH);
    
    adcStartConversion(&ADCD2, &adc_con_group_2,
                       (adcsample_t*)samples_2, ADC_BUF_DEPTH);

    adcStartConversion(&ADCD3, &adc_con_group_3,
                       (adcsample_t*)samples_3, ADC_BUF_DEPTH);

    /*
    * Start the GPT timer
    * Timer is clocked at 40kHz and reloaded when it reaches 1.
    * We manually set master mode to UPDATE to generate TRGO on updates.
    * We manually disable the update interrupt as we're not using it.
    */
    GPTD3.tim->CR2 |= STM32_TIM_CR2_MMS(2);
    gptStartContinuous(&GPTD3, 2);
    GPTD3.tim->DIER &= ~STM32_TIM_DIER_UIE;

    m2status_adc_status(STATUS_OK);

    /* Sleep until woken up by the ADC buffer callback, then dump buffers
     * to the microSD card and go back to sleep.
     */
    while(true) {
        chSysLock();
        tp = chThdSelf();
        chSchGoSleepS(THD_STATE_SUSPENDED);
        chSysUnlock();
        save_results();
    }
}
