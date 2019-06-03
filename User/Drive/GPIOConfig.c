#include "GPIOConfig.h"
#include "Includes.h"

/**************************************************
FunctionName:GPIOTestConfig
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
void GPIOTestConfig(void)
{
	GPIOConfig_T GIPOConfig;
	
	 /* GPIOF_5 */
	/* 请根据实际硬件配置,结构体值不建议出现缺省 */
	GIPOConfig.GPIO_PERI = GPIO_General_Function;  //普通功能
	GIPOConfig.GPIO_OE_N_INTERN = Disable;  //输出使能
	GIPOConfig.GPIO_I_IE = Enable;  //输入使能
	GIPOConfig.GPIO_PULLUP = Disable;  //上拉使能
	GIPOConfig.GPIO_PULLDOWN = Enable;  //下拉使能
	
	GIPOConfig.Int.isINT = Enable;    //配置中断
	GIPOConfig.Int.En_Dis_able = Enable;  //GPIO_Pin中断
	#if 0  //边沿触发，单沿触发 上升沿触发
		GIPOConfig.Int.INTMode = edge_Trigging; //边沿触发
		GIPOConfig.Int.dpulse = _SPULSE;  //单沿触发
		GIPOConfig.Int.INTtrigger = Rising_edge_Trigging;
	#endif 
	#if 0 //边沿触发，单沿触发  下升沿触发
		GIPOConfig.Int.INTMode = edge_Trigging; //边沿触发
		GIPOConfig.Int.dpulse = _SPULSE;  //单沿触发
		GIPOConfig.Int.INTtrigger = falling_edge_Tringging;
	#endif
	#if 1 //边沿触发， 双沿沿触发  
		GIPOConfig.Int.INTMode = edge_Trigging; //边沿触发
		GIPOConfig.Int.dpulse = _DPULSE;  //双沿触发
