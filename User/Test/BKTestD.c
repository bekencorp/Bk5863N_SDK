#include "BKTestAPP.h"
#include "BKTestD.h"
#include "Includes.h"
#include "app_esam_5121.h"

unsigned int Temp_data[100];


//*********************************************************//
//Write Register from uart1 command 
//*********************************************************//
void driver_uart_write_register(void)
{
	BK5863_WRITE_REG(TestInfo.reg_addr,TestInfo.reg_value);
}

/********************************************************
	Function: BK5863 Wakeup From Standby Mode
	Parameter: None
********************************************************/
void BK5863Wakeup(void)
{  
  REG_APB3_ICU_CLOCKSOURCE    = SYSCLK_HFSRC_30M_DCO; //clock source select
	REG_APB3_ICU_CPUCLKCON      = 1 << 1;               //clock division frequency factor
  REG_APB7_GPIOC_CFG          = 0x0000F6FF;	        //gpioc[0,3] output '1', so that open BK5824 & 32mhz xtal
	REG_APB7_GPIOC_DATA         = 0x00360009;	        //gpioc[1,2,4,5] input
	REG_APB11_LBD_CLK32K_PWD    = 0x0;                  //open 32kHz xtal
  delay_us(20);
	REG_APB3_ICU_CLOCKSOURCE    = SYSCLK_HFSRC_32M;     //switch to 32m clock

	BK5824PowerUp();  
	
	REG_APB7_GPIO_INT_ENABLE   &= ~BK5863_WAKEUP_COND_1;//clear wakeup int enable
	REG_APB7_GPIO_INT_STATUS    = BK5863_WAKEUP_COND_1;	//clear int status	
	REG_APB7_GPIO_INT2_ENABLE  &= ~BK5863_WAKEUP_COND_2;//clear wakeup int enable
	REG_APB7_GPIO_INT2_STATUS   = BK5863_WAKEUP_COND_2;	//clear int status    
	REG_APB3_ICU_ANA2_CTRL     &= 0xfffbffff;	        //power on card reader
	    
	#if 0
		#ifdef BEKEN_STANDBY_CURRENT_TEST
			REG_APB7_GPIOB_DATA        &= 0xffffffdf;
			delay_us(1000);
			BK5863EnterStandby(1);								//auto enter standby
		#endif
	#endif
}

