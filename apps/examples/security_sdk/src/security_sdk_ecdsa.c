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
 * examples/security_sdk/security_sdk_ecdsa.c
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
#include <security_sdk_ecc.h>

#define SEE_ECDSA_SV_TC 16

int security_sdk_ecdsa_sign_verify(void)
{
	artik_security_module *security = NULL;
	artik_security_handle handle = NULL;
	artik_error err = S_OK;
	char key_name[10] = { 0 };
	int ret = 1, i, j, cnt = 0;
	unsigned int test_result = 0;
	unsigned int algo_type[SEE_ECDSA_SV_TC] = {
		(ECC_BRAINPOOL_P256R1),
		(ECC_BRAINPOOL_P256R1),
		(ECC_BRAINPOOL_P256R1),
		(ECC_BRAINPOOL_P256R1),
		(ECC_SEC_P256R1),
		(ECC_SEC_P256R1),
		(ECC_SEC_P256R1),
		(ECC_SEC_P256R1),
		(ECC_SEC_P384R1),
		(ECC_SEC_P384R1),
		(ECC_SEC_P384R1),
		(ECC_SEC_P384R1),
		(ECC_SEC_P521R1),
		(ECC_SEC_P521R1),
		(ECC_SEC_P521R1),
		(ECC_SEC_P521R1)
	};
	unsigned int hash_size[SEE_ECDSA_SV_TC] = { 20, 32, 48, 64, 20, 32, 48, 64, 20, 32, 48, 64, 20, 32, 48, 64 };
	unsigned char *hash = NULL;
	unsigned char *out = NULL;
	unsigned int outlen = 0;

	fprintf(stderr, "------------------------------------------------------\n");
	fprintf(stderr, "  SECURITY SDK TESTCASE : ecdsa_sign_verify\n");
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

	for (i = 0; i < 9; i++) {
		memset(key_name, 0, sizeof(key_name));
		sprintf(key_name, "%s/%X", SECURE_STORAGE_DEFAULT, i);

		for (j = 0; j < SEE_ECDSA_SV_TC; j++) {
			if (j < 4) {
				ret = security->set_key(handle, ECC_BRAINPOOL_P256R1, key_name, test_ecdsa_dev_bp, sizeof(test_ecdsa_dev_bp));
			} else if (j < 8) {
				ret = security->set_key(handle, ECC_SEC_P256R1, key_name, test_ecdsa_256_dev_nist, sizeof(test_ecdsa_256_dev_nist));
			} else if (j < 12) {
				ret = security->set_key(handle, ECC_SEC_P384R1, key_name, test_ecdsa_384_dev_nist, sizeof(test_ecdsa_384_dev_nist));
			} else {
				ret = security->set_key(handle, ECC_SEC_P521R1, key_name, test_ecdsa_521_dev_nist, sizeof(test_ecdsa_521_dev_nist));
			}

			ret += security->get_random_bytes(handle, hash_size[j], &hash);
			ret += security->get_ecdsa_signature(handle, algo_type[j], key_name, hash, hash_size[j], &out, &outlen);
			ret += security->verify_ecdsa_signature(handle, algo_type[j], key_name, hash, hash_size[j], out, outlen);

			see_selfprintf("[%d] ", cnt++);

			if (i >= 8 && ret) {
				see_selfprintf(" success\n");
			} else if (ret) {
				test_result++;
				see_selfprintf(" fail\n");
			} else {
				see_selfprintf(" success\n");
			}

			if (out != NULL) {
				free(out);
				out = NULL;
			}

			if (hash != NULL) {
				free(hash);
				hash = NULL;
			}
		}
	}

	for (i = 0; i < 4; i++) {
		memset(key_name, 0, sizeof(key_name));
		sprintf(key_name, "%s/%X", MEMORY_STORAGE, i);

		for (j = 0; j < SEE_ECDSA_SV_TC; j++) {
			if (j < 4) {
				ret = security->set_key(handle, ECC_BRAINPOOL_P256R1, key_name, test_ecdsa_dev_bp, sizeof(test_ecdsa_dev_bp));
			} else if (j < 8) {
				ret = security->set_key(handle, ECC_SEC_P256R1, key_name, test_ecdsa_256_dev_nist, sizeof(test_ecdsa_256_dev_nist));
			} else if (j < 12) {
				ret = security->set_key(handle, ECC_SEC_P384R1, key_name, test_ecdsa_384_dev_nist, sizeof(test_ecdsa_384_dev_nist));
			} else {
				ret = security->set_key(handle, ECC_SEC_P521R1, key_name, test_ecdsa_521_dev_nist, sizeof(test_ecdsa_521_dev_nist));
			}

			ret += security->get_random_bytes(handle, hash_size[j], &hash);
			ret += security->get_ecdsa_signature(handle, algo_type[j], key_name, hash, hash_size[j], &out, &outlen);
			ret += security->verify_ecdsa_signature(handle, algo_type[j], key_name, hash, hash_size[j], out, outlen);
			ret += security->remove_key(handle, algo_type[j], key_name);

			see_selfprintf("[%d] ", cnt++);

			if (ret) {
				test_result++;
				see_selfprintf(" fail\n");
			} else {
				see_selfprintf(" success\n");
			}

			if (out != NULL) {
				free(out);
				out = NULL;
			}

			if (hash != NULL) {
				free(hash);
				hash = NULL;
			}
		}
	}

exit:
	if (out != NULL) {
		free(out);
		out = NULL;
	}

	if (hash != NULL) {
		free(hash);
		hash = NULL;
	}

	if (handle) {
		security->release(handle);
	}

	if (security) {
		artik_release_api_module(security);
	}

	return test_result;
}
