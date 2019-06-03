#ifndef _BK5824Test_H_
#define _BK5824Test_H_

#include "type.h"
#include "BKTestCom.h"






void bk_RX_data_test(void);
void bk_TX_data_test(void);
void bk_enter_RX(void);


#ifdef BEKEN_BK5824_MODULE_TEST
void bk_test_pn9(void);
void bk_test_ber(void);
void bk_test_single(void);
void bk_test_obu0Wakeup(void);
void bk_test_obu0TRx(void);
void bk_test_obu(void);
void bk_test_rsu(void);
void bk5824_wk2_single(int state);
void BK5863TestBK5824(void);



#endif

#endif /* _BK5824Test_H_ */
