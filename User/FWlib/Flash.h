#ifndef _FLASH_H_
#define _FLASH_H_

#include "Type.h"

#define FLASH_8MBIT 1

enum {
	FLASH_OPCODE_WREN    = 1,
	FLASH_OPCODE_WRDI    = 2,
	FLASH_OPCODE_RDSR    = 3,
	FLASH_OPCODE_WRSR    = 4,
	FLASH_OPCODE_READ    = 5,
	FLASH_OPCODE_RDSR2   = 6,
	FLASH_OPCODE_WRSR2   = 7,
	FLASH_OPCODE_PP      = 12,
	FLASH_OPCODE_SE      = 13,
	FLASH_OPCODE_BE1     = 14,
	FLASH_OPCODE_BE2     = 15,
	FLASH_OPCODE_CE      = 16,
	FLASH_OPCODE_DP      = 17,
	FLASH_OPCODE_RFDP    = 18,
	FLASH_OPCODE_RDID    = 20,
	FLASH_OPCODE_HPM     = 22,
	FLASH_OPCODE_CRMR    = 22,
	FLASH_OPCODE_CRMR2   = 23,
};

typedef enum {
    FLASH_LINE_1 = 0,
    FLASH_LINE_2,
    FLASH_LINE_4
} FLASH_LINE_MODE;

typedef enum{
    VOL_INFO_BT,
    VOL_INFO_HFP,
    VOL_INFO_MUSIC,
    VOL_INFO_LINEIN,
    SONGIDX_INFO,
    CUR_STATION_INFO,
    ALL_STATION_INFO
} flash_info_t;

typedef enum
{
    FLASH_PROTECT_NONE = 0,
    FLASH_PROTECT_ALL = 1,
    FLASH_PROTECT_HALF = 2,
    FLASH_UNPROTECT_LAST_BLOCK = 3
} PROTECT_TYPE;

#define CFG_DEFAULT_FLASH_PROTECT_MODE        (FLASH_PROTECT_ALL)
#define CFG_DEFAULT_FLASH_LINE_OPEN           1
#define CFG_REDUCE_FLASH_WR_SR                0     ////for GD flash
//FLASH
#define BIT_ADDRESS_SW                 0
#define BIT_OP_TYPE_SW                 24
#define BIT_OP_SW                      29
#define BIT_WP_VALUE                   30
#define BIT_BUSY_SW                    31

#define BIT_FLASH_CLK_CONF             0
#define BIT_MODE_SEL                   4
#define BIT_FWREN_FLASH_CPU            9
#define BIT_WRSR_DATA                  10
#define BIT_CRC_EN                     26

#define SET_FLASH_CLK_CONF             0xFU      << BIT_FLASH_CLK_CONF
#define SET_MODE_SEL                   0x1FU     << BIT_MODE_SEL
#define SET_FWREN_FLASH_CPU            0x1U      << BIT_FWREN_FLASH_CPU
#define SET_WRSR_DATA                  0xFFFFU   << BIT_WRSR_DATA
#define SET_CRC_EN                     0x1U      << BIT_CRC_EN

#define SET_ADDRESS_SW                 0xFFFFFFU << BIT_ADDRESS_SW
#define SET_OP_TYPE_SW                 0x1FU     << BIT_OP_TYPE_SW
#define SET_OP_SW                      0x1U      << BIT_OP_SW
#define SET_WP_VALUE                   0x1U      << BIT_WP_VALUE
#define SET_BUSY_SW                    0x1U      << BIT_BUSY_SW

#define FLASH_DCO_CLK_32MHz     8
#define FLASH_CLK_32MHz         0
#define FLASH_CLK_16MHz         1
#define FLASH_CLK_8MHz          2
#define FLASH_CLK_4MHz          3
#define FLASH_CLK_64MHz         8



#define FLASH_CLK_SEL   FLASH_CLK_32MHz   //FLASH_CLK_78mHz

#if CFG_DEFAULT_FLASH_PROTECT_MODE == FLASH_PROTECT_HALF
#elif CFG_DEFAULT_FLASH_PROTECT_MODE == FLASH_PROTECT_ALL
#elif CFG_DEFAULT_FLASH_PROTECT_MODE == FLASH_UNPROTECT_LAST_BLOCK
#elif CFG_DEFAULT_FLASH_PROTECT_MODE == FLASH_PROTECT_NONE
#else
	#error
#endif

typedef struct{
	unsigned char FlashClkConf;
	unsigned char ModeSel;
	unsigned char FwrenFlashCPU;
//	unsigned short int WrsrData;
	unsigned char CRC_EN;
}FLASHPara_T;


void FlashCONFIG(FLASHPara_T *par);

void FlashOperationAddress(unsigned int addr);
unsigned int ReadFlashOperationAddress(void);


unsigned char ReadFlashStatusBusyStatus(void);


unsigned char ReadSrDataFlash(void);
unsigned char ReadCrcErrCounter(void);
unsigned char ReadDataFlashSwSel(void);
unsigned char ReadDataSwFlashSel(void);
unsigned int ReadFlashCONF(void);


void set_flash_clk(unsigned char clk_conf);
void set_flash_qe(void);


static void set_flash_qwfr(void);
void clr_flash_qwfr(void);

void set_flash_protect(uint8 all) ;
void flash_erase_sector(uint32 address);
void flash_set_line_mode(FLASH_LINE_MODE mode) ;
uint32 flash_read_sr(void);
uint32 flash_read_sr2(void);
uint32 get_flash_rdsr(unsigned char size);
void set_flash_wrsr(unsigned char sr_width,  unsigned short val);
void set_flash_ctrl_config(void);
uint8 flash_read_mID(void);
uint32 flash_read_mID32(void);
void flash_write_enable(void);
void flash_write_disable(void);
void flash_init(void);
void flash_read_data (uint8 *buffer, uint32 address, uint32 len);
void flash_write_data (uint8 *buffer, uint32 address, uint32 len);
void flash_crc_remove(uint32 address);

//uint8 app_read_info(flash_info_t type,void * ptr1,void *ptr2,void *ptr3);
//void app_write_info(flash_info_t type,void * ptr1,void *ptr2,void *ptr3);

void save_volume_task(void *arg);
void app_backup_volume(flash_info_t vol_type);
void app_restore_volume(flash_info_t vol_type);




#endif


/***********************  END OF FILES  ***********************/
