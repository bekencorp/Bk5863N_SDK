#ifndef _DAC_H_
#define _DAC_H_


#include "Type.h"






void AUD_CLKDIV(uint32_t Div);
void DACClock(uint8_t cmd);
void DACInt(uint8_t cmd);
void DACINT_WAKENA(uint8_t cmd);



#endif /*_DAC_H_*/


/***********************  END OF FILES  ***********************/
