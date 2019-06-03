#include "BKTestAPP.h"
#include "BKTestD.h"
#include "Includes.h"

/**************************************************/

TestPara_T TestInfo;

/**************************************************/
const unsigned char write_Data[16]= {0x01, 0x02, 0x03, 0x04, 0x05,
	0x06, 0x07, 0x08, 0x09, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16};								
const unsigned char passWd[6]={0xff, 0xff, 0xff, 0xff, 0xff, 0xff};	

/**************************************************
FunctionName:TestInit
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
void TestInit(void)
{
	unsigned char i;
	TestInfo.uart_state = 0;
	TestInfo.CardType = 0;
	TestInfo.TxSpeed = 0;
	TestInfo.RxSpeed = 0;
	TestInfo.PiccCid = 2;
	TestInfo.KuaiN  = 1;
	
	TestInfo.BK5823BerEn = 0;		
  TestInfo.BK5824RecTest = 1;
	
	for(i=0;i<16;i++)
	{
		TestInfo.Write_Data[i] = write_Data[i];
	}
  for(i=0;i<6;i++)
	{
		TestInfo.PassWd[i] = passWd[i];
	}
	TestInfo.PcdFrameDly = 25;
	TestInfo.PcdReqGuardTime = 500;
	TestInfo.PcdSFGIDly = 0;
	TestInfo.PcdOprtNum = 16;
	TestInfo.StandbyMode = 1;
	TestInfo.SectorMode = 0;
	TestInfo.SpiIdx = 1;
	TestInfo.SpiNssmd = 3;
	TestInfo.SpiCkphapol = 0;
	TestInfo.SpiRole = 1;
	TestInfo.SpiCkr = 0x03;
	TestInfo.SpiDataLen = 100;
	/* IIC */
	TestInfo.I2cIdx = 0;  //IICx IIC1 = 0,iiC2 = 1
	TestInfo.I2cRole = 0;  //½ÇÉ«
	TestInfo.I2cRwMode = 0x1;  //
	TestInfo.I2cLocalAddr = 0x3a;  //
	TestInfo.I2cRemoteAddr = 0xA0;
	TestInfo.I2cDataLen = 10;  
	TestInfo.I2cClkdiv = 0x1f;  //·ÖÆµ
}

