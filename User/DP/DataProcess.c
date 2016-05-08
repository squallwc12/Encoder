#include "DataProcess.h"

DataBase_TypeDef		DBTeam1[DBTEAM1_SIZE];

void	DataProcess_Config(void)
{
		DBTeam1[ADC1_DI_Power_CH].DataID = ADC1_DI_Power_CH;
		DBTeam1[ADC1_DI_Power_CH].DataType = UBit16Type;
		DBTeam1[ADC1_DI_Power_CH].DataUpdateTim = 0;
		DBTeam1[ADC1_DI_Power_CH].DataSource = SELF;
		DBTeam1[ADC1_DI_Power_CH].DataMode = READ_WRITE;
	
		DBTeam1[ADC1_12V_Power_CH].DataID = ADC1_12V_Power_CH;
		DBTeam1[ADC1_12V_Power_CH].DataType = UBit16Type;
		DBTeam1[ADC1_12V_Power_CH].DataUpdateTim = 0;
		DBTeam1[ADC1_12V_Power_CH].DataSource = SELF;
		DBTeam1[ADC1_12V_Power_CH].DataMode = READ_WRITE;
	
		DBTeam1[ADC1_5V_Power_CH].DataID = ADC1_5V_Power_CH;
		DBTeam1[ADC1_5V_Power_CH].DataType = UBit16Type;
		DBTeam1[ADC1_5V_Power_CH].DataUpdateTim = 0;
		DBTeam1[ADC1_5V_Power_CH].DataSource = SELF;
		DBTeam1[ADC1_5V_Power_CH].DataMode = READ_WRITE;
		
		DBTeam1[ADC1_BAT_Power_CH].DataID = ADC1_BAT_Power_CH;
		DBTeam1[ADC1_BAT_Power_CH].DataType = UBit16Type;
		DBTeam1[ADC1_BAT_Power_CH].DataUpdateTim = 0;
		DBTeam1[ADC1_BAT_Power_CH].DataSource = SELF;
		DBTeam1[ADC1_BAT_Power_CH].DataMode = READ_WRITE;
		
		DBTeam1[ADC1_3V3_Power_CH].DataID = ADC1_3V3_Power_CH;
		DBTeam1[ADC1_3V3_Power_CH].DataType = UBit16Type;
		DBTeam1[ADC1_3V3_Power_CH].DataUpdateTim = 0;
		DBTeam1[ADC1_3V3_Power_CH].DataSource = SELF;
		DBTeam1[ADC1_3V3_Power_CH].DataMode = READ_WRITE;
		
		DBTeam1[ADC1_TempSensor_CH].DataID = ADC1_TempSensor_CH;
		DBTeam1[ADC1_TempSensor_CH].DataType = UBit16Type;
		DBTeam1[ADC1_TempSensor_CH].DataUpdateTim = 0;
		DBTeam1[ADC1_TempSensor_CH].DataSource = SELF;
		DBTeam1[ADC1_TempSensor_CH].DataMode = READ_WRITE;
		
		DBTeam1[ADC1_1V2_Ref_CH].DataID = ADC1_1V2_Ref_CH;
		DBTeam1[ADC1_1V2_Ref_CH].DataType = UBit16Type;
		DBTeam1[ADC1_1V2_Ref_CH].DataUpdateTim = 0;
		DBTeam1[ADC1_1V2_Ref_CH].DataSource = SELF;
		DBTeam1[ADC1_1V2_Ref_CH].DataMode = READ_WRITE;
}

