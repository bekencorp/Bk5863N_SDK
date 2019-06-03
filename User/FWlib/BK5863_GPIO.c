#include "BK5863_GPIO.h"
#include "BK5863.h"

/*******************************************************************
*函数名称:GPIO_Config
*函数描述:配置GPIO
*作    者:
*输入参数:
*输出参数:无
*返回说明:无
*其它说明:无
*******************************************************************/
void GPIO_Config(uint8_t GPIOx,GPIOConfig_T *GPIOConfig)
{
	uint32_t Temp = 0;
	if(!(GPIOx <= GPIOF)) return;	
	Temp = REG_APB7_GPIO_CFG(GPIOx);
	if(GPIOConfig->GPIO_PERI != GPIO_General_Function)
		Temp &= ~GPIOConfig->GPIO_Pin;
	else
		Temp |= GPIOConfig->GPIO_Pin;
	if(GPIOConfig->GPIO_OE_N_INTERN)
		Temp &= ~(GPIOConfig->GPIO_Pin<<8);
	else
		Temp |= (GPIOConfig->GPIO_Pin<<8);
	if(GPIOConfig->GPIO_PULLUP)
		Temp |= (GPIOConfig->GPIO_Pin<<16);
	else
		Temp &= ~(GPIOConfig->GPIO_Pin<<16);
	if(GPIOConfig->GPIO_PULLDOWN)
		Temp |= (GPIOConfig->GPIO_Pin<<24);
	else
		Temp &= ~(GPIOConfig->GPIO_Pin<<24);
  REG_APB7_GPIO_CFG(GPIOx) = Temp;
	
  if(GPIOConfig->GPIO_I_IE)
	  REG_APB7_GPIO_DATA(GPIOx) |= (GPIOConfig->GPIO_Pin<<16);
	else
	  REG_APB7_GPIO_DATA(GPIOx) &= ~(GPIOConfig->GPIO_Pin<<16);
	
	if(GPIOConfig->Int.isINT) 
	{
		SetGPIO_INTConfig(GPIOx,GPIOConfig);
	}
}

/*******************************************************************
*函数名称:SetGPIOIntCmd
*函数描述:使能或关闭GPIO总中断源
*作    者:
*输入参数:
*输出参数:无
*返回说明:无
*其它说明:无
*******************************************************************/
void SetGPIOIntCmd(int cmd)
{
	if(cmd)
		REG_APB3_ICU_INT_ENABLE |= (1<<5);  //使能GPIO中断源
	else
		REG_APB3_ICU_INT_ENABLE &= ~(1<<5);  //使能GPIO中断源	
}

