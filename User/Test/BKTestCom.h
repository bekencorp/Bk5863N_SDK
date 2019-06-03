#ifndef _BKTestCom_H_
#define _BKTestCom_H_


#define BK5863_CHIP_TEST
#define BEKEN_UART_MODULE_TEST
#define BEKEN_SPI_MODULE_TEST
#define BEKEN_I2C_MODULE_TEST
#define BEKEN_STANDBY_CURRENT_TEST
#define BEKEN_CARD_READER_MODULE_TEST
#define BEKEN_DES_MODULE_TEST
#define BEKEN_ADC_MODULE_TEST
#define BEKEN_PWM_MODULE_TEST
#define BEKEN_DAC_MODULE_TEST
#define BEKEN_BK5824_MODULE_TEST
#define BEKEN_BK5863_V2_TEST

#define MODULE_CMD_READ_CARD                    0x0101
#define MODULE_CMD_WRITE_CARD                   0x0102
#define MODULE_CMD_ENTER_STANDBY                0x0103
#define MODULE_CMD_FLASH_SECTOR                 0x0104
#define MODULE_CMD_SPI_TEST                     0x0105
#define MODULE_CMD_I2C_TEST                     0x0106
#define MODULE_CMD_ADC_TEST                     0x0107
#define MODULE_CMD_DAC_TEST                     0x0108
#define MODULE_CMD_PWM_TEST                     0x0109
#define MODULE_CMD_MFC_TEST                     0x010a
#define MODULE_CMD_DES_TEST                     0x010b
#define MODULE_CMD_BER_TEST                     0x010c
#define MODULE_CMD_PN9_TEST                     0x010d
#define MODULE_CMD_SINGLE_TEST                  0x010e
#define MODULE_CMD_OBU_TEST                     0x010f
#define MODULE_CMD_RSU_TEST                     0x0110
#define MODULE_CMD_5823_SPI_TEST                0x0111
#define MODULE_CMD_ARM_WRITE_FLASH_PROTECT_TEST 0x0112
#define MODULE_CMD_VOLT_DETECT_TEST             0x0113
#define MODULE_CMD_VDDSWITCH_TEST               0x0116
#define MODULE_CMD_0WAKEUP_TEST                 0x0117
#define MODULE_CMD_MCU_CLK_TEST                 0x0120


#define MODULE_CMD_ESAM_TEST                    0x0118
#define MODULE_CMD_ESAM_TEST_STEP1              0x0119
#define MODULE_CMD_ESAM_TEST_STEP2              0x011A
#define MODULE_CMD_ESAM_TEST_STEP3              0x011B


#define MODULE_CMD_TEST_5824_SEND_DATA          0x011C


#define MODULE_CMD_GPIOB7XHClk_TEST              0x0123
#define MODULE_CMD_GPIOF5IntTest_TEST            0x0124
#define MODULE_CMD_PWMOUT_TEST                   0x0125
#define MODULE_CMD_LBD_TEST                      0x0126
#define MODULE_CMD_CRCCalculate_TEST             0x0127
#define MODULE_CMD_BK5824RecIntTest_TEST         0x0128
#define MODULE_CMD_DeepSleep_TEST                0x0129
#define MODULE_CMD_RFRouse_Sleep_TEST            0x0130
#define MODULE_CMD_ADC_Temp_TEST                 0x0131


///#define DEBUG_OPEN

#define ST_G ( 0x3F << 10)
#define TCI_BEKEN_HARDWARE_TEST                 (0x00e0 + ST_G)
#define HCI_COMMAND_COMPLETE_EVENT              0x0E
enum
{
    BEKEN_UART_LINK_CHECK=0x00,
    BEKEN_UART_REGISTER_WRITE_CMD=0x01,
    BEKEN_UART_REGISTER_CONINUOUSE_WRITE_CMD = 0x02,
    BEKEN_UART_REGISTER_READ_CMD = 0x03,
    BEKEN_UART_BT_START_CMD,
    BEKEN_UART_BT_STOP_CMD,
    BEKEN_UART_PATCH_CMD,   

    BEKEN_UART_MODULE_TEST_CMD=0x80,
    BEKEN_UART_MODULE_SUB_TEST_CMD,
    BEKEN_UART_MODULE_GENERAL_CMD    
};

enum
{
    TRA_HCIT_COMMAND = 1,
    TRA_HCIT_ACLDATA = 2,
    TRA_HCIT_SCODATA = 3,
    TRA_HCIT_EVENT   = 4,       
    TRA_HCIT_SREADTRUM  = 0xc0,       

    TRA_HCIT_BEKEN_UART_COMMAND = 0x1B
};

#endif
