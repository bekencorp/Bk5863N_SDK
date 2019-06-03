#ifndef _DACConfig_H_
#define _DACConfig_H_


#include "BK5863.h"
#include "BKDACTest.h"
//BK5863 DAC
#define AUDIO_BUFF_LEN					64//3072
#define AUDIO_EMPTY_COUNT_THRE			20

#define AUDIO_VOLUME_MUTE				-1
#define AUDIO_VOLUME_MAX				7
#define AUDIO_VOLUME_GAIN_SHIFT			22
#define AUDIO_SPIDACPWD                 26
#define AUDIO_VOLUME_MUTE_MASK			0x01c00000
#define AUDIO_VOLUME_MUTE_CONF			0x30000000

typedef struct _driver_ringbuff_s
{
    unsigned short int buffer_len;
    unsigned short int wptr;
    unsigned short int rptr;
    unsigned char *buffp;
}driver_ringbuff_t;


typedef struct BK_AUDIO_CTRL_BLK_s
{
    unsigned char  data_buff[AUDIO_BUFF_LEN];
    driver_ringbuff_t aud_rb;
    unsigned int bufLen;
    unsigned int intr_open;
    int empty_count;
    int volume;
}BK_AUDIO_CTRL_BLK;


void BK5863DACClose(void);
void rb_init( driver_ringbuff_t *rb, unsigned char *pbuff, unsigned short int len );
void BK5863DACInit(void);
void BK5863DACOpen(void);


extern BK_AUDIO_CTRL_BLK DAC_CTRL_BLK;

#endif /*_DACConfig_H_*/
