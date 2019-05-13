/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
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
 * either express or implied. SEC the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/
/*
 * Test Crypto API
 */

#include <tinyara/config.h>

#include <stdio.h>
#include <security/security_crypto.h>
#include <security/security_keymgr.h>
#include "security_api_utils.h"

/*	security api doesn't support key name now. it'll be supported later
 * currently ss/01 will be convert to slot 1.
 */
#define AES128_KEY "ss/01"
#define RSA1024_KEY "ss/02"

void
test_crypto(void)
{
	security_handle hnd;
	security_data iv = {NULL, 0};
	security_data input = {NULL, 0};
	security_data aes_enc_data = {NULL, 0};
	security_data aes_dec_data = {NULL, 0};
	security_data rsa_gen_key = {NULL, 0};
	security_data rsa_enc_data = {NULL, 0};
	security_data rsa_dec_data = {NULL, 0};

	iv.data = "1234567890123456";
	iv.length = 16;

	input.data = "1234567890123456";
	input.length = 16;

	printf("  . SEC Initialize ...\n");
	fflush(stdout);
	if (0 != security_init(&hnd)) {
		printf("Fail\n	! security_init\n");
		return;
	}
	printf("ok\n");

	printf("  . SEC Generate Key : AES128 ...\n");
	fflush(stdout);
	if (0 != keymgr_generate_key(hnd, KEY_AES_128, AES128_KEY)) {
		printf("Fail\n	! keymgr_generate_key\n");
		goto exit;
	}
	printf("ok\n");

	printf("  . SEC Get Key");

	printf("  . SEC AES Encryption ...\n");
	fflush(stdout);
	security_aes_param aes_param;
	aes_param.mode = AES_ECB_NOPAD;
	aes_param.iv = iv.data;
	aes_param.iv_len = iv.length;

	if (0 != crypto_aes_encryption(hnd, aes_param, AES128_KEY, &input, &aes_enc_data)) {
		printf("Fail\n	! crypto_aes_encryption\n");
		goto exit;
	}
	printf("ok\n");
	PrintBuffer("Input", input.data, input.length);
	PrintBuffer("Enc Data", aes_enc_data.data, aes_enc_data.length);

	printf("  . SEC AES Decryption ...\n");
	fflush(stdout);
	if (0 != crypto_aes_decryption(hnd, aes_param, AES128_KEY, &aes_enc_data, &aes_dec_data)) {
		printf("Fail\n	! security_aes_decryption\n");
		goto exit;
	}
	printf("ok\n");
	PrintBuffer("Enc Data", aes_enc_data.data, aes_enc_data.length);
	PrintBuffer("Dec Data", aes_dec_data.data, aes_dec_data.length);

	printf("  . SEC Remove Key : AES128 ...\n");
	fflush(stdout);
	if (0 != keymgr_remove_key(hnd, KEY_AES_128, AES128_KEY)) {
		printf("Fail\n	! keymgr_remove_key\n");
		goto exit;
	}
	printf("ok\n");

	printf("  . SEC Generate Key : RSA1024 ...\n");
	fflush(stdout);
	if (0 != keymgr_generate_key(hnd, KEY_RSA_1024, RSA1024_KEY)) {
		printf("Fail\n	! keymgr_generate_key\n");
		goto exit;
	}
	printf("ok\n");

	printf("  . SEC RSA Encryption ...\n");
	fflush(stdout);
	security_rsa_param rsa_param;
	rsa_param.rsa_a = RSASSA_PKCS1_V1_5;
	rsa_param.hash_t = HASH_SHA256;
	rsa_param.mgf = 0;
	rsa_param.salt_byte_len = 0;

	if (0 != crypto_rsa_encryption(hnd, rsa_param, RSA1024_KEY, &input, &rsa_enc_data)) {
		printf("Fail\n	! crypto_rsa_encryption\n");
		goto exit;
	}
	printf("ok\n");
	PrintBuffer("Input", input.data, input.length);
	PrintBuffer("Enc Data", rsa_enc_data.data, rsa_enc_data.length);

	printf("  . SEC RSA Decryption ...\n");
	fflush(stdout);
	if (0 != crypto_rsa_decryption(hnd, rsa_param, RSA1024_KEY, &rsa_enc_data, &rsa_dec_data)) {
		printf("Fail\n	! crypto_rsa_decryption\n");
		goto exit;
	}
	printf("ok\n");
	PrintBuffer("Dec Data", rsa_dec_data.data, rsa_dec_data.length);

	printf("  . SEC Remove Key : RSA1024 ...\n");
	fflush(stdout);

	if (0 != keymgr_remove_key(hnd, KEY_RSA_1024, RSA1024_KEY)) {
		printf("Fail\n	! keymgr_remove_key\n");
		goto exit;
	}
	printf("ok\n");

exit:
	free_security_data(&aes_enc_data);
	free_security_data(&aes_dec_data);
	free_security_data(&rsa_gen_key);
	free_security_data(&rsa_enc_data);
	free_security_data(&rsa_dec_data);

	printf("  . SEC Deinitialize ...\n");
	security_deinit(hnd);
	printf("ok\n");
	fflush(stdout);

	return;
}
