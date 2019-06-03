#include "TDes.h"
#include "BK5863.h"

/**************************************************
FunctionName:TDESClk
Describe:
InputParameter:
OutputParameter:
Notice:
**************************************************/
void TDESClk(uint8_t cmd)
{
	if(cmd)
		REG_APB3_ICU_DESCLKCON  = 0;	//3DES clock enable
	else
   	REG_APB3_ICU_DESCLKCON  |= 0x01;	
}

/**************************************************
FunctionName:TDESReset
Describe: Reset all the DES/3DES module when write 1 into it.
InputParameter:
OutputParameter:
Notice:
**************************************************/
void TDESReset(void)
{
	REG_APB4_DES_CTL |= 1<<DES_MODULE_RST_BIT; 		//reset
}

/**************************************************
FunctionName:TDESSetKey
Describe: 
InputParameter:
OutputParameter:
Notice:
**************************************************/
void TDESSetKey(uint8_t *Key1,uint8_t *Key2,uint8_t *Key3)
{
	unsigned char i;
	for(i=0; i<_DES_KEY_LEN; i++)
	{
		REG_APB4_DES_KEY1 = Key1[i];
		REG_APB4_DES_KEY2 = Key2[i];
		REG_APB4_DES_KEY3 = Key3[i];
	}	
}

/**************************************************
FunctionName:TDESIntCmd
Describe: 
InputParameter:
OutputParameter:
Notice:
**************************************************/
void TDESIntCmd(unsigned char Cmd)
{
	if(Cmd)
	  REG_APB3_ICU_INT_ENABLE |= INT_STATUS_DES_bit;
	else
		REG_APB3_ICU_INT_ENABLE &= (~(INT_STATUS_DES_bit));
}

/**************************************************
FunctionName:TDESIntWakeupCmd
Describe: 
InputParameter:
OutputParameter:
Notice:
**************************************************/
void TDESIntWakeupCmd(unsigned char Cmd)
{
	if(Cmd)
	  REG_APB3_ICU_INT_WAKENA |= INT_STATUS_DES_bit;
	else
		REG_APB3_ICU_INT_WAKENA &= (~(INT_STATUS_DES_bit));
}

/**************************************************
FunctionName:TDESOperation
Describe: 
InputParameter:DES_DECRYPTION			1
							 DES_ENCRYPTION			0
OutputParameter:
Notice:
**************************************************/
void SelectDecEncryption(unsigned int operation)
{
	if(operation == DES_ENCRYPTION)
		REG_APB4_DES_CTL &= ~(1<<DES_ENC_BIT);		//encode
	else
		REG_APB4_DES_CTL |= 1<<DES_ENC_BIT;			//decode
}
/**************************************************
FunctionName:TDESConfig
Describe: 
InputParameter:
OutputParameter:
Notice:
**************************************************/
void TDESConfig(TDEConfigPara_T *conf)
{
	unsigned char i;
	unsigned int temp;
	if(conf->DESCLKCON)
		REG_APB3_ICU_DESCLKCON  = 0;	//3DES clock enable
	else
   	REG_APB3_ICU_DESCLKCON  |= 0x01;	//3DES clock enable
	/*  DES_RST */
	if(conf->DoDES_RST)
	{
		REG_APB4_DES_CTL = 1<<DES_MODULE_RST_BIT; 		//reset
		delay_us(10);
	}
	
	temp = 0;
	if(conf->MAC)
		temp |= (1<<4);
	if(conf->DEC_ENC)
	  temp |= (1<<5);
	if(conf->DES_TDES)
		temp |= (1<<6);
	if(conf->DES_EN)
		temp |= (1<<7);
	
	REG_APB4_DES_CTL = temp;
	
	if(conf->INT_ENABLE)
	  REG_APB3_ICU_INT_ENABLE |= INT_STATUS_DES_bit;
	else
		REG_APB3_ICU_INT_ENABLE &= (~(INT_STATUS_DES_bit));
	if(conf->INT_WAKENA)
	  REG_APB3_ICU_INT_WAKENA |= INT_STATUS_DES_bit;
	else
		REG_APB3_ICU_INT_WAKENA &= (~(INT_STATUS_DES_bit));
	
	for(i=0; i<_DES_KEY_LEN; i++)
	{
		if(conf->Key1)
			REG_APB4_DES_KEY1 = conf->Key1[i];
		if(conf->Key2)
			REG_APB4_DES_KEY2 = conf->Key2[i];
		if(conf->Key3)
			REG_APB4_DES_KEY3 = conf->Key3[i];
	}	
}
/**************************************************
FunctionName:InputDESData
Describe: 
InputParameter:
OutputParameter:
Notice:
**************************************************/
uint8_t InputDESData(uint8_t *Indat,uint8_t length)
{
	unsigned int i;
	if(length%DES_FRAME_LEN)
	  return FALSE;
	
  for(i=0;i<DES_FRAME_LEN;i++)
    REG_APB4_DES_IN = Indat[i];
  return TRUE;	
}

/**************************************************
FunctionName:TDESADDR_RST
Describe: 
InputParameter:
OutputParameter:
Notice:
**************************************************/
void TDESADDR_RST(void)
{
	REG_APB4_DES_CTL |= 1<<DES_ADDR_RST_BIT;
}



/***********************  END OF FILES  ***********************/
