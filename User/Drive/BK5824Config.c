#include "BK5824Config.h"
#include "Includes.h"
#include "obu_app.h"
struct BK5824SysFlag BK5824Stat;

/* default value: max power, 5.8Ghz, 14k 3times,*/
unsigned char  bk_config_flag[BK_CONFIG_FLAG_MAX]= {0x07,0x01,3,1}; 
/**************************************************
FunctionName:
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
void BK5824Config(void)
{
#if BK5824_REG_LEN_OPTION == BK5824_NEW_REG_LEN_OPTION
	set_bk5824_reg_write_len(16,1);
	set_bk5824_reg_read_len(16,4);
#elif BK5824_REG_LEN_OPTION == BK5824_REG_LEN_OPTION_PREV
	unsigned char i;
	BK5824SPIConfig();	
	for(i = 0; i < 21; i ++)
	{
		if(i == 16) BK5824RegWriteLen[i] = 1;
		else if(i == 17) BK5824RegWriteLen[i] = 14;	
		else if(i == 18) BK5824RegWriteLen[i] = 11;	
		else if(i == 19 || i == 20) BK5824RegWriteLen[i] = 0;
		else BK5824RegWriteLen[i] = 4;
		
    	if(i == 16) BK5824RegReadLen[i] = 4;
		else if(i < 9) BK5824RegReadLen[i] = 4;
		else if(i == 17) BK5824RegReadLen[i] = 14;	
		else if(i == 18) BK5824RegReadLen[i] = 11;	
		else BK5824RegReadLen[i] = 4;
	}
#else
	#err1 ...
#endif
}

/**************************************************
FunctionName:
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
void BK5824SPIConfig(void)
{
	BK5863SPI1Config();
}
/**************************************************
FunctionName:
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
void BK5863SPI1Config(void)
{
	SPIPara_T SPIpara;
	
				/* ����SPI IO */
	REG_APB3_ICU_SPI1CLKCON = 0x02; //open spi1 clock
  REG_APB7_GPIOE_CFG     &= 0xFFFFFFF0;	    //SPI1 2nd func enable
				/* ����SPI���� */
	SPIpara.SPIEN = Enable;  //SPIʹ��
  SPIpara.MSTEN = 1;  //1����ʽ��0�ӷ�ʽ
  SPIpara.CKPHA = 0;
  SPIpara.CKPOL = 0;
  SPIpara.BIT_WDTH = 0; //����λ�� 0:8bit  1:16bit
  SPIpara.NSSMD = 10; //��ѡ��ʽλ 1
  SPIpara.SPI_CKR = SPI_CKR4; //SPIʱ������
  SPIpara.RxINT_EN = Disable;
  SPIpara.TxINT_EN = Disable;
  SPIpara.RxOVR_EN = Disable;
  SPIpara.TXOVR_EN = Disable;
  SPIpara.RXINT_mode = RxFIFO12ByteINT; //�����жϲ�����ʽ
  SPIpara.TxINT_MODE = TxFIFO1ByteINT;
	SPIpara.DelaySLK = 0x08;
	SPIConfig(SPI1,&SPIpara);
	/* ѡ��ʱ��Դ */
	SelectSPIClockSource(SPI1,_SPIXTALClock);
	/* ��SPI�ж�Դ */
	
	/* ��SPIʱ�� */
	SPIClock(SPI1,Enable);
