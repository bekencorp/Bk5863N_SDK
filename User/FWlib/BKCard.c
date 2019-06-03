#include "BKCard.h"
#include "BK5863.h"


static unsigned char block_num;


/**********************************************************************
  设置读卡器数字逻辑的工作方式 
**********************************************************************/
char PcdConfigISOType(unsigned char type)
{
   if (type == 'A')                     //ISO14443_A
   { 
		ClearBitMask(Status2Reg,0x08);
		WriteRawRC(ModeReg,0x3D);//3
		WriteRawRC(RxSelReg,0x86);//UartSel = 2; 
		WriteRawRC(TxSelReg,0x16);       
		WriteRawRC(TReloadRegL,30);//tmoLength);// TReloadVal = 'h6a =tmoLength(dec) 
		WriteRawRC(TReloadRegH,0);
		WriteRawRC(TModeReg,0x8D);
		WriteRawRC(TPrescalerReg,0x3E);       
		WriteRawRC(RFU1A, 0x03); 
		delay_us(10);
   }
   else
   { 
	   return MI_ERR; 
	 }
   return MI_OK;
}

/**********************************************************************
功    能：清RC522寄存器位
参数说明：reg[IN]:寄存器地址
          mask[IN]:清位值
**********************************************************************/
void ClearBitMask(unsigned char reg,unsigned char mask)  
{
    char tmp = 0x0;
    tmp = ReadRawRC(reg);
    WriteRawRC(reg, tmp & ~mask);  // clear bit mask
} 

/**********************************************************************
//功    能：置RC522寄存器位
//参数说明：reg[IN]:寄存器地址
//          mask[IN]:置位值
**********************************************************************/
void SetBitMask(unsigned char reg,unsigned char mask)  
{
	char tmp = 0x0;
	tmp = ReadRawRC(reg);
	WriteRawRC(reg,tmp | mask);  // set bit mask
}

/**********************************************************************
功    能：关断读卡器
参数说明：无
**********************************************************************/
void PcdPowerDown(void)
{
  REG_APB3_ICU_ANA2_CTRL     &= ~(1<<29);       //power down card reader
}

/**********************************************************************
功    能：打开读卡器
参数说明：无
**********************************************************************/
void PcdPowerOn(void)
{
	unsigned char       i;

	REG_APB3_ICU_ANA2_CTRL      = 0x6371ee01;//0x927860F1;       //power on card reader
	delay_us(1000);
	for(i = 0; i < 100; i++);
	REG_APB3_ICU_ANA2_CTRL      = 0x6371ee01|(1<<18);//0x927878F1;       //filter calibration
	for(i = 0; i < 100; i++);
	REG_APB3_ICU_ANA2_CTRL      = 0x6371ee01;//0x927868F1;  
	delay_us(1000);
}

/**********************************************************************
功    能：读卡器电源
参数说明：Enable或Disable
**********************************************************************/
void PcdPower(unsigned char cmd)
{
  if(cmd)
		PcdPowerOn();
	else
		PcdPowerDown();
}

/**********************************************************************
功    能：复位读卡器的数字逻辑
返    回: 成功返回MI_OK
**********************************************************************/
char PcdReset(void)
{
    delay_us(10);
    WriteRawRC(CommandReg,PCD_RESETPHASE);
    delay_us(10);
    
    WriteRawRC(ModeReg,0x3D);            //和Mifare卡通讯，CRC初始值0x6363
    WriteRawRC(TReloadRegL,30);           
    WriteRawRC(TReloadRegH,0);
//	WriteRawRC(TReloadRegH,1);  //debug
    WriteRawRC(TModeReg,0x8D);
    WriteRawRC(TPrescalerReg,0x3E);
		WriteRawRC(TxAutoReg,0x40);     //必须要
		WriteRawRC(RxThresholdReg, 0x30); 
   	WriteRawRC(TxModeReg, 0x0);
    WriteRawRC(RxModeReg, 0x0);
    WriteRawRC(ModGsCfgReg, 0x05);  //demod_opt is '1' 
    
		WriteRawRC(CWGsCfgReg, 0x01);	//DC Calibration
    while(ReadRawRC(CWGsCfgReg));
		
		block_num  = 0;
    return MI_OK;
}

