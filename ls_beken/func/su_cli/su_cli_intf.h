#ifndef _SU_CLI_INTF_H_
#define _SU_CLI_INTF_H_

#include "gernel.h"
#include <string.h>
#include <stdio.h>
#include <stdarg.h>


int su_cli_printf(const char *msg, ...);
int su_cli_putstr(const char *msg);
int su_cli_putchar(const char msg);
int su_cli_getchar(char *inbuf);
int su_cli_getchars(char *inbuf, int len);
int su_cli_get_all_chars_len(void);

#endif
