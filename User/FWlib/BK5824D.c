#include "BK5824D.h"
#include "BK5863.h"
typedef struct{
	unsigned tx_busy:1;
#if BK5824_REG_LEN_OPTION == BK5824_NEW_REG_LEN_OPTION
	unsigned reg16_write_len : 8;
	unsigned reg16_read_len : 8;
#endif
}rf58_status_t;

static rf58_status_t _rf58_t = {0};

#if BK5824_REG_LEN_OPTION == BK5824_REG_LEN_OPTION_PREV
unsigned char BK5824RegWriteLen[21], BK5824RegReadLen[21];
#endif

int get_bk5824_reg_read_len(unsigned char RegAddr)
{
#if BK5824_REG_LEN_OPTION == BK5824_NEW_REG_LEN_OPTION
	int len;
	if((RegAddr >= 0)&&(RegAddr <= 15)){
		len = 4;
	}else if(RegAddr == 16){
		len = _rf58_t.reg16_read_len;
	}else if(RegAddr == 17){
		len = 14;
	}else if(RegAddr == 18){
		len = 11;
	}else if(RegAddr == 19){
		len = 0;
	}else if(RegAddr == 20){
		len = 0;
	}else{
		len = 4;
	}
	return len;
#elif BK5824_REG_LEN_OPTION == BK5824_REG_LEN_OPTION_PREV
	return BK5824RegReadLen[RegAddr];
#else
	#err1 ....
#endif
}

int get_bk5824_reg_write_len(unsigned char RegAddr)
{
#if BK5824_REG_LEN_OPTION == BK5824_NEW_REG_LEN_OPTION
	int len;
	if((RegAddr >= 0)&&(RegAddr <= 15)){
		len = 4;
	}else if(RegAddr == 16){
		len = _rf58_t.reg16_write_len;
	}else if(RegAddr == 17){
		len = 14;
	}else if(RegAddr == 18){
		len = 11;
	}else if(RegAddr == 19){
		len = 0;
	}else if(RegAddr == 20){
		len = 0;
	}else{
		len = 4;
	}
	return len;
#elif BK5824_REG_LEN_OPTION == BK5824_REG_LEN_OPTION_PREV
	return BK5824RegWriteLen[RegAddr];
#else
	#err1 ....
#endif
}

int set_bk5824_reg_read_len(unsigned char RegAddr,unsigned char len)
{
#if BK5824_REG_LEN_OPTION == BK5824_NEW_REG_LEN_OPTION
	if(RegAddr == 16)
	{
		_rf58_t.reg16_read_len = len;
		return TRUE;
	}else if((RegAddr >= 0)&&(RegAddr <= 15)){
		if(len == 4) return TRUE;
	}else if(RegAddr == 17){
		if(len == 14) return TRUE;
	}else if(RegAddr == 18){
		if(len == 11) return TRUE;
	}else if(RegAddr == 19){
		if(len == 0) return TRUE;
	}else if(RegAddr == 20){
		if(len == 0) return TRUE;
	}
	return FALSE;
#elif BK5824_REG_LEN_OPTION == BK5824_REG_LEN_OPTION_PREV
	BK5824RegReadLen[RegAddr] = len;
	return TRUE;
#else
	#err1 ....
#endif
}

int set_bk5824_reg_write_len(unsigned char RegAddr,unsigned char len)
{
#if BK5824_REG_LEN_OPTION == BK5824_NEW_REG_LEN_OPTION
	if(RegAddr == 16)
	{
		_rf58_t.reg16_write_len = len;
		return TRUE;
	}else if((RegAddr >= 0)&&(RegAddr <= 15)){
		if(len == 4)return TRUE;
	}else if(RegAddr == 17){
		if(len == 14)return TRUE;
	}else if(RegAddr == 18){
		if(len == 11)return TRUE;
	}else if(RegAddr == 19){
		if(len == 0)return TRUE;
	}else if(RegAddr == 20){
		if(len == 0)return TRUE;
	}
	return FALSE;
#elif BK5824_REG_LEN_OPTION == BK5824_REG_LEN_OPTION_PREV
	BK5824RegWriteLen[RegAddr] = len;
	return TRUE;
#else
	#err1 ....
#endif
}

