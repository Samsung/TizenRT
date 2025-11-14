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
#include <psa/crypto.h>
#include <psa/crypto_extra.h>
#include "psa/storage_common.h"
#include "psa/protected_storage.h"

#include "tfm_ns_interface.h"

#if defined(CONFIG_TFM_AES_GCM_NSC)
#include "tfm_aes_gcm_nsc.h"
#endif
#include "_otp.h"

#define APP_SUCCESS             (0)
#define APP_ERROR               (-1)
#define AES_GCM_KEY_LENGTH      (32)
#define AES_GCM_IV_LENGTH       (12)
#define AES_GCM_KEY_SHIFT_BITS  (3)
#define AES_GCM_MAX_TEXT_SIZE   (2048)
#define AES_GCM_BLOCK_SIZE      (16)
#define AES_GCN_TEST_LOG        (1)

#define TAG "AES_GCM"
#define APP_SUCCESS_MESSAGE "Example finished successfully!"
#define APP_ERROR_MESSAGE "Example exited with error!"


static uint8_t s_encrypted_text[AES_GCM_MAX_TEXT_SIZE];
static uint8_t s_decrypted_text[AES_GCM_MAX_TEXT_SIZE];
#if 1
static uint8_t s_plain_text[] = 
{
0xbc, 0xf2, 0x3f, 0x2,  0xa5, 0x4d, 0x5a, 0xce, 0xa,  0x97, 0x6f, 0x5c, 0x89, 0x17, 0x8e, 0x3b,
0x7,  0x0,  0xb8, 0x77, 0x95, 0x7,  0x3f, 0x48, 0x59, 0x10, 0x5a, 0x2b, 0xdb, 0xa5, 0xc5, 0xe, 
0x41, 0x37, 0x6c, 0x14, 0xd6, 0x60, 0xb,  0x8d, 0xbb, 0x4a, 0x5a, 0xa3, 0x41, 0xbe, 0x5f, 0xf8,
0x6c, 0xb0, 0x7a, 0xc0, 0x61, 0x36, 0x3c, 0x84, 0xf1, 0x87, 0xc6, 0x8a, 0x53, 0xd9, 0x91, 0xf8, 
0xd9, 0x8f, 0x7d, 0x28, 0xcb, 0x67, 0x4b, 0xd8, 0xf9, 0x24, 0x17, 0x23, 0xc1, 0x17, 0x44, 0x2f,
0x8,  0x6,  0x85, 0x80, 0x42, 0xe2, 0x43, 0xbc, 0xf2, 0x3f, 0x2,  0xa5, 0x4d, 0x5a, 0xce, 0xa,  
0x7,  0x0,  0xb8, 0x77, 0x95, 0x7,  0x3f, 0x48, 0x59, 0x10, 0x5a, 0x2b, 0xdb, 0xa5, 0xc5, 0xe, 
0x41, 0x37, 0x6c, 0x14, 0xd6, 0x60, 0xb,  0x8d, 0xbb, 0x4a, 0x5a, 0xa3, 0x41, 0xbe, 0x5f, 0xf8,
0x6c, 0xb0, 0x7a, 0xc0, 0x61, 0x36, 0x3c, 0x84, 0xf1, 0x87, 0xc6, 0x8a, 0x53, 0xd9, 0x91, 0xf8, 
0xd9, 0x8f, 0x7d, 0x28, 0xcb, 0x67, 0x4b, 0xd8, 0xf9, 0x24, 0x17, 0x23, 0xc1, 0x17, 0x44, 0x2f, 
0x8,  0x6,  0x85, 0x80, 0x42, 0xe2, 0x43, 0x29, 0x16, 0xac, 0x7,  0x8a, 0x48, 0xc7, 0xb6, 0x45, 
0x65, 0x0,  0x79, 0x50, 0x13, 0xee, 0x3e, 0x6c, 0x29, 0xa5, 0xa5, 0x61, 0x15, 0xbd, 0x6c, 0xde, 
0xbe, 0x4e, 0x18, 0x87, 0xb8, 0xaa, 0x94, 0xdd, 0x76, 0xa9, 0x29, 0xdc, 0x47, 0xd0, 0xb1, 0xe2, 
0xc7, 0xd2, 0x22, 0xe7, 0x80, 0xee, 0xe9, 0xb4, 0xd3, 0x2,  0xcb, 0x54, 0x93, 0x17, 0x7e, 0x19,
0x33, 0x17, 0xa2, 0x44, 0x91, 0x27, 0xf6, 0x77, 0xe6, 0x2c, 0x9a, 0xb0, 0x49, 0xe2, 0x1,  0xfd,
0x59, 0x1,  0x31, 0xd3, 0xf6, 0xfe, 0x91, 0xd6, 0xd6, 0x2f, 0xe3, 0x8e, 0xc6, 0xcb, 0x21, 0x3d,
0xb8, 0x4e, 0xe,  0x8c, 0x6f, 0x19, 0x91, 0xd6, 0xd6, 0x2f, 0xe3, 0x8e, 0xc6, 0xcb, 0x21, 0x3d};
#endif

