
/* ----------------------- Platform includes --------------------------------*/
#include "CAN_Configure.h"
#include "GPIO_Configure.h"
#include "CANDAT_Handle.h"
#include "CANDAT_Handle.h"
#include "MT_Configure.h"

/**
  * @brief  Configures the CAN, transmit and receive by polling
  * @param  None
  * @retval PASSED if the reception is well done, FAILED in other case
  */
void CAN_Polling(void)
{
		CAN_InitTypeDef        CAN_InitStructure;
		CAN_FilterInitTypeDef  CAN_FilterInitStructure;


		/* Enable CAN clock */
		RCC_APB1PeriphClockCmd(CAN_CLK, ENABLE);
		/* CAN register init */
		CAN_DeInit(CANx);
		/* CAN cell init */
		CAN_InitStructure.CAN_TTCM = DISABLE;
		CAN_InitStructure.CAN_ABOM = DISABLE;
		CAN_InitStructure.CAN_AWUM = DISABLE;
		CAN_InitStructure.CAN_NART = DISABLE;
		CAN_InitStructure.CAN_RFLM = DISABLE;
		CAN_InitStructure.CAN_TXFP = DISABLE;
		CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
		CAN_InitStructure.CAN_SJW =  CAN_SJW_1tq;
		/* CAN Baudrate = 500kbps (CAN clocked at 72 MHz) */
		CAN_InitStructure.CAN_BS1 = CAN_BS1_8tq;
		CAN_InitStructure.CAN_BS2 = CAN_BS2_7tq;
		CAN_InitStructure.CAN_Prescaler = 9;
		CAN_Init(CANx, &CAN_InitStructure);

		/* CAN filter init */
		CAN_FilterInitStructure.CAN_FilterNumber = 0;
		
		CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
		CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
		/* 	标志位 
				A: CAN_FxR1[31:24]| CAN_FxR1[23:16]      | CAN_FxR1[15:8]| CAN_FxR1[7:0]              
				B: CAN_FxR2[31:24]| CAN_FxR2[23:16]      | CAN_FxR2[15:8]| CAN_FxR2[7:0]              
				C: STID[10:3]     | STID[2:0] EXID[17:13]| EXID[12:5]    | EXID[4:0] IDE[1] RTR[1] 0[1]
				
				A: CAN_FilterId
				B: CAN_FilterMaskId
				C: CAN ID	[STID:StdID 基本ID] [EXID: 扩展ID] [IDE:扩展帧标志] [RTR:远程帧标志] [0:默认为0]
		*/
		CAN_FilterInitStructure.CAN_FilterIdHigh 			= 0x9180;
		CAN_FilterInitStructure.CAN_FilterIdLow  			= 0x0000;
		CAN_FilterInitStructure.CAN_FilterMaskIdHigh 	= 0xFFE0;
		CAN_FilterInitStructure.CAN_FilterMaskIdLow 	= 0x0000;
		
		CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FIFO0;
		CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
		CAN_FilterInit(&CAN_FilterInitStructure);
		
		CAN_ClearITPendingBit(CANx, CAN_IT_FMP0);
		CAN_ClearITPendingBit(CANx, CAN_IT_TME);
		
		CAN_ITConfig(CANx, CAN_IT_FMP0, ENABLE);
		CAN_ITConfig(CANx, CAN_IT_TME, ENABLE);

}

/**
  * @brief  Configures the NVIC.
  * @param  None
  * @retval None
  */
void CAN_RX_NVIC_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Enable CANx RX0 interrupt IRQ channel */
  NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 7;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
  /* Enable CANx TX interrupt IRQ channel */
  NVIC_InitStructure.NVIC_IRQChannel = USB_HP_CAN1_TX_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 7;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

void EquipmentDataTx(u32	DataAddr, u8 DataSize, u8	*Data)
{
	CanTxMsg 	TxMessage;
	u8				j;
	
	LED2_OFF();
	
	/* transmit */
	TxMessage.StdId = 0x00;
	TxMessage.ExtId = DataAddr;
	TxMessage.RTR = CAN_RTR_DATA;
	TxMessage.IDE = CAN_ID_EXT;
	TxMessage.DLC = 8;
	for(j=0; j<DataSize; j++)
	{
		TxMessage.Data[j] = (u8)Data[j];
	}

	(void)CAN_Transmit(CANx, &TxMessage);
}

/**
  * @brief  CAN1_TxExtData
  * @param  None
  * @retval None
  */
void CAN1_TxExtData( u32 Addr, u8 * pData, u8 Size )
{
	CanTxMsg 	TxMessage;
	u8				Txbufi;
	
	LED2_OFF();
	
	/* size check */
	Size = Size > 8 ? 8 : Size;
	/* transmit box */
	TxMessage.StdId = 0x00;
	TxMessage.ExtId = (Addr & 0xFFFFFF00) | CAN_RAND();
	TxMessage.RTR = CAN_RTR_DATA;
	TxMessage.IDE = CAN_ID_EXT;
	TxMessage.DLC = Size;
	for(Txbufi = 0; Txbufi < Size; Txbufi++)
	{
		TxMessage.Data[Txbufi] = * pData ++;
	}
	
	(void)CAN_Transmit( CAN1, &TxMessage );
}


/**
  * @brief  USB_LP_CAN1_RX0_IRQHandler
  * @param  None
  * @retval None
  */
void USB_LP_CAN1_RX0_IRQHandler(void)
{
  CanRxMsg 	RxMessage;
	
	LED2_ON();

	if(CAN_GetITStatus(CANx ,CAN_IT_FMP0) != RESET)
	{
		CAN_ClearITPendingBit(CANx ,CAN_IT_FMP0);

		CAN_Receive(CANx, CAN_FIFO0, &RxMessage);
		
		if( RxMessage.RTR == CAN_RTR_REMOTE)
		{
			if( CAN1_Remote.lock_flag == UNLOCK)
			{
				/* lock data buffer */
				CAN1_Remote.lock_flag = LOCK;
				/* copy data address*/
				CAN1_Remote.Source = RxMessage.ExtId;
				/* copy data size */
				CAN1_Remote.DLS    = RxMessage.DLC;
				/* copy data */
				//CopyArrary( CAN1_Remote.Data, RxMessage.DLC, RxMessage.Data);
			}
		}
		else
		{
			switch( RxMessage.IDE )
			{
				/* Standard frame */
				case CAN_ID_STD:
					break;
				/* Extension frame */
				case CAN_ID_EXT:
					if( (RxMessage.ExtId & 0xFFE00000L) == 0x0BA00000L )
					{
						CanCommandGet( RxMessage.Data);
					}
					break; 
				/* Error frame */
				default:
					break;
			}
		}
	}
}


/**
  * @brief  USB_HP_CAN1_TX_IRQHandler
  * @param  None
  * @retval None
  */
void USB_HP_CAN1_TX_IRQHandler(void)
{
	if(CAN_GetITStatus(CANx ,CAN_IT_TME) != RESET)
	{
		CAN_ClearITPendingBit(CANx ,CAN_IT_TME);
	}
}

//#endif  /* USE_CAN1 */
