#ifndef _CRC_H_
#define _CRC_H_


#include "Type.h"




enum{
	_CRC0 = 0,
	_CRC1 = 1
};

void CRC_START(void);
void CRCDataIn(unsigned char dat);
void CRCDataInString(unsigned char *str, unsigned int length);
unsigned int CRC_CRC0Out(void);
unsigned int CRC_CRC1Out(void);
unsigned int CRCCalculate(uint8_t CRCType,uint8_t *str,uint32_t strlength);

#endif

/***********************  END OF FILES  ***********************/