#if 0
static uint8_t s_plain_text[] ={"hello Example string to demonstrate basic usage of AES GCM mode"};
#endif

static aes_gcm_context_t s_aes_gcm_context;

static int encrypt_aes_gcm(void)
{
	psa_status_t status = APP_ERROR;
	size_t plaintext_len;

	BK_LOGI(TAG, "encrypting using AES GCM MODE...\r\n");

	plaintext_len = sizeof(s_plain_text);

#if defined(CONFIG_TFM_AES_GCM_NSC)
	tfm_ns_interface_lock();
	status = psa_aes_gcm_encrypt(&s_aes_gcm_context, s_plain_text, plaintext_len, s_encrypted_text);
	tfm_ns_interface_unlock();
    //extern 	int encData(const u8 * const pBuf, const size_t len, const u8 *pOutBuf);
	//status = encData(s_plain_text, plaintext_len, s_encrypted_text);
#endif
	if (status != PSA_SUCCESS) 
	{
		BK_LOGI(TAG, "psa_aead_decrypt failed! (Error: %d)\r\n", status);
		return APP_ERROR;
	}

	BK_LOGI(TAG, "Encryption successful!\r\n");
	
	return APP_SUCCESS;
}

static int decrypt_aes_gcm(void)
{
	psa_status_t status = APP_ERROR;

	BK_LOGI(TAG, "Decrypting using AES GCM MODE...\r\n");

	BK_LOGI(TAG, "s_aes_gcm_context.out_length :0x%x ...\r\n",s_aes_gcm_context.out_length);
#if defined(CONFIG_TFM_AES_GCM_NSC)
	tfm_ns_interface_lock();
	status = psa_aes_gcm_decrypt(&s_aes_gcm_context, s_encrypted_text, s_aes_gcm_context.out_length, s_decrypted_text);
	tfm_ns_interface_unlock();
	//extern int decData(const u8 * pBuf, const u32 len, const u8 *pOutBuf) ;
	//status = decData(s_encrypted_text, sizeof(s_encrypted_text), s_decrypted_text) ;
#endif

	if (status != PSA_SUCCESS) {
		BK_LOGI(TAG, "psa_aead_decrypt failed! (Error: %d)\r\n", status);
		return APP_ERROR;
	}

	/* Check the validity of the decryption */
	if (memcmp(s_decrypted_text, s_plain_text, s_aes_gcm_context.out_length) != 0)
	{
		BK_LOGI(TAG, "Error: Decrypted text doesn't match the plaintext\r\n");
		return APP_ERROR;
	}

	BK_LOGI(TAG, "Decryption and authentication successful!\r\n");

	return APP_SUCCESS;
}

int tz_aes_gcm_test_main(void)
{
	int status = APP_ERROR;

#if defined(CONFIG_TFM_PS)
	size_t read_len;
#endif

	BK_LOGI(TAG, "Starting AES-GCM example..and do aes_gcm encryption \r\n");

	s_aes_gcm_context.item_id = OTP_RANDOM_KEY_1;   //(_otp.h)

	status = encrypt_aes_gcm();
	if (status != APP_SUCCESS)
	{
		BK_LOGI(TAG, APP_ERROR_MESSAGE);
		return APP_ERROR;
	}

#if defined(CONFIG_TFM_PS)
	BK_LOGI(TAG, "save encrypted_text into the protected flash\r\n");

	status = psa_ps_set(PS_ID_SSL_CERTIFICATE_INFO, s_aes_gcm_context.out_length, s_encrypted_text, PSA_STORAGE_FLAG_NONE);
	if (status != APP_SUCCESS)
	{
		BK_LOGI(TAG, "psa_ps_set failed! (status: %d)\r\n", status);
		return APP_ERROR;
	}

	BK_LOGI(TAG, "get encrypted_text from the protected flash\r\n");

	status = psa_ps_get(PS_ID_SSL_CERTIFICATE_INFO, 0, s_aes_gcm_context.out_length, s_encrypted_text, &read_len);
	if (status != APP_SUCCESS)
	{
		BK_LOGI(TAG, "psa_ps_get failed! (status: %d)\r\n", status);
		return APP_ERROR;
	}

	BK_LOGI(TAG, "read_len :0x%x \r\n",read_len);
#endif

	BK_LOGI(TAG, "do aes_gcm decryption\r\n");

	status = decrypt_aes_gcm();
	if (status != APP_SUCCESS)
	{
		BK_LOGI(TAG, APP_ERROR_MESSAGE);
		return APP_ERROR;
	}

	BK_LOGI(TAG, APP_SUCCESS_MESSAGE);

	return APP_SUCCESS;
}
