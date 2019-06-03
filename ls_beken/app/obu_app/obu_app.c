#include "obu_app.h"
#include "su_cli_intf.h"
#include "UARTConfig.h"
#include "dsrc_intf.h"

#define USART_UART1_COMMAND    0

void obu_rec_msg_handler(unsigned char *msg,unsigned int len)
{
	int i;
//	unsigned char ucDataMsg[32];
#if OBU_REC_HEX_SHOW
	su_cli_putchar(0xA5U);
	su_cli_putchar(0x21U);
	su_cli_putchar(0x00U);
	su_cli_putchar((len&0xFF));
	su_cli_putchar(((len>>8)&0xFF));
#elif OBU_REC_STR_SHOW
	printf("%02x%02x%02x%02x%02x",0xA5U,0x21U,0x00U,(len&0xFF),((len>>8)&0xFF));
#endif
	for(i=0;i<len;i++){
	#if OBU_REC_HEX_SHOW
		su_cli_putchar((char)(msg[i]));
	#elif OBU_REC_STR_SHOW
		printf("%02x",msg[i]);
	#endif
	#if USART_UART1_COMMAND
		ucDataMsg[i] = (unsigned char)(msg[i]);
	#endif
	}
#if OBU_REC_STR_SHOW
	printf("\r\n");
#endif

#if 1	
	sdrc_msg_input(msg,len);
#elif USART_UART1_COMMAND
    if (ucDataMsg[0] == 0x53)
    {
        len = len - 1 - 2;
//        printf("len = %d\r\n", len);
//        printf("BKUART_CTL_BLK[0].rxRdIndex 1 = %d\r\n", BKUART_CTL_BLK[0].rxRdIndex);
//        printf("BKUART_CTL_BLK[0].rxWrIndex 1 = %d\r\n", BKUART_CTL_BLK[0].rxWrIndex);
        for (i=0; i<len; i++)
        {
            BKUART_CTL_BLK[0].pRx[BKUART_CTL_BLK[0].rxWrIndex] = ucDataMsg[i+1];
            BKUART_CTL_BLK[0].rxWrIndex ++;
            if (BKUART_CTL_BLK[0].rxWrIndex == Uart1RxBuffLength)
            {
                BKUART_CTL_BLK[0].rxWrIndex = 0;
            }
        }
//        printf("BKUART_CTL_BLK[0].rxRdIndex 2 = %d\r\n", BKUART_CTL_BLK[0].rxRdIndex);
//        printf("BKUART_CTL_BLK[0].rxWrIndex 2 = %d\r\n", BKUART_CTL_BLK[0].rxWrIndex);

//        for (i=0; i<len-1-2; i++)
//        {
//            su_cli_putchar(BKUART_CTL_BLK[0].pRx[i]);
//        }
    }
    else if ((ucDataMsg[0] == 0xFF) && (ucDataMsg[1] == 0xFF)
          && (ucDataMsg[2] == 0xFF) && (ucDataMsg[3] == 0xFF))
    {
//        unsigned char rsu1[] = {0xA5, 0x11, 0x00, 0x14, 0x00, 0x01, 0x02, 0x03,
//                                0x04, 0xC0, 0x03, 0x91, 0xD0, 0xA0, 0x00, 0x00,
//                                0x01, 0x01, 0xC2, 0x01, 0x00, 0x80, 0x00, 0xFB,
//                                0x16};
        unsigned char obu1_command[] = {0x01, 0xE0, 0xFC, 0x05, 0x80, 0x01, 0x1C};
        for (i=0; i<sizeof(obu1_command); i++)
        {
            BKUART_CTL_BLK[0].pRx[BKUART_CTL_BLK[0].rxWrIndex] = obu1_command[i];
            BKUART_CTL_BLK[0].rxWrIndex ++;
            if (BKUART_CTL_BLK[0].rxWrIndex == Uart1RxBuffLength)
            {
                BKUART_CTL_BLK[0].rxWrIndex = 0;
            }
        }
    }
#endif
}


