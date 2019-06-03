#include "icc_app.h"
#include "com_op_math.h"
#include "CardConfig.h"
#include "string.h"
#include "stdio.h"
#include "icc_m1.h"


struct icc_app_mang{
	unsigned int event;
	unsigned wk : 1;
	unsigned sel_cd : 1;
	unsigned select_card_timeout : 16;
	unsigned cd_type : 2;
	void *card_info;
};

typedef struct{
	const unsigned int event;
	void (*fun)(int);
	int arg;
}icc_evt_tag;

static struct icc_app_mang _iapp = {
	0
};

void set_icc_app_card_infomation(void *info)
{
	_iapp.card_info = info;
}

void set_icc_app_card_type(char type)
{
	if(type > 0x03)
		return;
	_iapp.cd_type = type&0x03;
}

unsigned int get_icc_app_event(void)
{
	return _iapp.event;
}

int set_icc_app_event(unsigned int event)
{
	GLOBAL_INT_DECLARATION();
	
	GLOBAL_INT_DISABLE();
	_iapp.event |= event;
	GLOBAL_INT_RESTORE();
	
	return 0;
}

int reset_icc_app_event(unsigned int event)
{
	GLOBAL_INT_DECLARATION();
	
	GLOBAL_INT_DISABLE();
	_iapp.event &= (~event);
	GLOBAL_INT_RESTORE();
	
	return 0;
}

void icc_app_open_event(int arg)
{
	BK5863CardReaderInit();
	_iapp.wk = 1;
	_iapp.sel_cd = 0;
	_iapp.select_card_timeout = 1;
	reset_icc_app_event(ICC_APP_OPEN_EVENT);
}

void icc_app_close_event(int arg)
{
	PcdPowerDown();
	_iapp.wk = 0;
	_iapp.sel_cd = 0;
	reset_icc_app_event(ICC_APP_CLOSE_EVENT);
}

void icc_app_card_select_event(int arg)
{
	if(icc_m1_select_card() == 0){
		_iapp.sel_cd = 1;
	}
	else{
		_iapp.sel_cd = 0;
	}
	reset_icc_app_event(ICC_APP_CARD_SELECT_EVENT);
}


const static icc_evt_tag _icc_evt_table[] = {
	{ICC_APP_OPEN_EVENT , icc_app_open_event ,0},
	{ICC_APP_CLOSE_EVENT , icc_app_close_event ,0},
	{ICC_APP_CARD_SELECT_EVENT,icc_app_card_select_event,0},
};

void icc_app_handler(void)
{
	unsigned int field = get_icc_app_event();
	unsigned int event;
	
	while (field) // Compiler is assumed to optimize with loop inversion
	{
		// Find highest priority event set
		event = co_clz(field);
	
		// Sanity check
		ASSERT_ERR((event < ICC_APP_EVT_MAX) && _icc_evt_table[event].fun);

		// Execute corresponding handler
		(_icc_evt_table[event].fun)(_icc_evt_table[event].arg);

		// Update the volatile value
		field = get_icc_app_event();
	}
	
}

void icc_app_timer_handler(unsigned char step)
{
	if(_iapp.select_card_timeout){
		if(_iapp.select_card_timeout > step){
			_iapp.select_card_timeout -= step;
		}
		else{
			_iapp.select_card_timeout = 0;
			set_icc_app_event(ICC_APP_CARD_SELECT_EVENT);
		}
	}
}

int get_icc_app_wk_state(void)
{
	return (_iapp.wk == 1)?1:0;
}