/**************************************************
FunctionName: 
Describe: BK5824 SPI Read
InputParameter: RegAddr : Reg address.
OutputParameter: pData   : Reg read data pointer.
Notice:
**************************************************/
void BK5824SpiRead(unsigned char RegAddr, unsigned char *pData)
{
	unsigned char 	i, DataByteLen, DataNum;
	uint32_t irq_cpu;
#if BK5824_REG_LEN_OPTION == BK5824_NEW_REG_LEN_OPTION
	DataByteLen = get_bk5824_reg_read_len(RegAddr)&0x0FF;
#elif BK5824_REG_LEN_OPTION == BK5824_REG_LEN_OPTION_PREV
	DataByteLen = BK5824RegReadLen[RegAddr];
#endif
	irq_cpu = REG_APB3_ICU_IRQTYPE_EN;
	REG_APB3_ICU_IRQTYPE_EN = 0;
	if(DataByteLen > 0) 
	{
//		REG_APBA_SPI1_CTRL  = 0x00C20f00;
		/* config SPI */
		REG_APBA_SPI1_CTRL  = (0x00C20200&(~(0xFF<<8)))|(0x03<<8)|(0x01<<24);
	  /* Write Addr */
		REG_APBA_SPI1_DAT   = (RegAddr & 0x1f);
		REG_APBA_SPI1_CTRL |= 0x00000040;	        //open txint_en
		while(!(REG_APBA_SPI1_STAT & 0x100));       //wait spi transfer finish
		i = REG_APBA_SPI1_DAT;				        //discard
	
		while(DataByteLen)
		{
			DataNum = MIN(DataByteLen, 15);
			for(i = 0; i < DataNum; i ++)
			{
				REG_APBA_SPI1_DAT = 0x0;
			}
//			BKDelay100uS(1);
			REG_APBA_SPI1_STAT = 0x100;					//clear tx int
			while(!(REG_APBA_SPI1_STAT & 0x100));       //wait spi transfer finish
			for(i = 0; i < DataNum; i ++)
			{
				*pData = REG_APBA_SPI1_DAT & 0xff;
				pData ++;
			} 
			DataByteLen -= DataNum;
		}
	  REG_APBA_SPI1_CTRL = 0x00430f00;		//nss0 set, close enable
	}
	 REG_APB3_ICU_IRQTYPE_EN = irq_cpu;
}
/**************************************************
FunctionName: 
Describe: BK5824 SPI Write
InputParameter: RegAddr : Reg address.
          pData   : Reg write data pointer.
OutputParameter:
Notice:
**************************************************/
void BK5824SpiWrite(unsigned char RegAddr, unsigned char *pData)
{
	unsigned char 	i, DataByteLen, DataNum;
	unsigned char	*pDataLast;
	uint32_t irq_cpu;
#if BK5824_REG_LEN_OPTION == BK5824_NEW_REG_LEN_OPTION
	DataByteLen = get_bk5824_reg_write_len(RegAddr)&0x0FF;
#elif BK5824_REG_LEN_OPTION == BK5824_REG_LEN_OPTION_PREV	
	DataByteLen = BK5824RegWriteLen[RegAddr];
#endif
	pDataLast = pData + DataByteLen - 1;
	
	irq_cpu = REG_APB3_ICU_IRQTYPE_EN;
	REG_APB3_ICU_IRQTYPE_EN = 0;
	if(DataByteLen > 0) 
	{
//		REG_APBA_SPI1_CTRL  = 0x00C20f00;
		/* config SPI */
	 	REG_APBA_SPI1_CTRL  = (0x00C20200&(~(0xFF<<8)))|(0x03<<8)|(0x01<<24);
	  /* Write Addr */
		REG_APBA_SPI1_DAT   = 0x20 | (RegAddr & 0x1f);
		REG_APBA_SPI1_CTRL |= 0x00000040;	        //open txint_en
		while(!(REG_APBA_SPI1_STAT & 0x100));    //wait spi transfer finish
    /* Write Data */
		while(DataByteLen)
		{
			DataNum = MIN(DataByteLen, 15);
			for(i = 0; i < DataNum; i ++)
			{
				if(RegAddr < 0x09) 					        //analog register
				{
					REG_APBA_SPI1_DAT = *pDataLast;
					pDataLast --;
				}
				else
				{
					REG_APBA_SPI1_DAT = *pData;
					pData ++;
				}
			}
			REG_APBA_SPI1_STAT = 0x100;									//clear tx int
			while(!(REG_APBA_SPI1_STAT & 0x100));       //wait spi transfer finish
//			BKDelay100uS(5);
			delay_us(1);
			DataByteLen -= DataNum;
		}
	  REG_APBA_SPI1_CTRL = 0x00430f00;		//nss0 set, close enable
	}
	 REG_APB3_ICU_IRQTYPE_EN = irq_cpu;
}

