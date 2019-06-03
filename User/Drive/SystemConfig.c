#include "SystemConfig.h"
#include "Includes.h"
#include "isoiec7816_intf.h"
//#include "fake_timer.h"
#include "fake_clock.h"
#include "dsrc.h"
#include "dsrc_app.h"
#include "icc_app.h"

/**************************************************
FunctionName: 
Describe: 
InputParameter:	
OutputParameter:
Notice:
**************************************************/
void SystemConfig(void)
{
	/* 初始化动态内存 */
	sys_mem_init(); 
	/* UART Config */
	#if 0
	  BK5863UartInit(BK5863_UART1, 115200, Uart1RxBuff, sizeof(Uart1RxBuff));
	#else
	  UART1Config(115200,Uart1RxBuff,sizeof(Uart1RxBuff));
	#endif
	flash_init();
	/* BK5824Init */
	///BK5824Config();
	BK5824Init();
	Timer3TestConfig();
}

void system_timer_task(void)
{
	isoiec7816_receive_timerout_handler();
	fclk_hdl(1);
	dsrc_timer_handler(1);
	dsrc_app_timer_handler(1);
	icc_app_timer_handler(1);
}










/***********************  END OF FILES  ***********************/
