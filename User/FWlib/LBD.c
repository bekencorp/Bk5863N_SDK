#include "LBD.h"
#include "BK5863.h"

/**************************************************
FunctionName:LBDVoltageCompartorResult
Describe: 读取LBD Voltage Compartor Output 
          如改变，保留上一次的值
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
Describe: 读取高电压中断标志位
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
Describe: 读取低电压中断标志位
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
Describe: 读取电压中断标志位
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
Describe: 清除中断标志位
          Wirte 1 to clear
InputParameter:cleanflag:需要清除的中断标志位
OutputParameter:
Notice:
**************************************************/
void CleanVoltageInterruptFlag(unsigned char cleanflag)
{
	REG_APB11_LBD_LBD_INT_FLAG |= (cleanflag&0x03);
}

/**************************************************
FunctionName:SwitchCLK32KHz
Describe: 开关32KHz时钟，
          软件关闭
          硬件打开
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
