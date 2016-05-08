#include "GPIO_Configure.h"
#include "STDAT_Demux.h"

/**
  * @brief  GPIO_Config program.
  * @param  None
  * @retval None
  */
void GPIO_Config(void)
{
		GPIO_InitTypeDef  GPIO_InitStructure;

		RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA , ENABLE);
		RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB , ENABLE);
	
		/* LED */
		GPIO_StructInit(&GPIO_InitStructure);
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
	
		GPIO_StructInit(&GPIO_InitStructure);
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
	
		/* MTMUX */
		GPIO_StructInit(&GPIO_InitStructure);
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
	
		/* MTDAT */
		GPIO_StructInit(&GPIO_InitStructure);
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
		
		/* Wake UP */
		GPIO_StructInit(&GPIO_InitStructure);
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
	
		/* Configure CAN RX and TX pins */
		GPIO_StructInit(&GPIO_InitStructure);
		GPIO_InitStructure.GPIO_Pin = 	GPIO_Pin_12;
		GPIO_InitStructure.GPIO_Mode = 	GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		
		GPIO_StructInit(&GPIO_InitStructure);
		GPIO_InitStructure.GPIO_Pin = 	GPIO_Pin_11;
		GPIO_InitStructure.GPIO_Mode = 	GPIO_Mode_IPU;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		
		/* Configure CAN Rs pins */
		GPIO_StructInit(&GPIO_InitStructure);
		GPIO_InitStructure.GPIO_Pin = 	GPIO_Pin_8;
		GPIO_InitStructure.GPIO_Mode = 	GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		
		/* Configure USART RX and TX pins */
		
		/* Configure SPI CLK Pins */
		GPIO_StructInit(&GPIO_InitStructure);
		GPIO_InitStructure.GPIO_Pin = 	GPIO_Pin_5;
		GPIO_InitStructure.GPIO_Mode = 	GPIO_Mode_AF_OD;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		/* Configure SPI MOSI Pins */
		GPIO_StructInit(&GPIO_InitStructure);
		GPIO_InitStructure.GPIO_Pin = 	GPIO_Pin_7;
		GPIO_InitStructure.GPIO_Mode = 	GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		/* SPI1 MISO Pins*/
		GPIO_StructInit(&GPIO_InitStructure);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		/* SPI SS0 & SS1 Pins*/
		GPIO_StructInit(&GPIO_InitStructure);
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void GPIO_NVIC_Config(void)
{
		NVIC_InitTypeDef  NVIC_InitStructure;
		EXTI_InitTypeDef  EXTI_InitStructure;
	
		RCC_APB2PeriphClockCmd( RCC_APB2Periph_AFIO , ENABLE);
	
		NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
		
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource6);
	
		EXTI_InitStructure.EXTI_Line = EXTI_Line6;
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
		EXTI_Init(&EXTI_InitStructure);
}

void EXTI9_5_IRQHandler(void)
{
		//GPIOE Pin14
		if(EXTI_GetITStatus(EXTI_Line6) != RESET)
		{
				EXTI_ClearITPendingBit(EXTI_Line6);
				/* Phase correction */
				TurnCheck_Interput();
		}
}