/**************************************************
FunctionName: BKDelay100uS
Describe:  Delay 100uS
InputParameter: 
OutputParameter: 
Notice:
**************************************************/
void BKDelay100uS(unsigned int time)
{
	delay_us(99);
}

/**************************************************
FunctionName: BK5824EnterSleep
Describe: 
InputParameter: 
OutputParameter: 
Notice:
**************************************************/
void BK5824EnterSleep(void)
{
	unsigned int reg_value;

	BK5824SpiRead(9, (unsigned char*)&reg_value);
	reg_value &= ~(1<<0);                               //pwr_up=sleep
	BK5824SpiWrite(9,(unsigned char*)&reg_value);
	BKDelay100uS(1);   
	BK5824_SoftWakeup_toggle(); 	
}

/**************************************************
FunctionName: 
Describe: 
InputParameter: 
OutputParameter: 
Notice:
**************************************************/
void BK5824PowerUp(void)
{
	unsigned int reg_value;

	BK5824SpiRead(9, (unsigned char*)&reg_value);
	reg_value |= 1;                               //pwr_up
	BK5824SpiWrite(9,(unsigned char*)&reg_value);
//    BKDelay100uS(1);  
}

void bk5824_crc_seed(int ops)
{
	unsigned int reg_value;

	BK5824SpiRead(15, (unsigned char*)&reg_value);
	if(ops){
		reg_value |= (1<<5);
	}
	else{
		reg_value &= (~(1U<<5));
	}
	BK5824SpiWrite(15,(unsigned char*)&reg_value);
}

void bk5824_crc_en(int ops)
{
	unsigned int reg_value;

	BK5824SpiRead(15, (unsigned char*)&reg_value);
	if(ops){
		reg_value |= (1<<4);
	}
	else{
		reg_value &= (~(1U<<4));
	}
	BK5824SpiWrite(15,(unsigned char*)&reg_value);
}


/**************************************************
FunctionName: 
Describe: 
InputParameter: 
OutputParameter: 
Notice:
**************************************************/
void BK5824Power(unsigned char cmd)
{
  unsigned int reg_value;

  BK5824SpiRead(9, (unsigned char*)&reg_value);
	if(cmd)
		reg_value |= 1;      
	else
    reg_value &= ~(1<<0);                               //pwr_up=sleep
  BK5824SpiWrite(9,(unsigned char*)&reg_value);
      	
}

/**************************************************
FunctionName: 使能或者禁止零唤醒功能
Describe: 
          Reg9<31>   
InputParameter: 
OutputParameter: 
Notice: 
**************************************************/
void BK5824_0Wakeup(uint8_t  status)
{
	unsigned int reg_value;

	if(status)
	{
		BK5824SpiRead(9, (unsigned char*)&reg_value);
		reg_value |= ((unsigned int)1<<31);  
		BK5824SpiWrite(9,(unsigned char*)&reg_value);
	}
	else
	{
		BK5824SpiRead(9, (unsigned char*)&reg_value);
		reg_value &= (~((unsigned int)1<<31));  
		BK5824SpiWrite(9,(unsigned char*)&reg_value);
	}
}
/**************************************************
FunctionName: BK5824_wuCal_toggle
Describe: Reg0x0F<bit28> 
InputParameter: 
OutputParameter: 
Notice:
**************************************************/
void BK5824_wuCal_toggle(void)
{
	unsigned int reg_value;

	BK5824SpiRead(15, (unsigned char*)&reg_value);
	reg_value |= (1<<28);  
	BK5824SpiWrite(15,(unsigned char*)&reg_value);
    BKDelay100uS(100);
	reg_value &= ~(1<<28);  
	BK5824SpiWrite(15,(unsigned char*)&reg_value);
}

