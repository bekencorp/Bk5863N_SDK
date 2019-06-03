#ifndef _TEST_H_
#define _TEST_H_
#include "Type.h"

//#define _TEST_DEMO_
#ifdef _TEST_DEMO_

#define AUTO_RX_ENABLE 1
#define AUTO_RX_DISABLE 0
#define OUTPUT_DEBUG_INFO 0
#define MCU_NO_SLEEP_FUNC 1
void		zero_wakeup_test_3_init(uint8 	status);
void		zero_wakeup_test_3(uint8	status);

#endif /*_TEST_DEMO_*/
#endif

