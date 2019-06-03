#ifndef _BK5863_it_H_
#define _BK5863_it_H_






void BK5863NCardReaderIntService(void);

void BK5863nUart1IntService(void);
void BK5863nUart2IntService(void);

void BK5863nI2c1IntService(void);
void BK5863nI2c2IntService(void);

void BK5863nSpi1IntService(void);
void BK5863nSpi2IntService(void);
void BK5863nSpi3IntService(void);
void BK5863nSpi4IntService(void);


void GPIOTest_IRQHandler(void);
void GPIOF5_IRQHandler(void);
void GPIOB5_IRQHandler(void);
void GPIOB6_IRQHandler(void);
void GPIOB7_IRQHandler(void);
 /* GPIOC */
void GPIOC_Pin4_IRQHandle(void);
void GPIOC_Pin5_IRQHandle(void);



void BK5863nDESIntService(void);
void BK5863nADCIntService(void);
void BK5863nDACIntService(void);

void BK5863nPWM0IntService(void);
void BK5863nPWM1IntService(void);
void BK5863nPWM2IntService(void);
void BK5863nPWM3IntService(void);
void BK5863nPWM4IntService(void);

void BK5863nMFIFOIntService(void);

void BK5863nDetoutaoIntService(void);
void BK5863nDetoutboIntService(void);

void BK5863nLBDLowVoltageIntService(void);
void BK5863nLBDHighVoltageIntService(void);

#endif
