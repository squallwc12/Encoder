#ifndef _CAN_CONFIGURE_H
#define _CAN_CONFIGURE_H


#include "stm32f10x.h"


#define CANx                       CAN1
#define CAN_CLK                    RCC_APB1Periph_CAN1


void CAN_Polling(void);
void CAN_RX_NVIC_Config(void);
void EquipmentDataTx(u32	DataAddr, u8 DataSize, u8	*Data);
void CAN1_TxExtData( u32 Addr, u8 * pData, u8 Size );


#endif
