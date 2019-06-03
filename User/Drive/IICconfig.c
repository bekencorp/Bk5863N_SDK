#include "IICconfig.h"
#include "Includes.h"

I2cMsg_t *pI2cMsg[2];

/********************************************************
Function: BK5863 I2c Initial
Parameter: 
      I2cIdx: Index of I2c
 pI2cMsgPara: data structure of I2C property
********************************************************/
void BK5863I2cInit(unsigned char I2cIndex, I2cMsg_t *pI2cMsgPara)
{
	switch(I2cIndex)
	{
		case 0x0:       
				IICClk(IIC1,Enable);//open i2c1 clock
				REG_APB7_GPIOD_CFG &= 0xfffffffc; //gpiod[1:0] 2nd func enable
				IICIntCmd(IIC1,Enable); //open i2c1 int enable
				#if 1
				  printf("Role = %02x\r\n",pI2cMsgPara->Role);
				  printf("LocalAddr = %02x\r\n",pI2cMsgPara->LocalAddr);
				  printf("ClkDiv = %02x\r\n",pI2cMsgPara->ClkDiv);
				#endif
				REG_APBE_I2C1_CN = 0xac000023 | (pI2cMsgPara->Role << 30) \
																			| (pI2cMsgPara->LocalAddr << 16) \
																			| (pI2cMsgPara->ClkDiv << 6);
				REG_APBE_I2C1_STAT = (pI2cMsgPara->Role << 6);
		printf("REG_APBE_I2C1_CN = %08x\r\n",REG_APBE_I2C1_CN);
				break;
		case 0x1:
				IICClk(IIC2,Enable); //open i2c2 clock
				REG_APB7_GPIOD_CFG &= 0xfffffff3; //gpiod[3:2] 2nd func enable
				IICIntCmd(IIC2,Enable);  //open i2c2 int enable
				REG_APBF_I2C2_CN = 0xac000023 | (pI2cMsgPara->Role << 30) \
																			| (pI2cMsgPara->LocalAddr << 16)\
		                                  | (pI2cMsgPara->ClkDiv << 6);
				REG_APBF_I2C2_STAT = (pI2cMsgPara->Role << 6);
				break;    
		default:
				break;
	}
	pI2cMsg[I2cIndex] = pI2cMsgPara;
}

void I2C1Init(void)
{
	IICPara_T i2c1conf;
	
	IICClk(IIC1,Enable);//open i2c1 clock
	REG_APB7_GPIOD_CFG &= 0xfffffffc; //gpiod[1:0] 2nd func enable
	IICIntCmd(IIC1,Enable); //open i2c1 int enable
	
	i2c1conf.ENSMB = Enable;
	i2c1conf.INH = 1;    //master or slave 1:Disable Slave
	i2c1conf.SMBFTE = 1;
	i2c1conf.SMBTOE = 0;
	i2c1conf.SMBCS = _InternalClock; //2bit
	i2c1conf.SLV_ADDR = 0x16; //10bit
	i2c1conf.FREQ_DIV = 0x1F;  //10bit
	i2c1conf.SCL_CR = 0x04;
	i2c1conf.IDLE_CR = 0x03;
	
	IICConfig(IIC1,&i2c1conf);
}
/********************************************************
Function: BK5863 I2c Disable
Parameter:  
       I2cIdx: Index of I2c
********************************************************/
void BK5863I2cDisable(unsigned char I2cIdx)
{
	switch(I2cIdx)
	{
		case 0x0: 
				REG_APBE_I2C1_CN   &= ~I2C_EN_BIT;
				break;
		case 0x1: 
				REG_APBF_I2C2_CN &= ~I2C_EN_BIT;
				break;
		default:
				break;
	}
}
/********************************************************
Function: BK5863 I2c Reset
Parameter:  
       I2cIdx: Index of I2c
********************************************************/
void BK5863I2cReset(unsigned char I2cIDX)
{  
	pI2cMsg[I2cIDX]->RxBuff.RdPtr = 0;
	pI2cMsg[I2cIDX]->RxBuff.WrPtr = 0;
	pI2cMsg[I2cIDX]->RxBuff.VldDataNum = 0;
	pI2cMsg[I2cIDX]->RxBuff.LastPkt= 0;
	pI2cMsg[I2cIDX]->TxBuff.RdPtr = 0;
	pI2cMsg[I2cIDX]->TxBuff.WrPtr = 0;
	pI2cMsg[I2cIDX]->TxBuff.VldDataNum = 0;
	pI2cMsg[I2cIDX]->TxBuff.LastPkt= 0;
}

/********************************************************
Function: BK5863 I2c Start Send in Master Role
Parameter: 
       I2cIdx: Index of I2c
 AddrByteData: slave address and operate code
********************************************************/
void BK5863I2cSendStart(unsigned char i2cIdx, unsigned char AddrByteData)
{
	switch(i2cIdx)
	{
		case 0x0: 
			REG_APBE_I2C1_DAT   = AddrByteData; //address byte
			REG_APBE_I2C1_STAT |= I2C_STA_BIT;  //set sta
			break;
		case 0x1: 
			REG_APBF_I2C2_DAT   = AddrByteData; //address byte
			REG_APBF_I2C2_STAT |= I2C_STA_BIT;  //set sta
			break;
		default:
			break;
	}
}

