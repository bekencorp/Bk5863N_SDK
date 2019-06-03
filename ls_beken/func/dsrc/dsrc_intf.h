#ifndef _DSRC_INTF_H_
#define _DSRC_INTF_H_

#include "dsrc_config.h"



void sdrc_msg_input(unsigned char *msg,unsigned short len);
void sdrc_msg_output(unsigned char *msg,unsigned short len);
int sdrc_set_profile(unsigned char profile);

int sdrc_app_get_profile(char ops,char *msg,int len,int offs,char type);


void dsrc_app_notification_other_mode_run(char type);
void dsrc_app_notification_other_mode_stop(char type);
int dsrc_app_operation_esam(char ops,unsigned char *in,unsigned short len,
	unsigned char *out,unsigned short out_len);
int dsrc_app_operation_icc(char ops,unsigned char *in,unsigned short len,
	unsigned char *out,unsigned short out_len);



#endif
