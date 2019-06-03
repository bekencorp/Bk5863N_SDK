#ifndef _ADC_H_
#define _ADC_H_



#define  adc_clk_36 0
#define  adc_clk_18 1

enum{
	ADCPowerDown = 0,
	ADCSigleStep = 1,
	ADCSoftware = 2,
	ADCContinuous = 3
};
enum{
	ADC_CLK_8 = 1,
	ADC_CLK_4 = 0
};
enum{
	ADCDiv4 = 0,
	ADCDiv8 = 1,
	ADCDiv16 = 2,
	ADCDiv32 = 3
};
typedef struct{
	unsigned char ADC_MODE;
	unsigned char ch;
	unsigned char ADC_SAMPLE_RATE;
	unsigned char setting;
	unsigned char ADC_CLK_RATE;
	unsigned char ADCClock;
	
	unsigned char ADCInt;
	unsigned char ADCWAKENA;
}ADCParaConfig_T;


void BK5863VoltDetect(void);

void ADCConfig(ADCParaConfig_T para);
void SelectADC_CHNL(unsigned int ch);
unsigned short ReadADCValue(void);
void ADC_CLK_RATE(unsigned char div);
void SelectADC_MODE(unsigned char mode);
char ReadADC_BUSY(void);
void ADCclock(unsigned char cmd);
void ADCintCmd(unsigned char cmd);
void ADCWAKENACmd(unsigned char cmd);


#endif





/***********************  END OF FILES  ***********************/
