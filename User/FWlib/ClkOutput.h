#ifndef _ClkOutput_H_
#define _ClkOutput_H_

#include "Type.h"

/* GPIO Config */
#define M_XH_CLKOUT_B7_INIT  REG_APB7_GPIOB_CFG &= ~((int32u)BIT(7)\
|((int32u)BIT(7) << 8)\
|((int32u)BIT(7) << 16)|((int32u)BIT(7) << 24))
#define M_XH_CLKOUT_B7_EN	 REG_APB3_ICU_GPIOB7CLKCON |= B7_XH_CLKOUT_EN

#define B7_XH_CLKOUT_EN		0x08			//GPIOB7 Clock output enable

#define B7_XH_DIV_32M			0x00			//GPIOB7 Clock output divice
#define B7_XH_DIV_16M			0x01
#define B7_XH_DIV_8M			0x02
#define B7_XH_DIV_4M			0x03
#define B7_XH_DIV_2M			0x04
#define B7_XH_DIV_1M			0x05
#define B7_XH_DIV_500K		0x06
#define B7_XH_DIV_250K		0x07




void ClockOutputinit(uint8_t Clk,uint8_t cmd);
void SetClockOutputEnable(int cmd);
void SetXHClock(uint8_t Clk);

#endif

/***********************  END OF FILES  ***********************/