/**********************************************************************
功    能：寻卡
参数说明: req_code[IN]:寻卡方式
                0x52 = 寻感应区内所有符合14443A标准的卡
                0x26 = 寻未进入休眠状态的卡
          pTagType[OUT]：卡片类型代码
                0x4400 = Mifare_UltraLight
                0x0400 = Mifare_One(S50)
                0x0200 = Mifare_One(S70)
                0x0800 = Mifare_Pro(X)
                0x4403 = Mifare_DESFire
返    回: 成功返回MI_OK
**********************************************************************/
char PcdRequest(unsigned char req_code,unsigned char *pTagType)
{
	char status;  
	unsigned int unLen;
	unsigned char ucComMF522Buf[MAXRLEN]; 

	ClearBitMask(Status2Reg,0x08);
	WriteRawRC(BitFramingReg,0x07);
	SetBitMask(TxControlReg,0x03);
 
	ucComMF522Buf[0] = req_code;
	status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,1,ucComMF522Buf,&unLen);

	if ((status == MI_OK) && (unLen == 0x10))
	{    
		*pTagType     = ucComMF522Buf[0];
		*(pTagType+1) = ucComMF522Buf[1];
	}
	else
	{
	    printf("Fun:PcdRequest, unLen=%d\r\n",unLen);
	    status = MI_ERR;   
	}
   
	return status;
}

/**********************************************************************
功    能：通过RC522和ISO14443卡通讯
参数说明：Command[IN]:RC522命令字
          pInData[IN]:通过RC522发送到卡片的数据
          InLenByte[IN]:发送数据的字节长度
          pOutData[OUT]:接收到的卡片返回数据
          *pOutLenBit[OUT]:返回数据的位长度
**********************************************************************/
char PcdComMF522(uint8_t Command, uint8_t *pInData,uint8_t InLenByte,
                 uint8_t *pOutData,unsigned int *pOutLenBit)
{
	char status = MI_ERR;
	unsigned char irqEn   = 0x00;
	unsigned char waitFor = 0x00;
	unsigned char lastBits;
	unsigned char n;
	unsigned int i;
	switch (Command)
	{
		case PCD_AUTHENT:
			irqEn   = 0x12;
			waitFor = 0x10;
			break;
		case PCD_TRANSCEIVE:
			irqEn   = 0x77;
			waitFor = 0x30;
			break;
		default:
			break;
	}
 
	WriteRawRC(ComIEnReg,irqEn|0x80);
	ClearBitMask(ComIrqReg,0x80);
	WriteRawRC(CommandReg,PCD_IDLE);
	SetBitMask(FIFOLevelReg,0x80);
	
	for (i=0; i<InLenByte; i++)
	{   
		WriteRawRC(FIFODataReg, pInData[i]); 
	}
	WriteRawRC(CommandReg, Command);
 
 
	if (Command == PCD_TRANSCEIVE)
	{    
		SetBitMask(BitFramingReg,0x80);  
	}
	
	//i = 600;//根据时钟频率调整，操作M1卡最大等待时间25ms

	i = 20000;
	do 
	{
		n = ReadRawRC(ComIrqReg);
		i--;
	}
	while ((i!=0) && !(n&0x01) && !(n&waitFor));
 
	ClearBitMask(BitFramingReg,0x80);

	if (i!=0)
	{    
		if(!(ReadRawRC(ErrorReg)&0x1B))
		{
			status = MI_OK;
			if (n & irqEn & 0x01)
			{
					status = MI_NOTAGERR;
					printf("Fun:PcdComMF522 status= MI_NOTAGERR \r\n");
			}

			if (Command == PCD_TRANSCEIVE)
			{
				n = ReadRawRC(FIFOLevelReg);
				lastBits = ReadRawRC(ControlReg) & 0x07;
				if (lastBits)
				{   
					*pOutLenBit = (n-1)*8 + lastBits; 
				}
				else
				{ 
					*pOutLenBit = n*8;   
				}
				if (n == 0)
				{  
					n = 1;   
				}
				if (n > MAXRLEN)
				{ 
					n = MAXRLEN;   
				}
				for (i=0; i<n; i++)
				{ 
					pOutData[i] = ReadRawRC(FIFODataReg);  
				}
			}
		}
		else
		{
			status = MI_ERR;
			printf("Fun:PcdComMF522 status= MI_ERR \r\n");
		}
			
	}
 
	SetBitMask(ControlReg,0x80);           // stop timer now
	WriteRawRC(CommandReg,PCD_IDLE); 
	return status;
}

