#include "RCC_Configure.h"

/**
  * @brief  RCC_Config program.
  * @param  None
  * @retval None
  */
void RCC_Config(void)
{
	ErrorStatus HSEStartUpStatus = ERROR;
	
	/*重置RCC*/
	RCC_DeInit();
	/*使能外部高速晶振*/
	RCC_HSEConfig(RCC_HSE_ON);
	/*等待高速晶振稳定*/
	HSEStartUpStatus = RCC_WaitForHSEStartUp();
	
	if(HSEStartUpStatus == SUCCESS)
	{
		/*使能Flash预读取缓冲区*/
		FLASH_PrefetchBufferCmd( FLASH_PrefetchBuffer_Enable);
		/*令Flash处于等待状态*/
		FLASH_SetLatency( FLASH_Latency_2);
		
		/*HCLK = SYSCLK 设置高速总线时钟=系统时钟*/
		RCC_HCLKConfig( RCC_SYSCLK_Div1);
		/*PCLK2 = HCLK 设低速总线2时钟=高速总线时钟*/
		RCC_PCLK2Config( RCC_HCLK_Div4);
		/*PCLK1 = HCLK 设低速总线1时钟=高速总线时钟*/
		RCC_PCLK1Config( RCC_HCLK_Div1);
		/*ADCCLK = PCLK2/6 设置ADC外设时钟=低速总线2时钟的六分频*/
		RCC_ADCCLKConfig( RCC_PCLK2_Div6);
		/*Set PLL clock output to 72MHz using HSE(8MHz) as entry clock*/
		RCC_PLLConfig( RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
		/*Enable PLL*/
		RCC_PLLCmd( ENABLE);
		/*Wait till PLL is ready*/
		while(RCC_GetFlagStatus( RCC_FLAG_PLLRDY) == RESET)
		{}
		/*Select PLL as system clock source*/
		RCC_SYSCLKConfig( RCC_SYSCLKSource_PLLCLK);
		/*Wait till PLL is used as system clock source*/
		while( RCC_GetSYSCLKSource() != 0x08)
		{}
	}
}
