
#include "usart.h"
#include <stdarg.h>

u8 g_DatRev[256];
u8 g_DatRev_num;
u8 USART2_FLAG;

/*
* 函数名：USART1_Config
* USART1_Config 配置 工作模式配置 115200-8-N-1
* 输入：无
* 输出：无
* 调用：外部调用
*/
int USART1_Config(uint32_t Baud_Rate)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    if (Baud_Rate == 0)
        return 0;
    else
    {
        /* config USART1 clock */
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);

        /* USART1 GPIO config */
        /* Configure USART1 Tx (PA.09) as alternate function push-pull */
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOA, &GPIO_InitStructure);
        /* Configure USART1 Rx (PA.10) as input floating */
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
        GPIO_Init(GPIOA, &GPIO_InitStructure);

        /* USART1 mode config */
        USART_InitStructure.USART_BaudRate = Baud_Rate;
        USART_InitStructure.USART_WordLength = USART_WordLength_8b;
        USART_InitStructure.USART_StopBits = USART_StopBits_1;
        USART_InitStructure.USART_Parity = USART_Parity_No;
        USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
        USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
        USART_Init(USART1, &USART_InitStructure);
        /* 接受缓冲器溢出中断 接收中断 */
        USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
        /* 使能发送中断 */
        //USART_ITConfig(USART1, USART_IT_TXE, ENABLE);	

        USART_Cmd(USART1, ENABLE);
        USART_ClearFlag(USART1, USART_FLAG_TC);   //发送完成标志位
    }
    return 1;
}


/*
* 函数名：USART2_Config
* USART2_Config 配置 工作模式配置 115200-8-N-1
* 输入：无
* 输出：无
* 调用：外部调用
*/
void USART2_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    /* config USART1 clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    /* USART2 GPIO config */
    /* Configure USART1 Tx (PA.02) as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    /* Configure USART1 Rx (PA.03) as input floating */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* USART2 mode config */
    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART2, &USART_InitStructure);
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
    USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);
		//USART_ITConfig(USART1, USART_IT_TXE, ENABLE);	
    USART_Cmd(USART2, ENABLE);
    USART_ClearFlag(USART2, USART_FLAG_TC);

}


#if  defined( __GNUC__ )
int _fputc(int ch, FILE *f)
{
    /*  */
    USART_SendData(USART1, (unsigned char)ch);
    while (!(USART1->SR & USART_FLAG_TXE))
        ;
    return (ch);
}

int _read(int file, char *ptr, int len)
{
    return 0;
}

int _write(int file, char *ptr, int len)
{
    usart1_send_nbyte(ptr, len);
    return len;
}


static char *itoa(int value, char *string, int radix)
{
    int     i, d;
    int     flag = 0;
    char    *ptr = string;

    /* This implementation only works for decimal numbers. */
    if (radix != 10)
    {
        *ptr = 0;
        return string;
    }

    if (!value)
    {
        *ptr++ = 0x30;
        *ptr = 0;
        return string;
    }

    /* if this is a negative value insert the minus sign. */
    if (value < 0)
    {
        *ptr++ = '-';

        /* Make the value positive. */
        value *= -1;
    }

    for (i = 10000; i > 0; i /= 10)
    {
        d = value / i;

        if (d || flag)
        {
            *ptr++ = (char)(d + 0x30);
            value -= (d * i);
            flag = 1;
        }
    }

    /* Null terminate the string. */
    *ptr = 0;

    return string;

} /* NCL_Itoa */


char strcmp1(char *lk, char *ls)
{
    char *a = lk, *b = ls;
    u8 bSame = TRUE;
    while (*b != '\0')
    {
        if (*a++ != *b++)
        {
            bSame = FALSE; break;
        }
    }
    return bSame;
}


void USART1_Putc(unsigned char c)
{
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
        ;
    USART_SendData(USART1, c);
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
        ;
}
//串口1 发送字符串 
void usart1_send_nbyte(unsigned char *str, int len)
{
    int i;
    for (i = 0; i < len; i++)
    {
        USART1_Putc(*str++);
    }
}

#elif defined( __CC_ARM )

