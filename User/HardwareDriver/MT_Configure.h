#ifndef _MT_CONFIGURE_H
#define _MT_CONFIGURE_H


#include "stm32f10x.h"

#define	MT_CHNUM			6
#define MT_G					GPIOB
#define	MT_CacheSize	3

#define MTMUX0Pin		GPIO_Pin_13
#define MTMUX1Pin		GPIO_Pin_14
#define MTMUX2Pin		GPIO_Pin_15

#define MTDAT0Pin		GPIO_Pin_8
#define MTDAT1Pin		GPIO_Pin_9
#define MTDAT2Pin		GPIO_Pin_10

typedef	struct
{
	u8	MT_BUSY_FLAG;
	u8	MT_TRSE_FLAG;
	u8	MT_EOC;
	u8	MT_ERROR_FLAG;
	u16	TP1s;
	u16	MT_MUX_TP1s;
	u16	MTValue_12BIT;
	u16	MTValue_13BIT;
	u8	MTValue_TempNum;
	u16	MTValue_12BitTemp[MT_CacheSize];
	u16	MTValue_13BitTemp[MT_CacheSize];
	u8	MTDAT[MT_CHNUM];
	u8	MTDAT_TEM[MT_CHNUM];
	u8	MTDAT_REC[MT_CHNUM];
	u8	MTTurnNum[MT_CHNUM];
	u16	MTTurnCount;
	float	MTTemp;
}MT_MUXDAT_TyprDef;

extern MT_MUXDAT_TyprDef		MT_MUXDAT;

void 	Delay_MT(u16 timx);
void	MTMUX_Ctrl(u8	MTMUXStatus);
u8		MTDAT_Ctrl(void);
void 	MT_TimerScan(void);
void	MT_Scan(void);
void	MT_Turn_Count(void);

#endif
