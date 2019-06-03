#include "BKTestAPP.h"
#include "BKCardTest.h"
#include "Includes.h"

/*****************************************************************
	Test CPU Card
*****************************************************************/
char BK5863TestCpuCard(CardReaderMsg_t *pCardReaderMsg)
{
	char status;
	unsigned int  unLen, ii;
	//unsigned char EnterMainDir[7] = {0x00, 0xa4, 0x00, 0x00, 0x02, 0x3f, 0x00};
	unsigned char Enter1001Dir[7] = {0x00, 0xa4, 0x00, 0x00, 0x02, 0x10, 0x01};
	unsigned char Read0015File[5] = {0x00, 0xb0, 0x95, 0x00, 0x2b};
	unsigned char ReadRandom[5] = {0x00, 0x84, 0x00, 0x00, 0x04};
	unsigned char ReadRemain[5] = {0x80, 0x5c, 0x00, 0x02, 0x04};
	unsigned char CidSupp = 0;
	unsigned char DSSupp = 0, DRSupp = 0, DSameSupp = 0;
//	unsigned char FWISupp;
//	unsigned char	SFGISupp;
	unsigned char PPSSReq;
	unsigned char RevBuff[128];

	delay_us(pCardReaderMsg->FrameDly);
	status = PcdRATS(pCardReaderMsg->PiccCid, RevBuff, &unLen);
	if(status != MI_OK)
	{
		delay_us(pCardReaderMsg->FrameDly);
		status=PcdRATS(pCardReaderMsg->PiccCid, RevBuff, &unLen);
		if(status != MI_OK)
		{
			delay_us(pCardReaderMsg->FrameDly);
			status = PcdDeSelect(CidSupp, pCardReaderMsg->PiccCid, RevBuff, &unLen);
			printf("Pcd RATS Communication Failed!\r\n");
			return status;
		}
	} 
	#ifdef DEBUG_OPEN
   // printf("Pcd Receive Respond(RATS):");
   // for(ii = 0; ii < unLen/8; ii ++)
    {
	//		printf("%02x ", RevBuff[ii]);
    }
   // printf("!\r\n");   
	#endif   
          
	delay_us(pCardReaderMsg->FrameDly);
	ii = 1;
	if(RevBuff[1] & 0x10)               //TA(1) Parser
	{
			ii++;
			DSSupp = (RevBuff[ii] >> 3) & 0x0e;
			DRSupp = (RevBuff[ii] << 1) & 0x0e; 
			DSameSupp = (RevBuff[ii] >> 7) & 0x01;
	}
	if(RevBuff[1] & 0x20)               //TB(1) Parser
	{
		ii++;
		#if 0
			FWISupp = (RevBuff[ii] >> 4) & 0x0f;
			SFGISupp = RevBuff[ii] & 0x0f;
		#endif
	}
    
	if(RevBuff[1] & 0x40)               //TC(1) Parser
	{
		ii ++;
		CidSupp = (RevBuff[ii] >> 1) & 0x01;
	}

	if(DSameSupp)
	{
		PPSSReq = (pCardReaderMsg->TxSpeed == pCardReaderMsg->RxSpeed) & 
							(DSSupp & (1 << pCardReaderMsg->TxSpeed)) &
							(DRSupp & (1 << pCardReaderMsg->RxSpeed));
	}
	else
	{
		PPSSReq = (DSSupp & (1 << pCardReaderMsg->TxSpeed)) |
							(DRSupp & (1 << pCardReaderMsg->RxSpeed));
	}
    
	delay_us(pCardReaderMsg->SFGIDly);
    
	if(PPSSReq)
	{
		status = PcdPPSS(pCardReaderMsg->TxSpeed, pCardReaderMsg->RxSpeed, 
									 pCardReaderMsg->PiccCid, RevBuff, &unLen);
		if(status != MI_OK)
		{
			printf("Pcd PPS Communication Failed!\r\n");
			return status;
		}				
		#ifdef DEBUG_OPEN
		//	printf("Pcd Receive Respond(PPS):");
		//for(ii = 0; ii < unLen/8; ii ++)
		//	{
			// printf("%02x ", RevBuff[ii]);
		//	}
		//	printf("!\r\n");   
		#endif
		delay_us(pCardReaderMsg->FrameDly);
	}
	ResetGPIOPin(GPIOF,GPIO_Pin_0);

  //printf("CidSupp = %0d, PPSSReq = %0d!\r\n", CidSupp, PPSSReq);
    
	status = PcdIBlockTrans(CidSupp, &Enter1001Dir[0], pCardReaderMsg->PiccCid, 7, RevBuff, &unLen);

	if(status != MI_OK)
	{
		printf("Pcd Enter 1001 Dir Failed!\r\n");
		return status;
	} 
	ResetGPIOPin(GPIOF,GPIO_Pin_1);

	#ifdef DEBUG_OPEN
    printf("Pcd Receive Respond(Enter1001Dir):");
    for(ii = 0; ii < unLen/8; ii ++)
    {
			printf("%02x ", RevBuff[ii]);
    }
    printf("!\r\n");   
	#endif       
    
	delay_us(pCardReaderMsg->FrameDly);
	
	status = PcdIBlockTrans(CidSupp, &ReadRemain[0], pCardReaderMsg->PiccCid, 5, RevBuff, &unLen);
	if(status != MI_OK)
	{
		printf("Pcd Read Remain Data Failed!\r\n");
		return status;
	} 
    
	#ifdef DEBUG_OPEN
		printf("Pcd Receive Respond(ReadRemain):");
		for(ii = 0; ii < unLen/8; ii ++)
		{
			printf("%02x ", RevBuff[ii]);
		}
		printf("!\r\n");   
	#endif   
    
	delay_us(pCardReaderMsg->FrameDly);

	status = PcdIBlockTrans(CidSupp, &ReadRandom[0], pCardReaderMsg->PiccCid, 5, RevBuff, &unLen);
	if(status != MI_OK)
	{
		printf("Pcd Read Random Data Failed!\r\n");
		return status;
	} 
    
	#ifdef DEBUG_OPEN
		printf("Pcd Receive Respond(ReadRandom):");
		for(ii = 0; ii < unLen/8; ii ++)
		{
				printf("%02x ", RevBuff[ii]);
		}
		printf("!\r\n");   
	#endif   
    
	delay_us(pCardReaderMsg->FrameDly);

	status = PcdIBlockTrans(CidSupp, &Read0015File[0], pCardReaderMsg->PiccCid, 5, RevBuff, &unLen);
	if(status != MI_OK)
	{
		printf("Pcd Read 0015 File Failed!\r\n");
		return status;
	} 
    
	#ifdef DEBUG_OPEN
    printf("Pcd Receive Respond(Read0015File):");
    for(ii = 0; ii < unLen/8; ii ++)
    {
			printf("%02x ", RevBuff[ii]);
    }
    printf("!\r\n");   
	#endif   
    
	delay_us(pCardReaderMsg->FrameDly);
	
	status = PcdDeSelect(CidSupp, pCardReaderMsg->PiccCid, RevBuff, &unLen);
	
	return status;
}
/*****************************************************************
	Test Mifare Classic One Card
*****************************************************************/
char BK5863TestOperateM1Card(CardReaderMsg_t *pCardReaderMsg)
{
	char            status;
//	int             ii;
	unsigned char   RevBuff[32];

	delay_us(pCardReaderMsg->FrameDly);

	status = PcdAuthState(PICC_AUTHENT1A,pCardReaderMsg->BlockNum,pCardReaderMsg->Key,pCardReaderMsg->CardSn);//
	if(status != MI_OK)
	{
		printf("Pcd Authent Failed!\r\n");
		return MI_ERR;
	}
	
	delay_us(pCardReaderMsg->FrameDly);
	if(pCardReaderMsg->OprtMode == READCARD)
		status = PcdRead(pCardReaderMsg->BlockNum, RevBuff);
	else if(pCardReaderMsg->OprtMode == WRITECARD)
		status = PcdWrite(pCardReaderMsg->BlockNum, pCardReaderMsg->pBuff);
	else
		status = MI_ERR;
	
	if(status == MI_OK)
	{
		if(pCardReaderMsg->OprtMode == READCARD)
		{
//			#ifdef DEBUG_OPEN
            int ii;
				printf("Pcd Finish Read(%0dth Block) Operate Successfully!\r\n", pCardReaderMsg->BlockNum);
				printf("Read Data: ");
				for(ii=0;ii<16;ii++)
					printf("%02x ",RevBuff[ii]);
				printf("!\r\n");    
//			#endif
		}
		else
		{
			#ifdef DEBUG_OPEN
				printf("Pcd Finish Write(%0dth Block) Operate Successfully!\r\n", pCardReaderMsg->BlockNum);
			#endif
		}
	}
	else       
		printf("Pcd Opearte Mifare Classic One Card Failed!\r\n");
	
	PcdHalt();
	return status;
}

