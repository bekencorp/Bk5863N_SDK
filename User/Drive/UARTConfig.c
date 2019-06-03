#include "UARTConfig.h"
#include "Includes.h"


BKUART_CONTROL_STRU BKUART_CTL_BLK[2];
unsigned char       Uart1RxBuff[Uart1RxBuffLength];
unsigned char       Uart1TxBuff[Uart1TxBuffLength];
/**************************************************
FunctionName:BK5863UartInit
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
void BK5863UartInit(unsigned int index, unsigned int speed, unsigned char* pRx, unsigned int rxLen)
{
	unsigned int uart_clk_div;
	unsigned int temp;
	uart_clk_div = UART_CLK/speed;
	uart_clk_div -= 1;
	BKUART_CTL_BLK[index].active = 1;
	BKUART_CTL_BLK[index].pRx = pRx;
	BKUART_CTL_BLK[index].rxBufLen = rxLen;
	BKUART_CTL_BLK[index].rxWrIndex = 0;
	BKUART_CTL_BLK[index].rxRdIndex = 0;
	BKUART_CTL_BLK[index].txIndex = 0;
	if(index == BK5863_UART1)
	{
		REG_APB3_ICU_UART1CLKCON = 0;  						  	//UART1 clock enable
		REG_APB7_GPIOD_CFG &= 0xffffffcf;							//uart1 2nd func enable
		temp = 0;
		temp = (1<<UART_TX_EN_BIT)|(1<<UART_RX_EN_BIT);
		temp |= (UART_FRAME_LEN_8_BIT<<UART_DATA_LEN_BIT);  //数据长度
		temp |= (uart_clk_div<<UART_CLK_DIV_BIT);  //时钟分频
		REG_APB8_UART1_CFG = temp;
    
		/*发送FIFO小于某数时中断，接收FIFO大于某数时读中断 */
		REG_APB8_UART1_FIFO_THRESHOLD = (1<<0)|(1<<8);
		REG_APB8_UART1_FLOW_CTRL = 0x00000000;
		REG_APB8_UART1_WAKE_CTRL = 0x00000000;	//default

		/* RX interrupt enable */
//		UART_RX_INT_EN;		
		SetUARTIntSourceENABLE(UART1,_RX_FIFO_NEED_READ_MASK,Enable);
		SetUARTIntSourceENABLE(UART1,_UART_RX_STOP_END_MASK,Enable);
		/* RX interrupt enable */
