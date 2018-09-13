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
 * examples/security_sdk/security_sdk_genkey.c
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

#define SEE_GENERATE_KEY_TC 12

int security_sdk_generate_key(void)
{
	artik_security_module *security = NULL;
	artik_security_handle handle = NULL;
	artik_error err = S_OK;
	struct hmac_key_param hmac_param;
	struct rsa_key_param rsa_param;
	int i, j, ret = 0, cnt = 0;
	unsigned int test_result = 0;
	char key_name_sk[20] = { 0 };
	char key_name_mem[20] = { 0 };
	unsigned int genkey_input[SEE_GENERATE_KEY_TC][3] = {
		{AES_128, 16, SEE_OK}, {AES_192, 24, SEE_OK}, {AES_256, 32, SEE_OK},
		{HMAC_ALGORITHM, 0, SEE_ERROR}, {HMAC_ALGORITHM, 128, SEE_OK},
		{HMAC_ALGORITHM, 129, SEE_ERROR},
		{ECC_BRAINPOOL_P256R1, 0, SEE_OK}, {ECC_SEC_P256R1, 0, SEE_OK},
		{ECC_SEC_P384R1, 0, SEE_OK}, {ECC_SEC_P521R1, 0, SEE_OK},
		{0xffff, 0, SEE_INVALID_INPUT_PARAMS}
	};

	fprintf(stderr, "------------------------------------------------------\n");
	fprintf(stderr, "  SECURITY SDK TESTCASE : generate key\n");
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

	for (j = 0; j < SEE_GENERATE_KEY_TC; j++) {
		for (i = 0; i < 1; i++) {
			see_selfprintf("[%d] ", cnt++);

			memset(key_name_sk, 0, sizeof(key_name_sk));
			sprintf(key_name_sk, "%s/%X", SECURE_STORAGE_DEFAULT, i);
			memset(key_name_mem, 0, sizeof(key_name_mem));
			sprintf(key_name_mem, "%s/%X", MEMORY_STORAGE, i);

			if ((genkey_input[j][0] & 0xF000) == HMAC_ALGORITHM) {
				hmac_param.key_size = genkey_input[j][1];
			}

			ret = security->generate_key(handle, genkey_input[j][0], key_name_sk, &hmac_param);
			ret += security->generate_key(handle, genkey_input[j][0], key_name_mem, &hmac_param);
			ret += security->remove_key(handle, genkey_input[j][0], key_name_mem);

			if (ret && genkey_input[j][2]) {
				see_selfprintf(" success\n");
			} else if (!ret && !genkey_input[j][2]) {
				see_selfprintf(" success\n");
			} else {
				test_result++;
				see_selfprintf(" fail\n");
			}
		}
	}

	unsigned char exp1[] = { 0x00, 0x00, 0x00, 0x00 };
	unsigned char exp2[] = { 0x00, 0x01, 0x00, 0x01 };
	unsigned int genkey_input_rsa[4][3] = {
		{RSA_1024, 0}, {RSA_2048, 0},
		{RSA_1024, 0}, {RSA_2048, 0}
	};
	unsigned char *exp[] = {
		exp1, exp1, exp2, exp2
	};

	for (j = 0; j < 4; j++) {
		see_selfprintf("[%d] ", cnt++);

		memset(key_name_sk, 0, sizeof(key_name_sk));
		sprintf(key_name_sk, "%s/%X", SECURE_STORAGE_DEFAULT, j);

		rsa_param.exponent_size = 4;
		rsa_param.exponent = (unsigned char *)exp[j];

		ret = security->generate_key(handle, genkey_input_rsa[j][0], key_name_sk, &rsa_param);

		if (ret && genkey_input_rsa[j][1]) {
			see_selfprintf(" success\n");
		} else if (!ret && !genkey_input_rsa[j][1]) {
			see_selfprintf(" success\n");
		} else {
			test_result++;
			see_selfprintf(" fail\n");
		}
	}

exit:

	if (handle) {
		security->release(handle);
	}

	if (security) {
		artik_release_api_module(security);
	}

	return test_result;
}
