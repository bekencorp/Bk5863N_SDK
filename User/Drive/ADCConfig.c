#include "ADCConfig.h"
#include "Includes.h"

BKADC_CONTROL_STRU BKADC_CTL_BLK;
//********************************************************//
//Function: BK5863 ADC Initial
//Parameter: 
//          pBuf: the pointer of output buffer
//			bufLen: the size of output buffer
//********************************************************//
void BK5863ADCInit(unsigned int* pBuf, unsigned int bufLen)
{
	REG_APB3_ICU_ADCCLKCON  = 0;							//ADC clock enable
	REG_APB7_GPIOA_CFG &= 0xffffffc0;						//ADC_DIN[0-5] 2nd enable
	BKADC_CTL_BLK.adc_index = 0;
	BKADC_CTL_BLK.pBuf = pBuf;
	BKADC_CTL_BLK.bufLen = bufLen;
	REG_APB3_ICU_INT_ENABLE |= INT_STATUS_ADC_bit;			//interrupt enable
	REG_APB3_ICU_INT_WAKENA |= INT_STATUS_ADC_bit;
}

/********************************************************
Function: BK5863 ADC Configuration and Exampling
Parameter: 
          pConfig:  the pointer of BKADCCONFIG_STRU structure

          bitmap: the bitmap of configure type 
				BK_ADC_MODE: set ADC mode
			  | BK_ADC_MODE: set channel
			  | BK_ADC_SAMPLE_RATE: set sample rate
			  | BK_ADC_SETTLING: settling mode
            | BK_ADC_CLK_RATE: set clock rate
********************************************************/
void BK5863ADCConfig(BKADCCONFIG_STRU* pConfig, unsigned int bitmap)
{
	unsigned int value = REG_APB1_ADC_CFG;
	
	if(bitmap & BK_ADC_MODE)
	{
		value = (value & (~0x3))|pConfig->mode;
	}

	if(bitmap & BK_ADC_CHNL)
	{
		value = (value & (~0x3c))|(pConfig->channel<<ADC_CHANNEL_SEL_BIT);
	}

	if(bitmap & BK_ADC_SAMPLE_RATE)
	{
		value = (value & (~0x300))|(pConfig->sample_rate<<ADC_SAMPLE_RATE_BIT);
	}

	if(bitmap & BK_ADC_SETTLING)
	{
		value = (value & (~0x400))|(pConfig->settling<<ADC_SETTLING_BIT);
	}
	
	if(bitmap & BK_ADC_CLK_RATE)
	{
		value = (value & (~0x70000))|(pConfig->clk_rate<<ADC_CLK_RATE_BIT);
	}

	REG_APB1_ADC_CFG = value;
	BKADC_CTL_BLK.adc_index = 0;

	while((BKADC_CTL_BLK.adc_index != BKADC_CTL_BLK.bufLen) && (REG_APB1_ADC_CFG & 0x03))
	{
		delay_us(5);
	}
}






