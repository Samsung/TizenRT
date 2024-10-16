/****************************************************************************
 *
 * Copyright 2021 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/
/// @file   tash_security.c
/// @brief  functions to check secure tash command

#include <tinyara/config.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#ifdef CONFIG_TASH_REBOOT
#include <sys/boardctl.h>
#endif
#include <security/security_crypto.h>
#include <mbedtls/pkcs5.h>
#include "tash_internal.h"

#define CONVERT_nCHAR           (sizeof(unsigned long) * 2) // unsigned long for strtoul

#define PBKDF2_KEY_nCHAR        (64) // 32 bytes
#define PBKDF2_KEY_LEN          (32) // 32 bytes
#define PBKDF2_SALT_LEN         (16) // 16 bytes
#define PBKDF2_SALT_nCHAR       (32) // 16 bytes
#define AES_BLOCK_SIZE          (16)
#define AES128_KEY_SLOT         "ss/38" // slot where AES key is stored


static int salt_aes_decrypt(char *input, unsigned int input_size, char *output, unsigned int output_size) 
{
	if ((input_size <= 0) || (input_size % AES_BLOCK_SIZE)) {
		printf("Invalid AES key input size\n");
		printf("input size : %d\n", input_size);
		return -1;
	}

	if (input == NULL) {
		printf("AES input is null\n");
		return -1;
	} 

	if (output_size < input_size) {
		printf("Invalid AES key output size\n");
		printf("output size : %d\n", output_size);
		return -1;
	}

	if (output == NULL) {
		printf("AES output is null\n");
		return -1;
	}

	security_handle hnd;
	security_data encrypt_data;
	security_data decrypt_data;
	security_aes_param aes_param;
	security_error ret;
	char *tmp = NULL;

	ret = security_init(&hnd);
	if (ret != SECURITY_OK) {
		printf("security_crypto : security_init failed. ret : %d\n", ret);
		return -1;
	}

	encrypt_data.data = input;
	encrypt_data.length = input_size;

	decrypt_data.length = output_size;

	aes_param.mode = AES_CBC_NOPAD;
	aes_param.iv = NULL;
	aes_param.iv_len = 0;

	ret = crypto_aes_decryption(hnd, &aes_param, AES128_KEY_SLOT, &encrypt_data, &decrypt_data);
	if (ret != SECURITY_OK) {
		printf("security_crypto : crypto_aes_decryption failed. ret : %d\n", ret);
		return -1;
	}

	tmp = decrypt_data.data;
	for (int i = 0; i < output_size; i++) {
		output[i] = tmp[i];
	}

	return 1;
}

static int get_pbkdf2(char *input, int input_len, char *salt, int salt_len, int iteration, unsigned char *pbkdf2_output)
{
	mbedtls_md_context_t sha_ctx;
	const mbedtls_md_info_t *info_sha;

	/* Setup the hash/HMAC function, for the PBKDF2 function. */
	mbedtls_md_init(&sha_ctx);

	info_sha = mbedtls_md_info_from_type(MBEDTLS_MD_SHA256);
	if (info_sha == NULL) {
		printf("Failed to get hash information\n");
		mbedtls_md_free(&sha_ctx);
		return -1;
	}

	int ret = mbedtls_md_setup(&sha_ctx, info_sha, 1);
	if (ret != 0) {
		printf("Failed to setup hash function");
		mbedtls_md_free(&sha_ctx);
		return -1;
	}

	ret = mbedtls_pkcs5_pbkdf2_hmac(&sha_ctx, (const unsigned char *)input, input_len, (const unsigned char *)salt, salt_len, iteration, PBKDF2_KEY_LEN, pbkdf2_output);
	if (ret != 0) {
		printf("Call to mbedtls PBKDF2 function failed\n");
		mbedtls_md_free(&sha_ctx);
		return -1;
	}

	mbedtls_md_free(&sha_ctx);

	return 1;
}