/**************************************************
FunctionName: BK5824_SoftWakeup_toggle
Describe: 重新进入0WK Mode
          Reg9<10> 
InputParameter: 
OutputParameter: 
Notice: 
**************************************************/
void BK5824_SoftWakeup_toggle(void)
{
	unsigned int reg_value;

	BK5824SpiRead(9, (unsigned char*)&reg_value);
	reg_value |= (1<<10);  
	BK5824SpiWrite(9,(unsigned char*)&reg_value);
	BK5824SpiRead(9, (unsigned char*)&reg_value);
	reg_value &= (~(1<<10));  
	BK5824SpiWrite(9,(unsigned char*)&reg_value);
}

/**************************************************
FunctionName: Bk5824_IRQ_malloc
Describe: 
InputParameter: 
OutputParameter: 
Notice: 该函数关闭了接收，若您需要接收新的数据，
        应打开Reg15<1>的接收使能
**************************************************/
uint8_t Bk5824_IRQ_malloc(uint8_t **p_recv,uint8_t *p_recvlen)
{
	uint8_t flag[4];
	uint8_t buf[4];
	uint32_t  reg_value,stu = 0;
	/*读取Reg6的状态字节 -- 低8位*/
	BK5824SpiRead(6, (unsigned char*)&reg_value);
	flag[0] = (uint8_t)reg_value;
	/*判断接收结束标志是否有效*/
	if((flag[0]&Bk5824RxEndIrt))
	{
		BK5824SpiRead(5, (unsigned char*)&reg_value);			//读取FIFO中的数据长度
		buf[0] = (uint8_t)reg_value;
		*p_recvlen = buf[0];
		if(buf[0] > 128)    //国标规定单次数据传输个数不能超过128个
		{
			BK5824ClearIrq(0x07);
			stu = BK58Rx_Err;
			Bk5824_RX(Enable);
			goto end;
		}
		else if(buf[0] == 0)
		{
			BK5824ClearIrq(0x07);
			Bk5824_RX(Enable);
		#if BK5824_DEFALUT_LOG
			printf("buf[0] == 0\r\n");
		#endif
			goto end;
		}
		//从FIFO中读取指定长度的数据 
		*p_recv = (uint8_t*)BKmmgmt_malloc((*p_recvlen));
		BK5824ReadData((*p_recv),p_recvlen);
//		Bk5824_RX(Disable);
		Bk5824_RX(Enable);
		BK5824ClearIrq(0x07);
		stu = BK58Rx_OK;
	}
	if(flag[0] & Bk5824RxErrIrt)  //rx_error_irq
	{
		Bk5824_RX(Enable);
	#if BK5824_DEFALUT_LOG
		printf("Rx_error_irq\r\n");
	#endif
		BK5824ClearIrq(0x07);
	}
	if(flag[0] & Bk5824TxEndIrt)  //Tx_irq
	{
		Bk5824_RX(Enable);
	#if BK5824_DEFALUT_LOG
		printf("Tx_irq\r\n");
	#endif
		set_rf58_tx_busy_state(0);
		BK5824ClearIrq(0x07);
	}
	return stu;
end:	
	BK5824ClearIrq(0x07);
	return stu;
}
/**************************************************
FunctionName: Bk5824_IRQ_malloc
Describe: 
InputParameter: 
OutputParameter: 
Notice: 该函数关闭了接收，若您需要接收新的数据，
        应打开Reg15<1>的接收使能
**************************************************/
uint8_t Bk5824IRQdeal(uint8_t *p_recv,uint8_t *p_recvlen)
{
	uint8_t flag[4];
	uint8_t buf[4];
	uint32_t  reg_value,stu = 0;
	/*读取Reg6的状态字节 -- 低8位*/
	BK5824SpiRead(6, (unsigned char*)&reg_value);
	flag[0] = (uint8_t) reg_value;
	/*判断接收结束标志是否有效*/
	if((flag[0]&Bk5824RxEndIrt))
	{
		BK5824SpiRead(5, (unsigned char*)&reg_value);			//读取FIFO中的数据长度
		buf[0] = (uint8_t)reg_value;
		*p_recvlen = buf[0];
		if(buf[0] > 128)    //国标规定单次数据传输个数不能超过128个
		{
			BK5824ClearIrq(0x07);
			stu = BK58Rx_Err;
			Bk5824_RX(Enable);
			goto end;
		}
		//从FIFO中读取指定长度的数据 
		BK5824ReadData(p_recv,p_recvlen);
//		Bk5824_RX(Disable);
		Bk5824_RX(Enable);
		BK5824ClearIrq(0x07);
		stu = BK58Rx_OK;
	}
	if(flag[0]&Bk5824RxErrIrt)  //rx_error_irq
	{
		Bk5824_RX(Enable);
	#if BK5824_DEFALUT_LOG
		printf("rx_error_irq\r\n");
	#endif
		BK5824ClearIrq(0x07);
	}
	if(flag[0]&Bk5824TxEndIrt)  //Tx_irq
	{
		Bk5824_RX(Enable);
		set_rf58_tx_busy_state(0);
	#if BK5824_DEFALUT_LOG
		printf("Tx_irq\r\n");
	#endif
		BK5824ClearIrq(0x07);
	}
	return stu;
end:	
	BK5824ClearIrq(0x07);
	return stu;
}
/**************************************************
FunctionName: BK5824ClearIrq
Describe: 
InputParameter: 
OutputParameter: 
Notice:
**************************************************/
void BK5824ClearIrq(uint8_t flag)
{
	unsigned int  reg_value;		
	if(flag>0x0F) return;
	BK5824SpiRead(15, (unsigned char*)&reg_value);; //读取REG15的值
	reg_value &= ~(0x0F<<24);
	reg_value |= ((unsigned int)flag)<<24;
	BK5824SpiWrite(15, (unsigned char*)&reg_value);
}

