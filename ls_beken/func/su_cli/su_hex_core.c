#include "su_hex_core.h"
#include "su_cli.h"

#if SU_CLI_HEX_ENABLE

struct su_hex_core{
	unsigned init : 1;
	unsigned step : 4;
	unsigned char type;
	unsigned char su_type;
	unsigned short rec_len;
	unsigned short bp;
	unsigned char *buf;
};

struct su_hex_type_table{
	unsigned char type;
	void (*handle)(unsigned char su_type,unsigned char *msg,unsigned int msg_len);
};

static struct su_hex_core _su_hex = {
	0
};

void reset_uart_buf(unsigned char su_type,unsigned char *msg,unsigned int msg_len)
{
	char ii;
	while(su_cli_getchar(&ii));
}

static void obu_send_msg(unsigned char su_type,unsigned char *msg,unsigned int msg_len)
{
	BK5824SendData(msg,msg_len);
}

const static struct su_hex_type_table _su_hex_table[] = {
	{0,         reset_uart_buf},
	{0x21U,        obu_send_msg},
};

const static unsigned char _su_hex_table_size = sizeof(_su_hex_table);

void su_hex_core_init(void)
{
///	_su_hex.buf = get_su_cli_input();
}

void su_hex_core_handler(void)
{
	unsigned char i;

	for(i=0;i<_su_hex_table_size;i++){
		if(_su_hex.type == _su_hex_table[i].type){
			if(_su_hex_table[i].handle){
				_su_hex_table[i].handle(_su_hex.su_type,_su_hex.buf,_su_hex.rec_len);
			}
			break;
		}
	}

	_su_hex.bp = 0;
	_su_hex.rec_len = 0;
	_su_hex.step = 0;
}

void su_hex_core_main(void)
{
	unsigned char ii;
	
	if(su_cli_getchar((char*)&ii)){
		switch(_su_hex.step)
		{
			case 0:
				if(ii == 0xA5U){
					_su_hex.buf = get_su_cli_input();
					_su_hex.step = 1;
				}else{
					_su_hex.step = 0;
				}
				break;
			case 1:
				_su_hex.type = ii;
				_su_hex.step = 2;
				break;
			case 2:
				_su_hex.su_type = ii;
				_su_hex.step = 3;
				break;
			case 3:
				_su_hex.rec_len = ii;
				_su_hex.bp = 0;
				_su_hex.step = 4;
				break;
			case 4:
				_su_hex.rec_len |= (ii << 8);
				if(_su_hex.rec_len){
					_su_hex.step = 5;
				}else{
					su_hex_core_handler();
				}
				break;
			case 5:
				if(_su_hex.bp < _su_hex.rec_len){
					if(_su_hex.buf != NULL)
					{
						_su_hex.buf[_su_hex.bp] = ii;
					}
					_su_hex.bp++;
				}
				
				if(_su_hex.bp >= _su_hex.rec_len){
					su_hex_core_handler();
				}
				break;
			default:
				
				break;
		}
	}
}





#endif ///SU_CLI_HEX_ENABLE


