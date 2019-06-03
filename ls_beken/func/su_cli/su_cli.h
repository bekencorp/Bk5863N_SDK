#ifndef _SU_CLI_H_
#define _SU_CLI_H_

#include "su_cli_config.h"
#include "gernel.h"
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#define SU_CLI_MAX_COMMANDS	     4
#define SU_CLI_INBUF_SIZE        160
#define SU_CLI_OUTBUF_SIZE       0

enum{
	SU_CLI_HEX = 0,
	SU_CLI_STR
};

struct su_cli_command
{
    /** The name of the CLI command */
    const char *name;
    /** The help text associated with the command */
    const char *help;
    /** The function that should be invoked for this command. */
    void (*function) (char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
};


struct su_cli_st
{
    unsigned initialized : 1;
    unsigned echo_disabled : 1;
	
    unsigned su_cli_mode : 2;
    unsigned short bp;	/* buffer pointer */
	
    unsigned int num_commands;
    const struct cli_command *commands[SU_CLI_MAX_COMMANDS];
	
    char inbuf[SU_CLI_INBUF_SIZE];
	#if SU_CLI_OUTBUF_SIZE
    char outbuf[SU_CLI_OUTBUF_SIZE];
	#endif
} ;


unsigned short *get_su_cli_bp(void);
unsigned char *get_su_cli_input(void);

void su_cli_main(void);

#endif
