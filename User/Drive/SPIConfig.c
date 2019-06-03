#include "SPIConfig.h"
#include "Includes.h"

SpiMsg_t *pSpiMsg[4];
/********************************************************
Function: BK5863 Spi Iniital
Parameter: 
      SpiIdx: Index of Spi
 pSpiMsgPara: data structure of SPI property 
********************************************************/
void BK5863SpiInit(unsigned char SpiIdx, SpiMsg_t *pSpiMsgPara)
{   
	unsigned int    CfgValue;
	
	CfgValue = (pSpiMsgPara->Role << 22) | (pSpiMsgPara->Ckr << 8) | 
						 (pSpiMsgPara->Ckphapol<< 20) | (pSpiMsgPara->Nssmd<< 16);
  CfgValue |= (pSpiMsgPara->WaitCLK << 24);
	if(pSpiMsgPara->Role) 
		CfgValue |= 0x00800002;  //rxint disable
	else
		CfgValue |= 0x0080008e; //rxint enable, rxint mode set 3
	
	switch(SpiIdx)
	{
		case 0x0:
			REG_APB3_ICU_SPI1CLKCON     = 0x0;    //open spi1 clock
			REG_APB7_GPIOE_CFG         &= 0xfffffff0;   //gpioe[3:0] 2nd func enable
			REG_APB3_ICU_INT_ENABLE    |= INT_STATUS_SPI1_bit;  //open spi1 int enable
			REG_APBA_SPI1_CTRL          = CfgValue;
			break;
		case 0x1: 
			REG_APB3_ICU_SPI2CLKCON     = 0x0;                  //open spi2 clock
			REG_APB7_GPIOE_CFG         &= 0xffffff0f;           //gpioe[7:4] 2nd func enable
			REG_APB3_ICU_INT_ENABLE    |= INT_STATUS_SPI2_bit;  //open spi2 int enable
			REG_APBB_SPI2_CTRL          = CfgValue;
			break;
		case 0x2:    
			REG_APB3_ICU_SPI3CLKCON     = 0x0;                  //open spi3 clock
			REG_APB7_GPIOF_CFG         &= 0xfffffff0;           //gpiof[3:0] 2nd func enable
			REG_APB3_ICU_INT_ENABLE    |= INT_STATUS_SPI3_bit;  //open spi2 int enable
			REG_APBC_SPI3_CTRL          = CfgValue;
			break;
		case 0x3: 
			REG_APB3_ICU_SPI4CLKCON     = 0x0;                  //open spi4 clock
			REG_APB7_GPIOF_CFG         &= 0xffffff0f;           //gpiof[7:4] 2nd func enable
			REG_APB3_ICU_INT_ENABLE    |= INT_STATUS_SPI4_bit;  //open spi2 int enable
			REG_APBD_SPI4_CTRL          = CfgValue;
			break;
		default:
			break;
	}
	
	pSpiMsg[SpiIdx] = pSpiMsgPara;
}

/********************************************************
Function: BK5863 Spi Reset
Parameter:
      SpiIdx: Index of Spi
********************************************************/
void BK5863SpiReset(unsigned char SpiIdx)
{
//	unsigned long        *pSpiCtrl;
//	
//	pSpiCtrl = (unsigned long*)(APBA_SPI1_BASE + 0x1000 * SpiIdx + 0x00);
	
	pSpiMsg[SpiIdx]->RxBuff.RdPtr = 0;
	pSpiMsg[SpiIdx]->RxBuff.WrPtr = 0;
	pSpiMsg[SpiIdx]->RxBuff.VldDataNum = 0;
	pSpiMsg[SpiIdx]->RxBuff.LastPkt= 0;
	pSpiMsg[SpiIdx]->TxBuff.RdPtr = 0;
	pSpiMsg[SpiIdx]->TxBuff.WrPtr = 0;
	pSpiMsg[SpiIdx]->TxBuff.VldDataNum = 0;
	pSpiMsg[SpiIdx]->TxBuff.LastPkt= 0;
}

