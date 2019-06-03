#ifndef _DSRC_H_
#define _DSRC_H_


#include "dsrc_config.h"


void dsrc_msg_handler(unsigned char *msg,unsigned short len);
void dsrc_handler(void);
void dsrc_timer_handler(unsigned short step);
void dsrc_link_reset(void);

#endif
