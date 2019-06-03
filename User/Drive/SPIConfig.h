#ifndef _SPICONFIG_H_
#define _SPICONFIG_H_

#include "BK5863.h"





void BK5863SpiInit(unsigned char SpiIdx, SpiMsg_t *pSpiMsgPara);
void BK5863SpiReset(unsigned char SpiIdx);
void BK5863SpiDisable(unsigned char SpiIdx);
void BK5863SpiSendStart(unsigned char SpiIdx);


#endif /*_SPICONFIG_H_*/

