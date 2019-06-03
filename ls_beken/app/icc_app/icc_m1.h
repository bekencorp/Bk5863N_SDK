#ifndef _ICC_M1_H_
#define _ICC_M1_H_



int icc_m1_select_card(void);
int icc_m1_read(char ops,unsigned char *msg,int len);
int icc_m1_write(char ops,unsigned char *msg,int len);

#endif
