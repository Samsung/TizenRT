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
#include <string.h>
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
#include <psa/crypto.h>
#include <psa/crypto_extra.h>
#include <os/os.h>

#include <psa/crypto.h>

#include "tfm_dubhe_key_ladder_nsc.h"

#include "_otp.h"
#if defined(CONFIG_TFM_AES_GCM_NSC)
#include "tfm_aes_gcm_nsc.h"
#endif

#define APP_SUCCESS                                            (0)
#define APP_ERROR                                              (-1)
#define APP_SUCCESS_MESSAGE "Example finished successfully!"
#define APP_ERROR_MESSAGE   "Example exited with error!"
#define CRYPTO_EXAMPLE_HMAC_TEXT_SIZE                          (100)
#define CRYPTO_EXAMPLE_HMAC_KEY_SIZE                           (32)
#define TAG "mbedtls"

#define TZ_SYMMETRIC_KEY_IDX                                   (1)
#define TZ_SOFTAP_KEY_IDX                                      (2)
#define TZ_ASYMMETRIC_KEY_IDX                                  (3)


#define ASY_KEYGEN_ECP                                         (1)
#define BIT_SIZE_AES_128                                       (128)
#define BIT_SIZE_AES_256                                       (256)
#define RSA_KEY_SIZE_128                                       (128)
#define RSA_KEY_SIZE_256                                       (256)
#define AES_KEY_BIT_PER_BYTE                                   (8)

#define CRYPTO_EXAMPLE_AES_MAX_TEXT_SIZE                       (64)
#define CRYPTO_EXAMPLE_AES_BLOCK_SIZE                          (16)
#define CRYPTO_RSA_PUBLIC_KEY_SIZE                             (65)
#define HAL_TZ_PK_KEY_COUNT                                    (2)

#define TZ_ENC_DEC_TIMEOUT                                     (5000) //5s

#define PSA_KEY_NUM (sizeof(s_psa_key_attr_cfg)/sizeof(key_attributes_t))


typedef struct
{
	psa_key_id_t id;
	uint16_t type;
	uint16_t bits;
	uint32_t usage;
	uint32_t alg;
}key_attributes_t;

enum psa_aes_key_id_e {
	PSA_KEY_ID_1 = 1,
	PSA_KEY_ID_2,
	PSA_KEY_ID_3,
	PSA_KEY_ID_MAX,        
};
typedef struct
{
	psa_key_id_t keypair_id;
	uint32_t     keytype;
	uint8_t      pub_key[CRYPTO_RSA_PUBLIC_KEY_SIZE];
} hal_tz_pk_key_t;


static key_attributes_t s_psa_key_attr_cfg[]       =
{
	/* id           type                         bits              usage                                         alg                         */
	{PSA_KEY_ID_1, PSA_KEY_TYPE_AES,          BIT_SIZE_AES_128, PSA_KEY_USAGE_ENCRYPT | PSA_KEY_USAGE_DECRYPT, PSA_ALG_CBC_NO_PADDING           },
	{PSA_KEY_ID_2, PSA_KEY_TYPE_AES,          BIT_SIZE_AES_256, PSA_KEY_USAGE_ENCRYPT | PSA_KEY_USAGE_DECRYPT, PSA_ALG_CBC_NO_PADDING           },
	{PSA_KEY_ID_3, PSA_KEY_TYPE_RSA_KEY_PAIR, RSA_KEY_SIZE_128, PSA_KEY_USAGE_ENCRYPT | PSA_KEY_USAGE_DECRYPT, PSA_ALG_RSA_OAEP(PSA_ALG_SHA_256)}
};

#define CONFIG_AES_CBC_MUTEX_PROTECT       (0)
#define CONFIG_AES_GCM_MUTEX_PROTECT       (0)

#if defined(CONFIG_AES_CBC_MUTEX_PROTECT)
static beken_mutex_t s_mutex_aes_cbc_enc;
static beken_mutex_t s_mutex_aes_cbc_dec;
#endif
#if defined(CONFIG_AES_GCM_MUTEX_PROTECT)
static beken_mutex_t s_mutex_aes_gcm_enc;
static beken_mutex_t s_mutex_aes_gcm_dec;
#endif

static int s_aes_key_id;

