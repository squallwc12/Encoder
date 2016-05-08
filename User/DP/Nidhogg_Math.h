/**
  ******************************************************************************
  * @file    
  * @author  Chad
  * @version V2.6
  * @date    9-July-2014
  * @brief   直行程(Nidhogg),math 数学函数.
  ******************************************************************************
  * @attention
  *
  * 简单的数学程序,用于简单的数字极计算
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#ifndef __NIDHOGG_MATH_H__ 
#define __NIDHOGG_MATH_H__


/* Public define -------------------------------------------------------------*/


/* exported macro ------------------------------------------------------------*/
#define LIMIT_VALUE(X,Y) X=X>Y?Y:X  //限制X的最大值


/* exported function----------------------------------------------------------*/


/** @addtogroup math 数学函数
  * @{
  */

unsigned short MidAverageCount(unsigned char BufferSize, unsigned short* Buffer);
unsigned long  MidAverageValue(unsigned char BufferSize, unsigned long* Buffer);
unsigned short TypeConversionAD1_num(unsigned short Xnum, unsigned short XnumMax, unsigned short Xmax);
unsigned short AverageDeviation(unsigned char BufferSize, unsigned short* Buffer);
void CopyArrary(unsigned char * pBuft, unsigned char size, unsigned char * pBufs);

/**
  * @}
  */


#endif  /* __NIDHOGG_MATH_H__ */
