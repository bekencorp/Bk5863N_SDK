#ifndef _SPI_H_
#define _SPI_H_

#include "Type.h"
#include "BK5863Com.h"

enum{
	SPI1 = 0,
	SPI2 = 1,
	SPI3 = 2,
	SPI4 = 3
};

enum{
	SPI_CKR1 = 1,
	SPI_CKR2 = 2,
	SPI_CKR3 = 3,
	SPI_CKR4 = 4,
	SPI_CKR5 = 5,
	SPI_CKR6 = 6,
	SPI_CKR7 = 7,
	SPI_CKR8 = 8,
	SPI_CKR9 = 9,
	SPI_CKR10 = 10,
	SPI_CKR11 = 11,
	SPI_CKR12= 12,
	SPI_CKR13 = 13,
	SPI_CKR14 = 14
	
	
	
};

enum{
	RxFIFO12ByteINT = 0,
	RxFIFO8ByteINT = 1,
	RxFIFO4ByteINT = 2,
	RxFIFO1ByteINT = 3
};
enum{
	TxFIFO1ByteINT = 0,
	TxFIFO4ByteINT = 1,
	TxFIFO8ByteINT = 2,
	TxFIFO12ByteINT = 3
};

typedef struct{
	unsigned char SPIEN;  //SPIʹ��
	unsigned char MSTEN;  //1����ʽ��0�ӷ�ʽ
	unsigned char CKPHA;
	unsigned char CKPOL;
	unsigned char BIT_WDTH; //����λ��
	unsigned char NSSMD; //��ѡ��ʽλ
	unsigned char SPI_CKR; //SPIʱ������
	unsigned char RxINT_EN;
	unsigned char TxINT_EN;
	unsigned char RxOVR_EN;
	unsigned char TXOVR_EN;
	unsigned char RXINT_mode; //�����жϲ�����ʽ
	unsigned char TxINT_MODE;
	unsigned char DelaySLK;  //�ȴ�
}SPIPara_T;


enum{
	_SPIDCOClock = 0,
	_SPIXTALClock,
};



void SPIConfig(unsigned char SPIx,SPIPara_T *Cfg);
void SPIClock(uint8_t SPIx,uint8_t cmd);
void SelectSPIClockSource(uint8_t SPIx,uint8_t source);



#endif



/***********************  END OF FILES  ***********************/
