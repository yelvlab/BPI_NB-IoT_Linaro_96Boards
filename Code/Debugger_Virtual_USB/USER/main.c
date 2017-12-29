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
  ******************************************************************************
  */
  

#include "main.h"
#include "string.h"

extern u8 g_DatRev[256];
extern u8 g_DatRev_num;
extern u8 USART2_FLAG;

void LED_Init(void) {

    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_SetBits(GPIOC, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3);
			GPIO_ResetBits(GPIOC,GPIO_Pin_2);

}


void Init_Board() {
    LED_Init();
    BC95_POWER_Init();
    USART1_Config(9600);          							//串口初始化
    USART_ClearFlag(USART1, USART_FLAG_TC);
    USART2_Config();
    USART_ClearFlag(USART2, USART_FLAG_TC);

    //NVIC_Configuration();           						//设置中断优先级
    printf("open bc95 power ...\r\n");
    BC95_POWER_ON_OFF(ON);
    delay(2000);
    BC95_POWER_ON_OFF(OFF);

}


int main(void)
{
//	SystemInit();
	
    uint32_t count = 0;
    uint32_t len = 0;
    static uint8_t buf[2048] = {0};
    Init_SysTick();     												//Init Systick

    BSP_Init();
		
		USB_Config();
		
		Init_Board();
		LED_Init();
		
		
		
		/*****************************************************/
		
		
		while(1) {
			
			  GPIO_SetBits(GPIOC, GPIO_Pin_1);
        GPIO_SetBits(GPIOC, GPIO_Pin_2);

				if (++count > 0x8FFFF) count = 0;
        len = USB_RxRead(buf, sizeof(buf));
        if (len > 0)
        {
            USB_TxWrite(buf, len);
						USART_print(USART2, buf);
        }
				
				if(USART2_FLAG ==1) {
				    uint8_t buff[256];
					  USART2_FLAG = 0;
					  sprintf((char*)buff, "%d", g_DatRev_num);
					  //Send buff to usb
					  USB_TxWrite((uint8_t*)g_DatRev, g_DatRev_num);
					  memset(g_DatRev, 0, 256);
					  g_DatRev_num = 0;
				}
		}
}

/*********************************************END OF FILE**********************/

