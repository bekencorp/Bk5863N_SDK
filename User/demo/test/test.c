#include "test.h"
#include "Includes.h"
#ifdef _TEST_DEMO_

#if MCU_NO_SLEEP_FUNC
char test_mcu_flag  = 0;
#endif
void BK5824_read_reg_test(void)
{
	unsigned int reg_value;
	BK5824SpiRead(9, (unsigned char*)&reg_value);
	printf("%x\n",reg_value);
	BK5824SpiRead(15, (unsigned char*)&reg_value);
	printf("RegF:%x\n",reg_value);
}

void BK5863n_gpiof0_set_high(void)
{
	GPIOConfig_T GIPOConfig;
	GIPOConfig.GPIO_PERI = GPIO_General_Function;  //普通功能
	GIPOConfig.GPIO_OE_N_INTERN = Enable;  //输出使能
	GIPOConfig.GPIO_PULLUP = Disable;//Enable;  //上拉使能
	GIPOConfig.GPIO_PULLDOWN = Disable;  //下拉使能
	GIPOConfig.GPIO_I_IE = Disable;//Enable;  //输入使能
	GIPOConfig.Int.isINT = Disable;    //配置中断
	GIPOConfig.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_3;

	GPIO_Config(GPIOF,&GIPOConfig);
	/* Set High Level */
	SetGPIOPin(GPIOF, GPIO_Pin_0|GPIO_Pin_3);

}

void BK5863n_gpiof0_set_float(void)
{
	GPIOConfig_T GIPOConfig;
	GIPOConfig.GPIO_PERI = GPIO_General_Function;  //普通功能
	GIPOConfig.GPIO_OE_N_INTERN = Disable;  //输出使能
	GIPOConfig.GPIO_PULLUP = Disable;//Enable;  //上拉使能
	GIPOConfig.GPIO_PULLDOWN = Disable;  //下拉使能
	GIPOConfig.GPIO_I_IE = Disable;//Enable;  //输入使能
	GIPOConfig.Int.isINT = Disable;    //配置中断
	GIPOConfig.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_3;
	GPIO_Config(GPIOF,&GIPOConfig);
}

void bk5863n_gpiof1_2_config(void)
{
	GPIOConfig_T GIPOConfig;
	GIPOConfig.GPIO_PERI = GPIO_General_Function;  //普通功能
	GIPOConfig.GPIO_OE_N_INTERN = Enable;  //输出使能
	GIPOConfig.GPIO_PULLUP = Disable;//Enable;  //上拉使能
	GIPOConfig.GPIO_PULLDOWN = Disable;  //下拉使能
	GIPOConfig.GPIO_I_IE = Disable;//Enable;  //输入使能
	GIPOConfig.Int.isINT = Disable;    //配置中断
	GIPOConfig.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_1;
	/* Config */
	GPIO_Config(GPIOF,&GIPOConfig);
}

void test_bk5824_config(int stat)
{
		BK5824Init();
		BK5824PowerUp();
		if(stat == AUTO_RX_ENABLE)
		{
			BK5824_0Wakeup(Enable);		
		}
		else
		{
			BK5824_0Wakeup(Disable);
		}
		BK5824_wuCal_toggle();

		BK5824Power(Enable);
		BK5824SendData("1234",1);
		delay_ms(10);
		BK5824Power(Disable);
}

int ready_to_sleep(int state)
{
		Bk5824_RX(Disable);
		BK5824Power(Disable);
		BK5824ClearIrq(0x07);
		if(state == AUTO_RX_ENABLE)
		{
			//BK5824_0Wakeup(Enable);
			BK5824_SoftWakeup_toggle();	
		}
		else
		{
		//	BK5824_0Wakeup(Disable);
//			BK5824_SoftWakeup_toggle();	
		}
		#if OUTPUT_DEBUG_INFO
		printf("ent slp\n\n");
		#endif
		delay_us(500);
		#if 0
			BK5863nSysSleep(_OnlyRFRouse); 
		#elif MCU_NO_SLEEP_FUNC
		 			
					if(BK5824Stat.BK5824WKOFlag != TRUE)
					{
							test_mcu_flag  = 1;
							return 1;
					}
		#else
			do{
			
			}while(BK5824Stat.BK5824WKOFlag != TRUE );
		#endif
		return 0;

}

