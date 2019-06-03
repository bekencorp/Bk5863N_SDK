#ifndef _SysConfig_H_
#define _SysConfig_H_


#include "type.h"

#define Enable 1
#define Disable 0

#define SYS_32K_CLK_TIMER				32768

void BK5863SystemInit(void);

void BK5863EnterStandby(unsigned char StandbyMode);



extern int wakeupflag;



#endif
