
#include "delay.h"
__IO uint32_t myTick;


void Init_SysTick(void)
{

#if STM32F40_41xxx
    /* Init Systick */
    if (SysTick_Config(SystemCoreClock / 1000))
        while (1) {
            // Error;
        }
#elif 	STM32F10X_HD 
    if (SysTick_Config(SystemCoreClock / 1000))
        while (1) {
            // Error;
        }
#else
    if (SysTick_Config(SystemCoreClock / 1000))
        while (1) {
            // Error;
        }
#endif

}


void delay_us(__IO uint32_t nTime) {

}



void delay(__IO uint32_t nTime) {
    uint32_t start = myTick;
    while (myTick - start < nTime)
        ;
}

