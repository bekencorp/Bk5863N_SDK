#include "dsrc_app_intf.h"
#include "icc_m1.h"


int bk_rsu_obu_icc_read(unsigned char ops,unsigned char *msg,int len)
{
	return icc_m1_read((char)ops,msg,len);
}