//		GIPOConfig.Int.INTtrigger = falling_edge_Tringging;
	#endif
	#if 0 //电平触发, 高电平触发
		GIPOConfig.Int.INTMode = level_Tringging;  //电平触发
		GIPOConfig.Int.INTtrigger = High_level_Tringging;
	#endif
	#if 0 //电平触发, 低电平触发
		GIPOConfig.Int.INTMode = level_Tringging;  //电平触发
		GIPOConfig.Int.INTtrigger = Low_level_Trigging;
	#endif
	GIPOConfig.GPIO_Pin = GPIO_Pin_5;  //选择GPIO_Pin
	
	GPIO_Config(GPIOF,&GIPOConfig);
	SetGPIOPin(GPIOF,GPIO_Pin_5);
	
	/* GPIO中断唤醒 */
	SetGPIOINT_WAKENA(Enable);
	/* 使能GPIO中断源 */
	SetGPIOIntCmd(Enable);
}
/**************************************************
FunctionName: LEDConfig
Describe: 指示灯配置
InputParameter:
OutputParameter:
Notice:  GPIOB1;GPIOB0
**************************************************/
void LEDConfig(void)
{
	GPIOConfig_T GIPOConfig;

	GIPOConfig.GPIO_PERI = GPIO_General_Function;  //普通功能
	GIPOConfig.GPIO_OE_N_INTERN = Enable;  //输出使能
	GIPOConfig.GPIO_PULLUP = Disable;//Enable;  //上拉使能
	GIPOConfig.GPIO_PULLDOWN = Disable;  //下拉使能
	GIPOConfig.GPIO_I_IE = Disable;//Enable;  //输入使能
	GIPOConfig.Int.isINT = Disable;    //配置中断
	GIPOConfig.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_0|GPIO_Pin_6;
	/* Config */
	GPIO_Config(GPIOB,&GIPOConfig);
	GPIO_Config(GPIOF,&GIPOConfig);
	/* Set High Level */
	SetGPIOPin(GPIOB,GPIO_Pin_1|GPIO_Pin_0|GPIO_Pin_6);
	ResetGPIOPin(GPIOF,GPIO_Pin_1|GPIO_Pin_0);
	
#ifdef BK5870
//	GPIOConfig_T GIPOConfig;
	
	GIPOConfig.GPIO_PERI = GPIO_General_Function;  //脝脮脥篓鹿娄脛脺
	GIPOConfig.GPIO_OE_N_INTERN = Enable;  //脢盲鲁枚脢鹿脛脺
	GIPOConfig.GPIO_PULLUP = Disable;//Enable;  //脡脧脌颅脢鹿脛脺
	GIPOConfig.GPIO_PULLDOWN = Disable;  //脧脗脌颅脢鹿脛脺
	GIPOConfig.GPIO_I_IE = Disable;//Enable;  //脢盲脠毛脢鹿脛脺
	GIPOConfig.Int.isINT = Disable;    //脜盲脰脙脰脨露脧
	GIPOConfig.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6;
	/* Config */
	GPIO_Config(GPIOE,&GIPOConfig);
	/* Set High Level */
	SetGPIOPin(GPIOE,GPIO_Pin_5|GPIO_Pin_6);
#endif
}
/**************************************************
FunctionName:
Describe:
InputParameter:
OutputParameter:
Notice: GPIOB1;GPIOB0
**************************************************/
void FlashLED(void)
{
	unsigned char i= 5;
	for(;i>0;i--)
	{
		delay_ms(20);
	#ifdef BK5870
		ResetGPIOPin(GPIOE,GPIO_Pin_6);
		SetGPIOPin(GPIOE,GPIO_Pin_5);
	#endif
		ResetGPIOPin(GPIOB,GPIO_Pin_1);
		SetGPIOPin(GPIOB,GPIO_Pin_0);
		delay_ms(20);
		ResetGPIOPin(GPIOB,GPIO_Pin_0);
		SetGPIOPin(GPIOB,GPIO_Pin_1);
	#ifdef BK5870
		SetGPIOPin(GPIOE,GPIO_Pin_6);
		ResetGPIOPin(GPIOE,GPIO_Pin_5);
	#endif
		delay_ms(20);
		SetGPIOPin(GPIOB,GPIO_Pin_1|GPIO_Pin_0);
	#ifdef BK5870
		SetGPIOPin(GPIOE,GPIO_Pin_5|GPIO_Pin_6);
	#endif
		delay_ms(40);
		ResetGPIOPin(GPIOB,GPIO_Pin_1|GPIO_Pin_0);
	#ifdef BK5870
		ResetGPIOPin(GPIOE,GPIO_Pin_5|GPIO_Pin_6);
	#endif
		delay_ms(40);
		SetGPIOPin(GPIOB,GPIO_Pin_1|GPIO_Pin_0);
	#ifdef BK5870
		SetGPIOPin(GPIOE,GPIO_Pin_5|GPIO_Pin_6);
	#endif
		delay_ms(40);
	#ifdef BK5870
		ResetGPIOPin(GPIOE,GPIO_Pin_5|GPIO_Pin_6);
	#endif
		ResetGPIOPin(GPIOB,GPIO_Pin_1|GPIO_Pin_0);
	}
}
/**************************************************
FunctionName:
Describe:
InputParameter:
OutputParameter:
Notice: GPIOB1;GPIOB0
**************************************************/
void LEDOpen(uint32_t ledx)
{
#ifdef BK5863N
	if((ledx&0x01) == 0x1)
		ResetGPIOPin(GPIOB,GPIO_Pin_0);
	if((ledx&0x02) == 0x02)
		ResetGPIOPin(GPIOB,GPIO_Pin_1);
#endif
#ifdef BK5870
	if((ledx&0x01) == 0x1)
		ResetGPIOPin(GPIOE,GPIO_Pin_5);
	if((ledx&0x02) == 0x02)
		ResetGPIOPin(GPIOE,GPIO_Pin_6);
#endif
}
/**************************************************
FunctionName:
Describe:
InputParameter:
OutputParameter:
Notice: GPIOB1;GPIOB0
**************************************************/
void LEDClose(uint32_t ledx)
{
#ifdef BK5863N
	if((ledx&0x01) == 0x1)
		SetGPIOPin(GPIOB,GPIO_Pin_0);
	if((ledx&0x02) == 0x02)
		SetGPIOPin(GPIOB,GPIO_Pin_1);
#endif
#ifdef BK5870
	if((ledx&0x01) == 0x1)
		SetGPIOPin(GPIOE,GPIO_Pin_5);
	if((ledx&0x02) == 0x02)
		SetGPIOPin(GPIOE,GPIO_Pin_6);
#endif
}


/***********************  END OF FILES  ***********************/
