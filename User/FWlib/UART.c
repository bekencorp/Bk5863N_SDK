#include "UART.h"
#include "BK5863.h"

///**************************************************
//FunctionName: uart_fifo_flush
//Describe:
//InputParameter:
//OutputParameter:
//Notice:
//**************************************************/
//void uart_fifo_flush(void)
//{
//	UINT32 val;
//	UINT32 reg;

//	val = REG_READ(REG_UART_CONFIG);
//	reg = val & (~(UART_TX_ENABLE | UART_RX_ENABLE));

//	REG_WRITE(REG_UART_CONFIG, reg);
//	REG_WRITE(REG_UART_CONFIG, val);
//}

/**************************************************
FunctionName:SetUARTClk
Describe:  开关UARTx时钟
InputParameter:
OutputParameter:
Notice:
**************************************************/
void SetUARTClk(uint8_t UARTx,uint8_t cmd)
{
	if(UARTx>UART2) return;
	if(cmd)
  	(*((volatile unsigned long *)(0x00a03024+(4*UARTx)))) &= (~0x01);
	else
		(*((volatile unsigned long *)(0x00a03024+(4*UARTx)))) |= 0x01;
}

/**************************************************
FunctionName: SetUARTInterrupt
Describe: 设置Uart中断
InputParameter:
OutputParameter:
Notice:
**************************************************/
void SetUARTInterrupt(uint8_t UARTx,uint8_t cmd)
{
	if(UARTx == UART1)
	{
		if(cmd)
			REG_APB3_ICU_INT_ENABLE |= INT_STATUS_UART1_bit;//interrupt enable
		else
			REG_APB3_ICU_INT_ENABLE &= (~INT_STATUS_UART1_bit);
  }
	else if(UARTx == UART2)
	{
		if(cmd)
			REG_APB3_ICU_INT_ENABLE |= INT_STATUS_UART2_bit;//interrupt enable
		else
			REG_APB3_ICU_INT_ENABLE &= (~INT_STATUS_UART2_bit);
  }
}
/**************************************************
FunctionName:SetUARTCLKDIV
Describe: UART波特率分频数
InputParameter:
OutputParameter:
Notice:
**************************************************/
void SetUARTCLKDIV(uint8_t UARTx,uint16_t div)
{
	if(UARTx == UART1)
	{
		REG_APB8_UART1_CFG = (REG_APB8_UART1_CFG&(~(0x1FFF<<8)))|((div&0x1FFF)<<8);
	}
	else  if(UARTx == UART2)
	{
		REG_APB9_UART2_CFG = (REG_APB9_UART2_CFG&(~(0x1FFF<<8)))|((div&0x1FFF)<<8);
	}
}
/**************************************************
FunctionName:UARTStopIntervalTime
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
void UARTStopIntervalTime(uint8_t UARTx,uint8_t xbit)
{
	if(UARTx == UART1)
	{
		REG_APB8_UART1_CFG = (REG_APB8_UART1_CFG&(~(0x1<<7)))|((xbit&0x1)<<7);
	}
	else  if(UARTx == UART2)
	{
		REG_APB9_UART2_CFG = (REG_APB9_UART2_CFG&(~(0x1<<7)))|((xbit&0x1)<<7);
	}
}
/**************************************************
FunctionName:SetUARTVerify
Describe:
InputParameter:	_Verify_NONE = 0,
								_Verify_ODD = 1,  
								_Verify_EVEN = 2,
OutputParameter:
Notice:
**************************************************/
void SetUARTVerify(uint8_t UARTx,uint8_t Verify)
{
	if(UARTx == UART1)
	{
		if(Verify == _Verify_NONE)
		  REG_APB8_UART1_CFG = (REG_APB8_UART1_CFG&(~(0x1<<5)));
		else if(Verify == _Verify_ODD)
			REG_APB8_UART1_CFG |= (0x3<<5);
		else if(Verify == _Verify_EVEN)
			REG_APB8_UART1_CFG = (REG_APB8_UART1_CFG&(~(0x1<<6)))|(1<<5);
	}
	else  if(UARTx == UART2)
	{
		if(Verify == _Verify_NONE)
		  REG_APB9_UART2_CFG = (REG_APB9_UART2_CFG&(~(0x1<<5)));
		else if(Verify == _Verify_ODD)
			REG_APB9_UART2_CFG |= (0x3<<5);
		else if(Verify == _Verify_EVEN)
			REG_APB9_UART2_CFG = (REG_APB9_UART2_CFG&(~(0x1<<6)))|(1<<5);
	}
}
/**************************************************
FunctionName:SetUARTDataBit
Describe:
InputParameter:	_UART_5Bits = 0,
								_UART_6Bits = 1,  
								_UART_7Bits = 2,
								_UART_8Bits = 3
OutputParameter:
Notice:
**************************************************/
void SetUARTDataBit(uint8_t UARTx,uint8_t xbits)
{
	if(UARTx == UART1)
	{
		REG_APB8_UART1_CFG = (REG_APB8_UART1_CFG&(~(0x3<<3)))|((xbits&0x03)<<3);
	}
	else  if(UARTx == UART2)
	{
		REG_APB9_UART2_CFG = (REG_APB9_UART2_CFG&(~(0x3<<3)))|((xbits&0x03)<<3);
	}
}
/**************************************************
FunctionName: SetUART_IRDA
Describe:
InputParameter:Uart_Irda ::={ 
                              0:UART帧格式 
                              1:IRDA模式
                            }
OutputParameter:
Notice:
**************************************************/
void SetUART_IRDA(uint8_t UARTx,uint8_t Uart_Irda)
{
	if(UARTx == UART1)
	{
		REG_APB8_UART1_CFG = (REG_APB8_UART1_CFG&(~(0x1<<2)))|((Uart_Irda&0x01)<<2);
	}
	else  if(UARTx == UART2)
	{
		REG_APB9_UART2_CFG = (REG_APB9_UART2_CFG&(~(0x1<<2)))|((Uart_Irda&0x01)<<2);
	}
}
/**************************************************
FunctionName:SetUARTTxENABLE
Describe:
InputParameter:  Enable 1
                 Disable 0
OutputParameter:
Notice:
**************************************************/
void SetUARTTxENABLE(uint8_t UARTx,uint8_t cmd)
{
	if(UARTx == UART1)
	{
		REG_APB8_UART1_CFG = (REG_APB8_UART1_CFG&(~(0x1<<0)))|((cmd&0x01)<<0);
	}
	else  if(UARTx == UART2)
	{
		REG_APB9_UART2_CFG = (REG_APB9_UART2_CFG&(~(0x1<<0)))|((cmd&0x01)<<0);
	}
}

