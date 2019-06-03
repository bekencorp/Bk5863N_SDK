#include "TIM.h"
#include "BK5863.h"

/**************************************************
FunctionName:CmdTIMCLK
Describe: 开关TIME时钟源
InputParameter:
OutputParameter:
Notice:
**************************************************/
void CmdTIMCLK(unsigned char cmd)
{
	if(cmd)
		REG_APB3_ICU_PWMCLKCON &= (~0x01);
	else
		REG_APB3_ICU_PWMCLKCON |= 0x01;
}

/**************************************************
FunctionName:  ClockSelect_PT3_4
Describe:   
InputParameter:  ClockSelect_PT3_4_32KHZ 0
                 ClockSelect_PT3_4_32MHZ 1
OutputParameter:
Notice:
**************************************************/
void ClockSelect_PT3_4(unsigned char CLK)
{
	REG_APB10_PWM_CLK = (REG_APB10_PWM_CLK&(~(0x01<<8)))|((CLK&0x01)<<8);
}

/**************************************************
FunctionName: CleanTimInterruptFlag
Describe: 
InputParameter:
OutputParameter:
Notice:
**************************************************/
void CleanTimInterruptFlag(unsigned int TimIntFlag)
{
	REG_APB10_PWM_CFG |= ((TimIntFlag&0x1F)<<16);
}

/**************************************************
FunctionName:  PT_ENABLE
Describe: 
InputParameter:
OutputParameter:
Notice:
**************************************************/
void PT_ENABLE(unsigned char cmd)
{
	if(cmd)
	  REG_APB10_PWM_CFG |= (1<<7);
	else
		REG_APB10_PWM_CFG &= (~(1<<7));
}

/**************************************************
FunctionName:  PTx_MODE
Describe: 
InputParameter:
OutputParameter:
Notice:
**************************************************/
void PTx_MODE(uint8_t TIMx,uint8_t mode)
{
	if(TIMx > _TIM4) return;
	if(_TIMER_MODE == mode)
	{
		REG_APB10_PWM_CFG &=(~(1<<(PT0_MODE_BIT+TIMx)));
	}
	else if(_PWM_MODE == mode)
	{
		REG_APB10_PWM_CFG |= (1<<(PT0_MODE_BIT+TIMx));
	}
}

/**************************************************
FunctionName:  PTx_MID
Describe: 
InputParameter:
OutputParameter:
Notice:
**************************************************/
void PTx_MID(uint8_t TIMx,uint16_t midCout)
{
	volatile unsigned long *pReg;
	if(TIMx > _TIM4) return;
	pReg = ((volatile unsigned long *)(0x00a10000+(TIMx*4)));
	(*pReg) = ((*pReg)&0xFFFF0000)|midCout;
}

/**************************************************
FunctionName: PTx_START
Describe: 
InputParameter:
OutputParameter:
Notice:
**************************************************/
void PTx_START(uint8_t TIMx,uint16_t StartCout)
{
	volatile unsigned long *pReg;
	if(TIMx > _TIM4) return;
	pReg = ((volatile unsigned long *)(0x00a10000+(TIMx*4)));
	(*pReg) = ((*pReg)&0x0000FFFF)|(StartCout<<16);
}

/**************************************************
FunctionName:TimConfig
Describe: 
InputParameter:
OutputParameter:
Notice:
**************************************************/
char TimConfig(uint8_t TIMx,TIMPara_T *TimConf)
{
	if(TimConf->MODE == _TIMER_MODE)
	{
		(*((volatile unsigned long *)(0x00a10000+(TIMx*4)))) = (TimConf->STcounter<<16);
		REG_APB10_PWM_CFG |= (1<<PT_ENABLE_BIT); //bit7
		/* Config Clk */
		if((TIMx == _TIM3)||(TIMx == _TIM4))
		{
			REG_APB10_PWM_CLK = (REG_APB10_PWM_CLK&(~0x100))|((TimConf->PT3_4_SelClk&0x01)<<8);
//			REG_APB10_PWM_CLK = (REG_APB10_PWM_CLK&(~0xFF))|(TimConf->PT3_4_CLK32M_DIV&0xFF);
			REG_APB10_PWM_CLK = (REG_APB10_PWM_CLK&(~0xFF))|(TimConf->CLK_DIV&0xFF);
		}
		else
		{
			REG_APB10_PWM_CFG = (REG_APB10_PWM_CFG&(~0x0F00))|((TimConf->CLK_DIV&0x0F)<<8);
		}
		REG_APB10_PWM_CFG &=(~(1<<(PT0_MODE_BIT+TIMx)));
		
		/* Int */
		if(TimConf->Int_En)
			REG_APB3_ICU_INT_ENABLE |= 1<<(14+TIMx);
		else
			REG_APB3_ICU_INT_ENABLE &= (~(1<<(14+TIMx)));
		/* Wake */
		if(TimConf->IntWake_En)
		  REG_APB3_ICU_INT_WAKENA |= 1<<(14+TIMx);
		else
			REG_APB3_ICU_INT_WAKENA &= (~(1<<(14+TIMx)));
	}
	else if(TimConf->MODE == _PWM_MODE)
	{
		(*((volatile unsigned long *)(0x00a10000+(TIMx*4)))) = (TimConf->STcounter<<16);
		(*((volatile unsigned long *)(0x00a10000+(TIMx*4)))) |= TimConf->MIDcounter;
	  REG_APB10_PWM_CFG |= (1<<PT_ENABLE_BIT);	
			/* Config Clk */
		if((TIMx == _TIM3)||(TIMx == _TIM4))
		{
			REG_APG3_ICU_DEEPSLEEP &= ~(1<<3);  //关闭IO复用 
			REG_APB10_PWM_CLK = (REG_APB10_PWM_CLK&(~0x100))|((TimConf->PT3_4_SelClk&0x01)<<8);
//			REG_APB10_PWM_CLK = (REG_APB10_PWM_CLK&(~0xFF))|(TimConf->PT3_4_CLK32M_DIV&0xFF);
			REG_APB10_PWM_CLK = (REG_APB10_PWM_CLK&(~0xFF))|(TimConf->CLK_DIV&0xFF);
		}
		else
		{
			REG_APB10_PWM_CFG = (REG_APB10_PWM_CFG&(~0x0F00))|((TimConf->CLK_DIV&0x0F)<<8);
		}
		REG_APB10_PWM_CFG |= (1<<(PT0_MODE_BIT+TIMx));
	}
	else
	{
		return 0;
	}
	return 1;
}

