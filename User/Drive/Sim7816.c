#include "Sim7816.h"
#include "Includes.h"
/*************************************************
				Simulation Port 7816
*************************************************/
/*************************************************
				Simulation Port 7816 
*************************************************/
SimPort7816_T SimP7816;
/**************************************************
FunctionName: Port7816_clock_Cmd
Describe: 
InputParameter: cmd 1 Enable
                    0 Disable
OutputParameter: 
Notice:
**************************************************/
void Port7816_clock_Cmd(int cmd)
{
	SetClockOutputEnable(cmd);
}

/**************************************************
FunctionName:  Port7816Init
Describe: 模拟7816接口初始化
InputParameter: 
OutputParameter: 
Notice:
**************************************************/
void Port7816Init(void)
{
	ClockOutputinit(B7_XH_DIV_4M,Disable);
	Port7816RSTInit();
	Port7816IOInit(132129);
//	ClockOutputinit(B7_XH_DIV_2M,Disable);
//	Port7816IOInit(132173/2);
//	Port7816IOInit(115200);
//	RST_Reset();
	Port7816_clock_Cmd(Disable);
}
/**************************************************
FunctionName:  Port7816RSTInit
Describe: 模拟7816接口RST初始化
InputParameter: 
OutputParameter: 
Notice:
**************************************************/
void Port7816RSTInit(void)
{
	GPIOConfig_T GIPOConfig;

	GIPOConfig.GPIO_PERI = GPIO_General_Function;  //普通功能
	GIPOConfig.GPIO_OE_N_INTERN = Enable;  //输出使能
	GIPOConfig.GPIO_PULLUP = Enable;  //上拉使能
	GIPOConfig.GPIO_PULLDOWN = Disable;  //下拉使能
	GIPOConfig.GPIO_I_IE = Disable;  //输入使能
	GIPOConfig.Int.isINT = Disable;    //配置中断
	GIPOConfig.GPIO_Pin = GPIO_Pin_3;
	/* Config */
	GPIO_Config(GPIOD,&GIPOConfig);
	/* Set Low Level */
	ResetGPIOPin(GPIOD,GPIO_Pin_3);
}

