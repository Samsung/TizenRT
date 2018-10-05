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
 * examples/security_sdk/security_sdk_dhm.c
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
#include <security_sdk_dhm.h>

int security_sdk_set_compute_dhm_params(void)
{
	artik_security_module *security = NULL;
	artik_security_handle handle = NULL;
	artik_error err = S_OK;
	char key_name[8] = {0};
	int ret = 1, i, cnt = 0;
	unsigned int test_result = 0;
	unsigned char *pub = NULL;
	unsigned int publen = 0;
	unsigned char *secret = NULL;
	unsigned int secretlen = 0;

	unsigned char *input_key[][3] = {
			{dh1024_key, dh1024_key_pub, dh1024_secret},
			{dh1024_5114_key, dh1024_5114_key_pub, dh1024_5114_secret},
			{dh2048_key, dh2048_key_pub, dh2048_secret},
			{dh2048_5114_key, dh2048_5114_key_pub, dh2048_5114_secret}
	};
	unsigned int input_key_size[][3] = {
			{sizeof(dh1024_key), sizeof(dh1024_key_pub), sizeof(dh1024_secret)},
			{sizeof(dh1024_5114_key), sizeof(dh1024_5114_key_pub), sizeof(dh1024_5114_secret)},
			{sizeof(dh2048_key), sizeof(dh2048_key_pub), sizeof(dh2048_secret)},
			{sizeof(dh2048_5114_key), sizeof(dh2048_5114_key_pub), sizeof(dh2048_5114_secret)}
	};

	unsigned char *input_param[] = {
			dh1024_pg, dh1024_5114_pg, dh2048_pg, dh2048_5114_pg
	};
	unsigned int input_param_size[] = {
			sizeof(dh1024_pg), sizeof(dh1024_5114_pg), sizeof(dh2048_pg),
			sizeof(dh2048_5114_pg)
	};
	unsigned int algo;

	fprintf(stderr, "------------------------------------------------------\n");
	fprintf(stderr, "  SECURITY SDK TESTCASE : generate_compute_dhm_params\n");
	fprintf(stderr, "------------------------------------------------------\n");

	security = (artik_security_module *)artik_request_api_module("security");

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

	/*
	 * Validation Test
	 */
	for (i = 0; i < sizeof(input_key_size)/(sizeof(int)*3); i++) {
		memset(key_name, 0, sizeof(key_name));
		sprintf(key_name, "%s/%X", SECURE_STORAGE_DEFAULT, i);

		if (i < 2) {
			algo = DH_1024;
		} else {
			algo = DH_2048;
		}

		ret = security->set_key(handle, algo, key_name, input_key[i][0], input_key_size[i][0]);
		ret += security->compute_dhm_params(handle, key_name,
				input_key[i][1], input_key_size[i][1],
				&secret, &secretlen);

		see_selfprintf("[%d] ", cnt++);

		if (i >= 8 && ret) {
			see_selfprintf(" success\n");
		} else if (ret) {
			test_result++;
			see_selfprintf(" fail\n");
		} else {
			if (!memcmp(input_key[i][2], secret, secretlen)) {
				see_selfprintf(" success\n");
			} else {
				test_result++;
				see_selfprintf(" fail\n");
			}
		}

		if (pub != NULL && publen > 0) {
			free(pub);
			pub = NULL;
			publen = 0;
		}

		if (secret != NULL && secretlen > 0) {
			free(secret);
			secret = NULL;
			secretlen = 0;
		}
	}

	/*
	 * Function Test
	 */
	for (i = 0; i < sizeof(input_param_size)/sizeof(int); i++) {
		memset(key_name, 0, sizeof(key_name));
		sprintf(key_name, "%s/%X", SECURE_STORAGE_DEFAULT, i);

		ret = security->set_dhm_params(handle, key_name,
				input_param[i], input_param_size[i], &pub, &publen);
		ret += security->compute_dhm_params(handle, key_name,
				pub, publen, &secret, &secretlen);

		see_selfprintf("[%d] ", cnt++);

		if (i >= 8 && ret) {
			see_selfprintf(" success\n");
		} else if (ret) {
			test_result++;
			see_selfprintf(" fail\n");
		} else {
			see_selfprintf(" success\n");
		}

		if (pub != NULL && publen > 0) {
			free(pub);
			pub = NULL;
			publen = 0;
		}

		if (secret != NULL && secretlen > 0) {
			free(secret);
			secret = NULL;
			secretlen = 0;
		}
	}

exit:
	if (pub != NULL && publen > 0) {
		free(pub);
		pub = NULL;
	}

	if (secret != NULL && secretlen > 0) {
		free(secret);
		secret = NULL;
	}

	if (handle)
		security->release(handle);

	if (security)
		artik_release_api_module(security);

	return test_result;
}