/*******************************************************************
		Record Trig mode & Pulse & Dpulse 
*******************************************************************/
uint32_t GPIO_INT_TRIG_MODE[2] = {0x00,0x00};
uint32_t GPIO_INT_PULSE[2] = {0x00,0x00};
uint32_t GPIO_INT_TRIG_dpulse[2] = {0x00,0x00};
/*******************************************************************
*函数名称: SetGPIO_INTConfig
*函数描述: 配置GPIO中断方式
*作    者:
*输入参数:GPIOx GPIOConfig
*输出参数:无
*返回说明:无
*其它说明:请注意部分寄存器为只读寄存器
*******************************************************************/
void SetGPIO_INTConfig(uint8_t GPIOx,GPIOConfig_T *GPIOConfig)
{
	if(GPIOx <= GPIOD)
	{
		if(GPIOConfig->Int.INTMode)  //边沿触发
		{	
			GPIO_INT_TRIG_MODE[0] |=  ((GPIOConfig->GPIO_Pin)<<(8*GPIOx));
			REG_APB7_GPIO_INT_TRIG_MODE = GPIO_INT_TRIG_MODE[0];
			if(GPIOConfig->Int.INTtrigger)   //GPIOx_pinx 0: 上升沿  1:下降沿
			  GPIO_INT_PULSE[0] |= (GPIOConfig->GPIO_Pin<<(8*GPIOx));  
		  else
				GPIO_INT_PULSE[0] &= ~(GPIOConfig->GPIO_Pin<<(8*GPIOx));
       REG_APB7_GPIO_INT_PULSE = GPIO_INT_PULSE[0];			
			if(GPIOConfig->Int.dpulse)   //双沿触发
			  GPIO_INT_TRIG_dpulse[0] |= (GPIOConfig->GPIO_Pin<<(8*GPIOx));  
		  else //单沿触发
				GPIO_INT_TRIG_dpulse[0] &= ~(GPIOConfig->GPIO_Pin<<(8*GPIOx)); 
			REG_APB7_GPIO_INT_TRIG_dpulse = GPIO_INT_TRIG_dpulse[0];
		}
		else
		{
			GPIO_INT_TRIG_MODE[0] &= ~((GPIOConfig->GPIO_Pin)<<(8*GPIOx));
			REG_APB7_GPIO_INT_TRIG_MODE = GPIO_INT_TRIG_MODE[0];
			if(GPIOConfig->Int.INTtrigger)   //GPIOx_Pinx: 高电平  0:低电平
			  REG_APB7_GPIO_INT_LEVEL |= (GPIOConfig->GPIO_Pin<<(8*GPIOx)); 
		  else
				REG_APB7_GPIO_INT_LEVEL &= ~(GPIOConfig->GPIO_Pin<<(8*GPIOx)); 
		}
		/* 中断开关 */
		if(GPIOConfig->Int.En_Dis_able)
		  REG_APB7_GPIO_INT_ENABLE |= (GPIOConfig->GPIO_Pin<<(8*GPIOx));  //Enable
		else
			REG_APB7_GPIO_INT_ENABLE &= ~(GPIOConfig->GPIO_Pin<<(8*GPIOx));  //disable
	}
	else if((GPIOx == GPIOF) || (GPIOx == GPIOE))
	{
		GPIOx -= GPIOE;
		if(GPIOConfig->Int.INTMode)  //边缘触发
		{	
			GPIO_INT_TRIG_MODE[1] |= ((GPIOConfig->GPIO_Pin)<<(8*GPIOx));
			REG_APB7_GPIO_INT_TRIG_MODE2 = GPIO_INT_TRIG_MODE[1];
			if(GPIOConfig->Int.INTtrigger)   //GPIOx_pinx 1: 上升沿  0:下降沿
			  GPIO_INT_PULSE[1] |= (GPIOConfig->GPIO_Pin<<(8*GPIOx));  
		  else
				GPIO_INT_PULSE[1] &= ~(GPIOConfig->GPIO_Pin<<(8*GPIOx)); 
			REG_APB7_GPIO_INT_PULSE2 = GPIO_INT_PULSE[1];
			if(GPIOConfig->Int.dpulse)   //双沿触发
			  GPIO_INT_TRIG_dpulse[1] |= (GPIOConfig->GPIO_Pin<<(8*GPIOx));  
		  else //单沿触发
				GPIO_INT_TRIG_dpulse[1] &= ~(GPIOConfig->GPIO_Pin<<(8*GPIOx));
			REG_APB7_GPIO_INT_TRIG_dpulse2 = GPIO_INT_TRIG_dpulse[1];
		}
		else
		{
			GPIO_INT_TRIG_MODE[1] &= ~((GPIOConfig->GPIO_Pin)<<(8*GPIOx));
			REG_APB7_GPIO_INT_TRIG_MODE2 = GPIO_INT_TRIG_MODE[1];
			if(GPIOConfig->Int.INTtrigger)   //GPIOx_Pinx: 高电平  0:低电平
			  REG_APB7_GPIO_INT2_LEVEL |= (GPIOConfig->GPIO_Pin<<(8*GPIOx)); 
		  else
				REG_APB7_GPIO_INT2_LEVEL &= ~(GPIOConfig->GPIO_Pin<<(8*GPIOx)); 
		}
		if(GPIOConfig->Int.En_Dis_able)
		  REG_APB7_GPIO_INT2_ENABLE |= (GPIOConfig->GPIO_Pin<<(8*GPIOx));  //Enable
		else
			REG_APB7_GPIO_INT2_ENABLE &= ~(GPIOConfig->GPIO_Pin<<(8*GPIOx));  //disable
	}
}

