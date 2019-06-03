#ifndef _WDT_H_
#define _WDT_H_





void BK5863WDT(unsigned int  counter);
void BK5863WDTClose(void);
void BK5863WDTCLK(int cmd);

void BK5863WDT_PERIOD(unsigned int counter);
void BK5863WDTLaunch(int cmd);



#endif


/***********************  END OF FILES  ***********************/
