#ifndef _BK5863NReg_H_
#define _BK5863NReg_H_

/* WDT Reg Addr */
#define  REG_WDT_CTRL   (*((volatile unsigned long *)  0x00a06000))

/* FLASH */
#define FLASH_BASIC_ADDR 0x00b00000
#define reg_FLASH_OPERATE_SW 			(*((volatile unsigned long *)  (FLASH_BASIC_ADDR + 0 * 4)))
#define reg_FLASH_DATA_SW_FLASH 	    (*((volatile unsigned long *)  (FLASH_BASIC_ADDR + 1 * 4)))
#define reg_FLASH_DATA_FLASH_SW 	    (*((volatile unsigned long *)  (FLASH_BASIC_ADDR + 2 * 4)))	
#define REG_FLASH_Reserved1    	        (*((volatile unsigned long *)  (FLASH_BASIC_ADDR + 3 * 4)))
#define reg_FLASH_RDID_DATA_FLASH       (*((volatile unsigned long *)  (FLASH_BASIC_ADDR + 4 * 4)))
#define reg_FLASH_SR_DATA_CRC_CNT       (*((volatile unsigned long *)  (FLASH_BASIC_ADDR + 5 * 4)))
#define REG_FLASH_Reserved2             (*((volatile unsigned long *)  (FLASH_BASIC_ADDR + 6 * 4)))
#define reg_FLASH_CONF 					(*((volatile unsigned long *)  (FLASH_BASIC_ADDR + 7 * 4)))

/* CRC */
#define CRC_BASIC_ADDR 0x00800000
#define REG_CRC_STARTE       (*((volatile unsigned long *)   0x00800000))
#define REG_CRC_DATA_IN      (*((volatile unsigned long *)   (CRC_BASIC_ADDR+1*4)))
#define REG_CRC_CRC_IN       (*((volatile unsigned long *)   (CRC_BASIC_ADDR+2*4)))
#define REG_CRC_CRC0_OUT     (*((volatile unsigned long *)   (CRC_BASIC_ADDR+3*4)))
#define REG_CRC_CRC1_OUT     (*((volatile unsigned long *)   (CRC_BASIC_ADDR+4*4)))

//BK5863N DAC  
#define DAC_BASIC_ADDR  0x00a02000
#define REG_APB2_DAC_CLKDIVID   (*((volatile unsigned long *)  (DAC_BASIC_ADDR + 0 *4)))
#define REG_APB2_DAC_FIFOCONF   (*((volatile unsigned long *)  (DAC_BASIC_ADDR + 1 *4)))
#define REG_APB2_DAC_TESTCONF   (*((volatile unsigned long *)  (DAC_BASIC_ADDR + 2 *4)))
#define REG_APB2_DAC_ENABLE     (*((volatile unsigned long *)  (DAC_BASIC_ADDR + 3 *4)))
#define REG_APB2_DAC_DACRFIFO   (*((volatile unsigned long *)  (DAC_BASIC_ADDR + 8 *4)))
#define REG_APB2_DAC_DACLFIFO   (*((volatile unsigned long *)  (DAC_BASIC_ADDR + 9 *4)))
#define REG_APB2_DAC_ADCRFIFO   (*((volatile unsigned long *)  (DAC_BASIC_ADDR + 10 *4)))
#define REG_APB2_DAC_ADCLFIFO   (*((volatile unsigned long *)  (DAC_BASIC_ADDR + 11 *4)))
#define REG_APB2_DAC_FIFOSTAT   (*((volatile unsigned long *)  (DAC_BASIC_ADDR + 12 *4)))
//Calibration
#define Calib_BASIC_ADDR 0x00a12000
#define REG_Start_ANA_Calib     (*((volatile unsigned long *)  (Calib_BASIC_ADDR + 0 *4)))
#define REG_Calib_DONE 			(*((volatile unsigned long *)  (Calib_BASIC_ADDR + 1 *4)))
#define REG_Calib_Conf_L 		(*((volatile unsigned long *)  (Calib_BASIC_ADDR + 2 *4)))
#define REG_Calib_Conf_H  	    (*((volatile unsigned long *)  (Calib_BASIC_ADDR + 3 *4)))
#define REG_Calib_Conf    	    (*((volatile unsigned long *)  (Calib_BASIC_ADDR + 4 *4)))
#define REG_Calib_Coef6   	    (*((volatile unsigned long *)  (Calib_BASIC_ADDR + 5 *4)))
#define REG_Calib_Coef7   	    (*((volatile unsigned long *)  (Calib_BASIC_ADDR + 6 *4)))


#endif
