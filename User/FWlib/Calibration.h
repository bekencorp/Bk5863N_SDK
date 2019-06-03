#ifndef _calibration_H_
#define _calibration_H_

#include "Type.h"



unsigned int ReadCalibCoef6(void);

void StartANA_Calib(void);
unsigned char CalibrationDone(void);
unsigned int ReadCalibrationResult(void);
char CalibrationConf(unsigned int conf);

unsigned int ExecuteCalibAndGetCalibValue(void);

#endif




/***********************  END OF FILES  ***********************/
