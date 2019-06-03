#ifndef _SYSSLEEP_H_
#define _SYSSLEEP_H_

#include "BK5863.h"


enum{
	_DeepSleep = (1<<0),
	_OnlyRFRouse = (1<<1),
	_OnlyCharge = (1<<2),
	_RFRouseAndCharge = (1<<3),
};




void DeepSleepConfig(void);
void OnlyRFRouseConfig(void);
void OnlyChargeConfig(void);
void RFRouseAndChargeConfig(void);

void ConfigRouseSystem(void);
void BK5863nSysSleep(uint32_t SleepMode);

void MyBoardSleepConfig(uint32_t SleepMode);
void MyBoardSleepRecoverConfig(uint32_t SleepMode);



#endif

/***********************  END OF FILES  ***********************/
