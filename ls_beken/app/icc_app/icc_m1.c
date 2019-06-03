#include "icc_m1.h"
#include "BKCard.h"
#include "icc_app.h"
#include "CardConfig.h"
#include "BKTestD.h"
#include "BKCardTest.h"
#include "string.h"
#include "stdio.h"

static CardReaderMsg_t m1CrdRdMsg;

int icc_m1_select_card(void)
{
	int status;
	if(get_icc_app_wk_state() == 0){
		return -1;
	}
	memset(&m1CrdRdMsg, sizeof(CardReaderMsg_t), 0);
	m1CrdRdMsg.BlockNum = 1;
	m1CrdRdMsg.CardType = 0;  //M1
	m1CrdRdMsg.OprtMode = READCARD;
	m1CrdRdMsg.PiccCid  = 2;
	m1CrdRdMsg.TxSpeed  = 0;
	m1CrdRdMsg.RxSpeed  = 0;
	memset(m1CrdRdMsg.Key, 0xFF, 6);
	m1CrdRdMsg.FrameDly = 25;
	m1CrdRdMsg.ReqGuardTime = 500;
	m1CrdRdMsg.SFGIDly  = 0;
	m1CrdRdMsg.OprtNum  = 12;   ///16
////	BK5863TestCardReader(&m1CrdRdMsg);
	PcdPower(Enable);
	delay_ms(3);  //等待电源稳定
	status = BK5863TestSelectCard(&m1CrdRdMsg);
	if(status != MI_OK)
	{
		set_icc_app_card_infomation(0);
		return -1;
	}
	set_icc_app_card_infomation(&m1CrdRdMsg);
	set_icc_app_card_type(0);
	PcdHalt();
	return 0;
}

int set_m1_card_key(char *key)
{
	memcpy(m1CrdRdMsg.Key, key, 6);
	return 0;
}

int icc_m1_read(char ops,unsigned char *msg,int len)
{
	int status;
	
	m1CrdRdMsg.OprtMode = READCARD;
	
	m1CrdRdMsg.BlockNum = ops;
	status = BK5863TestSelectCard(&m1CrdRdMsg);
	status = BK5863TestOperateM1Card_ops(&m1CrdRdMsg,(char*)msg,len);
	if(status < 0)
	{
		return -1;
	}
	return status;
}

int icc_m1_write(char ops,unsigned char *msg,int len)
{
	int status;

	m1CrdRdMsg.OprtMode = WRITECARD;
	m1CrdRdMsg.BlockNum = ops;
	m1CrdRdMsg.pBuff = msg;
	
	status = BK5863TestSelectCard(&m1CrdRdMsg);
	status = BK5863TestOperateM1Card_ops(&m1CrdRdMsg,(char*)msg,len);
	if(status < 0)
	{
		return -1;
	}
	return status;
}




