#include "app_esam_5121.h"
//#include "fake_clock.h"
#include "sl_esam.h"
#include "stdio.h"
#include "string.h"
#include "SysTime.h"
#include "mmgmt.h"
#include "type.h"

int bk_esam_reset(void)
{
	int result;
	char log[40];
#if APP_ESAM_RESET_TIME_LOG_CFG
	unsigned int startime,usetime;
	startime = fclk_get_tick();
#endif
	result = esam_hardware_open(0,log,40);
#if APP_ESAM_RESET_TIME_LOG_CFG
	usetime = fclk_get_tick() - startime;
	printf("esam reset time:%d mS\r\n",usetime);
#endif
	if( result != -1)
	{
		APP_ESAM_LOG_ARRAY(APP_ESAM_LOG_DEFAULT_GRADE,"RESET",result,(char*)log);
	}
	else
	{
		APP_ESAM_LOG(APP_ESAM_LOG_DEFAULT_GRADE,"RESET FAILED");
		return -1;
	}
	
	return 0;
}

int bk_esam_select_file(unsigned int fid,unsigned char *resdat,unsigned short llen)
{
	return esam_select_file(fid,resdat,llen);
}


int bk_esam_transfer_operation(char *cmd_dat,unsigned short len,char *out,int out_len)
{
	return esam_hardware_operation(0,cmd_dat,len,out,out_len);
}

////
int bk_esam_read_df01_ef04(unsigned char *outbuf,int len)
{
	unsigned char temp[50];
	int ret;
	ret = esam_select_file(0xDF01,temp,sizeof(temp));
	if(ret == ESAM_ERR_OPS_FLAG)
	{
		return -1;
	}
	ret = esam_select_file(0xEF04,temp,sizeof(temp));
	if(ret == ESAM_ERR_OPS_FLAG)
	{
		return -1;
	}
	ret = esam_read_binary_no_mac(0,0,len,(char*)outbuf,len);
	if(ret == ESAM_ERR_OPS_FLAG)
	{
		return -1;
	}
	return 0;
}

int bk_esam_write_df01_ef04(unsigned char *in,int len,unsigned char *output,int out_len)
{
	unsigned char temp[50];
	int ret;
	ret = esam_select_file(0xDF01,temp,sizeof(temp));
	if(ret == ESAM_ERR_OPS_FLAG)
	{
		return -1;
	}
	ret = esam_select_file(0xEF04,temp,sizeof(temp));
	if(ret == ESAM_ERR_OPS_FLAG)
	{
		return -1;
	}
	
	ret = esam_update_binary_no_mac(0,0,len&0xFF,(char*)in,(char*)output,out_len);
	if(ret == ESAM_ERR_OPS_FLAG)
	{
		return -1;
	}
	return 0;
}



void bk_esam_app_test(void)
{
	int rst;
	
	char *log = mmgmt_malloc(BK_ESAM_APP_TEST_BUF_DEPTH);
	if(!log){
		APP_ESAM_LOG(APP_ESAM_LOG_DEFAULT_GRADE,"bk_esam_app_test malloc FAILED");
		return;
	}
	
	bk_esam_reset();
//	rst = esam_get_random(8,(unsigned char*)log);
//	APP_ESAM_CHECKT_RESPONSE(rst,"get random8",log);
//	delay_ms(1000);
//	/*************************** get 8 Bytes Random **************************************/
//	log[0] = 0x00;log[1] = 0x84;log[2] = 0;log[3] = 0;log[4] = 0x04;
//	rst = bk_esam_transfer_operation(log,5,log,BK_ESAM_APP_TEST_BUF_DEPTH);
//	APP_ESAM_CHECKT_RESPONSE(rst,"get random4",log);
	/*************************** MF **************************************/
	rst = bk_esam_select_file(0x3f00,(unsigned char *)log,BK_ESAM_APP_TEST_BUF_DEPTH);
	APP_ESAM_CHECKT_RESPONSE(rst,"Sel MF",log);
	
	///rst = bk_esam_read_df01_ef03((unsigned char*)log,BK_ESAM_APP_TEST_BUF_DEPTH);
	rst = bk_esam_read_df01_ef04((unsigned char*)log,50);
	APP_ESAM_CHECKT_RESPONSE(rst,"Read DF01-EF04",log);

	memset(log,0xAA,50);
	rst = bk_esam_write_df01_ef04((unsigned char*)log,50,(unsigned char*)log,BK_ESAM_APP_TEST_BUF_DEPTH);
	APP_ESAM_CHECKT_RESPONSE(rst,"Write DF01-EF04",log);

	rst = bk_esam_read_df01_ef04((unsigned char*)log,50);
	APP_ESAM_CHECKT_RESPONSE(rst,"Read DF01-EF04",log);

	memset(log,0x55,50);
	rst = bk_esam_write_df01_ef04((unsigned char*)log,50,(unsigned char*)log,BK_ESAM_APP_TEST_BUF_DEPTH);
	APP_ESAM_CHECKT_RESPONSE(rst,"Write DF01-EF04",log);

	rst = bk_esam_read_df01_ef04((unsigned char*)log,50);
	APP_ESAM_CHECKT_RESPONSE(rst,"Read DF01-EF04",log);
	if(log){
		mmgmt_free(log);
	}
}

int bk_rsu_obu_m_step1(void)
{
	return bk_esam_reset();
}

int bk_rsu_obu_m_step2(unsigned char *msg,int len)
{
	int rst = 0;
	if(get_esam_app_wk_state() == 0){
		return 0;
	}
	
	char *log = mmgmt_malloc(BK_ESAM_APP_TEST_BUF_DEPTH);
	if(!log){
		APP_ESAM_LOG(APP_ESAM_LOG_DEFAULT_GRADE,"bk_esam_app_test malloc FAILED");
		return 0;
	}

	/*************************** MF **************************************/
	rst = bk_esam_select_file(0x3f00,(unsigned char *)log,BK_ESAM_APP_TEST_BUF_DEPTH);
	APP_ESAM_CHECKT_RESPONSE(rst,"Sel MF",log);

	rst = bk_esam_read_df01_ef04((unsigned char*)log,50);
	APP_ESAM_CHECKT_RESPONSE(rst,"Read DF01-EF04",log);
	if(rst != 0){
		rst = 0;
		goto ext;
	}

	rst = MIN(len,50);
	memcpy(msg,log,rst);
ext:
	if(log){
		mmgmt_free(log);
	}
	
	return rst;
}

int bk_rsu_obu_m_step3(void)
{
	int rst = 0;
		
	esam_hardware_close(0,0,0);

	return rst;
}


