#ifndef _IICconfig_H_
#define _IICconfig_H_

#include "BK5863.h"
#include "BK5863Com.h"


void BK5863I2cDisable(unsigned char I2cIdx);
void BK5863I2cInit(unsigned char I2cIdx, I2cMsg_t *pI2cMsgPara);
void I2C1Init(void);
void BK5863I2cReset(unsigned char I2cIdx);
void BK5863I2cSendStart(unsigned char I2cIdx, unsigned char AddrByteData);
void BK5863I2cIntService(unsigned char I2cIdx);

extern I2cMsg_t *pI2cMsg[2];

#endif 

