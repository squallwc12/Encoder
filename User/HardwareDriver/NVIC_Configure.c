#include "NVIC_Configure.h"


/**
  * @brief  NVIC_Config program.
  * @param  None
  * @retval None
  */
void NVIC_Config(void)
{
	/* Set the Vector Table base address at 0x08000000 */
	NVIC_SetVectorTable( NVIC_VectTab_FLASH, 0x0 );
	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4);
}
