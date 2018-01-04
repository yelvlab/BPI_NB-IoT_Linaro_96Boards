
#include "bc95_power.h"


#define BC95_POWER_PORT     GPIOA
#define BC95_POWER_GPIO     GPIO_Pin_0

void BC95_POWER_Init(void) {

    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Pin = BC95_POWER_GPIO;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(BC95_POWER_PORT, &GPIO_InitStructure);

    GPIO_SetBits(BC95_POWER_PORT, BC95_POWER_GPIO);

}

void BC95_POWER_ON_OFF(int8_t power) {

    if(power == ON) {
        GPIO_SetBits(BC95_POWER_PORT, BC95_POWER_GPIO);
    }
    else if(power == OFF){
        GPIO_ResetBits(BC95_POWER_PORT, BC95_POWER_GPIO);
    }
}