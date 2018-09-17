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
 * examples/security_sdk/security_sdk_getpub.c
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

int security_sdk_get_publickey(void)
{
	artik_security_module *security = NULL;
	artik_security_handle handle = NULL;
	artik_error err = S_OK;
	struct rsa_key_param rsa_param;
	char key_name[8] = { 0 };
	int ret = 1, i, j, cnt = 0;
	unsigned int test_result = 0;
	unsigned char *out = NULL;
	unsigned int outlen = 0;
	unsigned int genkey_input[6][3] = {
		{ECC_BRAINPOOL_P256R1, 0, SECURITY_SDK_OK}, {ECC_SEC_P256R1, 0, SECURITY_SDK_OK},
		{ECC_SEC_P384R1, 0, SECURITY_SDK_OK}, {ECC_SEC_P521R1, 0, SECURITY_SDK_OK},
		{RSA_1024, 0, SECURITY_SDK_OK}, {RSA_2048, 0, SECURITY_SDK_OK}
	};
	unsigned char exp[] = { 0x00, 0x00, 0x00, 0x05 };
	unsigned char ecc_pair[] = {
			0x30, 0x77, 0x02, 0x01, 0x01, 0x04, 0x20, 0xA4, 0xF4, 0xDE, 0xDF, 0x15, 0x8F, 0x0B, 0xB8, 0xEA,
			0x94, 0x84, 0xE9, 0xF5, 0x9D, 0x9E, 0x38, 0xE0, 0x1C, 0x00, 0xE7, 0xC2, 0x06, 0xDE, 0x9A, 0x15,
			0x7C, 0xAF, 0x72, 0xA5, 0x2A, 0x4C, 0x5C, 0xA0, 0x0A, 0x06, 0x08, 0x2A, 0x86, 0x48, 0xCE, 0x3D,
			0x03, 0x01, 0x07, 0xA1, 0x44, 0x03, 0x42, 0x00, 0x04, 0xA3, 0x55, 0xAE, 0xFA, 0xD1, 0x50, 0x43,
			0x69, 0x66, 0x48, 0x19, 0x9C, 0x59, 0x71, 0x70, 0x86, 0x59, 0xB7, 0x2E, 0x49, 0x97, 0x5B, 0x1E,
			0x81, 0x09, 0x4E, 0x6C, 0xB7, 0x53, 0x15, 0xC3, 0xA2, 0xF3, 0x69, 0xE6, 0x05, 0x48, 0x02, 0xE2,
			0xE8, 0xD1, 0x16, 0x65, 0x49, 0x00, 0x82, 0xCD, 0x15, 0xA0, 0xFD, 0x87, 0xD5, 0x49, 0x83, 0x03,
			0x62, 0x66, 0x29, 0xD5, 0x23, 0xA9, 0x83, 0xB4, 0xCD
	};
	unsigned char ecc_pub[] = {
			0x30, 0x59, 0x30, 0x13, 0x06, 0x07, 0x2A, 0x86, 0x48, 0xCE, 0x3D, 0x02, 0x01, 0x06, 0x08, 0x2A,
			0x86, 0x48, 0xCE, 0x3D, 0x03, 0x01, 0x07, 0x03, 0x42, 0x00, 0x04, 0xA3, 0x55, 0xAE, 0xFA, 0xD1,
			0x50, 0x43, 0x69, 0x66, 0x48, 0x19, 0x9C, 0x59, 0x71, 0x70, 0x86, 0x59, 0xB7, 0x2E, 0x49, 0x97,
			0x5B, 0x1E, 0x81, 0x09, 0x4E, 0x6C, 0xB7, 0x53, 0x15, 0xC3, 0xA2, 0xF3, 0x69, 0xE6, 0x05, 0x48,
			0x02, 0xE2, 0xE8, 0xD1, 0x16, 0x65, 0x49, 0x00, 0x82, 0xCD, 0x15, 0xA0, 0xFD, 0x87, 0xD5, 0x49,
			0x83, 0x03, 0x62, 0x66, 0x29, 0xD5, 0x23, 0xA9, 0x83, 0xB4, 0xCD
	};
	unsigned char rsa1024_pub[] = {
			0x30, 0x81, 0x9F, 0x30, 0x0D, 0x06, 0x09, 0x2A, 0x86, 0x48, 0x86, 0xF7, 0x0D, 0x01, 0x01, 0x01,
			0x05, 0x00, 0x03, 0x81, 0x8D, 0x00, 0x30, 0x81, 0x89, 0x02, 0x81, 0x81, 0x00, 0xAC, 0x5A, 0x9D,
			0xDA, 0xD4, 0x90, 0xCE, 0xA1, 0xCD, 0x05, 0xE0, 0x84, 0xCD, 0x25, 0xB1, 0x8C, 0x8A, 0xAD, 0x8F,
			0xAC, 0xE3, 0xD7, 0x3D, 0x4B, 0xF3, 0xC6, 0xEF, 0xD2, 0x82, 0xCE, 0x3F, 0xB4, 0x03, 0xEC, 0x60,
			0xB9, 0x20, 0x39, 0xE8, 0x2E, 0x2D, 0xCF, 0x52, 0xB9, 0x3D, 0x8B, 0x6C, 0x5F, 0xA5, 0xD6, 0xAB,
			0x30, 0xD0, 0xD0, 0x0B, 0x00, 0x2A, 0x0B, 0x21, 0xD3, 0x6A, 0x82, 0x6F, 0x37, 0xFF, 0xB9, 0x00,
			0xCD, 0x1E, 0xD7, 0xD6, 0x80, 0x73, 0x53, 0xF4, 0x75, 0xB9, 0x73, 0xA4, 0x99, 0xEE, 0xB7, 0xE7,
			0x79, 0xEE, 0x8A, 0xAD, 0x92, 0x0B, 0xE9, 0x82, 0x08, 0xC4, 0x9E, 0xA1, 0x58, 0xE7, 0x92, 0x1B,
			0x5C, 0x30, 0x7A, 0x01, 0xC9, 0x9E, 0x1F, 0x80, 0x4E, 0x64, 0xBE, 0x19, 0x7E, 0x2A, 0x20, 0x1B,
			0xAB, 0xFF, 0x10, 0x85, 0x08, 0x60, 0x6E, 0xBD, 0xDE, 0xAF, 0xEA, 0x11, 0xB5, 0x02, 0x03, 0x01,
			0x00, 0x01
	};

	fprintf(stderr, "------------------------------------------------------\n");
	fprintf(stderr, "  SECURITY SDK TESTCASE : get_publickey\n");
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

	/*
	 * ARTIK Device Public Key
	 */
	ret = security->get_publickey(handle, ECC_SEC_P256R1, ARTIK_DEVICE_KEY_ID, &out, &outlen);

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

	unsigned char *sig = NULL;
	unsigned int siglen;
	unsigned char hash[32] = {0};

	memset(hash, 0, 32);

	memset(key_name, 0, sizeof(key_name));
	sprintf(key_name, "%s/%X", SECURE_STORAGE_DEFAULT, 1);
	ret = security->set_key(handle, ECC_SEC_P256R1, key_name, ecc_pair, sizeof(ecc_pair));

	ret += security->get_ecdsa_signature(handle, ECC_SEC_P256R1, key_name, hash, 32, &sig, &siglen);
	if (security->verify_ecdsa_signature(handle, ECC_SEC_P256R1, key_name, hash, 32, sig, siglen) != 0) {
		see_selfprintf("Signature verification fail\n");
	}

	ret += security->get_publickey(handle, ECC_SEC_P256R1, key_name, &out, &outlen);

	see_selfprintf("[%d] ", cnt++);
	if (ret || memcmp(out, ecc_pub, sizeof(ecc_pub))) {
		test_result++;
		see_selfprintf(" fail : %d\n", ret);
	} else {
		see_selfprintf(" success\n");
	}

	free(out);
	out = NULL;

	ret = security->set_key(handle, RSA_1024, key_name, rsa1024_pub, sizeof(rsa1024_pub));
	ret += security->get_publickey(handle, RSA_1024, key_name, &out, &outlen);

	see_selfprintf("[%d] ", cnt++);
	if (ret || memcmp(out, rsa1024_pub, sizeof(rsa1024_pub))) {
		test_result++;
		see_selfprintf(" fail : %d\n", ret);
	} else {
		see_selfprintf(" success\n");
	}

	free(out);
	out = NULL;

	for (i = 0; i < 9; i++) {
		memset(key_name, 0, sizeof(key_name));
		sprintf(key_name, "%s/%X", SECURE_STORAGE_DEFAULT, i);

		for (j = 0; j < 6; j++) {
			if (genkey_input[j][0] == RSA_1024 || genkey_input[j][0] == RSA_2048) {
				rsa_param.exponent_size = 4;
				rsa_param.exponent = exp;
			}

			ret = security->generate_key(handle, genkey_input[j][0], key_name, &rsa_param);
			ret += security->get_publickey(handle, genkey_input[j][0], key_name, &out, &outlen);

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
		}
	}

	/*
	 * Does not support RSA in encrypted key
	 */
	for (i = 0; i < 4; i++) {
		memset(key_name, 0, sizeof(key_name));
		sprintf(key_name, "%s/%X", MEMORY_STORAGE, i);

		for (j = 0; j < 6; j++) {
			if (genkey_input[j][0] == RSA_1024 || genkey_input[j][0] == RSA_2048) {
				rsa_param.exponent = exp;
			}

			ret = security->generate_key(handle, genkey_input[j][0], key_name, &rsa_param);
			ret += security->get_publickey(handle, genkey_input[j][0], key_name, &out, &outlen);
			ret += security->remove_key(handle, genkey_input[j][0], key_name);

			see_selfprintf("[%d] ", cnt++);

			if ((j == 4 || j == 5) && ret) {
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