/*********************************************************
		Module Test Item Parser 
*********************************************************/
CardReaderMsg_t CrdRdMsg;
SpiMsg_t SpiMsg;
I2cMsg_t I2cMsg;
void Do_Total_Test(void)
{
	switch(TestInfo.test_item)
	{
		case MODULE_CMD_READ_CARD:
		{
			printf("------------------Start Read Carder Test!----------------\r\n");
			if(!TestInfo.cardreader_init)
			{
				BK5863CardReaderInit();
				TestInfo.cardreader_init++;
			}
			memset(&CrdRdMsg, sizeof(CrdRdMsg), 0);
			CrdRdMsg.BlockNum = TestInfo.KuaiN;
			CrdRdMsg.CardType = TestInfo.CardType;
			#if 0
				CrdRdMsg.CardType = 1;  //CPU
			#elif 1
				CrdRdMsg.CardType = 0;  //M1
			#else
				CrdRdMsg.CardType = 2;  //
			#endif
			CrdRdMsg.OprtMode = READCARD;
			CrdRdMsg.PiccCid  = TestInfo.PiccCid;
			CrdRdMsg.TxSpeed  = TestInfo.TxSpeed;
			CrdRdMsg.RxSpeed  = TestInfo.RxSpeed;
			memcpy(CrdRdMsg.Key, TestInfo.PassWd, 6);
			CrdRdMsg.FrameDly = TestInfo.PcdFrameDly;
			CrdRdMsg.ReqGuardTime = TestInfo.PcdReqGuardTime;
			CrdRdMsg.SFGIDly  = TestInfo.PcdSFGIDly;
			TestInfo.PcdOprtNum = 16;
			CrdRdMsg.OprtNum  = TestInfo.PcdOprtNum;
			BK5863TestCardReader(&CrdRdMsg);
			break;
		}
		case MODULE_CMD_WRITE_CARD:
		{
			printf("------------------Start Write Carder Test!----------------\r\n");
			if(!TestInfo.cardreader_init)
			{
				BK5863CardReaderInit();
				TestInfo.cardreader_init++;
			}
			memset(&CrdRdMsg, sizeof(CrdRdMsg), 0);
			TestInfo.KuaiN++;
			if(TestInfo.KuaiN >= 64)
				TestInfo.KuaiN = 1;
			if(((TestInfo.KuaiN+1)%4) == 0){
					TestInfo.KuaiN ++;
					if(TestInfo.KuaiN >= 64)
						TestInfo.KuaiN = 1;
			}
			CrdRdMsg.BlockNum = TestInfo.KuaiN;
			CrdRdMsg.CardType = TestInfo.CardType;
			CrdRdMsg.OprtMode = WRITECARD;
			CrdRdMsg.PiccCid  = TestInfo.PiccCid;
			CrdRdMsg.TxSpeed  = TestInfo.TxSpeed;
			CrdRdMsg.RxSpeed  = TestInfo.RxSpeed;
			memcpy(CrdRdMsg.Key,TestInfo.PassWd,6);
			CrdRdMsg.FrameDly = TestInfo.PcdFrameDly;
			CrdRdMsg.ReqGuardTime = TestInfo.PcdReqGuardTime;
			CrdRdMsg.SFGIDly  = TestInfo.PcdSFGIDly;
			CrdRdMsg.OprtNum  = TestInfo.PcdOprtNum;
			CrdRdMsg.pBuff    = &bk_config_flag[4];
			BK5863TestCardReader(&CrdRdMsg);
			break;
		}
		case MODULE_CMD_ESAM_TEST:
		{
			printf("------------------ ESAM Test!----------------\r\n");
            bk_esam_app_test();
			break;
		}
		case MODULE_CMD_ESAM_TEST_STEP1:
		{
			printf("------------------ ESAM Test Step1!----------------\r\n");
            bk_rsu_obu_m_step1();
			break;
		}
		case MODULE_CMD_ESAM_TEST_STEP2:
		{
            unsigned char ucMsg[BK_ESAM_APP_TEST_BUF_DEPTH];
			printf("------------------ ESAM Test Step1!----------------\r\n");
            bk_rsu_obu_m_step2(ucMsg, BK_ESAM_APP_TEST_BUF_DEPTH);
			break;
		}
		case MODULE_CMD_ESAM_TEST_STEP3:
		{
			printf("------------------ ESAM Test Step1!----------------\r\n");
            bk_rsu_obu_m_step3();
			break;
		}
		case MODULE_CMD_TEST_5824_SEND_DATA:
		{
            uint8_t data_temp[] = {0x01, 0x02, 0x03, 0x04,
                                   0xC0, 0x03, 0x91, 0xD0,
                                   0xA0, 0x00, 0x00, 0x01,
                                   0x01, 0xC2, 0x01, 0x00, 
                                   0x80, 0x00};
			printf("------------------ 5824 send data Test!----------------\r\n");
            BK5824SendData(data_temp, sizeof(data_temp));
			break;
		}
		case MODULE_CMD_ENTER_STANDBY:
		{
			printf("------------------Start Standby(Mode=%0d) Test!----------------\r\n", bk_config_flag[BK_SOC_SLEEP_MODE]);
			delay_ms(10);	
			BK5863EnterStandby(bk_config_flag[BK_SOC_SLEEP_MODE]);
			break;
		}
		case MODULE_CMD_DeepSleep_TEST:
		{
			printf("------------------ DeepSleep Test!----------------\r\n");
			delay_ms(10);	
			BK5863nSysSleep(_DeepSleep);
			break;
		}
		case MODULE_CMD_RFRouse_Sleep_TEST:
		{
			printf("------------------ RFRouse Sleep(OBU_0WK) Test!----------------\r\n");
			delay_ms(10);	
			bk_test_obu0Wakeup();
			BK5863nSysSleep(_OnlyRFRouse); 
			break;
		}
		case MODULE_CMD_FLASH_SECTOR:
		{
			printf("------------------Start Flash Sector Test!----------------\r\n");
			BK5863FlashSectorTest(TestInfo.SectorMode);
			break;
		}
		case MODULE_CMD_SPI_TEST:
		{
			printf("------------------Start SPI(%0d) Test!----------------\r\n",TestInfo.SpiIdx);
			memset(&SpiMsg, sizeof(SpiMsg), 0);
			SpiMsg.Ckphapol = TestInfo.SpiCkphapol;
			SpiMsg.Ckr      = TestInfo.SpiCkr;
			SpiMsg.Nssmd    = TestInfo.SpiNssmd;
			SpiMsg.Role     = TestInfo.SpiRole;
			SpiMsg.WaitCLK = 15;
			BK5863WDT(1000);
			BK5863TestSpi(TestInfo.SpiIdx, &SpiMsg, TestInfo.SpiDataLen);
			BK5863WDTClose();
			break;
		}
		case MODULE_CMD_I2C_TEST:
		{
			printf("------------------Start I2C(%0d) Test!----------------\r\n",TestInfo.I2cIdx);
			memset(&I2cMsg, sizeof(I2cMsg), 0);
			I2cMsg.ClkDiv       = TestInfo.I2cClkdiv;
			I2cMsg.LocalAddr    = TestInfo.I2cLocalAddr;
			I2cMsg.RemoteAddr   = TestInfo.I2cRemoteAddr;
			I2cMsg.Role         = TestInfo.I2cRole;
			I2cMsg.RwMode       = TestInfo.I2cRwMode;
			I2cMsg.RemainLen    = TestInfo.I2cDataLen;
			/* Open WDT -- MCU Avoid Died */
			BK5863WDT(0xAFF0);
			BK5863I2cInit(TestInfo.I2cIdx,&I2cMsg);     
			BK5863I2cReset(TestInfo.I2cIdx);
			BK5863TestI2c(TestInfo.I2cIdx,&I2cMsg);
			BK5863WDTClose();
			break;
		}
		case MODULE_CMD_ADC_TEST:
		{
			printf("-------Start ADC Test!-------\r\n");   
			BK5863WDT(0xFFF0);			
			bk_test_adc(8);
//			bk_Read_adc_CH2();
			BK5863WDTClose();
			break;
		}
		case MODULE_CMD_ADC_Temp_TEST:
		{
			printf("-------Start Read_ADC_CH2 Test!-------\r\n");   
			BK5863WDT(0xFFF0);			
			bk_test_adc(8);
			bk_Read_adc_CH2();
			BK5863WDTClose();
			break;
		}
		case MODULE_CMD_PWM_TEST:
		{
			printf("-------Start PWM Test!-------\r\n");
			bk_test_pwm();
			break;
		}
		case MODULE_CMD_DAC_TEST:
		{
			printf("-------Start DAC Test!-------\r\n");
			BK5863DACClose();
			bk_test_dac(AUD_SIN_32KFS, sizeof(AUD_SIN_32KFS));
			REG_APB2_SDDAC_CLKDIVID = 0x08000000;	//32M clock, 32k sampling, CLKDIV =(32768K/128*8K)<<24 
			break;    	
		}
		case MODULE_CMD_DES_TEST:
		{
			printf("-------Start DES Test!-------\r\n");
			BK5863WDT(0xFFF0);
			bk_test_des();
			BK5863WDTClose();
			break; 
		}
		case MODULE_CMD_PN9_TEST:
		{
			printf("-------Start PN9 Test!-------\r\n");
			bk_test_pn9();
			break;
		}
		case MODULE_CMD_BER_TEST:
		{
			printf("-------Start BER Test!-------\r\n");
			bk_test_ber();
			break;
		}
		case MODULE_CMD_SINGLE_TEST:
		{
			printf("-------Start Single Carrier Test!-------\r\n");
			bk_test_single();
			break;
		}
		case MODULE_CMD_OBU_TEST:
		{
			printf("-------Set as OBU -------\r\n");
			bk_test_obu();
			break;
		}
		case MODULE_CMD_RSU_TEST:
		{
			printf("-------Set as RSU -------\r\n");
			bk_test_rsu();
			break;
		}
		case MODULE_CMD_0WAKEUP_TEST:
		{
			printf("-------Set as OBU 0WAKEUP -------\r\n");
			bk_test_obu0Wakeup();
			break;
		}
	#ifdef BEKEN_BK5863_V2_TEST 
		case MODULE_CMD_5823_SPI_TEST:
		{
			printf("-------Start Test BK5824 SPI -------\r\n");
			BK5863TestBK5824();
			break;
		}
		case MODULE_CMD_ARM_WRITE_FLASH_PROTECT_TEST:
		{
			printf("-------Start Test ARM Write Flash Protect -------\r\n");
			
			break;
		}
		case MODULE_CMD_VOLT_DETECT_TEST:
		{
			printf("-------Start Test BK5863 Voltage Detect -------\r\n");
			BK5863VoltDetect();
			break;
		}
	#endif
		case MODULE_CMD_VDDSWITCH_TEST:
		{
			printf("******** VDDSWITCH ********\r\n");
			if(REG_APB3_ICU_ANA1_CTRL & (1<<28))
			{
				REG_APB3_ICU_ANA1_CTRL &= ~(1<<28);
				printf("ICU0x30bit28 = 0\r\n");
			}
			else
			{
				REG_APB3_ICU_ANA1_CTRL |= (1<<28);
				printf("ICU0x30bit28 = 1\r\n");
			}
			break;
		}
		case MODULE_CMD_CRCCalculate_TEST:
		{
			printf("******** CRCTest ********\r\n");
			CRCTest();
			break;
		}
		case MODULE_CMD_LBD_TEST:
		{
			printf("******** LBDTest ********\r\n");
			LBDTest();
			break;
		}
		case MODULE_CMD_PWMOUT_TEST:
		{
			printf("******** PWMOutTest ********\r\n");
			PWMOUTTestConfig();
			break;
		}
		case MODULE_CMD_GPIOB7XHClk_TEST:
		{
			printf("******** GPIOB7 XH Clk 16M TEST ********\r\n");
			ClockOutputinit(B7_XH_DIV_16M,Enable);
			break;
		}
		case MODULE_CMD_GPIOF5IntTest_TEST:
		{
			printf("******** GPIOF5 Int Test ********\r\n");
			GPIOTestConfig();
			break;
		}
		case MODULE_CMD_BK5824RecIntTest_TEST:
		{
			printf("******** BK5824 Rec ********\r\n");
			if(TestInfo.BK5824RecTest)
				TestInfo.BK5824RecTest = 0;
			else
				TestInfo.BK5824RecTest = 1;
			BK5824Init();
			break;
		}
		case MODULE_CMD_MCU_CLK_TEST:
		{
			printf("******** MCUSleep -- Config GPIOF5 Wakeup********\r\n");
			delay_ms(10);
			LEDClose(0x03);
			GPIOTestConfig(); 
			LEDClose(0x03);
			MCUSleep();
			LEDOpen(0x03);
			printf("******** MCU Sleep Wakeup********\r\n");
			delay_ms(10);
			break;
		}
		default:
			printf("No Execute!\r\n");
		break;
	}
	printf("BK5863 Module Test Finish!\r\n");
}
/********************************************************
	Description: BEKEN BK5863 PWM example code
********************************************************/
int timer_cnt[5];
void bk_test_pwm(void)
{
	BK5863PWMInit(PT0_MODE_BIT, BK_TIMER_MODE, SYS_32K_CLK_TIMER);
	BK5863PWMInit(PT1_MODE_BIT, BK_TIMER_MODE, SYS_32K_CLK_TIMER/2);
	BK5863PWMInit(PT2_MODE_BIT, BK_TIMER_MODE, SYS_32K_CLK_TIMER/4);
	BK5863PWMInit(PT3_MODE_BIT, BK_TIMER_MODE, SYS_32K_CLK_TIMER/8);
	BK5863PWMInit(PT4_MODE_BIT, BK_TIMER_MODE, SYS_32K_CLK_TIMER/16);
}