/**************************************************
FunctionName: Bk5824_RX
Describe:   接收使能或者关闭
InputParameter: 
OutputParameter: 
Notice:
**************************************************/
void Bk5824_RX(uint8_t	Reg15bit1)
{
	unsigned int reg_value; 
	BK5824SpiRead(15, (unsigned char*)&reg_value);		//Set RX mode 
	if(Reg15bit1)
		reg_value |= (1<<1);
	else
		reg_value &= ~(1<<1);	
	BK5824SpiWrite(15,(unsigned char*)&reg_value);
}
/**************************************************
FunctionName: BK5824_ReadDate
Describe: 	@读取接收数据
						@para:data(返回的数据);length(返回长度)
InputParameter: 
OutputParameter: 
Notice:
**************************************************/
void BK5824ReadData(uint8_t *RecDat,uint8_t *length)
{
//	unsigned char Len;
	/* Read Length */
//	BK5824SpiRead(5, (unsigned char*)&Len); 
//	*length = Len;
//	Len = *length;
	/* Read Data */
#if BK5824_REG_LEN_OPTION == BK5824_REG_LEN_OPTION_PREV
	BK5824RegReadLen[16] = (*length);
#else
	set_bk5824_reg_read_len(16,(*length));
#endif
	BK5824SpiRead(16, RecDat);
}

/**************************************************
FunctionName: BK5824SendDate
Describe: 	@发送数据
						@para:RecDat(待发送的数据);length(发送长度)
InputParameter: 
OutputParameter: 
Notice:
**************************************************/
void BK5824SendData(uint8_t *RecDat,uint8_t length)
{
#if BK5824_REG_LEN_OPTION == BK5824_REG_LEN_OPTION_PREV
	BK5824RegWriteLen[16] = length;
#else
	set_bk5824_reg_write_len(16,length);
#endif
	set_rf58_tx_busy_state(1);
	BK5824SpiWrite(16, RecDat);
#if BK5824_REG_LEN_OPTION == BK5824_REG_LEN_OPTION_PREV
	BK5824RegWriteLen[16] = 1;
#else
	set_bk5824_reg_write_len(16,1);
#endif
}

int bk5824_send_data_safety(unsigned char *msg,unsigned char len)
{
	if(get_rf58_tx_busy_state()){
		return FALSE;
	}
	BK5824SendData(msg,len);
	return TRUE;
}

/**************************************************
FunctionName: reset
Describe: 
          Reg9<31>   
InputParameter: 
OutputParameter: 
Notice: 
**************************************************/
void BK5824_reset(void)
{
	unsigned int reg_value;

	BK5824SpiRead(15, (unsigned char*)&reg_value);
	reg_value &= (~(1<<0));  
	BK5824SpiWrite(15,(unsigned char*)&reg_value);
	BKDelay100uS(1);
	BK5824SpiRead(15, (unsigned char*)&reg_value);
	reg_value |= (1<<0);  
	BK5824SpiWrite(15,(unsigned char*)&reg_value);
}