/**************************************************
FunctionName:  SetUARTRxENABLE
Describe:
InputParameter:  Enable 1
                 Disable 0
OutputParameter:
Notice:
**************************************************/
void SetUARTRxENABLE(uint8_t UARTx,uint8_t cmd)
{
	if(UARTx == UART1)
	{
		REG_APB8_UART1_CFG = (REG_APB8_UART1_CFG&(~(0x1<<1)))|((cmd&0x01)<<1);
	}
	else  if(UARTx == UART2)
	{
		REG_APB9_UART2_CFG = (REG_APB9_UART2_CFG&(~(0x1<<1)))|((cmd&0x01)<<1);
	}
}
/**************************************************
FunctionName:  SetUARTRxENABLE
Describe:
InputParameter:  Enable 1
                 Disable 0
OutputParameter:
Notice:
**************************************************/
void SetUARTIntENABLE(uint8_t UARTx,uint8_t cmd)
{
	if(cmd > 1) cmd = 1;
	if(UARTx == UART1)
	{
		REG_APB3_ICU_INT_ENABLE = (REG_APB3_ICU_INT_ENABLE&(~(1<<6)))|((cmd&0x01)<<6);//interrupt enable
	}
	else  if(UARTx == UART2)
	{
		REG_APB3_ICU_INT_ENABLE = (REG_APB3_ICU_INT_ENABLE&(~(1<<7)))|((cmd&0x01)<<7);
	}
}
/**************************************************
FunctionName:SetUARTIntSourceENABLE
Describe: 设置UART中断方式(如接收停止使能),
InputParameter:	UARTx ::={UART1,UART2}
								Intflag::={
											_TX_FIFO_NEED_WRITE_MASK = 1,
											_RX_FIFO_NEED_READ_MASK = 2,
											_RX_FIFO_OVER_FLOW_MASK = 4,
											_UART_RX_PARITY_ERR_MASK = 8,
											_UART_RX_STOP_ERR_MASK = 0x10,
											_UART_TX_STOP_END_MASK = 0x20,
											_UART_RX_STOP_END_MASK = 0x40,
											_UART_RXD_WAKEUP_MASK = 0x80
								}
								cmd ::={Enable,Disable}
OutputParameter:
Notice:
**************************************************/
void SetUARTIntSourceENABLE(uint8_t UARTx,uint32_t Intflag,uint8_t cmd)
{
	if(UARTx == UART1)
	{
		if(cmd)
			REG_APB8_UART1_INT_ENABLE |= Intflag;
		else
			REG_APB8_UART1_INT_ENABLE &= (~Intflag);
	}
	else if(UARTx == UART2)
	{
		if(cmd)
			REG_APB9_UART2_INT_ENABLE |= Intflag;
		else
			REG_APB9_UART2_INT_ENABLE &= (~Intflag);
	}
}
/**************************************************
FunctionName: CleanUARTIntFlag
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
void CleanUARTIntFlag(uint8_t UARTx,uint32_t Intflag)
{
	if(UARTx == UART1)
	{
		REG_APB8_UART1_INT_FLAG |= (Intflag&0xFF);
	}
	else if(UARTx == UART2)
	{
		REG_APB9_UART2_INT_FLAG |= (Intflag&0xFF);
	}
}
/**************************************************
FunctionName:ReadUARTIntFlag
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
uint32_t ReadUARTIntFlag(uint8_t UARTx)
{
	if(UARTx == UART1)
	{
		return	(REG_APB8_UART1_INT_FLAG&0xFF);
	}
	else if(UARTx == UART2)
	{
	  return	(REG_APB9_UART2_INT_FLAG&0xFF);
	}
	return 0;
}

/**************************************************
FunctionName: UARTxFIFORdReady
Describe: Read UARTx FIFO Status 
InputParameter: UARTx ::={
                          UART1 = 0,
                          UART2 = 1
                         }
OutputParameter:
Notice: 
**************************************************/
uint32_t ReadUARTxFIFOStatus(uint8_t UARTx)
{
	if(UARTx == UART1)
	{
		return REG_APB8_UART1_FIFO_COUNT;
	}
	else if(UARTx == UART2)
	{
		return REG_APB9_UART2_FIFO_COUNT;
	}
	return 0;
}
/**************************************************
FunctionName: UARTxFIFORdReady
Describe: 读取是否FIFO有接收的数据(数据为被读空)标志 
InputParameter: UARTx ::={
                          UART1 = 0,
                          UART2 = 1
                         }
OutputParameter:
Notice: 
**************************************************/
uint32_t UARTxFIFORdReady(uint8_t UARTx)
{
	if(UARTx == UART1)
	{
		#if 1
			return (REG_APB8_UART1_FIFO_COUNT & (0x1 << 21));
		#else
			return ((REG_APB8_UART1_FIFO_COUNT>>21)&0x1);
		#endif
	}
	else if(UARTx == UART2)
	{
		#if 1
			return (REG_APB9_UART2_FIFO_COUNT & (0x1 << 21));
		#else
			return ((REG_APB9_UART2_FIFO_COUNT>>21)&0x1);
		#endif
	}
	return 0;
}
/**************************************************
FunctionName:
Describe:
InputParameter: UARTx ::={
                          UART1 = 0,
                          UART2 = 1
                         }
OutputParameter: UARTx Data (one Byte)
Notice:
**************************************************/
uint8_t UARTxRxData(uint8_t UARTx)
{
	if(UARTx == UART1)
	{
		return ((REG_APB8_UART1_DATA>>8)&0xFF);
	}
	else if(UARTx == UART2)
	{
		return ((REG_APB9_UART2_DATA>>8)&0xFF);
	}
	return 0;
}

