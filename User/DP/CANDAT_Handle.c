#include "CANDAT_Handle.h"
#include "CAN_Configure.h"
#include "STDAT_Demux.h"
#include "MT_Configure.h"
#include "Nidhogg_Math.h"

Self_Message_Typedef		Self_Message;
CANx_DataBuf_Type				CAN1_Remote;
CANx_DataBuf_Type				CAN1_Response;
u8	HalfTurnFlag, PG;
u16	tempiii;

u32	Can_IDOrder(u32 SID, u32 EID);
u8 CAN_RAND(void);

void	CanData_12BitBuf(u16 MTD, u16 STD)
{
	u16 tempST;
	
	Self_Message.Data12Bit.MT_Data = MTD;
	//Self_Message.Data12Bit.ST_Data = 0x0FFF - STD;
	tempST = STD >= 0x0800 ? STD - 0x0800 : STD + 0x0800;
	tempST = 0x0FFF - tempST;
	Self_Message.Data12Bit.ST_Data = tempST;
}

void	CanData_13BitBuf(u16 MTD, u16 STD)
{
	static u8	TTnum;
	
	u32 tempQT;
	u16	tempST;
	
	tempST = STD >= 0x0800 ? STD - 0x0800 : STD + 0x0800;
	tempST = 0x0FFF - tempST;
	tempQT = MTD*4096 + tempST;
	
	Self_Message.AngleTotalTemp[TTnum++] = tempQT;
	if( TTnum >= AngleTotal_TempSize )
	{
		TTnum = 0;
		Self_Message.AngleTotal = MidAverageValue( AngleTotal_TempSize, (unsigned long *)Self_Message.AngleTotalTemp );
	}
}

void	CanCommandGet(u8	*Data)
{
	switch(Data[0])
	{
		case READ_DATA:
			if(Data[1] == READ_12BITMODE)
			{
				//圈数+角度
				Self_Message.CAN_TxBuffer[0] = EncoderID;
				Self_Message.CAN_TxBuffer[1] = 0x00;
				Self_Message.CAN_TxBuffer[2] = READ_DATA;
				Self_Message.CAN_TxBuffer[3] = READ_12BITMODE;
				Self_Message.CAN_TxBuffer[4] = Self_Message.Data12Bit.MT_Data >> 8;
				Self_Message.CAN_TxBuffer[5] = Self_Message.Data12Bit.MT_Data & 0x00FF;
				Self_Message.CAN_TxBuffer[6] = Self_Message.Data12Bit.ST_Data >> 8;
				Self_Message.CAN_TxBuffer[7] = Self_Message.Data12Bit.ST_Data & 0x00FF;
			}
			if(Data[1] == READ_13BITMODE)
			{
				//角度
				Self_Message.CAN_TxBuffer[0] = EncoderID;
				Self_Message.CAN_TxBuffer[1] = 0x00;
				Self_Message.CAN_TxBuffer[2] = READ_DATA;
				Self_Message.CAN_TxBuffer[3] = READ_13BITMODE;
				Self_Message.CAN_TxBuffer[4] =(Self_Message.AngleTotal >> 24) & 0x00FF;
				Self_Message.CAN_TxBuffer[5] =(Self_Message.AngleTotal >> 16) & 0x00FF;
				Self_Message.CAN_TxBuffer[6] =(Self_Message.AngleTotal >>  8) & 0x00FF;
				Self_Message.CAN_TxBuffer[7] =(Self_Message.AngleTotal) & 0x00FF;
			}
			Self_Message.CAN_CallBack = 1;
			break;
		case READ_STATIC:
			{
				Self_Message.CAN_TxBuffer[0] = EncoderID;
				Self_Message.CAN_TxBuffer[1] = 0x00;
				Self_Message.CAN_TxBuffer[2] = READ_STATIC;
				Self_Message.CAN_TxBuffer[3] = 0x00;
				Self_Message.CAN_TxBuffer[4] = STDAT_Buf.ErrorValue >> 8;
				Self_Message.CAN_TxBuffer[5] = STDAT_Buf.ErrorValue & 0x00FF;
				Self_Message.CAN_TxBuffer[6] = 0x00;
				Self_Message.CAN_TxBuffer[7] = 0x00;
			}
			Self_Message.CAN_CallBack = 1;
			break;
		case WRITE_STATIC:
			{
				if(Data[1] < CAN_DataBufferSize)
				{
					Self_Message.CAN_DataBuffer[ Data[1]] = (u16)Data[2]<<8 + (u16)Data[3];
				}
				Self_Message.CAN_TxBuffer[0] = EncoderID;
				Self_Message.CAN_TxBuffer[1] = 0x00;
				Self_Message.CAN_TxBuffer[2] = WRITE_STATIC;
				Self_Message.CAN_TxBuffer[3] = Data[1];
				Self_Message.CAN_TxBuffer[4] = Self_Message.CAN_DataBuffer[ Data[1]] >> 8;
				Self_Message.CAN_TxBuffer[5] = Self_Message.CAN_DataBuffer[ Data[1]] & 0x00FF;
				Self_Message.CAN_TxBuffer[6] = 0x00;
				Self_Message.CAN_TxBuffer[7] = 0x00;
			}
			Self_Message.CAN_CallBack = 1;
			break;
	}
}

