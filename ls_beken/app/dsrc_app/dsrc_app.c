#include "dsrc_app.h"
#include "dsrc_app_intf.h"
#include "dsrc_app_item1.h"
#include "dsrc.h"
#include "dsrc_intf.h"
#include "dsrc_mac.h"
#include "dsrc_llc.h"
#include "dsrc_lsdu.h"
#include "string.h"
#include "stdio.h"
#include "mmgmt.h"

typedef int (*dsrc_ContainerType_func)(unsigned char type,unsigned char *msg,unsigned int len,char **output,unsigned int *out_len);
struct rsu_infomation{
	unsigned char Icctransmode;
};

struct _dsrc_app_mang{
	unsigned wk : 1;
	unsigned wk_timout : 16;
	unsigned hold_rsu : 1;
	unsigned hold_rsu_timout : 18;
	
	unsigned Profile : 7;
	unsigned char rbid[Rsu_BeaconID_SIZE];
	unsigned int unix;

	struct rsu_infomation rsu;
};

static struct _dsrc_app_mang _dsrc_app = {
	0,
	
};

struct _DSRC_APP_ContainerType{
	unsigned char Co_Type;
	unsigned char limit_type;
	dsrc_ContainerType_func func;
};

int set_dsrc_app_hold_rsu(int ops)
{
	if(_dsrc_app.hold_rsu == 0){
		if(get_dsrc_proprietary_link_flag() == 0){
			return -1;
		}
		_dsrc_app.hold_rsu = 1;
	}
	return 0;
}

int update_dsrc_app_wk_info(void)
{
	GLOBAL_INT_DECLARATION();
	
	GLOBAL_INT_DISABLE();
	_dsrc_app.wk = 1;
	_dsrc_app.wk_timout = DSRC_APP_COMMUNICTION_TIME;
	GLOBAL_INT_RESTORE();
	printf("%s,%d update time\r\ ",__FUNCTION__,__LINE__);
}

int dsrc_app_container_type_31H(unsigned char type,unsigned char *msg,
	unsigned int len,char **output,unsigned int *out_len)
{
	union{
		struct{
			unsigned Option_indicator:3;
			unsigned Option_indicator5:1;
			unsigned Option_indicator4:1;
			unsigned Option_indicator3:1;
			unsigned Option_indicator2:1;
			unsigned Option_indicator1:1;
		}para;
		unsigned char param;
	}c_op;
	if(output){
		*output = NULL;
	}else{
		return -1;
	}
	if(out_len){
		*out_len = 0;
	}else{
		return -1;
	}
	if(msg[0] != 0x31U){
		return -1;
	}
	*output = mmgmt_malloc(128);
	if((*output) == NULL){
		goto err;
	}
	c_op.param = msg[1];
	if(c_op.param == 0)
	{
		(*output)[0] = 0x01;
		(*output)[1] = 0x00;
		*out_len = 2;
		return 1;
	}
	
	return 0;
	
err:
	if((*output) != NULL){
		mmgmt_free((*output));
		*output = NULL;
	}
	
	if(out_len){
		*out_len = 0;
	}
	return -1;
}

int dsrc_app_container_type_29H(unsigned char type,unsigned char *msg,
	unsigned int len,char **output,unsigned int *out_len)
{
	if(output){
		*output = NULL;
	}
	if(out_len){
		*out_len = 0;
	}
	
	return -1;
}

const struct _DSRC_APP_ContainerType _DSRC_APP_ContainerType_table[] ={
	{0x31, 0x0C,  dsrc_app_container_type_31H},
	{0x29, 0x0C,  dsrc_app_container_type_29H},
};

int *dsrc_app_find_container_type_handler(unsigned char type,unsigned char Co_Type)
{
	dsrc_ContainerType_func func = NULL;
	unsigned char i;
	
	for(i=0;i<(sizeof(_DSRC_APP_ContainerType_table)/sizeof(struct _DSRC_APP_ContainerType));i++){
		if(Co_Type == _DSRC_APP_ContainerType_table[i].Co_Type){
			if((_DSRC_APP_ContainerType_table[i].limit_type == type)
				||(_DSRC_APP_ContainerType_table[i].limit_type == 0xFF)){
				func = _DSRC_APP_ContainerType_table[i].func;
				break;
			}
		}
	}
	
	return (int*)func;
}


