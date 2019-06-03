#include "dsrc_lsdu.h"
#include "dsrc_app.h"
#include "string.h"
#include "stdio.h"


struct lsdu_type_item{
	unsigned char type;
	int (*func)(int arg,unsigned char *msg,unsigned int len);
	int arg;
};

const static struct lsdu_type_item lsdu_type_table[] = {
	{0xC,  dsrc_app_bst,   0},///BST
	{0xD,  NULL,   0},///VST
	{0,    dsrc_app_Action_Request   ,0},
	{1,    NULL   ,0},
	{6,    dsrc_app_event_report   ,0}
};

const unsigned int lsdu_type_table_size = sizeof(lsdu_type_table)/sizeof(struct lsdu_type_item);

int dsrc_msg_period_head(unsigned char hd)
{
	char flg;
	if((hd & (~(0xF<<3))) != 0x81){
		return -1;
	}
	
	flg = (hd>>3)&0xF;
	if((flg == 0x0) || (flg == 0x01)){
		return -1;
	}
	
	return 0;
}

int dsrc_msg_add_period_head(char ops,unsigned char *msg,
	unsigned short len,unsigned short offset,char flg)
{
	if(DSRC_VERSION_1 == ops){
		if(len < (DSRC_MAC_SIZE + 1 + 2 + 1)){
			return -1;
		}
	}else{
		return -1;
	}
	
	if(DSRC_VERSION_1 == ops){
		flg = (((flg>0x01)?flg:0x02) << 3);
		msg[offset] = 0x81 | flg;
		return 1;
	}
	return 0;
}

int dsrc_msg_lsdu_handler(char ops,unsigned char *msg,unsigned short len)
{
	unsigned char type;
	unsigned char i;
	
	if(DSRC_VERSION_1 == ops){
		type = (msg[0]>>4)&0x0F;
		for(i = 0;i < lsdu_type_table_size;i++){
			if(lsdu_type_table[i].type == type){
				if(lsdu_type_table[i].func){
					lsdu_type_table[i].func(lsdu_type_table[i].arg,msg,len);
				}
				break;
			}
		}
	}else{
		return -1;
	}
	
	return 0;
}









