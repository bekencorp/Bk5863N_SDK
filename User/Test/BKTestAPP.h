#ifndef _BKTestAPP_H_
#define _BKTestAPP_H_

#include "BK5863.h"
#include "type.h"
#include "BKCard.h"
#include "BKCardTest.h"
#include "BKTestD.h"
#include "BK5824Test.h"
#include "BKTestCom.h"
#include "BKDACTest.h"
#define UART_OUTPUT_CHAR(a)                             \
              do{                                       \
                    Uart1TxBuff[TestInfo.Uart1TxLen++] = (a);      \
                }while(0)
							

typedef struct{
	unsigned char uart_state;
	unsigned char uart_op_cmd;
	unsigned char addr_data_length;
	unsigned char CardType;
	unsigned char TxSpeed;
	unsigned char RxSpeed;
	unsigned char PiccCid;
	unsigned char KuaiN;
	unsigned char BK5823BerEn;
	unsigned char I2cIdx;
	unsigned char I2cRole;
	unsigned char I2cRwMode;
	unsigned char I2cLocalAddr;
	unsigned char I2cRemoteAddr;
	unsigned char StandbyMode, SectorMode;
	unsigned char SpiIdx, SpiNssmd, SpiCkphapol;
	unsigned char SpiRole, SpiCkr;
	unsigned char PassWd[6];
	unsigned char Write_Data[16];
	unsigned char  reg_data[16];
	unsigned char BK5824RecTest;  //¥¶¿ÌBK5824Irq÷–∂œ
	unsigned short int I2cClkdiv;
	unsigned short int PcdFrameDly;
	unsigned short int PcdReqGuardTime;
	unsigned short int PcdSFGIDly;
	unsigned short int test_item;
	unsigned int reg_value;
	unsigned int reg_addr;
	unsigned int  Uart1TxLen;
	unsigned int  PcdOprtNum;
	unsigned int  SpiDataLen;
	unsigned int  I2cDataLen;
	unsigned int cardreader_init;
}TestPara_T,TestPara_t;


void TestInit(void);
void TestConfig(void);
void application_uart_command_analysis(void);
void driver_uart_link_check(void);
void driver_uart_read_register(void);
void BK5863ModuleParaUpdate(unsigned char addr, unsigned char *pData);



extern TestPara_T TestInfo;



extern const unsigned char write_Data[16];
extern const unsigned char passWd[6];




#endif
