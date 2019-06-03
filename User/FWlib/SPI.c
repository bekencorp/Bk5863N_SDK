#include "SPI.h"
#include "BK5863.h"

/**************************************************
FunctionName:SPIConfig
Describe: ����SPI����
InputParameter:
OutputParameter:
Notice:
**************************************************/
void SPIConfig(unsigned char SPIx,SPIPara_T *Cfg)
{
	unsigned int temp = REG_APBA_SPI1_CTRL;
	
	if(Cfg->SPIEN)
		temp |= (1<<23);
	else
		temp &= (~(1<<23));
	
	if(Cfg->MSTEN)  //����ʽ
		temp |= (1<<22);
	else
		temp &= (~(1<<22));
	
	if(Cfg->CKPHA)  //SPIʱ����λ 1�ڶ������ز�������
		temp |= (1<<21);
	else
		temp &= (~(1<<21));
	
	if(Cfg->CKPOL)
		temp |= (1<<20);
	else
		temp &= (~(1<<20));
	
	if(Cfg->BIT_WDTH)  //16bit
		temp |= (1<<18);
	else  //8bit
		temp &= (~(1<<18));
	
	temp &= (~(3<<16));
	temp |= ((Cfg->NSSMD&0x03)<<16);
	
	temp &= (~(0xFF<<8));  //ʱ������
	temp |= ((Cfg->SPI_CKR&0xFF)<<8);
	
	if(Cfg->RxINT_EN)  
		temp |= (1<<7);
	else
		temp &= (~(1<<7));
	
	if(Cfg->TxINT_EN)  
		temp |= (1<<6);
	else
		temp &= (~(1<<6));
	
	if(Cfg->RxOVR_EN)  
		temp |= (1<<5);
	else
		temp &= (~(1<<5));
	
	if(Cfg->TXOVR_EN)  
		temp |= (1<<4);
	else
		temp &= (~(1<<4));
	
	temp &= (~(0x3<<2));  //�����жϲ�����ʽ
	temp |= ((Cfg->RXINT_mode&0x03)<<2);
	
	temp &= (~(0x03<<0));  //���ͷ�ʽ������ʽ
	temp |= ((Cfg->TxINT_MODE&0x03)<<0);
	
	temp &= (~0xFF000000); //�ȴ�����
	temp |= ((Cfg->DelaySLK&0xFF)<<24);
	
	switch(SPIx)
	{
		case SPI1:
			REG_APBA_SPI1_CTRL = temp;
		break;
		case SPI2:
			REG_APBB_SPI2_CTRL = temp;
		break;
		case SPI3:
			REG_APBC_SPI3_CTRL = temp;
		break;
		case SPI4:
			REG_APBD_SPI4_CTRL = temp;
		break;
		default:
			break;
	}
}

/**************************************************
FunctionName:SPIClock
Describe: ����SPIʱ��
InputParameter:
OutputParameter:
Notice:
**************************************************/
void SPIClock(uint8_t SPIx,uint8_t cmd)
{
	switch(SPIx)
	{
		case SPI1:
			if(cmd)
				REG_APB3_ICU_SPI1CLKCON &= (~0x01);              //open spi1 clock
			else
				REG_APB3_ICU_SPI1CLKCON |= 0x01; 
		break;
		case SPI2:
			if(cmd)
				REG_APB3_ICU_SPI2CLKCON &= (~0x01);                //open spi2 clock
			else
				REG_APB3_ICU_SPI2CLKCON |= 0x01; 
		break;
		case SPI3:
			if(cmd)
				REG_APB3_ICU_SPI3CLKCON &= (~0x01);                //open spi3 clock
			else
				REG_APB3_ICU_SPI3CLKCON |= 0x01;  
		break;
		case SPI4:
			if(cmd)
				REG_APB3_ICU_SPI4CLKCON &= (~0x01);                //open spi4 clock
			else
				REG_APB3_ICU_SPI4CLKCON |= 0x01; 
		break;
		default:
			break;
	}
}

/**************************************************
FunctionName:SelectSPIClockSource
Describe: ѡ��SPIʱ��Դ
InputParameter:
OutputParameter:
Notice: ֻ��SPI1��ѡDCO ���� 32MHz Xtal
**************************************************/
void SelectSPIClockSource(uint8_t SPIx,uint8_t source)
{
	switch(SPIx)
	{
		case SPI1:
			if(!source)  //DCO
				REG_APB3_ICU_SPI1CLKCON &= (~0x02);             
			else
				REG_APB3_ICU_SPI1CLKCON |= 0x02; 
		break;
		default:
			break;
	}
}


/***********************  END OF FILES  ***********************/