u8	CAN_RandomNo;

void	CanCommandBack(void)
{
	u32	StdAddr;
	u32	dHead;

	/**
		*	测试模式
		*/
	if( Self_Message.CAN_CallBack != 0 )
	{
		Self_Message.CAN_CallBack = 0;
		
		CAN_RandomNo = (++CAN_RandomNo) & 0x7F;

		EquipmentDataTx(CAN_MasterID(MasterID, (((u16)EncoderID << 8)|CAN_RandomNo)), 8, Self_Message.CAN_TxBuffer);
	}
	/**
		*	通讯模式
		*/
	if( CAN1_Remote.lock_flag == LOCK )
	{
		StdAddr = STD_ADDRESS( CAN1_Remote.Source );
		dHead 	=	DATA_ADDRESS( CAN1_Remote.Source );
		
		switch( dHead )
		{
			case BLOCK_ID:
				CAN1_Response.Source	=	CAN_ID_CONNECT( StdAddr, dHead );
				CAN1_Response.DLS			= CAN1_Remote.DLS;
				CAN1_Response.Data[0] = (Self_Message.AngleTotal);
				CAN1_Response.Data[1] = (Self_Message.AngleTotal >>  8);
				CAN1_Response.Data[2] = (Self_Message.AngleTotal >> 16);
				CAN1_Response.Data[3] = (Self_Message.AngleTotal >> 24);
				CAN1_Response.Data[4] = (Self_Message.APM);
				CAN1_Response.Data[5] = (Self_Message.APM >>  8);
				CAN1_Response.Data[6] = (Self_Message.APM >>  16);
				CAN1_Response.Data[7] = (Self_Message.APM >>  24);
				CAN1_TxExtData( CAN1_Response.Source, CAN1_Response.Data, CAN1_Response.DLS );
				break;
			case BLOCK_STATE:
				CAN1_Response.Source	=	CAN_ID_CONNECT( StdAddr, dHead );
				CAN1_Response.DLS			= CAN1_Remote.DLS;
				CAN1_Response.Data[0] = 0xAA;
				CAN1_Response.Data[1] = MT_MUXDAT.MT_ERROR_FLAG | 0x00;	//error byte
				CAN1_Response.Data[2] = 0x00;
				CAN1_Response.Data[3] = 0x00;
				CAN1_Response.Data[4] = 0x00;
				CAN1_Response.Data[5] = 0x00;
				CAN1_Response.Data[6] = 0x00;
				CAN1_Response.Data[7] = 0x55;
				CAN1_TxExtData( CAN1_Response.Source, CAN1_Response.Data, CAN1_Response.DLS );
				break;
		}
		
		CAN1_Remote.lock_flag = UNLOCK;
	}
}

u32	Can_IDOrder(u32 SID, u32 EID)
{
	return (((u32)SID &	0x000003FFL)<<21)|(((u32)EID & 0x0001FFFFL) << 3)|((u32)0x00000004L);
}

/**
  * @brief  Configures the NVIC.
  * @param  None
  * @retval None
  */
u8 CAN_RAND(void)
{
	static u8	RAND_NUM;
	
	return (u8)(RAND_NUM ++);
}
