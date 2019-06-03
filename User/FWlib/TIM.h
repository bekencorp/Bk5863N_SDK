#ifndef _TIM_H_
#define _TIM_H_

#include "type.h"




#define ClockSelect_PT3_4_32KHZ 0
#define ClockSelect_PT3_4_32MHZ 1

#define BK_PWM_MODE				0
#define BK_TIMER_MODE			1
enum{
	_TIMER_MODE = 1,
	_PWM_MODE	= 0
};
enum{
	_TIM4 = 4,
	_TIM3 = 3,
	_TIM2 = 2,
	_TIM1 = 1,
	_TIM0 = 0
};

typedef struct{
	unsigned char MODE;//BK_TIMER_MODE BK_PWM_MODE
	unsigned char CLK_DIV;  //时钟分频
	unsigned char PT3_4_SelClk; //Select Clock
//	unsigned char PT3_4_CLK32M_DIV;
	uint16_t STcounter;  //结束值
	uint16_t MIDcounter;  //中间值
	
	uint8_t Int_En;
	uint8_t IntWake_En;
}TIMPara_T;





void CmdTIMCLK(unsigned char cmd);
void ClockSelect_PT3_4(unsigned char CLK);
void CleanTimInterruptFlag(unsigned int TimIntFlag);


void PT_ENABLE(unsigned char cmd);
void PTx_MODE(uint8_t TIMx,uint8_t mode);
void PTx_MID(uint8_t TIMx,uint16_t midCout);
void PTx_START(uint8_t TIMx,uint16_t StartCout);

char TimConfig(uint8_t TIMx,TIMPara_T *TimConf);


unsigned int PTxCountResult(uint8_t TIMx);
void PTxDefault(uint8_t TIMx);

		/* RTC */
void SetRTC_EN(unsigned char Cmd);
unsigned char ReadRTC_EN(void);
void CleanRTCInterruptFlag(unsigned int RTCIntFlag);
uint8_t ReadRTCInterruptFlag(void);
void SetRTC_CNT_VALUE(unsigned char Value);
unsigned char ReadRTC_CNT_VALUE(void);


#endif




/***********************  END OF FILES  ***********************/
