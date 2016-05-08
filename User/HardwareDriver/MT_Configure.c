#include 	"stm32f10x.h"
#include 	"MT_Configure.h"
#include 	"MTDAT_Demux.h"
#include	"STDAT_Demux.h"
#include 	"Nidhogg_Math.h"
#include  "CAN_Configure.h"

#define		MAX_MUXDATABUF				1024
#define		MAX_MUXDATAERRORBUF		3
#define		ABSD(X,Y)							((X>Y)?(X-Y):(Y-X))

u16									MT_MUXDATABUF[MAX_MUXDATABUF];
u16									MT_MUXDATAERROR[MAX_MUXDATAERRORBUF];
MT_MUXDAT_TyprDef		MT_MUXDAT;

u8	MT_Check( u16 MT_Value );

void Delay_MT(u16 timx)
{
		u16		x;
		for(;timx != 0; timx--)
		{
				x = 1000;
				while(x--);
		}
}

void	MTMUX_Ctrl(u8	MTMUXStatus)
{
	
	switch(MTMUXStatus)
	{
		case 0:
			GPIO_ResetBits(MT_G, MTMUX0Pin);
			GPIO_ResetBits(MT_G, MTMUX1Pin);
			GPIO_ResetBits(MT_G, MTMUX2Pin);
			break;
		case 1:
			GPIO_SetBits(MT_G, MTMUX0Pin);
			GPIO_ResetBits(MT_G, MTMUX1Pin);
			GPIO_ResetBits(MT_G, MTMUX2Pin);
			break;
		case 2:
			GPIO_ResetBits(MT_G, MTMUX0Pin);
			GPIO_SetBits(MT_G, MTMUX1Pin);
			GPIO_ResetBits(MT_G, MTMUX2Pin);
			break;
		case 3:
			GPIO_SetBits(MT_G, MTMUX0Pin);
			GPIO_SetBits(MT_G, MTMUX1Pin);
			GPIO_ResetBits(MT_G, MTMUX2Pin);
			break;
		case 4:
			GPIO_ResetBits(MT_G, MTMUX0Pin);
			GPIO_ResetBits(MT_G, MTMUX1Pin);
			GPIO_SetBits(MT_G, MTMUX2Pin);
			break;
		case 5:
			GPIO_SetBits(MT_G, MTMUX0Pin);
			GPIO_ResetBits(MT_G, MTMUX1Pin);
			GPIO_SetBits(MT_G, MTMUX2Pin);
			break;
		case 6:		//Unuseful
			GPIO_ResetBits(MT_G, MTMUX0Pin);
			GPIO_SetBits(MT_G, MTMUX1Pin);
			GPIO_SetBits(MT_G, MTMUX2Pin);
			break;
		case 7:		//Status
			GPIO_SetBits(MT_G, MTMUX0Pin);
			GPIO_SetBits(MT_G, MTMUX1Pin);
			GPIO_SetBits(MT_G, MTMUX2Pin);
			break;
		default:
			GPIO_SetBits(MT_G, MTMUX0Pin);
			GPIO_SetBits(MT_G, MTMUX1Pin);
			GPIO_SetBits(MT_G, MTMUX2Pin);
			break;
	}
}

u8	MTDAT_Ctrl(void)
{
	u8	Data = 0x00;

	//[2]
	if(GPIO_ReadInputDataBit(MT_G, MTDAT0Pin) != Bit_RESET)
	{
			Data |= 0x02; 
	}
	//[0]
	if(GPIO_ReadInputDataBit(MT_G, MTDAT1Pin) != Bit_RESET)
	{
			Data |= 0x01; 
	}
	//[1]
	if(GPIO_ReadInputDataBit(MT_G, MTDAT2Pin) != Bit_RESET)
	{
			Data |= 0x04; //02
	}
	
	return(Data);
}

void MT_TimerScan(void)
{
	static u8	TempCHx;
	static u8	HisCHx;
	//static u8	MAT_MUXDATTEMP[MT_CHNUM][MT_CacheSize];
	//static u8 MT_CacheSize_i;
	u8		i;
	
	MT_MUXDAT.MTDAT_TEM[ HisCHx ] = MTDAT_Ctrl();
	MTMUX_Ctrl( TempCHx );
	
	HisCHx = TempCHx;
	
	if( ++TempCHx < MT_CHNUM )
	{
	}
	else
	{
		TempCHx = 0;
		MT_MUXDAT.TP1s ++;
		MT_MUXDAT.MT_TRSE_FLAG = 1;
		
		for( i=0; i<MT_CHNUM; i++ )
		{
			MT_MUXDAT.MTDAT[i] = MT_MUXDAT.MTDAT_TEM[i];
		}
	}
}

