#include "BK5824Test.h"
#include "BK5863.h"
#include "Includes.h"


/**************************************************
FunctionName: bk_RX_data_test
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
void bk_RX_data_test(void)
{
    unsigned char reg_value = 0;
    unsigned char data[128] = {0};
    unsigned char Len;
    int i = 0;
	
    BK5824SpiRead(6, (unsigned char*)&reg_value);
    if(reg_value & 0x02)
    {
			BK5824SpiRead(5, (unsigned char*)&Len); 
			printf("BK5823 RX data length: %d \r\n", Len);
		#if BK5824_REG_LEN_OPTION == BK5824_REG_LEN_OPTION_PREV
			BK5824RegReadLen[16] = Len;	
		#else
			set_bk5824_reg_read_len(16,Len);
		#endif
			BK5824SpiRead(16, data);
			for(i=0; i<Len; i++)
			{
				printf("%d ", data[i]);
				if((i % 5) == 4) printf("\r\n");
			}
    }
    else
    {
        printf("RX error! Register 6:0x%x \r\n", reg_value);
    }
    printf("RX test finish \r\n"); 
}
/**************************************************
FunctionName:
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
void bk_TX_data_test(void)
{
    unsigned char reg_value = 0;
    unsigned char data[22] = {0,1,2,3,4,5, 6, 7, 8, 9, 0,1,2,3,4,5, 6,7,8,9,0,1};

    BK5824SpiRead(9, (unsigned char*)&reg_value);
    reg_value |= 1;                                //set Idle mode 
    BK5824SpiWrite(9,(unsigned char*)&reg_value);
#if BK5824_REG_LEN_OPTION == BK5824_REG_LEN_OPTION_PREV
    BK5824RegWriteLen[16] = 22;
#else
	set_bk5824_reg_write_len(16,22);
#endif
    BK5824SpiWrite(16, (unsigned char*)data);
    printf("TX test finish \r\n");
    
}
/**************************************************
FunctionName:
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
void bk_enter_RX(void)
{
	unsigned int  reg_value;
//  unsigned int err,total;
	
	BK5824SpiRead(9, (unsigned char*)&reg_value);
	reg_value |= 1;                                     //bit 0 = 1 idle
	BK5824SpiWrite(9,(unsigned char*)&reg_value);

	BK5824SpiRead(15, (unsigned char*)&reg_value);		//Set RX mode 
	reg_value |= (1<<1);
	BK5824SpiWrite(15,(unsigned char*)&reg_value);
	

	BK5824SpiRead(15, (unsigned char*)&reg_value);
	reg_value |= (1<<0);                                // TRX RESET: "1"--"0"--"1"
	BK5824SpiWrite(15,(unsigned char*)&reg_value);
	delay_us(100);

	BK5824SpiRead(15, (unsigned char*)&reg_value);
	reg_value &= ~(1<<0);
	BK5824SpiWrite(15,(unsigned char*)&reg_value);
	delay_us(100);

	BK5824SpiRead(15, (unsigned char*)&reg_value);
	reg_value |= (1<<0);
	BK5824SpiWrite(15, (unsigned char*)&reg_value);
	delay_us(100);    
}

#ifdef BEKEN_BK5824_MODULE_TEST
/**************************************************
FunctionName:
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
void bk_test_pn9(void)
{
	unsigned int  reg_value = bk5823_rf_pn9[5];
	unsigned int Bk5823_rf_pn9[16];
	unsigned char Bk5823_rf_pn9_17[14];
	unsigned char Bk5823_rf_pn9_18[11];
	unsigned char i;
  for(i=0;i<16;i++)
	{
		Bk5823_rf_pn9[i] = bk5823_rf_pn9[i];
	}
	for(i=0;i<14;i++)
	{
		Bk5823_rf_pn9_17[i] = bk5823_rf_pn9_17[i];
	}
	for(i=0;i<11;i++)
	{
		Bk5823_rf_pn9_18[i] = bk5823_rf_pn9_18[i];
	}
    if(bk_config_flag[BK_5823_TX_RF_BASE]== 1)
    {
			Bk5823_rf_pn9[1] = 0x760FF200;	    //TX Carrier = 5.8G   
    }
    else
    {
			Bk5823_rf_pn9[1]  = 0x760FF200;   //TX Carrier = 5.79G 
    }

    Bk5823_rf_pn9[9] &= ~(0x07<<1);             //PA power setting 
    Bk5823_rf_pn9[9] |= (bk_config_flag[BK_5823_POWER_SETTING]<<1);

    BK5824RegConfig(Bk5823_rf_pn9, Bk5823_rf_pn9_17, Bk5823_rf_pn9_18);
    reg_value  |= (1<<26);
    BK5824SpiWrite(5,(unsigned char*)&reg_value);
    delay_us(10);
    reg_value = bk5823_rf_pn9[5];
    BK5824SpiWrite(5,(unsigned char*)&reg_value);
	
}
/**************************************************
FunctionName:
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
void bk_test_ber(void)
{
	unsigned int err,total, reg_value;    
	int ber, i; 
	unsigned int Bk5824_rf_ber[16];
	unsigned char Bk5824_rf_ber_17[14];
	unsigned char Bk5824_rf_ber_18[11];
  for(i=0;i<16;i++)
	{
		Bk5824_rf_ber[i] = bk5823_rf_ber[i];
	}
	for(i=0;i<14;i++)
	{
		Bk5824_rf_ber_17[i] = bk5823_rf_ber_17[i];
	}
	for(i=0;i<11;i++)
	{
		Bk5824_rf_ber_18[i] = bk5823_rf_ber_18[i];
	}

	BK5824RegConfig(Bk5824_rf_ber,Bk5824_rf_ber_17,Bk5824_rf_ber_18);    

	BK5824SpiRead(9, (unsigned char*)&reg_value);
	reg_value &= ~(1<<29);                              //hold = 0
	BK5824SpiWrite(9,(unsigned char*)&reg_value);


	BK5824SpiRead(15, (unsigned char*)&reg_value);
	reg_value |= (1<<0);                                // TRX RESET: "1"--"0"--"1"
	BK5824SpiWrite(15,(unsigned char*)&reg_value);
	delay_us(100);

	BK5824SpiRead(15, (unsigned char*)&reg_value);
	reg_value &= ~(1<<0);
	BK5824SpiWrite(15,(unsigned char*)&reg_value);
	delay_us(100);

	BK5824SpiRead(15, (unsigned char*)&reg_value);
	reg_value |= (1<<0);
	BK5824SpiWrite(15, (unsigned char*)&reg_value);

	delay_ms(800);                                      //waiting for a while

	BK5824SpiRead(9, (unsigned char*)&reg_value);
	reg_value |= (1<<29);                               // hold = 1
	BK5824SpiWrite(9, (unsigned char*)&reg_value);
	delay_us(100);

	BK5824SpiRead(20, (unsigned char*)&err);            //REG20 error
	printf("err = %d \r\n", err);

	BK5824SpiRead(19, (unsigned char*)&total);          //REG19 total
	printf("total = %d \r\n", total);

	if(total)
	{
		i=0;
		while((err< total)&& err)
		{
			err *=10;
			i++;
		} 
		ber = err/total;
		printf("BER= %d.00E-00%d \r\n", ber, i);
	}
	else
	{
		printf("total received number is 0 \r\n");
	}
}
/**************************************************
FunctionName:
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
void bk_test_single(void)
{
	unsigned int  reg_value = bk5823_rf_cw[5];
	unsigned int Bk5824_rf_cw[16];
	unsigned char Bk5824_rf_cw_17[14];
	unsigned char Bk5824_rf_cw_18[11];
	unsigned char i;
  for(i=0;i<16;i++)
	{
		Bk5824_rf_cw[i] = bk5823_rf_cw[i];
	}
	for(i=0;i<14;i++)
	{
		Bk5824_rf_cw_17[i] = bk5823_rf_cw_17[i];
	}
	for(i=0;i<11;i++)
	{
		Bk5824_rf_cw_18[i] = bk5823_rf_cw_18[i];
	}
	if(bk_config_flag[BK_5823_TX_RF_BASE]== 1)
	{
			Bk5824_rf_cw[1] = 0x760FF280;           //TX Carrier = 5.8G
	}
	else
	{
			Bk5824_rf_cw[1] = 0x760FF280;        //TX Carrier = 5.79G
	}
	Bk5824_rf_cw[9] &= ~(0x07<<1);              //PA power setting 
	Bk5824_rf_cw[9] |= (bk_config_flag[BK_5823_POWER_SETTING]<<1);
	BK5824RegConfig(Bk5824_rf_cw, Bk5824_rf_cw_17, Bk5824_rf_cw_18);
	reg_value  |= (1<<26);
	BK5824SpiWrite(5,(unsigned char*)&reg_value);
	delay_us(10);
	reg_value = bk5823_rf_cw[5];
	BK5824SpiWrite(5,(unsigned char*)&reg_value);
}
/**************************************************
FunctionName:
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
void bk_test_obu0Wakeup(void)
{
	unsigned int Bk5824_rf_obu[16];
	unsigned char Bk5824_rf_obu_17[14];
	unsigned char Bk5824_rf_obu_18[11];
	unsigned char i;
  	for(i=0;i<16;i++)
	{
		Bk5824_rf_obu[i] = bk5823_rf_obu[i];
	}
	for(i=0;i<14;i++)
	{
		Bk5824_rf_obu_17[i] = bk5823_rf_obu_17[i];
	}
	for(i=0;i<11;i++)
	{
		Bk5824_rf_obu_18[i] = bk5823_rf_obu_18[i];
	}
  	BK5824RegConfig(Bk5824_rf_obu, Bk5824_rf_obu_17, Bk5824_rf_obu_18);
	BK5824PowerUp();
	BK5824_wuCal_toggle();
	/* Power Down */
	BK5824Power(Disable);
	BK5824_SoftWakeup_toggle();
}
/**************************************************
FunctionName:
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
void bk_test_obu0TRx(void)
{
  unsigned int Bk5824_rf_obu[16];
	unsigned char Bk5824_rf_obu_17[14];
	unsigned char Bk5824_rf_obu_18[11];
	unsigned char i;
  for(i=0;i<16;i++)
	{
		Bk5824_rf_obu[i] = bk5823_rf_obuTRx[i];
	}
	for(i=0;i<14;i++)
	{
		Bk5824_rf_obu_17[i] = bk5823_rf_obuTRx_17[i];
	}
	for(i=0;i<11;i++)
	{
		Bk5824_rf_obu_18[i] = bk5823_rf_obuTRx_18[i];
	}
  BK5824RegConfig(Bk5824_rf_obu, Bk5824_rf_obu_17, Bk5824_rf_obu_18);
	BK5824PowerUp();
//	BK5824_wuCal_toggle();
//	/* Power Down */
//	BK5824Power(Disable);
//	BK5824_SoftWakeup_toggle();
}
/**************************************************
FunctionName:
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
void bk_test_obu(void)
{
  unsigned int Bk5823_rf_obu[16];
	unsigned char Bk5823_rf_obu_17[14];
	unsigned char Bk5823_rf_obu_18[11];
	unsigned char i;
  for(i=0;i<16;i++)
	{
		Bk5823_rf_obu[i] = bk5823_rf_obu[i];
	}
	for(i=0;i<14;i++)
	{
		Bk5823_rf_obu_17[i] = bk5823_rf_obu_17[i];
	}
	for(i=0;i<11;i++)
	{
		Bk5823_rf_obu_18[i] = bk5823_rf_obu_18[i];
	}
  BK5824RegConfig(Bk5823_rf_obu, Bk5823_rf_obu_17, Bk5823_rf_obu_18);
//	BK5824_wuCal_toggle();
//	
//	BK5824Power(Disable);
}

/**************************************************
FunctionName:
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
void bk5824_wk2_single(int state)
{
	unsigned int reg_value;
	unsigned int Bk5824_rf_obu_reg1,Bk5824_rf_obu_reg3;
	
	Bk5824_rf_obu_reg1 = bk5823_rf_obu[1];
	Bk5824_rf_obu_reg3 = bk5823_rf_obu[3];

	BK5824Power(1);
	delay_ms(2);

	if(state == Enable)
	{
		//REG3 =reg3 | 0xff000000; if no hold channal
		Bk5824_rf_obu_reg3 &= ~0xff000000;
		Bk5824_rf_obu_reg3 |= 0xfE000000;
		reg_value = Bk5824_rf_obu_reg3;
		BK5824SpiWrite(3, (unsigned char*)&reg_value);

		//reg1[7] =1		
		Bk5824_rf_obu_reg1 |=(1<<7);
		reg_value =Bk5824_rf_obu_reg1;
		BK5824SpiWrite(1, (unsigned char*)&reg_value);		
	}
	else
	{
		reg_value = Bk5824_rf_obu_reg1;
		BK5824SpiWrite(1, (unsigned char*)&reg_value);
		
		reg_value = Bk5824_rf_obu_reg3;
		BK5824SpiWrite(3, (unsigned char*)&reg_value);
		
		//reg[f] <3:2> =00
		BK5824SpiRead(15, (unsigned char*)&reg_value);
		reg_value &=(~ (3<<2));
		BK5824SpiWrite(15,(unsigned char*)&reg_value);
	}
}

/**************************************************
FunctionName:
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
void bk_test_rsu(void)
{
	unsigned int Bk5824_rf_rsu[16];
	unsigned char Bk5824_rf_rsu_17[14];
	unsigned char Bk5824_rf_rsu_18[11];
	unsigned char i;
  for(i=0;i<16;i++)
	{
		Bk5824_rf_rsu[i] = bk5823_rf_rsu[i];
	}
	for(i=0;i<14;i++)
	{
		Bk5824_rf_rsu_17[i] = bk5823_rf_rsu_17[i];
	}
	for(i=0;i<11;i++)
	{
		Bk5824_rf_rsu_18[i] = bk5823_rf_rsu_18[i];
	}
	BK5824RegConfig(Bk5824_rf_rsu, Bk5824_rf_rsu_17, Bk5824_rf_rsu_18);
}
/********************************************************
Function: BK5824 test example
Parameter: None
********************************************************/
void BK5863TestBK5824(void)
{
	unsigned char TestData[16];
	int         i, j;
    
	for(i = 0; i < 21; i ++)
	{ 
		if(i >= 9) 
		{
			for(j = 0; j < 16; j ++) 
			{
				TestData[j] = i*2 + j + 5;
			}
			delay_us(100);
			BK5824SpiWrite(i, TestData);
			
			printf("BK5823 SPI Write Data(Reg%0d): 0x",i);
		#if BK5824_REG_LEN_OPTION == BK5824_REG_LEN_OPTION_PREV
			for(j = 0; j < BK5824RegWriteLen[i]; j++)
		#else
			for(j = 0; j < get_bk5824_reg_write_len(i); j++)
		#endif
			{
				printf("%02x", TestData[j]);
			}
			printf("!\r\n");
		}
		delay_us(100);			
		BK5824SpiRead(i, TestData);
		printf("BK5823 SPI Read Data(Reg%0d): 0x",i);
	#if BK5824_REG_LEN_OPTION == BK5824_REG_LEN_OPTION_PREV
		for(j = 0; j < BK5824RegReadLen[i]; j++)
	#else
		for(j = 0; j < get_bk5824_reg_read_len(i); j++)
	#endif
		{
			printf("%02x", TestData[j]);
		}
		printf("!\r\n");
	}
}
#endif