void BK5863PWMIntService(unsigned char PwmIdx)
{
	int i;
	switch(PwmIdx)
	{
		case 0x00:  //add user code
			
			REG_APB10_PWM_CFG |= PT0_INT_FLAG;
			break;
		case 0x01:  //add user code
		
			REG_APB10_PWM_CFG |= PT1_INT_FLAG;
			break;
		case 0x02:  //add user code
		
			REG_APB10_PWM_CFG |= PT2_INT_FLAG;
			break;
		case 0x03:  //add user code
		
			REG_APB10_PWM_CFG |= PT3_INT_FLAG;
			break;
		case 0x04:  //add user code
		
			REG_APB10_PWM_CFG |= PT4_INT_FLAG;
			break;
		default:
			break;               
	}
	#ifdef BEKEN_PWM_MODULE_TEST
		timer_cnt[PwmIdx]++;
		if(timer_cnt[0] >= 2)
		{
			REG_APB3_ICU_PWMCLKCON = 1;					//PWM clock disable
			REG_APG3_ICU_DEEPSLEEP |= (1<<3);

			for(i=0; i<5; i++)
				printf("timer%d counter: %x \n", i, timer_cnt[i]);
			printf("Close PWM module \n");
			memset(timer_cnt, 0, sizeof(timer_cnt));
		}
	#endif	
}

/********************************************************
	Description: BEKEN BK5863 ADC example code
********************************************************/
void bk_test_adc(unsigned char ch)
{
	int i;
	unsigned int length = 100;
	unsigned int *adc_data = Temp_data;
	BKADCCONFIG_STRU config;
	printf("adc testing\n");
	memset(adc_data, 0, length);
	BK5863ADCInit(adc_data,length);	
	/*****************************************************************
		function test: channel = 1;  product version: channel = 2
	*****************************************************************/
	config.channel = ch;
	config.clk_rate = 7;
	config.mode = 3;
	config.settling = 1;//0;
	config.sample_rate = 0;
	BK5863ADCConfig(&config,BK_ADC_MODE|BK_ADC_CHNL|BK_ADC_SAMPLE_RATE|BK_ADC_SETTLING|BK_ADC_CLK_RATE);
	delay_us(50);
	REG_APB3_ICU_ADCCLKCON  = 1;		//close ADC test 
	REG_APB3_ICU_INT_ENABLE &= ~INT_STATUS_ADC_bit;			//Disable interrupt
	for(i=0; i<length; i++)
		printf("%x \r\n", adc_data[i]);
}

/********************************************************
	Description: BEKEN BK5863 ADC CH2 example code
********************************************************/
void bk_Read_adc_CH2(void)
{
	int i;
	unsigned int length = 100;
	unsigned int *adc_data = Temp_data;
	BKADCCONFIG_STRU config;
	printf("adc testing\n");
	memset(adc_data, 0, length);
  BK5863ADCInit(adc_data,length);	
	/*****************************************************************
		function test: channel = 1;  product version: channel = 2
	*****************************************************************/
	config.channel = 2;
	config.clk_rate = 7;
	config.mode = 3;
	config.settling = 1;//0;
	config.sample_rate = 0;
	BK5863ADCConfig(&config,BK_ADC_MODE|BK_ADC_CHNL|BK_ADC_SAMPLE_RATE|BK_ADC_SETTLING|BK_ADC_CLK_RATE);
	delay_us(100);
	REG_APB3_ICU_ADCCLKCON  = 1;		//close ADC test 
	REG_APB3_ICU_INT_ENABLE &= ~INT_STATUS_ADC_bit;			//Disable interrupt
	for(i=0; i<length; i++)
		printf("%x \r\n", adc_data[i]);
}

#ifdef BEKEN_SPI_MODULE_TEST
/********************************************************
Function: BK5863 Spi test example
Parameter: 
      SpiIdx: Index of Spi
 pSpiMsgPara: data structure of SPI property
         Len: length of data
********************************************************/
void BK5863TestSpi(unsigned char SpiIdx, SpiMsg_t *pSpiMsgPara, unsigned int Len)
{   
	unsigned int    TxRemainLen = Len, RxRemainLen = Len;
	DataBuff_t      *pTxBuff, *pRxBuff;
//	unsigned char   Flag = 1;
	unsigned char   TxDataTemp = 0, RxDataTemp;
	unsigned char   RecvData[100];
	unsigned int    i, RecvLen = 0, ErrCnt = 0, DataNum;
	unsigned char   TxBuf[SPI_BUF_LEN], RxBuf[SPI_BUF_LEN];

	pTxBuff = &(pSpiMsgPara->TxBuff);
	pRxBuff = &(pSpiMsgPara->RxBuff);

	pTxBuff->pBuf = TxBuf;
	pRxBuff->pBuf = RxBuf;

	BK5863SpiInit(SpiIdx, pSpiMsgPara);     

	BK5863SpiReset(SpiIdx);

	while(RxRemainLen)
	{
		//prepare tx data
		if(TxRemainLen && (pTxBuff->VldDataNum <= SPI_BUF_LEN/2))
		{
			DataNum = MIN(TxRemainLen, SPI_BUF_LEN/2);
			for(i = 0; i < DataNum; i++)
			{
					pTxBuff->pBuf[pTxBuff->WrPtr++] = TxDataTemp++;
					if(pTxBuff->WrPtr == SPI_BUF_LEN) pTxBuff->WrPtr = 0;
					pTxBuff->VldDataNum ++;
			}
			BK5863SpiSendStart(SpiIdx);
			TxRemainLen -= DataNum;
			if(!TxRemainLen) pTxBuff->LastPkt = 1;
			//printf("TL = %0d\r\n", TxRemainLen);
		}  
		
		//read rx data
		DataNum = MIN(RxRemainLen, SPI_BUF_LEN/2);    
		if(pRxBuff->VldDataNum >= DataNum)
		{  
			for(i = 0; i < DataNum; i++)
			{           
				RxDataTemp = pRxBuff->pBuf[pRxBuff->RdPtr++];
				if(pRxBuff->RdPtr == SPI_BUF_LEN) pRxBuff->RdPtr = 0;
				pRxBuff->VldDataNum --;
				
				if(RxDataTemp != (RecvLen & 0xff)) ErrCnt++;
				if(RecvLen < 100) RecvData[RecvLen] = RxDataTemp;
				RecvLen++;
			}
			RxRemainLen -= DataNum;
			//printf("RN=%0d\r\n",RxRemainLen);
		}
	}
	BK5863SpiDisable(SpiIdx);

	DataNum = MIN(RecvLen, 100);
	printf("SPI(%0d) Receive First %0dth Data: ", SpiIdx, DataNum);
	for(i = 0; i < DataNum; i++)
		printf("%02x ", RecvData[i]);    
	printf("!\r\n");
	printf("Total Counter(%0d), Error Counter(%0d)!\r\n", Len, ErrCnt);
}
#endif

#ifdef BEKEN_I2C_MODULE_TEST
/**********************************************************************
//Function: BK5863 I2c test example
//Parameter: 
//      I2cIdx: Index of I2c
**********************************************************************/
void BK5863NTestI2C(void)
{
	I2C1Init();
	printf("REG_APBE_I2C1_CN = %08x\r\n",(int)REG_APBE_I2C1_CN);
}