void test_send_single(void)
{
	bk5824_set_tx_freq(FREQ_5800);
	bk5824_set_rf_power(15);
	bk5824_wk2_single(Enable);
	delay_ms(30);
	bk5824_wk2_single(Disable);
}
const char sendtable[]={"0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789"};
void test_send_data(void)
{
	unsigned int reg_value;
	int times = 5;
	bk5824_set_tx_freq(FREQ_5800);
	bk5824_set_rf_power(15);
	BK5824SendData((unsigned char*)sendtable,100);
	delay_ms(3);
	do
	{
	  	BK5824SpiRead(6, (unsigned char*)&reg_value);
	    if(reg_value & 0x01)
	    {
				
#if OUTPUT_DEBUG_INFO
				printf("sd irq\n");
#endif
				times = 0;
				BK5824ClearIrq(0x07);
				return;
	    }
	    else
	    {
	        times--;
			delay_ms(1);
	    }
	}while(times != 0);
	BK5824ClearIrq(0x07);
	
	#if OUTPUT_DEBUG_INFO
		printf("tx timeout \r\n");
		#endif
}

void test_set_high_set_float_gpiof0(void)
{
	BK5863n_gpiof0_set_high();
	BK5863n_gpiof0_set_float();
}

void hex_printf(char len,char *buf)
{
	#if OUTPUT_DEBUG_INFO		
	unsigned char i;
	
	for(i=0;i<(unsigned char)len;i++)
	{
		printf("%x ", buf[i]);
		if((i % 30) == 29) printf("\n");
	}
	printf("\n");
	#endif
}

int read_bk5824_status(int state)
{
	unsigned int reg_value; 
	unsigned char data[128] = {0};
	unsigned char Len;
	
		if(BK5824Stat.BK5824WKOFlag == TRUE )		
		{
				BK5824Stat.BK5824IRQFlag = FALSE;
				BK5824Stat.BK5824WKOFlag =FALSE;
				//收到数据

			    BK5824SpiRead(6, (unsigned char*)&reg_value);
			    if(reg_value & 0x02)
			    {
						BK5824SpiRead(5, (unsigned char*)&Len); 
						#if OUTPUT_DEBUG_INFO
							printf("RF RX length: %d\n", Len);
						#endif
						
						BK5824ReadData((unsigned char*)data,&Len);
						hex_printf(Len,(char*)data);
			    }
				 if(reg_value & 0x01)
				 {
						
						 #if OUTPUT_DEBUG_INFO
							 printf("Tx irq\n");
						#endif
				 }
			    if(reg_value & 0x04)
			    {
			        
						#if OUTPUT_DEBUG_INFO
							printf("Rx err\n");
						#endif
			    }
				
				BK5824ClearIrq(0x07);

				#if OUTPUT_DEBUG_INFO
					printf("sgl start\n");
				#endif
			
				//io light
				ResetGPIOPin(GPIOB,GPIO_Pin_0);
				ResetGPIOPin(GPIOF, GPIO_Pin_2|GPIO_Pin_1);		//
				test_send_single();
				SetGPIOPin(GPIOB,GPIO_Pin_0);
				SetGPIOPin(GPIOF, GPIO_Pin_2|GPIO_Pin_1);		//
				
				#if OUTPUT_DEBUG_INFO
					printf("sgl ext\r\n");
				#endif
				test_set_high_set_float_gpiof0();
				return 1;
		}
		return 0;
}

void		zero_wakeup_test_3_init(uint8 	status)
{
	//bk5824_wko_output_to_other_gpio(Enable);
	bk5863n_gpiof1_2_config();
	SetGPIOPin(GPIOF, GPIO_Pin_2|GPIO_Pin_1);				//金溢管脚输出LED1，LED2
	
	test_bk5824_config(status);
}

void		zero_wakeup_test_3(uint8	status)
{
	int state;
	if(BK5824Stat.BK5824WKOFlag != TRUE)
	{
		return;
	}
	#if MCU_NO_SLEEP_FUNC
		if( test_mcu_flag  == 1)
		{
			goto as1;
		}
	#endif
	
	do
	{
		state = ready_to_sleep(status);
		if(state)
		{
			return;
		}
#if MCU_NO_SLEEP_FUNC
as1:
#endif
		#if OUTPUT_DEBUG_INFO
			read_bk5824_reg6();
		#endif
		read_bk5824_status(status);
	}while(0);
}




#endif

