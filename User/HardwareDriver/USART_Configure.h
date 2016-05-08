#ifndef _USART_H
#define _USART_H
/**
  *************************************************************************************
  * 串口1初始化并实现printf()函数
  * 波特率为115200
  * 注意勾选MircoLIB
  * 如要使能接收中断, 必须定义USART3_RxIRQ
  * 使用DMA时必须先定义USART3_DMA_TX或者USART3_DMA_RX
  * 重发时需要重新设置发送个数，存储器地址也可改！！！！非常方便
  * 调用USART1_DMA_SendBytes()可实现DMA发送字符
  * DMA接收时要开辟一块缓冲区，如缓冲区大小更改，初始化函数中的DMA_BufferSize也需更改
  **************************************************************************************
  */
  
#include "stm32f10x.h"
#include <stdio.h>


//函数声明
void USART2_Init(void);	//串口1的初始化函数, TX1(PA9), RX1(PA10)
void USART2_DMA_SendBytes(uint8_t *bytes, uint16_t counts);//通过DMA向串口发送字节

int fputc(int ch, FILE *f);	//重定义C库函数printf到USART3	 

#endif
