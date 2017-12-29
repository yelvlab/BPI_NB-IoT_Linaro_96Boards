#ifndef _USART_1_H
#define _USART_1_H

#include "stm32f10x.h"

#include <stdarg.h>
#include <stdio.h>


#if defined ( __CC_ARM )       /* ARM Compiler */
#include <stdarg.h>
#define TRUE 1
#define FALSE 0

int USART1_Config(uint32_t);
void USART2_Config(void);
int fputc(int ch, FILE *f);
char strcmp1(char *lk,char *ls);

#elif defined ( __GNUC__ )
int USART1_Config(uint32_t);
void USART2_Config(void);
int fputc(int ch, FILE *f);
char strcmp1(char *lk, char *ls);
void USART1_Putc(unsigned char c);
#endif



#endif // !_USART_1_H
