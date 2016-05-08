#include "MTDAT_Demux.h"
#include "STDAT_Demux.h"

u8	LogicValue[] ={0x01, 0x03, 0x02, 0x06, 0x07, 0x05, 0x04, 0x00};
u8	MTZeroPos[] = {0x03, 0x03, 0x03, 0x05, 0x01, 0x06};
u8	MTZeroPosi[]= {1, 1, 1, 5, 0, 3};

u16	MT_Logic(u8	GearData, u8 Gear)
{
	u8	Temp = 0;
	u8	Tempi = 0;
	
	for(Temp=0; Temp< 8; Temp++)
	{
		if(LogicValue[Temp] == GearData)
		{
			break;
		}
	}
	
	if(Temp >= MTZeroPosi[Gear])
	{
		Tempi = Temp - MTZeroPosi[Gear];
	}
	else
	{
		Tempi = (8 - MTZeroPosi[Gear]) + Temp;
	}
	
	return(Tempi);
}

u16	MT_LogicTransform12Bit(u8	*GearN)
{
	u16 BackC;
	u16	BackNum[6];
	
	BackNum[5] = GearN[5] * 512;
	BackNum[4] = (GearN[4]%4) * 128;
	BackNum[3] = (GearN[3]%4) * 32;
	BackNum[2] = (GearN[2]%4) * 8;
	BackNum[1] = (GearN[1]%4) * 2;
	BackNum[0] = (GearN[0]%4) >> 1;
	
	BackC = BackNum[0] + BackNum[1] + BackNum[2] + BackNum[3] + BackNum[4] + BackNum[5];
	return(BackC);
}

u16	MT_LogicTransform13Bit(u8	*GearN)
{
	u16 BackC;
	u16	BackNum[6];
	
	BackNum[5] = (GearN[5] * 1024);
	BackNum[4] = ((GearN[4]%4) * 256);
	BackNum[3] = ((GearN[3]%4) * 64);
	BackNum[2] = ((GearN[2]%4) * 16);
	BackNum[1] = ((GearN[1]%4) * 4);
	BackNum[0] = (GearN[0]%4);
	
	BackC = BackNum[0] + BackNum[1] + BackNum[2] + BackNum[3] + BackNum[4] + BackNum[5];
	return(BackC);
}

u8 TempAngleValue_HalfF;