psa_status_t hal_tz_generate_key(uint32_t key_id, uint16_t type, uint16_t bits, uint32_t usage, uint32_t alg)
{
	psa_status_t status;

	BK_LOGI(TAG, "Generating random AES key...\r\n");

	status = psa_crypto_init();
	if (status != APP_SUCCESS) {
		BK_LOGE(TAG, "psa_crypto_init\r\n");
		return APP_ERROR;
	}

	/* Configure the key attributes */
	psa_key_attributes_t key_attributes = PSA_KEY_ATTRIBUTES_INIT;

	psa_set_key_usage_flags(&key_attributes, usage);
	psa_set_key_lifetime(&key_attributes, PSA_KEY_LIFETIME_VOLATILE);
	psa_set_key_algorithm(&key_attributes, alg);
	/*set key type*/
	if (PSA_ALG_IS_RSA_OAEP(alg) || alg == PSA_ALG_RSA_PKCS1V15_CRYPT)
	{
        psa_set_key_type(&key_attributes, PSA_KEY_TYPE_RSA_KEY_PAIR);
	}
	else
	{
		psa_set_key_type(&key_attributes, type);
	}

	psa_set_key_bits(&key_attributes, bits);
	psa_set_key_id(&key_attributes, key_id );

	/* Generate a random key. The key is not exposed to the application,
	 * we can use it to encrypt/decrypt using the key handle
	 */
	status = psa_generate_key(&key_attributes, &key_id);
	if (status != PSA_SUCCESS) {
		BK_LOGE(TAG, "psa_generate_key failed! (Error: %d)\r\n", status);
		return APP_ERROR;
	}

	/* After the key handle is acquired the attributes are not needed */
	psa_reset_key_attributes(&key_attributes);

	BK_LOGI(TAG, "AES key generated successfully keyid = 0x%x \r\n", key_id);

	return APP_SUCCESS;
}

int hal_tz_crypto_init()
{
	psa_status_t status;
	/* Initialize PSA Crypto */
	status = psa_crypto_init();
	if (status != PSA_SUCCESS)
		return APP_ERROR;

	return APP_SUCCESS;
}
int hal_tz_generate_key_init()
{
	psa_key_attributes_t key_attributes = PSA_KEY_ATTRIBUTES_INIT;
	key_attributes_t *key_attr;
	psa_status_t status;

	for(int i = 0; i < PSA_KEY_NUM; i++)
	{
		key_attr = &s_psa_key_attr_cfg[i];
		status = psa_get_key_attributes(key_attr->id, &key_attributes);

		if(status != PSA_SUCCESS)
		{
			//generate key
			status = hal_tz_generate_key(key_attr->id, key_attr->type, key_attr->bits, key_attr->usage, key_attr->alg);
			if(status != PSA_SUCCESS)
			{
				BK_LOGE(TAG, "AES key generated failed, keyid = 0x%x ,status = 0x%x\r\n", key_attr->id, status);
				return APP_ERROR;
			}
		}
		else
		{
			psa_key_id_t    id      = psa_get_key_id(&key_attributes);
			psa_key_type_t  type    = psa_get_key_type(&key_attributes);
			size_t          bits    = psa_get_key_bits(&key_attributes);
			psa_key_lifetime_t life = psa_get_key_lifetime(&key_attributes);
			psa_key_usage_t usage   = psa_get_key_usage_flags(&key_attributes);
			psa_algorithm_t alg     = psa_get_key_algorithm(&key_attributes);

			BK_LOGI(TAG, "key 0x%x already exist, type = 0x%x,bits = 0x%x, usage = 0x%x, alg = 0x%x, life = 0x%x\r\n",
				id, type, bits, usage, alg, life);
		}
	}

	return APP_SUCCESS;
}

