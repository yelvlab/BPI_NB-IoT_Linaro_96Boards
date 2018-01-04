#ifndef _DELAY_H
#define _DELAY_H

#include "stm32f10x.h"


void Init_SysTick(void);
void delay_us(__IO uint32_t nTime);
void delay(__IO uint32_t nTime);

#endif // !_DELAY_H
