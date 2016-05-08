#ifndef _DATAPROCESS_H
#define _DATAPROCESS_H


#include "stm32f10x.h"
#include "ADC_Configure.h"

#define DBTEAM1_SIZE	50
#define PLUS_NUM(x)		(x = (x>=100)?(100):(x+1))

#define ADC1_DI_Power_CH		1
#define ADC1_12V_Power_CH		2
#define ADC1_5V_Power_CH		3
#define ADC1_3V3_Power_CH		4
#define ADC1_BAT_Power_CH		5
#define ADC1_TempSensor_CH	6
#define	ADC1_1V2_Ref_CH			7

typedef enum
{
		READ_WRITE = 0,
		READ_ONLY = 1
}DataTypeDef;

typedef enum
{
		SELF = 0,
		ExPart1 = 1,
		ExPart2 = 2
}DataSourceTypeDef;

typedef enum
{
		UBit8_2Type = 0,
		UBit16Type  = 1,
		SBit16Type	= 2,
		FloatType	  = 3
}DataType_TypeDef;

typedef	struct
{
		union
		{
				u8		DataU8[2];
				u16		DataU16;
				s16		DataS16;
				float	DataFL;
		}DataValue;													//数据值
		
		u16								DataID;						//数据标识符
		DataType_TypeDef	DataType;					//数据类型
		u16								DataUpdateTim;		//数据更新时间
		DataSourceTypeDef	DataSource;				//数据源
		DataTypeDef				DataMode;					//数据状态
}DataBase_TypeDef;

extern DataBase_TypeDef		DBTeam1[DBTEAM1_SIZE];

void	DataProcess_Config(void);
void	DataProcess_Scan(void);

#endif
