#ifndef _ATECALIBRATION_H_
#define _ATECALIBRATION_H_

#include "BK5863.h"


union Uuint16{
	unsigned int U32V;
	unsigned char C8V[4];
};

void ATECalibrationExecute(void);



#endif /* _ATECALIBRATION_H_ */

/***********************  END OF FILES  ***********************/