/**************************************************
FunctionName:  Port7816RSTSystemSleepConfig
Describe:
InputParameter: 
OutputParameter: 
Notice:
**************************************************/
void Port7816RSTSystemSleepConfig(void)
{
//	GPIOConfig_T GIPOConfig;

//	GIPOConfig.GPIO_PERI = GPIO_General_Function;  //普通功能
//	GIPOConfig.GPIO_OE_N_INTERN = Disable;  //输出使能
//	GIPOConfig.GPIO_PULLUP = Disable;  //上拉使能
//	GIPOConfig.GPIO_PULLDOWN = Disable;  //下拉使能
//	GIPOConfig.GPIO_I_IE = Disable;  //输入使能
//	GIPOConfig.Int.isINT = Disable;    //配置中断
//	GIPOConfig.GPIO_Pin = GPIO_Pin_3;
//	/* Config */
//	GPIO_Config(GPIOD,&GIPOConfig);
	/* Set Low Level */
	SetGPIOPin(GPIOD,GPIO_Pin_3);
}
/**************************************************
FunctionName:  Port7816IOInit
Describe: 模拟7816接口IO初始化(uart2模拟)
InputParameter: 
OutputParameter: 
Notice:
**************************************************/
void Port7816IOInit(uint32_t baud)
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
	Uart2Conf.Tx_EN = Enable;  //发送使能
	Uart2Conf.RX_EN = Enable;  //接收使能
	Uart2Conf.DataLenBit = _UART_8Bits;  //数据长度
	Uart2Conf.CLK_DIV = uart_clk_div;  //分频
	Uart2Conf.UartVerify = _Verify_ODD;//_Verify_EVEN;  //校验
	Uart2Conf.UartIRDA = 0;  //UART格式
	Uart2Conf.UartStopBit = 1;  //停止位 0:1bit 1:2bit
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
	/* Set Init value */
	SimP7816.g_uart2_rx_flag = FALSE;
	SimP7816.g_uart2_tx_flag = FALSE;
}
/**************************************************
FunctionName:  Port7816IOSystemSleepConfig
Describe: 
InputParameter: 
OutputParameter: 
Notice:
**************************************************/
void Port7816IOSystemSleepConfig(void)
{
	Port7816_TX_DISABLE;
	Port7816_RX_DISABLE;
	REG_APB7_GPIOD_CFG &= (~0xC0C0C0C0);
	REG_APB7_GPIOD_DATA &= (~0xC000C0);
	REG_APB7_GPIOD_CFG |= 0x000000C0;
	REG_APB7_GPIOD_DATA |= 0x0000C0;
}
/**************************************************
FunctionName:  SimulationPort7816IOService
Describe: Simulation Port 7816 Service
InputParameter: 
OutputParameter: 
Notice:
**************************************************/
void SimulationPort7816IOService(void)
{
	
}
/**************************************************
FunctionName:  uart2_open_rx
Describe: 
InputParameter: 
OutputParameter: 
Notice:
**************************************************/
void uart2_open_rx(void)
{
	if(TRUE == SimP7816.g_uart2_rx_flag)	
		return;
	UART2_TX_ENABLE;
//	UART2_TX_DISABLE;
//	REG_APB7_GPIOD_CFG &= (~0x40404040);
//	REG_APB7_GPIOD_DATA &= (~0x400040);
	REG_APB7_GPIOD_CFG |= 0x00400040;
	REG_APB7_GPIOD_DATA |= 0x000040;

	Port7816_TX_DISABLE;
	UART2_RX_ENABLE;
	UART2RxFIFOReset();
	Port7816_RX_ENABLE;	
	SimP7816.g_uart2_rx_flag = TRUE;
	SimP7816.g_uart2_tx_flag = FALSE;
}
/**************************************************
FunctionName:  uart2_open_tx
Describe: 
InputParameter: 
OutputParameter: 
Notice:
**************************************************/
void uart2_open_tx(void)
{	
  if(SimP7816.g_uart2_tx_flag == TRUE) 
		return;
	UART2_TX_ENABLE;
	UART2_RX_DISABLE;
//	UART2_RX_ENABLE;
	UART2RxFIFOReset();
//	REG_APB7_GPIOD_CFG |= 0x00800080;
//	REG_APB7_GPIOD_DATA |= 0x000080;
	Port7816_TX_ENABLE;
	
	Port7816_RX_DISABLE;
	SimP7816.g_uart2_rx_flag = FALSE;
	SimP7816.g_uart2_tx_flag = TRUE;
}
/**************************************************
FunctionName:  UART2RxFIFOReset
Describe: 
InputParameter: 
OutputParameter: 
Notice:
**************************************************/
void UART2RxFIFOReset(void)
{
	Port7816_RX_DISABLE;
	Port7816_RX_ENABLE;	
}
/**************************************************
FunctionName:  uart_receive
Describe: 
InputParameter: 
OutputParameter: 
Notice:
**************************************************/
uint8_t uart_receive(uint8_t *rec_buf,const uint8_t expect_len,uint8_t *rec_len)
{
	uint8_t i;

	SimP7816.rec_timerout = UART_TIME_OUT;
	(*rec_len) = 0;
	
  uart2_open_rx();  //接收模式
	do
	{
		if(REG_APB9_UART2_FIFO_COUNT&(1<<21))
		{					
			i = (REG_APB9_UART2_DATA>>8);
			rec_buf[(*rec_len)] = i;
			(*rec_len)++;
			SimP7816.rec_timerout = UART_TIME_OUT;
			if((*rec_len) >= expect_len) 
			{
				break;
			}			
		}		
		if((*rec_len) >= expect_len) 
		{
			break;
		}
	}while(SimP7816.rec_timerout);
	
  if(!SimP7816.rec_timerout)
	{		
		return FALSE;
	}
	return TRUE;
}
/**************************************************
FunctionName:  uart_send
Describe: 
InputParameter: 
OutputParameter: 
Notice:
**************************************************/
uint8_t uart_send(uint8_t *send_buf,const uint16_t length)
{
	uint16_t	i;
	uart2_open_tx();
	
	for(i=0;i<length; i++)
	{
		/* Tx FIFO Full */
    while(!(REG_APB9_UART2_FIFO_COUNT&(1<<20))){}
		REG_APB9_UART2_DATA = send_buf[i];
	}	
	while(!(REG_APB9_UART2_INT_FLAG&(1<<5)));
	REG_APB9_UART2_INT_FLAG |= (1<<5);
	return	TRUE;
}
/**************************************************
FunctionName:  do_esam_Cold_reset
Describe: 
InputParameter: 
OutputParameter: 
Notice:
**************************************************/
void do_esam_Cold_reset(void)
{	 
	uart2_open_rx();
	Port7816_clock_Cmd(Enable);
	delay_us(1000);	
	RST_Set();;
//	delay_us(60);	 	 
}
/**************************************************
FunctionName:  
Describe: 
InputParameter: 
OutputParameter: 
Notice:
**************************************************/
void do_esam_reset(void)
{	
	uart2_open_rx();
	RST_Reset();
	delay_us(100);	
  RST_Set();
	delay_ms(15);			 
}

/**************************************************
FunctionName:  SIM7816UARTTimerOut
Describe:      接收超时执行函数
InputParameter: 
OutputParameter: 
Notice:
**************************************************/
void SIM7816UARTTimerOut(void)
{
	if(SimP7816.rec_timerout)
	  SimP7816.rec_timerout--;
}

/***********************  END OF FILES  ***********************/
