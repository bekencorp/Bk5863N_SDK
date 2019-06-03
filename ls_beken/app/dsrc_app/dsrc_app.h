#ifndef _DSRC_APP_H_
#define _DSRC_APP_H_

#include "dsrc_config.h"

#define DSRC_APP_COMMUNICTION_TIME              (5000)

#define DSRC_APP_RSU_HOLD_TIME                  (255*1000)

#define Rsu_BeaconID_SIZE                      (4)

int dsrc_app_bst(int arg,unsigned char *msg,unsigned int len);
int dsrc_app_vst_make(char profile,char *in,unsigned int in_len);
int dsrc_app_Action_Request(int arg,unsigned char *msg,
	unsigned int len);
int dsrc_app_event_report(int arg,unsigned char *msg,unsigned int len);

void dsrc_app_timer_handler(unsigned char step);
void dsrc_app_handler(void);

int dsrc_app_reset(char ops);

#endif
