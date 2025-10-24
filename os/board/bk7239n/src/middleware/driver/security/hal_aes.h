#ifndef _BK_AES_H_
#define _BK_AES_H_

#if (defined(CONFIG_SOC_BK7251))

enum AES_MODE {
	AES128      = 0,
	AES256      = 1,
	AES192      = 2
};

enum AES_ENCODE {
	DECODE      = 0,
	ENCODE      = 1
};

void hal_aes_init(void *ctx);
int hal_aes_setkey_dec(void *ctx, const unsigned char *key,
					   unsigned int keybits);
int hal_aes_setkey_enc(void *ctx, const unsigned char *key,
					   unsigned int keybits);
int hal_aes_crypt_ecb(void *ctx,
					  int mode,
					  const unsigned char input[16],
					  unsigned char output[16]);

void hal_aes_free(void *ctx);
#endif
#endif