static void convert_str_to_hex(char *input, unsigned char *output, int char_len)
{
	char input_char[CONVERT_nCHAR + 1] = { 0, };
	unsigned long converted_val;
	int offset = 0;
	int hex_idx = 0;

	do {
		strncpy(input_char, ((char *)input + offset), CONVERT_nCHAR);
		converted_val = strtoul(input_char, NULL, 16);
		output[hex_idx++] = (unsigned char)((converted_val >> 24) & 0xFF);
		output[hex_idx++] = (unsigned char)((converted_val >> 16) & 0xFF);
		output[hex_idx++] = (unsigned char)((converted_val >> 8) & 0xFF);
		output[hex_idx++] = (unsigned char)(converted_val & 0xFF);
		offset += CONVERT_nCHAR;
	} while (offset < char_len);
}

void tash_check_security(int fd)
{
	const char passwd_prompt[] = "PASSWORD>>";
	char *input;
	int nbytes;
	int input_len = 0;
	unsigned char pw_pbkdf2_hex[PBKDF2_KEY_LEN];
	unsigned char input_pbkdf2_hex[PBKDF2_KEY_LEN];
	unsigned char salt_encrypted_hex[PBKDF2_SALT_LEN];
	unsigned char pbkdf2_salt[PBKDF2_SALT_LEN] = { 0, };
	int retry_count = 0;

	if (strlen(CONFIG_TASH_PASSWORD_HASH) != PBKDF2_KEY_nCHAR) {
		printf("TASH: CONFIG_TASH_PASSWORD_HASH has wrong length (%d).\n", strlen(CONFIG_TASH_PASSWORD_HASH));
		printf("      It should be 32 bytes, 64 characters.\n");
		return;
	}

	/* Convert password character to hex */
	convert_str_to_hex(CONFIG_TASH_PASSWORD_HASH, pw_pbkdf2_hex, PBKDF2_KEY_nCHAR);

	if (strlen(CONFIG_TASH_SALT_ENCRYPTED) != PBKDF2_SALT_nCHAR) {
		printf("TASH: CONFIG_TASH_SALT_ENCRYPTED has wrong length (%d).\n", strlen(CONFIG_TASH_SALT_ENCRYPTED));
		printf("      It should be 16 bytes, 32 characters.\n");
		return;
	}

	/* Convert salt character to hex */
	convert_str_to_hex(CONFIG_TASH_SALT_ENCRYPTED, salt_encrypted_hex, PBKDF2_SALT_nCHAR);
	
	/* Decrypt encrypted SALT by AES */
	if (1 != salt_aes_decrypt(salt_encrypted_hex, PBKDF2_SALT_LEN, pbkdf2_salt, PBKDF2_SALT_LEN)) {
		printf("TASH: salt_aes_decrypt() failed\n");
		return;
	}

	do {
		/* Print secure prompt */
		nbytes = write(fd, passwd_prompt, sizeof(passwd_prompt));
		if (nbytes < sizeof(passwd_prompt)) {
			usleep(20);
			continue;
		}

		/* Read user input */
		input = tash_read_input_line(fd);
		input_len = strlen(input);
		if (input_len == 0) {
			tash_free(input);
			continue;
		}

		/* Allow reboot command regardless of TASH authentication */
		#ifdef CONFIG_TASH_REBOOT
		if (!strncmp((unsigned char *)input, "reboot", 7)) {
			(void)tash_reboot(0, NULL);
			return;
		}
		#endif

		/* Convert user input to pbkdf2 value */
		if (get_pbkdf2(input, input_len, pbkdf2_salt, PBKDF2_SALT_LEN, CONFIG_TASH_HASH_ITERATION, input_pbkdf2_hex) < 0) {
			return;
		}

		tash_free(input);

		/* Confirm the password */
		if (!memcmp((char *)pw_pbkdf2_hex, (char *)input_pbkdf2_hex, PBKDF2_KEY_LEN)) {
			break;
		} else {
			retry_count++;
			if (retry_count % 3 == 0) {
				printf("TASH: It will be locked for 30sec\n");
				sleep(30);
			}
		}
	} while (1);
}
