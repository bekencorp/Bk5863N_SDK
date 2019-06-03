#ifndef _ICC_APP_H_
#define _ICC_APP_H_

#include "dsrc_config.h"
#include "gernel.h"

enum{
	ICC_APP_OPEN = 0,
	ICC_APP_CLOSE,
	ICC_CARD_SELECT,
	
	ICC_APP_EVT_MAX
};

#define ICC_APP_OPEN_EVENT          (1U << (31 - ICC_APP_OPEN))
#define ICC_APP_CLOSE_EVENT         (1U << (31 - ICC_APP_CLOSE))
#define ICC_APP_CARD_SELECT_EVENT   (1U << (31 - ICC_CARD_SELECT))


void icc_app_handler(void);
void icc_app_timer_handler(unsigned char step);


void set_icc_app_card_infomation(void *info);
void set_icc_app_card_type(char type);

unsigned int get_icc_app_event(void);
int set_icc_app_event(unsigned int event);
int reset_icc_app_event(unsigned int event);


int get_icc_app_wk_state(void);

#endif
