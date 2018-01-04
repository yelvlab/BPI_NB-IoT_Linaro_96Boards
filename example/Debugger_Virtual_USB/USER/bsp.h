#ifndef _BSP_H

#define _BSP_H

#include <stdint.h>
#include <stddef.h>
#include "stm32f10x.h"


void IWDG_Init(void);
void WDG_Clear(void);
void SysTickInit(void);
void BSP_Init(void);




















#endif

