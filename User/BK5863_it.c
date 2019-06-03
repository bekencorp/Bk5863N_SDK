#include "BK5863_it.h"
#include "BK5863.h"

#include "Includes.h"
/**************************************************
FunctionName:
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
void BK5863NCardReaderIntService(void)
{
	
}
/**************************************************
FunctionName:
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
void BK5863nUart1IntService(void)
{
	BK5863Uart1IntService();
}
/**************************************************
FunctionName:
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
void BK5863nUart2IntService(void)
{
	#if 1  //UART
		UART2Service();
	#else  //simulation 7816
	  SimulationPort7816IOService();
	#endif
}
/**************************************************
FunctionName:
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
void BK5863nI2c1IntService(void)
{
	BK5863I2cIntService(0);
}

/**************************************************
FunctionName:
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
void BK5863nI2c2IntService(void)
{
	BK5863I2cIntService(1);
}

/**************************************************
FunctionName:
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
void BK5863nSpi1IntService(void)
{
	BK5863SpiIntService(0);
}

/**************************************************
FunctionName:
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
void BK5863nSpi2IntService(void)
{
	BK5863SpiIntService(1);
}

/**************************************************
FunctionName:
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
void BK5863nSpi3IntService(void)
{
	BK5863SpiIntService(2);
}

/**************************************************
FunctionName:
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
void BK5863nSpi4IntService(void)
{
	BK5863SpiIntService(3);
}
/**************************************************
FunctionName:  GPIOTest_IRQHandler
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
void GPIOTest_IRQHandler(void)
{
	BK5863GPIOIntService();
}
/**************************************************
FunctionName:  GPIOB5_IRQHandler
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
void GPIOB5_IRQHandler(void)
{
	printf("GPIOB5_IRQHandler\r\n");
}
/**************************************************
FunctionName:  GPIOB6_IRQHandler
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
void GPIOB6_IRQHandler(void)
{
	printf("GPIOB6_IRQHandler\r\n");
}
/**************************************************
FunctionName:  GPIOB7_IRQHandler
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
void GPIOB7_IRQHandler(void)
{
	printf("GPIOB7_IRQHandler\r\n");
}
/**************************************************
FunctionName:  GPIOC_Pin4_IRQHandle
Describe: IRQ
InputParameter:
OutputParameter:
Notice:
**************************************************/
void GPIOC_Pin4_IRQHandle(void)
{
	BK5824Stat.BK5824IRQFlag = TRUE;
//	BK5824Stat.BKIRQIntSta = FALSE;
//	SetGPIOIRQ(GPIOC,GPIO_Pin_4,Disable);
}
/**************************************************
FunctionName:  GPIOC_Pin5_IRQHandle
Describe: WKO
InputParameter:
OutputParameter:
Notice:
**************************************************/
void GPIOC_Pin5_IRQHandle(void)
{
	BK5824Stat.BK5824WKOFlag = TRUE;
//	BK5824Stat.BKWKOIntSta = FALSE;
//	SetGPIOIRQ(GPIOC,GPIO_Pin_5,Disable);  
}

/**************************************************
FunctionName:  GPIOF5_IRQHandler
Describe:
InputParameter:
OutputParameter:
Notice:  唤醒在中断加打印需要谨慎
**************************************************/
void GPIOF5_IRQHandler(void)
{
//	delay_ms(10);
//	printf("GPIOF5_IRQHandler\r\n");
//	delay_ms(10);
}


/**************************************************
FunctionName:
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
void BK5863nDESIntService(void)
{
	int i;
	int data_len;
	
	if(REG_APB4_DES_INTF&0x01)		// in decry status
	{
	  /* 读取加密结果 */
		for(i=0; i<DES_FRAME_LEN; i++)
			BKDES_CTL_BLK.pOutput[BKDES_CTL_BLK.outIndex++]= REG_APB4_DES_OUT;
			
		REG_APB4_DES_CTL |= 1<<DES_ADDR_RST_BIT;
		REG_APB4_DES_INTF = 0; //clear
    
		if(BKDES_CTL_BLK.inIndex < BKDES_CTL_BLK.bufLen)
		{
			/* 继续未完的数据加密 */
			for(data_len = 0; data_len< DES_FRAME_LEN; data_len++)
			{
				REG_APB4_DES_IN = BKDES_CTL_BLK.pInput[BKDES_CTL_BLK.inIndex++];				
			}
		}		
	}
}

