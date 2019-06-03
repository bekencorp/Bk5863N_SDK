#ifndef _ESAM_APP_H_
#define _ESAM_APP_H_

#include "dsrc_config.h"
#include "gernel.h"


enum{
	ESAM_APP_OPEN = 0,
	ESAM_APP_WARN_RESET,
	ESAM_APP_CLOSE,
	
	
	ESAM_APP_EVT_MAX
};

#define ESAM_APP_OPEN_EVENT   (1U << (31 - ESAM_APP_OPEN))
#define ESAM_APP_CLOSE_EVENT   (1U << (31 - ESAM_APP_CLOSE))
#define ESAM_APP_WARN_RESET_EVENT   (1U << (31 - ESAM_APP_WARN_RESET))

unsigned int get_esam_app_event(void);
int set_esam_app_event(unsigned int event);
int reset_esam_app_event(unsigned int event);

void esam_app_handler(void);


int get_esam_app_wk_state(void);

#endif
