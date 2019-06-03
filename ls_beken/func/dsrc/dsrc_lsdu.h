#ifndef _DSRC_LSDU_H_
#define _DSRC_LSDU_H_

#include "dsrc_config.h"



int dsrc_msg_period_head(unsigned char hd);
int dsrc_msg_add_period_head(char ops,unsigned char *msg,
	unsigned short len,unsigned short offset,char flg);
int dsrc_msg_lsdu_handler(char ops,unsigned char *msg,unsigned short len);




#endif
