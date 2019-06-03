#include "sl_des.h"


#if SL_DES_CRYPTION_SOFTWARE_CFG
#include "sw_des.h"
#endif



/*
 * DES  encryption/decryption
 * @param mode:1 encryption;0:decryption
 * 仅做8字节的加解密
 *
 */
void des_cryption(char mode,char *input,char* key, char *output)
{
#if SL_DES_CRYPTION_SOFTWARE_CFG
	des_context ctx;
	
	if(!output)   return;
	if(mode == SL_DES_ENCRYPTION)
	{
		//encrypt standard testing vector
		des_setkey_enc(&ctx, (unsigned char*)key);
	}
	else
	{
		//decrypt testing
		des_setkey_dec(&ctx, (unsigned char*)key);
	}
	des_crypt_ecb(&ctx, (unsigned char*)input, (unsigned char*)output);
#elif SL_DES_CRYPTION_HARDWARE_CFG
	
#endif
}





