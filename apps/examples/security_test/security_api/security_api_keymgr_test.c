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

#include <tinyara/config.h>
#include <stdio.h>
#include <security/security_keymgr.h>
#include "security_api_utils.h"

#define HMACSHA256_KEY "ss/01"
#define AES128_KEY "ss/02"
#define AES128SET_KEY "ss/03"
#define RSA1024_KEY "ss/04"

void
test_keymgr(void)
{
	security_handle hnd;
	security_data hash_gen_key = {NULL, 0};
	security_data aes_gen_key = {NULL, 0};
	security_data rsa_gen_key = {NULL, 0};
	security_data get_x_key = {NULL, 0};
	security_data get_y_key = {NULL, 0};
	security_data aes_set_key = {NULL, 0};

	aes_set_key.data = "1234567890123456";
	aes_set_key.length = 16;

	printf(" Key Manager Test\n");

	printf("  . SEC Initialize ...");
	fflush(stdout);

	if (0 != security_init(&hnd)) {
		printf("Fail\n	! security_init\n");
		return;
	}
	printf("ok\n");

	printf("  . SEC Generate Key : HMAC_SHA256 ...\n");
	fflush(stdout);

	if (0 != keymgr_generate_key(hnd, HMAC_SHA256, HMACSHA256_KEY)) {
		printf("Fail\n	! keymgr_generate_key\n");
		goto exit;
	}
	printf("ok\n");

	printf("  . SEC Generate Key : AES128 ...\n");
	fflush(stdout);

	if (0 != keymgr_generate_key(hnd, KEY_AES_128, AES128_KEY)) {
		printf("Fail\n	! keymgr_generate_key\n");
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
	// PrintBuffer("RSA1024 Public key", rsa_gen_key.data, rsa_gen_key.length);

	printf("  . SEC Get Publickey ...\n");
	fflush(stdout);

	security_key_type key_type = KEY_RSA_1024;
	if (0 != keymgr_get_key(hnd, key_type, RSA1024_KEY, &get_x_key, &get_y_key)) {
		printf("Fail\n	! keymgr_get_pubkey\n");
		goto exit;
	}
	printf("ok\n");
	PrintBuffer("RSA1024 Public key", get_x_key.data, get_x_key.length);

	printf("  . SEC Set Key : AES128 ...\n");
	fflush(stdout);

	if (0 != keymgr_set_key(hnd, KEY_AES_128, AES128SET_KEY, &aes_set_key, NULL)) {
		printf("Fail\n	! keymgr_set_key\n");
		goto exit;
	}
	printf("ok\n");

	printf("  . SEC Remove Key : HMAC_SHA256 ...\n");
	fflush(stdout);

	if (0 != keymgr_remove_key(hnd, KEY_HMAC_SHA256, HMACSHA256_KEY)) {
		printf("Fail\n	! keymgr_remove_key\n");
		goto exit;
	}
	printf("ok\n");

	printf("  . SEC Remove Key : AES128 ...\n");
	fflush(stdout);

	if (0 != keymgr_remove_key(hnd, KEY_AES_128, AES128_KEY)) {
		printf("Fail\n	! keymgr_remove_key\n");
		goto exit;
	}
	printf("ok\n");

	printf("  . SEC Remove Key : RSA1024 ...\n");
	fflush(stdout);

	if (0 != keymgr_remove_key(hnd, KEY_RSA_1024, RSA1024_KEY)) {
		printf("Fail\n	! keymgr_remove_key\n");
		goto exit;
	}
	printf("ok\n");

	printf("  . SEC Remove Key : SET_AES128 ...\n");
	fflush(stdout);

	if (0 != keymgr_remove_key(hnd, KEY_AES_128, AES128SET_KEY)) {
		printf("Fail\n	! keymgr_remove_key\n");
		goto exit;
	}
	printf("ok\n");

exit:
	free_security_data(&hash_gen_key);
	free_security_data(&aes_gen_key);
	free_security_data(&rsa_gen_key);
	free_security_data(&get_x_key);
	free_security_data(&get_y_key);

	printf("  . SEC Deinitialize ...\n");
	security_deinit(hnd);
	printf("ok\n");
	fflush(stdout);

	return;
}
