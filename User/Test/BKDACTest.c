#include "BKDACTest.h"
#include "Includes.h"

unsigned short AUD_SIN_32KFS[32] = {0x0 	  , 
                         0x18f8 ,
                         0x30fb ,
                         0x471c ,
                         0x5a82 ,
                         0x6a6d ,
                         0x7641 ,
                         0x7d8a ,
                         0x7fff ,
                         0x7d8a ,
                         0x7641 ,
                         0x6a6d ,
                         0x5a82 ,
                         0x471c ,
                         0x30fb ,
                         0x18f8 ,
                         0x0 		,
                         0xe707 ,
                         0xcf04 ,
                         0xb8e3 ,
                         0xa57d ,
                         0x9592 ,
                         0x89be ,
                         0x8275 ,
                         0x8000 ,
                         0x8275 ,
                         0x89be ,
                         0x9592 ,
                         0xa57d ,
                         0xb8e3 ,
                         0xcf04 ,
                         0xe707 };
unsigned short AUD_COMMOD[32]={0};
int g_dac_index = 0;
unsigned short* dac_pointer;
unsigned int dac_data_len;
void bk_test_dac(unsigned short* input, unsigned int len)
{
//    int times=0;
    BK5863DACInit();
  	dac_pointer = input;
	  dac_data_len = len/2;

    BK5863DACOpen();
    return;
}

void BK5863nDACIntService(void )
{
    unsigned int sts = REG_APB2_SDDAC_FIFOSTAT;
    unsigned short int *ptr= (unsigned short *)dac_pointer;
    //unsigned short int data;
    if(sts&FIFO_DACL_INT_bit)   //left channel
    {
      while(!(sts & FIFO_DACL_FULL_bit))
      {
				REG_APB2_SDDAC_DACLFIFO = ptr[g_dac_index]; //*ptr;
				g_dac_index = (g_dac_index+1)%dac_data_len;
				sts = REG_APB2_SDDAC_FIFOSTAT;
      }
    }

    sts = REG_APB2_SDDAC_FIFOSTAT;
    if( sts & FIFO_DACL_EMPTY_bit)
    {
       DAC_CTRL_BLK.empty_count++;
       if( DAC_CTRL_BLK.empty_count >= AUDIO_EMPTY_COUNT_THRE )
       {
          REG_APB2_SDDAC_FIFOCONF &= ~0x00300000; 
          DAC_CTRL_BLK.intr_open = 0;
          DAC_CTRL_BLK.empty_count = 0;
       }
       //add user code
    }
    else
    {
        DAC_CTRL_BLK.empty_count = 0;
    }
    
}







