/****************************************************************************
 *
 * Copyright 2021 Samsung Electronics All Rights Reserved.
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
 * apps/system/sekey_test/sekey_test.c
 *
 *   Copyright (C) 2008, 2011-2012 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
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

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tinyara/security_hal.h>
#include <tinyara/seclink.h>
#include <mbedtls/x509_crt.h>

#ifdef CONFIG_ECDSA_KEY_SLOT
#define ECDSA_KEY_SLOT CONFIG_ECDSA_KEY_SLOT
#else
#define ECDSA_KEY_SLOT "0"
#endif

#ifdef CONFIG_CERT_SLOT
#define CERT_SLOT CONFIG_CERT_SLOT
#else
#define CERT_SLOT "1,2,3"
#endif

#define HASH_LEN 32
#define SIGN_LEN 32
#define CERT_MAX_SIZE 1024

static uint8_t key_test[128] = {
	0,
};
static uint8_t cert_test[128] = {
	0,
};

int sekey_test_main(int argc, char *argv[])
{
	int i;
	int ret;
	int res = 0;
	char *ptr;
	sl_ctx shnd;
	hal_result_e hres = 0;
	hal_ecdsa_mode ecdsa_mode;
	hal_data t_hash = {
		0,
	};
	hal_data sign = {
		0,
	};

	mbedtls_x509_crt cert = {
		0,
	};
	hal_data cert_out = {
		0,
	};
	char buf[256] = {
		0,
	};

	char key_temp[] = ECDSA_KEY_SLOT;
	char cert_temp[] = CERT_SLOT;
	int key_cnt = 0;
	int cert_cnt = 0;

	ret = sl_init(&shnd);
	if (ret != SECLINK_OK) {
		printf("[SECLINK] sl_init fail \n");
		return -1;
	}

	ptr = strtok(key_temp, ",");
	while (ptr != NULL) {
		key_test[key_cnt++] = atoi(ptr);
		ptr = strtok(NULL, ",");
	}

	ptr = strtok(cert_temp, ",");
	while (ptr != NULL) {
		cert_test[cert_cnt++] = atoi(ptr);
		ptr = strtok(NULL, ",");
	}

	t_hash.data = malloc(HASH_LEN);
	if (t_hash.data == NULL) {
		res = -1;
		printf("[sekey] Out of memory \n");
		goto exit;
	}
	memset(t_hash.data, 0xa5, HASH_LEN);
	t_hash.data_len = HASH_LEN;

	sign.data = malloc(SIGN_LEN);
	if (sign.data == NULL) {
		res = -1;
		printf("[sekey] Out of memory \n");
		goto exit;
	}
	memset(sign.data, 0, SIGN_LEN);

	ecdsa_mode.curve = HAL_ECDSA_SEC_P256R1;
	ecdsa_mode.hash_t = HAL_HMAC_SHA256;

	printf("Start Key & Cert Checking ...\n");

	for (i = 0; i < key_cnt; i++) {

		printf("[Key] ECDSA Sign Slot#%d ... ", key_test[i]);
		ret = sl_ecdsa_sign_md(shnd, ecdsa_mode, &t_hash, key_test[i], &sign, &hres);
		if (ret != SECLINK_OK || hres != HAL_SUCCESS) {
			printf("[ FAIL ] / Slot #%d / [%d][%d] \n", key_test[i], ret, hres);
			res = -1;
			goto exit;
		}
		printf("[ OK ]\n");

		printf("[Key] ECDSA Verify Slot#%d ... ", key_test[i]);
		ret = sl_ecdsa_verify_md(shnd, ecdsa_mode, &t_hash, &sign, key_test[i], &hres);
		if (ret != SECLINK_OK || hres != HAL_SUCCESS) {
			printf("[ FAIL ] / Slot #%d / [%d][%d] \n", key_test[i], ret, hres);
			res = -1;
			goto exit;
		}
		printf("[ OK ] \n");
	}

	cert_out.data = malloc(CERT_MAX_SIZE);
	if (cert_out.data == NULL) {
		res = -1;
		goto exit;
	}

	for (i = 0; i < cert_cnt; i++) {
		memset(buf, 0, sizeof(buf));
		memset(cert_out.data, 0, CERT_MAX_SIZE);
		cert_out.data_len = CERT_MAX_SIZE;

		printf("[Cert] Get Cert Slot#%d ... ", cert_test[i]);
		ret = sl_get_certificate(shnd, cert_test[i], &cert_out, &hres);
		if (ret != SECLINK_OK || hres != HAL_SUCCESS) {
			printf("[ FAIL ] / ret : %d, hres : %d \n", ret, hres);
			res = -1;
			goto exit;
		}
		printf("[ OK ] \n");

		mbedtls_x509_crt_init(&cert);

		printf("[Cert] Parsing Cert Slot#%d ... ", cert_test[i]);
		ret = mbedtls_x509_crt_parse(&cert, cert_out.data, cert_out.data_len);
		if (ret != 0) {
			printf("[ FAIL ] / ret : %d, hres : %d \n", ret, hres);
			res = -1;
			goto exit;
		}
		printf("[ OK ] \n");

		printf("[Cert] Get Cert Subject Slot#%d ... ", cert_test[i]);
		ret = mbedtls_x509_dn_gets(buf, sizeof(buf), &(cert.subject));
		if (ret <= 0) {
			printf("[ FAIL ] #%d cert is invalid.\n", cert_test[i]);
			res = -1;
			goto exit;
		}
		printf("[ OK ] \n");

		mbedtls_x509_crt_free(&cert);
	}

exit:
	if (t_hash.data != NULL) {
		free(t_hash.data);
	}
	if (sign.data != NULL) {
		free(sign.data);
	}
	if (cert_out.data != NULL) {
		free(cert_out.data);
	}
	mbedtls_x509_crt_free(&cert);
	sl_deinit(shnd);

	if (res == 0) {
		printf("[KEY & CERT] Validation ... OK \n");
	} else {
		printf("[KEY & CERT] Validation ... Fail\n");
	}

	return res;
}
