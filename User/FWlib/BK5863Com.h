#ifndef _BK5863COM_H_
#define _BK5863COM_H_


typedef struct DataBuffer
{
    unsigned char *pBuf;
    unsigned int  WrPtr, RdPtr;
    unsigned int  VldDataNum;
    unsigned char LastPkt;
} DataBuff_t;

//BK5863SpiAPI
#define SPI_BUF_LEN     64

typedef struct SpiMessage
{
	unsigned char Role;
	unsigned char Nssmd;
	unsigned char Ckr;
	unsigned char Ckphapol;
	unsigned char WaitCLK;
	DataBuff_t    TxBuff;
	DataBuff_t    RxBuff;
}SpiMsg_t,SpiMsg_T;

//BK5863I2cAPI
#define I2C_BUF_LEN     64

typedef struct I2cMessage
{
	unsigned char Role;
	unsigned char LocalAddr;
	unsigned char RemoteAddr;
	unsigned char ClkDiv;
	unsigned char RwMode;
	int           RemainLen;
	DataBuff_t    TxBuff;
	DataBuff_t    RxBuff;
}I2cMsg_t,I2cMsg_T;




#endif /* _BK5863COM_H_ */
