#include "SysConfig.h"
#include "BK5863.h"

/********************************************************
Function: BK5863 System Initial
Parameter: None
********************************************************/
void BK5863SystemInit(void)
{
	CLOSEAllIntEnableAndFlag();
#ifdef RELEASE
	CalibrationConf(ReadCalibCoef6());
#endif
#if 0
	  REG_APB3_ICU_CLOCKSOURCE    = SYSCLK_HFSRC_30M_DCO;
		REG_APB3_ICU_CPUCLKCON      = 1 << 1;               //clock division frequency factor
		REG_APG3_ICU_DEEPSLEEP      = 0x0e;                 //gpio test enable
		/* gpioc[0,3] output '1', so that open BK5823 & 32mhz xtal */
	  REG_APB7_GPIOC_CFG  = 0x0000F6FF;
	  /* gpioc[1,2,4,5] input   */
		REG_APB7_GPIOC_DATA = 0x00360009;
		delay_ms(3);	
    /* switch to 32Mhz */
		REG_APB3_ICU_CLOCKSOURCE    = SYSCLK_HFSRC_32M;
		delay_us(100);
		REG_APB3_ICU_ANA4_CTRL  |=  1<<21;//32k clock calibration (toggle)
		delay_us(100);
		REG_APB3_ICU_ANA4_CTRL  &= ~(1<<21); 
		delay_us(100);
	
#else
	/* */
	REG_APB3_ICU_CLOCKSOURCE    = SYSCLK_HFSRC_30M_DCO; //clock source select
	//REG_APB3_ICU_CPUCLKCON      = 1 << 1; //clock division frequency factor
	REG_APB3_ICU_CPUCLKCON = 0;
	REG_APG3_ICU_DEEPSLEEP      = 0x0e;                 //gpio test enable
#if 0
	REG_APB7_GPIOA_CFG = 0x00FFFFFF;
	REG_APB7_GPIOA_DATA = 0x00FF00FF;
	REG_APB7_GPIOB_CFG = 0x00FFFFFF;
	REG_APB7_GPIOB_DATA = 0x00FF00FF;
	REG_APB7_GPIOC_CFG = 0x00FFFFFF;
	REG_APB7_GPIOC_DATA = 0x00FF00FF;
	REG_APB7_GPIOD_CFG = 0x00FFFFFF;
	REG_APB7_GPIOD_DATA = 0x00FF00FF;
	REG_APB7_GPIOE_CFG = 0x00FFFFFF;
	REG_APB7_GPIOE_DATA = 0x00FF00FF;
	REG_APB7_GPIOF_CFG = 0x00FFFFFF;
	REG_APB7_GPIOF_DATA = 0x00FF00FF;
#endif

	REG_APB3_ICU_IRQTYPE_EN     = 0x01;                 //irq enable
	REG_APG3_ICU_ARM_WR_FLASH   = 0x0;                  //prohabit arm write flash by AHB bus
	REG_APB3_ICU_ANA3_CTRL     |= 0x1;                  //power down flash enable

	REG_APB7_GPIOC_CFG          = 0x0000F6FF;	        //gpioc[0,3] output '1', so that open BK5823 & 32mhz xtal
	REG_APB7_GPIOC_DATA         = 0x00360009;	        //gpioc[1,2,4,5] input
	REG_APB7_GPIOD_CFG = 0x0CFFFF;                      //I2C 2 inter pull up
	delay_us(10);
	REG_APB3_ICU_CLOCKSOURCE    = SYSCLK_HFSRC_32M;     //switch to 32Mhz

    REG_APB3_ICU_ANA4_CTRL  |=  1<<21; //32k clock calibration (toggle)
    delay_us(10);
    REG_APB3_ICU_ANA4_CTRL  &= ~(1<<21); 
    
	//	#ifdef BK5863_CHIP_TEST	
	#if 1
		REG_APB7_GPIOB_DATA        &= 0xffdfffdf;           //gpioB[5]:output low(light on)
		REG_APB7_GPIOB_CFG         &= 0xcfcfcfE1;	        //gpioB[5]:output, B2,3,4 2nd function(wko,clk,io)
		REG_APB7_GPIOE_CFG         |= 0x10001000;           //gpioe[4] diable output, pull down
		REG_APB7_GPIOE_DATA        |= 0x100000;				//gpioe[4] input
		REG_APB7_GPIOF_CFG         |= 0x20002000;           //gpioe[4] diable output, pull down
		REG_APB7_GPIOF_DATA        |= 0x200000;				//gpioe[4] input

		REG_APB3_ICU_INT_ENABLE    |= INT_STATUS_GPIO_bit;  // suppot gpiof5 --reset key
		REG_APB3_ICU_INT_WAKENA    |= INT_STATUS_GPIO_bit;
		REG_APB7_GPIO_INT2_LEVEL   |= BK5863_WAKEUP_COND_2;// suppot gpiof5 --reset key
		REG_APB7_GPIO_INT2_STATUS  |= BK5863_WAKEUP_COND_2;// clear
		REG_APB7_GPIO_INT2_ENABLE  |= BK5863_WAKEUP_COND_2;// suppot gpiof5 --reset key

		REG_APB3_ICU_INT_CLEAR = 0xfffffffe;
		
	#endif
#endif
	set_bk58xx_figure_voltage(FIGURE_VOLTAGE_1_9V);

	/* */
	SetClockOutputEnable(Disable);
///	flash_init();
}

