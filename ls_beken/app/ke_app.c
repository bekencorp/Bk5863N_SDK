#include "ke_app.h"
#include "su_cli.h"
#include "obu_app.h"
#include "ke_event.h"
#include "dsrc.h"
#include "dsrc_app.h"
#include "esam_app.h"
#include "icc_app.h"

const loop_call_t lpcall_table[] = {
	{"su_cli",  su_cli_main},
	{"ke_evt",  ke_evt_scheduler},
	{"dsrc",    dsrc_handler},
	{"dsrc_app",dsrc_app_handler},
	{"esam",    esam_app_handler},
	{"icc",     icc_app_handler},
	{NULL,      NULL}
};

const unsigned int lpcall_table_size = sizeof(lpcall_table);

#ifndef __INLINE
void loop_call(void)
{
	int i;
	const loop_call_t *lpcall;
	
	for(i=0;i<(sizeof(lpcall_table)/sizeof(loop_call_t));i++)
	{
		lpcall = &lpcall_table[i];
		if(lpcall->fun)
		{
			lpcall->fun();
		}
	}
}
#endif