unsigned char   I2CTxBuff[I2C_BUF_LEN],I2CRxBuff[I2C_BUF_LEN];
void BK5863TestI2c(unsigned char I2CIdx, I2cMsg_t *pi2cMsgPara)
{
	unsigned int    TxRemainLen, RxRemainLen;
	DataBuff_t      *pTxBuff, *pRxBuff;
	unsigned char   Flag = 1;
	int             i, DataNum, RxDataTemp;
	unsigned char   RecvData[100];
	int             TxDataTemp = 0, RecvLen = 0, ErrCnt = 0;
	unsigned char   addrByteData;
	
	pTxBuff = &(pi2cMsgPara->TxBuff);
	pRxBuff = &(pi2cMsgPara->RxBuff);
	
	TxRemainLen = pi2cMsgPara->RemainLen;
	RxRemainLen = pi2cMsgPara->RemainLen;
	
	pTxBuff->pBuf = I2CTxBuff;
	pRxBuff->pBuf = I2CRxBuff;
	
//	BK5863I2cInit(I2CIdx,pi2cMsgPara);     
//	BK5863I2cReset(I2CIdx);
	while((TxRemainLen || pTxBuff->VldDataNum) && RxRemainLen)
	{
		//prepare tx data
		if(pTxBuff->VldDataNum < I2C_BUF_LEN/2)
		{
			DataNum = MIN(TxRemainLen,I2C_BUF_LEN/2);
			for(i = 0; i < DataNum; i++)
			{
				pTxBuff->pBuf[pTxBuff->WrPtr++] = TxDataTemp++;
				if(pTxBuff->WrPtr == I2C_BUF_LEN)
					pTxBuff->WrPtr = 0;
				pTxBuff->VldDataNum ++;
			}
			TxRemainLen -= DataNum;
		}  
//		pi2cMsgPara->RemoteAddr = 0x68;
		if(Flag && pi2cMsgPara->Role)
		{
			Flag = 0;
			addrByteData = (pi2cMsgPara->RemoteAddr << 1) | pi2cMsgPara->RwMode;
			printf("pi2cMsgPara->RemoteAddr = %02x\r\n",pi2cMsgPara->RemoteAddr);
			printf("pi2cMsgPara->RwMode = %02x\r\n",pi2cMsgPara->RwMode);
			printf("addrByteData = %02x\r\n",addrByteData);
			BK5863I2cSendStart(I2CIdx, addrByteData);
		}  
		
		//read rx data
		for(i = 0; i < pRxBuff->VldDataNum; i++)
		{
			RxDataTemp = pRxBuff->pBuf[pRxBuff->RdPtr++];
			if(pRxBuff->RdPtr == I2C_BUF_LEN)
				pRxBuff->RdPtr = 0;
			pRxBuff->VldDataNum --;
			if(RxDataTemp != RecvLen) 
				ErrCnt++;
			if(RecvLen < 100) 
				RecvData[RecvLen] = RxDataTemp;
			RecvLen++;
			RxRemainLen--;
		}   
	}

	delay_us(100);  //wait last data send done in tx mode
	BK5863I2cDisable(I2CIdx);
	
	DataNum = MIN(RecvLen, 100);
	printf("I2C(%0d) Receive First %0dth Data: ", I2CIdx, DataNum);
	for(i = 0; i < DataNum; i++)
		printf("%0x ", RecvData[i]);    
	printf("!\r\n");
	printf("Total Counter(%0d), Error Counter(%0d)!\r\n", RecvLen, ErrCnt);   
}
#endif

/**********************************************************************
	Test Card Reader Function
**********************************************************************/
void BK5863TestCardReader(CardReaderMsg_t *pCardReaderMsg)
{
	char        status;
	int         OprtNum = pCardReaderMsg->OprtNum;
	int         ErrCnt = 0;
//	unsigned char RegData[4];
//	int BerErrCnt;
//	int	BerRecvCnt;
//	float BerErrRatio;
//  REG_APB7_GPIOF_CFG &= 0xfffffffc;       //open 2nd func, for mfin & mfout test

	while(OprtNum--)
	{     
		SetGPIOPin(GPIOF,GPIO_Pin_0|GPIO_Pin_1);
		PcdPower(Enable);
		delay_ms(3);  //µÈ´ýµçÔ´ÎÈ¶¨
		status = BK5863TestSelectCard(pCardReaderMsg);
		if(status == MI_OK)
		{        
			if(pCardReaderMsg->CardType == 1) //cpu card
			{
				printf("  ~~~~ CPU Card! ~~~~  \r\n");
				status = BK5863TestCpuCard(pCardReaderMsg);
			}
			else if(pCardReaderMsg->CardType == 0) //M1Card
			{
				printf("  ~~~~ M1 Card! ~~~~  \r\n");
				status = BK5863TestOperateM1Card(pCardReaderMsg);
			}
			else
			{
				delay_ms(5000);
			}
		}
			
		if(status != MI_OK)
		{
			ErrCnt++; 
		}
		else
		{
			printf("Pcd %0dth Operate Card Successfully!\r\n", pCardReaderMsg->OprtNum - OprtNum);            
		}
		PcdPower(Disable);
		delay_ms(10);
		delay_us(pCardReaderMsg->ReqGuardTime);
	}
	
	printf("Pcd Test Finish, Total Counter(%0d) Error Counter(%0d)\r\n", pCardReaderMsg->OprtNum, ErrCnt);
	if(pCardReaderMsg->OprtNum > ErrCnt){
		printf("Total Counter %d succeed\r\n", pCardReaderMsg->OprtNum - ErrCnt);
	}else{
		printf("Total Counter %0d failed\r\n", ErrCnt);
	}
	
	PcdPowerDown();
}

/**********************************************************************
Test Select Card according to ISO14443-3
**********************************************************************/
char BK5863TestSelectCard(CardReaderMsg_t *pCardReaderMsg)
{
	char            status;
	unsigned char   RevBuffer[32];

	PcdReset();
	if(pCardReaderMsg->CardType)
	  status = PcdRequest(PICC_REQIDL,&RevBuffer[0]);   
	else
	  status = PcdRequest(PICC_REQALL,&RevBuffer[0]); 
	if(status != MI_OK)
	{
		printf("Pcd Request Card Failed!\r\n");
		return MI_ERR;
	}
	
	delay_us(pCardReaderMsg->FrameDly);
	status =	PcdAnticoll(&RevBuffer[2]);          //·À³å×²£¬·µ»Ø¿¨µÄÐòÁÐºÅ 4×Ö½Ú
	if(status != MI_OK)
	{       
		printf("Pcd AntiCollision Failed!\r\n");
		return MI_ERR;
	}
	
	delay_us(pCardReaderMsg->FrameDly);
	memcpy(pCardReaderMsg->CardSn,&RevBuffer[2],4);
	status = PcdSelect(pCardReaderMsg->CardSn);     //Ñ¡¿¨
	
	if(status != MI_OK)
		printf("Pcd Select Card Failed!\r\n");
	else
	{
		//#ifdef DEBUG_OPEN
			printf("Pcd Select Card Success. Sn=0x%0x%0x%0x%0x!\r\n",
						pCardReaderMsg->CardSn[0], pCardReaderMsg->CardSn[1], 
						pCardReaderMsg->CardSn[2], pCardReaderMsg->CardSn[3]);
		//#endif
	}
	return status;
}

