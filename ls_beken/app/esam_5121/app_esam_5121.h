#ifndef _APP_ESAM_5121_H_
#define _APP_ESAM_5121_H_

#include "stdio.h"
#include "string.h"

#ifndef NULL
#define NULL                        0UL
#endif

#ifndef INLINE
///#define INLINE __inline
#define INLINE static inline
#endif


#define APP_ESAM_ERR_LOG_CFG              1
#define APP_ESAM_LOG_DEFAULT_GRADE        1
#define APP_ESAM_RESET_TIME_LOG_CFG       0


#define BK_ESAM_APP_TEST_BUF_DEPTH    260





#if APP_ESAM_ERR_LOG_CFG
#define APP_ESAM_LOG(grade,str)    if(grade)printf("[a_esam]%s\r\n",str)
#else
#define APP_ESAM_LOG(grade,str)
#endif

#if APP_ESAM_LOG_DEFAULT_GRADE
INLINE void APP_ESAM_LOG_ARRAY(char grade,char *str,int len, char *arr)
{ 
	int index; 
	if(grade)
	{
		printf("[a_esam]%s",str); 
		if(len)
		{
			printf("(%d) = 0x",(len)); 
			for(index = 0;index < (len);index++) 
			{  
				printf("%02x",(unsigned char)arr[index]);  
			} 
		}
		printf("\r\n"); 
	}	
}
#else
#define APP_ESAM_LOG_ARRAY(grade,str,len,arr,index)
#endif

#define APP_ESAM_CHECKT_RESPONSE(res,str,array)  	if(res >= 0)  \
													{  \
														APP_ESAM_LOG_ARRAY(APP_ESAM_LOG_DEFAULT_GRADE,str,res,(char*)array);  \
													}   \
													else  \
													{  \
														APP_ESAM_LOG(APP_ESAM_LOG_DEFAULT_GRADE,str" FAILED");   \
													}


int bk_esam_reset(void);
int bk_esam_select_file(unsigned int fid,unsigned char *resdat,unsigned short llen);
int bk_esam_transfer_operation(char *cmd_dat,unsigned short len,char *out,int out_len);

void bk_esam_app_test(void);

#endif