/*****************************************************************
	Test Mifare Classic One Card
*****************************************************************/
char BK5863TestOperateM1Card_ops(CardReaderMsg_t *pCardReaderMsg,char *msg,unsigned char len)
{
	char            status = 0;
//	int             ii;
	unsigned char   RevBuff[32];
	PcdReset();

	delay_us(pCardReaderMsg->FrameDly);
	printf("BlockNum:%d\r\n",pCardReaderMsg->BlockNum);

	status = PcdAuthState(PICC_AUTHENT1A,pCardReaderMsg->BlockNum,pCardReaderMsg->Key,pCardReaderMsg->CardSn);//
	if(status != MI_OK)
	{
		printf("Pcd Authent Failed!\r\n");
		return MI_ERR;
	}
	
	delay_us(pCardReaderMsg->FrameDly);
	if(pCardReaderMsg->OprtMode == READCARD)
		status = PcdRead(pCardReaderMsg->BlockNum, RevBuff);
	else if(pCardReaderMsg->OprtMode == WRITECARD)
		status = PcdWrite(pCardReaderMsg->BlockNum, pCardReaderMsg->pBuff);
	else
		status = MI_ERR;
	
	if(status == MI_OK)
	{
		if(pCardReaderMsg->OprtMode == READCARD)
		{
//			#ifdef DEBUG_OPEN
            int ii;
				printf("Pcd Finish Read(%0dth Block) Operate Successfully!\r\n", pCardReaderMsg->BlockNum);
				printf("Read Data: ");
				for(ii=0;ii<16;ii++)
					printf("%02x ",RevBuff[ii]);
				printf("!\r\n");  
				if(len > 16) len = 16;
				memcpy(msg,RevBuff,len);
				status = len;
//			#endif
		}
		else
		{
			#ifdef DEBUG_OPEN
				printf("Pcd Finish Write(%0dth Block) Operate Successfully!\r\n", pCardReaderMsg->BlockNum);
			#endif
		}
	}
	else       
		printf("Pcd Opearte Mifare Classic One Card Failed!\r\n");
	
	PcdHalt();
	return status;
}

