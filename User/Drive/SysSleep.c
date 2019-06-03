#include "SysSleep.h"
#include "Includes.h"

/**************************************************
FunctionName: 
Describe: 
InputParameter: 
OutputParameter: 
Notice:
**************************************************/
void DeepSleepConfig(void)
{
	uint32_t CpuFlag = REG_APB3_ICU_IRQTYPE_EN;
	/*  irq disable & FIQ */
	REG_APB3_ICU_IRQTYPE_EN     = 0x00;  	
	/* Close Charge */
	CmdCharge(Disable);
		/* power down card reader */
	REG_APB3_ICU_ANA2_CTRL &= (~(1<<29)); ///power down card reader
	REG_APB3_ICU_ANA1_CTRL  &= (~(1<<28));///  |= (1<<28); //open spibmen
	REG_APB3_ICU_ANA1_CTRL |= (1<<26);
	/* cpu clock switch dco, prepare sleep */
	REG_APB3_ICU_CLOCKSOURCE    = SYSCLK_HFSRC_30M_DCO;
	REG_APB3_ICU_CPUCLKCON    |= (1<<9);
	REG_APB3_ICU_ANA2_CTRL |= (1<<24);
	REG_APB3_ICU_ANA2_CTRL |= (1<<25);
	REG_APB3_ICU_ANA2_CTRL &= (~(1<<8));
	/* CLOSE BK5824 CE = 0 */
	REG_APB7_GPIOC_CFG &= 0xC1C1FFFF;
	REG_APB7_GPIOC_CFG |= 0x00003E3E;
	REG_APB7_GPIOC_DATA |= 0x3E0000;
    REG_APB7_GPIOC_DATA &= 0xfffffC1;	 
	
	/* FLASH CLK */
	set_flash_clk(FLASH_DCO_CLK_32MHz);
  /* XTAL CLOSE 32MHZ XTAL */	
	REG_APB7_GPIOC_DATA        &= 0xfffffffe;
	
	REG_APB3_ICU_IRQTYPE_EN = CpuFlag;
	
	
	REG_APB11_LBD_CLK32K_PWD	   = 0x1;					//close 32kHz
}
/**************************************************
FunctionName: 
Describe: 
InputParameter: 
OutputParameter: 
Notice:
**************************************************/
void test_gpiod_5_uart1_rx(unsigned char cmd)
{
	/* uart1 2nd func enable */
	GPIOConfig_T GIPOConfig;

	if(cmd)
	{
		REG_APB7_GPIOD_CFG |= (1<<5);	
		 /* GPIOD_5 */
		/* �����ʵ��Ӳ������,�ṹ��ֵ���������ȱʡ */
		GIPOConfig.GPIO_PERI = GPIO_General_Function;  //��ͨ����
		GIPOConfig.GPIO_OE_N_INTERN = Disable;  //���ʹ��
		GIPOConfig.GPIO_I_IE = Enable;  //����ʹ��
		GIPOConfig.GPIO_PULLUP = Disable;  //����ʹ��
		GIPOConfig.GPIO_PULLDOWN = Enable;  //����ʹ��
		
		GIPOConfig.Int.isINT = Enable;    //�����ж�
		GIPOConfig.Int.En_Dis_able = Enable;  //GPIO_Pin�ж�
		#if 0  //���ش��������ش��� �����ش���
			GIPOConfig.Int.INTMode = edge_Trigging; //���ش���
			GIPOConfig.Int.dpulse = _SPULSE;  //���ش���
			GIPOConfig.Int.INTtrigger = Rising_edge_Trigging;
		#endif 
		#if 0 //���ش��������ش���  �����ش���
			GIPOConfig.Int.INTMode = edge_Trigging; //���ش���
			GIPOConfig.Int.dpulse = _SPULSE;  //���ش���
			GIPOConfig.Int.INTtrigger = falling_edge_Tringging;
		#endif
		#if 1 //���ش����� ˫���ش���  
			GIPOConfig.Int.INTMode = edge_Trigging; //���ش���
			GIPOConfig.Int.dpulse = _DPULSE;  //˫�ش���
	//		GIPOConfig.Int.INTtrigger = falling_edge_Tringging;
		#endif
		#if 0 //��ƽ����, �ߵ�ƽ����
			GIPOConfig.Int.INTMode = level_Tringging;  //��ƽ����
			GIPOConfig.Int.INTtrigger = High_level_Tringging;
		#endif
		#if 0 //��ƽ����, �͵�ƽ����
			GIPOConfig.Int.INTMode = level_Tringging;  //��ƽ����
			GIPOConfig.Int.INTtrigger = Low_level_Trigging;
		#endif
		GIPOConfig.GPIO_Pin = GPIO_Pin_5;  //ѡ��GPIO_Pin
		
		GPIO_Config(GPIOD,&GIPOConfig);
		SetGPIOPin(GPIOD,GPIO_Pin_5);
		
		/* GPIO�жϻ��� */
		SetGPIOINT_WAKENA(Enable);
		/* ʹ��GPIO�ж�Դ */
		SetGPIOIntCmd(Enable);
	}
	else
	{
		 /* GPIOD_5 */
		GIPOConfig.GPIO_PERI = GPIO_General_Function;  //��ͨ����
		GIPOConfig.GPIO_OE_N_INTERN = Disable;  //���ʹ��
		GIPOConfig.GPIO_I_IE = Disable;  //����ʹ��
		GIPOConfig.GPIO_PULLUP = Disable;  //����ʹ��
		GIPOConfig.GPIO_PULLDOWN = Disable;  //����ʹ��
		
		GIPOConfig.Int.isINT = Disable;    //�����ж�
		GIPOConfig.Int.En_Dis_able = Disable;  //GPIO_Pin�ж�

		GIPOConfig.GPIO_Pin = GPIO_Pin_5;  //ѡ��GPIO_Pin
		
		GPIO_Config(GPIOD,&GIPOConfig);

		REG_APB7_GPIOD_CFG &= 0xffffffcf;	
	}
}

