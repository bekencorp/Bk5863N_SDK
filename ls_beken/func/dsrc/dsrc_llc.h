#ifndef _DSRC_LLC_H_
#define _DSRC_LLC_H_

#include "dsrc_config.h"



int dsrc_llc_release(char ops);
int dsrc_llc_msg_handler(char ops,unsigned char *msg,unsigned short len);
int dsrc_llc_add_head(char ops,unsigned char *msg,
	unsigned short len,unsigned short offset,char type,unsigned char llc_state);

#endif
