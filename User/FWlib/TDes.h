#ifndef _TDES_H_
#define _TDES_H_


#include "Type.h"

//BK5863N DES
#define DES_FRAME_LEN   		8
#define DES_KEY_LEN   			8
#define DES_MAC_ECB				0
#define DES_MAC_CBC				1
#define DES_1_KEY_MODE			1
#define DES_3_KEY_MODE			0
#define DES_DECRYPTION			1
#define DES_ENCRYPTION			0
#define _DES_KEY_LEN   			8

typedef struct{
	unsigned char DESCLKCON;  //´ò¿ªCLOCK
	unsigned char DoDES_RST;  //ÏÈRST
	unsigned char DES_TDES;
	unsigned char DES_EN;
	unsigned char DEC_ENC;
	unsigned char MAC;
	unsigned char INT_ENABLE;
	unsigned char INT_WAKENA;
	unsigned char *Key1;
	unsigned char *Key2;
	unsigned char *Key3;
}TDEConfigPara_T;

void TDESClk(uint8_t cmd);
void TDESReset(void);
void TDESSetKey(uint8_t *Key1,uint8_t *Key2,uint8_t *Key3);
void TDESIntWakeupCmd(unsigned char Cmd);
void TDESIntCmd(unsigned char Cmd);
void SelectDecEncryption(unsigned int operation);

void TDESConfig(TDEConfigPara_T *conf);

void TDESADDR_RST(void);

uint8_t InputDESData(uint8_t *Indat,uint8_t length);

#endif

/***********************  END OF FILES  ***********************/
