#include "CRCconfig.h"
#include "Includes.h"

/**************************************************
FunctionName:
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
unsigned int CalculateCRC(uint8_t CRCType,uint8_t *str,uint32_t strlength)
{
  CRC_START();
  return (CRCCalculate(CRCType,str,strlength));
}




