#include "su_cli_intf.h"
#include "UART_Config.h"
#include "UARTConfig.h"

int su_cli_printf(const char *msg, ...)
{
    va_list ap;
    char *pos, message[256];
    int sz;
    int nMessageLen = 0;

    memset(message, 0, 256);
    pos = message;

    sz = 0;
    va_start(ap, msg);
    nMessageLen = vsnprintf(pos, 256 - sz, msg, ap);
    va_end(ap);

    if( nMessageLen <= 0 ) return 0;

    su_cli_putstr((const char *)message);
    return 0;
}

int su_cli_putstr(const char *msg)
{
    if (msg[0] != 0){
        printf("%s",(const char *)msg);
		}
		
    return 0;
}

int su_cli_putchar(const char msg)
{
    uart_send_byte((unsigned char)msg);
    return 0;
}

int su_cli_getchar(char *inbuf)
{
	GLOBAL_INT_DECLARATION();
	
	if(su_cli_get_all_chars_len() > 0)
	{
			GLOBAL_INT_DISABLE();
      inbuf[0] = BKUART_CTL_BLK[0].pRx[BKUART_CTL_BLK[0].rxRdIndex];
		  BKUART_CTL_BLK[0].rxRdIndex++;
		  if(BKUART_CTL_BLK[0].rxRdIndex >= BKUART_CTL_BLK[0].rxBufLen){
		    BKUART_CTL_BLK[0].rxRdIndex = 0;
			}
		  GLOBAL_INT_RESTORE();
			return 1;
	}
	else{
			return 0;
	}
}

int su_cli_getchars(char *inbuf, int len)
{
		int i;
    if(su_cli_get_all_chars_len() >= len){
				for(i=0;i<len;i++){
					su_cli_getchar(&inbuf[i]);
				}
        return 1;
		}
    else{
        return 0;
		}
}

int su_cli_get_all_chars_len(void)
{
	GLOBAL_INT_DECLARATION();
	int len;
	
	GLOBAL_INT_DISABLE();
	if(BKUART_CTL_BLK[0].rxRdIndex == BKUART_CTL_BLK[0].rxWrIndex){
		len = 0;
	}
	else if(BKUART_CTL_BLK[0].rxRdIndex > BKUART_CTL_BLK[0].rxWrIndex){
		len = BKUART_CTL_BLK[0].rxWrIndex + (BKUART_CTL_BLK[0].rxBufLen - BKUART_CTL_BLK[0].rxRdIndex);
	}
	else if(BKUART_CTL_BLK[0].rxRdIndex < BKUART_CTL_BLK[0].rxWrIndex){
		len = BKUART_CTL_BLK[0].rxWrIndex - BKUART_CTL_BLK[0].rxRdIndex;
	}
	GLOBAL_INT_RESTORE();
	return len;
}



