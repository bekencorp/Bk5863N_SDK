#ifndef _UART_H_
#define _UART_H_

#include "type.h"
#include "BK5863_reg.h"
#include "BK5863Reg.h"



/* UARTx */
enum{
	UART1 = 0,
	UART2 = 1,
};
/* UART Verify */
enum{
	_Verify_NONE = 0,
	_Verify_ODD = 1,  
	_Verify_EVEN = 2,
};
/* UART Data Len */
enum{
	_UART_5Bits = 0,
	_UART_6Bits = 1,  
	_UART_7Bits = 2,
	_UART_8Bits = 3
};
/* int_enable Reg4 */
enum{
	_TX_FIFO_NEED_WRITE_MASK = (1<<0),
	_RX_FIFO_NEED_READ_MASK = (1<<1),
	_RX_FIFO_OVER_FLOW_MASK = (1<<2),
	_UART_RX_PARITY_ERR_MASK = (1<<3),
	_UART_RX_STOP_ERR_MASK = (1<<4),
	_UART_TX_STOP_END_MASK = (1<<5),
	_UART_RX_STOP_END_MASK = (1<<6),
	_UART_RXD_WAKEUP_MASK = (1<<7)
};
/* int_status Reg5 */
enum{
	_TX_FIFO_NEED_WRITE = (1<<0),
	_RX_FIFO_NEED_READ = (1<<1),
	_RX_FIFO_OVER_FLOW = (1<<2),
	_UART_RX_PARITY_ERROR = (1<<3),
	_UART_RX_STOP_ERROR = (1<<4),
	_UART_TX_STOP_END = (1<<5),
	_UART_RX_STOP_END = (1<<6),
	_UART_RXD_WAKEUP = (1<<7)
};

/* UARTx FIFO Status */
enum{
	
	_TX_FIFO_FULL = (1<<16),
	_TX_FIFO_EMPTY = (1<<17),
	_RX_FIFO_FULL = (1<<18),
	_RX_FIFO_EMPTY = (1<<19),
	_FIFO_WR_READY = (1<<20),
	_FIFO_RD_READY = (1<<21)
};



#define REG_UART_CONFIG                      (BK5863_UART1_BASE_ADDR + 4 * 0)
#define UART_TX_ENABLE                         (1 << 0)
#define UART_RX_ENABLE                         (1 << 1)
#define UART_IRDA                              (1 << 2)
#define UART_DATA_LEN_POSI                     (3)
#define UART_DATA_LEN_MASK                     (0x03)
#define UART_PAR_EN                            (1 << 5)
#define UART_PAR_ODD_MODE                      (1 << 6)
#define UART_STOP_LEN_2                        (1 << 7)
#define UART_CLK_DIVID_POSI                    (8)
#define UART_CLK_DIVID_MASK                    (0x1FFF)

#define REG_UART_FIFO_CONFIG                 (BK5863_UART1_BASE_ADDR + 4 * 1)
#define TX_FIFO_THRESHOLD_MASK                 (0xFF)
#define TX_FIFO_THRESHOLD_POSI                 (0)
#define RX_FIFO_THRESHOLD_MASK                 (0xFF)
#define RX_FIFO_THRESHOLD_POSI                 (8)
#define RX_STOP_DETECT_TIME_MASK               (0x03)
#define RX_STOP_DETECT_TIME_POSI               (16)
#define RX_STOP_DETECT_TIME32                  (0)
#define RX_STOP_DETECT_TIME64                  (1)
#define RX_STOP_DETECT_TIME128                 (2)
#define RX_STOP_DETECT_TIME256                 (3)

#define REG_UART_FIFO_STATUS                 (BK5863_UART1_BASE_ADDR + 4 * 2)
#define TX_FIFO_COUNT_MASK                     (0xFF)
#define TX_FIFO_COUNT_POSI                     (0)
#define RX_FIFO_COUNT_MASK                     (0xFF)
#define RX_FIFO_COUNT_POSI                     (8)
#define TX_FIFO_FULL                           (1 << 16)
#define TX_FIFO_EMPTY                          (1 << 17)
#define RX_FIFO_FULL                           (1 << 18)
#define RX_FIFO_EMPTY                          (1 << 19)
#define FIFO_WR_READY                          (1 << 20)
#define FIFO_RD_READY                          (1 << 21)

#define REG_UART_FIFO_PORT                   (BK5863_UART1_BASE_ADDR + 4 * 3)
#define UART_TX_FIFO_DIN_MASK                  (0xFF)
#define UART_TX_FIFO_DIN_POSI                  (0)
#define UART_RX_FIFO_DOUT_MASK                 (0xFF)
#define UART_RX_FIFO_DOUT_POSI                 (8)

#define REG_UART_INTR_ENABLE                 (BK5863_UART1_BASE_ADDR + 4 * 4)
#define RX_FIFO_NEED_WRITE_EN                  (1 << 0)
#define RX_FIFO_NEED_READ_EN                   (1 << 1)
#define RX_FIFO_OVER_FLOW_EN                   (1 << 2)
#define UART_RX_PARITY_ERR_EN                  (1 << 3)
#define UART_RX_STOP_ERR_EN                    (1 << 4)
#define UART_TX_STOP_END_EN                    (1 << 5)
#define UART_RX_STOP_END_EN                    (1 << 6)
#define UART_RXD_WAKEUP_EN                     (1 << 7)

