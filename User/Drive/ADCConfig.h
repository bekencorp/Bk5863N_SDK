#ifndef _ADCConfig_H_
#define _ADCConfig_H_


#include "BK5863.h"

//BK5863ADCAPI
typedef enum {
	BK_ADC_MODE = 0x01,
	BK_ADC_CHNL = 0x02,
	BK_ADC_SAMPLE_RATE = 0x04,
	BK_ADC_SETTLING = 0x08,
	BK_ADC_CLK_RATE = 0x10
}BKADC_CONFIG_ENUM;

typedef struct BKADCCONFIG_STRU_s
{
	unsigned char	mode;			//0x00:pwrdown, 0x01:step, 0x02:software, 0x03:continue
	unsigned char	channel;		//channel selection
	unsigned char	sample_rate;	//0x00:adc_clk/36, 0x01:adc_clk/18
	unsigned char	settling;		//0x01:8 ADC_CLK, 0x00:4 ADC_CLK
	unsigned char	clk_rate;		//0x00: 4div, 0x01:8div, 0x02:16div, 0x03:32div
}BKADCCONFIG_STRU;

typedef struct BKADC_CONTROL_STRU_internal
{
	unsigned int	adc_index;
	unsigned int	*pBuf;
	unsigned int 	bufLen;
}BKADC_CONTROL_STRU;

void BK5863ADCInit(unsigned int* pBuf, unsigned int bufLen);
void BK5863ADCConfig(BKADCCONFIG_STRU* pConfig, unsigned int bitmap);




extern BKADC_CONTROL_STRU BKADC_CTL_BLK;

#endif /*_ADCConfig_H_*/