/*-----------------------symmetric key  start----------------------------------------*/
int hal_tz_symmetric_keygen(int len)
{
	psa_status_t status;
	psa_key_id_t key_id;

	BK_LOGI(TAG, "Generating random AES key...\r\n");

	status = psa_crypto_init();
	if (status != APP_SUCCESS) {
		BK_LOGE(TAG, "psa_crypto_init\r\n");
		return APP_ERROR;
	}

	/* Configure the key attributes */
	psa_key_attributes_t key_attributes = PSA_KEY_ATTRIBUTES_INIT;

	psa_set_key_usage_flags(&key_attributes, PSA_KEY_USAGE_ENCRYPT | PSA_KEY_USAGE_DECRYPT);
	psa_set_key_lifetime(&key_attributes, PSA_KEY_LIFETIME_VOLATILE);
	psa_set_key_algorithm(&key_attributes, PSA_ALG_CBC_NO_PADDING);

	psa_set_key_type(&key_attributes, PSA_KEY_TYPE_AES);


	psa_set_key_bits(&key_attributes, AES_KEY_BIT_PER_BYTE*len);
	//psa_set_key_id(&key_attributes, key_id );

	/* Generate a random key. The key is not exposed to the application,
	 * we can use it to encrypt/decrypt using the key handle
	 */
	status = psa_generate_key(&key_attributes, &key_id);
	if (status != PSA_SUCCESS) {
		BK_LOGE(TAG, "psa_generate_key failed! (Error: %d)\r\n", status);
		return APP_ERROR;
	}

	/* After the key handle is acquired the attributes are not needed */
	psa_reset_key_attributes(&key_attributes);

	BK_LOGI(TAG, "AES key generated successfully keyid = 0x%x \r\n", key_id);

	return key_id;
}
int hal_tz_mbedtls_aes_init(mbedtls_aes_context *ctx)
{
	return hal_tz_crypto_init();
}
int hal_tz_mbedtls_aes_setkey_enc(mbedtls_aes_context *ctx, const unsigned char *key, unsigned int keybits)
{
	s_aes_key_id = (int)key[0];
	BK_LOGI(TAG, "%s[%d]\r\n",__func__,s_aes_key_id);
	return 0;
}
int hal_tz_mbedtls_aes_setkey_dec(mbedtls_aes_context *ctx, const unsigned char *key, unsigned int keybits)
{
	s_aes_key_id = (int)key[0];
	BK_LOGI(TAG, "%s[%d]\r\n",__func__,s_aes_key_id);
	return 0;
}

int hal_tz_aes_crypt_cbc(mbedtls_aes_context *ctx, 
	                             size_t length,
	                             unsigned char iv[16],
	                             const unsigned char *input,
								 unsigned char *output )
{
	uint32_t olen;
	psa_status_t status;
	psa_cipher_operation_t operation = PSA_CIPHER_OPERATION_INIT;

	BK_LOGI(TAG, "Encrypting using AES CBC MODE...\r\n");
	BK_LOGI(TAG, "%s[i:%s][l:%d][o:%d][id:%d]\r\n",__func__,input,sizeof(input),sizeof(output),s_aes_key_id);
	/* Setup the encryption operation */
	status = psa_cipher_encrypt_setup(&operation, s_aes_key_id, PSA_ALG_CBC_NO_PADDING);
	if (status != PSA_SUCCESS) {
		BK_LOGI(TAG, "psa_cipher_encrypt_setup failed! (Error: %d)\r\n", status);
		return APP_ERROR;
	}
	/* Set the IV generated in encryption */
	status = psa_cipher_set_iv(&operation, iv, 16);
	if (status != PSA_SUCCESS) {
		BK_LOGI(TAG, "psa_cipher_set_iv failed! (Error: %d)\r\n", status);
		return APP_ERROR;
	}
	/* Generate an IV */
	#if 0
	status = psa_cipher_generate_iv(&operation, m_iv, sizeof(m_iv), (size_t *)&olen);
	if (status != PSA_SUCCESS) {
		BK_LOGI(TAG, "psa_cipher_generate_iv failed! (Error: %d)\r\n", status);
		return APP_ERROR;
	}

	for(int i = 0; i < CRYPTO_EXAMPLE_AES_BLOCK_SIZE; i++)
	{
		//BK_LOGI(TAG, "%s[0x%x][%d]\r\n",__func__,m_iv[i],olen);
	}
	#endif
	/* Perform the encryption */
	status = psa_cipher_update(&operation, 
								input,
				   				length, 
				   				output,
				   				length, 
				   				(size_t *)&olen);
	if (status != PSA_SUCCESS)
	{
		BK_LOGI(TAG, "psa_cipher_update failed! (Error: %d)\r\n", status);
		return APP_ERROR;
	}
	BK_LOGI(TAG, "%s[%s][%d]\r\n",__func__,output,olen);
	/* Finalize the encryption */
	status = psa_cipher_finish(&operation, 
								output + olen,
							    length - olen,
							   (size_t *)&olen);
	if (status != PSA_SUCCESS) {
		BK_LOGI(TAG, "psa_cipher_finish failed! (Error: %d)\r\n", status);
		return APP_ERROR;
	}
	BK_LOGI(TAG, "%s[%s][%d]\r\n",__func__,output,olen);
	BK_LOGI(TAG, "Encryption successful!\r\n");

	/* Clean up cipher operation context */
	psa_cipher_abort(&operation);

	return APP_SUCCESS;
}
int hal_tz_aes_decrypt_cbc(mbedtls_aes_context *ctx, 
	                               size_t length,
	                               unsigned char iv[16],
	                               const unsigned char *input,
								   unsigned char *output )
{
	
	uint32_t olen;
	psa_status_t status;
	psa_cipher_operation_t operation = PSA_CIPHER_OPERATION_INIT;

	BK_LOGI(TAG, "Decrypting using AES CBC MODE...\r\n");

	/* Setup the decryption operation */
	status = psa_cipher_decrypt_setup(&operation, s_aes_key_id, PSA_ALG_CBC_NO_PADDING);
	if (status != PSA_SUCCESS) {
		BK_LOGI(TAG, "psa_cipher_decrypt_setup failed! (Error: %d)\r\n", status);
		return APP_ERROR;
	}

	/* Set the IV generated in encryption */
	//status = psa_cipher_set_iv(&operation, m_iv, sizeof(m_iv));
	status = psa_cipher_set_iv(&operation, iv, 16);
	if (status != PSA_SUCCESS) {
		BK_LOGI(TAG, "psa_cipher_set_iv failed! (Error: %d)\r\n", status);
		return APP_ERROR;
	}

	/* Perform the decryption */
	status = psa_cipher_update(&operation, input,
				   length, output,
				   length, (size_t *)&olen);
	if (status != PSA_SUCCESS) {
		BK_LOGI(TAG, "psa_cipher_update failed! (Error: %d)\r\n", status);
		return APP_ERROR;
	}

	/* Finalize the decryption */
	status = psa_cipher_finish(&operation, output + olen,
				   length - olen,
				   (size_t *)&olen);
	if (status != PSA_SUCCESS) {
		BK_LOGI(TAG, "psa_cipher_finish failed! (Error: %d)\r\n", status);
		return APP_ERROR;
	}

	BK_LOGI(TAG, "Decryption successful!\r\n");

	/*  Clean up cipher operation context */
	psa_cipher_abort(&operation);

	return APP_SUCCESS;
}