/**********************************************************************
//功    能：防冲撞
//参数说明: pSnr[OUT]:卡片序列号，4字节
//返    回: 成功返回MI_OK
**********************************************************************/  
char PcdAnticoll(unsigned char *pSnr)
{
	char status;
	unsigned char i,snr_check=0;
	unsigned int unLen;
	unsigned char ucComMF522Buf[MAXRLEN]; 
	

	ClearBitMask(Status2Reg,0x08);
	WriteRawRC(BitFramingReg,0x00);
	ClearBitMask(CollReg,0x80);

	ucComMF522Buf[0] = PICC_ANTICOLL1;  //0x93
	ucComMF522Buf[1] = 0x20;

	status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,2,ucComMF522Buf,&unLen);

	if (status == MI_OK)
	{
		for (i=0; i<4; i++)
		{   
			 *(pSnr+i)  = ucComMF522Buf[i];
			 snr_check ^= ucComMF522Buf[i];
		}
		if (snr_check != ucComMF522Buf[i])
		{
			printf("Fun:PcdAnticoll, snr check fail! \r\n");
			status = MI_ERR;    
		}
	}
	
	SetBitMask(CollReg,0x80);
	return status;
}

/**********************************************************************
功    能：选定卡片
参数说明: pSnr[IN]:卡片序列号，4字节
返    回: 成功返回MI_OK
**********************************************************************/
char PcdSelect(unsigned char *pSnr)
{
    char status;
    unsigned char i;
    unsigned int unLen;
    unsigned char ucComMF522Buf[MAXRLEN]; 
    
    ucComMF522Buf[0] = PICC_ANTICOLL1;
    ucComMF522Buf[1] = 0x70;
    ucComMF522Buf[6] = 0;
    for (i=0; i<4; i++)
    {
    	ucComMF522Buf[i+2] = *(pSnr+i);
    	ucComMF522Buf[6]  ^= *(pSnr+i);
    }
    CalulateCRC(ucComMF522Buf,7,&ucComMF522Buf[7]);
  
    ClearBitMask(Status2Reg,0x08);

    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,9,ucComMF522Buf,&unLen);
    
    if ((status == MI_OK) && (unLen == 0x18))
    {   status = MI_OK;   }
    else
    {   status = MI_ERR;    }

    return status;
}

/**********************************************************************
	用MF522计算CRC16函数
**********************************************************************/
void CalulateCRC(unsigned char *pIndata,unsigned char len,unsigned char *pOutData)
{
    unsigned char i,n;
    
    ClearBitMask(DivIrqReg,0x90);   
    WriteRawRC(CommandReg,PCD_IDLE);
    SetBitMask(FIFOLevelReg,0x80);
    for (i=0; i<len; i++)
    {   WriteRawRC(FIFODataReg, *(pIndata+i));   }
    WriteRawRC(CommandReg, PCD_CALCCRC);
    i = 0xFF;
    do 
    {
        n = ReadRawRC(DivIrqReg);
        i--;
    }
    while ((i!=0) && !(n&0x04));
    pOutData[0] = ReadRawRC(CRCResultRegL);
    pOutData[1] = ReadRawRC(CRCResultRegM);
    WriteRawRC(CommandReg,PCD_IDLE);
}

/**********************************************************************
	Send RATS comand accoring to ISO14443-4
**********************************************************************/
char PcdRATS(unsigned char Cid, unsigned char *ucComMF522Buf, unsigned int *unLen)
{
    char status;
    
    ucComMF522Buf[0] = 0xE0;
    ucComMF522Buf[1] = 0x20 | (Cid & 0x0f);    //FSD = 32byte

    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
  
    ClearBitMask(Status2Reg,0x08);

    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,unLen);
    
    if ((status == MI_OK) && *unLen)
    {        
        status = MI_OK;  
    }
    else
    {   status = MI_ERR;    }

    return status;
}

