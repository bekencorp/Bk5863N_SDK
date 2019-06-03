#include "IIC.h"
#include "BK5863.h"
/**************************************************
FunctionName:IICClk
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
void IICClk(uint8_t IICx,uint8_t cmd)
{
	if(IICx == IIC1)
	{
		if(cmd)
			REG_APB3_ICU_I2C1CLKCON  = 0;	//IIC1 clock enable
	  else
			REG_APB3_ICU_I2C1CLKCON  |= 0x01;	
  }
	else if(IICx == IIC2)
	{
		if(cmd)
			REG_APB3_ICU_I2C2CLKCON  = 0;	//IIC2 clock enable
	  else
			REG_APB3_ICU_I2C2CLKCON  |= 0x01;	
	}
}
/**************************************************
FunctionName:IICIntCmd
Describe:开IIC中断或关闭
InputParameter:
OutputParameter:
Notice:
**************************************************/
void IICIntCmd(unsigned char IICx,unsigned char Cmd)
{
	if(IICx == IIC1)
	{
		if(Cmd)
			REG_APB3_ICU_INT_ENABLE |= INT_STATUS_I2C1_bit;
		else
			REG_APB3_ICU_INT_ENABLE &= (~(INT_STATUS_I2C1_bit));
  }
	else if(IICx == IIC2)
	{
		if(Cmd)
			REG_APB3_ICU_INT_ENABLE |= INT_STATUS_I2C2_bit;
		else
			REG_APB3_ICU_INT_ENABLE &= (~(INT_STATUS_I2C2_bit));
  }
}
/**************************************************
FunctionName:BK5863I2CENSMB
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
void BK5863I2CENSMB(unsigned char IICx,unsigned char Cmd)
{
	switch(IICx)
	{
		case IIC1: 
			if(Cmd)
				REG_APBE_I2C1_CN |= I2C_EN_BIT;
			else
				REG_APBE_I2C1_CN &= ~I2C_EN_BIT;
			break;
		case IIC2: 
			if(Cmd)
				REG_APBF_I2C2_CN |= I2C_EN_BIT;
			else
				REG_APBF_I2C2_CN &= ~I2C_EN_BIT;
			break;
		default:
			break;
	}
}
/**************************************************
FunctionName:
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
unsigned char IICReadData(unsigned char IICx)
{
	if(IICx == IIC1)
		return (REG_APBE_I2C1_DAT&0xFF);
	else if(IICx == IIC2)
		return (REG_APBF_I2C2_DAT&0xFF);
	return 0;
}

/**************************************************
FunctionName:
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
unsigned char IICWriteData(unsigned char IICx ,unsigned char dat)
{
	if(IICx == IIC1)
	{
		REG_APBE_I2C1_DAT = dat;
		return 1;
	}
	else if(IICx == IIC2)
	{
	  REG_APBF_I2C2_DAT = dat;
		return 1;
	}
	return 0;
}

/**************************************************
FunctionName: IICConfig
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
void IICConfig(uint8_t IICx,IICPara_T *conf)
{
	uint32_t Temp = 0;
	Temp |= (conf->ENSMB&0x01)<<31;
	Temp |= (conf->INH&0x01)<<30;
	Temp |= (conf->SMBFTE&0x01)<<29;
	Temp |= (conf->SMBTOE&0x01)<<28;
	Temp |= (conf->SMBCS&0x03)<<26;
	Temp |= (conf->SLV_ADDR&0x3FF)<<16;
	Temp |= (conf->FREQ_DIV&0x3FF)<<6;
	Temp |= (conf->SCL_CR&0x07)<<3;
	Temp |= (conf->IDLE_CR&0x07)<<0;
	if(IICx == IIC1)
		REG_APBE_I2C1_CN = Temp;
	else if(IICx == IIC2)
		REG_APBF_I2C2_CN = Temp;	
}

/**************************************************
FunctionName:I2cSendStart
Describe:I2c Start Send in Master Role
InputParameter:
OutputParameter:
Notice: slave address and operate code
**************************************************/
void I2cSendStart(uint8_t IICx,uint8_t AddrByteData)
{
	if(IICx==IIC1)
	{
		REG_APBE_I2C1_DAT = AddrByteData;
	  REG_APBE_I2C1_STAT |= I2C_STA_BIT;      //set sta
	}
	else if(IICx == IIC2)
	{
		REG_APBF_I2C2_DAT	= AddrByteData;//address byte
		REG_APBF_I2C2_STAT |= I2C_STA_BIT;      //set sta 
	}
}
/**************************************************
FunctionName:SetI2cSLV_ADDR
Describe: Set I2c SLV_ADDR
InputParameter:
OutputParameter:
Notice:
**************************************************/
void SetI2cSLV_ADDR(uint8_t IICx,uint16_t SLV_ADDR)
{
	uint32_t Temp;
	if(IICx == IIC1)
	{
		Temp = REG_APBE_I2C1_CN;
		Temp &= (~(0X2FF<<16));
		Temp |= ((SLV_ADDR&0x2FF)<<16);
		REG_APBE_I2C1_CN = Temp;
	}
	else if(IICx == IIC2)
	{
		Temp = REG_APBF_I2C2_CN;
		Temp &= (~(0X2FF<<16));
		Temp |= ((SLV_ADDR&0x2FF)<<16);
		REG_APBF_I2C2_CN = Temp;
	}
}

/***********************  END OF FILES  ***********************/
