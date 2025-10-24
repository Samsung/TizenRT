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
#include "psa/storage_common.h"
#include "psa/protected_storage.h"
#include "tz_ps.h"

#define TAG "PSA_PS"

/* key storage */
#define KEY_STORAGE_NUM_MAX (16)
#define KEY_NAME_LENGTH_MAX (16)

#define PS_AES_IV_SIZE      (16)
typedef struct
{	
	char names[KEY_NAME_LENGTH_MAX];
	psa_key_id_t id;
	uint8_t flag;
}key_attributes_t;

int ps_key_get_id_by_name(char *names, uint32_t *id)
{
	key_attributes_t key_arr[KEY_STORAGE_NUM_MAX] = {0};
	psa_status_t status;

	size_t read_len = 0;

	status = psa_ps_get(PS_ID_KEY_INFO, 0x0, sizeof(key_arr), key_arr, &read_len);
	if(status != PSA_SUCCESS)
	{
		BK_LOGE(TAG, "psa_ps_get ps 1 empty, status = %d\r\n", status);
		psa_ps_set(PS_ID_KEY_INFO, sizeof(key_arr), &key_arr, PSA_STORAGE_FLAG_NONE);
		return -1;
	}

	for(int i = 0; i < KEY_STORAGE_NUM_MAX; i++)
	{
		if(strcmp(names, key_arr[i].names) == 0)
		{
			*id = key_arr[i].id;
			BK_LOGI(TAG, "name %s, id = %d \r\n",names, *id);
			return 0;
		}
		else
		{
		}
	}
	
	BK_LOGE(TAG, "get %s key failed,  need generate.\r\n", names);
	return -1;
}

int ps_key_generate_by_attribute(char *names, uint32_t key_id, uint16_t type, uint16_t bits, uint32_t usage, uint32_t alg)
{
	psa_status_t status;
	key_attributes_t key_arr[KEY_STORAGE_NUM_MAX] = {0};

	size_t read_len = 0;

	BK_LOGI(TAG, "Generating random key %s \r\n",names);

	status = psa_ps_get(PS_ID_KEY_INFO, 0x0, sizeof(key_arr), key_arr, &read_len);
	if(status != PSA_SUCCESS)
	{
		status = psa_ps_set(PS_ID_KEY_INFO, sizeof(key_arr), &key_arr, PSA_STORAGE_FLAG_NONE);
		if(status != PSA_SUCCESS){
			BK_LOGE(TAG, "psa_ps_set failed, status = %d\r\n", status);
			return -1;
		}
	}

	for(int i = 0; i < KEY_STORAGE_NUM_MAX; i++)
	{
		if(strcmp(names, key_arr[i].names) == 0)
		{
			BK_LOGI(TAG, "key  %s has exist, id = %d \r\n",names, key_arr[i].id);
			return 0;
		}
	}

	for(int i = 0; i < KEY_STORAGE_NUM_MAX; i++)
	{
		if(key_arr[i].flag == PS_STATUS_EMPTY)
		{
			BK_LOGI(TAG, "key %s  index = %d \r\n",names, i);
			break;
		}
		else if( i == KEY_STORAGE_NUM_MAX-1)
		{
			BK_LOGE(TAG, "ps key full!\r\n");
			return -1;
		}
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
		return -1;
	}
	/* After the key handle is acquired the attributes are not needed */
	psa_reset_key_attributes(&key_attributes);

	/* store key id and name together  */
	//psa_ps_get(PS_ID_KEY_INFO, 0x0, sizeof(key_arr), key_arr, &read_len);
	for(int i = 0; i < KEY_STORAGE_NUM_MAX; i++)
	{
		if(key_arr[i].flag == PS_STATUS_EMPTY)
		{
			key_arr[i].id = key_id;
			strcpy(key_arr[i].names, names);
			key_arr[i].flag = PS_STATUS_INIT;
			break;
		}
	}

	status = psa_ps_set(PS_ID_KEY_INFO, sizeof(key_arr), &key_arr, PSA_STORAGE_FLAG_NONE);
	if(status != PSA_SUCCESS)
	{
		BK_LOGE(TAG, "psa_ps_set failed! (key id: %d Error: %d)\r\n",key_id, status);
		return -1;
	}
	return 0;
}

int ps_key_destroy_by_id(uint32_t id)
{
	psa_status_t status;
	key_attributes_t key_arr[KEY_STORAGE_NUM_MAX];

	size_t read_len = 0;

	status = psa_ps_get(PS_ID_KEY_INFO, 0x0, sizeof(key_arr), key_arr, &read_len);
	if(status != PSA_SUCCESS)
	{
		BK_LOGE(TAG, "psa_ps_get failed, (Error: %d)\r\n", status);
		return -1;
	}

	for(int i = 0; i < KEY_STORAGE_NUM_MAX; i++)
	{
		if(id == key_arr[i].id)
		{
			/* Destroy the key handle */
			status = psa_destroy_key(key_arr[i].id);
			if (status != PSA_SUCCESS) {
				BK_LOGE(TAG, "psa_destroy_key failed! (Error: %d)\r\n", status);
				return -1;
			}

			memset(&key_arr[i], 0x0, sizeof(key_attributes_t));
			status = psa_ps_set(PS_ID_KEY_INFO, sizeof(key_arr), &key_arr, PSA_STORAGE_FLAG_NONE);
			if(status != PSA_SUCCESS)
			{
				BK_LOGE(TAG, "psa_ps_set destroy failed! (key id: %d Error: %d)\r\n",id, status);
				return -1;
			}
			break;
		}
		else if(i == KEY_STORAGE_NUM_MAX-1)
		{
			BK_LOGE(TAG, "key id %s not exist \r\n", id);
			return 0;
		}
	}
	BK_LOGI(TAG, "id = %d destroy \r\n", id);

	return 0;
}

