#include "ADC.h"
#include "BK5863.h"
//********************************************************//
//Function: BK5863 Voltage Detect
//********************************************************//
void BK5863VoltDetect(void)
{
    REG_APB11_LBD_LBD_INT_FLAG  = 0x3;    //clear int status
    REG_APB3_ICU_FIQ_CLEAR      = 0x3;  
    REG_APB3_ICU_FIQ_ENABLE     = 0x3;   //open voltage detect int enable
    REG_APB3_ICU_FIQ_WAKENA     = 0x3;   //open voltage detect int wakeup enable
    REG_APB3_ICU_IRQTYPE_EN    |= 0x3;   //irq & fiq enable
    REG_APB3_ICU_ANA1_CTRL     |= 0x00100000; //open spibmen
}


/**************************************************
FunctionName:
Describe:ADC ����
InputParameter:
OutputParameter:
Notice:
**************************************************/
void ADCConfig(ADCParaConfig_T para)
{
	unsigned int Temp = REG_APB1_ADC_CFG;
	Temp &= ~(0x3+(0xF<<2)+(0x03<<8)+(0x01<<10)+(0x03<<16));
	Temp |= para.ADC_MODE&0x3;
	Temp |= ((para.ch&0x0F)<<ADC_CHANNEL_SEL_BIT);
	Temp |= ((para.ADC_SAMPLE_RATE&0x03)<<8);
	Temp |= ((para.setting&0x01)<<10);
	Temp |= ((para.ADC_CLK_RATE&0x03)<<16);
	
	REG_APB1_ADC_CFG = Temp;
	
	if(para.ADCInt)
		REG_APB3_ICU_INT_ENABLE |= INT_STATUS_ADC_bit;			//interrupt enable
	else
		REG_APB3_ICU_INT_ENABLE &= (~INT_STATUS_ADC_bit);			
	
	if(para.ADCWAKENA)
		REG_APB3_ICU_INT_WAKENA |= INT_STATUS_ADC_bit;
	else
		REG_APB3_ICU_INT_WAKENA &= (~INT_STATUS_ADC_bit);
	
	if(para.ADCClock)
		REG_APB3_ICU_ADCCLKCON  = 0;							//ADC clock enable
	else
		REG_APB3_ICU_ADCCLKCON  = 1;						
}
/*
 ѡ��ADCͨ��
*/
void SelectADC_CHNL(unsigned int ch)
{
	unsigned int Temp = REG_APB1_ADC_CFG;
	Temp = (Temp & (~0x3c))|(ch<<ADC_CHANNEL_SEL_BIT);
	REG_APB1_ADC_CFG = Temp;
}

/*
 ��ȡADC����
*/
unsigned short ReadADCValue(void)
{
	unsigned int value;
	value = (REG_APB1_ADC_DAT&0x3FF);
	return value;
}

/*
 ADC��Ƶ
*/
void ADC_CLK_RATE(unsigned char div)
{
	unsigned int Temp = REG_APB1_ADC_CFG;
	Temp &= (~(0x03<<16));
	Temp |= ((div&0x03)<<16);
	REG_APB1_ADC_CFG = Temp;
}

/*
ѡ��ADCģʽ
*/
void SelectADC_MODE(unsigned char mode)
{
	unsigned int Temp = REG_APB1_ADC_CFG;
	ADCclock(Disable);
	Temp &= (~0x03);
	Temp |= (mode&0x03);
	REG_APB1_ADC_CFG = Temp;
	ADCclock(Enable);
}

/*
ADC ��æ��־
*/
char ReadADC_BUSY(void)
{
	if(REG_APB1_ADC_CFG&(0x01<<7))
	  return 1; //����ת������,
	else
		return 0; //ת����ɣ����ݶ�����ת��Ϊ1
}

/*
��ADCʱ��
*/

void ADCclock(unsigned char cmd)
{
	if(cmd)
		REG_APB3_ICU_ADCCLKCON  &= ~0x01;							//ADC clock enable
	else
		REG_APB3_ICU_ADCCLKCON  |= 1;						
}

/*
 ADC �ж�ʹ��
*/
void ADCintCmd(unsigned char cmd)
{
	if(cmd)
		REG_APB3_ICU_INT_ENABLE |= INT_STATUS_ADC_bit;			//interrupt enable
	else
		REG_APB3_ICU_INT_ENABLE &= (~INT_STATUS_ADC_bit);		
}

/*
ADC �жϻ���
*/
void ADCWAKENACmd(unsigned char cmd)
{
	if(cmd)
		REG_APB3_ICU_INT_WAKENA |= INT_STATUS_ADC_bit;
	else
		REG_APB3_ICU_INT_WAKENA &= (~INT_STATUS_ADC_bit);
}










/***********************  END OF FILES  ***********************/
