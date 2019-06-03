#include "dsrc_app_item1.h"
#include "dsrc_lsdu.h"
#include "dsrc_app_intf.h"

int dsrc_app_Action_Request_88handler(char *msg,unsigned short len)
{
	unsigned int ef03_len;
	unsigned int ef03_offset;
	unsigned char apdulist = 0;
	unsigned short apdu_length;
	unsigned short offset = 0;
	unsigned char idx;
	///unsigned char buf[128];
	int ret;
	
	if(len < 8){
		return -1;
	}
	ef03_len = msg[offset + 3] + (msg[offset + 2] << 8);
	ef03_len |= (msg[offset + 1] << 16) + (msg[offset + 0] << 24);
	ef03_offset = msg[offset + 7] + (msg[offset + 6] << 8);
	ef03_offset |= (msg[offset + 5] << 16) + (msg[offset + 4] << 24);
	offset += 8;
	
	if(len <= offset){
		return -1;
	}
	apdulist = msg[offset];
	
#if 0
	offset++;
	for(idx=0;idx<apdulist;idx++){
		if(len <= offset){
			return -1;
		}
		apdu_length = msg[offset];
		offset++;
		
		if(len <= (offset+apdu_length)){
			return -1;
		}
		sw1sw2 = esam_cmd_parse(&msg[offset], apdu_length, (char *)buf, &length);
		offset += apdu_length;
	}
#else
	offset++;
	if(apdulist != 0x01){
		return -1;
	}
	
	if(len <= offset){
		return -1;
	}
	apdu_length = msg[offset];
	if(apdu_length != 0x05){
		return -1;
	}
	offset++;
	
	if(len <= offset){
		return -1;
	}
	///0x00 B0 00 00 50
	///
	offset += apdu_length;
	ret = dsrc_app_action_response_89H_make(ef03_len,ef03_offset);
	if(ret == -1){
		return -1;
	}
#endif

	return 0;
}

int dsrc_app_action_response_89H_make(unsigned int ef03_len,unsigned int ef03_offset)
{
	char msg[126];
	unsigned char len = 0;
	char offset = 0;
	int ret;
	
	dsrc_mac_add_head(DSRC_VERSION,msg,126,0,3,1);
	offset += (DSRC_MAC_SIZE + 1);
	ret = dsrc_llc_add_head(DSRC_VERSION,msg,126,offset,3,0x00);
	if(ret == -1){
		return -1;
	}
	else{
		offset += ret;
	}
	
	ret = dsrc_msg_add_period_head(DSRC_VERSION,msg,126,offset,0x02);
	if(ret == -1){
		return -1;
	}
	else{
		offset += ret;
	}

	msg[offset++] = 0x18;
	msg[offset++] = 0x01;
	msg[offset++] = 0x89;

	ret = bk_rsu_obu_m_step2(&msg[offset + 4],ef03_len);
	if(ret < 0){
		msg[offset++] = (0 >> 24) & 0xFF;
		msg[offset++] = (0 >> 16) & 0xFF;
		msg[offset++] = (0 >> 8) & 0xFF;
		msg[offset++] = (0) & 0xFF;
	}else{
		msg[offset++] = (ret >> 24) & 0xFF;
		msg[offset++] = (ret >> 16) & 0xFF;
		msg[offset++] = (ret >> 8) & 0xFF;
		msg[offset++] = (ret) & 0xFF;
	}
	if(ret > 0){
		if(ret > ef03_len)
			offset += ef03_len;
		else{
			offset += ret;
		}
	}
	
	len = offset;
	sdrc_msg_output((unsigned char*)msg,len);
	
	return 0;
}




int dsrc_app_Action_Request_90handler(char *msg,unsigned short len)
{
	unsigned int rd_len;
	unsigned int block;
	unsigned short offset = 0;
	unsigned char idx;
	///unsigned char buf[128];
	int ret;
	
	if(len < 9){
		return -1;
	}
	block = msg[offset + 3] + (msg[offset + 2] << 8);
	block |= (msg[offset + 1] << 16) + (msg[offset + 0] << 24);
	rd_len = msg[offset + 7] + (msg[offset + 6] << 8);
	rd_len |= (msg[offset + 5] << 16) + (msg[offset + 4] << 24);
	offset += 8;

	if(msg[offset++] != 0x00){
		return -1;
	}

	ret = dsrc_app_action_response_91H_make(block,rd_len);
	if(ret == -1){
		return -1;
	}

	return 0;
}

int dsrc_app_action_response_91H_make(unsigned int block,unsigned int rd_len)
{
	char msg[126];
	unsigned char len = 0;
	char offset = 0;
	int ret;
	
	dsrc_mac_add_head(DSRC_VERSION,msg,126,0,3,1);
	offset += (DSRC_MAC_SIZE + 1);
	ret = dsrc_llc_add_head(DSRC_VERSION,msg,126,offset,3,0x00);
	if(ret == -1){
		return -1;
	}
	else{
		offset += ret;
	}
	
	ret = dsrc_msg_add_period_head(DSRC_VERSION,msg,126,offset,0x02);
	if(ret == -1){
		return -1;
	}
	else{
		offset += ret;
	}

	msg[offset++] = 0x18;
	msg[offset++] = 0x01;
	msg[offset++] = 0x91;

	ret = bk_rsu_obu_icc_read(block,&msg[offset + 4],rd_len);
	if(ret > 0){
		msg[offset++] = (ret >> 24) & 0xFF;
		msg[offset++] = (ret >> 16) & 0xFF;
		msg[offset++] = (ret >> 8) & 0xFF;
		msg[offset++] = (ret) & 0xFF;
	}
	else{
		msg[offset++] = (0 >> 24) & 0xFF;
		msg[offset++] = (0 >> 16) & 0xFF;
		msg[offset++] = (0 >> 8) & 0xFF;
		msg[offset++] = (0) & 0xFF;
	}
	if(ret > 0){
		if(ret > rd_len)
			offset += rd_len;
		else{
			offset += ret;
		}
	}
	
	len = offset;
	sdrc_msg_output((unsigned char*)msg,len);
	
	return 0;
}





