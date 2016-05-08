#ifndef __NVIC_CONFIGURE_H__
#define __NVIC_CONFIGURE_H__

#include "stm32f10x.h"

#define CriticalInterruptLevel			0
#define MT_InterruptLevel						CriticalInterruptLevel
#define	ST_InterruptLevel						(CriticalInterruptLevel + 1)
#define TransmissionInterruptLevel	7
#define	SecondaryInterruptLevel			15

void NVIC_Config(void);

#endif
