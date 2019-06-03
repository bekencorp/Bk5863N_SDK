#include "ATEcalibration.h"
#include "ATE.h"

/**************************************************
FunctionName:
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
void ATECalibrationExecute(void)
{
	union Uuint16 Value,ReReadValue;
	unsigned int StartAddr = 0x70000,len = 4;
	unsigned char Times = 3;
	flash_init();
	Value.U32V = ExecuteCalibAndGetCalibValue();
//	printf("CalibValue = 0x%08x\r\n",Value.U32V);
  
a:
	flash_write_data(Value.C8V,StartAddr,len);
	flash_read_data(ReReadValue.C8V,StartAddr,len);
//  printf("ReadCalibValue = 0x%08x\r\n",ReReadValue.U32V);
	if(ReReadValue.U32V != Value.U32V)
	{
		flash_erase_sector(StartAddr);
//		printf("Error\r\n");
		Times--;
		if(Times)
			goto a;
	}
	return;
}

/**************************************************
FunctionName:
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/















/***********************  END OF FILES  ***********************/