/**************************************************
FunctionName:TestConfig
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
void TestConfig(void)
{
	TestInit();
	if(!(REG_APB7_GPIOE_DATA & 0x00001000))       //master gpioE[4]
	{
		/* SPI */
		TestInfo.SpiRole = 1;
		TestInfo.SpiNssmd = 3;
		TestInfo.SpiCkr = 0x03;
		/* I2C */
		TestInfo.I2cRole = 1;
		TestInfo.I2cLocalAddr = 0x3a;
		TestInfo.I2cRemoteAddr = 0xA0;
	}
	else  //slave
	{
			/* SPI */
		TestInfo.SpiRole = 0;
		TestInfo.SpiNssmd = 1;
		TestInfo.SpiCkr = 0x00;
			/* I2C */
		TestInfo.I2cRole = 0;
		TestInfo.I2cLocalAddr = 0x68;
		TestInfo.I2cRemoteAddr = 0x3a;
	}
}
/**************************************************
FunctionName:
Describe:Uart1 Command Parser 
InputParameter:
OutputParameter:
Notice:
**************************************************/
void application_uart_command_analysis(void)
{
	unsigned short Key;
//	unsigned int temp;
	static unsigned char DataLen;

	if(BKUART_CTL_BLK[0].rxRdIndex == BKUART_CTL_BLK[0].rxWrIndex)
	{
		return;
	}
	
	Key = BKUART_CTL_BLK[0].pRx[BKUART_CTL_BLK[0].rxRdIndex++];
	if(BKUART_CTL_BLK[0].rxRdIndex >= BKUART_CTL_BLK[0].rxBufLen)
		BKUART_CTL_BLK[0].rxRdIndex = 0;
    
	switch(TestInfo.uart_state)
	{
		case 0:
			if( Key == 0x01 )
			{
				TestInfo.uart_state = 1;
			}
			else if( Key == 0x02 )
			{
				bk_RX_data_test();
				TestInfo.uart_state = 0;
			}
			else if(Key == 0x03)
			{
				bk_TX_data_test();
				TestInfo.uart_state = 0;
			}
			else if(Key == 0x04)
			{
				 bk_enter_RX();
				 TestInfo.uart_state = 0;
			}
			else if(Key == 0x06)
			{
				printf("BK5863 Reset\r\n");
				REG_APB7_GPIOE_DATA      = 0x000000ff;
				BK5863Wakeup();
				BK5863WDT(64);
				TestInfo.uart_state = 0;
				delay_ms(1000);
			}
			break;
		case 1:
			if( Key == (TCI_BEKEN_HARDWARE_TEST&0xff) ) //e0
				TestInfo.uart_state = 2;
			else 
				TestInfo.uart_state = 0;
			break;
		case 2:
			if( Key == ( (TCI_BEKEN_HARDWARE_TEST>>8)&0xff) )  //fc
				TestInfo.uart_state = 3;
			else
				TestInfo.uart_state = 0;
			break;
		case 3:
			if(Key >= 5) //
				DataLen = Key-5;
			else 
				DataLen = 0;
			TestInfo.uart_state = 4;
			break;
		case 4:
			TestInfo.uart_op_cmd = Key;
			TestInfo.reg_addr = 0;
			TestInfo.reg_value = 0;
			TestInfo.addr_data_length = 0;
			if(Key == BEKEN_UART_LINK_CHECK) //0x00
			{
				driver_uart_link_check();
				TestInfo.uart_state = 0;
			}
			else if(Key == BEKEN_UART_REGISTER_WRITE_CMD)  //Ð´ 0x01
			{
				TestInfo.uart_state = 5;
			}
			else if(Key == BEKEN_UART_REGISTER_READ_CMD)  //¶Á 0x03
			{
				TestInfo.uart_state = 5;
			}
			else if(Key == BEKEN_UART_MODULE_TEST_CMD)  //80
			{
				TestInfo.uart_state = 7;
			}
			break;
		case 5:
			TestInfo.reg_addr = TestInfo.reg_addr>>8;
			TestInfo.reg_addr = TestInfo.reg_addr|(Key<<24);
			TestInfo.addr_data_length++;
			if(TestInfo.addr_data_length >= 4)
			{
				if(TestInfo.uart_op_cmd == BEKEN_UART_REGISTER_WRITE_CMD)  //Ð´ 0x01
				{
					TestInfo.addr_data_length = 0;
					TestInfo.uart_state = 6;
				}
				else if(TestInfo.uart_op_cmd == BEKEN_UART_REGISTER_READ_CMD) //¶Á 0x03
				{
					if((TestInfo.reg_addr & 0xfffff000) == 0x10a11000)  //Ð´BK5824²¿·Ö
					{
						TestInfo.uart_state = 10;
					}             
					else
					{
						driver_uart_read_register();
						TestInfo.uart_state = 0;
					}
				}
			}
			break;
		case 6:
			/* Ð´ */
			if((TestInfo.reg_addr & 0xfffff000) == 0x10a11000) //access bk5824
			{
				TestInfo.reg_data[TestInfo.addr_data_length] = Key;
			}
			else if((TestInfo.reg_addr & 0xfffff000) == 0x10b00000) 
			{
				/* update module test parameter */
				TestInfo.reg_data[TestInfo.addr_data_length] = Key;
			}
			else if((TestInfo.reg_addr & 0xfffff000) == 0x10b10000)  
			{
				DataLen = 4;
				TestInfo.reg_data[TestInfo.addr_data_length] = Key;
			}
			else if((TestInfo.reg_addr & 0xfffff000) == 0x10b20000)  
			{
				DataLen = 4;
				TestInfo.reg_data[TestInfo.addr_data_length] = Key;
			}
			else
			{
				TestInfo.reg_value = TestInfo.reg_value>>8;
				TestInfo.reg_value = TestInfo.reg_value|(Key<<24);
			}

			TestInfo.addr_data_length++;

			if(TestInfo.addr_data_length >= DataLen)
			{
				if((TestInfo.reg_addr & 0xfffff000) == 0x10a11000) //access bk5824
				{
				#if 0
					temp = TestInfo.reg_data[1]+(TestInfo.reg_data[2]<<8);
					temp |= (TestInfo.reg_data[3] << 16)+(TestInfo.reg_data[4]<<24);
					BK5824SpiWrite((TestInfo.reg_addr & 0xff),(unsigned char*)&temp);
				#else
					if((TestInfo.reg_addr & 0xff) != 0x10)
						BK5824SpiWrite((TestInfo.reg_addr & 0xff), TestInfo.reg_data+1);
				#endif
				}
				else if((TestInfo.reg_addr & 0xfffff000) == 0x10b00000) 
				{
					/* update module test parameter */
					BK5863ModuleParaUpdate(((TestInfo.reg_addr >> 2) & 0xff),TestInfo.reg_data);
				}
				else if((TestInfo.reg_addr & 0xfffff000) == 0x10b10000) 
				{
					TEST5863N((TestInfo.reg_addr&0xfff),TestInfo.reg_data);
				}
			#ifdef BK5870
				else if((TestInfo.reg_addr & 0xfffff000) == 0x10b20000) 
				{
					TEST5870((TestInfo.reg_addr&0xfff),TestInfo.reg_data);
				}
			#endif
				else if((TestInfo.reg_addr & 0xfffff000) == 0x00a11000)  //LBD register
				{
					TestInfo.reg_addr = (TestInfo.reg_addr & 0xff) | 0x00a11000;
					driver_uart_write_register();
				}
				else
				{
					driver_uart_write_register();
				}
				TestInfo.uart_state = 0;
			}
			break;
		case 7:
			TestInfo.test_item = Key<<8;
			TestInfo.uart_state = 8;
			break;
		case 8:
			TestInfo.test_item = TestInfo.test_item|Key;
			if(!DataLen)
			{
				Do_Total_Test();
				TestInfo.uart_state = 0;
			}
			else
			{
				TestInfo.uart_state = 9;
			}
			break;
		case 9:
			bk_config_flag[BK_CONFIG_FLAG_MAX-DataLen]= Key;
			DataLen--;
			
			if(!DataLen)
			{
				Do_Total_Test();
				TestInfo.uart_state = 0;
			}
			break;
		case 10:
			driver_uart_read_register();
			TestInfo.uart_state = 0;
			break;
	}
}

