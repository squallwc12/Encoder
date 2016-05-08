/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/main.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */  

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "NVIC_Configure.h"
#include "GPIO_Configure.h"
#include "RCC_Configure.h"
#include "CAN_Configure.h"
#include "TIM_Configure.h"
#include "USART_Configure.h"
#include "SPI_Configure.h"
#include "WDG_Configure.h"

#include "MT_Configure.h"
#include "CANDAT_Handle.h"
#include "STDAT_Demux.h"



/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
	/* MCU Configure 
		 */
	RCC_Config();
	NVIC_Config();
	GPIO_Config();
	CAN_Polling();
	SPI1_Config();
	TIM2_Config();
	TIM3_Config();
	TIM4_Config();
	IWDG_Config();
	//USART2_Init();

	/* Interput
		 */
	GPIO_NVIC_Config();
	CAN_RX_NVIC_Config();
	
	/* GPIO
		 */
	GPIO_ResetBits(GPIOB, GPIO_Pin_4);
	GPIO_ResetBits(GPIOB, GPIO_Pin_5);
	
	/* Add your application code here
		 */
	MT_MUXDAT.MT_EOC = 1;
	/* Infinite loop 
		 */
	while (1)
	{
		//Feed IWDG
		if( (STDAT_Buf.BSY_TIM < 100) && 
				(MT_MUXDAT.MT_ERROR_FLAG == 0) )
		{
			Feed_IWDG();
		}
		
		//ST Scan
		if(STDAT_Buf.SPI_RxFlag != 0)
		{
			MT_Scan();
		}
		else
		{
			SPI1_DMA_TxData();
		}
		Delay_MT(10);
		
		//CAN Scan
		if( MT_MUXDAT.MT_EOC != 0 )
		{
			MT_MUXDAT.MT_EOC = 0;
			STDAT_Buf.SPI_RxFlag = 0;
			
			CanData_12BitBuf(MT_MUXDAT.MTValue_12BIT, STDAT_Buf.AngleValue);
			CanData_13BitBuf(MT_MUXDAT.MTValue_12BIT, STDAT_Buf.AngleValue);
			
			LED1_FLASH();
		}
		
		//CAN	CallBack
		CanCommandBack();
	}
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif




/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
