#ifndef _BK5863_GPIO_H_
#define _BK5863_GPIO_H_

#include "type.h"

enum{
	GPIO_General_Function,
	GPIO_Special_Function,
};

enum{
	edge_Trigging = 1,
	level_Tringging = 0,
};
/* GPIOx */
enum{
 GPIOA		=			0,
 GPIOB		=			1,
 GPIOC		=			2,
 GPIOD		=			3,
 GPIOE		=			4,
 GPIOF		=			5
};

enum{
	 GPIO0	=				0,
	 GPIO1	=				1,
	 GPIO2	=				2,
	 GPIO3	=				3,
	 GPIO4	=				4,
	 GPIO5	=				5
};
/* GPIO_Pinx */
enum{
	GPIO_Pin_0 = 0x01,
	GPIO_Pin_1 = 0x02,
	GPIO_Pin_2 = 0x04,
	GPIO_Pin_3 = 0x08,
	GPIO_Pin_4 = 0x10,
	GPIO_Pin_5 = 0x20,
	GPIO_Pin_6 = 0x40,
	GPIO_Pin_7 = 0x80,
};
/*单沿触发，双沿触发 */
enum{
	_DPULSE = 1,
	_SPULSE = 0,
};
/* 高低电平触发 */
enum{
	Low_level_Trigging = 0,
	High_level_Tringging =1,
};
/* 上升沿触发，下降沿触发 */
enum{
	Rising_edge_Trigging = 0,
	falling_edge_Tringging = 1,  //下降沿
};

typedef struct{
	uint8_t isINT;  //是否配置中断,若为FlASE，则不对中断进行配置(保持原值)
	uint8_t En_Dis_able;  //是否使能某个GPIDO中断
	uint8_t INTMode;  //电平触发、边沿触发  中断方式
	uint8_t dpulse;  //单沿触发 或 双沿触发
	uint8_t INTtrigger;  //触发方式 
}GPIOINT_T;

typedef struct{
	uint8_t GPIO_Pin;
	uint8_t GPIO_PERI;
	uint8_t GPIO_OE_N_INTERN;
	uint8_t GPIO_PULLUP;
	uint8_t GPIO_PULLDOWN;
	uint8_t GPIO_I_IE;
	GPIOINT_T Int;
}GPIOConfig_T;

/* GPIO */
#define GPIO_BFD_BS_NORMAL			0
#define GPIO_BFD_BL_NORMAL			8
#define GPIO_BFD_BS_OE_N			8
#define GPIO_BFD_BL_OE_N			8
#define GPIO_BFD_BS_PU				16
#define GPIO_BFD_BL_PU				8
#define GPIO_BFD_BS_PD				24
#define GPIO_BFD_BL_PD				8

#define GPIO_BFD_BS_OD				0
#define GPIO_BFD_BL_OD				8
#define GPIO_BFD_BS_ID				8
#define GPIO_BFD_BL_ID				8
#define GPIO_BFD_BS_IE				16
#define GPIO_BFD_BL_IE				8


/* GPIO Operation */
//#define	BASE_GPIO_PTR			((HW_GPIO*)0x00a07000)
#define NUM_GPIO_PORT			5
#define NUM_GPIO_PORT			5
#define	ADDR_GPIO_PTR	    0x00a07000
#define	APB_GPIO_BASE			ADDR_GPIO_PTR		
#define BK5863_GPIO_CONFIG(gpio)		(*((volatile unsigned long *)	(APB_GPIO_BASE+8*(gpio)) ))
#define BK5863_GPIO_DATA(gpio)			(*((volatile unsigned long *)	(APB_GPIO_BASE+8*(gpio)+4) ))

#define GPIO_SetSF(pn,sfl)	\
	/*功能:指定端口设置成特殊功能脚*/\
	/*pn:端口号;sfl:将要设为特殊功能的管脚列表*/ \
	{ \
	BK5863_GPIO_CONFIG(pn) &= (_BFD(sfl,GPIO_BFD_BS_NORMAL,GPIO_BFD_BL_NORMAL)&\
		_BFD(sfl,GPIO_BFD_BS_PU,GPIO_BFD_BL_PU)& \
		_BFD(sfl,GPIO_BFD_BS_PD,GPIO_BFD_BL_PD)); \
	}
	
#define GPIO_InputSetup(pn,i,pullup,pulldw) \
	/*功能:对指定端口进行输入特性设定*/\
	/*pn:端口号;i、pu、pd:将要设为输入、使能上拉、使能下拉的管脚列表*/\
	{\
	BK5863_GPIO_CONFIG(pn) |= (BFD(i,GPIO_BFD_BS_OE_N,GPIO_BFD_BL_OE_N)| \
										BFD(i,GPIO_BFD_BS_NORMAL,GPIO_BFD_BL_NORMAL)| \
										BFD(pullup,GPIO_BFD_BS_PU,GPIO_BFD_BL_PU)| \
										BFD(pulldw,GPIO_BFD_BS_PD,GPIO_BFD_BL_PD)); \
	BK5863_GPIO_DATA(pn) |= (BFD(i,GPIO_BFD_BS_IE,GPIO_BFD_BL_IE)); \
	}




void GPIOFwlibInit(void);

/* GPIO Config */
void GPIO_Config(uint8_t GPIOx,GPIOConfig_T *GPIOConfig);

/* INT */
void SetGPIOIntCmd(int cmd);
void SetGPIO_INTConfig(uint8_t GPIOx,GPIOConfig_T *GPIOConfig);
void SetGPIOIRQ(uint8_t GPIOx,uint8_t GPIO_Pin,uint8_t cmd);
void SetGPIOINTPulseMode(uint8_t GPIOx,uint8_t GPIO_Pin,uint8_t mode);
void SetGPIOINTPulse(uint8_t GPIOx,uint8_t GPIO_Pin,uint8_t mode);
void SetGPIOINTMode(uint8_t GPIOx,uint8_t GPIO_Pin,uint8_t mode);
void SetGPIOINTLevel(uint8_t GPIOx,uint8_t GPIO_Pin,uint8_t mode);
void SetGPIOInt(uint8_t cmd);
/* Read INT */
uint8_t ReadGPIO_IRQflag(uint8_t GPIOx,uint8_t GPIO_Pin);


/* GPIO输出状态 */
void ResetGPIOPin(uint8_t GPIOx,uint8_t Pin);
void SetGPIOPin(uint8_t GPIOx,uint8_t Pin);
void SetGPIO(uint8_t GPIOx,uint8_t Value);

/* 读GPIO状态 */
uint8_t ReadGPIO_Data(uint8_t GPIOx);
uint8_t ReadGPIO_Pin_Data(uint8_t GPIOx,uint8_t GPIO_Pin);

/* GPIO 中断唤醒 */
void SetGPIOINT_WAKENA(uint8_t Cmd);

/* GPIO Second Function */
void SetGPIOSecondFunction(uint8_t GPIOx,uint8_t GPIO_Pin,uint8_t cmd);


/*******************************************************************
		Record Trig mode & Pulse & Dpulse 
*******************************************************************/
extern uint32_t GPIO_INT_TRIG_MODE[2];
extern uint32_t GPIO_INT_PULSE[2];
extern uint32_t GPIO_INT_TRIG_dpulse[2];





#endif /* _BK5863_GPIO_H_ */






/***********************  END OF FILES  ***********************/
