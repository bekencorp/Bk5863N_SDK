#ifndef _DSRC_MAC_H_
#define _DSRC_MAC_H_

#include "dsrc_config.h"


void set_dsrc_mac_receive_bst(unsigned char ops);
int get_dsrc_mac_receive_bst(void);
void set_dsrc_rsu_Q_flag(unsigned char ops);
int get_dsrc_rsu_Q_flag(void);
void set_dsrc_proprietary_link_flag(unsigned char ops);
int get_dsrc_proprietary_link_flag(void);
void set_dsrc_mac_idle(unsigned char ops);
int get_dsrc_mac_idle(void);
void set_dsrc_mac_LPDU(unsigned char ops);
int get_dsrc_mac_LPDU(void);
int dsrc_mac_release(char ops);

int dsrc_mac_add_head(char ops,unsigned char *msg,unsigned short len,
	unsigned short offset,char type,char LPDU);
int dsrc_mac_msg_handler(char ops,unsigned char *msg,unsigned short len);
void dsrc_mac_handler(void);
void dsrc_mac_timer_handler(unsigned char step);


#endif