/*-----------------------asymmetric key  start----------------------------------------*/

static hal_tz_pk_key_t s_pk_key;


int hal_tz_pk_init(mbedtls_pk_context *ctx)
{
#if 0
	if(ctx != NULL)
	{
		mbedtls_pk_init(ctx);
	}
	else
	{
		return APP_ERROR;
	}

#else
	psa_status_t status;

	/* Initialize PSA Crypto */
	status = psa_crypto_init();
	if (status != PSA_SUCCESS)
		return APP_ERROR;

	return APP_SUCCESS;

#endif
}

void hal_tz_pk_free(mbedtls_pk_context *ctx)
{
	//mbedtls_pk_free(ctx);
	mbedtls_psa_crypto_free();

}

int	hal_tz_asymmetric_keygen(mbedtls_pk_context *pk_key, int key_idx, int type)
{
	psa_status_t status;
	size_t olen;
	//psa_key_id_t keypair_id;

	BK_LOGI(TAG, "Generating random asymmetric keypair...\r\n");

	/* Configure the key attributes */
	psa_key_attributes_t key_attributes = PSA_KEY_ATTRIBUTES_INIT;

	/* Configure the key attributes */
	psa_set_key_usage_flags(&key_attributes, PSA_KEY_USAGE_SIGN_HASH);
	psa_set_key_lifetime(&key_attributes, PSA_KEY_LIFETIME_VOLATILE);
	psa_set_key_algorithm(&key_attributes, PSA_ALG_RSA_OAEP(PSA_ALG_SHA_256));
	psa_set_key_type(&key_attributes, PSA_KEY_TYPE_RSA_KEY_PAIR);
	psa_set_key_bits(&key_attributes, RSA_KEY_SIZE_256);

	/* Generate a random keypair. The keypair is not exposed to the application,
	 * we can use it to sign hashes.
	 */
	status = psa_generate_key(&key_attributes, &s_pk_key.keypair_id);
	if (status != PSA_SUCCESS) 
	{
		BK_LOGI(TAG, "psa_generate_key failed! (Error: %d)\r\n", status);
		return APP_ERROR;
	}

	/* Export the public key */
	status = psa_export_public_key(s_pk_key.keypair_id, s_pk_key.pub_key, sizeof(s_pk_key.pub_key), &olen);
	if (status != PSA_SUCCESS) 
	{
		BK_LOGI(TAG, "psa_export_public_key failed! (Error: %d)\r\n", status);
		return APP_ERROR;
	}

	/* Reset key attributes and free any allocated resources. */
	psa_reset_key_attributes(&key_attributes);

	return APP_SUCCESS;
}

