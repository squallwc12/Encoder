/**
  ******************************************************************************
  * @file    
  * @author  Chad
  * @version V2.6
  * @date    9-July-2014
  * @brief   ֱ�г�(Nidhogg),��ѧ�㷨.
  ******************************************************************************
  * @attention
  * 
  * <h2><center>&copy; COPYRIGHT 2011 Flowinn</center></h2>
  ******************************************************************************
  */
#include "Nidhogg_Math.h"

/* Includes ------------------------------------------------------------------*/

/** @addtogroup Niddhogg_main
  * @{
  */
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/


/**
  * @brief  MidAverageCount   ��ֵ�˲�
  * @param  BufferSize        ���鳤��
  * @param  Buffer            ����ָ��
  * @retval �м�ƽ��ֵ
  */
unsigned short MidAverageCount(unsigned char BufferSize, unsigned short* Buffer)
{
	unsigned short    temp;
	unsigned long     Buffer_Totle=0;
	unsigned char     i,j;
	/**
		* 1.ð������
		* ���ȶ��ĸ��Ӷ�
		* 2.ȥ����Сֵ�����ֵ,���м�ֵ
		* 3.ȡ�м�ƽ��ֵ
		*/
	for(j=1; j<BufferSize; j++)
	{
		for(i=0;i<BufferSize-j;i++)
		{
			if(Buffer[i]>Buffer[i+1])
			{
				temp = Buffer[i];
				Buffer[i] = Buffer[i+1];
				Buffer[i+1] = temp;
			}
		}
	}
	
	for(j=1;j<BufferSize-1;j++)
	{
		Buffer_Totle += Buffer[j];
	}
	
	Buffer_Totle = Buffer_Totle/(BufferSize-2);
	
	return (unsigned short)Buffer_Totle;
}

/**
  * @brief  MidAverageCount   ��ֵ
  * @param  BufferSize        ���鳤��
  * @param  Buffer            ����ָ��
  * @retval �м�ƽ��ֵ
  */
unsigned long MidAverageValue( unsigned char BufferSize, unsigned long * Buffer )
{
	unsigned long    temp;
	unsigned char     i,j;
	/**
		* 1.ð������
		* ���ȶ��ĸ��Ӷ�
		* 2.ȥ����Сֵ�����ֵ,���м�ֵ
		* 3.ȡ�м�ƽ��ֵ
		*/
	for(j=1; j<BufferSize; j++)
	{
		for(i=0; i<BufferSize-j; i++)
		{
			if(Buffer[i]>Buffer[i+1])
			{
				temp = Buffer[i];
				Buffer[i] = Buffer[i+1];
				Buffer[i+1] = temp;
			}
		}
	}
	
	return (unsigned long)Buffer[(BufferSize>>1)];
}


/**
  * @brief  TypeConversionAD1_num   ���ݱ�������
  * @param  Xnum                    ��Ҫת��������
  * @param  XnumMax                 ��Ҫת�������ֿ��ܵ����ֵ(����)
  * @param  Xmax                    ת������������ֵ(����)
  * @retval ת���������
  */
unsigned short TypeConversionAD1_num(unsigned short Xnum, unsigned short XnumMax, unsigned short Xmax)
{
	unsigned long Temp;
	
	LIMIT_VALUE(Xnum, XnumMax);
	/**
		* a/b = x/d 
		* x = a*d/b
		* Temp = Xnum * Xmax / XunumMax
		* Temp = Xnum * Xmax +(XnumMax>>1)/ XunumMax 
		* XnumMax >>1 = XnumMax/2 (��������)
		* @warning  ����ʹ�ó����ή��Ч��
		*/
	if(Xnum>0)
	{
		Temp = ((unsigned long)Xnum * Xmax + (XnumMax>>1)) / XnumMax;
	}
	else
	{
		Temp = 0;
	}
	
	return (unsigned short)Temp;
}

unsigned short AverageDeviation(unsigned char BufferSize, unsigned short* Buffer)
{
	unsigned short tempi = 0, tempj = 0;
	unsigned char  i;
	
	tempi = MidAverageCount(BufferSize, Buffer);
	
	for(i=0; i<BufferSize; i++)
	{
		if(tempi > Buffer[i])
			tempj += (tempi - Buffer[i]);
		else
			tempj += (Buffer[i] - tempi);
	}
	
	return tempj;
}

/**
  * @brief  DyadicSigma							���
  * @param  BufferSize              ���ݳ���
  * @param  Buffer                 	������
  * @retval ��
  */
void CopyArrary(unsigned char * pBuft, unsigned char size, unsigned char * pBufs)
{
	unsigned char i;
	for(i=0; i<size; i++)
	{
		* pBuft ++ = * pBufs ++;
	}
}

  /**
  * @}
  */
/******************* (C) COPYRIGHT 2014 FLowinn ****************END OF FILE****/
