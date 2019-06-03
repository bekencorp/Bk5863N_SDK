#include "ke_event.h"

static struct ke_event_tag _ke_evt = {0};

static const ke_evt_tag _ke_evt_table[32] = {
	
	{NULL,                     NULL           ,NULL}
};

void ke_evt_set(event_field_t const event)
{
	GLOBAL_INT_DECLARATION();

	GLOBAL_INT_DISABLE();
	if(!(_ke_evt.evt_field & event))
	{
		_ke_evt.evt_field |= event;
	}
	GLOBAL_INT_RESTORE();
}

void ke_evt_clear(event_field_t const event)
{
	GLOBAL_INT_DECLARATION();

	GLOBAL_INT_DISABLE();
	_ke_evt.evt_field &= ~event;
	GLOBAL_INT_RESTORE();
}

void ke_evt_scheduler(void)
{
	unsigned int field,event;
	
	field = _ke_evt.evt_field;
	while (field) // Compiler is assumed to optimize with loop inversion
	{
		// Find highest priority event set
		event = co_clz(field);

		// Sanity check
		ASSERT_ERR((event < KE_EVT_MAX) && _ke_evt_table[event].fun);

		// Execute corresponding handler
		(_ke_evt_table[event].fun)(_ke_evt_table[event].arg);

		// Update the volatile value
		field = _ke_evt.evt_field;
	}
	
}

void ke_event_init(void)
{
	ke_evt_clear(0xFFFFFFFF);
}





