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
 * examples/security_sdk/security_sdk_hmac.c
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

#define SEE_GET_HMAC_TC 4

int security_sdk_get_hmac(void)
{
	artik_security_module *security = NULL;
	artik_security_handle handle = NULL;
	artik_error err = S_OK;
	int cnt = 0, i, j, ret = 0;
	unsigned int test_result = 0;
	char key_name[8] = { 0 };
	unsigned char input_hmac[300] = { 0 };
	unsigned int input_size[] = {32, 64, 96, 128, 160};
	unsigned int input_oid[SEE_GET_HMAC_TC] = {
		HASH_SHA1_160, HASH_SHA2_256, HASH_SHA2_384, HASH_SHA2_512,
	};
	unsigned int expect_result[] = {
			SECURITY_SDK_OK, SECURITY_SDK_OK, SECURITY_SDK_OK, SECURITY_SDK_OK,	SECURITY_SDK_OK,
			SECURITY_SDK_OK, SECURITY_SDK_OK, SECURITY_SDK_OK, SECURITY_SDK_ERROR, SECURITY_SDK_ERROR,
			SECURITY_SDK_OK, SECURITY_SDK_OK, SECURITY_SDK_ERROR, SECURITY_SDK_ERROR, SECURITY_SDK_OK,
			SECURITY_SDK_OK, SECURITY_SDK_ERROR, SECURITY_SDK_ERROR, SECURITY_SDK_ERROR, SECURITY_SDK_ERROR,
			SECURITY_SDK_OK, SECURITY_SDK_OK, SECURITY_SDK_OK, SECURITY_SDK_OK,	SECURITY_SDK_OK,
			SECURITY_SDK_OK, SECURITY_SDK_OK, SECURITY_SDK_OK, SECURITY_SDK_ERROR, SECURITY_SDK_ERROR,
			SECURITY_SDK_OK, SECURITY_SDK_OK, SECURITY_SDK_ERROR, SECURITY_SDK_ERROR, SECURITY_SDK_OK,
			SECURITY_SDK_OK, SECURITY_SDK_ERROR, SECURITY_SDK_ERROR, SECURITY_SDK_ERROR, SECURITY_SDK_ERROR
	};
	unsigned char *out = NULL;
	unsigned int outlen = 0;

	fprintf(stderr, "------------------------------------------------------\n");
	fprintf(stderr, "  SECURITY SDK TESTCASE : get_hmac\n");
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

	for (i = 0; i < sizeof(input_size)/sizeof(int); i++) {
		for (j = 0; j < SEE_GET_HMAC_TC; j++) {
			memset(key_name, 0, sizeof(key_name));
			sprintf(key_name, "%s/%X", SECURE_STORAGE_DEFAULT, j);

			outlen = 0;
			ret = security->set_key(handle, HMAC_ALGORITHM, key_name, input_hmac, input_size[i]);
			ret += security->get_hmac(handle, input_oid[j], key_name, input_hmac, sizeof(input_hmac), &out, &outlen);

			see_selfprintf("[%d] ", cnt++);

			if (ret && expect_result[cnt - 1]) {
				see_selfprintf(" success\n");
			} else if (!ret && !expect_result[cnt - 1]) {
				see_selfprintf(" success\n");
			} else {
				test_result++;
				see_selfprintf(" fail\n");
			}

			if (out != NULL) {
				free(out);
				out = NULL;
			}
		}
	}

	for (i = 0; i < sizeof(input_size)/sizeof(int); i++) {
		for (j = 0; j < SEE_GET_HMAC_TC; j++) {
			memset(key_name, 0, sizeof(key_name));
			sprintf(key_name, "%s/%X", MEMORY_STORAGE, j);

			ret = security->set_key(handle, HMAC_ALGORITHM, key_name, input_hmac, input_size[i]);
			ret += security->get_hmac(handle, input_oid[j], key_name, input_hmac, sizeof(input_hmac), &out, &outlen);
			ret += security->remove_key(handle, HMAC_ALGORITHM, key_name);

			see_selfprintf("[%d] ", cnt++);

			if (ret && expect_result[cnt - 1]) {
				see_selfprintf(" success\n");
			} else if (!ret && !expect_result[cnt - 1]) {
				see_selfprintf(" success\n");
			} else {
				test_result++;
				see_selfprintf(" fail\n");
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