/*********************************************************
	Paramter Read Back for Module Test 
*********************************************************/
unsigned char BK5863ModuleParaRead(unsigned char addr, unsigned char *pData)
{
	unsigned char   Para;
	unsigned char   i;
	unsigned char	len = 4;

	switch(addr)
	{
		case 0x00:
			Para = (TestInfo.CardType & 0x01) | ((TestInfo.TxSpeed & 0x03) << 4) | ((TestInfo.RxSpeed & 0x03) << 6);
			*pData++ = Para;
			*pData++ = TestInfo.KuaiN & 0xff;
			*pData++ = TestInfo.PiccCid & 0xff;
			*pData++ = (TestInfo.BK5823BerEn & 0x01);
			len = 4;
			break;
		case 0x01:
		case 0x02:
		case 0x03:
		case 0x04:  
			for(i = 0; i < 4; i++)
				*pData++ = TestInfo.Write_Data[i+(addr-1)*4];
			len = 4;
			break;
		case 0x05:  
			for(i = 0; i < 4; i++)
				*pData++ = TestInfo.PassWd[i];
			len = 4;
			break;
		case 0x06:  
			*pData++ = TestInfo.PassWd[4];  
			*pData++ = TestInfo.PassWd[5];
			*pData++ = 0;
			*pData++ = 0;
			len = 4;
			break;
		case 0x09: 
			*pData++ = TestInfo.PcdFrameDly;
			*pData++ = TestInfo.PcdFrameDly >> 8;
			*pData++ = TestInfo.PcdReqGuardTime;
			*pData++ = TestInfo.PcdReqGuardTime >> 8;
			len = 4;
			break;
		case 0x0a:  
			*pData++ = TestInfo.PcdSFGIDly;
			*pData++ = TestInfo.PcdSFGIDly >> 8;
			*pData++ = 0;
			*pData++ = 0;
			len = 4;
			break;
		case 0x0b:  
			*pData++ = TestInfo.PcdOprtNum;
			*pData++ = TestInfo.PcdOprtNum >> 8;
			*pData++ = TestInfo.PcdOprtNum >> 16;
			*pData++ = TestInfo.PcdOprtNum >> 24;
			len = 4;
			break;
		case 0x0c:  
			Para = (TestInfo.StandbyMode & 0x07) | ((TestInfo.SectorMode & 0x01) << 3);
			*pData++ = Para;
			Para = TestInfo.SpiNssmd | (TestInfo.SpiCkphapol << 2) | (TestInfo.SpiRole << 4) | (TestInfo.SpiIdx << 6);
			*pData++ = Para;
			*pData++ = TestInfo.SpiCkr;
			*pData++ = 0;
			len = 4;
			break;
		case 0x0d:  
			*pData++ = TestInfo.SpiDataLen;
			*pData++ = TestInfo.SpiDataLen >> 8;
			*pData++ = TestInfo.SpiDataLen >> 16;
			*pData++ = TestInfo.SpiDataLen >> 24;
			len = 4;   
			break;
		case 0x0e: 
			*pData++ = TestInfo.I2cIdx | (TestInfo.I2cRole << 2) | (TestInfo.I2cRwMode << 3);
			*pData++ = TestInfo.I2cLocalAddr;
			*pData++ = TestInfo.I2cRemoteAddr;
			*pData++ = 0;
			len = 4;
			break;
		case 0x0f: 
			memcpy(pData, &TestInfo.I2cDataLen, 4);
			len = 4;
			break;
		default	:	
			*pData++ = 0;
			*pData++ = 0;
			*pData++ = 0;
			*pData++ = 0;
			len = 4;
			break;
	}
	return len;
}

/********************************************************
Function: BK5863 GPIO ISR
Parameter: 
          None
********************************************************/
void BK5863GPIOIntService(void)
{
	volatile unsigned int stat,stat2;
	stat = REG_APB7_GPIO_INT_STATUS;
	stat2 = REG_APB7_GPIO_INT2_STATUS;
	if((stat & BK5863_WAKEUP_COND_1) || //gpioc 5 bk5823 wakeup
		 (REG_APB7_GPIO_INT2_STATUS & BK5863_WAKEUP_COND_2)) //GPIOE[4] wakeup
	{
		BK5863Wakeup();
	}
	REG_APB7_GPIO_INT_STATUS = stat;
	
	stat = REG_APB7_GPIO_INT2_STATUS;
	if(stat & 0x2000) //reset key
	{
			BK5863Wakeup();
//			BK5863WDT(32);
	}
	REG_APB7_GPIO_INT2_STATUS = stat;
}

/**************************************************
FunctionName:CRCTest
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
const uint8_t CRCTestData[9]={
	0xa1,0xa2,0xa3,0xb1,0xb2,0xb3,0xc1,0xc2,0xc3,
};
/* ´ËÊý¾ÝÎªBK5824½ÓÊÕCRCTestData[9]×Ö½ÚÊý¾ÝÊ±µÄÊý¾ÝÁ÷£¬×îºóÁ½×Ö½ÚÎªÆäCRCÐ£ÑéÂë */
const uint8_t CRCTestData1[11]={
	0xa1,0xa2,0xa3,0xb1,0xb2,0xb3,0xc1,0xc2,0xc3,0xae,0x2d
};
/* ´ËÊý¾ÝÎªBK5824½ÓÊÕCRCTestData1[11]×Ö½ÚÊý¾ÝÊ±µÄÊý¾ÝÁ÷£¬×îºóÁ½×Ö½ÚÎªÆäCRCÐ£ÑéÂë */
const uint8_t CRCTestData2[13]={
	0xa1,0xa2,0xa3,0xb1,0xb2,0xb3,0xc1,0xc2,0xc3,0xae,0x2d,0x47,0x0f
};
/********************************************************
  ×¢Òâ£ºCRCÖÐCRC0¶Á³öµÄÊýÖµºÍBK5824ÊÕµ½µÄCRCÐèÒªµ÷»»Ò»ÏÂÎ»ÖÃ
*********************************************************/
void CRCTest(void)
{
	uint32_t CRC0,CRC1;
	uint8_t CRCDat[15];
	memcpy(CRCDat,CRCTestData,9);
	CRC0 = sizeof(CRCDat);
	printf("CRCDat(%d) = 0x",CRC0);
	for(CRC0=0;CRC0<9;CRC0++)
	 printf("%02x",CRCDat[CRC0]);
	printf("\r\n");
	
	CRC0 = CalculateCRC(_CRC0,CRCDat,9);
	printf("CRC0 = 0x%08x\r\n",CRC0);
	CRC1 = CalculateCRC(_CRC1,CRCDat,9);
	printf("CRC1 = 0x%08x\r\n",CRC1);
	memcpy(CRCDat,CRCTestData1,11);
	printf("CRCDat(%d) = 0x",CRC0);
	for(CRC0=0;CRC0<11;CRC0++)
	 printf("%02x",CRCDat[CRC0]);
	printf("\r\n");
	CRC0 = CalculateCRC(_CRC0,CRCDat,11);
	printf("CRC0 = 0x%08x\r\n",CRC0);
	CRC1 = CalculateCRC(_CRC1,CRCDat,11);
	printf("CRC1 = 0x%08x\r\n",CRC1);
	memcpy(CRCDat,CRCTestData2,13);
	printf("CRCDat(%d) = 0x",CRC0);
	for(CRC0=0;CRC0<13;CRC0++)
	  printf("%02x",CRCDat[CRC0]);
	printf("\r\n");
	CRC0 = CalculateCRC(_CRC0,CRCDat,13);
	printf("CRC0 = 0x%08x\r\n",CRC0);
	CRC1 = CalculateCRC(_CRC1,CRCDat,13);
	printf("CRC1 = 0x%08x\r\n",CRC1);
}

/**************************************************
FunctionName:
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
void LBDTest(void)
{
	SetHighVoltageInterrupt(Enable);
	SetLowVoltageInterrupt(Enable);
	SetLowVoltageInterrupt(Enable);
	SetHighVoltageInterrupt(Enable);
	CmdFIQ(Enable);
}

void MCUSleep(void)
{
	uint32_t CpuFlag = REG_APB3_ICU_IRQTYPE_EN;
	unsigned char mcu64M_state;
		/*  irq disable & FIQ disable */
	REG_APB3_ICU_IRQTYPE_EN     = 0x00;  	

		/* power down card reader */
	REG_APB3_ICU_ANA2_CTRL     |= 0x00040000;	  
		/* cpu clock switch dco, prepare sleep */
	REG_APB3_ICU_CLOCKSOURCE    = SYSCLK_HFSRC_30M_DCO;
		/* CLOSE BK5824 0WK Mode */
	BK5824Power(Disable);
		/* close spibmen */
	REG_APB3_ICU_ANA1_CTRL &= 0xffefffff; 
	
	REG_APB3_ICU_ANA3_CTRL &= 0xfffffffe;
		/* FLASH CLK */
	set_flash_clk(FLASH_DCO_CLK_32MHz);
	if(REG_APB3_ICU_CPUCLKCON  & (0x1UL<<9)){
		mcu64M_state = 1;
		REG_APB3_ICU_CPUCLKCON  |= (0x1UL<<9);
		delay_us(10);
	}
	else {
		mcu64M_state = 0;
	}
	REG_APB3_ICU_CPUCLKCON  |= (0x1UL<<9);
	
	/* XTAL CLOSE 32MHZ XTAL */	
	REG_APB7_GPIOC_DATA        &= 0xfffffffe;