/**************************************************
FunctionName:
Describe:
InputParameter: UARTx ::={
                          UART1 = 0,
                          UART2 = 1
                         }
OutputParameter: UARTx Data (one Byte)
Notice: 注意检测FIFO是否允许写入 
**************************************************/
void UARTxTxData(uint8_t UARTx,uint8_t dat)
{
	if(UARTx == UART1)
	{
		#if 1
		while(!(REG_APB8_UART1_FIFO_COUNT&(1<<20)));
		#endif
		REG_APB8_UART1_DATA = dat;
	}
	else if(UARTx == UART2)
	{
		#if 1
		/* Wait Ready */
		while(!(REG_APB9_UART2_FIFO_COUNT&(1<<20)));
		#endif
		REG_APB9_UART2_DATA = dat;
	}
}
/**************************************************
FunctionName:
Describe:
InputParameter: UARTx ::={
                          UART1 = 0,
                          UART2 = 1
                         }
OutputParameter: UARTx Data (one Byte)
Notice:
**************************************************/
void SetUARTxTxFIFOTHRESHOLD(uint8_t UARTx,uint8_t TxFIFOLen)
{
	if(UARTx == UART1)
	{
		REG_APB8_UART1_FIFO_THRESHOLD = 
		 (REG_APB8_UART1_FIFO_THRESHOLD&(~0xFF))|TxFIFOLen;
	}
	else if(UARTx == UART2)
	{
		REG_APB9_UART2_FIFO_THRESHOLD = 
		 (REG_APB9_UART2_FIFO_THRESHOLD&(~0xFF))|TxFIFOLen;
	}
}
/**************************************************
FunctionName:
Describe:
InputParameter: UARTx ::={
                          UART1 = 0,
                          UART2 = 1
                         }
                RxFIFOLen
OutputParameter:
Notice:
**************************************************/
void SetUARTxRxFIFOTHRESHOLD(uint8_t UARTx,uint8_t RxFIFOLen)
{
	if(UARTx == UART1)
	{
		REG_APB8_UART1_FIFO_THRESHOLD = 
		 (REG_APB8_UART1_FIFO_THRESHOLD&(~0xFF00))|(RxFIFOLen<<8);
	}
	else if(UARTx == UART2)
	{
		REG_APB9_UART2_FIFO_THRESHOLD = 
		 (REG_APB9_UART2_FIFO_THRESHOLD&(~0xFF00))|(RxFIFOLen<<8);
	}
}
/**************************************************
FunctionName:
Describe:
InputParameter: UARTx ::={
                          UART1 = 0,
                          UART2 = 1
                         }
                dt ::={  0:32  1:64  2:128  3:256}
OutputParameter:
Notice:
**************************************************/
void SetUARTxRxStopDetectTime(uint8_t UARTx,uint8_t dt)
{
	if(UARTx == UART1)
	{
		REG_APB8_UART1_FIFO_THRESHOLD = 
		 (REG_APB8_UART1_FIFO_THRESHOLD&(~0x30000))|((dt&0x03)<<16);
	}
	else if(UARTx == UART2)
	{
		REG_APB9_UART2_FIFO_THRESHOLD = 
		 (REG_APB9_UART2_FIFO_THRESHOLD&(~0x30000))|((dt&0x03)<<16);
	}
}

