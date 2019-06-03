#ifndef _LBD_H_
#define _LBD_H_

#include "Type.h"



void SetHighVoltageInterrupt(uint8_t cmd);
void SetLowVoltageInterrupt(uint8_t cmd);

unsigned char ReadLBDVoltageCompartorResult(void);
unsigned char ReadHighVoltageInterruptFlag(void);
unsigned char ReadLowVoltageInterruptFlag(void);
void CleanVoltageInterruptFlag(unsigned char cleanflag);

void SwitchCLK32KHz(uint8_t cmd);

#endif






/***********************  END OF FILES  ***********************/