/*******************************************************************
*函数名称:SetGPIOIRQ
*函数描述:使能或关闭某个GPIO中断源
*作    者:
*输入参数:
*输出参数:无
*返回说明:无
*其它说明:无
*******************************************************************/
void SetGPIOIRQ(uint8_t GPIOx,uint8_t GPIO_Pin,uint8_t cmd)
{
	if(GPIOx <= GPIOD)
	{
	  if(cmd)
		  REG_APB7_GPIO_INT_ENABLE |= (GPIO_Pin<<(8*GPIOx));  //Enable
		else
			REG_APB7_GPIO_INT_ENABLE &= ~(GPIO_Pin<<(8*GPIOx));  //disable
	}
	else if((GPIOx == GPIOF) || (GPIOx == GPIOE))
	{
		GPIOx -= GPIOE;
		if(cmd)
		  REG_APB7_GPIO_INT2_ENABLE |= (GPIO_Pin<<(8*GPIOx));  //Enable
		else
			REG_APB7_GPIO_INT2_ENABLE &= ~(GPIO_Pin<<(8*GPIOx));  //disable
	}
}
/*******************************************************************
*函数名称:SetGPIOINTMode
*函数描述:设置中断方式，电平或者边沿触发
*作    者:
*输入参数:
*输出参数:无
*返回说明:无
*其它说明:无
*******************************************************************/
void SetGPIOINTMode(uint8_t GPIOx,uint8_t GPIO_Pin,uint8_t mode)
{
	if(mode) //边沿触发
	{
		if(GPIOx < GPIOE)
		{
			GPIO_INT_TRIG_MODE[0] |= GPIO_Pin<<(8*GPIOx);
			REG_APB7_GPIO_INT_TRIG_MODE = GPIO_INT_TRIG_MODE[0];
//	  	REG_APB7_GPIO_INT_TRIG_MODE |= GPIO_Pin<<(8*GPIOx);
		}
  	else if(GPIOx <= GPIOF)
		{
			GPIO_INT_TRIG_MODE[1] |= GPIO_Pin<<(8*(GPIOx-GPIOE));
			REG_APB7_GPIO_INT_TRIG_MODE2 = GPIO_INT_TRIG_MODE[1];
//	  	REG_APB7_GPIO_INT_TRIG_MODE2 |= GPIO_Pin<<(8*(GPIOx-GPIOE));
		}
  }
	else  //电平除服
	{
		if(GPIOx < GPIOE)
		{
			GPIO_INT_TRIG_MODE[0] &= ~(GPIO_Pin<<(8*GPIOx));
			REG_APB7_GPIO_INT_TRIG_MODE = GPIO_INT_TRIG_MODE[0];
//	  	REG_APB7_GPIO_INT_TRIG_MODE &= ~(GPIO_Pin<<(8*GPIOx));
		}
  	else if(GPIOx <= GPIOF)
		{
			GPIO_INT_TRIG_MODE[1] &= ~(GPIO_Pin<<(8*(GPIOx-GPIOE)));
			REG_APB7_GPIO_INT_TRIG_MODE2 = GPIO_INT_TRIG_MODE[1];
//	  	REG_APB7_GPIO_INT_TRIG_MODE2 &= ~(GPIO_Pin<<(8*(GPIOx-GPIOE)));
		}
	}
}

/*******************************************************************
*函数名称:SetGPIOINTPulseMode
*函数描述:单沿触发，还是双沿触发
*作    者:
*输入参数:
*输出参数:无
*返回说明:无
*其它说明:无
*******************************************************************/
void SetGPIOINTPulseMode(uint8_t GPIOx,uint8_t GPIO_Pin,uint8_t mode)
{
	if(mode) //双沿触发
	{
		if(GPIOx < GPIOE)
		{
			GPIO_INT_TRIG_dpulse[0] |= GPIO_Pin<<(8*(GPIOx-GPIOA));
			REG_APB7_GPIO_INT_TRIG_dpulse = GPIO_INT_TRIG_dpulse[0];
//	  	REG_APB7_GPIO_INT_TRIG_dpulse |= GPIO_Pin<<(8*GPIOx);
		}
  	else if(GPIOx <= GPIOF)
		{
			GPIO_INT_TRIG_dpulse[1] |= GPIO_Pin<<(8*(GPIOx-GPIOE));
			REG_APB7_GPIO_INT_TRIG_dpulse2 = GPIO_INT_TRIG_dpulse[1];
//	  	REG_APB7_GPIO_INT_TRIG_dpulse2 |= GPIO_Pin<<(8*(GPIOx-GPIOE));
		}
  }
	else
	{
		if(GPIOx < GPIOE)
		{
			GPIO_INT_TRIG_dpulse[0] &= ~(GPIO_Pin<<(8*GPIOx));
//	  	REG_APB7_GPIO_INT_TRIG_dpulse &= ~(GPIO_Pin<<(8*GPIOx));
			REG_APB7_GPIO_INT_TRIG_dpulse = GPIO_INT_TRIG_dpulse[0];
		}
  	else if(GPIOx <= GPIOF)
		{
			GPIO_INT_TRIG_dpulse[1] &= ~(GPIO_Pin<<(8*(GPIOx-GPIOE)));
//	  	REG_APB7_GPIO_INT_TRIG_dpulse2 &= ~(GPIO_Pin<<(8*(GPIOx-GPIOE)));
			REG_APB7_GPIO_INT_TRIG_dpulse2 = GPIO_INT_TRIG_dpulse[1];
		}
	}
}

