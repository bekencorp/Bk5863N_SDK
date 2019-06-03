#include "sw_des_test.h"

#if SW_DES_TEST_OPEN
#include "sw_des.h"
#include "stdio.h"
#include "string.h"

int sw_des_main(int argc, char** argv)
{
#if SW_SUPPORT_CFG
	unsigned char key[8] = {0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37};
	unsigned char input[8] = {0x30,0x31,0x30,0x35,0x32,0x30,0x31,0x39};
	unsigned char output[8];
	des_context ctx;
	char i;
	
	printf("\r\ndes key(H): ");
	for (i = 0; i < 8; i++)
		printf("%02x ", key[i]);
	printf("\n");
	
	printf("des ctx(H): ");
	for (i = 0; i < 8; i++)
		printf("%02x ", input[i]);
	printf("\n");
	
	des_setkey_enc(&ctx, key);
	des_crypt_ecb(&ctx, input, output);
	printf("des enc(H): ");
	for (i = 0; i < 8; i++)
		printf("%02x ", output[i]);
	printf("\n");
	
	des_setkey_dec(&ctx, key);
	des_crypt_ecb(&ctx, output, output);
	printf("des dec(H): ");
	for (i = 0; i < 8; i++)
		printf("%02x ", output[i]);
	printf("\n");
#endif
	return 0;
}

void sw_des_main_demo(void)
{
	sw_des_main(0,0);
}

#endif






