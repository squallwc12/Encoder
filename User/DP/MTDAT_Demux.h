#ifndef _MTDAT_DEMUX_H
#define _MTDAT_DEMUX_H


#include "stm32f10x.h"

u16		MT_Logic(u8	GearData, u8 Gear);
u16		MT_LogicTransform12Bit(u8	*GearN);
u16		MT_LogicTransform13Bit(u8	*GearN);
void 	MT_AdjustmentTurn(u8 *pDAT, u8 *pDAT_REC);

#endif