#if 0
	REG_APB7_GPIOE_CFG &= (~0x04040404);
	REG_APB7_GPIOE_CFG |= 0x0404;
	REG_APB7_GPIOE_DATA |= 0x04;
	REG_APB7_GPIOE_DATA &= (~0x040000);
#endif
	REG_APB3_ICU_IRQTYPE_EN = CpuFlag;

	REG_APB3_ICU_CPUCLKCON  |= (0x1UL<<9);
	REG_APB11_LBD_CLK32K_PWD	   = 0x1;					//close 32kHz
	REG_APB3_ICU_CPUCLKCON  |= 0x00000100;
	/* Wake up */
	REG_APB7_GPIOC_CFG |= 0x00010001;  //ÉÏÀ­ ÆÕÍ¨IO 
	REG_APB7_GPIOC_CFG &= (~0x00000100);  //Êä³öÊ¹ÄÜ
	REG_APB7_GPIOC_DATA |= 0x01;  //¿ªÊ±ÖÓ
	/* Switch to 32MHz XTAL */
	REG_APB3_ICU_CLOCKSOURCE    = SYSCLK_HFSRC_32M;
	if(mcu64M_state){
		REG_APB3_ICU_CPUCLKCON  &= ~(0x1UL<<9);
	}
}

/**************************************************
FunctionName: TEST5863N
Describe:
InputParameter:	
OutputParameter:
Notice:
**************************************************/
void ReadAndConfigADC(unsigned int Para);
void test_bk5863N_pm2_function(unsigned int temp){
	GPIOConfig_T GIPOConfig;
	unsigned char mcu64M_state;
	unsigned int reg9;
	GPIOTestConfig();
	BK5824SpiRead(9, (unsigned char*)&reg9);
	printf("reg9:0x%x\r\n",reg9);
	printf("******** MCUSleep ********\r\n");
	LEDClose(0x03);
	delay_ms(10);
	temp = REG_APB3_ICU_IRQTYPE_EN;
		/*  irq disable & FIQ disable */
	REG_APB3_ICU_IRQTYPE_EN     = 0x00;  	

	/* power down card reader */
	REG_APB3_ICU_ANA2_CTRL     |= 0x00040000;	  
	/* cpu clock switch dco, prepare sleep */
	REG_APB3_ICU_CLOCKSOURCE    = SYSCLK_HFSRC_30M_DCO;
	/* CLOSE BK5824 0WK Mode */
	BK5824Power(Disable);
	/* close spibmen */
	REG_APB3_ICU_ANA1_CTRL &= 0xffefffff; 
	
	REG_APB3_ICU_ANA3_CTRL &= 0xfffffffe;
	/* FLASH CLK */
	set_flash_clk(FLASH_DCO_CLK_32MHz);
	if(REG_APB3_ICU_CPUCLKCON  & (0x1UL<<9)) {
		mcu64M_state = 1;
		REG_APB3_ICU_CPUCLKCON  |= (0x1UL<<9);
		delay_us(100);
	}
	else {
		mcu64M_state = 0;
	}
	REG_APB3_ICU_CPUCLKCON  |= (0x1UL<<9);
	/* XTAL CLOSE 32MHZ XTAL */	
	REG_APB7_GPIOC_DATA        &= 0xfffffffe;
	///uart
	REG_APB7_GPIOD_CFG &= ~((0x03<<4)|(0x03<<4<<8)|(0x03<<4<<16)|(0x03U<<4<<24)); 
	REG_APB7_GPIOD_CFG |= ((0x03<<4)|(0x03<<4<<8)|(0x03<<4<<16));   ///æ™®é€šï¼Œä¸è¾“å‡ºï¼Œä¸Šæ‹‰
	REG_APB7_GPIOD_DATA &= ~((0x03<<4)|(0x03<<4<<8)|(0x03<<4<<16));
	REG_APB7_GPIOD_DATA |= (0x03<<4)|(0x03<<4<<16);   ///è¾“å‡ºå€¼1ï¼Œè¾“å…¥æ¨¡å¼
	REG_APB7_GPIO_INT_ENABLE |= (0x03<<4<<24);
	REG_APB3_ICU_UART1CLKCON = 1;
	GIPOConfig.GPIO_PERI = GPIO_General_Function;  //ÆÕÍ¨¹¦ÄÜ
	GIPOConfig.GPIO_OE_N_INTERN = Disable;  //Êä³öÊ¹ÄÜ
	GIPOConfig.GPIO_I_IE = Enable;  //ÊäÈëÊ¹ÄÜ
	GIPOConfig.GPIO_PULLUP = Disable;  //ÉÏÀ­Ê¹ÄÜ
	GIPOConfig.GPIO_PULLDOWN = Enable;  //ÏÂÀ­Ê¹ÄÜ
	
	GIPOConfig.Int.isINT = Enable;    //ÅäÖÃÖÐ¶Ï
	GIPOConfig.Int.En_Dis_able = Enable;  //GPIO_PinÖÐ¶Ï
	GIPOConfig.Int.INTMode = edge_Trigging; //±ßÑØ´¥·¢
	GIPOConfig.Int.dpulse = _DPULSE;  //Ë«ÑØ´¥·¢
	GIPOConfig.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_4;  //Ñ¡ÔñGPIO_Pin
	
	GPIO_Config(GPIOF,&GIPOConfig);

	
	REG_APB3_ICU_IRQTYPE_EN = temp;

	REG_APB3_ICU_CPUCLKCON  |= (0x1UL<<9);
	REG_APB11_LBD_CLK32K_PWD	   = 0x1;					//close 32kHz
	REG_APB3_ICU_CPUCLKCON  |= 0x00000100;
	/* Wake up */
	REG_APB7_GPIOC_CFG |= 0x00010001;  //ÉÏÀ­ ÆÕÍ¨IO 
	REG_APB7_GPIOC_CFG &= (~0x00000100);  //Êä³öÊ¹ÄÜ
	REG_APB7_GPIOC_DATA |= 0x01;  //¿ªÊ±ÖÓ
	/* Switch to 32MHz XTAL */
	REG_APB3_ICU_CLOCKSOURCE    = SYSCLK_HFSRC_32M;
	///uart
	REG_APB7_GPIOD_CFG &= ~((0x03<<4)|(0x03<<4<<8)|(0x03<<4<<16)|(0x03U<<4<<24)); 
	REG_APB7_GPIOD_CFG |= ((0x03<<4)|(0x03<<4<<8)|(0x03<<4<<16));   ///æ™®é€šï¼Œä¸è¾“å‡ºï¼Œä¸Šæ‹‰
	REG_APB7_GPIOD_DATA &= ~((0x03<<4)|(0x03<<4<<8)|(0x03<<4<<16));
	///REG_APB7_GPIOD_DATA |= (0x03<<4)|(0x03<<4<<16);   ///è¾“å‡ºå€¼1ï¼Œè¾“å…¥æ¨¡å¼
	REG_APB7_GPIOD_CFG &= (~(0x03<<4));    ///ç¬¬äºŒåŠŸèƒ½ä½¿èƒ½ UART1
	REG_APB7_GPIO_INT_ENABLE &= ~(0x03<<4<<24);
	REG_APB3_ICU_UART1CLKCON = 0;
	
	if(mcu64M_state) {
		REG_APB3_ICU_CPUCLKCON  &= ~(0x1UL<<9);
	}
	LEDOpen(0x03);
	printf("******** MCU Sleep Wakeup********\r\n");
	BK5824SpiRead(9, (unsigned char*)&temp);
	printf("reg9:0x%x\r\n",temp);
	if(temp == reg9) {
		printf("BK5824 NO Reset!(icu0x30<18:19> = %d)\r\n",((*((volatile unsigned long *)   (0x00a03000+0x30*4)))>>18)&0x3);
	}
	else {
		printf("BK5824 Reset\r\n");
	}
	delay_ms(100);
	BKUART_CTL_BLK[0].rxRdIndex = BKUART_CTL_BLK[0].rxWrIndex;
	LEDClose(0x03);
}
void TEST5863N(unsigned int Cmd,unsigned char *pData)
{
	unsigned int temp;
	
	switch(Cmd)
	{
		case 0:
		{
			printf("******** MCUSleep -- Config GPIOF5 Wakeup********\r\n");
			delay_ms(10);
			LEDClose(0x03);
			
			if(!(pData[0]&0x02)) {
				CmdCharge(Enable);
			}
			else {
				CmdCharge(Disable);
			}
			
			if(pData[0]&(0x01<<2)){
				set_bk5824_wko_weakup(0);
			}

			if(pData[0]&(0x01<<3)){
				set_bk5824_irq_weakup(0);
			}
			
			if(pData[0]&0x01) {
				GPIOTestConfig(); 
				LEDClose(0x03);
				MCUSleep();
				LEDOpen(0x03);
				printf("******** MCU Sleep Wakeup********\r\n");
				delay_ms(10);
			}
			break;
		}
		case 1:
		{
			if(pData[0] == 0x00)
			{
				printf("********_DeepSleep********\r\n");
				delay_ms(10);
				BK5863nSysSleep(_DeepSleep);
			}
			else if(pData[0] == 0x01)
			{
				printf("********_OnlyCharge********\r\n");
				delay_ms(10);
				BK5863nSysSleep(_OnlyCharge);
			}
			else if(pData[0] == 0x02)
			{
				printf("********_OnlyRFRouse(Do Config BK5824)********\r\n");
				delay_ms(10);
				bk_test_obu0Wakeup();
				BK5863nSysSleep(_OnlyRFRouse); 
			}
			else if(pData[0] == 0x03)
			{
				printf("********_RFRouseAndCharge(Do Config BK5824)********\r\n");
				delay_ms(10);
				bk_test_obu0Wakeup();
				BK5863nSysSleep(_RFRouseAndCharge); 
			}
			
			printf("-----  _MCU Wake UP-----  \r\n");
			break;
		}
		case 2:
		{
			temp = pData[0]+(pData[1]<<8);
			temp = temp + (pData[2]<<16) + (pData[3]<<24);
			printf("------- ADC CHx Test!-------\r\n");   
			BK5863WDT(0xFFF0);			
			ReadAndConfigADC(temp);
			BK5863WDTClose();
			break;
		}
		case 3:
		{
			break;
		}
		case 4:
		{
			temp = pData[0]+(pData[1]<<8);
			temp = temp + (pData[2]<<16) + (pData[3]<<24);
			if(pData[0]&(0x01<<0)) {
				set_bk5824_wko_weakup(1);
			}else {
				set_bk5824_wko_weakup(Disable);
			}
			
			if(pData[0]&(0x01<<1)) {
				set_bk5824_irq_weakup(1);
			}else {
				set_bk5824_irq_weakup(Disable);
			}
			break;
		}
		case 5:
		{
			temp = pData[0]+(pData[1]<<8);
			temp = temp + (pData[2]<<16) + (pData[3]<<24);
			if(temp&(1<<0)) {
				REG_APB3_ICU_INT_WAKENA |= INT_STATUS_UART1_bit;
				///REG_APB8_UART1_WAKE_CTRL &= ~((1<<20)|(1<<22));
				REG_APB8_UART1_WAKE_CTRL |= ((1<<20)|(1<<22));
			}
			else {
				REG_APB3_ICU_INT_WAKENA &= (~INT_STATUS_UART1_bit);
				REG_APB8_UART1_WAKE_CTRL &= ~((1<<20)|(1<<22));
			}
			break;
		}
		case 6:
			temp = pData[0]+(pData[1]<<8);
			temp = temp + (pData[2]<<16) + (pData[3]<<24);
			BK5863WDT(0xFFF0);			
			bk_test_adc(temp&0x0F);
			BK5863WDTClose();
			break;
		case 7:
			{
				temp = pData[0]+(pData[1]<<8);
				temp = temp + (pData[2]<<16) + (pData[3]<<24);
				(*((volatile unsigned long *)   (0x00a03000+0x30*4))) &= (~(3<<18));
				(*((volatile unsigned long *)   (0x00a03000+0x30*4))) |= ((temp&0x3)<<18);
				
				LEDClose(0x03);
				if((temp&(0x03<<3)) == 0) {
					CmdCharge(Enable);
				}
				else if(((temp&(0x03<<3))>>3) == 1) {
					CmdCharge(Disable);
				}
				
				if(temp&0x02) {
					bk_test_obu0Wakeup();
				}
				
				if(temp&(0x01<<5)) {
					BK5824SpiRead(9, (unsigned char*)&temp);
					printf("Reg9:%x\r\n",temp);
					GPIOTestConfig();
					printf("******** MCUSleep ********\r\n");
					delay_ms(10);
					LEDClose(0x03);
					MCUSleep();
					LEDOpen(0x03);
					printf("******** MCU Sleep Wakeup********\r\n");
					
					BK5824SpiRead(9, (unsigned char*)&temp);
					if(temp != 0) {
						printf("BK5824 NO Reset\r\n");
					}
					else {
						printf("BK5824 Reset\r\n");
					}
				}
				
			}
			break;
		case 8:
			{
				temp = pData[0]+(pData[1]<<8);
				temp = temp + (pData[2]<<16) + (pData[3]<<24);
				(*((volatile unsigned long *)   (0x00a03000+0x30*4))) &= (~(3<<18));
				(*((volatile unsigned long *)   (0x00a03000+0x30*4))) |= ((temp&0x3)<<18);
				
				LEDClose(0x03);
				if((temp&(0x03<<3)) == 0) {
					CmdCharge(Enable);
				}
				else if(((temp&(0x03<<3))>>3) == 1){
					CmdCharge(Disable);
				}
				
				if(temp&0x02){
					bk_test_obu0Wakeup();
				}
				
				if(temp&(0x01<<6)) {
					set_bk5824_wko_weakup(Disable);
				}
				
				if(temp&(0x01<<7)) {
					set_bk5824_irq_weakup(Disable);
				}
				
				if((temp&(0x01<<8))&&(temp&(0x01<<5))) {
					test_bk5863N_pm2_function(temp);
				}
				else if(temp&(0x01<<5)) {
					BK5824SpiRead(9, (unsigned char*)&temp);
					printf("Reg9:%x\r\n",temp);
					GPIOTestConfig();
					printf("******** MCUSleep ********\r\n");
					delay_ms(10);
					LEDClose(0x03);
					MCUSleep();
					LEDOpen(0x03);
					printf("******** MCU Sleep Wakeup********\r\n");
					BK5824SpiRead(9, (unsigned char*)&temp);
					printf("Reg9:%x\r\n",temp);
					if(temp != 0) {
						printf("BK5824 NO Reset!(icu0x30<18:19> = %d)\r\n",((*((volatile unsigned long *)   (0x00a03000+0x30*4)))>>18)&0x3);
					}
					else {
						printf("BK5824 Reset\r\n");
					}
				}
			}
			break;
		case 9:
			{
				temp = pData[0]+(pData[1]<<8);
				temp = temp + (pData[2]<<16) + (pData[3]<<24);
				BK5863CardReaderInit();
				TestInfo.cardreader_init++;
				memset(&CrdRdMsg, sizeof(CrdRdMsg), 0);
				CrdRdMsg.BlockNum = TestInfo.KuaiN;
				CrdRdMsg.CardType = TestInfo.CardType;
				if((temp&(0x03U<<30)) == (0x01U<<30)) {
					CrdRdMsg.CardType = 1;	//CPU
				}
				else if((temp&(0x03U<<30)) == (0x00U<<30)) {
					CrdRdMsg.CardType = 0;	//M1
				}
				else {
					CrdRdMsg.CardType = 2;	//
				}
				CrdRdMsg.OprtMode = READCARD;
				CrdRdMsg.PiccCid  = TestInfo.PiccCid;
				CrdRdMsg.TxSpeed  = TestInfo.TxSpeed;
				CrdRdMsg.RxSpeed  = TestInfo.RxSpeed;
				memcpy(CrdRdMsg.Key, TestInfo.PassWd, 6);
				CrdRdMsg.FrameDly = TestInfo.PcdFrameDly;
				CrdRdMsg.ReqGuardTime = TestInfo.PcdReqGuardTime;
				CrdRdMsg.SFGIDly  = TestInfo.PcdSFGIDly;
				TestInfo.PcdOprtNum = temp;   ///times
				CrdRdMsg.OprtNum  = TestInfo.PcdOprtNum;
				BK5863TestCardReader(&CrdRdMsg);
				break;

			}
		default:
			break;
	}
}

