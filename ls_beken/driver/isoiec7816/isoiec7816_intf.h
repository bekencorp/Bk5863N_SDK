#ifndef _ISOIEC7816_INTF_H_
#define _ISOIEC7816_INTF_H_

#include "isoiec7816.h"
#include "BK5863.h"


#ifndef INLINE
///#define INLINE          __inline
#define INLINE         static    inline
#endif




#define B7_XH_DIV_32M               0x00			//GPIOB7 Clock output divice
#define B7_XH_DIV_16M               0x01
#define B7_XH_DIV_8M                0x02
#define B7_XH_DIV_4M                0x03
#define B7_XH_DIV_2M                0x04
#define B7_XH_DIV_1M                0x05
#define B7_XH_DIV_500K              0x06
#define B7_XH_DIV_250K              0x07


#define	_isoiec7816_tx_io_enable()            GPIO_SetSF(GPIOD,BIT(6))             //Tx		
#define	_isoiec7816_tx_io_disable()           GPIO_InputSetup(GPIOD,BIT(6),0,0)
#define	_isoiec7816_rx_io_enable()            GPIO_SetSF(GPIOD,BIT(7))
#define	_isoiec7816_rx_io_disable()           GPIO_InputSetup(GPIOD,BIT(7),0,0)
#define	isoiec7816_tx_enable()             REG_APB9_UART2_CFG |=0x00000001
#define	isoiec7816_tx_disable()            REG_APB9_UART2_CFG &=~0x00000001
#define	isoiec7816_rx_enable()             REG_APB9_UART2_CFG |=0x00000002
#define	isoiec7816_rx_disable()            REG_APB9_UART2_CFG &=~0x00000002


#define isoiec7816_rst_pin_set()           SetGPIOPin(GPIOD,GPIO_Pin_3)
#define isoiec7816_rst_pin_reset()         ResetGPIOPin(GPIOD,GPIO_Pin_3)
INLINE void isoiec7816_clk_pin_control(int cmd)
{
	SetClockOutputEnable(cmd);
}

INLINE void isoiec7816_rx_fifo_reset(void)
{
	isoiec7816_rx_disable();
	isoiec7816_rx_enable();	
}

INLINE void isoiec7816_open_tx(void)
{	
	if(_isoiec7816.tx_st == 1) 
		return;
	_isoiec7816_tx_io_enable();
	_isoiec7816_rx_io_disable();
//	_isoiec7816_rx_io_enable();
	isoiec7816_rx_fifo_reset();
//	REG_APB7_GPIOD_CFG |= 0x00800080;
//	REG_APB7_GPIOD_DATA |= 0x000080;
	isoiec7816_tx_enable();
	
	isoiec7816_rx_disable();
	_isoiec7816.tx_st = 1;
	_isoiec7816.rx_st = 0;
}

INLINE void isoiec7816_open_rx(void)
{
	if(1 == _isoiec7816.rx_st)	
	{
	//	if(REG_APB9_UART2_FIFO_COUNT&(1<<21))
	//		isoiec7816_rx_fifo_reset();
		return;
	}
	_isoiec7816_tx_io_enable();
//	_isoiec7816_tx_io_disable();
//	REG_APB7_GPIOD_CFG &= (~0x40404040);
//	REG_APB7_GPIOD_DATA &= (~0x400040);
	REG_APB7_GPIOD_CFG |= 0x00400040;
	REG_APB7_GPIOD_DATA |= 0x000040;

	isoiec7816_tx_disable();
	_isoiec7816_rx_io_enable();
	isoiec7816_rx_fifo_reset();
	isoiec7816_rx_enable();	
	_isoiec7816.tx_st = 0;
	_isoiec7816.rx_st = 1;
}

INLINE void isoiec7816_receive_timerout_handler(void)
{
	if(_isoiec7816.rec_timerout)
		_isoiec7816.rec_timerout--;
}

void isoiec7816_hardware_intf_init(void);
void isoiec7816_hardware_intf_deinit(void);
void isoiec7816_hardware_intf_cold_reset(void);
void isoiec7816_hardware_intf_reset(void);
int isoiec7816_hardware_intf_send(const unsigned char *send_buf, unsigned short length);
int isoiec7816_hardware_intf_receive(unsigned char *rec_buf,const unsigned char expect_len,unsigned char *rec_len);
void isoiec7816_io_rate_set(unsigned int baud);



#endif

/***********************  END OF FILES  ***********************/
