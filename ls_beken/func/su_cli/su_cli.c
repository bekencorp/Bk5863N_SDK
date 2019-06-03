#include "su_cli.h"
#include "su_hex_core.h"
#include "su_str_core.h"


struct su_cli_st _su_cli = {
	0
};


unsigned char *get_su_cli_input(void)
{
	return (unsigned char *)_su_cli.inbuf;
}

unsigned short *get_su_cli_bp(void)
{
	return &_su_cli.bp;
}

void su_cli_main(void)
{
	switch(_su_cli.su_cli_mode)
	{
	#if SU_CLI_HEX_ENABLE
		case SU_CLI_HEX:
			su_hex_core_main();
			break;
	#endif 
		case SU_CLI_STR:
			break;
	}
}