void	DataProcess_Scan(void)
{
		float	DataTempx = 0;
		float	Ref_PV = 0;
	
		if(ADC1_Buf.ADCUpdate != 0)
		{
				DBTeam1[ADC1_1V2_Ref_CH].DataValue.DataU16 =  
						(	ADC1_Buf.ADCConvertedValue[0][6] + ADC1_Buf.ADCConvertedValue[1][6] +
							ADC1_Buf.ADCConvertedValue[2][6] + ADC1_Buf.ADCConvertedValue[3][6] +
							ADC1_Buf.ADCConvertedValue[4][6] + ADC1_Buf.ADCConvertedValue[5][6] +
							ADC1_Buf.ADCConvertedValue[6][6] + ADC1_Buf.ADCConvertedValue[7][6])/CollectNumber;
				PLUS_NUM( DBTeam1[ADC1_1V2_Ref_CH].DataUpdateTim);
				Ref_PV = (float)DBTeam1[ADC1_1V2_Ref_CH].DataValue.DataU16 / 1475;
				DBTeam1[ADC1_1V2_Ref_CH].DataValue.DataFL = Ref_PV * 3.3;
			
				DBTeam1[ADC1_DI_Power_CH].DataValue.DataU16 =  
						(	ADC1_Buf.ADCConvertedValue[0][0] + ADC1_Buf.ADCConvertedValue[1][0] +
							ADC1_Buf.ADCConvertedValue[2][0] + ADC1_Buf.ADCConvertedValue[3][0] +
							ADC1_Buf.ADCConvertedValue[4][0] + ADC1_Buf.ADCConvertedValue[5][0] +
							ADC1_Buf.ADCConvertedValue[6][0] + ADC1_Buf.ADCConvertedValue[7][0])/CollectNumber;
				PLUS_NUM( DBTeam1[ADC1_DI_Power_CH].DataUpdateTim);
				DBTeam1[ADC1_DI_Power_CH].DataValue.DataFL = 
						(float)DBTeam1[ADC1_DI_Power_CH].DataValue.DataU16 * Ref_PV;
			
				DBTeam1[ADC1_12V_Power_CH].DataValue.DataU16 =  
						(	ADC1_Buf.ADCConvertedValue[0][1] + ADC1_Buf.ADCConvertedValue[1][1] +
							ADC1_Buf.ADCConvertedValue[2][1] + ADC1_Buf.ADCConvertedValue[3][1] +
							ADC1_Buf.ADCConvertedValue[4][1] + ADC1_Buf.ADCConvertedValue[5][1] +
							ADC1_Buf.ADCConvertedValue[6][1] + ADC1_Buf.ADCConvertedValue[7][1])/CollectNumber;
				PLUS_NUM( DBTeam1[ADC1_12V_Power_CH].DataUpdateTim);
				DBTeam1[ADC1_12V_Power_CH].DataValue.DataFL = 
						(float)DBTeam1[ADC1_12V_Power_CH].DataValue.DataU16 * Ref_PV;
			
				DBTeam1[ADC1_5V_Power_CH].DataValue.DataU16 =  
						(	ADC1_Buf.ADCConvertedValue[0][2] + ADC1_Buf.ADCConvertedValue[1][2] +
							ADC1_Buf.ADCConvertedValue[2][2] + ADC1_Buf.ADCConvertedValue[3][2] +
							ADC1_Buf.ADCConvertedValue[4][2] + ADC1_Buf.ADCConvertedValue[5][2] +
							ADC1_Buf.ADCConvertedValue[6][2] + ADC1_Buf.ADCConvertedValue[7][2])/CollectNumber;
				PLUS_NUM( DBTeam1[ADC1_5V_Power_CH].DataUpdateTim);
				DBTeam1[ADC1_5V_Power_CH].DataValue.DataFL = 
						(float)DBTeam1[ADC1_5V_Power_CH].DataValue.DataU16 * Ref_PV;
			
				DBTeam1[ADC1_BAT_Power_CH].DataValue.DataU16 =  
						(	ADC1_Buf.ADCConvertedValue[0][3] + ADC1_Buf.ADCConvertedValue[1][3] +
							ADC1_Buf.ADCConvertedValue[2][3] + ADC1_Buf.ADCConvertedValue[3][3] +
							ADC1_Buf.ADCConvertedValue[4][3] + ADC1_Buf.ADCConvertedValue[5][3] +
							ADC1_Buf.ADCConvertedValue[6][3] + ADC1_Buf.ADCConvertedValue[7][3])/CollectNumber;
				PLUS_NUM( DBTeam1[ADC1_BAT_Power_CH].DataUpdateTim);
				DBTeam1[ADC1_BAT_Power_CH].DataValue.DataFL = 
						(float)DBTeam1[ADC1_BAT_Power_CH].DataValue.DataU16 * Ref_PV;
						
				DBTeam1[ADC1_3V3_Power_CH].DataValue.DataU16 =  
						(	ADC1_Buf.ADCConvertedValue[0][4] + ADC1_Buf.ADCConvertedValue[1][4] +
							ADC1_Buf.ADCConvertedValue[2][4] + ADC1_Buf.ADCConvertedValue[3][4] +
							ADC1_Buf.ADCConvertedValue[4][4] + ADC1_Buf.ADCConvertedValue[5][4] +
							ADC1_Buf.ADCConvertedValue[6][4] + ADC1_Buf.ADCConvertedValue[7][4])/CollectNumber;
				PLUS_NUM( DBTeam1[ADC1_3V3_Power_CH].DataUpdateTim);
				DBTeam1[ADC1_3V3_Power_CH].DataValue.DataFL = 
						(float)DBTeam1[ADC1_3V3_Power_CH].DataValue.DataU16 * Ref_PV;
				
				DBTeam1[ADC1_TempSensor_CH].DataValue.DataU16 =  
						(	ADC1_Buf.ADCConvertedValue[0][5] + ADC1_Buf.ADCConvertedValue[1][5] +
							ADC1_Buf.ADCConvertedValue[2][5] + ADC1_Buf.ADCConvertedValue[3][5] +
							ADC1_Buf.ADCConvertedValue[4][5] + ADC1_Buf.ADCConvertedValue[5][5] +
							ADC1_Buf.ADCConvertedValue[6][5] + ADC1_Buf.ADCConvertedValue[7][5])/CollectNumber;
				PLUS_NUM( DBTeam1[ADC1_TempSensor_CH].DataUpdateTim);
				DataTempx = ((float)DBTeam1[ADC1_TempSensor_CH].DataValue.DataU16 / 4095) * 3.33 * Ref_PV;
				DBTeam1[ADC1_TempSensor_CH].DataValue.DataFL = (1.43f - DataTempx)/0.0043f + 25;
		}
}