/**********************************************************************
Send DeSelect Command accoring to ISO14443-4
**********************************************************************/
char PcdDeSelect(unsigned char CidSupp, unsigned char Cid, unsigned char *ucComMF522Buf, unsigned int *unLen)
{
	char            status;
	unsigned char   len = 0;

	ucComMF522Buf[0] = 0xc2 | (CidSupp << 3);    //S-block, CID, Deselect
	if(CidSupp) ucComMF522Buf[1] = 0x00 | (Cid & 0x0f);   
	len = 1 + CidSupp;

	CalulateCRC(ucComMF522Buf,len,&ucComMF522Buf[len]);
	len +=2;

	ClearBitMask(Status2Reg,0x08);    
	status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,len,ucComMF522Buf,unLen);
	if ((status == MI_OK) && *unLen)
	{          
		status = MI_OK;  
	}
	else
	{  
		status = MI_ERR; 
	}

	return status;  
}
/**********************************************************************
//Send PPSS comand accoring to ISO14443-4
**********************************************************************/
char PcdPPSS(unsigned char TxSpeed, unsigned char RxSpeed, unsigned char Cid, unsigned char *ucComMF522Buf, unsigned int *unLen)
{
    char status;
    
    ucComMF522Buf[0] = 0xD0 | (Cid & 0x0f); 
    ucComMF522Buf[1] = 0x11;
    ucComMF522Buf[2] = TxSpeed | (RxSpeed << 2);

    CalulateCRC(ucComMF522Buf,3,&ucComMF522Buf[3]);
  
    ClearBitMask(Status2Reg,0x08);

    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,5,ucComMF522Buf,unLen);
    
    if ((status == MI_OK) && (*unLen == 24))
    {           
        WriteRawRC(TxModeReg, (TxSpeed << 4));
		if(RxSpeed > 0)
		{
	        WriteRawRC(RxModeReg, (RxSpeed << 4));
	        WriteRawRC(RxThresholdReg, 0x00);
		}
        status = MI_OK;  
    }
    else
    {   status = MI_ERR;    }

    return status;
}
/**********************************************************************
Send IBlock data accoring to ISO14443-4
**********************************************************************/
char PcdIBlockTrans(unsigned char CidSupp, unsigned char *pDataI, unsigned char Cid, unsigned char InLen, unsigned char *ucComMF522Buf, unsigned int *unLen)
{
	char status;
	unsigned char i = 0, len ;
	unsigned int TotalLen = 0;

	ucComMF522Buf[0] = 0x02 | (CidSupp << 3) | block_num;    //I-block, CID, no chain
	if(CidSupp) ucComMF522Buf[1] = 0x00 | (Cid & 0x0f);   
	len = 1 + CidSupp;

	for (i = 0; i<InLen; i++)    //information
	{    
		ucComMF522Buf[i+len] = *(pDataI+i);   
	}
	len += InLen;
	
	CalulateCRC(ucComMF522Buf,len,&ucComMF522Buf[len]);
	len += 2;
	
	ClearBitMask(Status2Reg,0x08);       
	status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,len,ucComMF522Buf, unLen);

	if ((status == MI_OK) && *unLen)
	{
		if((ucComMF522Buf[0] & 0x01) == block_num) 
			block_num ^= 0x01;
		while((ucComMF522Buf[TotalLen/8] & 0xf0) == 0x10) 
		{
			TotalLen += *unLen;
			delay_us(25);
			status = PcdRBlockTrans(CidSupp, Cid, &ucComMF522Buf[TotalLen/8], unLen);
			
			if(status != MI_OK)
			{   		    
				return MI_ERR;
			}
		}
		*unLen += TotalLen;
	}
	else
	{  
		status = MI_ERR;   
	}
	return status;
}

/**********************************************************************
Send RBlock data accoring to ISO14443-4
**********************************************************************/
char PcdRBlockTrans(unsigned char CidSupp, unsigned char Cid, unsigned char *ucComMF522Buf, unsigned int *unLen)
{
	char            status;
    unsigned char   len;

	ucComMF522Buf[0] = 0xa2 | (CidSupp << 3) | block_num;    //R-block, CID
    if(CidSupp) ucComMF522Buf[1] = 0x00 | (Cid & 0x0f);   
	len = 1 + CidSupp;
    
    CalulateCRC(ucComMF522Buf,len,&ucComMF522Buf[len]);
    len += 2;
    
    ClearBitMask(Status2Reg,0x08);  
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,len,ucComMF522Buf, unLen);
	
	if ((status == MI_OK) && *unLen)
    {         
        if((ucComMF522Buf[0] & 0x01) == block_num) block_num ^= 0x01;
        status = MI_OK;  
    }
    else
    {   status = MI_ERR;    }
    
    return status;
}

