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
	
				/* 配置SPI IO */
	REG_APB3_ICU_SPI1CLKCON = 0x02; //open spi1 clock
  REG_APB7_GPIOE_CFG     &= 0xFFFFFFF0;	    //SPI1 2nd func enable
				/* 配置SPI功能 */
	SPIpara.SPIEN = Enable;  //SPI使能
  SPIpara.MSTEN = 1;  //1主方式，0从方式
  SPIpara.CKPHA = 0;
  SPIpara.CKPOL = 0;
  SPIpara.BIT_WDTH = 0; //数据位宽 0:8bit  1:16bit
  SPIpara.NSSMD = 10; //从选择方式位 1
  SPIpara.SPI_CKR = SPI_CKR4; //SPI时钟速率
  SPIpara.RxINT_EN = Disable;
  SPIpara.TxINT_EN = Disable;
  SPIpara.RxOVR_EN = Disable;
  SPIpara.TXOVR_EN = Disable;
  SPIpara.RXINT_mode = RxFIFO12ByteINT; //接收中断产生方式
  SPIpara.TxINT_MODE = TxFIFO1ByteINT;
	SPIpara.DelaySLK = 0x08;
	SPIConfig(SPI1,&SPIpara);
	/* 选择时钟源 */
	SelectSPIClockSource(SPI1,_SPIXTALClock);
	/* 打开SPI中断源 */
	
	/* 开SPI时钟 */
	SPIClock(SPI1,Enable);
//	REG_APB3_ICU_SPI1CLKCON = 0x02;//open spi1 Clock XTAL 32MHz
}
/**************************************************
FunctionName:
Describe: 配置BK5824控制IO
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
	GIPOConfig.GPIO_PERI = GPIO_General_Function;  //普通功能
	GIPOConfig.GPIO_OE_N_INTERN = Enable;  //输出使能
	GIPOConfig.GPIO_PULLUP = Disable;  //上拉使能
	GIPOConfig.GPIO_PULLDOWN = Disable;  //下拉使能
	GIPOConfig.GPIO_I_IE = Disable;  //输入使能
	
	GIPOConfig.Int.isINT = Disable;    //配置中断
	GIPOConfig.GPIO_Pin = GPIO_Pin_3;  //选择GPIO_Pin
	
	GPIO_Config(GPIOC,&GIPOConfig);
  ResetGPIOPin(GPIOC,GPIO_Pin_3);  //ce =1
	
		/* IRQ C4 */
	GIPOConfig.GPIO_OE_N_INTERN = Disable;  //输出使能
	GIPOConfig.GPIO_I_IE = Enable;  //输入使能
	GIPOConfig.GPIO_PULLUP = Enable;  //上拉使能
	
	GIPOConfig.Int.isINT = Enable;    //配置中断
	GIPOConfig.Int.En_Dis_able = Enable;  //GPIO_Pin中断
	GIPOConfig.Int.INTMode = edge_Trigging;  //边沿触发
	GIPOConfig.Int.dpulse = _SPULSE;  //单沿触发
	GIPOConfig.Int.INTtrigger = falling_edge_Tringging;
  GIPOConfig.GPIO_Pin = GPIO_Pin_4;  //选择GPIO_Pin
	GPIO_Config(GPIOC,&GIPOConfig);
	SetGPIOPin(GPIOC,GPIO_Pin_4);
	
		/* WKO C5 */
	GIPOConfig.GPIO_PERI = GPIO_General_Function;  //普通功能
	GIPOConfig.GPIO_OE_N_INTERN = Disable;  //输出使能
	GIPOConfig.GPIO_I_IE = Enable;  //输入使能
	GIPOConfig.GPIO_PULLUP = Enable;  //上拉使能
	GIPOConfig.GPIO_PULLDOWN = Disable;  //下拉使能
	
	GIPOConfig.Int.isINT = Enable;    //配置中断
	GIPOConfig.Int.En_Dis_able = Enable;  //GPIO_Pin中断
	GIPOConfig.Int.INTMode = edge_Trigging; 
	GIPOConfig.Int.dpulse = _SPULSE;  //单沿触发
	GIPOConfig.Int.INTtrigger = Rising_edge_Trigging;
	GIPOConfig.GPIO_Pin = GPIO_Pin_5;  //选择GPIO_Pin
	GPIO_Config(GPIOC,&GIPOConfig);
	SetGPIOPin(GPIOC,GPIO_Pin_5);
		/* GPIO中断唤醒 */
	SetGPIOINT_WAKENA(Enable);
	SetGPIOPin(GPIOC,GPIO_Pin_3);  //ce =1
		/* 使能GPIO中断源 */
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
Describe:  检测是否IRQ中断，接收数据和清除IRQ中断信号
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
	  如果IRQ为低，则读取BK5823状态，若存在数据，读取数据。清除中断(irq恢复高)
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
		/* 错误处理 */
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