//		UART_TX_INT_DISENABLE;
		SetUARTIntSourceENABLE(UART1,_TX_FIFO_NEED_WRITE_MASK,Disable);
		SetUARTIntSourceENABLE(UART1,_UART_TX_STOP_END_MASK,Disable);
		REG_APB3_ICU_INT_ENABLE |= INT_STATUS_UART1_bit;//interrupt enable
	}
	else
	{
		REG_APB3_ICU_UART2CLKCON = 0;  								//UART2 clock enable
		REG_APB7_GPIOD_CFG &= 0xffffff3f;							//uart1 2nd func enable
		temp = 0;
		temp = (1<<UART_TX_EN_BIT)|(1<<UART_RX_EN_BIT);
		temp |= (UART_FRAME_LEN_8_BIT<<UART_DATA_LEN_BIT);  //数据长度
		temp |= (uart_clk_div<<UART_CLK_DIV_BIT);  //时钟分频
		REG_APB9_UART2_CFG = temp;
		REG_APB9_UART2_FIFO_THRESHOLD = (1<<0)|(1<<8);
		UART2_RX_INT_EN;											//RX interrupt enable
		UART2_TX_INT_DISENABLE;
		REG_APB3_ICU_INT_ENABLE |= INT_STATUS_UART2_bit;			//interrupt enable
		REG_APB9_UART2_WAKE_CTRL = 0x3fffff;						//default
	}
}
/**************************************************
FunctionName:BK5863UartPrint
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
void BK5863UartPrint(unsigned int index, unsigned char* pTx, unsigned int txLen)
{
	if(BKUART_CTL_BLK[index].active == 0)
		return;
	BKUART_CTL_BLK[index].pTx = pTx;
	BKUART_CTL_BLK[index].txBufLen = txLen;

	if(index == BK5863_UART1)
	{
		UART_TX_INT_DISENABLE;
		for(BKUART_CTL_BLK[index].txIndex = 0; BKUART_CTL_BLK[index].txIndex < MIN(UART_FIFO_DEPTH,txLen); )
		{
			REG_APB8_UART1_DATA = pTx[BKUART_CTL_BLK[index].txIndex++];
		}
		UART_TX_INT_EN;
	}
	else
	{
		UART2_TX_INT_DISENABLE;
		for(BKUART_CTL_BLK[index].txIndex = 0; BKUART_CTL_BLK[index].txIndex < MIN(UART_FIFO_DEPTH,txLen); )
		{
			REG_APB9_UART2_DATA = pTx[BKUART_CTL_BLK[index].txIndex++];
		}
		UART2_TX_INT_EN;
	}

	while(BKUART_CTL_BLK[index].txIndex != BKUART_CTL_BLK[index].txBufLen)
	{
		delay_us(5);
	}
}
/**************************************************
FunctionName:BK5863Uart1IntService
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
void BK5863Uart1IntService(void)
{
	unsigned int uart_status = REG_APB8_UART1_INT_FLAG;
  unsigned int uart_int_Ena = REG_APB8_UART1_INT_ENABLE;
	
	if((uart_status&uart_int_Ena)&(1<<UART_TX_INT_BIT))
	{
		if(BKUART_CTL_BLK[0].txIndex < BKUART_CTL_BLK[0].txBufLen)
      REG_APB8_UART1_DATA = BKUART_CTL_BLK[0].pTx[BKUART_CTL_BLK[0].txIndex++];
		else
      UART_TX_INT_DISENABLE;
	}
	#if 0
		if((uart_status&uart_int_Ena)&(1<<5))
		{
			
		}
	#endif
	#if 1
		if((uart_status & (0x01 << 6) )|| (uart_status & (1<<UART_RX_INT_BIT)))
		{
			while(REG_APB8_UART1_FIFO_COUNT & (0x1 << 21))
			{
				BKUART_CTL_BLK[0].pRx[BKUART_CTL_BLK[0].rxWrIndex] = (REG_APB8_UART1_DATA>>8);	
        BKUART_CTL_BLK[0].rxWrIndex++;				
				if(BKUART_CTL_BLK[0].rxWrIndex >= BKUART_CTL_BLK[0].rxBufLen)
					 BKUART_CTL_BLK[0].rxWrIndex = 0;	
			}
		}
	#else
		if(uart_status&(1<<UART_RX_INT_BIT))
		{
			BKUART_CTL_BLK[0].pRx[BKUART_CTL_BLK[0].rxWrIndex++] = (REG_APB8_UART1_DATA>>8);
			if(BKUART_CTL_BLK[0].rxWrIndex >= BKUART_CTL_BLK[0].rxBufLen)
				BKUART_CTL_BLK[0].rxWrIndex = 0;
		}
	#endif
	REG_APB8_UART1_INT_FLAG = uart_status;
}
/**************************************************
FunctionName:
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
#if 0
/* UART1 */
void uart_send_byte(uint8_t data) 
{
    while(!UART_TX_WRITE_READY);
	
    UART_WRITE_BYTE(data);
}
#endif

/**************************************************
FunctionName: UART1Config
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
void UART1Config(uint32_t baud,unsigned char* pRx,unsigned int rxLen)
{
	UARTPara_T Uart1Conf;
  unsigned int uart_clk_div;
	/* Div */
	uart_clk_div = UART_CLK/baud;
	uart_clk_div -= 1;
	#if 1  //
		BKUART_CTL_BLK[BK5863_UART1].active = 1;
		BKUART_CTL_BLK[BK5863_UART1].pRx = pRx;
		BKUART_CTL_BLK[BK5863_UART1].rxBufLen = rxLen;
		BKUART_CTL_BLK[BK5863_UART1].rxWrIndex = 0;
		BKUART_CTL_BLK[BK5863_UART1].rxRdIndex = 0;
		BKUART_CTL_BLK[BK5863_UART1].txIndex = 0;
	#endif
		/* UART1 Clk */
	SetUARTClk(UART1,Enable);
	/* uart1 2nd func enable */
	REG_APB7_GPIOD_CFG &= 0xffffffcf;	
		/* UART1 Config */
	Uart1Conf.Tx_EN = Enable;  //发送使能
	Uart1Conf.RX_EN = Enable;  //接收使能
	Uart1Conf.DataLenBit = _UART_8Bits;  //数据长度
	Uart1Conf.CLK_DIV = uart_clk_div;  //分频
	Uart1Conf.UartVerify = _Verify_NONE;  //校验
	Uart1Conf.UartIRDA = 0;  //UART格式
	Uart1Conf.UartStopBit = 0;  //停止位 0:1bit 1:2bit
	/*发送FIFO小于某数时中断，接收FIFO大于某数时读中断 */
	Uart1Conf.TxFIFOThreshold = 1;  //TxFIFO小于此数时，产生中断
	Uart1Conf.RxFIFOThreshold = 4;  //RxFIFO大于此数时，产生中断
	Uart1Conf.RxStopDetectTime = 0; //判断接收结束  0:32  1:64  2:128  3:256
	/* Uart1 config */
	UARTxConfig(UART1,&Uart1Conf);
		/* default */
	REG_APB8_UART1_FLOW_CTRL = 0x00000000;
	REG_APB8_UART1_WAKE_CTRL = 0x00000000;	//default
	/* RX interrupt enable */
