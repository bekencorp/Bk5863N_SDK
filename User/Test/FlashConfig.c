#include "FlashConfig.h"
#include "Includes.h"
#include "stdlib.h"

/*********************************************************
Flash Sector Test 
*********************************************************/
void BK5863FlashSectorTest(unsigned char SectorMode)
{
	unsigned int  i,StartAddr = 0;
	unsigned char Length = 32,WriteData[100];
	
//	FLASHInit();
	flash_init();
	printf("ReadAddr = %x\r\n", StartAddr);
	BK5863MfcOperate(FLASH_OPCODE_READ,Length,StartAddr,WriteData);
 
	printf("ReadData(%d) = 0x",Length);
	for(i=0;i<Length;i++)
	{
		printf("%02x",WriteData[i]);
	}
	printf("\r\n");
	#if 1
		StartAddr = 0x40000;
//		StartAddr = 0x6fff0; //Ð´±£»¤µØÖ· 
//		StartAddr = 0x70000;  //C-->
		printf("ReadAddr = %x\r\n", StartAddr);
		BK5863MfcOperate(FLASH_OPCODE_READ,Length,StartAddr,WriteData);
	 
		printf("ReadData(%d) = 0x",Length);
		for(i=0;i<Length;i++)
		{
			printf("%02x",WriteData[i]);
		}
		printf("\r\n");
		
		flash_erase_sector(StartAddr);
		printf("erase!ReadAddr = %x\r\n", StartAddr);
		BK5863MfcOperate(FLASH_OPCODE_READ,Length,StartAddr,WriteData);
		printf("ReadData(%d) = 0x",Length);
		for(i=0;i<Length;i++)
		{
			printf("%02x",WriteData[i]);
		}
		printf("\r\n");

		printf("WriteAddr = %x\r\n", StartAddr);
		printf("WriteData(%d) = 0x",Length);
		for(i=0;i<Length;i++)
		{
			WriteData[i] = i + rand()&0xFF;
			printf("%02x",WriteData[i]);
		}
		printf("\r\n");
		flash_write_data(WriteData,StartAddr,Length);
		memset(WriteData,0,Length);
	#else
		StartAddr = 0x000;		
	#endif
		
	printf("ReadAddr = %x\r\n", StartAddr);
	BK5863MfcOperate(FLASH_OPCODE_READ,Length,StartAddr,WriteData);
 
	printf("ReadData(%d) = 0x",Length);
	for(i=0;i<Length;i++)
	{
		printf("%02x",WriteData[i]);
	}
	printf("\r\n");
}

/********************************************************
Function: BK5863 Wakeup From Standby Mode
Parameter: 
          Type:
              0x0: Read;  0x1: Write;  0x2: Sector Earse;  0x3: Block Earse;  0x7: Chip Earse;
              Other: reserved
          Space: 
              0x0: Main Space;  0x1: NVR Space
          Addr: Read/Write address
          pData: Read/Write data pointer
********************************************************/
void BK5863MfcOperate(uint8_t Type,uint8_t ReadLength,unsigned int Addr,unsigned char *pData)
{
	if(Type == FLASH_OPCODE_READ)  //Read Flash
	{
		flash_read_data(pData,Addr,ReadLength);
	}
}

void FLASHInit(void)
{
//	FLASHPara_T conf;
//	
//	conf.CRC_EN = 0;
//	conf.FlashClkConf = FLASH_CLK_32MHz;
//	conf.ModeSel = FLASH_LINE_1;
//	conf.FwrenFlashCPU = 1;
//	
//	FlashCONFIG(&conf);
}




