#include "misc.h"
#include "BK5863.h"
/**************************************************
FunctionName:
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
void CmdIRQ(unsigned char cmd)
{
	if(cmd)
	{
		REG_APB3_ICU_IRQTYPE_EN |= 1;
	}
	else
	{
		REG_APB3_ICU_IRQTYPE_EN &= (~1);
	}
}
/**************************************************
FunctionName:CmdFIQ
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
void CmdFIQ(unsigned char cmd)
{
	if(cmd)
	{
		REG_APB3_ICU_IRQTYPE_EN |= (1<<1);
	}
	else
	{
		REG_APB3_ICU_IRQTYPE_EN &= (~(1<<1));
	}
}
/**************************************************
FunctionName:
Describe: 使能或关闭 ICU中断 
InputParameter:
OutputParameter:
Notice:
**************************************************/
void CmdINT(unsigned int Souce,unsigned char cmd)
{
	if(cmd)
	{
		REG_APB3_ICU_INT_ENABLE |= Souce;
	}
	else
	{
		REG_APB3_ICU_INT_ENABLE &= (~Souce);
	}
	
}
/**************************************************
FunctionName:
Describe: Charge
InputParameter:
OutputParameter:
Notice:
**************************************************/
void CmdCharge(unsigned char ICU0x30bit27)
{
	if(ICU0x30bit27)
	{
		REG_APB3_ICU_ANA1_CTRL |= (1<<27);
	}
	else
	{
		REG_APB3_ICU_ANA1_CTRL &= (~(1<<27));
	}
	
}
/**************************************************
FunctionName:
Describe: 
InputParameter:
OutputParameter:
Notice:
**************************************************/
void CmdOutVCC(unsigned char ICU0x30bit28)
{
	if(ICU0x30bit28)
	{
		REG_APB3_ICU_ANA1_CTRL |= (1<<28);
	}
	else
	{
		REG_APB3_ICU_ANA1_CTRL &= (~(1<<28));
	}
}

/**************************************************
FunctionName: ReadChipID
Describe:  读取芯片ID
InputParameter:
OutputParameter:
Notice:
**************************************************/
unsigned int ReadChipID(void)
{
	return ((*((volatile unsigned long *)(0x00a03000+(0x3C*4))))&0xFFFFFF);
}

/**************************************************
FunctionName: ReadDeviceID
Describe:  Read device Id
InputParameter:
OutputParameter:
Notice:
**************************************************/
unsigned int ReadDeviceID(void)
{
	return (*((volatile unsigned long *)(0x00a03000+(0x3d*4))));
}

/**************************************************
FunctionName: 
Describe: Close INT En & FLag
InputParameter:
OutputParameter:
Notice:
**************************************************/
void CLOSEAllIntEnableAndFlag(void)
{
	REG_APB3_ICU_INT_ENABLE = 0;
	REG_APB3_ICU_INT_CLEAR = REG_APB3_ICU_INT_FLAG;
	REG_APB3_ICU_FIQ_CLEAR = REG_APB3_ICU_FIQ_FLAG;
	REG_APB8_UART1_INT_ENABLE = 0;
	REG_APB8_UART1_CFG = 0;
	REG_APB8_UART1_INT_FLAG = REG_APB8_UART1_INT_FLAG;
	REG_APB9_UART2_INT_ENABLE = 0;
	REG_APB9_UART2_CFG = 0;
	REG_APB9_UART2_INT_FLAG = REG_APB9_UART2_INT_FLAG;
	REG_APBA_SPI1_STAT = REG_APBA_SPI1_STAT;
	REG_APBA_SPI1_STAT = 0;
	REG_APBA_SPI1_CTRL = 0;
	REG_APBB_SPI2_CTRL = 0;
	REG_APBB_SPI2_STAT = REG_APBB_SPI2_STAT;
	REG_APBB_SPI2_STAT = 0;
	REG_APBC_SPI3_CTRL = 0;
	REG_APBC_SPI3_STAT = REG_APBC_SPI3_STAT;
	REG_APBC_SPI3_STAT = 0;
	REG_APBD_SPI4_CTRL = 0;
	REG_APBD_SPI4_STAT = REG_APBD_SPI4_STAT;
	REG_APBD_SPI4_STAT = 0;
	REG_APBE_I2C1_STAT = REG_APBE_I2C1_STAT;
	REG_APBF_I2C2_STAT = REG_APBF_I2C2_STAT;
	REG_RTC_CFG = REG_RTC_CFG;
	REG_RTC_CFG = 0;
	REG_APB10_PWM_CFG = REG_APB10_PWM_CFG;
	REG_APB10_PWM_CFG = 0;
	REG_APB4_DES_INTF = 0;
	REG_APB4_DES_CTL |= 1<<DES_ADDR_RST_BIT;
	REG_APB7_GPIO_INT_STATUS = REG_APB7_GPIO_INT_STATUS;
	REG_APB7_GPIO_INT_STATUS = REG_APB7_GPIO_INT_STATUS;
	REG_APB7_GPIO_INT2_STATUS = REG_APB7_GPIO_INT2_STATUS;
	REG_APB7_GPIO_INT2_STATUS = REG_APB7_GPIO_INT2_STATUS;
	REG_APB3_ICU_GPIOB7CLKCON |= B7_XH_CLKOUT_EN;
}

void bk5824_wko_output_to_other_gpio(int stat)
{
	if(stat)
	 	REG_APG3_ICU_DEEPSLEEP      |= 1<<3;                 //gpio test enable
	 else
	 	REG_APG3_ICU_DEEPSLEEP      &= ~(1<<3);    
	 REG_APG3_ICU_DEEPSLEEP = 0x0e;
}

void set_bk58xx_figure_voltage(unsigned char voltage)
{
	unsigned int temp = (*((volatile unsigned long *)	(0x00a03000+0x30*4)));
	temp &= (~(3<<18));
	(*((volatile unsigned long *)	(0x00a03000+0x30*4))) = (temp | ((voltage&0x03)<<18));
}

int get_bk58xx_figure_voltage(void)
{
	unsigned int temp = (*((volatile unsigned long *)	(0x00a03000+0x30*4)));

	return ((temp>>18)&&0x03);
}


/***********************  END OF FILES  ***********************/