/*******************************************************************
*函数名称:SetGPIOINTPulse
*函数描述:设置中断方式,1 下降沿，0 上升沿
*作    者:
*输入参数:
*输出参数:无
*返回说明:无
*其它说明:无
*******************************************************************/
void SetGPIOINTPulse(uint8_t GPIOx,uint8_t GPIO_Pin,uint8_t mode)
{
	if(mode) //下降沿
	{
		if(GPIOx < GPIOE)
		{
			GPIO_INT_PULSE[0] |= GPIO_Pin<<(8*GPIOx);
			REG_APB7_GPIO_INT_PULSE = GPIO_INT_PULSE[0];
		}
  		else if(GPIOx <= GPIOF)
		{
			GPIO_INT_PULSE[1] |= GPIO_Pin<<(8*(GPIOx-GPIOE));
			REG_APB7_GPIO_INT_PULSE2 = GPIO_INT_PULSE[1];
		}
  }
	else
	{
		if(GPIOx < GPIOE)
		{
			GPIO_INT_PULSE[0] &= ~(GPIO_Pin<<(8*GPIOx));
			REG_APB7_GPIO_INT_PULSE = GPIO_INT_PULSE[0];
		}
		else if(GPIOx <= GPIOF)
		{
			GPIO_INT_PULSE[1] &= ~(GPIO_Pin<<(8*(GPIOx-GPIOE)));
			REG_APB7_GPIO_INT_PULSE2 = GPIO_INT_PULSE[1];
		}
	}
}

/*******************************************************************
*函数名称:GPIOINTMode
*函数描述:设置中断方式，高电平触发或者低电平触发
*作    者:
*输入参数:
*输出参数:无
*返回说明:无
*其它说明:无
*******************************************************************/
void SetGPIOINTLevelGPIOINTLevel(uint8_t GPIOx,uint8_t GPIO_Pin,uint8_t mode)
{
	if(mode) //高电平
	{
		if(GPIOx < GPIOE)
	  	REG_APB7_GPIO_INT_LEVEL |= GPIO_Pin<<(8*GPIOx);
  	else if(GPIOx <= GPIOF)
	  	REG_APB7_GPIO_INT_LEVEL |= GPIO_Pin<<(8*(GPIOx-GPIOE));
  }
	else
	{
		if(GPIOx < GPIOE)
	  	REG_APB7_GPIO_INT2_LEVEL &= ~(GPIO_Pin<<(8*GPIOx));
  	else if(GPIOx <= GPIOF)
	  	REG_APB7_GPIO_INT2_LEVEL &= ~(GPIO_Pin<<(8*(GPIOx-GPIOE)));
	}
}

/*******************************************************************
*函数名称:SetGPIOInt
*函数描述:使能或关闭GPIO总中断源
*作    者:
*输入参数:
*输出参数:无
*返回说明:无
*其它说明:无
*******************************************************************/
void SetGPIOInt(uint8_t cmd)
{
	if(cmd)
		REG_APB3_ICU_INT_ENABLE |= (1<<5);  //使能GPIO中断源
	else
		REG_APB3_ICU_INT_ENABLE &= ~(1<<5); 
}

/*******************************************************************
*函数名称:ReadGPIO_IRQflag
*函数描述:读中断标志位
*作    者:
*输入参数: GPIOx,GPIO_Pin_x
*输出参数:无
*返回说明:无
*其它说明:无
*******************************************************************/
uint8_t ReadGPIO_IRQflag(uint8_t GPIOx,uint8_t GPIO_Pin)
{
	uint8_t value = 0;
	
	if(GPIOx <= GPIOD)
	{
		value = (REG_APB7_GPIO_INT_STATUS >> (GPIOx*8))&GPIO_Pin;
	}
	else if((GPIOx == GPIOF) || (GPIOx == GPIOE))
	{
		GPIOx -= GPIOE;
		value = (REG_APB7_GPIO_INT2_STATUS >> (GPIOx*8))&GPIO_Pin;
	}
	
	return value;
}

