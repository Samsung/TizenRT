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

#include <string.h>
#include <os/os.h>
#include <os/mem.h>
#include "components/log.h"
#include "common/bk_include.h"
#include "psa/crypto.h"
#include "psa/crypto_extra.h"

#define TAG "mbedtls"
#define APP_SUCCESS    (0)
#define APP_ERROR      (-1)

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
	PSA_KEY_ID_MAX,        
};

#define BIT_SIZE_AES_128 (128)
#define BIT_SIZE_AES_256 (256)
#define BYTE_SIZE_IV     (16)

#define CRYPTO_EXAMPLE_AES_MAX_TEXT_SIZE (64)

/* Below text is used as plaintext for encryption/decryption */
static uint8_t m_plain_text[CRYPTO_EXAMPLE_AES_MAX_TEXT_SIZE] = {
	"Example string to demonstrate basic usage of AES CBC mode."
};

static uint8_t m_encrypted_text[CRYPTO_EXAMPLE_AES_MAX_TEXT_SIZE];
static uint8_t m_decrypted_text[CRYPTO_EXAMPLE_AES_MAX_TEXT_SIZE];

#define PSA_KEY_NUM (sizeof(psa_key_attr_cfg_g)/sizeof(key_attributes_t))
key_attributes_t psa_key_attr_cfg_g[] =
{
	/* id                    type                         bits                            usage                                                                           alg                                    */
	{PSA_KEY_ID_1, PSA_KEY_TYPE_AES, BIT_SIZE_AES_128, PSA_KEY_USAGE_ENCRYPT | PSA_KEY_USAGE_DECRYPT, PSA_ALG_CBC_NO_PADDING},
	{PSA_KEY_ID_2, PSA_KEY_TYPE_AES, BIT_SIZE_AES_256, PSA_KEY_USAGE_ENCRYPT | PSA_KEY_USAGE_DECRYPT, PSA_ALG_CBC_NO_PADDING}
};

static psa_status_t generate_aes_key(uint32_t key_id, uint16_t type, uint16_t bits, uint32_t usage, uint32_t alg)
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
	psa_set_key_type(&key_attributes, type);
	psa_set_key_bits(&key_attributes, bits);
	psa_set_key_id(&key_attributes, key_id);

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