#define REG_UART_INTR_STATUS                 (BK5863_UART1_BASE_ADDR + 4 * 5)
#define RX_FIFO_NEED_WRITE_STA                  (1 << 0)
#define RX_FIFO_NEED_READ_STA                   (1 << 1)
#define RX_FIFO_OVER_FLOW_STA                   (1 << 2)
#define UART_RX_PARITY_ERR_STA                  (1 << 3)
#define UART_RX_STOP_ERR_STA                    (1 << 4)
#define UART_TX_STOP_END_STA                    (1 << 5)
#define UART_RX_STOP_END_STA                    (1 << 6)
#define UART_RXD_WAKEUP_STA                     (1 << 7)

#define REG_UART_FLOW_CONFIG                 (BK5863_UART1_BASE_ADDR + 4 * 6)
#define FLOW_CTRL_LOW_CNT_MASK                   (0xFF)
#define FLOW_CTRL_LOW_CNT_POSI                   (0)
#define FLOW_CTRL_HIGH_CNT_MASK                  (0xFF)
#define FLOW_CTRL_HIGH_CNT_POSI                  (8)
#define FLOW_CONTROL_EN                          (1 << 16)

#define REG_UART_WAKE_CONFIG                 (BK5863_UART1_BASE_ADDR + 4 * 7)
#define UART_WAKE_COUNT_MASK                   (0x3FF)
#define UART_WAKE_COUNT_POSI                   (0)
#define UART_TXD_WAIT_CNT_MASK                 (0x3FF)
#define UART_TXD_WAIT_CNT_POSI                 (10)
#define UART_RXD_WAKE_EN                       (1 << 20)
#define UART_TXD_WAKE_EN                       (1 << 21)
#define RXD_NEGEDGE_WAKE_EN                    (1 << 22)

#define UART_TX_WRITE_READY  (REG_READ(REG_UART_FIFO_STATUS) & FIFO_WR_READY)

#define UART_WRITE_BYTE(v)               do                                   \
										{                                     \
											v = (v & UART_TX_FIFO_DIN_MASK)   \
													<< UART_TX_FIFO_DIN_POSI; \
											REG_WRITE(REG_UART_FIFO_PORT, v); \
										}while(0)
#define UART_READ_BYTE(v)               do                                    \
										{                                     \
											v = (REG_READ(REG_UART_FIFO_PORT) \
													>> UART_RX_FIFO_DOUT_POSI) \
													& UART_RX_FIFO_DOUT_MASK;\
										}while(0)
										
#define UART_READ_BYTE_DISCARD()       do                                    \
										{                                     \
											REG_READ(REG_UART_FIFO_PORT);\
										}while(0)

										
typedef struct{
	uint8_t Tx_EN;  //发送使能
	uint8_t RX_EN;  //接收使能
	uint8_t DataLenBit;  //数据长度
	uint16_t CLK_DIV;  //分频
	uint8_t UartVerify;  //校验
	uint8_t UartIRDA;
	uint8_t UartStopBit;  //停止位 0:1bit 1:2bit
	uint8_t TxFIFOThreshold;
	uint8_t RxFIFOThreshold;
	uint8_t RxStopDetectTime;
	
}UARTPara_T;
										
										
										
										

//void uart_fifo_flush(void);

/* UART CLK */
void SetUARTClk(uint8_t UARTx,uint8_t cmd);
/* Set Uart Interrupt */
void SetUARTInterrupt(uint8_t UARTx,uint8_t cmd);
/* Set UART CLK DIV  */
void SetUARTCLKDIV(uint8_t UARTx,uint16_t div);
/* 最小停止间隔时间 */
void UARTStopIntervalTime(uint8_t UARTx,uint8_t xbit);
/* Verify */
void SetUARTVerify(uint8_t UARTx,uint8_t Verify);
/* UART data bit */
void SetUARTDataBit(uint8_t UARTx,uint8_t xbits);
/* Set UART_IRDA */
void SetUART_IRDA(uint8_t UARTx,uint8_t Uart_Irda);

/* 使能收发 */
void SetUARTTxENABLE(uint8_t UARTx,uint8_t cmd);
void SetUARTRxENABLE(uint8_t UARTx,uint8_t cmd);

/* 使能UART 中断 */
void SetUARTIntENABLE(uint8_t UARTx,uint8_t cmd);
/* 设置UART中断源 */
void SetUARTIntSourceENABLE(uint8_t UARTx,uint32_t Intflag,uint8_t cmd);

/* Clean UART Interrupt Flag */
void CleanUARTIntFlag(uint8_t UARTx,uint32_t Intflag);
/* Read UART Interrupt Flag */
uint32_t ReadUARTIntFlag(uint8_t UARTx);

/* Read FIFO Status */

/* 读取是否FIFO有接收的数据(数据为被读空)标志 */
uint32_t UARTxFIFORdReady(uint8_t UARTx);


/* Read One Byte data */
uint8_t UARTxData(uint8_t UARTx);

/* FIFO Set */
void SetUARTxTxFIFOTHRESHOLD(uint8_t UARTx,uint8_t TxFIFOLen);
void SetUARTxRxFIFOTHRESHOLD(uint8_t UARTx,uint8_t RxFIFOLen);
void SetUARTxRxStopDetectTime(uint8_t UARTx,uint8_t dt);

/* UARTx Config */
void UARTxConfig(uint8_t UARTx,UARTPara_T *conf);





#endif



/***********************  END OF FILES  ***********************/
