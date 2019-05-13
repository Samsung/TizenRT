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
#include <security/security_auth.h>
#include <security/security_keymgr.h>
#include "security_api_utils.h"

/*
 * Test Authenticate API
 */
#define HMACSHA256_KEY "ss/01"
#define ARTIK_CERT "ss/02"

void test_authenticate(void)
{
	security_handle hnd;
	security_data rand = {NULL, 0};
	security_data cert = {NULL, 0};
	security_data plaintext = {NULL, 0};
	security_data hashed = {NULL, 0};
	security_data sign = {NULL, 0};
	security_data hmac = {NULL, 0};
//	security_data hash_gen_key;

	plaintext.data = "01234567890123456789";
	plaintext.length = 20;
	printf("  . SEC Initialize ...");
	fflush(stdout);


	if (0 != security_init(&hnd)) {
		printf("Fail\n	! security_init\n");
		return;
	}

	printf("ok\n");
	printf("  . SEC Get Random ...\n");
	fflush(stdout);
	if (0 != auth_generate_random(hnd, 100, &rand)) {
		printf("Fail\n	! auth_generate_random\n");
		goto exit;
	}
	printf("ok\n");
	PrintBuffer("Random", rand.data, rand.length);


	printf("  . SEC Get Certificate ...\n");
	fflush(stdout);
	if (0 != auth_get_certificate(hnd, ARTIK_CERT, &cert)) {
		printf("Fail\n	! auth_get_certificate\n");
		goto exit;
	}
	printf("ok\n");
	PrintBuffer("Certificate", cert.data, cert.length);


	printf("  . SEC Get Hash ...\n");
	fflush(stdout);
	if (0 != auth_get_hash(hnd, HASH_SHA256, &plaintext, &hashed)) {
		printf("  fail\n  ! auth_get_hash\n");
		goto exit;
	}
	printf("ok\n");
	PrintBuffer("Plain Text", plaintext.data, plaintext.length);
	PrintBuffer("Hashed Data", hashed.data, hashed.length);


	/*	does ecdsa require certificate to get signature? */
	printf("  . SEC Get ECDSA Signature ...\n");
	fflush(stdout);
	security_ecdsa_param mode;
	mode.curve = ECDSA_SEC_P256R1;
	mode.hash_t = HASH_SHA256;

	if (0 != auth_get_ecdsa_signature(hnd, mode, ARTIK_CERT, &hashed, &sign)) {
		printf("  fail\n  ! auth_get_ecdsa_signature\n");
		goto exit;
	}
	printf("ok\n");
	PrintBuffer("Signed Data", sign.data, sign.length);


	printf("  . SEC Verify ECDSA Signature ...\n");
	fflush(stdout);
	if (0 != auth_verify_ecdsa_signature(hnd, mode, ARTIK_CERT, &hashed, &sign)) {
		printf("  fail\n  ! auth_verify_ecdsa_signature\n");
		goto exit;
	}
	printf("ok\n");


	printf("  . SEC Generate Key : HMAC_SHA256 ...\n");
	fflush(stdout);
	if (0 != keymgr_generate_key(hnd, HMAC_SHA256, HMACSHA256_KEY)) {
		printf("Fail\n	! auth_generate_key\n");
		goto exit;
	}
	printf("ok\n");


	printf("  . SEC Get HMAC ...\n");
	fflush(stdout);
	if (0 != auth_get_hmac(hnd, HMAC_SHA256, HMACSHA256_KEY, &plaintext, &hmac)) {
		printf("  fail\n  ! auth_get_hmac\n");
		goto exit;
	}
	printf("ok\n");
	PrintBuffer("Plain Data", plaintext.data, plaintext.length);
	PrintBuffer("HMAC Data", hmac.data, hmac.length);


	printf("  . SEC Remove Key : HMAC_SHA256 ...\n");
	fflush(stdout);
	if (0 != keymgr_remove_key(hnd, HMAC_SHA256, HMACSHA256_KEY)) {
		printf("Fail\n	! auth_remove_key\n");
		goto exit;
	}
	printf("ok\n");
exit:
	free_security_data(&rand);
	free_security_data(&cert);
	free_security_data(&hashed);
	free_security_data(&sign);
	free_security_data(&hmac);
//	free_security_data(&hash_gen_key);

	printf("  . SEC Deinitialize ...\n");
	security_deinit(hnd);
	printf("ok\n");
	fflush(stdout);

	return;
}