/*********************************************************
	Test Uart1 Link
*********************************************************/
void driver_uart_link_check(void)
{    
  TestInfo.Uart1TxLen = 0;
    
	UART_OUTPUT_CHAR(TRA_HCIT_EVENT);
	UART_OUTPUT_CHAR(HCI_COMMAND_COMPLETE_EVENT);
	UART_OUTPUT_CHAR(6);
	UART_OUTPUT_CHAR(1);
	UART_OUTPUT_CHAR(TCI_BEKEN_HARDWARE_TEST&0xff);
	UART_OUTPUT_CHAR((TCI_BEKEN_HARDWARE_TEST>>8)&0xff);
	UART_OUTPUT_CHAR(BEKEN_UART_LINK_CHECK);

	BK5863UartPrint(BK5863_UART1, Uart1TxBuff, TestInfo.Uart1TxLen);
}

/*********************************************************
Read Register from uart1 command 
*********************************************************/
void driver_uart_read_register(void)
{
	unsigned char i;
	unsigned char	len = 0;
//	unsigned int temp;
  TestInfo.Uart1TxLen = 0;
    
	if((TestInfo.reg_addr & 0xfffff000) == 0x10a11000) //access bk5824
	{
	#if 1
		BK5824SpiRead((TestInfo.reg_addr & 0xff),TestInfo.reg_data);
	#else
		BK5824SpiRead((TestInfo.reg_addr & 0xff),(unsigned char*)&temp);
		TestInfo.reg_data[0] = temp&0xFF;
		TestInfo.reg_data[1] = (temp>>8)&0xFF;
		TestInfo.reg_data[2] = (temp>>16)&0xFF;
		TestInfo.reg_data[3] = (temp>>24)&0xFF;
	#endif
	}
	else if((TestInfo.reg_addr & 0xfffff000) == 0x10b00000)	//module test para
	{
		#if 1
			len = BK5863ModuleParaRead(((TestInfo.reg_addr >> 2) & 0xff),TestInfo.reg_data);
	  #else
			BK5863ModuleParaRead(((TestInfo.reg_addr >> 2) & 0xff),TestInfo.reg_data);
		#endif
	}
	else if((TestInfo.reg_addr & 0xfffff000) == 0x00a11000)	//LBD register
	{
		TestInfo.reg_value = BK5863_READ_REG(0x00a11000 | (TestInfo.reg_addr & 0xff));
	}
	else
	{
		TestInfo.reg_value = BK5863_READ_REG(TestInfo.reg_addr);
	}
	
	UART_OUTPUT_CHAR(0x04);
	UART_OUTPUT_CHAR(HCI_COMMAND_COMPLETE_EVENT);
	if((TestInfo.reg_addr & 0xfffff000) == 0x10a11000) //access bk5824
	{
	#if BK5824_REG_LEN_OPTION == BK5824_REG_LEN_OPTION_PREV
		UART_OUTPUT_CHAR(BK5824RegReadLen[(TestInfo.reg_addr & 0xff)]+8);
	#else
		UART_OUTPUT_CHAR(get_bk5824_reg_read_len((TestInfo.reg_addr & 0xff))+8);
	#endif
	}
	else if((TestInfo.reg_addr & 0xfffff000) == 0x00a11000)	//LBD register
	{
		UART_OUTPUT_CHAR((len+8));
	}
	else if((TestInfo.reg_addr & 0xfffff000) == 0x10b00000)	//module test para
	{
		UART_OUTPUT_CHAR((len+8));
	}
	else
	{
		UART_OUTPUT_CHAR(12);
	}
	UART_OUTPUT_CHAR(1);
	UART_OUTPUT_CHAR(TCI_BEKEN_HARDWARE_TEST&0xff);
	UART_OUTPUT_CHAR((TCI_BEKEN_HARDWARE_TEST>>8)&0xff);
	UART_OUTPUT_CHAR(BEKEN_UART_REGISTER_READ_CMD);
	UART_OUTPUT_CHAR((TestInfo.reg_addr>>24)&0xff);
	UART_OUTPUT_CHAR((TestInfo.reg_addr>>16)&0xff);
	UART_OUTPUT_CHAR((TestInfo.reg_addr>>8)&0xff);
	UART_OUTPUT_CHAR((TestInfo.reg_addr>>0)&0xff);
	if((TestInfo.reg_addr & 0xfffff000) == 0x10a11000) //access bk5823
	{
	#if BK5824_REG_LEN_OPTION == BK5824_REG_LEN_OPTION_PREV
		for(i = 0; i < BK5824RegReadLen[(TestInfo.reg_addr & 0xff)]; i ++)
	#else
		for(i = 0; i < get_bk5824_reg_read_len((TestInfo.reg_addr & 0xff)); i ++)
	#endif
			UART_OUTPUT_CHAR(TestInfo.reg_data[i]);
	}
	else if((TestInfo.reg_addr & 0xfffff000) == 0x10b00000)	//module test para
	{
		for(i = 0; i < len; i ++)
			UART_OUTPUT_CHAR(TestInfo.reg_data[i]);
	}
	else
	{
		UART_OUTPUT_CHAR((TestInfo.reg_value>>0)&0xff);
		UART_OUTPUT_CHAR((TestInfo.reg_value>>8)&0xff);
		UART_OUTPUT_CHAR((TestInfo.reg_value>>16)&0xff);
		UART_OUTPUT_CHAR((TestInfo.reg_value>>24)&0xff);
	}
  BK5863UartPrint(BK5863_UART1, Uart1TxBuff, TestInfo.Uart1TxLen);
}

