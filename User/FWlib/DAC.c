#include "DAC.h"
#include "BK5863.h"
/**************************************************
FunctionName:AUD_CLKDIV
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
void AUD_CLKDIV(uint32_t Div)
{
	REG_APB2_DAC_CLKDIVID = Div;
}

/**************************************************
FunctionName:
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
void DACClock(uint8_t cmd)
{
	if(cmd)
		REG_APB3_ICU_DACCLKCON  = 0;							//DAC clock enable
	else
		REG_APB3_ICU_DACCLKCON  |= 1;
}

/**************************************************
FunctionName:DACInt
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
void DACInt(uint8_t cmd)
{
	if(cmd)
		REG_APB3_ICU_INT_ENABLE |= INT_STATUS_DAC_bit;				
	else
		REG_APB3_ICU_INT_ENABLE &= (~INT_STATUS_DAC_bit);
}

/**************************************************
FunctionName:DACINT_WAKENA
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
void DACINT_WAKENA(uint8_t cmd)
{
	if(cmd)
		REG_APB3_ICU_INT_WAKENA |= INT_STATUS_DAC_bit;			
	else
		REG_APB3_ICU_INT_WAKENA &= (~INT_STATUS_DAC_bit);
}




/***********************  END OF FILES  ***********************/