#ifdef BK5870
void TEST5870(unsigned int Cmd,unsigned char *pData)
{
	unsigned int temp;
	switch(Cmd)
	{
		case 0:
		{
			printf("******** MCUSleep -- Config GPIOF5 Wakeup********\r\n");
			delay_ms(10);
			LEDClose(0x03);
			if(pData[0]&0x01) {
				CmdCharge(Enable);
			}
			else {
				CmdCharge(Disable);
			}
			if(pData[0]&(0x01<<2)){
				set_bk5824_wko_weakup(1);
			}else{
				set_bk5824_wko_weakup(Disable);
			}
			if(pData[0]&(0x01<<3)){
				set_bk5824_irq_weakup(1);
			}else{
				set_bk5824_irq_weakup(Disable);
			}
			
			if(pData[0]&0x02) {
				GPIOTestConfig(); 
				LEDClose(0x03);
				MCUSleep();
				LEDOpen(0x03);
				printf("******** MCU Sleep Wakeup********\r\n");
				delay_ms(10);
			}
			break;
		}
		case 1:
		{
			if(pData[0]==0x00)
			{
				printf("********_DeepSleep********\r\n");
				delay_ms(10);
				BK5863nSysSleep(_DeepSleep);
			}
			else if(pData[0]==0x01)
			{
				printf("********_OnlyCharge********\r\n");
			  delay_ms(10);
				BK5863nSysSleep(_OnlyCharge);
			}
			else if(pData[0]==0x02)
			{
				printf("********_OnlyRFRouse(Do Config BK5824)********\r\n");
				delay_ms(10);
				bk_test_obu0Wakeup();
				BK5863nSysSleep(_OnlyRFRouse); 
			}
			else if(pData[0]==0x03)
			{
				printf("********_RFRouseAndCharge(Do Config BK5824)********\r\n");
				delay_ms(10);
				bk_test_obu0Wakeup();
				BK5863nSysSleep(_RFRouseAndCharge); 
			}
			printf("-----  _MCU Wake UP-----  \r\n");
			break;
		}
		case 2:
		{
			temp = pData[0]+(pData[1]<<8);
			temp = temp + (pData[2]<<16) + (pData[3]<<24);
			printf("------- ADC CHx Test!-------\r\n");   
			BK5863WDT(0xFFF0);			
			ReadAndConfigADC(temp);
			BK5863WDTClose();
			break;
		}
		case 3:
		{
			temp = pData[0]+(pData[1]<<8);
			temp = temp + (pData[2]<<16) + (pData[3]<<24);
			if(temp&(0x1<<2)) {				
				switch(temp&0x07) {
					case 0x05:   ///C7 output = 1
						bk5824_internal_audion_ctrl(BK5824_INT_AUD_EN);
						break;
					case 0x06:   ///c7 pp output = 1
						bk5824_internal_audion_ctrl(BK5824_INT_AUD_EN_PP);
						break;
					case 0x04:   ///c7 output = 0
						bk5824_internal_audion_ctrl(BK5824_INT_AUD_DIS);
						break;
					default:  ///c7 != output&input
						bk5824_internal_audion_ctrl(BK5824_INT_AUD_NULL);
	 					break;
				}
			}
			else
			{
				bk5824_internal_audion_ctrl(BK5824_INT_AUD_NULL);
			}
			break;
		}
		case 4:
		{
			temp = pData[0]+(pData[1]<<8);
			temp = temp + (pData[2]<<16) + (pData[3]<<24);
			if(pData[0]&(0x01<<0)){
				set_bk5824_wko_weakup(1);
			}else{
				set_bk5824_wko_weakup(Disable);
			}
			if(pData[0]&(0x01<<1)){
				set_bk5824_irq_weakup(1);
			}else{
				set_bk5824_irq_weakup(Disable);
			}
			break;
		}
		case 5:
		{
			temp = pData[0]+(pData[1]<<8);
			temp = temp + (pData[2]<<16) + (pData[3]<<24);
			if(temp&(1<<0)) {
				REG_APB3_ICU_INT_WAKENA |= INT_STATUS_UART1_bit;
				///REG_APB8_UART1_WAKE_CTRL &= ~((1<<20)|(1<<22));
				REG_APB8_UART1_WAKE_CTRL |= ((1<<20)|(1<<22));
			}
			else {
				REG_APB3_ICU_INT_WAKENA &= (~INT_STATUS_UART1_bit);
				REG_APB8_UART1_WAKE_CTRL &= ~((1<<20)|(1<<22));
			}
			break;
		}
		case 6:
		{
			temp = pData[0]+(pData[1]<<8);
			temp = temp + (pData[2]<<16) + (pData[3]<<24);
			BK5863WDT(0xFFF0);			
			bk_test_adc(temp&0x0F);
			BK5863WDTClose();
		}
		default:
			break;
	}
}
#endif

