#ifndef _CANDAT_HANDLE_H
#define _CANDAT_HANDLE_H

#include "stm32f10x.h"

#define	CAN_DataBufferSize	16
#define AngleTotal_TempSize 3
#define BLOCK_ID						53
#define	BLOCK_STATE					73

#define	SELF_STID						(u32)(0x0000005DL &	0x000007FFL)	//Bit[10]:[0]
#define	SELF_EXID						(u32)(0x00000000L & 0x0003FF00L)	//Bit[17]:[0]
#define	SELF_FORMAT_TYPE		(u32)(0x00000004L	& 0x00000006L)	//Bit[2] :[0]
#define	SELF_SYNID					(u32)((SELF_STID << 21) | (SELF_EXID << 3) | SELF_FORMAT_TYPE)
#define	SELF_ID							(u32)((SELF_STID << 18) | (SELF_EXID))
#define	CAN_MasterID(S, E) 	((u32)(S &	0x000003FFL)<<21)|((u32)(E & 0x0001FFFFL) << 3)|((u32)0x00000004L)

#define	MasterID						0xD3
#define	EncoderID						0x5D

#define READ_DATA						0x01
#define READ_12BITMODE			0x0C
#define READ_13BITMODE			0x0D

#define READ_STATIC					0x02

#define WRITE_STATIC				0x03

#define UNLOCK							0
#define	LOCK								1
/* Public define -------------------------------------------------------------*/
#define	STD_ADDRESS(X)			(((X)&0xFFFC0000UL)>> 18 )
#define	DATA_ADDRESS(X)			(((X)&0x0000FF00UL)>>  8 )
#define	DATA_BLOCK(X)				(((X)&0x00030000UL)>> 16 )

#define CAN_ID_CONNECT(S, E)		(	((((u32)S)&0x000007FFUL)<<18) | \
																	((((u32)E)&0x000003FFUL)<<8 ) )

typedef	struct
{
	struct {
		u16	ST_Data;
		u16	MT_Data;
	}Data12Bit;
	
	u32	AngleTotal;
	u32	AngleTotalTemp[AngleTotal_TempSize];
	u8	CAN_CallBack;
	u8	CAN_TxBuffer[8];
	u16	CAN_DataBuffer[CAN_DataBufferSize];
	u32	HisAngleTotal;
	u32	APM;																	//当前角速度
}Self_Message_Typedef;

typedef struct 
{
	u8	lock_flag;
	u32	Source;		//数据来源
	u8	DLS;			//数据长度
	u8	Data[8];	//数据
}CANx_DataBuf_Type;

extern Self_Message_Typedef			Self_Message;
extern CANx_DataBuf_Type				CAN1_Remote;

void	CanData_12BitBuf(u16 MTD, u16 STD);
void	CanData_13BitBuf(u16 MTD, u16 STD);
void	CanCommandGet(u8	*Data);
void	CanCommandBack(void);
u8    CAN_RAND(void);

#endif
