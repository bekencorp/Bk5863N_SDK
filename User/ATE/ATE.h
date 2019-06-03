#ifndef _ATE_H_
#define _ATE_H_


#include "BK5863.h"
#include "ATEcalibration.h"
#define _ATE_Test_

enum{
	_ATE_TEST_Calibration = 0xC0,

};





void ATEInit(void);
void ATETest(unsigned char Cmd);

#endif /* _ATE_H_ */

/***********************  END OF FILES  ***********************/
