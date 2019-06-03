#include "BKCard.h"
#include "BK5863.h"


static unsigned char block_num;


/**********************************************************************
  ���ö����������߼��Ĺ�����ʽ 
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
��    �ܣ���RC522�Ĵ���λ
����˵����reg[IN]:�Ĵ�����ַ
          mask[IN]:��λֵ
**********************************************************************/
void ClearBitMask(unsigned char reg,unsigned char mask)  
{
    char tmp = 0x0;
    tmp = ReadRawRC(reg);
    WriteRawRC(reg, tmp & ~mask);  // clear bit mask
} 

/**********************************************************************
//��    �ܣ���RC522�Ĵ���λ
//����˵����reg[IN]:�Ĵ�����ַ
//          mask[IN]:��λֵ
**********************************************************************/
void SetBitMask(unsigned char reg,unsigned char mask)  
{
	char tmp = 0x0;
	tmp = ReadRawRC(reg);
	WriteRawRC(reg,tmp | mask);  // set bit mask
}

/**********************************************************************
��    �ܣ��ض϶�����
����˵������
**********************************************************************/
void PcdPowerDown(void)
{
  REG_APB3_ICU_ANA2_CTRL     &= ~(1<<29);       //power down card reader
}

/**********************************************************************
��    �ܣ��򿪶�����
����˵������
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
��    �ܣ���������Դ
����˵����Enable��Disable
**********************************************************************/
void PcdPower(unsigned char cmd)
{
  if(cmd)
		PcdPowerOn();
	else
		PcdPowerDown();
}

/**********************************************************************
��    �ܣ���λ�������������߼�
��    ��: �ɹ�����MI_OK
**********************************************************************/
char PcdReset(void)
{
    delay_us(10);
    WriteRawRC(CommandReg,PCD_RESETPHASE);
    delay_us(10);
    
    WriteRawRC(ModeReg,0x3D);            //��Mifare��ͨѶ��CRC��ʼֵ0x6363
    WriteRawRC(TReloadRegL,30);           
    WriteRawRC(TReloadRegH,0);
//	WriteRawRC(TReloadRegH,1);  //debug
    WriteRawRC(TModeReg,0x8D);
    WriteRawRC(TPrescalerReg,0x3E);
		WriteRawRC(TxAutoReg,0x40);     //����Ҫ
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
��    �ܣ�Ѱ��
����˵��: req_code[IN]:Ѱ����ʽ
                0x52 = Ѱ��Ӧ�������з���14443A��׼�Ŀ�
                0x26 = Ѱδ��������״̬�Ŀ�
          pTagType[OUT]����Ƭ���ʹ���
                0x4400 = Mifare_UltraLight
                0x0400 = Mifare_One(S50)
                0x0200 = Mifare_One(S70)
                0x0800 = Mifare_Pro(X)
                0x4403 = Mifare_DESFire
��    ��: �ɹ�����MI_OK
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
��    �ܣ�ͨ��RC522��ISO14443��ͨѶ
����˵����Command[IN]:RC522������
          pInData[IN]:ͨ��RC522���͵���Ƭ������
          InLenByte[IN]:�������ݵ��ֽڳ���
          pOutData[OUT]:���յ��Ŀ�Ƭ��������
          *pOutLenBit[OUT]:�������ݵ�λ����
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
	
	//i = 600;//����ʱ��Ƶ�ʵ���������M1�����ȴ�ʱ��25ms

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
//��    �ܣ�����ײ
//����˵��: pSnr[OUT]:��Ƭ���кţ�4�ֽ�
//��    ��: �ɹ�����MI_OK
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
��    �ܣ�ѡ����Ƭ
����˵��: pSnr[IN]:��Ƭ���кţ�4�ֽ�
��    ��: �ɹ�����MI_OK
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
	��MF522����CRC16����
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
��    �ܣ���֤��Ƭ����
����˵��: auth_mode[IN]: ������֤ģʽ
                 0x60 = ��֤A��Կ
                 0x61 = ��֤B��Կ 
          addr[IN]�����ַ
          pKey[IN]������
          pSnr[IN]����Ƭ���кţ�4�ֽ�
��    ��: �ɹ�����MI_OK
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
��    �ܣ���ȡM1��һ������
����˵��: addr[IN]�����ַ
          pData[OUT]�����������ݣ�16�ֽ�
��    ��: �ɹ�����MI_OK
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
��    �ܣ�д���ݵ�M1��һ��
����˵��: addr[IN]�����ַ
          pData[IN]��д������ݣ�16�ֽ�
��    ��: �ɹ�����MI_OK
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
��    �ܣ����Ƭ��������״̬
��    ��: �ɹ�����MI_OK
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