/**************************************************
FunctionName:  发射频点设置
Describe:         5790 和5800
InputParameter:
OutputParameter:
Notice:
**************************************************/
void bk5824_set_tx_freq(RF58_FREQ_E freq)
{
	unsigned int reg_value;	
	
	if(freq == FREQ_5790)			reg_value = get_lo_frequency_value(5790);
	else if(freq ==FREQ_5800)		reg_value = get_lo_frequency_value(5800);
	else if(freq ==FREQ_5830)		reg_value = get_lo_frequency_value(5830);
	else if(freq ==FREQ_5835)		reg_value = get_lo_frequency_value(5835);
	else if(freq ==FREQ_5840)		reg_value = get_lo_frequency_value(5840);
	else if(freq ==FREQ_5795)		reg_value = get_lo_frequency_value(5795);
	else if(freq ==FREQ_5805)		reg_value = get_lo_frequency_value(5805);
	else {
		return;
	}
	BK5824SpiWrite(5, (unsigned char *)&reg_value);
}

void bk5824_set_rx_freq(RF58_FREQ_E freq)
{
	unsigned int reg_value;	
	
	if(freq == FREQ_5790)			reg_value = get_lo_frequency_value(5790);
	else if(freq ==FREQ_5800)		reg_value = get_lo_frequency_value(5800);
	else if(freq ==FREQ_5830)		reg_value = get_lo_frequency_value(5830);
	else if(freq ==FREQ_5835)		reg_value = get_lo_frequency_value(5835);
	else if(freq ==FREQ_5840)		reg_value = get_lo_frequency_value(5840);
	else if(freq ==FREQ_5795)		reg_value = get_lo_frequency_value(5795);
	else if(freq ==FREQ_5805)		reg_value = get_lo_frequency_value(5805);
	else {
		return;
	}
	BK5824SpiWrite(6, (unsigned char *)&reg_value);
}

/**************************************************
FunctionName:  功率设置
Describe:         0-f
InputParameter:
OutputParameter:
Notice:
**************************************************/
void bk5824_set_rf_power(unsigned char power)
{
	unsigned int reg_value;

	BK5824SpiRead(9, (unsigned char *)&reg_value);
	if(power > 0x07) 
		power = 0x07;
	reg_value &= (~ ((unsigned int)0x07<<1));
	reg_value |= ((power&0x07)<<1);

	BK5824SpiWrite(9, (unsigned char *)&reg_value);
}

#ifdef BK5870
int bk5824_internal_audion_ctrl(BK5824_INT_AUD_CTL ops)
{
	unsigned int temp;
	switch(ops)
	{
		case BK5824_INT_AUD_EN:
			///C7 output = 1
			temp = REG_APB7_GPIOC_CFG;
			temp &= ~((1U<<7)|(1U<<(7+8))|(1U<<(7+16))|(1U<<(7+24)));
			temp |= ((1<<7)|(1<<(7+8)));
			REG_APB7_GPIOC_CFG = temp;
			
			temp = REG_APB7_GPIOC_DATA;
			temp &= ~((1U<<7)|(1U<<(7+8)));
			temp |= (1U<<7);
			REG_APB7_GPIOC_DATA = temp;
			break;
		case BK5824_INT_AUD_EN_PP:
			///C7 output = 1
			temp = REG_APB7_GPIOC_CFG;
			temp &= ~((1U<<7)|(1U<<(7+8))|(1U<<(7+16))|(1U<<(7+24)));
			temp |= ((1<<7)|(1<<(7+8))|(1U<<(7+16)));   ///pull up
			REG_APB7_GPIOC_CFG = temp;
			
			temp = REG_APB7_GPIOC_DATA;
			temp &= ~((1U<<7)|(1U<<(7+8)));
			temp |= (1U<<7);
			REG_APB7_GPIOC_DATA = temp;
			break;
		case BK5824_INT_AUD_DIS:
			///c7 output = 0
			temp = REG_APB7_GPIOC_CFG;
			temp &= ~((1U<<7)|(1U<<(7+8))|(1U<<(7+16))|(1U<<(7+24)));
			temp |= ((1<<7)|(1<<(7+8)));
			REG_APB7_GPIOC_CFG = temp;
			
			temp = REG_APB7_GPIOC_DATA;
			temp &= ~((1U<<7)|(1U<<(7+8))|(1U<<(7+16)));
			REG_APB7_GPIOC_DATA = temp;
			break;
		default:
			///c7 != output&input
			temp = REG_APB7_GPIOC_CFG;
			temp &= ~((1U<<7)|(1U<<(7+8))|(1U<<(7+16))|(1U<<(7+24)));
			temp |= (1U<<7);
			REG_APB7_GPIOC_CFG = temp;
			
			temp = REG_APB7_GPIOC_DATA;
			temp &= ~((1U<<7)|(1U<<(7+8))|(1U<<(7+16)));
			REG_APB7_GPIOC_DATA = temp;
			break;
	}
	return 0;
}
#endif

