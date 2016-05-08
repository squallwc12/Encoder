#ifndef __GPIO_CONFIGURE_H__
#define __GPIO_CONFIGURE_H__ (1)

#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"


#define	LED1_ON()		do{GPIO_ResetBits(GPIOA, GPIO_Pin_1);GPIO_ResetBits(GPIOB, GPIO_Pin_4);}while(0)
#define LED1_OFF()	do{GPIO_SetBits(GPIOA, GPIO_Pin_1);GPIO_SetBits(GPIOB, GPIO_Pin_4);}while(0)
#define LED1_FLASH()		do{	\
													(GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_1) != Bit_RESET) ? \
														GPIO_ResetBits(GPIOA, GPIO_Pin_1) : GPIO_SetBits(GPIOA, GPIO_Pin_1);	\
													(GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_4) != Bit_RESET) ? \
														GPIO_ResetBits(GPIOB, GPIO_Pin_4) : GPIO_SetBits(GPIOB, GPIO_Pin_4);	\
													}while(0)

#define	LED2_ON()		do{GPIO_ResetBits(GPIOA, GPIO_Pin_2);GPIO_ResetBits(GPIOB, GPIO_Pin_5);}while(0)
#define LED2_OFF()	do{GPIO_SetBits(GPIOA, GPIO_Pin_2);GPIO_SetBits(GPIOB, GPIO_Pin_4);}while(0)


#define	RS485_CH1Rx			GPIO_ResetBits(GPIOB, GPIO_Pin_5)
#define	RS485_CH1Tx			GPIO_SetBits(GPIOB, GPIO_Pin_5)

#define	RS485_CH2Rx			GPIO_ResetBits(GPIOB, GPIO_Pin_4)
#define	RS485_CH2Tx			GPIO_SetBits(GPIOB, GPIO_Pin_4)

void GPIO_Config(void);
void GPIO_NVIC_Config(void);

#endif
