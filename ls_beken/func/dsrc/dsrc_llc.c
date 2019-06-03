#include "dsrc_llc.h"
//#include "dsrc_mac.h"

struct _dsrc_llc_mang{
	unsigned type : 3;
	unsigned prev_type : 3;
	unsigned V_RI : 1;
//	unsigned V_RB : 1;
};

static struct _dsrc_llc_mang _dsrc_llc = {
	0x4,
	0x4,
	0,
//	0,
	
};

void set_dsrc_llc_type(unsigned char ops)
{
	_dsrc_llc.type = ops;
}

int get_dsrc_llc_type(void)
{
	return (_dsrc_llc.type);
}

void set_dsrc_llc_prev_type(unsigned char ops)
{
	_dsrc_llc.prev_type = ops;
}

int get_dsrc_llc_prev_type(void)
{
	return (_dsrc_llc.prev_type);
}

void set_dsrc_llc_V_RI(unsigned char ops)
{
	_dsrc_llc.V_RI = (ops == 0)?0:1;
}

int get_dsrc_llc_V_RI(void)
{
	return (_dsrc_llc.V_RI == 0)?0:1;
}

int dsrc_llc_release(char ops)
{
	set_dsrc_llc_type(0x4);
	set_dsrc_llc_prev_type(0x4);
	set_dsrc_llc_V_RI(0);
	return 0;
}

int dsrc_llc_msg_handler(char ops,unsigned char *msg,unsigned short len)
{
	unsigned char llc_control;
	
	if(DSRC_VERSION_1 == ops){
		if(len < (DSRC_MAC_SIZE + 1)){
			return -1;
		}
	}
	else{
		return -1;
	}
	
	if(DSRC_VERSION_1 == ops){
		llc_control = msg[DSRC_MAC_SIZE + 1];
		if(llc_control == 0x03){
			set_dsrc_llc_prev_type(get_dsrc_llc_type());
			set_dsrc_llc_type(0x00);
		}
		else if((llc_control&0x67) == 0x67){
			set_dsrc_llc_prev_type(get_dsrc_llc_type());
			set_dsrc_llc_type(0x03);
			set_dsrc_llc_V_RI(((llc_control&0x80)?1:0));
		}
		else{
			return -1;
		}
	}
	return 0;
}

int dsrc_llc_add_head(char ops,unsigned char *msg,
	unsigned short len,unsigned short offset,char type,unsigned char llc_state)
{
	unsigned char mac_control;
	
	if(DSRC_VERSION_1 == ops){
		if(len < (DSRC_MAC_SIZE + 1)){
			return -1;
		}
	}else{
		return -1;
	}
	
	if(type == 0){
		msg[offset] = 0x03;
		return 1;
	}
	else if(type == 3){
		msg[offset] = 0x77;
		if(get_dsrc_llc_V_RI()){
			msg[offset] &= (~(0x80U));
		}
		else{
			msg[offset] |= 0x80U;
		}
		offset++;
		msg[offset++] = llc_state;
		return 2;
	}
	else{
		return -1;
	}
	
	return 0;
}



