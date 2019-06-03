#ifndef _BK5824Config_H_
#define _BK5824Config_H_

#include "BK5863.h"
#include "BK5824RegMem.h"


#define REG_XVER_PN9_SETTING    9
#define REG_XVER_TRX_RESET      15
#define REG_XVER_BER_ERR_COUNT  19
#define REG_XVER_BER_BIT_COUNT  20
#define BK_CONFIG_FLAG_MAX      20
#define BK_5823_POWER_SETTING   0
#define BK_5823_TX_RF_BASE      1
#define BK_5823_14K_WAKE_NUM    2
#define BK_SOC_SLEEP_MODE            3

struct BK5824SysFlag{
	unsigned char BK5824IRQOpenInt : 1;
	unsigned char BK5824WKOOpenInt : 1;
	unsigned char BK5824IRQFlag : 1;
	unsigned char BK5824WKOFlag : 1;
	unsigned char BKIRQIntSta : 1;
	unsigned char BKWKOIntSta : 1;
	unsigned char tx_busy : 1;
};


void BK5824Config(void);
void BK5824SPIConfig(void);
void BK5863SPI1Config(void);
void BK5824RegConfig(uint32_t* reg_0_15,uint8_t* reg_17,uint8_t* reg_18);
void Bk5823GPIOConfig(void);

void BK5824Init(void);

void DSRCCheck_and_RecMeg(void);

void read_bk5824_reg6(void);

extern unsigned char  bk_config_flag[BK_CONFIG_FLAG_MAX];
extern struct BK5824SysFlag BK5824Stat;

#endif
