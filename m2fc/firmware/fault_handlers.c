/*
 * Interrupt handlers.
 * M2FC
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
        palClearPad(GPIOC, GPIOC_LED_C);
        palSetPad(GPIOC, GPIOC_LED_A);

        palSetPad(GPIOA, GPIOA_LED_SDCARD);
        palSetPad(GPIOA, GPIOA_LED_PYROS);
        palSetPad(GPIOA, GPIOA_LED_SENSORS);
        palSetPad(GPIOA, GPIOA_LED_STATUS);
        for(i=0; i<1000000; i++)
            asm("nop");

        palClearPad(GPIOC, GPIOC_LED_A);
        palClearPad(GPIOC, GPIOC_LED_C);

        palClearPad(GPIOA, GPIOA_LED_SDCARD);
        palClearPad(GPIOA, GPIOA_LED_PYROS);
        palClearPad(GPIOA, GPIOA_LED_SENSORS);
        palClearPad(GPIOA, GPIOA_LED_STATUS);
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
