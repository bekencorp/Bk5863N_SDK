#include "ClkOutput.h"
#include "BK5863.h"

/**************************************************
FunctionName: ClockOutputinit
Describe: 
InputParameter:
OutputParameter:
Notice:
**************************************************/
void ClockOutputinit(uint8_t Clk,uint8_t cmd)
{
	/* GPIO Config */
	M_XH_CLKOUT_B7_INIT;
	M_XH_CLKOUT_B7_EN;
	/* Clock Set DIV & Disable_Enable */
	REG_APB3_ICU_GPIOB7CLKCON = (REG_APB3_ICU_GPIOB7CLKCON&0xFFFFFFF8)|(Clk&0x07);
	if(cmd)
		REG_APB3_ICU_GPIOB7CLKCON &= (~B7_XH_CLKOUT_EN);  //Ê¹ÄÜ
	else
		REG_APB3_ICU_GPIOB7CLKCON |= B7_XH_CLKOUT_EN;
}

/**************************************************
FunctionName: SetClockOutput
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
void SetClockOutputEnable(int cmd)
{
	if(cmd)
	{
		REG_APB3_ICU_GPIOB7CLKCON &= (~B7_XH_CLKOUT_EN);
	}
	else
	{
		REG_APB3_ICU_GPIOB7CLKCON |= B7_XH_CLKOUT_EN;
	}
}
/**************************************************
FunctionName: SetXHClock
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
void SetXHClock(uint8_t Clk)
{
	REG_APB3_ICU_GPIOB7CLKCON = (REG_APB3_ICU_GPIOB7CLKCON&0xFFFFFFF8)|(Clk&0x07);
}



/***********************  END OF FILES  ***********************/
