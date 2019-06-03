#ifndef _PWMConfig_H_
#define _PWMConfig_H_


#include "BK5863.h"


#define _TIM3_TEST_

void BK5863PWMInit(unsigned int index, unsigned char type, unsigned int counter);
void PWMOUTTestConfig(void);

#ifdef _TIM3_TEST_
void Timer3TestConfig(void);
void Tim3_IRQHandler(void);
#endif

#endif