/*
 * 函数名：fputc
 * 描述  ：重定向c库函数printf到USART1
 * 输入  ：无
 * 输出  ：无
 * 调用  ：由printf调用
 */
int fputc(int ch, FILE *f)
{
    /* 将printf内容发往串口 */
	  USART_SendData(USART1, (unsigned char) ch);
	  while(!(USART1->SR & USART_FLAG_TXE));
	
	  return (ch);
}

/*
 * 函数名：itoa
 * 描述  ：将整形数据转换成字符串
 * 输入  ：-radix =10 表示10进制，其他结果为0
 *         -value 要转换的整形数
 *         -buf 转换后的字符串
 *         -radix = 10
 * 输出  ：无
 * 返回  ：无
 * 调用  ：被USART1_printf()调用
 */
static char *itoa(int value, char *string, int radix)
{
    int     i, d;
    int     flag = 0;
    char    *ptr = string;

    /* This implementation only works for decimal numbers. */
    if (radix != 10)
    {
        *ptr = 0;
        return string;
    }

    if (!value)
    {
        *ptr++ = 0x30;
        *ptr = 0;
        return string;
    }

    /* if this is a negative value insert the minus sign. */
    if (value < 0)
    {
        *ptr++ = '-';

        /* Make the value positive. */
        value *= -1;
    }

    for (i = 10000; i > 0; i /= 10)
    {
        d = value / i;

        if (d || flag)
        {
            *ptr++ = (char)(d + 0x30);
            value -= (d * i);
            flag = 1;
        }
    }

    /* Null terminate the string. */
    *ptr = 0;

    return string;

} /* NCL_Itoa */
/*
 * 函数名：USART1_printf
 * 描述  ：格式化输出，类似于C库中的printf，但这里没有用到C库
 * 输入  ：-USARTx 串口通道，这里只用到了串口1，即USART1
 *		     -Data   要发送到串口的内容的指针
 *			   -...    其他参数
 * 输出  ：无
 * 返回  ：无 
 * 调用  ：外部调用
 *         典型应用USART1_printf( USART1, "\r\n this is a demo \r\n" );
 *            		 USART1_printf( USART1, "\r\n %d \r\n", i );
 *            		 USART1_printf( USART1, "\r\n %s \r\n", j );
 */
void USART1_printf(USART_TypeDef* USARTx, uint8_t *Data,...)
{
	const char *s;
  int d;   
  char buf[16];

  va_list ap;
  va_start(ap, Data);

	while ( *Data != 0)     // 判断是否到达字符串结束符
	{
		if ( *Data == 0x5c )  //'\'
		{
			switch ( *++Data )
			{
				case 'r':							          //回车符
					USART_SendData(USARTx, 0x0d);
					Data ++;
					break;

				case 'n':							          //换行符
					USART_SendData(USARTx, 0x0a);	
					Data ++;
					break;
				
				default:
					Data ++;
				    break;
			}			 
		}
		else if ( *Data == '%')
		{									  //
			switch ( *++Data )
			{				
				case 's':										  //字符串
					s = va_arg(ap, const char *);
          for ( ; *s; s++) 
					{
						USART_SendData(USARTx,*s);
						while( USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET );
          }
					Data++;
          break;

        case 'd':										//十进制
          d = va_arg(ap, int);
          itoa(d, buf, 10);
          for (s = buf; *s; s++) 
					{
						USART_SendData(USARTx,*s);
						while( USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET );
          }
					Data++;
          break;
				 default:
						Data++;
				    break;
			}		 
		} /* end of else if */
		else USART_SendData(USARTx, *Data++);
		while( USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET );
	}
}

char strcmp1(char *lk,char *ls)
{
		char *a = lk, *b = ls;
		u8 bSame = TRUE;
		while (*b!='\0')
		{
				if (*a++ != *b++) 
				{
						bSame = FALSE; break;
				}
		}
    return bSame;
}


void USART_print(USART_TypeDef* USARTx, uint8_t *Data)
{
	const char *s;
  int d;   
  char buf[200];

	while ( *Data != 0)     // 判断是否到达字符串结束符
	{
		USART_SendData(USARTx, *Data++);
		while( USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET );
	}
}
#endif //  defined( __GNUC__ )

