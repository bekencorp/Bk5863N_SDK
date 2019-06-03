#include "dsrc_mac.h"
#include "string.h"
#include "stdio.h"

struct _dsrc_mac_mang{
	unsigned char mac[DSRC_MAC_SIZE];
	unsigned idle : 1;
	unsigned bst : 1;    ////BST receive
	unsigned rsu_Q : 1;
	unsigned p_link : 1;
	unsigned LPDU : 1;    ///本次数据包是否有LPDU 
	
	unsigned Q_timeout : 16;
};

static struct _dsrc_mac_mang _dsrc_mac = {
	DSRC_DEFAULT_MAC_ADDR,
	1,
	0,
	0,
	0,
	0,
	0
};

void set_dsrc_mac_receive_bst(unsigned char ops)
{
	_dsrc_mac.bst = (ops == 0)?0:1;
}

int get_dsrc_mac_receive_bst(void)
{
	return (_dsrc_mac.bst == 0)?0:1;
}

void set_dsrc_rsu_Q_flag(unsigned char ops)
{
	_dsrc_mac.rsu_Q = (ops == 0)?0:1;
}

int get_dsrc_rsu_Q_flag(void)
{
	return (_dsrc_mac.rsu_Q == 0)?0:1;
}

void set_dsrc_proprietary_link_flag(unsigned char ops)
{
	_dsrc_mac.p_link = (ops == 0)?0:1;
}

int get_dsrc_proprietary_link_flag(void)
{
	return (_dsrc_mac.p_link == 0)?0:1;
}

void set_dsec_mac_addr(unsigned char *mac)
{
	memcpy(_dsrc_mac.mac,mac,DSRC_MAC_SIZE);
}

unsigned char *get_dsec_mac_addr(void)
{
	return _dsrc_mac.mac;
}

void set_dsrc_mac_idle(unsigned char ops)
{
	_dsrc_mac.idle = (ops == 0)?0:1;
}

int get_dsrc_mac_idle(void)
{
	return (_dsrc_mac.idle == 0)?0:1;
}


void set_dsrc_mac_LPDU(unsigned char ops)
{
	_dsrc_mac.LPDU = (ops == 0)?0:1;
}

int get_dsrc_mac_LPDU(void)
{
	return (_dsrc_mac.LPDU == 0)?0:1;
}

void set_dsrc_mac_Q_timeout(unsigned int ops)
{
	GLOBAL_INT_DECLARATION();
	 
	GLOBAL_INT_DISABLE();
	_dsrc_mac.Q_timeout = (ops > 0xFFFFU)?ops:0xFFFFU;
	GLOBAL_INT_RESTORE();
}

int get_dsrc_mac_Q_timeout(void)
{
	return (int)(_dsrc_mac.Q_timeout);
}

int dsrc_mac_release(char ops)
{
	set_dsrc_rsu_Q_flag(0);
	set_dsrc_proprietary_link_flag(0);
	set_dsrc_mac_receive_bst(0);
	set_dsrc_mac_LPDU(0);
	set_dsrc_mac_Q_timeout(0);
	set_dsrc_mac_idle(1);
	return 0;
}

int dsrc_mac_msg_handler(char ops,unsigned char *msg,unsigned short len)
{
	char i;
	unsigned char mac_control;
	
	if(DSRC_VERSION_1 == ops){
		if(len < (DSRC_MAC_SIZE + 1)){
			return -1;
		}
	}
	else{
		return -1;
	}
	
	mac_control = msg[DSRC_MAC_SIZE];
	if(strncmp((char*)_dsrc_mac.mac,(char*)msg,DSRC_MAC_SIZE) == 0){
		if((mac_control&0x80) != 0) {
				return -1;
		}
		
		if((mac_control&0x20) != 0) {
				return -1;
		}
		
	#if 0
		if((mac_control&0x10)?1:0){
			return -1;
		}
	#endif
		

		if((get_dsrc_proprietary_link_flag() == 0)&&(get_dsrc_rsu_Q_flag() == 1)){
			set_dsrc_rsu_Q_flag(0);
			set_dsrc_mac_receive_bst(0);
			set_dsrc_proprietary_link_flag(1);
			set_dsrc_mac_Q_timeout(0);
		}
		else if(get_dsrc_proprietary_link_flag()){
//			set_dsrc_rsu_Q_flag(0);
//			set_dsrc_mac_receive_bst(0);
//			set_dsrc_mac_Q_timeout(0);
		}
		else{
			return -1;
		}
		
		set_dsrc_mac_LPDU(((mac_control&0x40)?1:0));
	}
	else{
		for(i=0;i<DSRC_MAC_SIZE;i++){
			if(msg[i] != 0xFFU){
				return -1;
			}
		}
		
		if(get_dsrc_proprietary_link_flag() == 0){
			if((mac_control&0x80) != 0){
				return -1;
			}
			
			if((mac_control&0x20) != 0){
				return -1;
			}	
			
			set_dsrc_mac_LPDU(((mac_control&0x40)?1:0));

			set_dsrc_rsu_Q_flag(((mac_control&0x10)?1:0));
			if(((mac_control&0x10)?1:0)){
				set_dsrc_mac_Q_timeout(DSRC_BST_MAC_CTRL_Q_TIMEOUT);
			}
			set_dsrc_mac_receive_bst(1);
			set_dsrc_mac_idle(0);
		}
		else{
			return -1;
		}
	}
	return 0;
}

int dsrc_mac_add_head(char ops,unsigned char *msg,unsigned short len,
	unsigned short offset,char type,char LPDU)
{
	unsigned char mac_control;
	
	if(DSRC_VERSION_1 == ops){
		if(len < (DSRC_MAC_SIZE + 1)){
			return -1;
		}
	}else{
		return -1;
	}
	memcpy(msg,_dsrc_mac.mac,DSRC_MAC_SIZE);

	if(ops == DSRC_VERSION_1){
		mac_control = (1U<<7);
		if(type == 3){
			mac_control |= (1U<<5);
		}else if(type == 0){
			mac_control &= (~(1U<<5));
		}else{
			return -1;
		}
		
		if(LPDU) {
			mac_control |= (1<<6);
		}
	}
	else{
		return -1;
	}

	msg[DSRC_MAC_SIZE] = mac_control;
	
	return 0;
}

void dsrc_mac_handler(void)
{
	if((get_dsrc_rsu_Q_flag())&&(get_dsrc_proprietary_link_flag() == 0)&&(_dsrc_mac.Q_timeout == 0)){
		///set_dsrc_rsu_Q_flag(0);
		dsrc_mac_release(0);
		printf("%s,%d dsrc_mac_release\r\n ",__FUNCTION__,__LINE__);
	}
}

void dsrc_mac_timer_handler(unsigned char step)
{
	if((_dsrc_mac.Q_timeout)&&(get_dsrc_rsu_Q_flag() == 1)&&(get_dsrc_proprietary_link_flag() == 0)){
		if(_dsrc_mac.Q_timeout > step) {
			_dsrc_mac.Q_timeout -= step;
		}else{
			_dsrc_mac.Q_timeout = 0;
		}
	}
}