/**************************************************
FunctionName:
Describe:
InputParameter: UARTx ::={
                          UART1 = 0,
                          UART2 = 1
                         }
                conf
OutputParameter:
Notice:
**************************************************/
void UARTxConfig(uint8_t UARTx,UARTPara_T *conf)
{
	uint32_t Temp;
	if(UARTx == UART1)
	{
//		Temp = 0;
		if(conf->Tx_EN) Temp = 1;  //发送使能
		if(conf->RX_EN) Temp |= (1<<1);  //接收使能
		Temp |= ((conf->DataLenBit&0x03)<<3);  //数据长度
		Temp |= ((conf->CLK_DIV&0x1FFF)<<8);  //分频
		if(conf->UartVerify == _Verify_ODD) Temp |= ((1<<5)|(1<<6)) ;  //校验
		else if(conf->UartVerify == _Verify_EVEN) Temp |= (1<<5); 
    
		if(conf->UartIRDA)Temp |= (1<<2);
		if(conf->UartStopBit) Temp |= (1<<7);  //停止位 0:1bit 1:2bit
		
		REG_APB8_UART1_CFG = Temp;
		
		REG_APB8_UART1_FIFO_THRESHOLD = (1<<0)|(1<<8);
		
		REG_APB8_UART1_FIFO_THRESHOLD = 
		   (conf->TxFIFOThreshold)|(conf->RxFIFOThreshold<<8)\
		   |((conf->RxStopDetectTime&0x03)<<16);
	}
	else if(UARTx == UART2)
	{
//	  Temp = 0;
		if(conf->Tx_EN) Temp = 1;  //发送使能
		if(conf->RX_EN) Temp |= (1<<1);  //接收使能
		Temp |= ((conf->DataLenBit&0x03)<<3);  //数据长度
		Temp |= ((conf->CLK_DIV&0x1FFF)<<8);  //分频
		if(conf->UartVerify == _Verify_ODD) Temp |= ((1<<5)|(1<<6)) ;  //校验
		else if(conf->UartVerify == _Verify_EVEN) {Temp |= (1<<5);Temp&=(~(1<<6));} //偶
    
		if(conf->UartIRDA)Temp |= (1<<2);
		if(conf->UartStopBit) Temp |= (1<<7);  //停止位 0:1bit 1:2bit
		
		REG_APB9_UART2_CFG = Temp;
		
		REG_APB9_UART2_FIFO_THRESHOLD = 
		   (conf->TxFIFOThreshold)|(conf->RxFIFOThreshold<<8)\
		   |((conf->RxStopDetectTime&0x03)<<16);
	}
}



/***********************  END OF FILES  ***********************/
