#include "WDG_Configure.h"

void IWDG_Config(void)
{
	//0x5555,Enable IWDG
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	//40k/256=156kHz(6.4ms)
	IWDG_SetPrescaler(IWDG_Prescaler_256);
	//5s/6.4ms = 781, Max = 0xFFF
	IWDG_SetReload( RELOAD_TIM_MS(1000) );
	//Feed the Watch Dog
	IWDG_ReloadCounter();
	//Enable Watch Dog
	IWDG_Enable();
}

void Feed_IWDG(void)
{
	//Feed the Watch Dog
	IWDG_ReloadCounter();
}