int generate_key_manage(void)
{
	psa_key_attributes_t key_attributes = PSA_KEY_ATTRIBUTES_INIT;
	key_attributes_t *key_attr;
	psa_status_t status;

	for(int i = 0; i < PSA_KEY_NUM; i++)
	{
		key_attr = &psa_key_attr_cfg_g[i];
		status = psa_get_key_attributes(key_attr->id, &key_attributes);

		if(status != PSA_SUCCESS)
		{
			//generate key
			status = generate_aes_key(key_attr->id, key_attr->type, key_attr->bits, key_attr->usage, key_attr->alg);
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


int aes_encrypt_decrypt(void)
{
	psa_key_attributes_t key_attributes = PSA_KEY_ATTRIBUTES_INIT;
	key_attributes_t *key_attr;
	psa_status_t status;
	psa_cipher_operation_t operation = PSA_CIPHER_OPERATION_INIT;
	uint32_t olen;
	uint8_t aes_iv[BYTE_SIZE_IV];

	for(int i = 0; i < PSA_KEY_NUM; i++)
	{
		key_attr = &psa_key_attr_cfg_g[i];

		status = psa_get_key_attributes(key_attr->id, &key_attributes);
		if(status != PSA_SUCCESS)
		{
			BK_LOGE(TAG, "get AES key failed, keyid = 0x%x ,status = 0x%x\r\n", key_attr->id, status);
			return APP_ERROR;
		}

		/* Setup the encryption operation */
		status = psa_cipher_encrypt_setup(&operation, key_attr->id, key_attr->alg);
		if (status != PSA_SUCCESS) {
			BK_LOGE(TAG, "psa_cipher_encrypt_setup failed! (Error: %d)\r\n", status);
			return APP_ERROR;
		}

		/* Generate an IV */
		status = psa_cipher_generate_iv(&operation, aes_iv, BYTE_SIZE_IV, (size_t *)&olen);
		if (status != PSA_SUCCESS) {
			BK_LOGE(TAG, "psa_cipher_generate_iv failed! (Error: %d)\r\n", status);
			return APP_ERROR;
		}

		/* Perform the encryption */
		status = psa_cipher_update(&operation, m_plain_text,
			   sizeof(m_plain_text), m_encrypted_text, sizeof(m_encrypted_text), (size_t *)&olen);
		if (status != PSA_SUCCESS) {
			BK_LOGE(TAG, "psa_cipher_update failed! (Error: %d)\r\n", status);
			return APP_ERROR;
		}

		/* Finalize the encryption */
		status = psa_cipher_finish(&operation, m_encrypted_text + olen,
					   sizeof(m_encrypted_text) - olen,
					   (size_t *)&olen);
		if (status != PSA_SUCCESS) {
			BK_LOGE(TAG, "psa_cipher_finish failed! (Error: %d)\r\n", status);
			return APP_ERROR;
		}

		BK_LOGI(TAG, "Encryption successful!\r\n");

		/* Setup the decryption operation */
		memset(&operation, 0x0, sizeof(psa_cipher_operation_t));
		status = psa_cipher_decrypt_setup(&operation, key_attr->id, key_attr->alg);
		if (status != PSA_SUCCESS) {
			BK_LOGE(TAG, "psa_cipher_decrypt_setup failed! (Error: %d)\r\n", status);
			return APP_ERROR;
		}

		/* Set the IV generated in encryption */
		status = psa_cipher_set_iv(&operation, aes_iv, BYTE_SIZE_IV);
		if (status != PSA_SUCCESS) {
			BK_LOGE(TAG, "psa_cipher_set_iv failed! (Error: %d)\r\n", status);
			return APP_ERROR;
		}

		/* Perform the decryption */
		status = psa_cipher_update(&operation, m_encrypted_text,
					   sizeof(m_encrypted_text), m_decrypted_text,
					   sizeof(m_decrypted_text), (size_t *)&olen);
		if (status != PSA_SUCCESS) {
			BK_LOGE(TAG, "psa_cipher_update failed! (Error: %d)\r\n", status);
			return APP_ERROR;
		}

		/* Finalize the decryption */
		status = psa_cipher_finish(&operation, m_decrypted_text + olen,
					   sizeof(m_decrypted_text) - olen,
					   (size_t *)&olen);
		if (status != PSA_SUCCESS) {
			BK_LOGE(TAG, "psa_cipher_finish failed! (Error: %d)\r\n", status);
			return APP_ERROR;
		}

		/* Check the validity of the decryption */
		if (memcmp(m_decrypted_text,
					m_plain_text,
					CRYPTO_EXAMPLE_AES_MAX_TEXT_SIZE) != 0){

			BK_LOGE(TAG, "Error: Decrypted text doesn't match the plaintext\r\n");
			return APP_ERROR;
		}

		BK_LOGI(TAG, "Decryption successful!\r\n");
	}

	return APP_SUCCESS;
}

int get_key_attributes(void)
{
	psa_status_t status;
	psa_key_attributes_t key_attributes = PSA_KEY_ATTRIBUTES_INIT;
	key_attributes_t *key_attr;

	for(int i = 0; i < PSA_KEY_NUM; i++)
	{
		key_attr = &psa_key_attr_cfg_g[i];

		status = psa_get_key_attributes(key_attr->id, &key_attributes);
		if(status != PSA_SUCCESS)
		{
			BK_LOGE(TAG, "get AES key failed, keyid = 0x%x ,status = 0x%x\r\n", key_attr->id, status);
			return APP_ERROR;
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

int destroy_key(uint32_t id)
{
	psa_status_t status;

	/* Destroy the key handle */
	status = psa_destroy_key(id);
	if (status != PSA_SUCCESS) {
		BK_LOGE(TAG, "psa_destroy_key failed! (Error: %d)\r\n", status);
		return APP_ERROR;
	}

	return APP_SUCCESS;
}