int dsrc_app_bst(int arg,unsigned char *msg,unsigned int len)
{
	unsigned short offset = 0;
	unsigned char profile;
	unsigned char MandApplications;
	unsigned char Icctransmode;
	char *output = NULL;
	unsigned int out_len = 0;
	int ret;
	dsrc_ContainerType_func func = NULL;
	
	if(_dsrc_app.hold_rsu == 1){
		goto error;
	}
	
	if(msg[0] != 0xC0U){
		goto error;
	}
	
	offset++;
	if(len < (offset + Rsu_BeaconID_SIZE)){
		goto error;
	}
	offset += Rsu_BeaconID_SIZE;
	
	if(len < (offset + 4)){
		goto error;
	}
	offset += 4;
	
	if(len < (offset + 1)){
		goto error;
	}
	if(msg[offset]&0x80U){
		goto error;
	}
	profile = msg[offset];
	offset++;
	
	if(msg[offset]&0x80U){
		goto error;
	}
	MandApplications = msg[offset];
	offset++;
	
	if(len < (offset + 1)){
		goto error;
	}
	if(msg[offset] != 0x42){
		goto error;
	}
	offset++;
	
	if(len < (offset + 1)){
		goto error;
	}
	Icctransmode = msg[offset]&0x7F;
	if(msg[offset]&0x80){
		offset++;
		if(len < (offset + 1)){
			goto error;
		}
		func = (dsrc_ContainerType_func)dsrc_app_find_container_type_handler((msg[0]>>4)&0x0F,msg[offset]);
		if(func != NULL){
			ret = func((msg[0]>>4)&0x0F,&msg[offset],len-(offset+1),&output,&out_len);
			if(ret == -1){
				goto error;
			}else{
				offset += ret;
			}
		}else{
			goto error;
		}
	}else{
		offset++;
	}
	
	if(len < (offset + 1)){
		goto error;
	}
	if(msg[offset] != 0x00){
		goto error;
	}
	offset++;
	if(len != offset)
	{
		return -1;
	}
	
	memcpy((char*)_dsrc_app.rbid,(char*)&msg[1],Rsu_BeaconID_SIZE);
	_dsrc_app.unix = msg[1+Rsu_BeaconID_SIZE] + (msg[1+Rsu_BeaconID_SIZE+1] <<8);
	_dsrc_app.unix |= (msg[1+Rsu_BeaconID_SIZE+2]<<16) + (msg[1+Rsu_BeaconID_SIZE+3]<<24);
	_dsrc_app.Profile = profile;

	if(sdrc_set_profile(profile) != 0){
		goto error;
	}
	
	_dsrc_app.rsu.Icctransmode = Icctransmode;
	ret = dsrc_app_vst_make(profile,output,out_len);
	if(ret != 0){
		goto error;
	}
	update_dsrc_app_wk_info();
	if(output){
		mmgmt_free(output);
	}
	
	dsrc_app_notification_other_mode_run(0);
	return 0;
error:
	if(output){
		mmgmt_free(output);
	}
	(void)MandApplications;
	(void)Icctransmode;
	return -1;
}

int dsrc_app_vst_make(char profile,char *in,unsigned int in_len)
{
	char msg[126];
	unsigned char len = 0;
	char offset = 0;
	int ret;

	dsrc_mac_add_head(DSRC_VERSION,(unsigned char*)msg,126,0,0,1);
	offset += (DSRC_MAC_SIZE + 1);
	ret = dsrc_llc_add_head(DSRC_VERSION,(unsigned char*)msg,126,offset,0,0);
	if(ret == -1){
		return -1;
	}
	else{
		offset += ret;
	}
	
	ret = dsrc_msg_add_period_head(DSRC_VERSION,(unsigned char*)msg,126,offset,0x02);
	if(ret == -1){
		return -1;
	}
	else{
		offset += ret;
	}
	msg[offset++] = 0xD0;
	memcpy(&msg[offset],_dsrc_app.rbid,Rsu_BeaconID_SIZE);
	offset += Rsu_BeaconID_SIZE;
	msg[offset+0] = 0x01;
	msg[offset+1] = 0xC2;
	offset += 2;
	memcpy(&msg[offset],in,in_len);
	offset += in_len;
	msg[offset] = 0x80;
	offset += 1;
	ret = sdrc_app_get_profile(DSRC_VERSION,msg,126,offset,0x0);
	if(ret == -1){
		return -1;
	}
	else{
		offset += ret;
	}
	len = offset;
	sdrc_msg_output((unsigned char*)msg,len);
	return 0;
}

