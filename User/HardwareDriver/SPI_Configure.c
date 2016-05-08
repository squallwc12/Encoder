#include 	"SPI_Configure.h"
#include	"STDAT_Demux.h"
#include 	"MT_Configure.h"

static u8 MLX90316BUF[STDATSIZE] = {0x55,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

void SPI1_Config(void)
{
	SPI_InitTypeDef		SPI_InitStruct;
	NVIC_InitTypeDef  NVIC_InitStructure;
	DMA_InitTypeDef 	DMA_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	RCC_AHBPeriphClockCmd (RCC_AHBPeriph_DMA1, ENABLE);
	
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStruct.SPI_CRCPolynomial = 7;
	SPI_Init(SPI1, &SPI_InitStruct);
	
	SPI_Cmd(SPI1, ENABLE);
	
	SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = SPI1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	SPI_I2S_ClearFlag(SPI1, SPI_I2S_IT_RXNE);
	SPI_I2S_ITConfig(SPI1, SPI_I2S_IT_RXNE, ENABLE);
	
	/*Enable SPI1 Tx DMA */
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&SPI1->DR;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)MLX90316BUF;
	DMA_InitStructure.DMA_BufferSize = STDATSIZE;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST; 
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; 
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; 
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; 
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal; 
	DMA_InitStructure.DMA_Priority = DMA_Priority_High; 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; 
	DMA_Init(DMA1_Channel3, &DMA_InitStructure);
	
	//NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel3_IRQn;
	//NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	//NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	//NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	//NVIC_Init(&NVIC_InitStructure);
	
	//DMA_ITConfig(DMA1_Channel3, DMA_IT_TC, ENABLE);
	
}


void SPIDelay_ms(u16 timx)
{
		u16		x;
		for(;timx != 0; timx--)
		{
				x = 100;
				while(x--);
		}
}

void SPI1_DMA_TxData(void)
{
	//if(STDAT_Buf.SPI_RxFlag == 1)
	//{
	//}
	//else
	//{
		if((STDAT_Buf.SPI1BsyFlag == 0) && (MT_MUXDAT.MT_TRSE_FLAG != 0))
		{
			MT_MUXDAT.MT_TRSE_FLAG = 0;
			
			STDAT_Buf.BSY_TIM = 0;
			STDAT_Buf.SPI1BsyFlag = 1;
			STDAT_Buf.DataTransferFlag = 1;
			
			GPIO_ResetBits(GPIOA, GPIO_Pin_4);
			SPIDelay_ms(1);
			
			DMA_Cmd(DMA1_Channel3, DISABLE);
			DMA1_Channel3->CNDTR = STDATSIZE;
			DMA_Cmd(DMA1_Channel3, ENABLE);
		}
		else
		{
			STDAT_Buf.BSY_TIM ++;
		}
	//}
}

void SPI1_IRQHandler(void)
{
	if(SPI_I2S_GetITStatus(SPI1, SPI_I2S_IT_RXNE) != RESET)
	{
		SPI_I2S_ClearITPendingBit(SPI1, SPI_I2S_IT_RXNE);
		
		ST_ReceiveData( (u8)SPI_I2S_ReceiveData(SPI1));
		
		//STDAT_Buf.SPI_RxFlag = 1;
	}
}

//void DMA1_Channel3_IRQHandler(void)
//{
//	if(DMA_GetITStatus(DMA1_IT_TC3) != RESET)
//	{
//		DMA_ClearITPendingBit(DMA1_IT_TC3); 
//	}
//}
