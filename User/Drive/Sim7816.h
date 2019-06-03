#ifndef _Sim7816_H_
#define _Sim7816_H_

#include "BK5863.h"

/* Simulation port 7816 RST */
#define RST_Set() SetGPIOPin(GPIOD,GPIO_Pin_3)
#define RST_Reset() ResetGPIOPin(GPIOD,GPIO_Pin_3)

  /* Simulation port 7816 IO (UART) */
#define	UART2_TX_ENABLE	  GPIO_SetSF(GPIO3,BIT(6))  //Tx		
#define	UART2_TX_DISABLE	GPIO_InputSetup(GPIO3,BIT(6),0,0)
#define	UART2_RX_ENABLE		GPIO_SetSF(GPIO3,BIT(7))
#define	UART2_RX_DISABLE	GPIO_InputSetup(GPIO3,BIT(7),0,0)
#define	Port7816_TX_ENABLE			REG_APB9_UART2_CFG |=0x00000001
#define	Port7816_TX_DISABLE			REG_APB9_UART2_CFG &=~0x00000001
#define	Port7816_RX_ENABLE			REG_APB9_UART2_CFG |=0x00000002
#define	Port7816_RX_DISABLE			REG_APB9_UART2_CFG &=~0x00000002

/* UART接收等待超时 */
#define	UART_TIME_OUT		30


typedef struct{
	unsigned char	g_uart2_rx_flag;  //simulation 7816 IO flag
	unsigned char	g_uart2_tx_flag;  //simulation 7816 IO flag
	uint16_t rec_timerout;
}SimPort7816_T;

void Port7816_clock_Cmd(int cmd);
void Port7816Init(void);
void Port7816RSTInit(void);

void Port7816IOInit(uint32_t baud);
void SimulationPort7816IOService(void);

void uart2_open_rx(void);
void uart2_open_tx(void);
void UART2RxFIFOReset(void);

uint8_t uart_receive(uint8_t *rec_buf,const uint8_t expect_len,uint8_t *rec_len);
uint8_t uart_send(uint8_t *send_buf,const uint16_t length);

void do_esam_Cold_reset(void);
void do_esam_reset(void);
void Port7816RSTSystemSleepConfig(void);
void Port7816IOSystemSleepConfig(void);

void SIM7816UARTTimerOut(void);





extern SimPort7816_T SimP7816;

#endif

/***********************  END OF FILES  ***********************/