void OnlyRFRouseConfig(void)
{
	uint32_t CpuFlag = REG_APB3_ICU_IRQTYPE_EN;
	/*  irq disable & FIQ disable */
	REG_APB3_ICU_IRQTYPE_EN     = 0x00;  	
	/* Close Charge */
	CmdCharge(Disable);
			/* power down card reader */
	REG_APB3_ICU_ANA2_CTRL &= (~(1<<29)); ///power down card reader
	REG_APB3_ICU_ANA1_CTRL  &= (~(1<<28));///  |= (1<<28); //open spibmen
	REG_APB3_ICU_ANA1_CTRL |= (1<<26);

	/* CLOSE BK5824 0WK Mode */
	#if 1
    	BK5824_SoftWakeup_toggle();
	#else
	   bk_test_obu0Wakeup();
	#endif
/* cpu clock switch dco, prepare sleep */
	REG_APB3_ICU_CLOCKSOURCE    = SYSCLK_HFSRC_30M_DCO;
	REG_APB3_ICU_CPUCLKCON    |= (1<<9);
	REG_APB3_ICU_ANA2_CTRL |= (1<<24);
	REG_APB3_ICU_ANA2_CTRL |= (1<<25);
	REG_APB3_ICU_ANA2_CTRL &= (~(1<<8));
	/* FLASH CLK */
	set_flash_clk(FLASH_DCO_CLK_32MHz);
  /* XTAL CLOSE 32MHZ XTAL */	
	REG_APB7_GPIOC_DATA        &= 0xfffffffe;
	test_gpiod_5_uart1_rx(1);
	REG_APB3_ICU_IRQTYPE_EN = CpuFlag;

	REG_APB11_LBD_CLK32K_PWD	   = 0x1;					//close 32kHz
}	
/**************************************************
FunctionName: 
Describe: 
InputParameter: 
OutputParameter: 
Notice:
**************************************************/
void OnlyChargeConfig(void)
{
  uint32_t CpuFlag = REG_APB3_ICU_IRQTYPE_EN;
	/*  irq disable & FIQ */
	REG_APB3_ICU_IRQTYPE_EN     = 0x00;             
	/* Close Charge */
	CmdCharge(Enable);
		/* power down card reader */
	REG_APB3_ICU_ANA2_CTRL &= (~(1<<29)); ///power down card reader
	REG_APB3_ICU_ANA1_CTRL  &= (~(1<<28));///  |= (1<<28); //open spibmen
	REG_APB3_ICU_ANA1_CTRL |= (1<<26);
	/* cpu clock switch dco, prepare sleep */
	REG_APB3_ICU_CLOCKSOURCE    = SYSCLK_HFSRC_30M_DCO;
	REG_APB3_ICU_CPUCLKCON    |= (1<<9);
	REG_APB3_ICU_ANA2_CTRL |= (1<<24);
	REG_APB3_ICU_ANA2_CTRL |= (1<<25);
	REG_APB3_ICU_ANA2_CTRL &= (~(1<<8));
	
	/* CLOSE BK5824 CE = 0 */
	REG_APB7_GPIOC_CFG &= 0xC1C1FFFF;
	REG_APB7_GPIOC_CFG |= 0x00003E3E;
	REG_APB7_GPIOC_DATA |= 0x3E0000;
  REG_APB7_GPIOC_DATA &= 0xfffffC1;	 

	/* FLASH CLK */
	set_flash_clk(FLASH_DCO_CLK_32MHz);
  /* XTAL CLOSE 32MHZ XTAL */	
	REG_APB7_GPIOC_DATA        &= 0xfffffffe;
	
	REG_APB3_ICU_IRQTYPE_EN = CpuFlag;
	
	REG_APB11_LBD_CLK32K_PWD	   = 0x1;					//close 32kHz
}	
/**************************************************
FunctionName: 
Describe: 
InputParameter: 
OutputParameter: 
Notice:
**************************************************/
void RFRouseAndChargeConfig(void)
{
	uint32_t CpuFlag = REG_APB3_ICU_IRQTYPE_EN;
	/*  irq disable & FIQ disable */
	REG_APB3_ICU_IRQTYPE_EN     = 0x00;  	
	/* Close Charge */
	CmdCharge(Enable);
	/* power down card reader */
				/* power down card reader */
	REG_APB3_ICU_ANA2_CTRL &= (~(1<<29)); ///power down card reader
	REG_APB3_ICU_ANA1_CTRL  &= (~(1<<28));///  |= (1<<28); //open spibmen
	REG_APB3_ICU_ANA1_CTRL |= (1<<26);
	/* cpu clock switch dco, prepare sleep */
	REG_APB3_ICU_CLOCKSOURCE    = SYSCLK_HFSRC_30M_DCO;
	REG_APB3_ICU_CPUCLKCON    |= (1<<9);
	REG_APB3_ICU_ANA2_CTRL |= (1<<24);
	REG_APB3_ICU_ANA2_CTRL |= (1<<25);
	REG_APB3_ICU_ANA2_CTRL &= (~(1<<8));
	/* CLOSE BK5824 0WK Mode */
	#if 1
    BK5824_SoftWakeup_toggle();
	#else
	  bk_test_obu0Wakeup();
	#endif

	/* FLASH CLK */
	set_flash_clk(FLASH_DCO_CLK_32MHz);
  /* XTAL CLOSE 32MHZ XTAL */	
	REG_APB7_GPIOC_DATA        &= 0xfffffffe;
	
	REG_APB3_ICU_IRQTYPE_EN = CpuFlag;
	
	REG_APB11_LBD_CLK32K_PWD	   = 0x1;					//close 32kHz
}

