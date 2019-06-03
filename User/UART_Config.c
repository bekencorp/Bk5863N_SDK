#include "UART_Config.h"

struct __FILE {
  int handle;                 // Add whatever  need here 
};

FILE __stdout;
FILE __stdin;

void uart_send_byte(uint8_t data) 
{
    while(!UART_TX_WRITE_READY);
	
    UART_WRITE_BYTE(data);
}

/*----------------------------------------------------------------------------
  fputc
 *----------------------------------------------------------------------------*/
int fputc(int ch, FILE *f) 
{
    uart_send_byte(ch);
	
    return ch;
}

/*----------------------------------------------------------------------------
  fgetc
 *----------------------------------------------------------------------------*/
int fgetc(FILE *f) 
{
  return (-1);
}






