#include "BK5863_GPIO.h"
#include "BK5863.h"

/*******************************************************************
*��������:GPIO_Config
*��������:����GPIO
*��    ��:
*�������:
*�������:��
*����˵��:��
*����˵��:��
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
*��������:SetGPIOIntCmd
*��������:ʹ�ܻ�ر�GPIO���ж�Դ
*��    ��:
*�������:
*�������:��
*����˵��:��
*����˵��:��
*******************************************************************/
void SetGPIOIntCmd(int cmd)
{
	if(cmd)
		REG_APB3_ICU_INT_ENABLE |= (1<<5);  //ʹ��GPIO�ж�Դ
	else
		REG_APB3_ICU_INT_ENABLE &= ~(1<<5);  //ʹ��GPIO�ж�Դ	
}

/*******************************************************************
		Record Trig mode & Pulse & Dpulse 
*******************************************************************/
uint32_t GPIO_INT_TRIG_MODE[2] = {0x00,0x00};
uint32_t GPIO_INT_PULSE[2] = {0x00,0x00};
uint32_t GPIO_INT_TRIG_dpulse[2] = {0x00,0x00};
/*******************************************************************
*��������: SetGPIO_INTConfig
*��������: ����GPIO�жϷ�ʽ
*��    ��:
*�������:GPIOx GPIOConfig
*�������:��
*����˵��:��
*����˵��:��ע�ⲿ�ּĴ���Ϊֻ���Ĵ���
*******************************************************************/
void SetGPIO_INTConfig(uint8_t GPIOx,GPIOConfig_T *GPIOConfig)
{
	if(GPIOx <= GPIOD)
	{
		if(GPIOConfig->Int.INTMode)  //���ش���
		{	
			GPIO_INT_TRIG_MODE[0] |=  ((GPIOConfig->GPIO_Pin)<<(8*GPIOx));
			REG_APB7_GPIO_INT_TRIG_MODE = GPIO_INT_TRIG_MODE[0];
			if(GPIOConfig->Int.INTtrigger)   //GPIOx_pinx 0: ������  1:�½���
			  GPIO_INT_PULSE[0] |= (GPIOConfig->GPIO_Pin<<(8*GPIOx));  
		  else
				GPIO_INT_PULSE[0] &= ~(GPIOConfig->GPIO_Pin<<(8*GPIOx));
       REG_APB7_GPIO_INT_PULSE = GPIO_INT_PULSE[0];			
			if(GPIOConfig->Int.dpulse)   //˫�ش���
			  GPIO_INT_TRIG_dpulse[0] |= (GPIOConfig->GPIO_Pin<<(8*GPIOx));  
		  else //���ش���
				GPIO_INT_TRIG_dpulse[0] &= ~(GPIOConfig->GPIO_Pin<<(8*GPIOx)); 
			REG_APB7_GPIO_INT_TRIG_dpulse = GPIO_INT_TRIG_dpulse[0];
		}
		else
		{
			GPIO_INT_TRIG_MODE[0] &= ~((GPIOConfig->GPIO_Pin)<<(8*GPIOx));
			REG_APB7_GPIO_INT_TRIG_MODE = GPIO_INT_TRIG_MODE[0];
			if(GPIOConfig->Int.INTtrigger)   //GPIOx_Pinx: �ߵ�ƽ  0:�͵�ƽ
			  REG_APB7_GPIO_INT_LEVEL |= (GPIOConfig->GPIO_Pin<<(8*GPIOx)); 
		  else
				REG_APB7_GPIO_INT_LEVEL &= ~(GPIOConfig->GPIO_Pin<<(8*GPIOx)); 
		}
		/* �жϿ��� */
		if(GPIOConfig->Int.En_Dis_able)
		  REG_APB7_GPIO_INT_ENABLE |= (GPIOConfig->GPIO_Pin<<(8*GPIOx));  //Enable
		else
			REG_APB7_GPIO_INT_ENABLE &= ~(GPIOConfig->GPIO_Pin<<(8*GPIOx));  //disable
	}
	else if((GPIOx == GPIOF) || (GPIOx == GPIOE))
	{
		GPIOx -= GPIOE;
		if(GPIOConfig->Int.INTMode)  //��Ե����
		{	
			GPIO_INT_TRIG_MODE[1] |= ((GPIOConfig->GPIO_Pin)<<(8*GPIOx));
			REG_APB7_GPIO_INT_TRIG_MODE2 = GPIO_INT_TRIG_MODE[1];
			if(GPIOConfig->Int.INTtrigger)   //GPIOx_pinx 1: ������  0:�½���
			  GPIO_INT_PULSE[1] |= (GPIOConfig->GPIO_Pin<<(8*GPIOx));  
		  else
				GPIO_INT_PULSE[1] &= ~(GPIOConfig->GPIO_Pin<<(8*GPIOx)); 
			REG_APB7_GPIO_INT_PULSE2 = GPIO_INT_PULSE[1];
			if(GPIOConfig->Int.dpulse)   //˫�ش���
			  GPIO_INT_TRIG_dpulse[1] |= (GPIOConfig->GPIO_Pin<<(8*GPIOx));  
		  else //���ش���
				GPIO_INT_TRIG_dpulse[1] &= ~(GPIOConfig->GPIO_Pin<<(8*GPIOx));
			REG_APB7_GPIO_INT_TRIG_dpulse2 = GPIO_INT_TRIG_dpulse[1];
		}
		else
		{
			GPIO_INT_TRIG_MODE[1] &= ~((GPIOConfig->GPIO_Pin)<<(8*GPIOx));
			REG_APB7_GPIO_INT_TRIG_MODE2 = GPIO_INT_TRIG_MODE[1];
			if(GPIOConfig->Int.INTtrigger)   //GPIOx_Pinx: �ߵ�ƽ  0:�͵�ƽ
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
*��������:SetGPIOIRQ
*��������:ʹ�ܻ�ر�ĳ��GPIO�ж�Դ
*��    ��:
*�������:
*�������:��
*����˵��:��
*����˵��:��
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
*��������:SetGPIOINTMode
*��������:�����жϷ�ʽ����ƽ���߱��ش���
*��    ��:
*�������:
*�������:��
*����˵��:��
*����˵��:��
*******************************************************************/
void SetGPIOINTMode(uint8_t GPIOx,uint8_t GPIO_Pin,uint8_t mode)
{
	if(mode) //���ش���
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
	else  //��ƽ����
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
*��������:SetGPIOINTPulseMode
*��������:���ش���������˫�ش���
*��    ��:
*�������:
*�������:��
*����˵��:��
*����˵��:��
*******************************************************************/
void SetGPIOINTPulseMode(uint8_t GPIOx,uint8_t GPIO_Pin,uint8_t mode)
{
	if(mode) //˫�ش���
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
*��������:SetGPIOINTPulse
*��������:�����жϷ�ʽ,1 �½��أ�0 ������
*��    ��:
*�������:
*�������:��
*����˵��:��
*����˵��:��
*******************************************************************/
void SetGPIOINTPulse(uint8_t GPIOx,uint8_t GPIO_Pin,uint8_t mode)
{
	if(mode) //�½���
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
*��������:GPIOINTMode
*��������:�����жϷ�ʽ���ߵ�ƽ�������ߵ͵�ƽ����
*��    ��:
*�������:
*�������:��
*����˵��:��
*����˵��:��
*******************************************************************/
void SetGPIOINTLevelGPIOINTLevel(uint8_t GPIOx,uint8_t GPIO_Pin,uint8_t mode)
{
	if(mode) //�ߵ�ƽ
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
*��������:SetGPIOInt
*��������:ʹ�ܻ�ر�GPIO���ж�Դ
*��    ��:
*�������:
*�������:��
*����˵��:��
*����˵��:��
*******************************************************************/
void SetGPIOInt(uint8_t cmd)
{
	if(cmd)
		REG_APB3_ICU_INT_ENABLE |= (1<<5);  //ʹ��GPIO�ж�Դ
	else
		REG_APB3_ICU_INT_ENABLE &= ~(1<<5); 
}

/*******************************************************************
*��������:ReadGPIO_IRQflag
*��������:���жϱ�־λ
*��    ��:
*�������: GPIOx,GPIO_Pin_x
*�������:��
*����˵��:��
*����˵��:��
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
*��������:ReadGPIO_Pin_Data
*��������:��GPIOĳЩIO״̬
*��    ��:
*�������: GPIOx,GPIO_Pin_x
*�������:��
*����˵��:��
*����˵��:��
*******************************************************************/
uint8_t ReadGPIO_Pin_Data(uint8_t GPIOx,uint8_t GPIO_Pin)
{
	uint8_t value = 0;
	if(GPIOx <= GPIOF)
		value = (REG_APB7_GPIO_DATA(GPIOx) >> 8)&GPIO_Pin;
	return value;
}

/*******************************************************************
*��������:ReadGPIO_Data
*��������:��GPIO����
*��    ��:
*�������: GPIOx
*�������: GPIO 8��Pin��ֵ
*����˵��:��
*����˵��:��
*******************************************************************/
uint8_t ReadGPIO_Data(uint8_t GPIOx)
{
	uint8_t value = 0;
  if(GPIOx <= GPIOF)
		value = (REG_APB7_GPIO_DATA(GPIOx) >> 8) & 0xff;
	return value;
}




/*******************************************************************
*��������:SetGPIO
*��������:����GPIO��� ״̬
*��    ��:
*�������: GPIOx
*�������: GPIO 8��Pin��ֵ
*����˵��:��
*����˵��:��
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
*��������:SetGPIOPin
*��������:����GPIOx_Pinx��� ��
*��    ��:
*�������: GPIOx Pin
*�������: 
*����˵��:��
*����˵��:��
*******************************************************************/
void SetGPIOPin(uint8_t GPIOx,uint8_t Pin)
{
	if(GPIOx <= GPIOF)
		REG_APB7_GPIO_DATA(GPIOx) |= Pin;
}

/*******************************************************************
*��������:ResetGPIOPin
*��������:����GPIOx_Pinx��� ��
*��    ��:
*�������: GPIOx Pin
*�������: 
*����˵��:��
*����˵��:��
*******************************************************************/
void ResetGPIOPin(uint8_t GPIOx,uint8_t Pin)
{
	if(GPIOx <= GPIOF)
	  REG_APB7_GPIO_DATA(GPIOx) &= (~Pin);
}


/*******************************************************************
*��������:ToggleGPIOPin
*��������:�����GPIO ȡ������
*��    ��:
*�������: GPIOx Pin
*�������: 
*����˵��:��
*����˵��:��
*******************************************************************/
void ToggleGPIOPin(uint8_t GPIOx,uint8_t Pin)
{
	if(GPIOx <= GPIOF)
		REG_APB7_GPIO_DATA(GPIOx) ^= Pin;
}



/*******************************************************************
*��������: SetGPIOINT_WAKENA
*��������: GPIO�����ܿ�
*��    ��:
*�������: 
*�������: 
*����˵��:��
*����˵��:��
*******************************************************************/
void SetGPIOINT_WAKENA(uint8_t Cmd)
{
	if(Cmd)
	  REG_APB3_ICU_INT_WAKENA |= (1<<5);
	else
		REG_APB3_ICU_INT_WAKENA &= (~(1<<5));
}

/*******************************************************************
*��������: SetGPIOSecondFunction
*��������: 
*��    ��:
*�������: GPIOx
           GPIOx Pin
*�������: 
*����˵��:��
*����˵��:��
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
