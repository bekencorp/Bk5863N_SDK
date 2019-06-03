#include "ATE.h"

void ATEInit(void)
{
	unsigned int StartAddr;
	/*erase Calibration Record Flash Address */
	StartAddr = 0x70000;
	flash_init();
	flash_erase_sector(StartAddr);
}

void ATETest(unsigned char Cmd)
{
	switch(Cmd)
	{
		case _ATE_TEST_Calibration:
			ATECalibrationExecute();
			break;
		
		default:
			break;
	}
}


