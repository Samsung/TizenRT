// Copyright 2024-2025 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <stdio.h>
#include <stdlib.h>
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
#include "hal_tz.h"

int tz_key_mgt_init(void)
{
	hal_tz_crypto_init();
	hal_tz_generate_key_init();
	return 0;
}
int tz_key_mgt_deinit(void)
{
	return 0;
}
int tz_key_storage_erase(void)
{
	return 0;
}
/*-----------------------symmetric key  start----------------------------------------*/
int tz_symmetric_keygen(int len)
{
	return hal_tz_symmetric_keygen(len);
}
int tz_mbedtls_aes_init(mbedtls_aes_context *ctx)
{
	return hal_tz_mbedtls_aes_init(ctx);
}
int tz_mbedtls_aes_free(mbedtls_aes_context *ctx)
{
	return 0;
}
int tz_mbedtls_aes_setkey_enc(mbedtls_aes_context *ctx, const unsigned char *key, unsigned int keybits)
{
	return hal_tz_mbedtls_aes_setkey_enc(ctx,key,keybits);
}
int tz_mbedtls_aes_setkey_dec(mbedtls_aes_context *ctx, const unsigned char *key, unsigned int keybits)
{
	return hal_tz_mbedtls_aes_setkey_dec(ctx,key,keybits);
}

int tz_mbedtls_aes_crypt_cbc(mbedtls_aes_context *ctx,
									    int mode,
									    size_t length,
									    unsigned char iv[16],
									    const unsigned char *input,
									    unsigned char *output )
{
	int ret = 0;
	if(mode == MBEDTLS_AES_ENCRYPT)
	{
 		ret = hal_tz_aes_crypt_cbc( ctx, length,&iv[0],input,output );
	}
	else
	{
		ret = hal_tz_aes_decrypt_cbc( ctx, length,&iv[0],input,output );
	}
	return ret;
}
int tz_set_symmetric_key( int index, unsigned char * key )
{
	return 0;
}
int tz_set_symmetric_key_with_len( int index, unsigned char * key, int len )
{
	return 0;
}

/*-----------------------symmetric key  end-------------------------------------------*/
/*-----------------------asymmetric key  start----------------------------------------*/
int tz_mbedtls_asymmetric_keygen(mbedtls_pk_context *pk_key, int key_idx, int type)
{
	return hal_tz_asymmetric_keygen(pk_key,key_idx,type);
}
void tz_mbedtls_pk_init(mbedtls_pk_context *ctx)
{
	hal_tz_pk_init(ctx);
}
void tz_mbedtls_pk_free(mbedtls_pk_context *ctx)
{
	hal_tz_pk_free(ctx);
}

int tz_mbedtls_pk_sign(mbedtls_pk_context *ctx, 
							mbedtls_md_type_t md_alg,
			                const unsigned char *hash, 
			                size_t hash_len,
			                unsigned char *sig, 
			                size_t *sig_len,
			                int (*f_rng)(void *, unsigned char *, size_t),
			                void *p_rng)
{
	return hal_tz_pk_sign(ctx,md_alg,hash,hash_len,sig,sig_len,f_rng,p_rng);
}
char * tz_key_mgt_get_client_key(int key_idx)
{
	return 0;
}
/*parse privae key*/
int tz_mbedtls_pk_parse_key(mbedtls_pk_context *pk,
					                char *key, 
					                size_t keylen,
					                const unsigned char *pwd, 
					                size_t pwdlen)
{
	
	return hal_tz_pk_parse_key(pk,key,keylen,pwd,pwdlen);
}
/*using the private key decrypt*/
int tz_mbedtls_pk_decrypt(mbedtls_pk_context *ctx,
				                  const unsigned char *input, 
				                  size_t ilen,
				                  unsigned char *output, 
				                  size_t *olen, 
				                  size_t osize,
				                  int (*f_rng)(void *, unsigned char *, size_t),
				                  void *p_rng)
{
	
	return hal_tz_pk_decrypt(ctx,input,ilen,output,olen,osize,f_rng,p_rng);
}
int tz_set_private_key( int index, unsigned char * key )
{
	return 0;
}
int tz_set_public_key( int index, unsigned char *key )
{
	return 0;
}
int tz_get_public_key( int index, unsigned char *buf, size_t size )
{
	return 0;
}
int tz_get_private_key( int index, unsigned char *buf, size_t size )
{
	return 0;
}
/*-----------------------asymmetric key  end----------------------------------------*/

/*--------------------------------csr  start----------------------------------------*/
int tz_mbedtls_ssl_conf_own_cert(mbedtls_ssl_config *conf,
                                			mbedtls_x509_crt *own_cert,
                                			int pk_key)
{
	return 0;
}
int tz_mbedtls_x509write_csr_set_key(mbedtls_x509write_csr *csr_ctx, mbedtls_pk_context *pk_ctx)
{
	return 0;
}
int tz_mbedtls_x509write_csr_pem( mbedtls_x509write_csr *ctx, unsigned char *buf, size_t size,
		                       int (*f_rng)(void *, unsigned char *, size_t),
		                       void *p_rng )
{
	return 0;
}
int tz_mbedtls_pk_write_pubkey_pem( mbedtls_pk_context *key, unsigned char *buf, size_t size )
{
	return hal_tz_pk_write_pubkey_pem(key,buf,size);
}
/*--------------------------------csr  end----------------------------------------*/

int my_random(void *p_rng, unsigned char *output, size_t output_len)
{
	hal_tz_generate_random(output,output_len);
	return 0;
}