/*parse privae key*/
int hal_tz_pk_parse_key(mbedtls_pk_context *pk,
					            char *key, 
					            size_t keylen,
					            const unsigned char *pwd, 
					            size_t pwdlen)
{
	//mbedtls_pk_parse_key(pk,(const unsigned char *)key,keylen,pwd,pwdlen,NULL, NULL);

	//psa_import_key();
	return 0;

}

int hal_tz_pk_encrypt(mbedtls_pk_context *ctx,
				            const unsigned char *input, 
				            size_t ilen,
				            unsigned char *output, 
				            size_t *olen, 
				            size_t osize,
				            int (*f_rng)(void *, unsigned char *, size_t),
				            void *p_rng)
{
	psa_status_t status  = PSA_SUCCESS;
	size_t output_length = 0;

	status = psa_asymmetric_encrypt(s_pk_key.keypair_id, 
									PSA_ALG_RSA_OAEP(PSA_ALG_SHA_256), 
									input,
									ilen,
									NULL, 
									0, 
									output,
									*olen, 
									&output_length);

    if (status != PSA_SUCCESS) 
	{
        //TEST_FAIL("asymmetric:failed to encrypt status :0x%x",status);
    }

	return status;
}

int hal_tz_pk_decrypt(mbedtls_pk_context *ctx,
				            const unsigned char *input, 
				            size_t ilen,
				            unsigned char *output, 
				            size_t *olen, 
				            size_t osize,
				            int (*f_rng)(void *, unsigned char *, size_t),
				            void *p_rng)
{
	psa_status_t status ;
	size_t output_length = 0;

	status = psa_asymmetric_decrypt(s_pk_key.keypair_id, 
									PSA_ALG_ECDSA(PSA_ALG_SHA_256), 
									input,
									ilen,
									NULL, 
									0, 
									output,
									osize, 
									&output_length);

    if (status != PSA_SUCCESS) 
	{
        //TEST_FAIL("asymmetric :failed to decrypt status :0x%x",status);
    }

	return status;
}

int hal_tz_pk_sign(mbedtls_pk_context *ctx, 
							mbedtls_md_type_t md_alg,
			                const unsigned char *hash, 
			                size_t hash_len,
			                unsigned char *sig, 
			                size_t *sig_len,
			                int (*f_rng)(void *, unsigned char *, size_t),
			                void *p_rng)
{    
	psa_status_t status;

	size_t signature_length = 0;

	status = psa_sign_hash(s_pk_key.keypair_id, 
						PSA_ALG_ECDSA(PSA_ALG_SHA_256), 
						hash, 
						hash_len, 
						sig, 
						*sig_len, 
						&signature_length);
						

    if (status != PSA_SUCCESS)
	{
        //TEST_FAIL("asymmetric :failed to sign status :0x%x",status);
    }

	return status;
}

int hal_tz_get_public_key( int index, unsigned char *buf, size_t size )
{
	psa_status_t status;
	size_t olen;
	
	status = psa_export_public_key(s_pk_key.keypair_id,  buf, size, &olen);
	if(status == PSA_SUCCESS)
	{
		return APP_SUCCESS;
	}
	else
	{
		return APP_ERROR;
	}
}

int hal_tz_get_private_key( int index, unsigned char *buf, size_t size )
{

	return APP_SUCCESS;
}

int hal_tz_pk_write_pubkey_pem( mbedtls_pk_context *key, unsigned char *buf, size_t size )
{	
	if((key != NULL)&&( buf != NULL))
	{
		mbedtls_pk_write_key_pem(key,buf,size);
		
		return APP_SUCCESS;
	}
	else
	{
		return APP_ERROR;
	}
}

int hal_tz_generate_random(unsigned char *output, size_t output_len)

{
	psa_status_t status;

	status = psa_generate_random(output,output_len);
	if(status == PSA_SUCCESS)
	{
		return APP_SUCCESS;
	}
	else
	{
		return APP_ERROR;
	}
}
static uint8_t input_iv[16] __attribute__((aligned(32))) = {
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
	0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};
extern beken_mutex_t ns_lock_handle;

