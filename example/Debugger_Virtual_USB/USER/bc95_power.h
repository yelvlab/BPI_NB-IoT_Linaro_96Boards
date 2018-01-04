#ifndef _BC95_POWER_H
#define _BC95_POWER_H


#include "stm32f10x.h"

#define ON      1
#define OFF     -1


void BC95_POWER_Init(void);
void BC95_POWER_ON_OFF(int8_t power);


#endif // !_BC95_POWER_H
