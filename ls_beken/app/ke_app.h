#ifndef _KE_APP_H_
#define _KE_APP_H_

#include "gernel.h"
#include <string.h>
#include <stdio.h>
#include <stdarg.h>


typedef struct{
		const char *name;
		void (*fun)(void);
}loop_call_t;


extern const loop_call_t lpcall_table[];
extern const unsigned int lpcall_table_size;

#ifdef __INLINE
__INLINE void loop_call(void)
{
	int i;
	const loop_call_t *lpcall;
	
	for(i=0;i<(lpcall_table_size/sizeof(loop_call_t));i++)
	{
		lpcall = &lpcall_table[i];
		if(lpcall->fun)
		{
			lpcall->fun();
		}
	}
}
#else
void loop_call(void);
#endif

#endif
