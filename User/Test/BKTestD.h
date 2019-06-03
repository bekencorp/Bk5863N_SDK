#ifndef _BKTestD_H_
#define _BKTestD_H_

#include "type.h"
#include "BK5863.h"
#include "EEPROM.h"




void driver_uart_write_register(void);
void BK5863Wakeup(void);
void Do_Total_Test(void);

void bk_test_pwm(void);
void BK5863PWMIntService(unsigned char PwmIdx);

void bk_test_adc(unsigned char ch);
void bk_Read_adc_CH2(void);
void BK5863TestSpi(unsigned char SpiIdx, SpiMsg_t *pSpiMsgPara, unsigned int Len);
void BK5863SpiSendStart(unsigned char SpiIdx);
void BK5863NTestI2C(void);
void BK5863TestI2c(unsigned char I2cIdx, I2cMsg_t *pi2cMsgPara);
void BK5863TestCardReader(CardReaderMsg_t *pCardReaderMsg);
char BK5863TestSelectCard(CardReaderMsg_t *pCardReaderMsg);
unsigned char BK5863ModuleParaRead(unsigned char addr, unsigned char *pData);

void BK5863SpiIntService(unsigned char SpiIdx);

void BK5863GPIOIntService(void);

void CRCTest(void);
void LBDTest(void);


extern CardReaderMsg_t CrdRdMsg;

extern unsigned int Temp_data[100];
extern const uint8_t CRCTestData[9];


void MCUSleep(void);
void TEST5863N(unsigned int Cmd,unsigned char *pData);
#ifdef BK5870
void TEST5870(unsigned int Cmd,unsigned char *pData);
#endif

#endif
