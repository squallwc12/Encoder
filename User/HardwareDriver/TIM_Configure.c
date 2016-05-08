#include 	"TIM_Configure.h"
#include 	"MT_Configure.h"
#include	"STDAT_Demux.h"
#include 	"CANDAT_Handle.h"
#include 	"NVIC_Configure.h"

void TIM2_Config(void)
{
		TIM_TimeBaseInitTypeDef  	TIM_TimeBaseStructure;
		NVIC_InitTypeDef  				NVIC_InitStructure;
	
		/* TIM6 clock enable */
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
		TIM_InternalClockConfig(TIM2);
	
		/* Time base configuration */
		/* 72MHz / 72 = 1MHz */
		TIM_TimeBaseStructure.TIM_ClockDivision = 0;
		TIM_TimeBaseStructure.TIM_Period = 10000;
		TIM_TimeBaseStructure.TIM_Prescaler = (7200 - 1);
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
		TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

		NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 15;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
	
		/* TIM6 clear TIM_FLAG_Update */
		TIM_ClearFlag(TIM2, TIM_FLAG_Update);
		/* TIM6 disable ArrPreload */
		TIM_ARRPreloadConfig(TIM2, DISABLE);
		/* TIM6 enable TIM_IT_Update */
		TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
		/* TIM6 enable counter */
		TIM_Cmd(TIM2, ENABLE);
}

void TIM3_Config(void)
{
		TIM_TimeBaseInitTypeDef  	TIM_TimeBaseStructure;
		NVIC_InitTypeDef  				NVIC_InitStructure;
	
		/* TIM6 clock enable */
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
		TIM_InternalClockConfig(TIM3);
	
		/* Time base configuration */
		/* 72MHz / 72 = 1MHz */
		TIM_TimeBaseStructure.TIM_ClockDivision = 0;
		TIM_TimeBaseStructure.TIM_Period = 550;
		TIM_TimeBaseStructure.TIM_Prescaler = (72 - 1);
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
		TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

		NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
	
		/* TIM6 clear TIM_FLAG_Update */
		TIM_ClearFlag(TIM3, TIM_FLAG_Update);
		/* TIM6 disable ArrPreload */
		TIM_ARRPreloadConfig(TIM3, DISABLE);
		/* TIM6 enable TIM_IT_Update */
		TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
		/* TIM6 enable counter */
		TIM_Cmd(TIM3, ENABLE);
}

void TIM4_Config(void)
{
		TIM_TimeBaseInitTypeDef  	TIM_TimeBaseStructure;
		NVIC_InitTypeDef  				NVIC_InitStructure;
	
		/* TIM6 clock enable */
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
		TIM_InternalClockConfig(TIM4);
	
		/* Time base configuration */
		/* 72MHz / 72 = 1MHz */
		TIM_TimeBaseStructure.TIM_ClockDivision = 0;
		TIM_TimeBaseStructure.TIM_Period = 1000;
		TIM_TimeBaseStructure.TIM_Prescaler = (72 - 1);
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
		TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

		NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 15;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
	
		/* TIM6 clear TIM_FLAG_Update */
		TIM_ClearFlag(TIM4, TIM_FLAG_Update);
		/* TIM6 disable ArrPreload */
		TIM_ARRPreloadConfig(TIM4, DISABLE);
		/* TIM6 enable TIM_IT_Update */
		TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
		/* TIM6 enable counter */
		TIM_Cmd(TIM4, ENABLE);
}

/*
	
*/
void TIM2_IRQHandler(void)
{
	/* TIM6 clear TIM_FLAG_Update */
	if( TIM_GetITStatus(TIM2, TIM_FLAG_Update)  != RESET)
	{
		TIM_ClearITPendingBit(TIM2, TIM_FLAG_Update);
		
		MT_MUXDAT.MT_MUX_TP1s = MT_MUXDAT.TP1s;
		MT_MUXDAT.TP1s = 0;
		STDAT_Buf.ST_MUX_TP1s = STDAT_Buf.TP1s;
		STDAT_Buf.TP1s = 0;
		
		Self_Message.APM = (Self_Message.HisAngleTotal > Self_Message.AngleTotal)?
			(Self_Message.HisAngleTotal - Self_Message.AngleTotal):
			(Self_Message.AngleTotal - Self_Message.HisAngleTotal);
		
		Self_Message.HisAngleTotal = Self_Message.AngleTotal;
	}
}


/*
	
*/
void TIM3_IRQHandler(void)
{
	/* TIM6 clear TIM_FLAG_Update */
	if( TIM_GetITStatus(TIM3, TIM_FLAG_Update)  != RESET)
	{
		TIM_ClearITPendingBit(TIM3, TIM_FLAG_Update);
		
		MT_TimerScan();
	}
}

/*
	
*/
void TIM4_IRQHandler(void)
{
	/* TIM6 clear TIM_FLAG_Update */
	if( TIM_GetITStatus(TIM4, TIM_FLAG_Update)  != RESET)
	{
		TIM_ClearITPendingBit(TIM4, TIM_FLAG_Update);
		
		TimOutCheck();
	}
}


