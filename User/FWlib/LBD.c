#include "LBD.h"
#include "BK5863.h"

/**************************************************
FunctionName:LBDVoltageCompartorResult
Describe: ��ȡLBD Voltage Compartor Output 
          ��ı䣬������һ�ε�ֵ
InputParameter:
OutputParameter:
Notice:
**************************************************/
unsigned char ReadLBDVoltageCompartorResult(void)
{
	return ((REG_APB11_LBD_LBD_INT_FLAG&0x04)>>2);
}

/**************************************************
FunctionName:ReadHighVoltageInterruptFlag
Describe: ��ȡ�ߵ�ѹ�жϱ�־λ
InputParameter:
OutputParameter:
Notice:
**************************************************/
unsigned char ReadHighVoltageInterruptFlag(void)
{
	return ((REG_APB11_LBD_LBD_INT_FLAG&0x02)>>1);
}
/**************************************************
FunctionName:SetHighVoltageInterrupt
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
void SetHighVoltageInterrupt(uint8_t cmd)
{
	if(cmd)
		REG_APB3_ICU_FIQ_ENABLE |= (1<<1);
	else
		REG_APB3_ICU_FIQ_ENABLE &= (~(1<<1));
}

/**************************************************
FunctionName:SetLowVoltageInterrupt
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
void SetLowVoltageInterrupt(uint8_t cmd)
{
	if(cmd)
		REG_APB3_ICU_FIQ_ENABLE |= (1<<0);
	else
		REG_APB3_ICU_FIQ_ENABLE &= (~(1<<0));
}
/**************************************************
FunctionName:ReadLowVoltageInterruptFlag
Describe: ��ȡ�͵�ѹ�жϱ�־λ
InputParameter:
OutputParameter:
Notice:
**************************************************/
unsigned char ReadLowVoltageInterruptFlag(void)
{
	return (REG_APB11_LBD_LBD_INT_FLAG&0x01);
}

/**************************************************
FunctionName:ReadVoltageInterruptFlag
Describe: ��ȡ��ѹ�жϱ�־λ
InputParameter:
OutputParameter:
Notice:
**************************************************/
unsigned char ReadVoltageInterruptFlag(void)
{
	return (REG_APB11_LBD_LBD_INT_FLAG&0x03);
}

/**************************************************
FunctionName:CleanVoltageInterruptFlag
Describe: ����жϱ�־λ
          Wirte 1 to clear
InputParameter:cleanflag:��Ҫ������жϱ�־λ
OutputParameter:
Notice:
**************************************************/
void CleanVoltageInterruptFlag(unsigned char cleanflag)
{
	REG_APB11_LBD_LBD_INT_FLAG |= (cleanflag&0x03);
}

/**************************************************
FunctionName:SwitchCLK32KHz
Describe: ����32KHzʱ�ӣ�
          ����ر�
          Ӳ����
InputParameter:
OutputParameter:
Notice:
**************************************************/
void SwitchCLK32KHz(uint8_t cmd)
{
	if(cmd)
		REG_APB11_LBD_CLK32K_PWD &= 0xFFFFFFFE;
	else
		REG_APB11_LBD_CLK32K_PWD |= 0x01;
}



/***********************  END OF FILES  ***********************/
