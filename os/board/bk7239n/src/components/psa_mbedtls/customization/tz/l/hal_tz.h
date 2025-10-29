#ifndef HAL_TZ_H_
#define HAL_TZ_H_
#include "mbedtls/debug.h"
#include "mbedtls/ssl.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/error.h"
#include "mbedtls/threading.h"
#include <mbedtls/pk.h>
#include <mbedtls/ecp.h>
#include <mbedtls/x509_csr.h>
#include <mbedtls/pem.h>
#include <mbedtls/platform.h>

int hal_tz_mbedtls_aes_init(mbedtls_aes_context *ctx);

int hal_tz_mbedtls_aes_setkey_enc(mbedtls_aes_context *ctx, const unsigned char *key, unsigned int keybits);

int hal_tz_mbedtls_aes_setkey_dec(mbedtls_aes_context *ctx, const unsigned char *key, unsigned int keybits);

int hal_tz_aes_crypt_cbc(mbedtls_aes_context *ctx, 
	                             size_t length,
	                             unsigned char iv[16],
	                             const unsigned char *input,
								 unsigned char *output );

int hal_tz_aes_decrypt_cbc(mbedtls_aes_context *ctx, 
	                               size_t length,
	                               unsigned char iv[16],
	                               const unsigned char *input,
								   unsigned char *output );

psa_status_t hal_tz_generate_key(uint32_t key_id, uint16_t type, uint16_t bits, uint32_t usage, uint32_t alg);

int hal_tz_crypto_init();

int hal_tz_generate_key_init();

int hal_tz_symmetric_keygen(int len);

int	hal_tz_asymmetric_keygen(mbedtls_pk_context *pk_key, int key_idx, int type);

int hal_tz_pk_parse_key(mbedtls_pk_context *pk,
					            char *key, 
					            size_t keylen,
					            const unsigned char *pwd, 
					            size_t pwdlen);

int hal_tz_pk_decrypt(mbedtls_pk_context *ctx,
				      const unsigned char *input, 
				      size_t ilen,
				      unsigned char *output, 
				      size_t *olen, 
				      size_t osize,
				      int (*f_rng)(void *, unsigned char *, size_t),
				      void *p_rng);
int hal_tz_pk_sign(mbedtls_pk_context *ctx, 
						mbedtls_md_type_t md_alg,
			            const unsigned char *hash, 
			            size_t hash_len,
			            unsigned char *sig, 
			            size_t *sig_len,
			            int (*f_rng)(void *, unsigned char *, size_t),
			            void *p_rng);
int hal_tz_generate_random(unsigned char *output, size_t output_len);


int hal_tz_pk_init(mbedtls_pk_context *ctx);
void hal_tz_pk_free(mbedtls_pk_context *ctx);
int hal_tz_pk_write_pubkey_pem( mbedtls_pk_context *key, unsigned char *buf, size_t size );
/************************************************************
Descriptions : Decrypt input data contaning ciphertext.
arguments,using the random key
- pbuf: output data containing ciphertext.
- pOutBuf : decrypted data
- len : input data buffer size, must be 16bytes aligned
************************************************************/
int hal_tz_aes_gcm_decrypt_data(const uint8_t * pBuf, const uint32_t len,const uint8_t *pOutBuf);

/************************************************************
Descriptions : The data passed from the caller is encrypted by the Secure Storage procedure and store in Flash(PTM).
arguments,using the random key
- pBuf : Plain data to be encrypted and stored.
- pOutBuf : encrypted data
- len : input data size, must be 16bytes aligned
************************************************************/
int hal_tz_aes_gcm_encrypt_data(const uint8_t * const pBuf, const uint32_t len, const uint8_t *pOutBuf);
#endif /* HAL_TZ_H_ */

