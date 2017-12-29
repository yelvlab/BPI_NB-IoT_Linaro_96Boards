/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   用3.5.0版本库建的工程模板
  ******************************************************************************
  * @attention
  *
  * 
  *	By tymaker
  ******************************************************************************
  */

#include "main.h"

void LED_Init(void) {

    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_SetBits(GPIOC, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3);
}


int main(void)
{
    //SystemInit();
    Init_SysTick();     												//Init Systick
    LED_Init();
    BC95_POWER_Init();
    USART1_Config(9600);          							//串口初始化
    USART_ClearFlag(USART1, USART_FLAG_TC);
    USART2_Config();
    USART_ClearFlag(USART2, USART_FLAG_TC);

    NVIC_Configuration();           						//设置中断优先级
    printf("open bc95 power ...\r\n");
    BC95_POWER_ON_OFF(ON);
    delay(2000);
    BC95_POWER_ON_OFF(OFF);

    while (1)
    {
        GPIO_SetBits(GPIOC, GPIO_Pin_1);
        GPIO_SetBits(GPIOC, GPIO_Pin_2);
        /*GPIO_SetBits(GPIOC, GPIO_Pin_3);
        delay(500);
        GPIO_ResetBits(GPIOC, GPIO_Pin_3);
        delay(500);*/
    }
}

/*********************************************END OF FILE**********************/

