#include "dsrc_intf.h"
#include "dsrc.h"
#include "BK5824D.h"
#include "esam_app.h"
#include "icc_app.h"
#include "icc_app.h"

void sdrc_msg_input(unsigned char *msg,unsigned short len)
{
	dsrc_msg_handler(msg,len);
}

void sdrc_msg_output(unsigned char *msg,unsigned short len)
{
	BK5824SendData(msg,len);
}


int sdrc_set_profile(unsigned char profile)
{

	return 0;
}

int sdrc_app_get_profile(char ops,char *msg,int len,int offs,char type)
{
	if(DSRC_VERSION_1 == ops){
		msg[offs] = 0;
		return 1;
	}
	else{
		return -1;
	}
	return 0;
}

void dsrc_app_notification_other_mode_run(char type)
{
	set_esam_app_event(ESAM_APP_OPEN_EVENT);
	set_icc_app_event(ICC_APP_OPEN_EVENT);
}

void dsrc_app_notification_other_mode_stop(char type)
{
	set_esam_app_event(ESAM_APP_CLOSE_EVENT);
	set_icc_app_event(ICC_APP_CLOSE_EVENT);
}


int dsrc_app_operation_esam(char ops,unsigned char *in,unsigned short len,
	unsigned char *out,unsigned short out_len)
{
	
	return -1;
}

int dsrc_app_operation_icc(char ops,unsigned char *in,unsigned short len,
	unsigned char *out,unsigned short out_len)
{
	
	return -1;
}


