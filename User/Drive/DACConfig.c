#include "DACConfig.h"
#include "Includes.h"

//DAC
BK_AUDIO_CTRL_BLK DAC_CTRL_BLK;

//********************************************************//
//Function: close BK5863 DAC Module
//Parameter: 
//          None
//********************************************************//
void BK5863DACClose(void)
{
	 REG_APB2_SDDAC_FIFOCONF &= ~0x00300000;  //disable intr
	 REG_APB2_SDDAC_ENABLE   = 0x00000000 ;
	 //Add user code to flush data buffer
	 memset( &DAC_CTRL_BLK, 0, sizeof(DAC_CTRL_BLK));
	 rb_init(&DAC_CTRL_BLK.aud_rb,(unsigned char *)DAC_CTRL_BLK.data_buff,AUDIO_BUFF_LEN);     
}

void rb_init(driver_ringbuff_t *rb, unsigned char *pbuff, unsigned short int len )
{
    memset( (unsigned char *)rb, 0, sizeof(driver_ringbuff_t));

    rb->buffp = pbuff;
    rb->buffer_len = len;

    return;
}
//********************************************************//
//Function: BK5863 DAC Initial
//Parameter: 
//          None
//********************************************************//
void BK5863DACInit(void)
{
	REG_APB3_ICU_DACCLKCON  = 0;	//DAC clock enable
	REG_APB3_ICU_INT_ENABLE |= INT_STATUS_DAC_bit;
	REG_APB3_ICU_INT_WAKENA |= INT_STATUS_DAC_bit;
	/* 32M clock, 16k sampling, CLKDIV =(32768K/128*16K)<<24 */
	REG_APB2_SDDAC_CLKDIVID = 0x08000000;	 
	REG_APB2_SDDAC_FIFOCONF = 0x00042108;
//    REG_APB2_SDDAC_TESTCONF = 0x00000010;
//    REG_APB2_SDDAC_TESTCONF |= (1<<7);          
	memset( &DAC_CTRL_BLK, 0, sizeof(DAC_CTRL_BLK));
	DAC_CTRL_BLK.volume = 0;   //AUDIO_VOLUME_MAX;
	rb_init( &DAC_CTRL_BLK.aud_rb, (unsigned char *)DAC_CTRL_BLK.data_buff, AUDIO_BUFF_LEN );
	REG_APB3_ICU_ANA1_CTRL &= ~(1<<AUDIO_SPIDACPWD);    //DAC power on
	REG_APB3_ICU_ANA1_CTRL &= ~( AUDIO_VOLUME_MAX << AUDIO_VOLUME_GAIN_SHIFT ); 
	#if 1
	REG_APB3_ICU_ANA1_CTRL |= AUDIO_VOLUME_MAX<<AUDIO_VOLUME_GAIN_SHIFT;
	#else
	REG_APB3_ICU_ANA1_CTRL |= (AUDIO_VOLUME_MAX-DAC_CTRL_BLK.volume)<< AUDIO_VOLUME_GAIN_SHIFT;
	#endif
	REG_APB3_ICU_ANA1_CTRL |= (3<<24);
}

//********************************************************//
//Function: Open BK5863 DAC Module
//Parameter: NONE
//********************************************************//
void BK5863DACOpen(void)
{
	if( DAC_CTRL_BLK.intr_open == 0 )
	{
		DAC_CTRL_BLK.intr_open = 1;
		REG_APB2_SDDAC_FIFOCONF |= 0x00300000;
		REG_APB2_SDDAC_ENABLE = 0x01;
	}
}
