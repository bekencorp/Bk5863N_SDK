#ifndef _IIC_H_
#define _IIC_H_

#include "BK5863Com.h"
#include "Type.h"

enum{
	IIC1 = 0,
	IIC2 = 1
};

enum{
	_Time0Spill = 0,
	_Time1Spill = 1,
	_Time2Spill = 2,
	_InternalClock = 3
};

typedef struct{
	uint8_t ENSMB;
	uint8_t INH;    //master or slave
	uint8_t SMBFTE;
	uint8_t SMBTOE;
	uint8_t SMBCS; //2bit
	uint16_t SLV_ADDR; //10bit
	uint16_t FREQ_DIV;  //10bit
	uint8_t SCL_CR;
	uint8_t IDLE_CR;
}IICPara_T;




void IICClk(uint8_t IICx,uint8_t cmd);
void IICIntCmd(unsigned char IICx,unsigned char Cmd);
void BK5863I2CENSMB(unsigned char IICx,unsigned char Cmd);
unsigned char IICReadData(unsigned char IICx);
unsigned char IICWriteData(unsigned char IICx ,unsigned char dat);
void IICConfig(uint8_t IICx,IICPara_T *conf);
void I2cSendStart(uint8_t IICx,uint8_t AddrByteData);
void SetI2cSLV_ADDR(uint8_t IICx,uint16_t SLV_ADDR);



#endif




/***********************  END OF FILES  ***********************/
