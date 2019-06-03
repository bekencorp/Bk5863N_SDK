#include "isoiec7816_intf.h"
//#include "mcu_platform.h"
#include "SysTime.h"

void isoiec7816_delay_us(unsigned int num)
{
	delay_us(num);
}

void isoiec7816_delay_ms(unsigned int num)
{
	delay_ms(num);
}

void isoiec7816_vcc_pin_init(unsigned char cmd)
{ 
	if(cmd) 
	{
		REG_APB7_GPIOD_CFG |= ((1<<2)<<16);
		SetGPIOPin(GPIOD,GPIO_Pin_2);
	}
	else 
	{
		REG_APB7_GPIOD_CFG &= ~((1<<2)<<16);
		ResetGPIOPin(GPIOD,GPIO_Pin_2);
	}
}

void isoiec7816_rst_pin_init(void)
{
	GPIOConfig_T GIPOConfig;

	GIPOConfig.GPIO_PERI = GPIO_General_Function;  
	GIPOConfig.GPIO_OE_N_INTERN = Enable;  
	GIPOConfig.GPIO_PULLUP = Enable;
	GIPOConfig.GPIO_PULLDOWN = Disable;  //����ʹ��
	GIPOConfig.GPIO_I_IE = Disable;      //����ʹ��
	GIPOConfig.Int.isINT = Disable;      //�����ж�
	GIPOConfig.GPIO_Pin = GPIO_Pin_3;
	/* Config */
	GPIO_Config(GPIOD,&GIPOConfig);
	/* Set Low Level */
	ResetGPIOPin(GPIOD,GPIO_Pin_3);
}

void isoiec7816_clk_pin_init(uint8_t Clk,uint8_t cmd)
{
	/* GPIO Config */
	M_XH_CLKOUT_B7_INIT;
	M_XH_CLKOUT_B7_EN;
	/* Clock Set DIV & Disable_Enable */
	REG_APB3_ICU_GPIOB7CLKCON = (REG_APB3_ICU_GPIOB7CLKCON&0xFFFFFFF8)|(Clk&0x07);
	if(cmd)
		REG_APB3_ICU_GPIOB7CLKCON &= (~B7_XH_CLKOUT_EN);  //??
	else
		REG_APB3_ICU_GPIOB7CLKCON |= B7_XH_CLKOUT_EN;
}

void isoiec7816_io_pin_init(unsigned int baud)
{
	UARTPara_T Uart2Conf;
	unsigned int uart_clk_div;
	/* Div */
	uart_clk_div = UART_CLK/baud;
	uart_clk_div -= 1;
	
	/* UART2 Clk */
	SetUARTClk(UART2,Enable);
	/* uart2 2nd func enable */
	REG_APB7_GPIOD_CFG &= 0xffffff3f;							//uart2 2nd func enable

	/* UART2 Config */
	Uart2Conf.Tx_EN = Enable;  //发送使�?
	Uart2Conf.RX_EN = Enable;  //接收使能
	Uart2Conf.DataLenBit = _UART_8Bits;  //数据长度
	Uart2Conf.CLK_DIV = uart_clk_div;  //分频
	Uart2Conf.UartVerify = _Verify_ODD;//_Verify_EVEN;  //校验
	Uart2Conf.UartIRDA = 0;  //UART格式
	Uart2Conf.UartStopBit = 1;  //停止�?0:1bit 1:2bit
	/*发送FIFO小于某数时中断，接收FIFO大于某数时读中断 */
	Uart2Conf.TxFIFOThreshold = 1;  //TxFIFO小于此数时，产生中断
	Uart2Conf.RxFIFOThreshold = 1;  //RxFIFO大于此数时，产生中断
	Uart2Conf.RxStopDetectTime = 0; //判断接收结束  0:32  1:64  2:128  3:256
	/* Uart2 config */
	UARTxConfig(UART2,&Uart2Conf);
	
	/* default */
	REG_APB9_UART2_FLOW_CTRL = 0x00000000;
	REG_APB9_UART2_WAKE_CTRL = 0x00000000;	//default

	/* RX interrupt enable */
	SetUARTIntSourceENABLE(UART2,_RX_FIFO_NEED_READ_MASK,Disable);
	SetUARTIntSourceENABLE(UART2,_UART_RX_STOP_END_MASK,Disable);
	
	/* RX interrupt enable */
	SetUARTIntSourceENABLE(UART2,_TX_FIFO_NEED_WRITE_MASK,Disable);
	SetUARTIntSourceENABLE(UART2,_UART_TX_STOP_END_MASK,Enable);
	/* UART2 Int Enable */
	SetUARTInterrupt(UART2,Disable);
}

