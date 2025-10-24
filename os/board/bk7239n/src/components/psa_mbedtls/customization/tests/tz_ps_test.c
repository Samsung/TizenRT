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

#include "tz_ps.c"
#include "tz_test.h"

#define TAG "PSA_PS"

#define APP_SUCCESS		(0)
#define APP_ERROR		(-1)

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


int ps_generate_key_manage(char *names, uint32_t id)
{
	psa_status_t status = -1;
	uint32_t key_id;
	
	BK_LOGI(TAG, "generate_key_manage (name: %s, key_id: %d)\r\n",names, id);
	status = ps_key_get_id_by_name(names, &key_id);
	if(status == 0)
	{
		if(id == key_id)
		{
			BK_LOGI(TAG, "generate_key_manage has exist! (name: %s, key_id: %d)\r\n",names, key_id);
			return 0;
		}
	}
	status = ps_key_generate_by_attribute(names, id, PSA_KEY_TYPE_AES, BIT_SIZE_AES_128, 
                                          PSA_KEY_USAGE_ENCRYPT | PSA_KEY_USAGE_DECRYPT, PSA_ALG_CBC_NO_PADDING);
	if(status != 0)
	{
		BK_LOGE(TAG, "generate_ps_key_by_attribute fail! (Error: %d)\r\n", status);
		return 0;
	}
	return 0;
}

#if defined(CONFIG_DEBUG_NEW_PS)
int ps_aes_encrypt_decrypt(char *names)
{
	tz_ps_aes_cbc_encrypt(names,m_plain_text,sizeof(m_plain_text),m_encrypted_text,sizeof(m_encrypted_text));

	tz_ps_aes_cbc_decrypt(names,m_encrypted_text,sizeof(m_encrypted_text),m_decrypted_text,sizeof(m_decrypted_text));

	/* Check the validity of the decryption */
	if (memcmp(m_decrypted_text,m_plain_text,CRYPTO_EXAMPLE_AES_MAX_TEXT_SIZE) != 0)
	{
		BK_LOGE(TAG, "Error: Decrypted text doesn't match the plaintext\r\n");
		return APP_ERROR;
	}

	BK_LOGI(TAG, "Decryption successful!\r\n");
	return APP_SUCCESS;
}
#else
int ps_aes_encrypt_decrypt(char *names)
{
	psa_status_t status;
	psa_cipher_operation_t operation = PSA_CIPHER_OPERATION_INIT;
	uint32_t olen;
	uint8_t aes_iv[BYTE_SIZE_IV];
	uint32_t id;

	status = ps_key_get_id_by_name(names, &id);
	if(status != PSA_SUCCESS)
	{
		BK_LOGE(TAG, "psa_ps_set failed! (key id: %d Error: %d)\r\n",id, status);
		return -1;
	}

	/* Setup the encryption operation */
	status = psa_cipher_encrypt_setup(&operation, id, PSA_ALG_CBC_NO_PADDING);
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
	status = psa_cipher_decrypt_setup(&operation, id, PSA_ALG_CBC_NO_PADDING);
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

	return APP_SUCCESS;
}
#endif
int ps_get_key_attributes(char *names)
{
	psa_status_t status;
	uint32_t id;

	status = ps_key_get_id_by_name(names, &id);
	if(status != PSA_SUCCESS)
	{
		BK_LOGE(TAG, "psa_ps_set failed! (key id: %d Error: %d)\r\n",id, status);
		return -1;
	}
	psa_key_attributes_t key_attributes = PSA_KEY_ATTRIBUTES_INIT;

		status = psa_get_key_attributes(id, &key_attributes);
		if(status != PSA_SUCCESS)
		{
			BK_LOGE(TAG, "get AES key failed, keyid = 0x%x (Error: %d)\r\n", id, status);
			return APP_ERROR;
		}
		else
		{
			psa_key_id_t    key_id  = psa_get_key_id(&key_attributes);
			psa_key_type_t  type    = psa_get_key_type(&key_attributes);
			size_t          bits    = psa_get_key_bits(&key_attributes);
			psa_key_lifetime_t life = psa_get_key_lifetime(&key_attributes);
			psa_key_usage_t usage   = psa_get_key_usage_flags(&key_attributes);
			psa_algorithm_t alg     = psa_get_key_algorithm(&key_attributes);

			BK_LOGI(TAG, "key 0x%x already exist, type = 0x%x,bits = 0x%x, usage = 0x%x, alg = 0x%x, life = 0x%x\r\n",
				key_id, type, bits, usage, alg, life);
		}

	return APP_SUCCESS;
}

int ps_destroy_key_by_name(char *names)
{
	if(ps_key_destroy_by_names(names) < 0)
		return APP_ERROR;

	return APP_SUCCESS;
}

int ps_destroy_key_by_id(uint32_t id)
{
	if(ps_key_destroy_by_id(id) < 0)
		return APP_ERROR;

	return APP_SUCCESS;
}

int ps_key_status(void)
{
	psa_status_t status;
	struct psa_storage_info_t info;
	size_t size;

	status = psa_ps_get_info(PS_ID_KEY_INFO, &info);
	if(status != PSA_SUCCESS)
	{
		BK_LOGE(TAG, "psa_ps_set failed! (Error: %d)\r\n", status);
		return -1;
	}
	size = info.size;
	BK_LOGI(TAG, "ps storage info size =%d \r\n", size);

	return 0;
}

int ps_get_key_id_by_name(char *names)
{
	psa_status_t status;
	uint32_t key_id;

	status = ps_key_get_id_by_name(names, &key_id);
	if(status != PSA_SUCCESS)
	{
		BK_LOGE(TAG, "psa_ps_set failed! (key id: %d Error: %d)\r\n",key_id, status);
		return -1;
	}

	BK_LOGI(TAG, "(name: %s get key_id: %d)\r\n",names, key_id);
	return 0;
}

