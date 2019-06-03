#ifndef _BKCardTest_H_
#define _BKCardTest_H_

#include "type.h"
#include "BK5863.h"

char BK5863TestCpuCard(CardReaderMsg_t *pCardReaderMsg);
char BK5863TestOperateM1Card(CardReaderMsg_t *pCardReaderMsg);


char BK5863TestOperateM1Card_ops(CardReaderMsg_t *pCardReaderMsg,char *msg,unsigned char len);


#endif