/*******************************************************************
*函数名称:ReadGPIO_Pin_Data
*函数描述:读GPIO某些IO状态
*作    者:
*输入参数: GPIOx,GPIO_Pin_x
*输出参数:无
*返回说明:无
*其它说明:无
*******************************************************************/
uint8_t ReadGPIO_Pin_Data(uint8_t GPIOx,uint8_t GPIO_Pin)
{
	uint8_t value = 0;
	if(GPIOx <= GPIOF)
		value = (REG_APB7_GPIO_DATA(GPIOx) >> 8)&GPIO_Pin;
	return value;
}

/*******************************************************************
*函数名称:ReadGPIO_Data
*函数描述:读GPIO数据
*作    者:
*输入参数: GPIOx
*输出参数: GPIO 8个Pin的值
*返回说明:无
*其它说明:无
*******************************************************************/
uint8_t ReadGPIO_Data(uint8_t GPIOx)
{
	uint8_t value = 0;
  if(GPIOx <= GPIOF)
		value = (REG_APB7_GPIO_DATA(GPIOx) >> 8) & 0xff;
	return value;
}




/*******************************************************************
*函数名称:SetGPIO
*函数描述:设置GPIO输出 状态
*作    者:
*输入参数: GPIOx
*输出参数: GPIO 8个Pin的值
*返回说明:无
*其它说明:无
*******************************************************************/
void SetGPIO(uint8_t GPIOx,uint8_t Value)
{
  uint32_t Temp;
	if(GPIOx <= GPIOF)
	{
		Temp = (REG_APB7_GPIO_DATA(GPIOx)&0xffffff00)|Value;
		REG_APB7_GPIO_DATA(GPIOx) = Temp;
	}
}

/*******************************************************************
*函数名称:SetGPIOPin
*函数描述:设置GPIOx_Pinx输出 高
*作    者:
*输入参数: GPIOx Pin
*输出参数: 
*返回说明:无
*其它说明:无
*******************************************************************/
void SetGPIOPin(uint8_t GPIOx,uint8_t Pin)
{
	if(GPIOx <= GPIOF)
		REG_APB7_GPIO_DATA(GPIOx) |= Pin;
}

/*******************************************************************
*函数名称:ResetGPIOPin
*函数描述:设置GPIOx_Pinx输出 低
*作    者:
*输入参数: GPIOx Pin
*输出参数: 
*返回说明:无
*其它说明:无
*******************************************************************/
void ResetGPIOPin(uint8_t GPIOx,uint8_t Pin)
{
	if(GPIOx <= GPIOF)
	  REG_APB7_GPIO_DATA(GPIOx) &= (~Pin);
}


/*******************************************************************
*函数名称:ToggleGPIOPin
*函数描述:对输出GPIO 取反操作
*作    者:
*输入参数: GPIOx Pin
*输出参数: 
*返回说明:无
*其它说明:无
*******************************************************************/
void ToggleGPIOPin(uint8_t GPIOx,uint8_t Pin)
{
	if(GPIOx <= GPIOF)
		REG_APB7_GPIO_DATA(GPIOx) ^= Pin;
}



/*******************************************************************
*函数名称: SetGPIOINT_WAKENA
*函数描述: GPIO唤醒总开
*作    者:
*输入参数: 
*输出参数: 
*返回说明:无
*其它说明:无
*******************************************************************/
void SetGPIOINT_WAKENA(uint8_t Cmd)
{
	if(Cmd)
	  REG_APB3_ICU_INT_WAKENA |= (1<<5);
	else
		REG_APB3_ICU_INT_WAKENA &= (~(1<<5));
}

/*******************************************************************
*函数名称: SetGPIOSecondFunction
*函数描述: 
*作    者:
*输入参数: GPIOx
           GPIOx Pin
*输出参数: 
*返回说明:无
*其它说明:无
*******************************************************************/
void SetGPIOSecondFunction(uint8_t GPIOx,uint8_t GPIO_Pin,uint8_t cmd)
{
	if(GPIOx > GPIOF) return;
	if(cmd)
	(*((volatile unsigned long *)(0x00a07000+2*GPIOx*4))) &= (~GPIO_Pin);
	else
	(*((volatile unsigned long *)(0x00a07000+2*GPIOx*4))) |= GPIO_Pin;
}







/***********************  END OF FILES  ***********************/