void isoiec7816_hardware_intf_init(void)
{
	isoiec7816_vcc_pin_init(0);
	isoiec7816_rst_pin_init();
	isoiec7816_clk_pin_init(B7_XH_DIV_8M,Disable);
	isoiec7816_io_pin_init(ISOIEC7816_DEFAULT_RATE);
	isoiec7816_open_tx();
	isoiec7816_rst_pin_reset();
	isoiec7816_clk_pin_control(0);
}

void isoiec7816_hardware_intf_deinit(void)
{

}

void isoiec7816_hardware_intf_cold_reset(void)
{
	isoiec7816_open_rx();
	isoiec7816_rst_pin_reset();
	isoiec7816_clk_pin_control(1);
	isoiec7816_delay_us(1000);	
	isoiec7816_rst_pin_set();
//	isoiec7816_delay_us(60);	 	 
}

void isoiec7816_hardware_intf_reset(void)
{
	isoiec7816_open_rx();
	isoiec7816_rst_pin_reset();
	isoiec7816_delay_us(100);	
	isoiec7816_rst_pin_set();
	isoiec7816_delay_ms(15);			 
}

int isoiec7816_hardware_intf_send(const unsigned char *send_buf, unsigned short length)
{
	unsigned int i;
	
	isoiec7816_open_tx();
	
	for(i=0;i<length; i++)
	{
		/* Tx FIFO Full */
    	//while(!(REG_APB9_UART2_FIFO_COUNT&(1<<20))){}
		while(!((REG_APB9_UART2_FIFO_COUNT&(FIFO_WR_READY))&&(!((TX_FIFO_FULL) & REG_APB9_UART2_FIFO_COUNT)))){}
		REG_APB9_UART2_DATA = send_buf[i];
	}
	while(!(REG_APB9_UART2_INT_FLAG&(1<<5)));
	REG_APB9_UART2_INT_FLAG |= (1<<5);
	return	1;
}

int isoiec7816_hardware_intf_receive(unsigned char *rec_buf,const unsigned char expect_len,unsigned char *rec_len)
{
	unsigned char rec;

	_isoiec7816.rec_timerout = ISOIEC7816_RECEIVE_TIMEOUT;
	(*rec_len) = 0;

	isoiec7816_open_rx();  //����ģʽ
	do
	{
	
		if(REG_APB9_UART2_FIFO_COUNT&(1<<21))
		{
			rec = (REG_APB9_UART2_DATA>>8);
			rec_buf[(*rec_len)] = rec;
			(*rec_len)++;
			_isoiec7816.rec_timerout = ISOIEC7816_RECEIVE_TIMEOUT;
			if((*rec_len) >= expect_len) 
			{
				break;
			}			
		}
		
		if((*rec_len) >= expect_len) 
		{
			break;
		}
	}while(_isoiec7816.rec_timerout);

	if(!_isoiec7816.rec_timerout)
	{		
		return 0;
	}
	return 1;
}

void isoiec7816_io_rate_set(unsigned int baud)
{
//	isoiec7816_tx_disable();
//	isoiec7816_rx_disable();
//	SetUARTClk(UART2,Disable);
	isoiec7816_io_pin_init(baud);
	_isoiec7816.tx_st = 0;
	isoiec7816_open_tx();
}



/***********************  END OF FILES  ***********************/

