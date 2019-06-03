#include "CardConfig.h"
#include "Includes.h"

/**********************************************************************
    初始化读卡器
		初始化完成后可通过PcdPowerDown和PcdPowerOn函数来打开和关闭磁场;
**********************************************************************/
void  BK5863CardReaderInit(void)
{ 
	REG_APB3_ICU_CDRDERCLKCON   = 0x0;              //open clock
	REG_APB3_ICU_ANA2_CTRL      = 0x6371ee01;// 0x927860F1;       //power on card reader  0x31
	REG_APB3_ICU_ANA3_CTRL      = 0xC180221d;  //0x32
	REG_APB3_ICU_ANA4_CTRL      = 0x20d422a0;//0x082842C0;     //0x33
	//GPIOD3 IC_RST (output pin)
	// REG_APB7_GPIOD_CFG &= ~(0x01<<11);
	// REG_APB7_GPIOD_DATA &= ~((0x01<11)|(0x01<<19));
	// REG_APB3_ICU_INT_ENABLE     |= INT_STATUS_CDRDER_bit;
	PcdConfigISOType( 'A' );
//	PcdPowerOn();

	//读卡电压调节REG_APB3_ICU_ANA2_CTRL 
	//0x31[28:26]	crtxldosel<2:0>		R/W	carder reader tx output selection.0:1.8V. 0.2/step
	//0x6371ee01   =0档
	//0x6771ee01   =1档
	//0x6b71ee01   =2档
	//0x6f71ee01   =3档
	//0x7371ee01   =4档
	//0x7771ee01   =5档
	//0x7b71ee01   =6档
	//0x7f71ee01   =7档
	


}










