#ifndef _WDG_CONFIGURE_H
#define _WDG_CONFIGURE_H

#include "stm32f10x.h"

#define	RELOAD_TIM_MS(A) (u16)((float)A/6.4f)

void IWDG_Config(void);
void Feed_IWDG(void);

#endif
