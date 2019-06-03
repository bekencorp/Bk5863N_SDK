#include "WDT.h"
#include "BK5863.h"

/**************************************************
FunctionName:BK5863WDT
Describe: 开启WDK功能
InputParameter:counter :Watch Dog Timer周期
OutputParameter:
Notice:
**************************************************/
void BK5863WDT(unsigned int  counter)
{
	REG_APB3_ICU_WDTCLKCON = 0;
	REG_WDT_CTRL = (0x5A<<16)|counter;
	REG_WDT_CTRL = (0xA5<<16)|counter;
}

/**************************************************
FunctionName:BK5863WDTClose
Describe: 关闭WDT
InputParameter:
OutputParameter:
Notice:关闭WDT的时钟及REG_WDT_CTRL值清0
**************************************************/
void BK5863WDTClose(void)
{
	REG_APB3_ICU_WDTCLKCON |= 1;
	REG_WDT_CTRL = 0;
}

/**************************************************
FunctionName:BK5863WDTCLK
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
void BK5863WDTCLK(int cmd)
{
	if(cmd)
		REG_APB3_ICU_WDTCLKCON = 0;
	else
		REG_APB3_ICU_WDTCLKCON |= 1;
}

/**************************************************
FunctionName:BK5863WDTCLK
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
void BK5863WDT_PERIOD(unsigned int  counter)
{
	REG_WDT_CTRL = (REG_WDT_CTRL&0xFFFF0000)|(counter&0xFFFF);
}

/**************************************************
FunctionName:BK5863WDTCLK
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
void BK5863WDTLaunch(int cmd)
{
	if(cmd)
	{
		REG_WDT_CTRL = (0x5A<<16)|0xFFFF;
	  REG_WDT_CTRL = (0xA5<<16)|0xFFFF;
	}
	else
	{
		REG_WDT_CTRL = 0;
	}
}






/***********************  END OF FILES  ***********************/
