#ifndef _SL_ESAM_H_
#define _SL_ESAM_H_

#include "stdio.h"
#include "string.h"

#define ESAM_ERR_LOG_CFG                                  0
#define ESAM_LOG_OUT_DEFAULT_GRADE                        1
#define ESAM_GET_ALL_CHIP_RETURN_MSG_CFG                  0

#define ESAM_ERR_OPS_FLAG                                (-1)     ////鎵ц閿欒
#define ESAM_ARG_ERR_FLAG                                (-1)     ////鎵ц閿欒

#ifndef NULL
#define NULL                        0UL
#endif

#ifndef INLINE
///#define INLINE __inline
#define INLINE static inline
#endif

#ifndef MIN
#define MIN(x,y)  ((x) < (y) ? (x) : (y))
#endif /* ifndef MIN */

#ifndef MAX
#define MAX(x,y)  ((x) > (y) ? (x) : (y))
#endif /* ifndef MIN */		

#if ESAM_LOG_OUT_DEFAULT_GRADE
INLINE void ESAM_LOG_ARRAY(char grade,char *str,int len, char *arr) 
{
	int index;
	if(grade)
	{  
		printf("[esam]%s",str); 
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
#define ESAM_LOG_ARRAY(grade,str,len,arr)
#endif

#if ESAM_ERR_LOG_CFG
#define ESAM_LOG(grade,str)    if(grade)printf("[esam]%s\r\n",str)
#else
#define ESAM_LOG(grade,str)
#endif


#if ESAM_GET_ALL_CHIP_RETURN_MSG_CFG
#define ESAM_SUCCEEND_RETURN(array,ack_len,out_array,limit)	do{ \
																if((out_array)&&(limit)&&(ack_len)) \
																{  \
																	limit = MIN((limit),(ack_len)); \
																	if(limit)memcpy(out_array,array,limit); \
																	return limit; \
																} \
																return 0;   \
															}while(0)

#else
#define ESAM_SUCCEEND_RETURN(array,ack_len,out_array,limit)	do{ \
																if((out_array)&&(limit)&&((ack_len)-2)) \
																{  \
																	limit = MIN((limit),((ack_len) - 2)); \
																	if(limit)memcpy(out_array,array,limit); \
																	return limit; \
																} \
																return 0;   \
															}while(0)
#endif

#define ESAM_STATUS_RETURN(array,ack_len,out_array,limit)	do{\
																if(out_array&&limit&&ack_len)\
																{  \
																	limit = MIN(limit,ack_len);\
																	if(limit)memcpy(out_array,array,limit);\
																	return limit;\
																}\
																return 0;\
															}while(0)


#define ESAM_RX_MODE_CLEAR_RXFIFO              0x01   //清空UART已收到的数据
#define ESAM_RX_MODE_RETAIN_RXFIFO             0x00   //保留UART已收到的数据


#define ESAM_ISO7816_DEFAULT_RATE             (264258) ///(22021)///(264258)
#define ESAM_RESET_DEFAULT_LENGTH             (19)

#define ESAM_RX_MODE_CLEAR_RXFIFO              0x01   //清空UART已收到的数据
#define ESAM_RX_MODE_RETAIN_RXFIFO             0x00   //保留UART已收到的数据

#define  ESAM_SW1SW2_LENGTH                    0x02  //状态字长度
#define  ESAM_MAX_RETRY_TIMES                  0x04

#define  ESAM_INS_OFFSET                      0x01
#define  ESAM_P1_OFFSET                       0x02
#define  ESAM_P2_OFFSET                       0x03
#define  ESAM_LC_OFFSET                       0x04
#define  ESAM_CMD_HEAD_SIZE                   0x05


typedef enum{
	ESAM_CTL_GET_RD8 = 0,
	ESAM_CTL_GET_RD4,
	ESAM_CTL_GET_RD16,
	ESAM_CTL_SEL_FIL,
	
}_esam_ctl_t;



void esam_hardware_init(void);
void esam_hardware_deinit(void);
int esam_hardware_open(int ops,char *buf,int len);
int esam_hardware_close(int ops,char *buf,int len);
int esam_hardware_operation(int ops,char *sd,unsigned short sd_len,char *out,int out_len);
int esam_hardware_control(_esam_ctl_t ops,void *arg);

int esam_get_random(unsigned char read_len,unsigned char *rd);
int esam_select_file(unsigned int fid,unsigned char *resdat,unsigned short llen);
int esam_get_sn(char *resdat,unsigned short llen);
int esam_decrease_counter(char *resdat,unsigned short llen);


int esam_get_response(char le,unsigned char *resdat,unsigned short llen);
int esam_read_data(char p1,char p2,char *dat,char*resdat,unsigned short llen);
int esam_read_binary_no_mac(char p1,char p2,unsigned char ReadLen,char*resdat,unsigned short llen);
int esam_read_binary_mac(char p1,char p2,char *mac,char *key,char*resdat,unsigned short llen);

int esam_update_binary_no_mac(char p1,char p2,char lc,
	char *dat,char*resdat,unsigned short llen);
int esam_update_binary_mac(char p1,char p2,char lc,
	char *dat,char*resdat,unsigned short llen);
int esam_update_record_no_mac(char p1,char p2,char lc,
	char *dat,char*resdat,unsigned short llen);
int esam_update_record_mac(char p1,char p2,char lc,
	char *dat,char*resdat,unsigned short llen);
int esam_update_key(char p2,char lc,char *dat,
	char*resdat,unsigned short llen);
int esam_internal_authentication(char type,char p2,
	char*dat,char *out,unsigned short limit_len);
int esam_external_authentication(char type,char p2,
	char*dat,char *out,unsigned short limit_len);
void esam_des_calculate(char In[8], char key[8], 
	char Type,char out[8]);
int esam_des_calculate_mac(unsigned char *pin, unsigned short length, 
	unsigned char initv[8],char key[16], unsigned char mac[4]);
int esam_read_record(char p1,char p2,char le,
	char *key,char*resdat,unsigned short llen);
int esam_read_record_mac(char p1,char p2,char *mac,
	char *key,char*resdat,unsigned short llen);

#endif

/***********************  END OF FILES  ***********************/