void set_bk5824_wko_weakup(int ops)
{
	if(ops){
		REG_APB7_GPIO_INT_ENABLE |= ((1U<<5)<<16);
	}else{
		REG_APB7_GPIO_INT_ENABLE &= (~((1U<<5)<<16));
	}
}

void set_bk5824_irq_weakup(int ops)
{
	if(ops){
		REG_APB7_GPIO_INT_ENABLE |= ((1U<<4)<<16);
	}else{
		REG_APB7_GPIO_INT_ENABLE &= (~((1U<<4)<<16));
	}
}

void set_rf58_tx_busy_state(int ops)
{
	if(ops){
		_rf58_t.tx_busy = 1;
	}
	else{
		_rf58_t.tx_busy = 0;
	}
}

int get_rf58_tx_busy_state(void)
{
	return (_rf58_t.tx_busy == 1)?1:0;
}

struct lo_freq_value_table{
	unsigned int freq;
	unsigned int value;
	float vco;
	unsigned int Ncal;
};

const static struct lo_freq_value_table lo_fvt[] = {
	{5790,0x75CC4000,3860,0x76},
	{5785,0x75B23555,3856.6667,0x76},
	{5830,0x769c9555,3886.6667,0x77},
	{5835,0x76b6a000,3890,0x77},

	{5795,0x75e64aab,3863.3333,0x76},
	{5800,0x76005555,3866.6667,0x76},
	{5840,0x76d0aaab,3893.3333,0x77},

	{5805,0x761A6000,3870,0x76},
	{5810,0x76346AAB,3873.3333,0x76},
	{5815,0x764E7555,3876.6667,0x76}
};

unsigned int get_lo_frequency_value(unsigned int freq)
{
	short i;
	
	for(i=0;i<(sizeof(lo_fvt)/sizeof(struct lo_freq_value_table));i++){
		if(lo_fvt[i].freq == freq){
			return lo_fvt[i].value;
		}
	}
	
	return 0;
}

unsigned int get_lo_value_frequency(unsigned int value)
{
	short i;

	for(i=0;i<(sizeof(lo_fvt)/sizeof(struct lo_freq_value_table));i++){
		if(lo_fvt[i].freq == value){
			return lo_fvt[i].freq;
		}
	}
	
	return 0;
}

float get_lo_frequency_vco(unsigned int freq)
{
	short i;
		
	for(i=0;i<(sizeof(lo_fvt)/sizeof(struct lo_freq_value_table));i++){
		if(lo_fvt[i].freq == freq){
			return lo_fvt[i].vco;
		}
	}
	
	return 0;
}

unsigned int get_lo_frequency_Ncal(unsigned int freq)
{
	short i;
		
	for(i=0;i<(sizeof(lo_fvt)/sizeof(struct lo_freq_value_table));i++){
		if(lo_fvt[i].freq == freq){
			return lo_fvt[i].Ncal;
		}
	}
	
	return 0;
}

void BK5824_cw_hold_toggle(unsigned int reg_value)
{
	unsigned int reg_value1 = reg_value;

	///BK5824SpiRead(15, (unsigned char*)&reg_value);
	reg_value1 &= ~(1<<17);
	BK5824SpiWrite(7,(unsigned char*)&reg_value1);
  BKDelay100uS(30);   
	reg_value1 = reg_value;
	reg_value1 |= (1<<17);  
	BK5824SpiWrite(7,(unsigned char*)&reg_value1);
}

/***********************  END OF FILES  ***********************/