/********************************************************
Function: BK5863 Spi Disable
Parameter: 
      SpiIdx: Index of Spi
********************************************************/
void BK5863SpiDisable(unsigned char SpiIdx)
{
	unsigned long        *pSpiCtrl;
	
	pSpiCtrl = (unsigned long*)(APBA_SPI1_BASE + 0x1000 * SpiIdx + 0x00);
	
	*pSpiCtrl &= ~SPI_EN_BIT;
}

/********************************************************
Function: BK5863 Spi start send data
Parameter: 
      SpiIdx: Index of Spi
********************************************************/
void BK5863SpiSendStart(unsigned char SpiIdx)
{
	unsigned long        *pSpiCtrl;
	
	pSpiCtrl = (unsigned long*)(APBA_SPI1_BASE + 0x1000 * SpiIdx + 0x00);
	
	if((*pSpiCtrl & SPI_MAST_BIT) && ((*pSpiCtrl & SPI_NSSMD_BIT) == (0x03<<16))) 
			*pSpiCtrl &= 0x00feffff;                    //clear nss
	
	*pSpiCtrl |= 0x42;                              //open tx int enable, set txint mode to 0x02
}

//********************************************************//
//Function: BK5863 Spi Interrupt Service
//Parameter: 
//      SpiIdx: Index of Spi
//********************************************************//
void BK5863SpiIntService(unsigned char SpiIdx)
{
    unsigned long      *pSpiCtrl, *pSpiStat, *pSpiData;
    unsigned int        DataNum;
    DataBuff_t          *pTxBuff, *pRxBuff;
    int                 i;
    
    pSpiCtrl = (unsigned long*)(APBA_SPI1_BASE + 0x1000 * SpiIdx + 0x00);
    pSpiStat = (unsigned long*)(APBA_SPI1_BASE + 0x1000 * SpiIdx + 0x04);
    pSpiData = (unsigned long*)(APBA_SPI1_BASE + 0x1000 * SpiIdx + 0x08);

    pTxBuff = &(pSpiMsg[SpiIdx]->TxBuff);
    pRxBuff = &(pSpiMsg[SpiIdx]->RxBuff);
  
    if(*pSpiStat & SPI_TXINT_BIT)                               //tx int assert
	{	
		if(pTxBuff->VldDataNum == 0)	
		{
			*pSpiCtrl &= 0xffffbf;                              //close txint enable
       if(pTxBuff->LastPkt && (*pSpiCtrl & SPI_MAST_BIT) && ((*pSpiCtrl & SPI_NSSMD_BIT) == (0x02 << 16)))        
		   {
          *pSpiCtrl |= 0x030000;                          //master and 4-wire, set nss                   
		   } 
		}
		else
		{	
		    DataNum = MIN(pTxBuff->VldDataNum, 8);
			for(i = 0; i < DataNum; i++)
			{    			    
				*pSpiData = pTxBuff->pBuf[pTxBuff->RdPtr++];
                if(pTxBuff->RdPtr == SPI_BUF_LEN) pTxBuff->RdPtr = 0; 
			}
            
	        pTxBuff->VldDataNum -= DataNum;
			if(pTxBuff->VldDataNum == 0)	
			{
				*pSpiCtrl &= 0xfffffc;                              //set txint mode to 0x0
			}
		}       
    }
    
    while(!(*pSpiStat & SPI_RXEMPTY_BIT))                       //read data from rx fifo
    {                                                           
        pRxBuff->pBuf[pRxBuff->WrPtr++] = *pSpiData ;	
        if(pRxBuff->WrPtr == SPI_BUF_LEN) pRxBuff->WrPtr = 0; 
        pRxBuff->VldDataNum ++;
	}

    *pSpiStat  |= SPI_TXINT_BIT | SPI_RXINT_BIT;   
}