void MT_Scan(void)
{
	MT_AdjustmentTurn( MT_MUXDAT.MTDAT, MT_MUXDAT.MTDAT_REC );
	MT_Turn_Count();
}

void	MT_Turn_Count(void)
{
	u8	i;
	u16	BIT12_Value, BIT13_Value;

	for( i=0; i<6; i++ )
	{
		MT_MUXDAT.MTTurnNum[i] = MT_Logic(MT_MUXDAT.MTDAT_REC[i], i);
	}
	
	BIT12_Value = MT_LogicTransform12Bit(MT_MUXDAT.MTTurnNum);
	BIT13_Value = MT_LogicTransform13Bit(MT_MUXDAT.MTTurnNum);
	
	if( MT_Check( MT_MUXDAT.MTValue_12BIT ) == 0 )
	{
		MT_MUXDAT.MTValue_12BIT = BIT12_Value;
		MT_MUXDAT.MTValue_13BIT = BIT13_Value;
	}

	MT_MUXDAT.MT_EOC = 1;
	
	//MT_MUXDAT.MTValue_12BitTemp[MT_MUXDAT.MTValue_TempNum] = MT_LogicTransform12Bit(MT_MUXDAT.MTTurnNum);
	//MT_MUXDAT.MTValue_13BitTemp[MT_MUXDAT.MTValue_TempNum] = MT_LogicTransform13Bit(MT_MUXDAT.MTTurnNum);
	//if( ++MT_MUXDAT.MTValue_TempNum >= MT_CacheSize )
	//{
	//	MT_MUXDAT.MTValue_TempNum = 0;
	//	MT_MUXDAT.MTValue_12BIT = MidAverageValue(MT_CacheSize, MT_MUXDAT.MTValue_12BitTemp);
	//	MT_MUXDAT.MTValue_13BIT = MidAverageValue(MT_CacheSize, MT_MUXDAT.MTValue_13BitTemp);
	//	MT_MUXDAT.MT_EOC = 1;
	//}

}

u8	MT_Check( u16 MT_Value )
{
	u8					RE_SR = 0;
	static u8		StartUp = 0;
	static u16	HisMTDATA;
	static u16	MTBufNum = 0, MTErrorBufNum = 0;
	static u8		MT_ErrorFlag    = 0;
	static u8		MT_ErrorFlagNum = 0;
	
	if( StartUp < 10)
	{
		StartUp ++;
	}
	else
	{
		//判断两次刷新圈数差值
		if( ABSD( MT_Value, HisMTDATA ) == 1 )
		{
			RE_SR = 0;
			
			if( MTBufNum < MAX_MUXDATABUF )
			{
				MT_MUXDATABUF[MTBufNum++] = MT_Value;
			}
			else
			{
				MTBufNum = 0;
			}
		}
		
		if( ABSD( MT_Value, HisMTDATA ) > 1 )
		{
			if( MT_Value != 0 && MT_Value != 4095 )
			{
				RE_SR = 1;
				
				MT_ErrorFlag = 1;
				
				MT_MUXDATAERROR[0] = HisMTDATA;
				MT_MUXDATAERROR[1] = MT_Value;
				MT_MUXDATAERROR[2] = 0xFFFF;
			}
		}
		
		/*	错误判断
				当前值等于历史值，而且上次检验的结果为跳码值
				既：跳码值稳定出现 5次
				*/
		if( HisMTDATA == MT_Value )
		{
			if( MT_ErrorFlag != 0)
			{
				RE_SR = 1;
				
				if( MT_ErrorFlagNum >= 5 )
				{
					MT_MUXDAT.MT_ERROR_FLAG = 0x01;
				}
				else
				{
					MT_ErrorFlagNum ++;
				}
			}
		}
		else
		{
			MT_ErrorFlagNum = 0;
			MT_ErrorFlag    = 0;
		}
	}
	
	HisMTDATA = MT_Value;
	
	return RE_SR;
}
