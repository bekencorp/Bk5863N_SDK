#ifndef _TDesConfig_H_
#define _TDesConfig_H_



#include "BK5863.h"



typedef struct BKDES_CONTROL_STRU_internal
{
	unsigned char	*pInput;
	unsigned char	*pOutput;	
	unsigned int	bufLen;
	unsigned int 	inIndex;
	unsigned int 	outIndex;
}BKDES_CONTROL_STRU,BKDES_CONTROL_STRU_T;


void bk_test_des(void);
void BK5863DESInit(int des_mode, int mac_mode);

void BK5863NTDESInit(void);

int BK5863DESOperation(uint8_t* input,uint32_t length,uint8_t* output,uint32_t operation);


extern BKDES_CONTROL_STRU  BKDES_CTL_BLK;
extern const unsigned char bk_des_input[32];

extern unsigned char des_key1_set[DES_KEY_LEN];
extern unsigned char des_key2_set[DES_KEY_LEN];
extern unsigned char des_key3_set[DES_KEY_LEN];





#endif