/********************************************************
	Description: BEKEN BK5863 ADC CH2 example code
********************************************************/
void ReadAndConfigADC(unsigned int Para)
{
	int i;
	unsigned int length = (Para>>25)&0x7F;//100;
	unsigned int *adc_data = Temp_data;
	BKADCCONFIG_STRU config;
	printf("adc testing\n");
	if(length>100) length = 100;
	if(length<= 0) length = 1;
	memset(adc_data, 0, length);
  BK5863ADCInit(adc_data,length);	
	/*****************************************************************
		function test: channel = 1;  product version: channel = 2
	*****************************************************************/
	config.channel = Para&0x0F;
	config.clk_rate = (Para>>7)&0x7;
	config.mode = (Para>>10)&3;
	config.settling = (Para>>6)&1;//0;
	config.sample_rate = (Para>>4)&3;
	printf("adc Channel %d\n",config.channel);
	BK5863ADCConfig(&config,BK_ADC_MODE|BK_ADC_CHNL|BK_ADC_SAMPLE_RATE|BK_ADC_SETTLING|BK_ADC_CLK_RATE);
	delay_us(100);
	REG_APB3_ICU_ADCCLKCON  = 1;		//close ADC test 
	REG_APB3_ICU_INT_ENABLE &= ~INT_STATUS_ADC_bit;			//Disable interrupt
	for(i=0; i<length; i++)
		printf("%x \r\n", adc_data[i]);
}

