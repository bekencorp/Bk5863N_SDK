#ifndef _MISC_H_
#define _MISC_H_

#undef FIGURE_VOLTAGE_1_7V
#undef FIGURE_VOLTAGE_1_8V
#undef FIGURE_VOLTAGE_1_9V
#undef FIGURE_VOLTAGE_2_0V

#define FIGURE_VOLTAGE_1_7V      (0)
#define FIGURE_VOLTAGE_1_8V      (1)
#define FIGURE_VOLTAGE_1_9V      (2)
#define FIGURE_VOLTAGE_2_0V      (3)


enum{
	INTcard_reader = 1,
	INTadc = (1<<1),
	INTdac = (1<<2),    
	INT3des  = (1<<3), 
	INTmodem_fifo = (1<<4), 
	INTgpio    = (1<<5), 
	INTuart1   = (1<<6), 
	INTuart2   = (1<<7), 
	INTspi1   = (1<<8), 
	INTspi2 = (1<<9), 
	INTspi3 = (1<<10), 
	INTspi4 = (1<<11), 
	INTi2c1 = (1<<12), 
	INTi2c2 = (1<<13), 
	INTpwm0 = (1<<14), 
	INTpwm1 = (1<<15), 
	INTpwm2 = (1<<16), 
	INTpwm3 = (1<<17), 
	INTpwm4 = (1<<18), 
	INTdetoutbo = (1<<19), 
	INTdetoutao = (1<<20)
};


void CmdIRQ(unsigned char cmd);
void CmdFIQ(unsigned char cmd);
void CmdINT(unsigned int Souce,unsigned char cmd);
void CmdCharge(unsigned char ICU0x30bit27);
void CmdOutVCC(unsigned char ICU0x30bit28);
unsigned int ReadChipID(void);
unsigned int ReadDeviceID(void);
void CLOSEAllIntEnableAndFlag(void);
void bk5824_wko_output_to_other_gpio(int stat);

void set_bk58xx_figure_voltage(unsigned char voltage);
int get_bk58xx_figure_voltage(void);


#endif









/***********************  END OF FILES  ***********************/
