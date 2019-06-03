#include "main.h"
#include "Includes.h"
#include "ATE.h"
#include "test.h"
#include "ke_event.h"
#include "ke_app.h"
#include "app_esam_5121.h"

int main(void)
{
	BK5863SystemInit();
	TestConfig();
	SystemConfig();

	Test_5863N();
	bk_esam_app_test();

	while(1)
	{
		loop_call();
	#ifdef BK5863_CHIP_TEST
		application_uart_command_analysis();
		if(TestInfo.BK5824RecTest)
		{
	    	DSRCCheck_and_RecMeg();
		}
	#endif
	}
}

/**************************************************
FunctionName: Test_5863N
Describe:
InputParameter:	
OutputParameter:
Notice:
**************************************************/
void Test_5863N(void)
{
	LEDConfig();
	printf("ChipID = 0x%08x\r\n",ReadChipID());
	printf("DeviceID = 0x%08x\r\n",ReadDeviceID());
	printf("flash = 0x%x\r\n",flash_read_mID32());
	printf("    %s     %s\r\n\r\n",__DATE__,__TIME__);

	/* GPIOF5 Int Config */
	GPIOTestConfig(); 
	
	FlashLED();
	
	TestInfo.BK5824RecTest = 1;
	BK5824Init();
	
	LEDClose(0x03);
	printf("BK5863 Initial Finish Successfully!\r\n");
}

void test_mcu_sleep(void)
{
	bk_test_obu0Wakeup();
	BK5863nSysSleep(_OnlyRFRouse);
}

/***********************  END OF FILES  ***********************/
