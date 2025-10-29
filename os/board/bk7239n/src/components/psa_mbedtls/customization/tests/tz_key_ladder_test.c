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
#include <psa/crypto.h>

#include "tfm_dubhe_key_ladder_nsc.h"

#define TAG "KEY_LADDER"
#include "_otp.h"

#if 0
static uint8_t input_iv[16] = {//random
	0x49, 0xC0, 0x68, 0x83, 0xF2, 0x98, 0x63, 0x7D,
	0x8D, 0x2F, 0x1B, 0x3C, 0xF5, 0x5D, 0x1A, 0xC4};
#else
static uint8_t input_iv[16] = {//random
	0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66,
	0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66};
#endif

#define KEY_LADDER_TEST_TEXT_SIZE (64)

static uint8_t key_ladder_plain_text[KEY_LADDER_TEST_TEXT_SIZE] = {
	0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96,
	0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a,
    0xae, 0x2d, 0x8a, 0x57, 0x1e, 0x03, 0xac, 0x9c,
    0x9e, 0xb7, 0x6f, 0xac, 0x45, 0xaf, 0x8e, 0x51,
    0x30, 0xc8, 0x1c, 0x46, 0xa3, 0x5c, 0xe4, 0x11,
    0xe5, 0xfb, 0xc1, 0x19, 0x1a, 0x0a, 0x52, 0xef,
    0xf6, 0x9f, 0x24, 0x45, 0xdf, 0x4f, 0x9b, 0x17,
    0xad, 0x2b, 0x41, 0x7b, 0xe6, 0x6c, 0x37, 0x10
};

static uint8_t m_encrypted_text[KEY_LADDER_TEST_TEXT_SIZE];
static uint8_t m_decrypted_text[KEY_LADDER_TEST_TEXT_SIZE];
extern beken_mutex_t ns_lock_handle;

int te200_key_ladder_aes_cbc_main(void)
{
	key_ladder_context_t ctx;
	int ret =0;

	ctx.key_id = OTP_EK_1;
	ctx.mode = AES_OPERATION_ENCRYPT;
	memcpy(ctx.iv, input_iv, 16);
	//memcpy(ctx.iv, 0x0, 16);

	extern int aes_cbc_encrypt_data(uint8_t *input, uint32_t input_len, uint8_t *output);
	ret = aes_cbc_encrypt_data(key_ladder_plain_text, sizeof(key_ladder_plain_text), m_encrypted_text);

	if(ret < 0){
		BK_LOGE(TAG, "dubhe_key_ladder_aes_crypt_cbc enc fail! (Error: %d)\r\n", ret);
		return -1;
	}

	ctx.key_id = OTP_EK_1;
	ctx.mode = AES_OPERATION_DECRYPT;
	memcpy(ctx.iv, input_iv, 16);

	extern int aes_cbc_decrypt_data(uint8_t *input, uint32_t input_len, uint8_t *output);
	ret = aes_cbc_decrypt_data(m_encrypted_text, sizeof(m_encrypted_text),m_decrypted_text);

	if(ret < 0){
		BK_LOGE(TAG, "dubhe_key_ladder_aes_crypt_cbc dec fail! (Error: %d)\r\n", ret);
		return -1;
	}

	if (memcmp(m_decrypted_text,
					key_ladder_plain_text,
					KEY_LADDER_TEST_TEXT_SIZE) != 0){

			BK_LOGE(TAG, "Error: Decrypted text doesn't match the plaintext\r\n");
			return -1;
		}
		else
		{
			BK_LOGI(TAG, "%s successful\r\n",__func__);
		}

	return 0;
} 

int te200_key_ladder_aes_ecb_main(void)
{
	key_ladder_context_t ctx;
	int ret =0;

	while (rtos_lock_mutex(&ns_lock_handle) != BK_OK);
	ctx.key_id = OTP_EK_1;
	ctx.mode = AES_OPERATION_ENCRYPT;
	ret = dubhe_key_ladder_crypt_aes_ecb(&ctx, sizeof(key_ladder_plain_text), key_ladder_plain_text, m_encrypted_text);
	rtos_unlock_mutex(&ns_lock_handle);
	if(ret < 0){
		BK_LOGE(TAG, "dubhe_key_ladder_aes_crypt_cbc enc fail! (Error: %d)\r\n", ret);
		return -1;
	}

	while (rtos_lock_mutex(&ns_lock_handle) != BK_OK);
    ctx.key_id = OTP_EK_1;
	ctx.mode = AES_OPERATION_DECRYPT;
	ret = dubhe_key_ladder_crypt_aes_ecb(&ctx, sizeof(m_encrypted_text), m_encrypted_text, m_decrypted_text);
	rtos_unlock_mutex(&ns_lock_handle);
	if(ret < 0){
		BK_LOGE(TAG, "dubhe_key_ladder_aes_crypt_ecb dec fail! (Error: %d)\r\n", ret);
		return -1;
	}

	if (memcmp(m_decrypted_text,
					key_ladder_plain_text,
					KEY_LADDER_TEST_TEXT_SIZE) != 0){

			BK_LOGE(TAG, "Error: Decrypted text doesn't match the plaintext\r\n");
			return -1;
		}

	return 0;
}
#if 0
int te200_key_ladder_main(void)
{

	int ret = 0;

	dubhe_aes_ladder_init();

	ret = dubhe_aes_ladder_set_model_key(256);
	if(ret != 0){
		BK_LOGE(TAG, "dubhe_aes_ladder_set_model_key encrypt fail: 0x%x \n", ret);
		return -1;
	}
	BK_LOGI(TAG, "key_ladder test begin 3");
#if 0
	/*encrypt*/
	ret = dubhe_aes_ladder_crypt_cbc(&ctx, 1, sizeof(key_ladder_plain_text), input_iv, key_ladder_plain_text,
                         m_encrypted_text);
	if(ret != 0){
		BK_LOGE(TAG, "dubhe_aes_ladder_crypt_cbc encrypt fail: 0x%x \n", ret);
		return -1;
	}
	/*decrypt*/
	ret = dubhe_aes_ladder_crypt_cbc(&ctx, 0, sizeof(m_encrypted_text), input_iv, m_encrypted_text,
                         m_decrypted_text);
	if(ret != 0){
		BK_LOGE(TAG, "dubhe_aes_ladder_crypt_cbc decrypt fail: 0x%x \n", ret);
		return -1;
	}
#endif
	dubhe_aes_ladder_set_iv(input_iv);
	BK_LOGI(TAG, "key_ladder test 3 ");
	ret = dubhe_aes_ladder_encrypt_cbc(sizeof(key_ladder_plain_text), key_ladder_plain_text, m_encrypted_text);
	if(ret != 0){
		BK_LOGE(TAG, "dubhe_aes_ladder_encrypt_cbc encrypt fail: 0x%x \n", ret);
		return -1;
	}
	dubhe_aes_ladder_set_iv(input_iv);
	ret = dubhe_aes_ladder_decrypt_cbc(sizeof(m_encrypted_text), m_encrypted_text, m_decrypted_text);
	if(ret != 0){
		BK_LOGE(TAG, "dubhe_aes_ladder_decrypt_cbc encrypt fail: 0x%x \n", ret);
		return -1;
	}

	dubhe_aes_ladder_free();

	if (memcmp(m_decrypted_text,
					key_ladder_plain_text,
					KEY_LADDER_TEST_TEXT_SIZE) != 0){

			BK_LOGE(TAG, "Error: Decrypted text doesn't match the plaintext\r\n");
			return -1;
		}

    return 0;

	return 0;
}
#endif