//	REG_APB3_ICU_SPI1CLKCON = 0x02;//open spi1 Clock XTAL 32MHz
}
/**************************************************
FunctionName:
Describe: ����BK5824����IO
					CE -- GPIOC3
					IRQ -- GPIOC4
					WKO -- GPIOC5
					GPIO2 --GPIOC1  DCLK
					GPIO3 --GPIOC2  DIO
InputParameter:
OutputParameter:
Notice:
**************************************************/
void Bk5823GPIOConfig(void)
{
	GPIOConfig_T GIPOConfig;
		/* CE */
	GIPOConfig.GPIO_PERI = GPIO_General_Function;  //��ͨ����
	GIPOConfig.GPIO_OE_N_INTERN = Enable;  //���ʹ��
	GIPOConfig.GPIO_PULLUP = Disable;  //����ʹ��
	GIPOConfig.GPIO_PULLDOWN = Disable;  //����ʹ��
	GIPOConfig.GPIO_I_IE = Disable;  //����ʹ��
	
	GIPOConfig.Int.isINT = Disable;    //�����ж�
	GIPOConfig.GPIO_Pin = GPIO_Pin_3;  //ѡ��GPIO_Pin
	
	GPIO_Config(GPIOC,&GIPOConfig);
  ResetGPIOPin(GPIOC,GPIO_Pin_3);  //ce =1
	
		/* IRQ C4 */
	GIPOConfig.GPIO_OE_N_INTERN = Disable;  //���ʹ��
	GIPOConfig.GPIO_I_IE = Enable;  //����ʹ��
	GIPOConfig.GPIO_PULLUP = Enable;  //����ʹ��
	
	GIPOConfig.Int.isINT = Enable;    //�����ж�
	GIPOConfig.Int.En_Dis_able = Enable;  //GPIO_Pin�ж�
	GIPOConfig.Int.INTMode = edge_Trigging;  //���ش���
	GIPOConfig.Int.dpulse = _SPULSE;  //���ش���
	GIPOConfig.Int.INTtrigger = falling_edge_Tringging;
  GIPOConfig.GPIO_Pin = GPIO_Pin_4;  //ѡ��GPIO_Pin
	GPIO_Config(GPIOC,&GIPOConfig);
	SetGPIOPin(GPIOC,GPIO_Pin_4);
	
		/* WKO C5 */
	GIPOConfig.GPIO_PERI = GPIO_General_Function;  //��ͨ����
	GIPOConfig.GPIO_OE_N_INTERN = Disable;  //���ʹ��
	GIPOConfig.GPIO_I_IE = Enable;  //����ʹ��
	GIPOConfig.GPIO_PULLUP = Enable;  //����ʹ��
	GIPOConfig.GPIO_PULLDOWN = Disable;  //����ʹ��
	
	GIPOConfig.Int.isINT = Enable;    //�����ж�
	GIPOConfig.Int.En_Dis_able = Enable;  //GPIO_Pin�ж�
	GIPOConfig.Int.INTMode = edge_Trigging; 
	GIPOConfig.Int.dpulse = _SPULSE;  //���ش���
	GIPOConfig.Int.INTtrigger = Rising_edge_Trigging;
	GIPOConfig.GPIO_Pin = GPIO_Pin_5;  //ѡ��GPIO_Pin
	GPIO_Config(GPIOC,&GIPOConfig);
	SetGPIOPin(GPIOC,GPIO_Pin_5);
		/* GPIO�жϻ��� */
	SetGPIOINT_WAKENA(Enable);
	SetGPIOPin(GPIOC,GPIO_Pin_3);  //ce =1
		/* ʹ��GPIO�ж�Դ */
//	SetGPIOIntCmd(Enable);
}
/**************************************************
FunctionName: BK5824RegConfig
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
void BK5824RegConfig(uint32_t* reg_0_15,uint8_t* reg_17,uint8_t* reg_18)
{
	int i;
	for(i = 0; i < 16; i++)
	{
	  BK5824SpiWrite(i, (unsigned char *)&reg_0_15[i]);
	}
	BK5824SpiWrite(17, (unsigned char *) reg_17);
	BK5824SpiWrite(18, (unsigned char *) reg_18);    
}
/**************************************************
FunctionName:  BK5824Init
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
void BK5824Init(void)
{
	BK5824Config();
	Bk5823GPIOConfig();
	#if 0
	bk_test_single();
	#elif 0
		bk_test_obu0Wakeup();
	#elif 1
		bk_test_obu0TRx();
	#elif 0
		bk_test_rsu();
		BK5824PowerUp();
		bk5824_crc_en(1);
	#endif
}

/**************************************************
FunctionName:  DSRCCheck_and_RecMeg
Describe:  ����Ƿ�IRQ�жϣ��������ݺ����IRQ�ж��ź�
InputParameter:
OutputParameter:
Notice:
**************************************************/
void DSRCCheck_and_RecMeg(void)
{
	unsigned char *RecDat = 0,Len = 0,status;
	uint8_t Temp[150];
	uint8_t i;
	if(BK5824Stat.BK5824WKOFlag)
	{ 
		BK5824Stat.BK5824WKOFlag = FALSE;
//		printf("WKO INT\r\n");
	}
	if(BK5824Stat.BK5824IRQFlag) 
	{	
//		printf("IRQ INT\r\n");
		BK5824Stat.BK5824IRQFlag = FALSE;
		goto ReadIRQ;
	}	
	#if 0
	/* 
	  ���IRQΪ�ͣ����ȡBK5823״̬�����������ݣ���ȡ���ݡ�����ж�(irq�ָ���)
	*/
	if(ReadGPIO_Pin_Data(GPIOC,GPIO_Pin_4))
	{
		if(BK5824Stat.BKIRQIntSta != TRUE)
			BK5824Stat.BK5824IRQOpenInt = TRUE;
		return;
	}
	#endif
ReadIRQ:		
	status = Bk5824IRQdeal(Temp,&Len);
//	status = Bk5824_IRQ_malloc(&RecDat,&Len);
	if(status != BK5824OK)
	{
		/* ������ */
		///printf("BK5824IRQ Err\r\n");
	}
	if(RecDat)
	{
	#if 0
		printf("BK5824Rec = 0x");
		for(i=0;i<Len;i++)
			printf("%02x",RecDat[i]);
		printf("\r\n");
	#endif
	}
	else if(Len)
	{
		obu_rec_msg_handler(Temp,Len);
	#if 0
//		printf("BK5824Rec = 0x");
		for(i=0;i<Len;i++)
			printf("%02x",Temp[i]);
		printf("\r\n");
	#endif
	}
	
	if(RecDat != NULL)
		BKmmgmt_free(RecDat);
	(void)i;
	return;
}

void read_bk5824_reg6(void)
{
	unsigned int reg_value;
	BK5824SpiRead(6, (unsigned char*)&reg_value);
	printf("reg6:%x\r\n",reg_value);
}