/*********************************************************
Paramter Update for Module Test 
*********************************************************/
void BK5863ModuleParaUpdate(unsigned char addr, unsigned char *pData)
{
	unsigned char   Para;
	unsigned char   i;

	switch(addr)
	{
		//Card Reader Parameter
		case 0x00:	Para = *pData++;
								TestInfo.CardType = Para & 0x01;
								TestInfo.TxSpeed = (Para >> 4) & 0x03;
								TestInfo.RxSpeed = (Para >> 6) & 0x03;
								TestInfo.KuaiN = *pData++;
								TestInfo.PiccCid = *pData++;
								Para = *pData++;
								TestInfo.BK5823BerEn = Para & 0x01;
					break;
		case 0x01:
		case 0x02:
		case 0x03:
		case 0x04: 	for(i = 0; i < 4; i++)
									TestInfo.Write_Data[i+(addr-1)*4] = *pData++;
			break;
		case 0x05:  for(i = 0; i < 4; i++)
										TestInfo.PassWd[i] = *pData++;
			break;
		case 0x06:  TestInfo.PassWd[4] = *pData++;
								TestInfo.PassWd[5] = *pData++;
								break;
		case 0x09:  Para = *pData++;
								TestInfo.PcdFrameDly = Para;
								Para = *pData++;
								TestInfo.PcdFrameDly |= Para << 8;
								Para = *pData++;
								TestInfo.PcdReqGuardTime = Para;
								Para = *pData++;
								TestInfo.PcdReqGuardTime |= Para << 8;
								break;
		case 0x0a:  Para = *pData++;
								TestInfo.PcdSFGIDly = Para;
								Para = *pData++;
								TestInfo.PcdSFGIDly |= Para << 8;   
								break;
		case 0x0b:  TestInfo.PcdOprtNum = *((unsigned int*)pData); 
								break;
		case 0x0c:  Para = *pData++;
								TestInfo.SectorMode = (Para >> 3) & 0x01;
								TestInfo.StandbyMode = Para & 0x07;
								Para = *pData++;
								TestInfo.SpiNssmd = Para & 0x3;
								TestInfo.SpiCkphapol = (Para >> 2) & 0x03;
								TestInfo.SpiRole = (Para >> 4) & 0x1;
								TestInfo.SpiIdx = (Para >> 6) & 0x03;
								TestInfo.SpiCkr = *pData++;;
			break;
		case 0x0d:  TestInfo.SpiDataLen = *((unsigned int*)pData);  
								break;
		case 0x0e:  Para = *pData++;
								TestInfo.I2cIdx = Para & 0x03;
								TestInfo.I2cRole = (Para >> 2) & 0x01;
								TestInfo.I2cRwMode = (Para >> 3) & 0x01;
								TestInfo.I2cLocalAddr = *pData++;
								TestInfo.I2cRemoteAddr = *pData++;
								break;
		case 0x0f:  TestInfo.I2cDataLen = *((unsigned int*)pData);  
								break;
		default	:	break;
	}
}


