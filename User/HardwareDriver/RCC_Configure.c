#include "RCC_Configure.h"

/**
  * @brief  RCC_Config program.
  * @param  None
  * @retval None
  */
void RCC_Config(void)
{
	ErrorStatus HSEStartUpStatus = ERROR;
	
	/*����RCC*/
	RCC_DeInit();
	/*ʹ���ⲿ���پ���*/
	RCC_HSEConfig(RCC_HSE_ON);
	/*�ȴ����پ����ȶ�*/
	HSEStartUpStatus = RCC_WaitForHSEStartUp();
	
	if(HSEStartUpStatus == SUCCESS)
	{
		/*ʹ��FlashԤ��ȡ������*/
		FLASH_PrefetchBufferCmd( FLASH_PrefetchBuffer_Enable);
		/*��Flash���ڵȴ�״̬*/
		FLASH_SetLatency( FLASH_Latency_2);
		
		/*HCLK = SYSCLK ���ø�������ʱ��=ϵͳʱ��*/
		RCC_HCLKConfig( RCC_SYSCLK_Div1);
		/*PCLK2 = HCLK ���������2ʱ��=��������ʱ��*/
		RCC_PCLK2Config( RCC_HCLK_Div4);
		/*PCLK1 = HCLK ���������1ʱ��=��������ʱ��*/
		RCC_PCLK1Config( RCC_HCLK_Div1);
		/*ADCCLK = PCLK2/6 ����ADC����ʱ��=��������2ʱ�ӵ�����Ƶ*/
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
