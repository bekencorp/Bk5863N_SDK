#ifndef _UARTConfig_H_
#define _UARTConfig_H_

#include "BK5863.h"

#define Uart1RxBuffLength 32
#define Uart1TxBuffLength 16

typedef struct BKUART_CONTROL_STRU_internal
{
	unsigned char	*pTx;
	unsigned int 	txBufLen;
	unsigned int 	txIndex;	
	unsigned char	*pRx;
	unsigned int 	rxBufLen;
	unsigned int 	rxWrIndex;
	unsigned int	rxRdIndex;	
	unsigned int	active;							//active flag
	unsigned int 	busy;
}BKUART_CONTROL_STRU;

void BK5863UartInit(unsigned int index, unsigned int speed, unsigned char* pRx, unsigned int rxLen);
void uart_send_byte(uint8_t data);
void BK5863UartPrint(unsigned int index, unsigned char* pTx, unsigned int txLen);
void BK5863Uart1IntService(void);

void UART1Config(uint32_t baud,unsigned char* pRx,unsigned int rxLen);
void UART2Config(uint32_t baud,unsigned char* pRx,unsigned int rxLen);
void UART2Service(void);	


extern BKUART_CONTROL_STRU BKUART_CTL_BLK[2];
extern unsigned char  Uart1RxBuff[Uart1RxBuffLength];
extern unsigned char  Uart1TxBuff[Uart1TxBuffLength];

#endif /*_UARTConfig_H_*/

/***********************  END OF FILES  ***********************/
