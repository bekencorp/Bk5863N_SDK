#ifndef _FlashConfig_H_
#define _FlashConfig_H_



#include "BK5863.h"


//#define MFC_READ                    0x00        
//#define MFC_WRITE                   0x01
//#define MFC_SECTOR_EARSE            0x02
//#define MFC_BLOCK_EARSE             0x03
//#define MFC_CHIP_EARSE              0x07
//#define MAIN_SPACE                  0x00
//#define NVR_SPACE                   0x01



void BK5863FlashSectorTest(unsigned char SectorMode);
void BK5863MfcOperate(uint8_t Type,uint8_t ReadLength,unsigned int Addr,unsigned char *pData);
void FLASHInit(void);

#endif