/**************************************************
FunctionName: 
Describe: ���û���ϵͳ
InputParameter: 
OutputParameter: 
Notice:
**************************************************/
void ConfigRouseSystem(void)
{
	uint32_t CpuFlag = REG_APB3_ICU_IRQTYPE_EN;
	/*  irq disable & FIQ */
	REG_APB3_ICU_IRQTYPE_EN     = 0x00;  
	#if 1	
		#if 1
			/* GPIOF_Pin5 INT Wake Up System */
	//	REG_APB7_GPIOF_CFG = 0x2000FFFF;
	//	REG_APB7_GPIOF_DATA = 0x202020;
	//		REG_APB7_GPIOF_CFG &= ~0x20202020;
			REG_APB7_GPIOF_CFG |= 0x20002020;
			REG_APB7_GPIOF_DATA |= 0x200020;
			#if 0
				/* ���ش��� */
				REG_APB7_GPIO_INT_TRIG_MODE2 |= 0x2000;
				#if 0
						/* ˫�ش��� */
					REG_APB7_GPIO_INT_TRIG_dpulse2 |= 0x2000;
				#else
					/* ���ش��� */
					REG_APB7_GPIO_INT_TRIG_dpulse2 &= ~0x2000;
					#if 0
						/* �½��ش��� */
						REG_APB7_GPIO_INT_PULSE2 |= 0x2000;
					#else
						/* �����ش��� */
						REG_APB7_GPIO_INT_PULSE2 &= ~0x2000;
					#endif
				#endif
			#else 
				/* ��ƽ */
				REG_APB7_GPIO_INT_TRIG_MODE2 &= ~0x2000;
				#if 1
					REG_APB7_GPIO_INT2_LEVEL |= 0x2000; //High
				#else
					REG_APB7_GPIO_INT2_LEVEL &= ~0x2000; //Low
				#endif
			#endif
			REG_APB7_GPIO_INT2_ENABLE |= 0x2000; 
			REG_APB7_GPIO_INT2_STATUS |= 0x2000;
			REG_APB3_ICU_INT_ENABLE    |= INT_STATUS_GPIO_bit;
			REG_APB3_ICU_INT_WAKENA    |= INT_STATUS_GPIO_bit;
			REG_APB3_ICU_INT_CLEAR |= INT_STATUS_GPIO_bit;
			CpuFlag |= 0x01;
		#endif	
	#endif
	REG_APB3_ICU_IRQTYPE_EN = CpuFlag;
}
/**************************************************
FunctionName: 
Describe: BK5863N����ģʽ�����������ߣ����Ѻ���������
InputParameter: 
OutputParameter: 
Notice:
**************************************************/
void BK5863nSysSleep(uint32_t SleepMode)
{
	MyBoardSleepConfig(SleepMode);
	ConfigRouseSystem();
	if(SleepMode == _DeepSleep)
		DeepSleepConfig();
	else if(SleepMode == _OnlyRFRouse)
		OnlyRFRouseConfig();
	else if(SleepMode == _OnlyCharge)
		OnlyChargeConfig();
	else if(SleepMode == _RFRouseAndCharge)
		RFRouseAndChargeConfig();
	
	REG_APB3_ICU_CPUCLKCON    |= (1<<9);
	/* close cpu clock, so that close dco == Sleep */
	REG_APB3_ICU_CPUCLKCON  |= 0x00000100;
	delay_us(1);
	/* Wake up */
	REG_APB7_GPIOC_CFG |= 0x00010001;  //���� ��ͨIO 
	REG_APB7_GPIOC_CFG &= (~0x00000100);  //���ʹ��
    REG_APB7_GPIOC_DATA |= 0x01;  //��ʱ��
	/* Wait XTAL Stabilization */
    REG_APB3_ICU_CPUCLKCON    &= (~(1<<9));
	delay_us(1);
	test_gpiod_5_uart1_rx(0);
	MyBoardSleepRecoverConfig(SleepMode);
	/* Switch to 32MHz XTAL */
	REG_APB3_ICU_CLOCKSOURCE    = SYSCLK_HFSRC_32M;
	delay_us(100);
}
/**************************************************
FunctionName: 
Describe: ���ݲ�ͬƽ̨���޸Ļ���������Ӧ���ã�
          �ﵽ�͵���ģʽ
InputParameter: 
OutputParameter: 
Notice:
**************************************************/
void MyBoardSleepConfig(uint32_t SleepMode)
{
	#if 0
	  /* Simulation 7816 Port */
	  Port7816RSTSystemSleepConfig(); 
		Port7816IOSystemSleepConfig();
	#else
	  
	#endif
	if(SleepMode == _DeepSleep)
	{
		/* MY BK5863N board */
		REG_APB7_GPIOD_CFG  |= 0x20000000;
	}
	else if(SleepMode == _OnlyRFRouse)
	{
		REG_APB7_GPIOD_CFG  |= 0x20000000;
	}
	else if(SleepMode == _OnlyCharge)
	{
		REG_APB7_GPIOD_CFG  |= 0x20000000;
	}
	else if(SleepMode == _RFRouseAndCharge)
	{
		REG_APB7_GPIOD_CFG  |= 0x20000000;
	}
}
/**************************************************
FunctionName: 
Describe: ���ݲ�ͬƽ̨���޸Ļ���������Ӧ���ã�
          �ظ�����
InputParameter: 
OutputParameter: 
Notice:
**************************************************/
void MyBoardSleepRecoverConfig(uint32_t SleepMode)
{
	if(SleepMode == _DeepSleep)
	{	
		REG_APB7_GPIOD_CFG  &= (~0x20000000);
	}
	else if(SleepMode == _OnlyRFRouse)
	{
		REG_APB7_GPIOD_CFG  &= (~0x20000000);
	}
	else if(SleepMode == _OnlyCharge)
	{
		REG_APB7_GPIOD_CFG  &= (~0x20000000);
	}
	else if(SleepMode == _RFRouseAndCharge)
	{
		REG_APB7_GPIOD_CFG  &= (~0x20000000);
	}
}



/***********************  END OF FILES  ***********************/