/**************************************************
FunctionName:
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
void BK5863nADCIntService(void)
{
	unsigned int stat = REG_APB1_ADC_CFG;
	unsigned int empty = stat & (1<<ADC_FIFO_EMPTY);

	if(BKADC_CTL_BLK.pBuf == NULL)
		return;

	while(!empty)
	{
		if(BKADC_CTL_BLK.adc_index >= BKADC_CTL_BLK.bufLen)
      break;
		BKADC_CTL_BLK.pBuf[BKADC_CTL_BLK.adc_index++] = REG_APB1_ADC_DAT & 0x3ff;
    empty = REG_APB1_ADC_CFG & (1<<ADC_FIFO_EMPTY);		
	}
}
/**************************************************
FunctionName:BK5863nDACIntService
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
//void BK5863nDACIntService(void)
//{

//}
/**************************************************
FunctionName:BK5863nPWM0IntService
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
void BK5863nPWM0IntService(void)
{
	BK5863PWMIntService(0);
}
/**************************************************
FunctionName:BK5863nPWM1IntService
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
void BK5863nPWM1IntService(void)
{
	BK5863PWMIntService(1);
}
/**************************************************
FunctionName:BK5863nPWM2IntService
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
void BK5863nPWM2IntService(void)
{
	BK5863PWMIntService(2);
}
/**************************************************
FunctionName:BK5863nPWM3IntService
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
void BK5863nPWM3IntService(void)
{
	#ifndef _TIM3_TEST_
		BK5863PWMIntService(3);
	#else
		Tim3_IRQHandler();
	#endif
}

/**************************************************
FunctionName:BK5863nPWM4IntService
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
void BK5863nPWM4IntService(void)
{
	BK5863PWMIntService(4);
}
/**************************************************
FunctionName:BK5863nMFIFOIntService
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
void BK5863nMFIFOIntService(void)
{
	
}

/**************************************************
FunctionName:BK5863nDetoutaoIntService
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
void BK5863nDetoutaoIntService(void)
{
	
}

/**************************************************
FunctionName:BK5863nDetoutboIntService
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
void BK5863nDetoutboIntService(void)
{
	
}


/**************************************************
FunctionName:BK5863nLBDLowVoltageIntService
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
void BK5863nLBDLowVoltageIntService(void)
{
	if(ReadLowVoltageInterruptFlag())
	{
		CleanVoltageInterruptFlag(INT_STATUS_LOW_VOLT_bit);
		/* task */
		printf("FIQ: The voltage is lower than 2.5v!\r\n");
		
		#if 0
		  /* Wait printf Done */
		  delay_ms(10); 
			/* cpu clock switch dco, prepare sleep */
			REG_APB3_ICU_CLOCKSOURCE = SYSCLK_HFSRC_30M_DCO;
			/* power down card reader */ 
			REG_APB3_ICU_ANA2_CTRL |= 0x00040000;  
			/*  */		
			REG_APB7_GPIOC_CFG &= 0xffff00ff; 
			/* power down BK5823 & 32MHz xtal */
			REG_APB7_GPIOC_DATA &= 0xffffff00;
			/* close 32kHz xtal */
			REG_APB11_LBD_CLK32K_PWD = 0x1; 
			/* close cpu clock, so that close dco */
			REG_APB3_ICU_CPUCLKCON |= 0x00000100; 
		#endif
	}		
}

/**************************************************
FunctionName:BK5863nLBDHighVoltageIntService
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
void BK5863nLBDHighVoltageIntService(void)
{
	if(ReadHighVoltageInterruptFlag())
	{	
		CleanVoltageInterruptFlag(INT_STATUS_HIGH_VOLT_bit);
		/* task */
		printf("FIQ: The voltage is higher than 3.0v!\r\n");
   
		#if 0	
		  /* Wait printf Done */
		  delay_ms(10);
	  	/* clock source select */
			REG_APB3_ICU_CLOCKSOURCE    = SYSCLK_HFSRC_30M_DCO; 
		  /*clock division frequency factor*/
			REG_APB3_ICU_CPUCLKCON      = 1 << 1;
		  /*gpioc[0,3] output '1', so that open BK5823 & 32mhz xtal*/	
	  	REG_APB7_GPIOC_CFG          = 0x0000F6FF;
			 /*gpioc[1,2,4,5] input */
		  REG_APB7_GPIOC_DATA         = 0x00360009;	
			 /*open 32kHz xtal*/
		  REG_APB11_LBD_CLK32K_PWD    = 0x0;   
			delay_us(20);
		   /*switch to 32m clock*/
			REG_APB3_ICU_CLOCKSOURCE    = SYSCLK_HFSRC_32M;
		   /*  */	
		  BK5824PowerUp();
       /*power on card reader*/		
			REG_APB3_ICU_ANA2_CTRL     &= 0xfffbffff;	 
		#endif
	}
}

/***********************  END OF FILES  ***********************/
