#include "esam_app.h"
#include "com_op_math.h"
#include "sl_esam.h"
#include "app_esam_5121.h"
#include "string.h"
#include "stdio.h"

struct esam_app_mang{
	unsigned int event;
	unsigned esam_wk :1;
};



typedef struct{
	const unsigned int event;
	void (*fun)(int);
	int arg;
}esam_evt_tag;

static struct esam_app_mang _eapp = {
	0
};

unsigned int get_esam_app_event(void)
{
	return _eapp.event;
}

int set_esam_app_event(unsigned int event)
{
	GLOBAL_INT_DECLARATION();
	
	GLOBAL_INT_DISABLE();
	_eapp.event |= event;
	GLOBAL_INT_RESTORE();
	
	return 0;
}

int reset_esam_app_event(unsigned int event)
{
	GLOBAL_INT_DECLARATION();
	
	GLOBAL_INT_DISABLE();
	_eapp.event &= (~event);
	GLOBAL_INT_RESTORE();
	
	return 0;
}

void esam_app_reset_event(int arg)
{
	reset_esam_app_event(ESAM_APP_OPEN_EVENT);
	if(bk_esam_reset() == 0){
		_eapp.esam_wk = 1;
	}
}

void esam_app_warn_reset_event(int arg)
{
	if(bk_esam_reset() == 0){
		_eapp.esam_wk = 1;
	}
	reset_esam_app_event(ESAM_APP_WARN_RESET_EVENT);
}

void esam_app_close_event(int arg)
{
	esam_hardware_close(0,0,0);
	_eapp.esam_wk = 0;
	reset_esam_app_event(ESAM_APP_CLOSE_EVENT);
}

int get_esam_app_wk_state(void)
{
	return (_eapp.esam_wk == 0)?0:1;
}

const static esam_evt_tag _esam_evt_table[] = {
	{ESAM_APP_OPEN_EVENT , esam_app_reset_event ,0},
	{ESAM_APP_WARN_RESET_EVENT , esam_app_warn_reset_event ,0},
	{ESAM_APP_CLOSE_EVENT , esam_app_close_event ,0},
};

void esam_app_handler(void)
{
	unsigned int field = get_esam_app_event();
	unsigned int event;
	
	while (field) // Compiler is assumed to optimize with loop inversion
	{
			// Find highest priority event set
			event = co_clz(field);

			// Sanity check
			ASSERT_ERR((event < ESAM_APP_EVT_MAX) && _esam_evt_table[event].fun);

			// Execute corresponding handler
			(_esam_evt_table[event].fun)(_esam_evt_table[event].arg);

			// Update the volatile value
			field = get_esam_app_event();
	}
	
}