void 	MT_AdjustmentTurn(u8 *pDAT, u8 *pDAT_REC)
{
	u8	TempData = 0;
	static u8 TempAHalfF = 0, TempBHalfF = 0;
	//static u8 TempAngleValue_HalfF;
	
	//pDAT_REC[0] = pDAT[0];
	//TempData    = pDAT[1];
	

	
	if( TempAngleValue_HalfF != STDAT_Buf.AngleValue_HalfF)
	{
		if(STDAT_Buf.AngleValue_HalfF != 0)
		{
			TempBHalfF = 1;
		}
		else
		{
			TempAHalfF = 1;
		}
		//TempAHalfF = (STDAT_Buf.AngleValue_HalfF == 0)?1:0;
		//TempBHalfF = (STDAT_Buf.AngleValue_HalfF != 0)?1:0;
	}
	TempAngleValue_HalfF = STDAT_Buf.AngleValue_HalfF;
	
	//BIT[0] Adjustment
	TempData = pDAT[0];
	
	//if(TempAHalfF != 0)
	if(STDAT_Buf.AngleValue_HalfF == 0)
	{
		switch(pDAT[0])
		{
			case 0x07:
				TempData = 0x05;
				break;
			case 0x04:
				TempData = 0x00;
				break;
			case 0x01:
				TempData = 0x03;
				break;
			case 0x02:
				TempData = 0x06;
				break;
		}
	}
	else
	{
		switch(pDAT[0])
		{
			case 0x06:
				TempData = 0x07;
				break;
			case 0x05:
				TempData = 0x04;
				break;
			case 0x00:
				TempData = 0x01;
				break;
			case 0x03:
				TempData = 0x02;
				break;
		}
	}
	//BIT[1] Adjustment
	pDAT_REC[0] = TempData;
	
	TempData = pDAT[1];
	//BIT[1] Adjustment
	if(pDAT_REC[0] == 0x03 || pDAT_REC[0] == 0x02)
	{
		switch(pDAT[1])
		{
			case 0x07:
				TempData = 0x05;
				break;
			case 0x04:
				TempData = 0x00;
				break;
			case 0x01:
				TempData = 0x03;
				break;
			case 0x02:
				TempData = 0x06;
				break;
		}
	}
	if(pDAT_REC[0] == 0x01 || pDAT_REC[0] == 0x00)
	{
		switch(pDAT[1])
		{
			case 0x05:
				TempData = 0x07;
				break;
			case 0x00:
				TempData = 0x04;
				break;
			case 0x03:
				TempData = 0x01;
				break;
			case 0x06:
				TempData = 0x02;
				break;
		}
	}
	if(pDAT_REC[0] == 0x05 || pDAT_REC[0] == 0x04)
	{
		switch(pDAT[1])
		{
			case 0x06:
				TempData = 0x07;
				break;
			case 0x05:
				TempData = 0x04;
				break;
			case 0x00:
				TempData = 0x01;
				break;
			case 0x03:
				TempData = 0x02;
				break;
		}
	}
	if(pDAT_REC[0] == 0x07 || pDAT_REC[0] == 0x06)
	{
		switch(pDAT[1])
		{
			case 0x07:
				TempData = 0x06;
				break;
			case 0x04:
				TempData = 0x05;
				break;
			case 0x01:
				TempData = 0x00;
				break;
			case 0x02:
				TempData = 0x03;
				break;
		}
	}
	//BIT[1] Adjustment
	pDAT_REC[1] = TempData;
	
	TempData = pDAT[2];
	//BIT[2] Adjustment
	if(pDAT_REC[1] == 0x07 || pDAT_REC[1] == 0x06)
	{
		switch(pDAT[2])
		{
			case 0x07:
				TempData = 0x06;
			break;
			case 0x04:
				TempData = 0x05;
			break;
			case 0x01:
				TempData = 0x00;
			break;
			case 0x02:
				TempData = 0x03;
			break;
		}
	}
	if(pDAT_REC[1] == 0x05 || pDAT_REC[1] == 0x04)
	{
		switch(pDAT[2])
		{
			case 0x06:
				TempData = 0x07;
			break;
			case 0x05:
				TempData = 0x04;
			break;
			case 0x00:
				TempData = 0x01;
			break;
			case 0x03:
				TempData = 0x02;
			break;
		}
	}
	if(pDAT_REC[1] == 0x01 || pDAT_REC[1] == 0x00)
	{
		switch(pDAT[2])
		{
			case 0x05:
				TempData = 0x07;
			break;
			case 0x00:
				TempData = 0x04;
			break;
			case 0x03:
				TempData = 0x01;
			break;
			case 0x06:
				TempData = 0x02;
			break;
		}
	}
	if(pDAT_REC[1] == 0x03 || pDAT_REC[1] == 0x02)
	{
		switch(pDAT[2])
		{
			case 0x07:
				TempData = 0x05;
			break;
			case 0x04:
				TempData = 0x00;
			break;
			case 0x01:
				TempData = 0x03;
			break;
			case 0x02:
				TempData = 0x06;
			break;
		}
	}
	//BIT[2] Adjustment
	pDAT_REC[2] = TempData;
	
	TempData = pDAT[3];
	//BIT[3] Adjustment
	if(pDAT_REC[2] == 0x07 || pDAT_REC[2] == 0x06)
	{
		switch(pDAT[3])
		{
			case 0x07:
				TempData = 0x06;
			break;
			case 0x04:
				TempData = 0x05;
			break;
			case 0x01:
				TempData = 0x00;
			break;
			case 0x02:
				TempData = 0x03;
			break;
		}
	}
	if(pDAT_REC[2] == 0x05 || pDAT_REC[2] == 0x04)
	{
		switch(pDAT[3])
		{
			case 0x06:
				TempData = 0x07;
			break;
			case 0x05:
				TempData = 0x04;
			break;
			case 0x00:
				TempData = 0x01;
			break;
			case 0x03:
				TempData = 0x02;
			break;
		}
	}
	if(pDAT_REC[2] == 0x01 || pDAT_REC[2] == 0x00)
	{
		switch(pDAT[3])
		{
			case 0x05:
				TempData = 0x07;
			break;
			case 0x00:
				TempData = 0x04;
			break;
			case 0x03:
				TempData = 0x01;
			break;
			case 0x06:
				TempData = 0x02;
			break;
		}
	}
	if(pDAT_REC[2] == 0x03 || pDAT_REC[2] == 0x02)
	{
		switch(pDAT[3])
		{
			case 0x07:
				TempData = 0x05;
			break;
			case 0x04:
				TempData = 0x00;
			break;
			case 0x01:
				TempData = 0x03;
			break;
			case 0x02:
				TempData = 0x06;
			break;
		}
	}
	//BIT[3] Adjustment
	pDAT_REC[3] = TempData;
	
	TempData = pDAT[4];
	//BIT[4] Adjustment
	if(pDAT_REC[3] == 0x07 || pDAT_REC[3] == 0x06)
	{
		switch(pDAT[4])
		{
			case 0x07:
				TempData = 0x06;
			break;
			case 0x04:
				TempData = 0x05;
			break;
			case 0x01:
				TempData = 0x00;
			break;
			case 0x02:
				TempData = 0x03;
			break;
		}
	}
	if(pDAT_REC[3] == 0x05 || pDAT_REC[3] == 0x04)
	{
		switch(pDAT[4])
		{
			case 0x06:
				TempData = 0x07;
			break;
			case 0x05:
				TempData = 0x04;
			break;
			case 0x00:
				TempData = 0x01;
			break;
			case 0x03:
				TempData = 0x02;
			break;
		}
	}
	if(pDAT_REC[3] == 0x01 || pDAT_REC[3] == 0x00)
	{
		switch(pDAT[4])
		{
			case 0x05:
				TempData = 0x07;
			break;
			case 0x00:
				TempData = 0x04;
			break;
			case 0x03:
				TempData = 0x01;
			break;
			case 0x06:
				TempData = 0x02;
			break;
		}
	}
	if(pDAT_REC[3] == 0x03 || pDAT_REC[3] == 0x02)
	{
		switch(pDAT[4])
		{
			case 0x07:
				TempData = 0x05;
			break;
			case 0x04:
				TempData = 0x00;
			break;
			case 0x01:
				TempData = 0x03;
			break;
			case 0x02:
				TempData = 0x06;
			break;
		}
	}
	//BIT[4] Adjustment
	pDAT_REC[4] = TempData;
	
	//BIT[5] Adjustment
	TempData = pDAT[5];
	if(pDAT_REC[4] == 0x06 || pDAT_REC[4] == 0x02)
	{
		switch(pDAT[5])
		{
			case 0x07:
				TempData = 0x06;
			break;
			case 0x04:
				TempData = 0x05;
			break;
			case 0x01:
				TempData = 0x00;
			break;
			case 0x02:
				TempData = 0x03;
			break;
		}
	}
	if(pDAT_REC[4] == 0x07 || pDAT_REC[4] == 0x05)
	{
		switch(pDAT[5])
		{
			case 0x06:
				TempData = 0x07;
			break;
			case 0x05:
				TempData = 0x04;
			break;
			case 0x00:
				TempData = 0x01;
			break;
			case 0x03:
				TempData = 0x02;
			break;
		}
	}
	if(pDAT_REC[4] == 0x00 || pDAT_REC[4] == 0x04)
	{
		switch(pDAT[5])
		{
			case 0x05:
				TempData = 0x07;
			break;
			case 0x00:
				TempData = 0x04;
			break;
			case 0x03:
				TempData = 0x01;
			break;
			case 0x06:
				TempData = 0x02;
			break;
		}
	}
	if(pDAT_REC[4] == 0x01 || pDAT_REC[4] == 0x03)
	{
		switch(pDAT[5])
		{
			case 0x07:
				TempData = 0x05;
			break;
			case 0x04:
				TempData = 0x00;
			break;
			case 0x01:
				TempData = 0x03;
			break;
			case 0x02:
				TempData = 0x06;
			break;
		}
	}
	pDAT_REC[5] = TempData;
}