/**************************************************
FunctionName:  BK5863I2cIntService
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
void BK5863I2cIntService(unsigned char I2cIndx)
{
//    unsigned long *pI2cCN;
	unsigned long *pI2cStat, *pI2cData;
	unsigned int I2cStat, DataNum,i;
	DataBuff_t *pTxBuff, *pRxBuff;
	
	#if 0
		pI2cCN   = (unsigned long*)(APBE_I2C1_BASE + 0x1000 * I2cIdx + 0x00);
	#endif
	
	pI2cStat = (unsigned long*)(APBE_I2C1_BASE + 0x1000 * I2cIndx + 0x04);
	pI2cData = (unsigned long*)(APBE_I2C1_BASE + 0x1000 * I2cIndx + 0x08);

	pTxBuff = &(pI2cMsg[I2cIndx]->TxBuff);  //获取待发送的数据
	pRxBuff = &(pI2cMsg[I2cIndx]->RxBuff);
	
	I2cStat = *pI2cStat;
	if(I2cStat & I2C_INT_BIT) //I2C int assert              
	{
		if(I2cStat & I2C_MAST_BIT) //master role
		{
			if(I2cStat & I2C_STA_BIT) //after address byte
			{
				I2cStat &= ~I2C_STA_BIT;  //clear start
				if(!(I2cStat&I2C_ACK_BIT)) //receive no-ack
				{
					I2cStat |= I2C_STO_BIT;  //send stop
				}
//				else 
//				{
				if(I2cStat&I2C_TXMODE_BIT)
					I2cStat &= ~I2C_INTMODE_BIT; //set int mode to 0x00
				else
					I2cStat |= I2C_INTMODE_BIT; //set int mode to 0x03
//				}
			}
									
			if(I2cStat&I2C_TXMODE_BIT)  //tx mode
			{
				if(pTxBuff->VldDataNum == 0) //all data send done
				{
					if(I2cIndx == 0)  //iic1
					{
						#ifdef I2C1_REPEAT_START
						//add your code to here
						
						#else
							I2cStat |= I2C_STO_BIT;  //send stop
						#endif
					}
					else  //iic2
					{
						#ifdef I2C2_REPEAT_START
							//add your code to here
						
						#else
							I2cStat |= I2C_STO_BIT;             //send stop
						#endif
					}           
				}
				else
				{
					DataNum = MIN(pTxBuff->VldDataNum, 16);
					for(i = 0; i < DataNum; i ++)
					{
						*pI2cData = pTxBuff->pBuf[pTxBuff->RdPtr++];  //发送数据
						if(pTxBuff->RdPtr == I2C_BUF_LEN) 
							pTxBuff->RdPtr = 0;
						pTxBuff->VldDataNum --;
					}    
				}
			}
			else //rx mode
			{
				I2cStat |= I2C_ACK_BIT;  //send ack
				while(!(*pI2cStat&I2C_RXFIFO_EMPTY_BIT))
				{
					pRxBuff->pBuf[pRxBuff->WrPtr++] = *pI2cData;
					if(pRxBuff->WrPtr == I2C_BUF_LEN) 
						pRxBuff->WrPtr = 0;
					pRxBuff->VldDataNum ++;
					pI2cMsg[I2cIndx]->RemainLen --;
				}          
				if(pI2cMsg[I2cIndx]->RemainLen <= 0)
				{
					I2cStat &= ~I2C_ACK_BIT;   //send no-ack
					I2cStat |= I2C_STO_BIT;  //send stop
				}
			}
		}
		else   //slave role
		{
			if(I2cStat & I2C_ACKRQ_BIT)
				I2cStat |= I2C_ACK_BIT;                     //send ack

			if(I2cStat & I2C_TXMODE_BIT)                    //tx mode
			{
				//if(I2cStat & I2C_ADDRMATCH_BIT)
				//    BK5863I2cSlaveTxDataPrepare(I2cIdx);    //prepare tx data
				*pI2cData = pTxBuff->pBuf[pTxBuff->RdPtr++];
				if(pTxBuff->RdPtr == I2C_BUF_LEN)
					pTxBuff->RdPtr = 0;
				pTxBuff->VldDataNum --;
			}
			else                                            //rx mode
			{
				while(!(*pI2cStat & I2C_RXFIFO_EMPTY_BIT))
				{
					pRxBuff->pBuf[pRxBuff->WrPtr++] = *pI2cData;
					if(pRxBuff->WrPtr == I2C_BUF_LEN) pRxBuff->WrPtr = 0;
					pRxBuff->VldDataNum ++;
				}  
			}
		}
		I2cStat &= ~I2C_INT_BIT; //clear int
		*pI2cStat = I2cStat;        
	}
}