/**************************************************
FunctionName: PTxDefault
Describe:  
InputParameter:
OutputParameter:
Notice:  与其它复用的寄存器不回复默认值
**************************************************/
void PTxDefault(uint8_t TIMx)
{
	if(TIMx >_TIM4) return;
	(*((volatile unsigned long *)(0x00a10000+(TIMx*4)))) = 0;
	REG_APB10_PWM_CFG &= (~(1<<(16+TIMx)));
	REG_APB10_PWM_CFG &= (~(1<<(0+TIMx)));
	REG_APB3_ICU_INT_ENABLE &= (~(1<<(14+TIMx)));
	REG_APB3_ICU_INT_WAKENA &= (~(1<<(14+TIMx)));
	if((TIMx == _TIM3)||(TIMx == _TIM4))
	{
		REG_APG3_ICU_DEEPSLEEP |= (1<<3); 
	}
}

/**************************************************
FunctionName: PTxCountResult
Describe: PTx Count Result
InputParameter:  _TIM4 = 4,
								 _TIM3 = 3,
								 _TIM2 = 2,
								 _TIM1 = 1,
								 _TIM0 = 0
OutputParameter: OK:   PTx Count Result Value(0~0xFFFF)
                 error:0xFFFFFFFF
Notice:
**************************************************/
unsigned int PTxCountResult(uint8_t TIMx)
{
	if(TIMx<_TIM4)
		return ((*((volatile unsigned long *)(0x00a10020+(TIMx*4))))&0xFFFF);
	else
		return 0xFFFFFFFF;
}

/**************************************************
FunctionName: CleanRTCInterruptFlag
Describe: 
InputParameter:
OutputParameter:
Notice:
**************************************************/
void CleanRTCInterruptFlag(unsigned int RTCIntFlag)
{
	REG_RTC_CFG |= ((RTCIntFlag&0x1)<<8);
}
/**************************************************
FunctionName: ReadRTCInterruptFlag
Describe: 
InputParameter:
OutputParameter:
Notice:
**************************************************/
uint8_t ReadRTCInterruptFlag(void)
{
	return ((REG_RTC_CFG>>8)&0x01);
}
/**************************************************
FunctionName: SetRTC_CNT_VALUE
Describe: 
InputParameter:
OutputParameter:
Notice:
**************************************************/
void SetRTC_CNT_VALUE(unsigned char Value)
{
	REG_RTC_CFG = (REG_RTC_CFG&(~0x7E))|((Value&0x3F)<<1);
}

/**************************************************
FunctionName: ReadRTC_CNT_VALUE
Describe: 
InputParameter:
OutputParameter:
Notice:
**************************************************/
unsigned char ReadRTC_CNT_VALUE(void)
{
	return ((REG_RTC_CFG&0x7E)>>1);
}

/**************************************************
FunctionName: SetRTC_EN
Describe: 
InputParameter:
OutputParameter:
Notice:
**************************************************/
void SetRTC_EN(unsigned char Cmd)
{
	if(Cmd)
	  REG_RTC_CFG |= 0x01;
	else
	  REG_RTC_CFG &= 0xFFFFFFFE;
}
/**************************************************
FunctionName: ReadRTC_EN
Describe: 
InputParameter:
OutputParameter:
Notice:
**************************************************/
unsigned char ReadRTC_EN(void)
{
	return (REG_RTC_CFG&0x01);
}
/***********************  END OF FILES  ***********************/
