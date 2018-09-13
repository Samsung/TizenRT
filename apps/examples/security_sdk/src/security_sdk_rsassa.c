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
 * examples/security_sdk/security_sdk_rsassa.c
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
#include <security_sdk_rsa.h>

#define SEE_RSASSA_SV_TC 16

int security_sdk_rsassa_sign_verify(void)
{
	artik_security_module *security = NULL;
	artik_security_handle handle = NULL;
	artik_error err = S_OK;
	char key_name[8] = { 0 };
	int ret = 1, i, j, cnt = 0;
	unsigned int test_result = 0;
	unsigned int algo_type[SEE_RSASSA_SV_TC] = {
			RSASSA_1024_PKCS1_V1_5_SHA160,
			RSASSA_1024_PKCS1_PSS_MGF1_SHA160,
			RSASSA_1024_PKCS1_V1_5_SHA256,
			RSASSA_1024_PKCS1_PSS_MGF1_SHA256,
			RSASSA_1024_PKCS1_V1_5_SHA384,
			RSASSA_1024_PKCS1_PSS_MGF1_SHA384,
			RSASSA_1024_PKCS1_V1_5_SHA512,
			RSASSA_1024_PKCS1_PSS_MGF1_SHA512,
			RSASSA_2048_PKCS1_V1_5_SHA160,
			RSASSA_2048_PKCS1_PSS_MGF1_SHA160,
			RSASSA_2048_PKCS1_V1_5_SHA256,
			RSASSA_2048_PKCS1_PSS_MGF1_SHA256,
			RSASSA_2048_PKCS1_V1_5_SHA384,
			RSASSA_2048_PKCS1_PSS_MGF1_SHA384,
			RSASSA_2048_PKCS1_V1_5_SHA512,
			RSASSA_2048_PKCS1_PSS_MGF1_SHA512,
	};
	unsigned int hash_type[SEE_RSASSA_SV_TC] = {
			HASH_SHA1_160, HASH_SHA1_160, HASH_SHA2_256, HASH_SHA2_256,
			HASH_SHA2_384, HASH_SHA2_384, HASH_SHA2_512, HASH_SHA2_512,
			HASH_SHA1_160, HASH_SHA1_160, HASH_SHA2_256, HASH_SHA2_256,
			HASH_SHA2_384, HASH_SHA2_384, HASH_SHA2_512, HASH_SHA2_512
	};
	unsigned char msg[300];
	unsigned int hash_size;
	unsigned int salt_size[SEE_RSASSA_SV_TC] = { 0, 20, 0, 32, 0, 48, 0, 48, 0, 20, 0, 32, 0, 48, 0, 64 };
	unsigned char *hash = NULL;
	unsigned char *out = NULL;
	unsigned int outlen = 0;

	fprintf(stderr, "------------------------------------------------------\n");
	fprintf(stderr, "  SECURITY SDK TESTCASE : rsassa_sign_verify\n");
	fprintf(stderr, "------------------------------------------------------\n");

	security = (artik_security_module *) artik_request_api_module("security");

	if (!security) {
		fprintf(stderr, "Security module is not available\n");
		return -1;
	}

	err = security->request(&handle);
	if (err != S_OK) {
		fprintf(stderr, "Failed to request security instance (err=%d)\n", err);
		err = -1;
		goto exit;
	}

	memset(msg, 0x35, sizeof(msg));

	for (i = 0; i < 9; i++) {
		memset(key_name, 0, sizeof(key_name));
		sprintf(key_name, "%s/%X", SECURE_STORAGE_DEFAULT, i);

		for (j = 0; j < SEE_RSASSA_SV_TC; j++) {
			if (j < 8) {
				ret = security->set_key(handle, RSA_1024, key_name, rsa1024_sample, sizeof(rsa1024_sample));
			} else {
				ret = security->set_key(handle, RSA_2048, key_name, rsa2048_sample, sizeof(rsa2048_sample));
			}

			ret += security->get_hash(handle, hash_type[j], msg, sizeof(msg), &hash, &hash_size);
			ret += security->get_rsa_signature(handle, algo_type[j], key_name, hash, hash_size, salt_size[j], &out, &outlen);
			ret += security->verify_rsa_signature(handle, algo_type[j], key_name, hash, hash_size, salt_size[j], out, outlen);

			see_selfprintf("[%d] ", cnt++);
			if (i >= 8 && ret) {
				see_selfprintf(" success\n");
			} else if (ret) {
				test_result++;
				see_selfprintf(" fail\n");
			} else {
				see_selfprintf(" success\n");
			}
		}

		if (hash != NULL) {
			free(hash);
			hash = NULL;
		}

		if (out != NULL) {
			free(out);
			out = NULL;
		}
	}

	for (i = 0; i < 9; i++) {
		memset(key_name, 0, sizeof(key_name));
		sprintf(key_name, "%s/%X", MEMORY_STORAGE, i % 4);

		for (j = 0; j < SEE_RSASSA_SV_TC; j++) {
			if (j < 8) {
				ret = security->set_key(handle, RSA_1024, key_name, rsa1024_sample, sizeof(rsa1024_sample));
			} else {
				ret = security->set_key(handle, RSA_2048, key_name, rsa2048_sample, sizeof(rsa2048_sample));
			}

			ret += security->get_hash(handle, hash_type[j], msg, sizeof(msg), &hash, &hash_size);
			ret += security->get_rsa_signature(handle, algo_type[j], key_name, hash, hash_size, salt_size[j], &out, &outlen);
			ret += security->verify_rsa_signature(handle, algo_type[j], key_name, hash, hash_size, salt_size[j], out, outlen);
			ret += security->remove_key(handle, algo_type[j], key_name);

			see_selfprintf("[%d] ", cnt++);
			if (i >= 8 && ret) {
				see_selfprintf(" success\n");
			} else if (ret) {
				test_result++;
				see_selfprintf(" fail\n");
			} else {
				see_selfprintf(" success\n");
			}

			if (hash != NULL) {
				free(hash);
				hash = NULL;
			}

			if (out != NULL) {
				free(out);
				out = NULL;
			}
		}
	}

exit:
	if (out != NULL) {
		free(out);
		out = NULL;
	}

	if (handle) {
		security->release(handle);
	}

	if (security) {
		artik_release_api_module(security);
	}

	return test_result;
}