int dsrc_app_Action_Request(int arg,unsigned char *msg,
	unsigned int len)
{
	unsigned char did;
	unsigned short offset = 0;
	unsigned char ActionType;
	unsigned char ChannelRq;
	struct act_param{
		unsigned accessCredentials : 1;
		unsigned actionParamter : 1;
		unsigned iid : 1;
		unsigned mode : 1;
	}act_p;

	if(offset >= len){
		goto error;
	}
	if(set_dsrc_app_hold_rsu(0) != 0){
		goto error;
	}
	if((msg[offset]&0xF0) != 0x00U){
		goto error;
	}
	act_p.accessCredentials = (msg[offset]&0x80)?1:0;
	act_p.actionParamter = (msg[offset]&0x40)?1:0;
	act_p.iid = (msg[offset]&0x20)?1:0;
	act_p.mode = (msg[offset]&0x10)?1:0;
	offset++;
	
	if(offset >= len){
		goto error;
	}
	did = msg[offset++];
	
	if(offset >= len){
		goto error;
	}
	ActionType = msg[offset++];
	if(ActionType != 0x03){
		goto error;
	}
	
	if(offset >= len){
		goto error;
	}
	ChannelRq = msg[offset++];
	if(ChannelRq == 0x88){
		if(dsrc_app_Action_Request_88handler((char*)(msg + offset),len - offset) == -1){
			printf("%s,%d goto error\r\ ",__FUNCTION__,__LINE__);
			goto error;
		}
	}
	else if(ChannelRq == 0x90){
		if(dsrc_app_Action_Request_90handler((char*)(msg + offset),len - offset) == -1){
			printf("%s,%d goto error\r\ ",__FUNCTION__,__LINE__);
			goto error;
		}
	}
	else{
		goto error;
	}
	
	update_dsrc_app_wk_info();
	return 0;
error:
	
	
	(void)act_p;
	(void)did;
	return -1;
}

int dsrc_app_event_report(int arg,unsigned char *msg,unsigned int len)
{
	unsigned offset = 0;
	unsigned char did;
	unsigned char EventType;
	struct{
		unsigned accessCredentials : 1;
		unsigned actionParamter : 1;
		unsigned IID : 1;
		unsigned mode : 1;
	}param;
	
	if(len < offset){
		return -1;
	}
	if((msg[offset]&0xF0) != 0x60){
		return -1;
	}
	param.accessCredentials = (msg[offset]&0x8)?1:0;
	param.actionParamter = (msg[offset]&0x4)?1:0;
	param.IID = (msg[offset]&0x2)?1:0;
	param.mode = (msg[offset]&0x1)?1:0;
	offset++;
	if(len < offset){
		return -1;
	}
	did = msg[offset++];
	EventType = msg[offset++];
	
	if(len < offset){
		return -1;
	}
	printf("%s,%d dsrc_link_reset\r\ ",__FUNCTION__,__LINE__);
	dsrc_link_reset();
}



void dsrc_app_timer_handler(unsigned char step)
{
	if((_dsrc_app.wk)&&(_dsrc_app.wk_timout)){
		if(_dsrc_app.wk_timout) {
			_dsrc_app.wk_timout -= step;
		}else{
			_dsrc_app.wk_timout = 0;
		}
	}
}

void dsrc_app_handler(void)
{
	if((_dsrc_app.wk)&&(_dsrc_app.wk_timout == 0)){
		printf("%s,%d dsrc_link_reset\r\ ",__FUNCTION__,__LINE__);
		dsrc_link_reset();
	}
}

int dsrc_app_reset(char ops)
{
	GLOBAL_INT_DECLARATION();

	GLOBAL_INT_DISABLE();
	_dsrc_app.hold_rsu = 0;
	_dsrc_app.wk = 0;
	_dsrc_app.wk_timout = 0;
	 GLOBAL_INT_RESTORE();
	return 0;
}


