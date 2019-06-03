#ifndef _BK5824D_H_
#define _BK5824D_H_

#include "type.h"

#define BK5824_DEFALUT_LOG                 1



#undef  BK5824_REG_LEN_OPTION
#undef  BK5824_REG_LEN_OPTION_PREV
#undef  BK5824_NEW_REG_LEN_OPTION

#define BK5824_REG_LEN_OPTION_PREV         (0)
#define BK5824_NEW_REG_LEN_OPTION          (1)


#define BK5824_REG_LEN_OPTION              BK5824_REG_LEN_OPTION_PREV
//#define BK5824_REG_LEN_OPTION              BK5824_NEW_REG_LEN_OPTION

/* 5824 FREQ */
typedef enum{
 FREQ_5790		=			0,
 FREQ_5800		=			1,
 FREQ_5830		=			2,
 FREQ_5840		=			3,
 FREQ_5835		=			4,
 FREQ_5795		=			5,
 FREQ_5805,
}RF58_FREQ_E;

#ifdef BK5870
typedef enum{
	BK5824_INT_AUD_NULL,
	BK5824_INT_AUD_DIS = 2,
	BK5824_INT_AUD_EN = 3,
	BK5824_INT_AUD_EN_PP = 4,
}BK5824_INT_AUD_CTL;
#endif
/* Read REG6寄存器位域定义 */
/* bit0~7 */
#define BK5824RbufRdy             (1<<7)
#define BK5824TbufRdy             (1<<5)
#define Bk5824RxErrIrt           	(1<<2)  
#define Bk5824RxEndIrt           	(1<<1) 
#define Bk5824TxEndIrt           	(1<<0)   

/* REG15寄存器位域定义 */
/* bit31~24*/

/*  */
enum{
	BK58_PARA_ERR = 0, //有中断产生
	BK58Rx_OK = 0,   //接收成功
	BK58Tx_OK = 0,   //发送成功
	BK58Rx_Err = 1,  //接收错误
	BK58Tx_Err = 1,  //发送错误
	BK58Tx_END = 0,  //发送结束
	BK5824OK = 0
};
int set_bk5824_reg_read_len(unsigned char RegAddr,unsigned char len);
int set_bk5824_reg_write_len(unsigned char RegAddr,unsigned char len);
int get_bk5824_reg_write_len(unsigned char RegAddr);
int get_bk5824_reg_read_len(unsigned char RegAddr);



void BK5824SpiRead(unsigned char RegAddr, unsigned char *pData);
void BK5824SpiWrite(unsigned char RegAddr, unsigned char *pData);
void BKDelay100uS(unsigned int time);


void BK5824_0Wakeup(uint8_t  status);
/* BK5824 Function */
void BK5824EnterSleep(void);
void BK5824PowerUp(void);
void BK5824Power(unsigned char cmd);
void BK5824_wuCal_toggle(void);
void BK5824_SoftWakeup_toggle(void);
void bk5824_crc_seed(int ops);
void bk5824_crc_en(int ops);

uint8_t Bk5824_IRQ_malloc(uint8_t **p_recv,uint8_t *p_recvlen);
uint8_t Bk5824IRQdeal(uint8_t *p_recv,uint8_t *p_recvlen);


void BK5824ClearIrq(uint8_t flag);
void Bk5824_RX(uint8_t	Reg15bit1);
void BK5824ReadData(uint8_t *RecDat,uint8_t *length);
void BK5824SendData(uint8_t *RecDat,uint8_t length);
int bk5824_send_data_safety(unsigned char *msg,unsigned char len);
void BK5824_reset(void);
void bk5824_set_tx_freq(RF58_FREQ_E freq);
void bk5824_set_rx_freq(RF58_FREQ_E freq);
void bk5824_set_rf_power(unsigned char power);

#ifdef BK5870
int bk5824_internal_audion_ctrl(BK5824_INT_AUD_CTL ops);
#endif

void set_bk5824_wko_weakup(int ops);
void set_bk5824_irq_weakup(int ops);

void set_rf58_tx_busy_state(int ops);
int get_rf58_tx_busy_state(void);


unsigned int get_lo_value_frequency(unsigned int value);
unsigned int get_lo_frequency_value(unsigned int freq);
float get_lo_frequency_vco(unsigned int freq);
unsigned int get_lo_frequency_Ncal(unsigned int freq);



#if BK5824_REG_LEN_OPTION == BK5824_REG_LEN_OPTION_PREV
extern unsigned char BK5824RegWriteLen[21], BK5824RegReadLen[21]; 
#endif 

#endif







/***********************  END OF FILES  ***********************/
