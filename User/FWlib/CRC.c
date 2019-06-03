#include "CRC.h"
#include "BK5863.h"

/**************************************************
FunctionName:CRC_START
Describe: 启动CRC校验功能
InputParameter:
OutputParameter:
Notice:
**************************************************/
void CRC_START(void)
{
  REG_CRC_STARTE   =0x5aa5 ;
}

/**************************************************
FunctionName:CRCDataIn
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
void CRCDataIn(unsigned char dat)
{
  REG_CRC_DATA_IN = dat;
}
/**************************************************
FunctionName:
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
void CRCDataInString(unsigned char *str, unsigned int length)
{
  unsigned int i;
  for(i=0;i<length;i++)
  {
	  REG_CRC_DATA_IN = str[i];
  }
}
/**************************************************
FunctionName:
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
unsigned int CRC_CRC0Out(void)
{
  return REG_CRC_CRC0_OUT;
}

/**************************************************
FunctionName:
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
unsigned int CRC_CRC1Out(void)
{
  return REG_CRC_CRC1_OUT;
}
/**************************************************
FunctionName:CRCCalculate
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
unsigned int CRCCalculate(uint8_t CRCType,uint8_t *str,uint32_t strlength)
{
  unsigned int i;
  for(i=0;i<strlength;i++)
  {
	  REG_CRC_DATA_IN = str[i];
  }
	REG_CRC_DATA_IN = 0;
	REG_CRC_DATA_IN = 0;
  if(CRCType)
  	return REG_CRC_CRC1_OUT;
  else
  	return REG_CRC_CRC0_OUT;
}




/***********************  END OF FILES  ***********************/
