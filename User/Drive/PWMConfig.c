#include "PWMConfig.h"
#include "Includes.h"
void system_timer_task(void);
/********************************************************
Function: BK5863 PWM Initial
Parameter: 
          index: PWM number 
					PT0_MODE_BIT: pwm0
					PT1_MODE_BIT: pwm1
					PT2_MODE_BIT: pwm2
					PT3_MODE_BIT: pwm3
					PT4_MODE_BIT: pwm4
          type: set PWM or TIMER
				BK_TIMER_MODE: timer
				BK_PWM_MODE: PWM
			counter: 
********************************************************/
void BK5863PWMInit(unsigned int index, unsigned char type, unsigned int counter)
{
	unsigned int  bit=0;
	
	if(type == BK_TIMER_MODE)
		bit = 16;
	
	switch(index)
	{
		case PT0_MODE_BIT:
			REG_APB10_PWM_0 =  (counter<<bit)+(counter/2);
			break;
		case PT1_MODE_BIT:
			REG_APB10_PWM_1 =  (counter<<bit)+(counter/2);
			break;
		case PT2_MODE_BIT:
			REG_APB10_PWM_2 =  (counter<<bit)+(counter/2);
			break;
		case PT3_MODE_BIT:
			REG_APB10_PWM_3 =  (counter<<bit)+(counter/2);
			break;
		case PT4_MODE_BIT:
			REG_APB10_PWM_4 =  (counter<<bit)+(counter/2);
			break;
	}

	if(type == BK_TIMER_MODE)
	{
		REG_APB10_PWM_CFG |= (1<<PT_ENABLE_BIT)&(~(1<<(PT0_MODE_BIT+index)));		// 1 div
		REG_APB3_ICU_INT_ENABLE |= 1<<(14+index);
		REG_APB3_ICU_INT_WAKENA |= 1<<(14+index);
	}
	else
	{
	  REG_APG3_ICU_DEEPSLEEP &= ~(1<<3);
		REG_APB10_PWM_CFG &= ~(1<<PT_ENABLE_BIT);	// 1 div
		REG_APB10_PWM_CFG |= 1<<(PT0_MODE_BIT+index);
		REG_APB7_GPIOB_CFG &= ~(1<<index);	//GPIO pwm0-4 2nd func enable
	}
	
	REG_APB3_ICU_PWMCLKCON = 0;					//PWM clock enable
}

/**************************************************
FunctionName:PWMOUTTestConfig
Describe:  PWM输出
InputParameter:
OutputParameter:
Notice:
**************************************************/
void PWMOUTTestConfig(void)
{
	TIMPara_T timConf;
	uint8_t TimerN = _TIM3;
	
	timConf.MODE = _PWM_MODE; //BK_TIMER_MODE BK_PWM_MODE
	timConf.CLK_DIV = 4-1;;  //时钟分频
	timConf.PT3_4_SelClk = ClockSelect_PT3_4_32MHZ; //Select Clock
	timConf.STcounter = 100-1;  //结束值
	timConf.MIDcounter = 50-1;  //中间值
	timConf.Int_En = Disable;  //使能中断
	timConf.IntWake_En = Disable;  //使能唤醒
	
	TimConfig(TimerN,&timConf);	
	REG_APB7_GPIOB_CFG &= ~(1<<TimerN);	//GPIO pwm0-4 2nd func enable
	
	TimerN = _TIM4;
	timConf.STcounter = 200-1;  //结束值
	timConf.MIDcounter = 10-1;  //中间值
	TimConfig(TimerN,&timConf);	
	REG_APB7_GPIOB_CFG &= ~(1<<TimerN);	//GPIO pwm0-4 2nd func enable
	
	REG_APB3_ICU_PWMCLKCON = 0;					//PWM clock enable
}
#ifdef _TIM3_TEST_
/**************************************************
FunctionName:Timer3TestConfig
Describe: 定时器3配置1ms
InputParameter:
OutputParameter:
Notice:
**************************************************/
void Timer3TestConfig(void)
{
	TIMPara_T timConf;
	uint8_t TimerN = _TIM3;
	
	timConf.MODE = _TIMER_MODE; //BK_TIMER_MODE BK_PWM_MODE
	timConf.CLK_DIV = 4-1;;  //时钟分频
	timConf.PT3_4_SelClk = ClockSelect_PT3_4_32MHZ; //Select Clock
	timConf.STcounter = 8192-1;  //结束值
	timConf.MIDcounter = 100-1;  //中间值
	timConf.Int_En = Enable;  //使能中断
	timConf.IntWake_En = Disable;  //使能唤醒
	
	TimConfig(TimerN,&timConf);	
	CmdTIMCLK(Enable);//PWM clock enable
//	REG_APB3_ICU_PWMCLKCON = 0;					
}
/**************************************************
FunctionName:Timer3TestConfig
Describe: 定时器3服务程序
InputParameter:
OutputParameter:
Notice:
**************************************************/
void Tim3_IRQHandler(void)
{
	REG_APB10_PWM_CFG |= PT3_INT_FLAG;

	system_timer_task();
}
#endif
/***********************  END OF FILES  ***********************/
