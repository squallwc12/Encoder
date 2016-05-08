#ifndef _STDAT_DEMUX_H
#define _STDAT_DEMUX_H


#include "stm32f10x.h"

#define	STDATSIZE					10
#define	STDATVALUESIZE		3

#define	ValueVerifyErrorBit		0x8000
#define ValueErrorBit					0x2000

typedef struct
{
	u8	SPI1BsyFlag;
	u8	SPI_EOC;
	u8	DataTransferFlag;
	u8	DataReceiveDataNum;
	u8	DataTempi;
	u8	STdataTemp[STDATSIZE];
	u8	SPI_RxFlag;
	u16	STdataValueTemp[STDATVALUESIZE];
	u16	AngleValue;
	u8	AngleValue_HalfF;
	u16	ErrorValue;
	u16	TC_STDAT;
	u16	TC_MTDAT;
	u32	TimOut_num;
	u32	TimOut_Temp;
	u32	BSY_TIM;
	u16	TP1s;
	u16	ST_MUX_TP1s;
}STDAT_Buf_Typedef;

extern STDAT_Buf_Typedef		STDAT_Buf;

void ST_ReceiveData(u8	StData);
u8	ST_GetData(u8 *STDataBuf);
void TurnCheck_Interput(void);
void TimOutCheck(void);

#endif
