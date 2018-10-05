/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
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
/****************************************************************************
 * examples/security_sdk/security_sdk_aes.c
 *
 *   Copyright (C) 2018 SAMSUNG ELECTRONICS CO., LTD. All rights reserved.
 *   Author: Youngdae Oh <yd.oh@samsung.com>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

#include <security_sdk_test.h>

int security_sdk_aes_encrypt_decrypt(void)
{
	artik_security_module *security = NULL;
	int test_result = 0;
	artik_security_handle handle = NULL;
	artik_error ret = S_OK;
	unsigned int i, j;
	unsigned int aes_info[3][2] = { {AES_128, 16}, {AES_192, 24}, {AES_256, 32} };
	unsigned int aes_mode[][2] = { {AES_ECB_NOPAD, 32}, {AES_ECB_PKCS7, 13},
		{AES_CBC_NOPAD, 32}, {AES_CBC_PKCS7, 13}, {AES_CTR_NOPAD, 32}
	};
	unsigned char aes_input[150] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0,
									 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0,
									 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0,
									 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0,
									 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0
								   };
	unsigned char iv[16] = { 0 };
	char key_name[16] = { 0 };
	unsigned char *out = NULL;
	unsigned int outlen = 0;
	unsigned char *check = NULL;
	unsigned int checklen = 0;
	unsigned int cnt = 0;

	fprintf(stderr, "------------------------------------------------------\n");
	fprintf(stderr, "  SECURITY SDK TESTCASE : aes encrypt/decrypt\n");
	fprintf(stderr, "------------------------------------------------------\n");

	security = (artik_security_module *) artik_request_api_module("security");

	if (!security) {
		fprintf(stderr, "Security module is not available\n");
		test_result++;
		goto exit;
	}

	ret = security->request(&handle);
	if (ret != S_OK) {
		fprintf(stderr, "Failed to request security instance (err=%d)\n", ret);
		test_result++;
		goto exit;
	}

	/*
	 * ARTIK PSK
	 */
	ret = security->aes_encryption(handle, AES_CBC_PKCS7, ARTIK_HMAC_KEY_ID, iv, sizeof(iv), aes_input, 32, &out, &outlen);
	ret += security->aes_decryption(handle, AES_CBC_PKCS7, ARTIK_HMAC_KEY_ID, iv, sizeof(iv), out, outlen, &check, &checklen);

	see_selfprintf("[%d] ", cnt++);
	if (ret) {
		test_result++;
		see_selfprintf(" fail\n");
	} else {
		see_selfprintf(" success\n");
	}

	if (out) {
		free(out);
		out = NULL;
	}
	if (check) {
		free(check);
		check = NULL;
	}

	for (i = 0; i < ARRAY_SIZE(aes_mode); i++) {
		for (j = 0; j < 3; j++) {
			memset(key_name, 0, sizeof(key_name));
			sprintf(key_name, "%s/%X", SECURE_STORAGE_DEFAULT, j);

			ret = security->set_key(handle, aes_info[j][0], key_name, aes_input, aes_info[j][1]);
			ret += security->aes_encryption(handle, aes_mode[i][0], key_name, iv, sizeof(iv), aes_input, aes_mode[i][1], &out, &outlen);
			ret += security->aes_decryption(handle, aes_mode[i][0], key_name, iv, sizeof(iv), out, outlen, &check, &checklen);
			ret += security->remove_key(handle, aes_info[j][0], key_name);

			see_selfprintf("[%d] ", cnt++);
			if (ret || memcmp(aes_input, check, checklen)) {
				test_result++;
				see_selfprintf(" fail\n");
			} else {
				see_selfprintf(" success\n");
			}

			if (out) {
				free(out);
				out = NULL;
			}
			if (check) {
				free(check);
				check = NULL;
			}
		}
	}

	for (i = 0; i < ARRAY_SIZE(aes_mode); i++) {
		for (j = 0; j < 3; j++) {
			memset(key_name, 0, sizeof(key_name));
			sprintf(key_name, "%s/%X", MEMORY_STORAGE, cnt % 4);

			ret = security->set_key(handle, aes_info[j][0], key_name, aes_input, aes_info[j][1]);
			ret += security->aes_encryption(handle, aes_mode[i][0], key_name, iv, sizeof(iv), aes_input, aes_mode[i][1], &out, &outlen);
			ret += security->aes_decryption(handle, aes_mode[i][0], key_name, iv, sizeof(iv), out, outlen, &check, &checklen);
			ret += security->remove_key(handle, aes_info[j][0], key_name);

			see_selfprintf("[%d] ", cnt++);
			if (ret || memcmp(aes_input, check, checklen)) {
				test_result++;
				see_selfprintf(" fail\n");
			} else {
				see_selfprintf(" success\n");
			}

			if (out) {
				free(out);
				out = NULL;
			}
			if (check) {
				free(check);
				check = NULL;
			}
		}
	}

exit:
	if (out) {
		free(out);
		out = NULL;
	}
	if (check) {
		free(check);
		check = NULL;
	}

	if (handle) {
		security->release(handle);
	}

	if (security) {
		artik_release_api_module(security);
	}

	return test_result;
}
