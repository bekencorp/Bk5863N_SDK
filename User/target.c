#include "target.h"
#include "BK5863.h"

/**************************************************
FunctionName:FIQ_Exception
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
void FIQ_Exception(void) 
{
	unsigned int IntStat;

	IntStat = REG_APB3_ICU_FIQ_FLAG;
	/* Voltage < 2.5v */
	if(IntStat & INT_STATUS_LOW_VOLT_bit)
	{
		BK5863nLBDLowVoltageIntService();
	}
	/* Voltage > 3.0v */
	if(IntStat & INT_STATUS_HIGH_VOLT_bit)
	{               
		BK5863nLBDHighVoltageIntService();
	}	
	/* Clear FIQ */
	REG_APB3_ICU_FIQ_CLEAR = IntStat;
}

/**************************************************
FunctionName:IRQ_Exception
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
void IRQ_Exception(void)
{
  unsigned int sts = REG_APB3_ICU_INT_FLAG;
	unsigned int en = REG_APB3_ICU_INT_ENABLE;
	wakeupflag = 1; 
	/* Uart1 Int Assert */
	if((en&sts)&(INT_STATUS_UART1_bit))
	{
		BK5863nUart1IntService();
	}
  /* Uart2 Int Assert */
	if((en&sts) & INT_STATUS_UART2_bit)
	{
		BK5863nUart2IntService();
	}   
  /* Spi1 Int Assert */
	if((en&sts) & INT_STATUS_SPI1_bit)
	{
		BK5863nSpi1IntService();
	}
	/* Spi2 Int Assert */
	if((en&sts) & INT_STATUS_SPI2_bit)
	{
		BK5863nSpi2IntService();
	}
	/* Spi3 Int Assert */
	if((en&sts) & INT_STATUS_SPI3_bit)
	{
		BK5863nSpi3IntService();
	}
	/* Spi4 Int Assert */
	if((en&sts) & INT_STATUS_SPI4_bit)
	{
		BK5863nSpi4IntService();
	}
  /* I2c1 Int Assert */
  if((en&sts) & INT_STATUS_I2C1_bit)
	{
		BK5863nI2c1IntService();
	}
  /* I2c2 Int Assert */
	if((en&sts) & INT_STATUS_I2C2_bit)
	{
		BK5863nI2c2IntService();
	}   
  /* GPIO Int Assert */
	if((en&sts) & INT_STATUS_GPIO_bit)
	{
		GPIOIntService();
//	  GPIOTest_IRQHandler();
	}
  /* DES Int Assert */
  if((en&sts) & INT_STATUS_DES_bit)
	{
		BK5863nDESIntService();
	}
  /* ADC Int Assert */ 
  if((en&sts) & INT_STATUS_ADC_bit)
	{
		BK5863nADCIntService();
	}
  /* DAC Int Assert */
  if((en&sts) & INT_STATUS_DAC_bit)
	{   
		BK5863nDACIntService();
	}
  /* PWM0 Int Assert */
	if((en&sts) & INT_STATUS_PWM0_bit)
	{
		BK5863nPWM0IntService();
	}
  /* PWM1 Int Assert */
	if((en&sts) & INT_STATUS_PWM1_bit) 
	{
		BK5863nPWM1IntService();
	}
  /* PWM2 Int Assert */
	if((en&sts) & INT_STATUS_PWM2_bit)
	{
		BK5863nPWM2IntService();
	}
  /* PWM3 Int Assert */
  if((en&sts) & INT_STATUS_PWM3_bit)
	{
		BK5863nPWM3IntService();
	}
  /* PWM4 Int Assert */
	if((en&sts) & INT_STATUS_PWM4_bit)
	{
		BK5863nPWM4IntService();
	}
	#if 0
		if((en&sts) & INT_STATUS_MFIFO_bit)
		{
			BK5863nMFIFOIntService();
		}
	#endif
	#if 0
		if((en&sts) & INT_STATUS_Detoutao_bit)
		{
			BK5863nDetoutaoIntService();
		}
	#endif
	#if 0
		if((en&sts) & INT_STATUS_Detoutbo_bit)
		{
			BK5863nDetoutboIntService();
		}
	#endif
	if((en&sts) & INT_STATUS_CDRDER_bit)         //CARD READER Int Assert
	{
		BK5863NCardReaderIntService();
	}  
	REG_APB3_ICU_INT_CLEAR = sts;
}
/**************************************************
FunctionName:GPIOIntService
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
void GPIOIntService(void)
{
	unsigned int stat;
	stat = REG_APB7_GPIO_INT_STATUS;
	#if 1
		if(stat&0xFF)  //GPIOA
		{
			
		}
	#endif
	#if 1
		if(stat&0xFF00) //GPIOB
		{
			if(stat&0x2000)
			  GPIOB5_IRQHandler();
			if(stat&0x4000)
				GPIOB6_IRQHandler();
			if(stat&0x8000)
				GPIOB7_IRQHandler();
		}
	#endif
	#if 1
		if(stat&0xFF0000) //GPIOC
		{
			if(stat&0x100000)
			  GPIOC_Pin4_IRQHandle();
			if(stat&0x200000)
			  GPIOC_Pin5_IRQHandle();
		}
	#endif
	#if 1
		if(stat&0xFF000000) //GPIOD
		{
			
		}
	#endif
	REG_APB7_GPIO_INT_STATUS = stat;
	stat = REG_APB7_GPIO_INT2_STATUS;
	#if 1
		if(stat&0xFF) //GPIOE
		{
			
		}
	#endif
	#if 1
		if(stat&0xFF00) //GPIOF
		{
			if(stat&0x2000)
			  GPIOF5_IRQHandler();
		}
	#endif
	REG_APB7_GPIO_INT2_STATUS = stat;
}

void Undefined_Exception(void)
{
	while(1)
	{
		
	}
}








//================================================================================
