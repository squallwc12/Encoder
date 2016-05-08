#ifndef _USART_H
#define _USART_H
/**
  *************************************************************************************
  * ����1��ʼ����ʵ��printf()����
  * ������Ϊ115200
  * ע�⹴ѡMircoLIB
  * ��Ҫʹ�ܽ����ж�, ���붨��USART3_RxIRQ
  * ʹ��DMAʱ�����ȶ���USART3_DMA_TX����USART3_DMA_RX
  * �ط�ʱ��Ҫ�������÷��͸������洢����ַҲ�ɸģ��������ǳ�����
  * ����USART1_DMA_SendBytes()��ʵ��DMA�����ַ�
  * DMA����ʱҪ����һ�黺�������绺������С���ģ���ʼ�������е�DMA_BufferSizeҲ�����
  **************************************************************************************
  */
  
#include "stm32f10x.h"
#include <stdio.h>


//��������
void USART2_Init(void);	//����1�ĳ�ʼ������, TX1(PA9), RX1(PA10)
void USART2_DMA_SendBytes(uint8_t *bytes, uint16_t counts);//ͨ��DMA�򴮿ڷ����ֽ�

int fputc(int ch, FILE *f);	//�ض���C�⺯��printf��USART3	 

#endif
