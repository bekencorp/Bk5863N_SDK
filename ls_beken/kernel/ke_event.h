#ifndef _KE_EVENT_H_
#define _KE_EVENT_H_

#include "gernel.h"
#include "com_op_math.h"
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

enum
{
    KE_EVT_RESET = 0,
	
    KE_EVT_MAX
};

#define KE_EVT_RESET_BIT          CO_BIT(31 - KE_EVT_RESET)          ///< Reset event

typedef unsigned int event_field_t;

typedef struct{
	const char *name;
	void (*fun)(int);
	int arg;
}ke_evt_tag;

struct ke_event_tag
{
    volatile event_field_t evt_field;
};


void ke_evt_set(event_field_t const event);
void ke_evt_clear(event_field_t const event);
void ke_evt_scheduler(void);
void ke_event_init(void);

#endif   ////_KE_EVENT_H_