/********************************************************
Function: BK5863 Enter Standby
Parameter: 
          StandbyMode:
              0x0: flash power on, others off (Transport mode 1)
              0x1: 32KHz is running, BK5824 is sleeping, others off
              0x2: flash power on, 32KHz is running,  BK5824 is sleeping, others off
              0x3: BK5824 is sleeping,others off
              0x4: All power off (Transport Mode2)
              Other: reserved       
********************************************************/
int wakeupflag = 0;
void BK5863EnterStandby(unsigned char StandbyMode)
{
	uint32_t CPUFlag = REG_APB3_ICU_IRQTYPE_EN;
	REG_APB3_ICU_IRQTYPE_EN     = 0x00;                 //irq disable
	wakeupflag = 0;
	REG_APB3_ICU_INT_FLAG = REG_APB3_ICU_INT_FLAG;
	
	REG_APB3_ICU_INT_ENABLE    |= INT_STATUS_GPIO_bit;
	REG_APB3_ICU_INT_WAKENA    |= INT_STATUS_GPIO_bit;
    
	REG_APB7_GPIO_INT_LEVEL    = BK5863_WAKEUP_COND_1;	//wakeup set int by high level
	REG_APB7_GPIO_INT_STATUS   = BK5863_WAKEUP_COND_1;	//clear int status	
	REG_APB7_GPIO_INT_ENABLE   = BK5863_WAKEUP_COND_1;	//wakeup int enable	
  #if 1
		/* GPIOF_Pin5 INT Wake Up System */
		REG_APB7_GPIOF_CFG &= ~0x20202020;
		REG_APB7_GPIOF_CFG |= 0x20002020;
		REG_APB7_GPIOF_DATA |= 0x200020;
		#if 0
			/* 边沿触发 */
			REG_APB7_GPIO_INT_TRIG_MODE2 |= 0x2000;
			#if 0
					/* 双沿触发 */
				REG_APB7_GPIO_INT_TRIG_dpulse2 |= 0x2000;
			#else
				/* 单沿触发 */
				REG_APB7_GPIO_INT_TRIG_dpulse2 &= ~0x2000;
				#if 1
					/* 下降沿触发 */
					REG_APB7_GPIO_INT_PULSE2 |= 0x2000;
				#else
					/* 上升沿触发 */
					REG_APB7_GPIO_INT_PULSE2 &= ~0x2000;
				#endif
			#endif
		#else 
			/* 电平 */
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
  #endif	

	#ifdef BEKEN_STANDBY_CURRENT_TEST
		/*OUTPUT: sda: E5;   INPUT: SCK:E4*/
    //REG_APB7_GPIOB_DATA        &= 0xffffffdf;           //gpioe[5]:output low
    REG_APB7_GPIOB_DATA        |= 0x20;           //gpioe[5]:output low
	#endif 
	CmdCharge(Disable);
//	CmdCharge(Enable);
		/* power down card reader */
	REG_APB3_ICU_ANA2_CTRL     |= 0x00040000;	  
  REG_APB3_ICU_CDRDERCLKCON	|= 0x01;
		/* cpu clock switch dco, prepare sleep */
	REG_APB3_ICU_CLOCKSOURCE    = SYSCLK_HFSRC_30M_DCO;
	/* BK5824 enter sleeping, and wait wakeup */
	BK5824EnterSleep(); 
	REG_APB7_GPIOC_DATA        &= 0xffffff1;	  
	REG_APB7_GPIOC_DATA        &= 0xfffffffe;	        //close 32MHz Xtal	
	
	if((StandbyMode == 4)||(StandbyMode == 0)) 
		REG_APB3_ICU_ANA1_CTRL &= 0xffefffff;       //close spibmen
	
	if(StandbyMode == 0 || StandbyMode == 4)
	{
		REG_APB7_GPIOC_CFG  &= 0xffff00ff;	
		REG_APB7_GPIOC_DATA &= 0xffffff00;	//power down BK5824
	}

	if((StandbyMode == 2) ||(StandbyMode == 0))
	{
		REG_APB3_ICU_ANA3_CTRL &= 0xfffffffe;
	}
	
	REG_APB3_ICU_IRQTYPE_EN = CPUFlag|0x01;  //IRQ OPEN
	
  if(!wakeupflag)	 // if receiving ISR, do nothing	
	{
		if((StandbyMode != 2)&&(StandbyMode != 1))   
		{   
			REG_APB11_LBD_CLK32K_PWD = 0x1;       //close 32kHz xtal
			delay_us(1); 
		} 
		REG_APB3_ICU_CPUCLKCON  |= (0x1UL<<9);
		/* close cpu clock, so that close dco */
		REG_APB3_ICU_CPUCLKCON  |= 0x00000100;	       
	  delay_us(5); 
	}
	
	REG_APB7_GPIOC_DATA |= 0x01;  //开时钟
	delay_us(1);
	REG_APB3_ICU_CLOCKSOURCE    = SYSCLK_HFSRC_32M;
	
	delay_ms(10);
}















