/**************************************************************************
* usart.c 
* 2015-09-25
***************************************************************************/
#include "USART_Configure.h"

//DMA接收时定义缓冲区
uint8_t data_received[255];

/*********************************************************************************************************
** Function name:       USART3_Init
** Descriptions:        USART Configure
** input parameters:    null
** output parameters:   null
** Returned value:      null
*********************************************************************************************************/
void USART2_Init(void)		
{	
		USART_InitTypeDef USART_InitStruct;
		NVIC_InitTypeDef  NVIC_InitStructure;
		DMA_InitTypeDef 	DMA_InitStructure;
	
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); 
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	
		
		//USART3 Configure
		USART_InitStruct.USART_BaudRate = 9600;
		USART_InitStruct.USART_WordLength = USART_WordLength_8b;
		USART_InitStruct.USART_StopBits = USART_StopBits_1;
		USART_InitStruct.USART_Parity = USART_Parity_No;
		USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
		USART_Init(USART2, &USART_InitStruct);
		USART_Cmd(USART2, ENABLE);
	
		/*Enable USART3 Rx DMA*/
		NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
		//接收中断使能,此步骤很重要!!!
		USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); 

		/*Enable USART3 Tx DMA */
		DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART2->DR; 
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST; 
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; 
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; 
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; 
		DMA_InitStructure.DMA_Mode = DMA_Mode_Normal; 
		DMA_InitStructure.DMA_Priority = DMA_Priority_High; 
		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; 
		DMA_Init(DMA1_Channel7, &DMA_InitStructure);

}

/*********************************************************************************************************
** Function name:       USART3_DMA_SendBytes
** Descriptions:        bytes:发送数据			counts:发送数据长度
** input parameters:    
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
//通过DMA向串口发送字节
void USART2_DMA_SendBytes(uint8_t *bytes, uint16_t counts)
{
		DMA1_Channel7->CNDTR = counts;
		DMA1_Channel7->CMAR = (uint32_t)bytes; 
		DMA_Cmd(DMA1_Channel7, ENABLE);
		USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);
		while(DMA_GetFlagStatus(DMA1_FLAG_TC7) == RESET);
		while(USART_GetFlagStatus(USART2, USART_FLAG_TC)==RESET);
		DMA_Cmd(DMA1_Channel7, DISABLE);
		DMA_ClearFlag(DMA1_FLAG_TC7);
		USART_DMACmd(USART2, USART_DMAReq_Tx, DISABLE); 
}

/*********************************************************************************************************
** Function name:       USART3_IRQHandler
** Descriptions:        
** input parameters:    略
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
/*
uint8_t usart2_rx_data_counts = 0;

void USART2_IRQHandler(void)
{
		if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) //如果是接收中断
		{
				USART_ClearITPendingBit(USART2, USART_IT_RXNE); //清除中断标志
				data_received[usart2_rx_data_counts++] = (uint8_t)USART_ReceiveData(USART2); 
				if(usart2_rx_data_counts == 255)
				{
						usart2_rx_data_counts = 0;
				}
				while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);  		
		}
}
*/
/*********************************************************************************************************
** Function name:       fputc
** Descriptions:        重定义fput(printf)
** input parameters:    略
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
int fputc(int ch, FILE *f)
{
	/* Place your implementation of fputc here */
	/* e.g. write a character to the USART */
	USART_SendData(USART2, (uint8_t) ch);

	/* Loop until the end of transmission */
	while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET)
	{}

	return ch;
}
