int aes_cbc_encrypt_data(uint8_t *input, uint32_t input_len, uint8_t *output)
{
    int ret =0;

    while (rtos_lock_mutex(&ns_lock_handle) != BK_OK);
    GLOBAL_INT_DECLARATION();
    GLOBAL_INT_DISABLE();

    key_ladder_context_t ctx;
    ctx.key_id = OTP_EK_1;
    ctx.mode = AES_OPERATION_ENCRYPT;
    memcpy(ctx.iv, input_iv, 16);
    ret = dubhe_key_ladder_crypt_aes_cbc(&ctx, input_len, input, output);

    GLOBAL_INT_RESTORE();
    rtos_unlock_mutex(&ns_lock_handle);

    if(ret < 0)
    {
        BK_LOGE(TAG, "%s enc fail! (Error: %d)\r\n",__func__,ret);
        return -1;
    }
    return 0;
}

int aes_cbc_decrypt_data(uint8_t *input, uint32_t input_len, uint8_t *output)
{
    int ret =0;

    while (rtos_lock_mutex(&ns_lock_handle) != BK_OK);
    GLOBAL_INT_DECLARATION();
    GLOBAL_INT_DISABLE();

    key_ladder_context_t ctx;
    ctx.key_id = OTP_EK_1;
    ctx.mode = AES_OPERATION_DECRYPT;
    memcpy(ctx.iv, input_iv, 16);

    ret = dubhe_key_ladder_crypt_aes_cbc(&ctx, input_len, input, output);

    GLOBAL_INT_RESTORE();
    rtos_unlock_mutex(&ns_lock_handle);

    if(ret < 0)
    {
        BK_LOGE(TAG, "%s fail! (Error: %d)\r\n",__func__,ret);
        return -1;
    }
    return 0;
}

/************************************************************
Descriptions : Decrypt input data contaning ciphertext.
arguments
- pbuf: output data containing ciphertext.
- pOutBuf : decrypted data
- len : input data buffer size, must be 16bytes aligned
************************************************************/

int hal_tz_aes_gcm_decrypt_data(const uint8_t * pBuf, const uint32_t len,const uint8_t *pOutBuf)
{
    int ret = -1;
#if defined(CONFIG_TFM_AES_GCM_NSC)
    while (rtos_lock_mutex(&ns_lock_handle) != BK_OK);
    GLOBAL_INT_DECLARATION();
    GLOBAL_INT_DISABLE();

    aes_gcm_context_t aes_gcm_context;
    aes_gcm_context.item_id = OTP_RANDOM_KEY_1;
    aes_gcm_context.out_length = len;

    ret = psa_aes_gcm_decrypt(&aes_gcm_context, (unsigned char*)pBuf, len, (unsigned char*)pOutBuf);

    GLOBAL_INT_RESTORE();
    rtos_unlock_mutex(&ns_lock_handle);

    if (ret != PSA_SUCCESS)
    {
        BK_LOGE(TAG, "aes_gcm dec failed! (Error: %d)\r\n",ret);
        return ret;
    }
#endif
    //printf("%ssuccessful!\r\n",__func__);
    return ret;
}

/************************************************************
Descriptions : The data passed from the caller is encrypted by the Secure Storage procedure and store in Flash(PTM).
arguments
- pBuf : Plain data to be encrypted and stored.
- pOutBuf : encrypted data
- len : input data size, must be 16bytes aligned
************************************************************/
int hal_tz_aes_gcm_encrypt_data(const uint8_t * const pBuf, const uint32_t len, const uint8_t *pOutBuf)
{
    int ret = -1;
#if defined(CONFIG_TFM_AES_GCM_NSC)
    while (rtos_lock_mutex(&ns_lock_handle) != BK_OK);
    GLOBAL_INT_DECLARATION();
    GLOBAL_INT_DISABLE();

    aes_gcm_context_t aes_gcm_context;
    aes_gcm_context.item_id = OTP_RANDOM_KEY_1;
    aes_gcm_context.out_length = len;

    ret = psa_aes_gcm_encrypt(&aes_gcm_context, (unsigned char*)pBuf, len, (unsigned char*)pOutBuf);

    GLOBAL_INT_RESTORE();
    rtos_unlock_mutex(&ns_lock_handle);

    if (ret != PSA_SUCCESS)
    {
        BK_LOGE(TAG, "aes_gcm enc failed! (Error: %d)\r\n",ret);
        return ret;
    }
#endif
    //printf("%s successful!\r\n",__func__);
    return ret;
}
