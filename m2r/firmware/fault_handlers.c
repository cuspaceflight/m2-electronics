/*
 * Interrupt handlers.
 * M2R
 * 2014 Adam Greig, Cambridge University Spaceflight
 */

#include "ch.h"
#include "hal.h"

void **HARDFAULT_PSP;
register void *stack_pointer asm("sp");

void panic(void);
void HardFaultVector(void);
void BusFaultVector(void);
void UsageFaultVector(void);
void MemManageVector(void);
void NMIVector(void);

void panic()
{
    int i;

    /* Funny magic to give GDB a good stack pointer after a fault. */
    asm("mrs %0, psp" : "=r"(HARDFAULT_PSP) : :);
    stack_pointer = HARDFAULT_PSP;

    while(1)
    {
        palSetPad(GPIOC, GPIOC_LED_EXT);

        palSetPad(GPIOB, GPIOB_LED_PYRO);
        palSetPad(GPIOB, GPIOB_LED_RADIO);
        palSetPad(GPIOB, GPIOB_LED_GPS);
        palSetPad(GPIOB, GPIOB_LED_STATUS);
        for(i=0; i<1000000; i++)
            asm("nop");

        palClearPad(GPIOC, GPIOC_LED_EXT);

        palClearPad(GPIOB, GPIOB_LED_PYRO);
        palClearPad(GPIOB, GPIOB_LED_RADIO);
        palClearPad(GPIOB, GPIOB_LED_GPS);
        palClearPad(GPIOB, GPIOB_LED_STATUS);
        for(i=0; i<1000000; i++)
            asm("nop");
    }
}

void HardFaultVector()
{
    panic();
}

void BusFaultVector()
{
    panic();
}

void UsageFaultVector()
{
    panic();
}

void MemManageVector()
{
    panic();
}

void NMIVector()
{
    panic();
}
