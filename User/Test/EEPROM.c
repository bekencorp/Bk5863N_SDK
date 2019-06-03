#include "EEPROM.h"
#include "BKTestAPP.h"
#include "Includes.h"
/**************************************************
FunctionName: 
Describe: 
InputParameter:	
OutputParameter:
Notice:
**************************************************/
void I2cE2promByteWrite(unsigned char Addr,unsigned char Data)
{
  /* 
	   Byte Write: 
	   STA+{SlvAddr, 1'b0}+RegAddr+Byte+STO 
	*/
//    I2cRepeatStart = 0;
//    I2cTxLen = 2;
//    I2cTxBuff[0] = Addr;
//    I2cTxBuff[1] = Data;
    I2cSendStart(IIC1,(I2C_REMOTE_ADDR<<1));     
//    while(!I2cTransDone);  
}
/**************************************************
FunctionName: 
Describe: 
InputParameter:	
OutputParameter:
Notice:
**************************************************/
void I2cE2promRandomRead(unsigned char Addr,unsigned char *pData)
{
  /*
  	Random Address Read:
	  STA+{SlvAddr, 1'b0}+RegAddr+STA+{SlvAddr, 1'b1}+Byte+STO 
	*/
//    I2cRepeatStart = 1;
//    I2cTxLen = 1;
//    I2cTxBuff[0] = Addr;
    I2cSendStart(IIC1,(I2C_REMOTE_ADDR<<1));
//    while(!I2cTransDone);
//    *pData = I2cRxBuff[0];
}
/**************************************************
FunctionName:  
Describe: 
InputParameter:	
OutputParameter:
Notice:
**************************************************/
void I2cE2promPageWrite(unsigned char Addr,unsigned char Len,unsigned char *pData)
{
  /* 
	   Page Write: 
	   STA+{SlvAddr, 1'b0}+RegAddr+Byte0+Byte1+......+Byte14+STO 
	*/
//    I2cRepeatStart = 0;
//    I2cTxLen = Len+1;           // 1 address + Len data
//    I2cTxBuff[0] = Addr;
//    memcpy(I2cTxBuff+1, pData, Len);
    I2cSendStart(IIC1,(I2C_REMOTE_ADDR<<1));     
//    while(!I2cTransDone);  
}
/**************************************************
FunctionName:  
Describe: 
InputParameter:	
OutputParameter:
Notice:
**************************************************/
void I2cE2promSequenceRead(unsigned char Len,unsigned char *pData)
{
   /* 
	   Sequence Read: 
	   STA+{SlvAddr, 1'b1}+Byte0+Byte1+......+Byte14+STO 
	 */
//    I2cRepeatStart = 0;
//    I2cRxLen = Len;
    I2cSendStart(IIC1,((I2C_REMOTE_ADDR<<1)+1));     
//    while(!I2cTransDone);  
//    memcpy(pData, I2cRxBuff, Len);
}

/**************************************************
FunctionName: 
Describe: 
InputParameter:	
OutputParameter:
Notice:
**************************************************/
void EEPROMConfig(void)
{
	IICPara_T i2c1conf;
	
	IICClk(IIC1,Enable);//open i2c1 clock
	REG_APB7_GPIOD_CFG &= 0xfffffffc; //gpiod[1:0] 2nd func enable
	IICIntCmd(IIC1,Enable); //open i2c1 int enable
	
	i2c1conf.ENSMB = Enable;
	i2c1conf.INH = 1;    //master or slave 1:Disable Slave
	i2c1conf.SMBFTE = 1;
	i2c1conf.SMBTOE = 0;
	i2c1conf.SMBCS = _InternalClock; //2bit
	i2c1conf.SLV_ADDR = 0x16; //10bit
	i2c1conf.FREQ_DIV = 0x1F;  //10bit
	i2c1conf.SCL_CR = 0x04;
	i2c1conf.IDLE_CR = 0x03;
	
	IICConfig(IIC1,&i2c1conf);
	
}











