#include "SysTime.h"

/*******************************************************************
*函数名称:delay_us()
*函数描述:等待
*作    者:
*输入参数:num:等待时间长度(单位us)
*输出参数:无
*返回说明:无
*其它说明:无
*******************************************************************/

//#pragma ARM
void delay_us(unsigned int num)
{
	while(num--)
	{
		__asm //volatile
		{
			nop;	//40ns
			nop;
			nop;
			nop;
			nop;
			nop;
			nop;
			nop;
			
			nop;
			nop;
			nop;
			nop;
			nop;
			nop;
			nop;
			nop;
			
			nop;
			nop;
			nop;
			nop;
			nop;
			nop;
			nop;
			nop;
			
			nop;
			nop;
			nop;
			nop;
		}
	}
}

/**********************************************************************************************
*函数名称:delay_ms()
*函数描述:等待
*作    者:
*输入参数:num:等待时间长度(单位ms)
*输出参数:无
*返回说明:无
*其它说明:无
***********************************************************************************************/
void delay_ms(unsigned int num)
{
	while(num--)
	{
		delay_us(1000);
	}
}
/**********************************************************************************************
*函数名称:delay_cycle()
*函数描述:延迟时钟周期
*作    者:
*输入参数:num:延迟时钟周期长度
*输出参数:无
*返回说明:无
*其它说明:无
***********************************************************************************************/
void delay_cycle(int num)
{
	while(num--);
}


/***********************  END OF FILES  ***********************/

