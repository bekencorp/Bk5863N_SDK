#include "dsrc.h"
#include "dsrc_intf.h"
#include "dsrc_mac.h"
#include "dsrc_llc.h"
#include "dsrc_lsdu.h"
#include "dsrc_app.h"
#include "string.h"
#include "stdio.h"



void dsrc_msg_handler(unsigned char *msg,unsigned short len)
{
	int ret;
	unsigned char offset = 0;
	
	if(len < (DSRC_MAC_SIZE + 1 + 1 + 2))
		return;
	
	ret = dsrc_mac_msg_handler(DSRC_VERSION,msg,len);
	if(ret == 0){
		ret = dsrc_llc_msg_handler(DSRC_VERSION,msg,len - 2);
		if(ret != 0){
			printf("llc handler error\r\n");
			return;
		}
		offset = DSRC_MAC_SIZE + 1 + 1;
	#if DSRC_VERSION == DSRC_VERSION_1
		if(len < (offset + 1 + 2))
			return;
		ret = dsrc_msg_period_head(msg[offset]);
		if(ret != 0){
			printf("p head error\r\n");
			return;
		}
		offset += 1;
	#endif
		
		if(((len - 2) - offset) <= 0){
			return;
		}
		
		ret = dsrc_msg_lsdu_handler(DSRC_VERSION,msg + offset,(len - 2) - offset);
		if(ret != 0){
			printf("lsdu handler error\r\n");
			return;
		}
	}
	else{
		printf("mac handler error\r\n");
	}
}

void dsrc_handler(void)
{
	dsrc_mac_handler();
}

void dsrc_timer_handler(unsigned short step)
{
	dsrc_mac_timer_handler(step);
}

void dsrc_link_reset(void)
{
	dsrc_mac_release(0);
	dsrc_llc_release(0);
	dsrc_app_reset(0);
	dsrc_app_notification_other_mode_stop(0);
}

