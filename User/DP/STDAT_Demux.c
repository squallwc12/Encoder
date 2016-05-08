#include	"STDAT_Demux.h"
#include 	"Nidhogg_Math.h"

static u8	STDataStandard[] = {0xAA, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF};

STDAT_Buf_Typedef		STDAT_Buf;

void ST_ReceiveData(u8	StData)
{
	if(STDAT_Buf.DataTransferFlag != 0)
	{
		STDAT_Buf.DataTransferFlag   = 0;
		STDAT_Buf.DataReceiveDataNum = 0;
	}
	
	if(STDAT_Buf.DataReceiveDataNum < STDATSIZE)
	{
		STDAT_Buf.STdataTemp[STDAT_Buf.DataReceiveDataNum] = StData;
		
		if(++STDAT_Buf.DataReceiveDataNum >= (STDATSIZE - 1))
		{
			ST_GetData(STDAT_Buf.STdataTemp);
			STDAT_Buf.SPI1BsyFlag = 0;
			GPIO_SetBits(GPIOA, GPIO_Pin_4);
		}
	}
	else
	{
	}
}

u8	ST_GetData(u8 *STDataBuf)
{
	u8	STDATStruct = 0;
	u16	STDAT_AN = 0;
	u16	STDAT_NAN = 0;
	u16	STTemp;
	
	if(STDataStandard[0] != STDataBuf[0] && STDataStandard[1] != STDataBuf[1])
	{
		STDATStruct = 1;
	}
	if(STDataStandard[6] != STDataBuf[6] && STDataStandard[7] != STDataBuf[7] &&
		 STDataStandard[8] != STDataBuf[8] && STDataStandard[9] != STDataBuf[9])
	{
		STDATStruct = 1;
	}
	
	if(STDATStruct == 0)
	{
		STDAT_AN = (u16)STDataBuf[2]<<8 | STDataBuf[3];
		STDAT_NAN= (u16)STDataBuf[4]<<8 | STDataBuf[5];
		
		if((STDAT_AN ^ STDAT_NAN) == 0xFFFF)
		{
			STTemp = STDAT_AN & 0x0003;
			
			switch(STTemp)
			{
				/* Get angle value Bit14 */
				case 0x0001:
					//STDAT_Buf.STdataValueTemp[STDAT_Buf.DataTempi ++] = (STDAT_AN >> 2)&0x3FFF;
					//if(STDAT_Buf.DataTempi >= STDATVALUESIZE)
					//{
					//	STDAT_Buf.SPI_RxFlag = 1;
					//	STDAT_Buf.DataTempi  = 0;
					//	STDAT_Buf.AngleValue = MidAverageValue(STDATVALUESIZE, STDAT_Buf.STdataValueTemp) >> 2;
					//	STDAT_Buf.AngleValue_HalfF = STDAT_Buf.AngleValue >= 0x0800 ? 1 : 0;
					//}
					STDAT_Buf.AngleValue = ((STDAT_AN >> 2)&0x3FFF)>>2;
					STDAT_Buf.AngleValue_HalfF = STDAT_Buf.AngleValue >= 0x0800 ? 1 : 0;
					STDAT_Buf.SPI_RxFlag = 1;
					STDAT_Buf.TP1s ++;
					/* Clear data verify error & data error flag */
					STDAT_Buf.ErrorValue &=~(ValueVerifyErrorBit | ValueErrorBit);
					break;
				/* Get error value Bit14 */
				case 0x0002:
					STDAT_Buf.ErrorValue = STDAT_AN;
					/* Clear data verify error & data error flag */
					STDAT_Buf.ErrorValue &=~(ValueVerifyErrorBit | ValueErrorBit);
					break;
				default:
					/* Set data error flag */
					STDAT_Buf.ErrorValue |= ValueErrorBit;
					break;
			}
		}
		else
		{
			/* Set data error flag */
			STDAT_Buf.ErrorValue |= ValueVerifyErrorBit;
		}
	}
	
	return(STDATStruct);
}

void TurnCheck_Interput(void)
{
	if(STDAT_Buf.ErrorValue == 0)
	{
			STDAT_Buf.TC_STDAT = STDAT_Buf.AngleValue;
	}
}

void TimOutCheck(void)
{
	if(STDAT_Buf.SPI1BsyFlag != 0)
	{
		if(STDAT_Buf.TimOut_num++ > 100)
		{
			STDAT_Buf.TimOut_num = 0;
			STDAT_Buf.SPI1BsyFlag = 0;
			STDAT_Buf.TimOut_Temp ++;
		}
	}
	else
	{
		STDAT_Buf.TimOut_num = 0;
	}
}
