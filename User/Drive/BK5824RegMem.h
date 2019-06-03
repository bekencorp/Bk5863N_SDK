#ifndef _BK5824RegMem_H_
#define _BK5824RegMem_H_


#define _RF_PN9_
#define _RF_BER_
#define _RF_CW_
#define _RF_OBU_
#define _RF_TRx_OBU_
#define _RF_RSU_



#ifdef _RF_PN9_
extern const unsigned int bk5823_rf_pn9[];
extern const unsigned char bk5823_rf_pn9_17[];
extern const unsigned char bk5823_rf_pn9_18[];
#endif

#ifdef _RF_BER_
extern const unsigned int bk5823_rf_ber[];
extern const unsigned char bk5823_rf_ber_17[];
extern const unsigned char bk5823_rf_ber_18[];
#endif
#ifdef _RF_CW_
extern const unsigned int bk5823_rf_cw[];
extern const unsigned char bk5823_rf_cw_17[];
extern const unsigned char bk5823_rf_cw_18[];
#endif

#ifdef _RF_OBU_
extern const unsigned int bk5823_rf_obu[];
extern const unsigned char bk5823_rf_obu_17[];
extern const unsigned char bk5823_rf_obu_18[];
#endif /*_RF_OBU_*/
#ifdef _RF_RSU_
extern const unsigned int bk5823_rf_rsu[];
extern const unsigned char bk5823_rf_rsu_17[];
extern const unsigned char bk5823_rf_rsu_18[];
#endif /*_RF_OBU_*/


#ifdef _RF_TRx_OBU_
extern const unsigned int bk5823_rf_obuTRx[];
extern const unsigned char bk5823_rf_obuTRx_17[];
extern const unsigned char bk5823_rf_obuTRx_18[];
#endif /*_RF_TRx_OBU_*/


#endif /*_BK5824RegMem_H_*/