//		UART_RX_INT_EN;		
	SetUARTIntSourceENABLE(UART1,_RX_FIFO_NEED_READ_MASK,Enable);
	SetUARTIntSourceENABLE(UART1,_UART_RX_STOP_END_MASK,Enable);
	/* RX interrupt enable */
//		UART_TX_INT_DISENABLE;
	SetUARTIntSourceENABLE(UART1,_TX_FIFO_NEED_WRITE_MASK,Disable);
	SetUARTIntSourceENABLE(UART1,_UART_TX_STOP_END_MASK,Disable);
		/* UART1 Int Enable */
	SetUARTInterrupt(UART1,Enable);
}

/**************************************************
FunctionName: UART2Config
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
void UART2Config(uint32_t baud,unsigned char* pRx,unsigned int rxLen)
{
	UARTPara_T Uart2Conf;
  unsigned int uart_clk_div;
	/* Div */
	uart_clk_div = UART_CLK/baud;
	uart_clk_div -= 1;
	#if 1  //
		BKUART_CTL_BLK[BK5863_UART2].active = 1;
		BKUART_CTL_BLK[BK5863_UART2].pRx = pRx;
		BKUART_CTL_BLK[BK5863_UART2].rxBufLen = rxLen;
		BKUART_CTL_BLK[BK5863_UART2].rxWrIndex = 0;
		BKUART_CTL_BLK[BK5863_UART2].rxRdIndex = 0;
		BKUART_CTL_BLK[BK5863_UART2].txIndex = 0;
	#endif
		/* UART2 Clk */
	SetUARTClk(UART2,Enable);
	/* uart2 2nd func enable */
	REG_APB7_GPIOD_CFG &= 0xffffff3f;							//uart2 2nd func enable
		/* UART2 Config */
	Uart2Conf.Tx_EN = Enable;  //发送使能
	Uart2Conf.RX_EN = Enable;  //接收使能
	Uart2Conf.DataLenBit = _UART_8Bits;  //数据长度
	Uart2Conf.CLK_DIV = uart_clk_div;  //分频
	Uart2Conf.UartVerify = _Verify_NONE;  //校验
	Uart2Conf.UartIRDA = 0;  //UART格式
	Uart2Conf.UartStopBit = 0;  //停止位 0:1bit 1:2bit
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
//		UART_RX_INT_EN;		
	SetUARTIntSourceENABLE(UART2,_RX_FIFO_NEED_READ_MASK,Enable);
	SetUARTIntSourceENABLE(UART2,_UART_RX_STOP_END_MASK,Enable);
	/* RX interrupt enable */
//		UART_TX_INT_DISENABLE;
	SetUARTIntSourceENABLE(UART2,_TX_FIFO_NEED_WRITE_MASK,Disable);
	SetUARTIntSourceENABLE(UART2,_UART_TX_STOP_END_MASK,Disable);
		/* UART2 Int Enable */
	SetUARTInterrupt(UART2,Enable);
}

void UART2Service(void)
{
	unsigned int uart_status = REG_APB9_UART2_INT_FLAG;
  unsigned int uart_int_Ena = REG_APB9_UART2_INT_ENABLE;
	
	if((uart_status&uart_int_Ena)&(1<<UART_TX_INT_BIT))
	{
		if(BKUART_CTL_BLK[1].txIndex < BKUART_CTL_BLK[1].txBufLen)
      REG_APB9_UART2_DATA = BKUART_CTL_BLK[1].pTx[BKUART_CTL_BLK[1].txIndex++];
		else
      UART2_TX_INT_DISENABLE;
	}
	#if 0
		if(uart_status&(1<<5))
		{
			
		}
	#endif
	#if 1
		if((uart_status & (0x01 << 6) )|| (uart_status & (1<<UART_RX_INT_BIT)))
		{
			while(REG_APB9_UART2_FIFO_COUNT & (0x1 << 21))
			{
				BKUART_CTL_BLK[1].pRx[BKUART_CTL_BLK[1].rxWrIndex] = (REG_APB9_UART2_DATA>>8);
				BKUART_CTL_BLK[1].rxWrIndex++;
				if(BKUART_CTL_BLK[1].rxWrIndex >= BKUART_CTL_BLK[1].rxBufLen)
					 BKUART_CTL_BLK[1].rxWrIndex = 0;	
			}
		}
	#endif
	REG_APB9_UART2_INT_FLAG = uart_status;
}

/***********************  END OF FILES  ***********************/
