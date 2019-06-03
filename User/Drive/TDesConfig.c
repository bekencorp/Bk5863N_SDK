#include "TDesConfig.h"
#include "Includes.h"

BKDES_CONTROL_STRU  BKDES_CTL_BLK;

const unsigned char bk_des_input[32]= {0x5A, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
								0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
								0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
								0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37};
//const unsigned char bk_des_output[32];
								
//DES
unsigned char		des_key1_set[DES_KEY_LEN] = {0x33,0xF5,0x29,0xE5,0x3E,0x51,0x74,0x3B};
unsigned char		des_key2_set[DES_KEY_LEN] = {0x8A,0x94,0x73,0x71,0x4e,0x8a,0xa1,0xb1};
unsigned char		des_key3_set[DES_KEY_LEN] = {0x33,0xF5,0x29,0xE5,0x3E,0x51,0x74,0x3B};

/********************************************************
Description: BEKEN BK5863 DES example code
********************************************************/
void bk_test_des(void)
{
	int i;
	unsigned char des_input[32];
	unsigned char des_output[32];

	memcpy(des_input,bk_des_input,32);
//	BK5863DESInit(DES_3_KEY_MODE, DES_MAC_ECB);	
	BK5863NTDESInit();
	BK5863DESOperation(des_input, 32, des_output, DES_ENCRYPTION);

	printf("Encryption:");
	for(i=0;i<32; i++)
	{
		if(!(i%8))
			printf("\r\n");
		printf("%02x",des_output[i]);
	}
	printf("\r\n");

	memset(des_input, 0, 32);
	BK5863DESOperation(des_output, 32, des_input, DES_DECRYPTION);

	printf("Decryption:");
	for(i=0; i<32; i++)
	{
		if(!(i%8))
			printf("\r\n");
		printf("%02x",des_input[i]);
	}
	printf("\r\n");
	
}

/********************************************************
Function: BK5863 DES initial
Parameter: 
          des_mode:
              DES_1_KEY_MODE: use KEY1 to calculate DES
				DES_3_KEY_MODE: use KEY1 KEY2 KEY3 to calculate 3DES

          mac_mode:
				DES_MAC_ECB: ECB mode.Encrypt the 64 bits input only
				DES_MAC_CBC: CBC mode. The result of the previous encryption 
					         operation is XOR’ed with incoming data
********************************************************/
void BK5863DESInit(int des_mode, int mac_mode)
{
	int i;
	TDESClk(Enable);  //3DES clock enable
	REG_APB4_DES_CTL = 1<<DES_MODULE_RST_BIT; 		//reset
	delay_us(10);
	REG_APB4_DES_CTL = (1<<DES_EN_BIT)|(des_mode<<DES_TDES_BIT)|(mac_mode<<DES_MAC_BIT); 	//3DES enable,	
	REG_APB3_ICU_INT_ENABLE |= INT_STATUS_DES_bit;
	REG_APB3_ICU_INT_WAKENA |= INT_STATUS_DES_bit;
	
	for(i=0; i<DES_KEY_LEN; i++)
	{
		REG_APB4_DES_KEY1 = des_key1_set[i];
		REG_APB4_DES_KEY2 = des_key2_set[i];
		REG_APB4_DES_KEY3 = des_key3_set[i];
	}	
}

/**************************************************
FunctionName:BK5863NTDESInit
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
void BK5863NTDESInit(void)
{
	TDEConfigPara_T DesConf;
	
	DesConf.DESCLKCON = Enable;
	DesConf.DoDES_RST = Enable;
	DesConf.DES_EN = Enable;
	DesConf.DES_TDES = DES_3_KEY_MODE;  //DES_3_KEY_MODE Or DES_1_KEY_MODE
	/*
	   0: ECB mode.  Encrypt the 64 bits input only
     1: CBC mode. The result of the previous encryption
		 operation is XOR'd with incoming data.
	*/
	DesConf.MAC = DES_MAC_ECB; 
	DesConf.DEC_ENC = DES_ENCRYPTION; //加密
	
	DesConf.INT_ENABLE = Enable;
	DesConf.INT_WAKENA = Enable;
	DesConf.Key1 = des_key1_set;
	DesConf.Key2 = des_key2_set;
	DesConf.Key3 = des_key3_set;
	
  TDESConfig(&DesConf);
}
/********************************************************
Function: BK5863 DES Encode/Decode operation
Parameter: 
          input:  the pointer of input data buffer

          length: the number of data

          output: the pointer of output data buffer

          operation: 
				DES_DECRYPTION:  DES descryption
				DES_ENCRYPTION:  DES encryption
********************************************************/
int BK5863DESOperation(uint8_t* input,uint32_t length,uint8_t* output,uint32_t operation)
{
	if(length%DES_FRAME_LEN)
	  return FALSE;					//error

	BKDES_CTL_BLK.pInput = input;
	BKDES_CTL_BLK.pOutput = output;
	BKDES_CTL_BLK.bufLen = length;
	BKDES_CTL_BLK.outIndex = 0;	
	/* ADDR_RST */
	TDESADDR_RST();
  /* 选择加解密 */
	SelectDecEncryption(operation);

	/* 写入数据 */
	BKDES_CTL_BLK.inIndex = DES_FRAME_LEN;
	InputDESData(input,DES_FRAME_LEN);
//	BKDES_CTL_BLK.inIndex = DES_FRAME_LEN;
  /* 等待处理完成 */
	while(BKDES_CTL_BLK.outIndex >= length)
	{
		delay_us(10);
	}

	return TRUE;
}

















