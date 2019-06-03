#ifndef _SL_DES_H_
#define _SL_DES_H_

#define SL_DES_CRYPTION_SOFTWARE_CFG                 1
#define SL_DES_CRYPTION_HARDWARE_CFG                 0

#define SL_DES_DECRYPTION			                 0
#define SL_DES_ENCRYPTION			                 1



void des_cryption(char mode,char *input,char* key, char *output);

#endif

/***********************  END OF FILES  ***********************/