int ps_key_destroy_by_names(char *names)
{
	psa_status_t status;
	key_attributes_t key_arr[KEY_STORAGE_NUM_MAX];

	size_t read_len = 0;

	status = psa_ps_get(PS_ID_KEY_INFO, 0x0, sizeof(key_arr), key_arr, &read_len);
	if(status != PSA_SUCCESS)
	{
		BK_LOGE(TAG, "psa_ps_get failed, status = %d\r\n", status);
		return -1;
	}

	for(int i = 0; i < KEY_STORAGE_NUM_MAX; i++)
	{
		BK_LOGI(TAG, "key_arr[%d].names %s id =%d \r\n", i, key_arr[i].names, key_arr[i].id);
		if(strncmp(names, key_arr[i].names, KEY_NAME_LENGTH_MAX) == 0)
		{
			/* Destroy the key handle */
			status = psa_destroy_key(key_arr[i].id);
			if (status != PSA_SUCCESS) {
				BK_LOGE(TAG, "psa_destroy_key failed! (Error: %d)\r\n", status);
				return -1;
			}

			memset(&key_arr[i], 0x0, sizeof(key_attributes_t));

			status = psa_ps_set(PS_ID_KEY_INFO, sizeof(key_arr), &key_arr, PSA_STORAGE_FLAG_NONE);
			if(status != PSA_SUCCESS)
			{
				BK_LOGE(TAG, "psa_ps_set destroy failed! (key name: %s Error: %d)\r\n", names);
				return -1;
			}
			break;
		}
		else if(i == KEY_STORAGE_NUM_MAX-1)
		{
			BK_LOGE(TAG, "key %s not exist \r\n", names);
			return 0;
		}
	}
	BK_LOGI(TAG, "key %s destroy \r\n", names);

	return 0;
}
int tz_ps_aes_cbc_encrypt(char *names,uint8_t * pInBuf,const uint32_t in_len,uint8_t *pOutBuf,const uint32_t out_len)
{
	psa_status_t status;
	psa_cipher_operation_t operation = PSA_CIPHER_OPERATION_INIT;
	uint32_t olen;
	uint8_t aes_iv[PS_AES_IV_SIZE];
	uint32_t id;

	status = ps_key_get_id_by_name(names, &id);
	if(status != 0)
	{
		BK_LOGE(TAG, "psa_ps_set failed! (key id: %d Error: %d)\r\n",id, status);
		return -1;
	}

	/* Setup the encryption operation */
	status = psa_cipher_encrypt_setup(&operation, id, PSA_ALG_CBC_NO_PADDING);
	if (status != 0) {
		BK_LOGE(TAG, "psa_cipher_encrypt_setup failed! (Error: %d)\r\n", status);
		return -1;
	}

	/* Generate an IV */
	status = psa_cipher_generate_iv(&operation, aes_iv, PS_AES_IV_SIZE, (size_t *)&olen);
	if (status != 0) {
		BK_LOGE(TAG, "psa_cipher_generate_iv failed! (Error: %d)\r\n", status);
		return -1;
	}

	/* Perform the encryption */
	status = psa_cipher_update(&operation,pInBuf,in_len, pOutBuf, out_len, (size_t *)&olen);
	if (status != 0) {
		BK_LOGE(TAG, "psa_cipher_update failed! (Error: %d)\r\n", status);
		return -1;
	}

	/* Finalize the encryption */
	status = psa_cipher_finish(&operation, pOutBuf + olen,
							   out_len - olen,
							   (size_t *)&olen);
	if (status != 0) {
		BK_LOGE(TAG, "psa_cipher_finish failed! (Error: %d)\r\n", status);
		return -1;
	}

	BK_LOGI(TAG, "Encryption successful!\r\n");
	return 0;
}
int tz_ps_aes_cbc_decrypt(char *names,uint8_t * pInBuf,const uint32_t in_len,uint8_t *pOutBuf,const uint32_t out_len)
{
	psa_status_t status;
	psa_cipher_operation_t operation = PSA_CIPHER_OPERATION_INIT;
	uint32_t olen;
	uint8_t aes_iv[PS_AES_IV_SIZE];
	uint32_t id;
	status = ps_key_get_id_by_name(names, &id);
	if(status != PSA_SUCCESS)
	{
		BK_LOGE(TAG, "psa_ps_set failed! (key id: %d Error: %d)\r\n",id, status);
		return -1;
	}
	/* Setup the decryption operation */
	memset(&operation, 0x0, sizeof(psa_cipher_operation_t));
	status = psa_cipher_decrypt_setup(&operation, id, PSA_ALG_CBC_NO_PADDING);
	if (status != 0) {
		BK_LOGE(TAG, "psa_cipher_decrypt_setup failed! (Error: %d)\r\n", status);
		return -1;
	}
	/* Set the IV generated in encryption */
	status = psa_cipher_set_iv(&operation, aes_iv, PS_AES_IV_SIZE);
	if (status != 0) {
		BK_LOGE(TAG, "psa_cipher_set_iv failed! (Error: %d)\r\n", status);
		return -1;
	}
	/* Perform the decryption */
	status = psa_cipher_update(&operation, pInBuf,in_len, pOutBuf,out_len, (size_t *)&olen);
	if (status != 0) 
	{
		BK_LOGE(TAG, "psa_cipher_update failed! (Error: %d)\r\n", status);
		return -1;
	}
	/* Finalize the decryption */
	status = psa_cipher_finish(&operation, pOutBuf + olen, out_len - olen,(size_t *)&olen);
	if (status != 0)
	{
		BK_LOGE(TAG, "psa_cipher_finish failed! (Error: %d)\r\n", status);
		return -1;
	}
	return 0;
}