/**********************************************************************
功    能：验证卡片密码
参数说明: auth_mode[IN]: 密码验证模式
                 0x60 = 验证A密钥
                 0x61 = 验证B密钥 
          addr[IN]：块地址
          pKey[IN]：密码
          pSnr[IN]：卡片序列号，4字节
返    回: 成功返回MI_OK
**********************************************************************/               
char PcdAuthState(unsigned char auth_mode,unsigned char addr,unsigned char *pKey,unsigned char *pSnr)
{
	char status;
	unsigned int unLen;
	unsigned char i,ucComMF522Buf[MAXRLEN]; 

	ucComMF522Buf[0] = auth_mode;
	ucComMF522Buf[1] = addr;
	for (i=0; i<6; i++)
	{   
		ucComMF522Buf[i+2] = *(pKey+i); 
	}
	for (i=0; i<6; i++)
	{    
		ucComMF522Buf[i+8] = *(pSnr+i);  
	}
	
//	status = PcdComMF522(PCD_AUTHENT,ucComMF522Buf,12,ucComMF522Buf,&unLen);
	status = PcdComMF522(PCD_AUTHENT,ucComMF522Buf,14,ucComMF522Buf,&unLen); //debug
	if ((status != MI_OK) || (!(ReadRawRC(Status2Reg) & 0x08)))
	{ 
		status = MI_ERR;  
	}
	
	return status;
}

/**********************************************************************
功    能：读取M1卡一块数据
参数说明: addr[IN]：块地址
          pData[OUT]：读出的数据，16字节
返    回: 成功返回MI_OK
**********************************************************************/ 
char PcdRead(unsigned char addr,unsigned char *pData)
{
	char status;
	unsigned int unLen;
	unsigned char i,ucComMF522Buf[MAXRLEN]; 

	ucComMF522Buf[0] = PICC_READ;
	ucComMF522Buf[1] = addr;
	CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
 
	status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);
	if ((status == MI_OK) && (unLen == 0x90))
	{
		for (i=0; i<16; i++)
		{    
			*(pData+i) = ucComMF522Buf[i];  
		}
	}
	else
	{  
		status = MI_ERR;   
	}
	
	return status;
}

/**********************************************************************
功    能：写数据到M1卡一块
参数说明: addr[IN]：块地址
          pData[IN]：写入的数据，16字节
返    回: 成功返回MI_OK
**********************************************************************/                 
char PcdWrite(unsigned char addr,unsigned char *pData)
{
	char status;
	unsigned int unLen;
	unsigned char i,ucComMF522Buf[MAXRLEN]; 
	
	ucComMF522Buf[0] = PICC_WRITE;
	ucComMF522Buf[1] = addr;
	CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);

	status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);

	if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
	{   status = MI_ERR;   }
			
	if (status == MI_OK)
	{
			for (i=0; i<16; i++)
			{    
				ucComMF522Buf[i] = *(pData+i);   
			}
			CalulateCRC(ucComMF522Buf,16,&ucComMF522Buf[16]);

			status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,18,ucComMF522Buf,&unLen);
			if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
			{   status = MI_ERR;   }
	}
	
	return status;
}


/**********************************************************************
功    能：命令卡片进入休眠状态
返    回: 成功返回MI_OK
**********************************************************************/
char PcdHalt(void)
{
//    char status;
	unsigned int unLen;
	unsigned char ucComMF522Buf[MAXRLEN]; 

	ucComMF522Buf[0] = PICC_HALT;
	ucComMF522Buf[1] = 0;
	CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
 #if 0
//    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);
	#else
		PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);
	#endif
	
	return MI_OK;
}


void set_crtxldosel(unsigned char step)
{
	unsigned int temp =	REG_APB3_ICU_ANA2_CTRL;
	
	temp &= (~(0x07U<<26));
	temp |= ((step&0x07)<<26);
	
	REG_APB3_ICU_ANA2_CTRL = temp;
}



/***********************  END OF FILES  ***********************/
