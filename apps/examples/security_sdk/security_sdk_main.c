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
 * examples/security_sdk/security_sdk_main.c
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

/*
 * Definition for handling pthread
 */
#define SECURITY_TEST_PRIORITY     100
#define SECURITY_TEST_STACK_SIZE   8096
#define SECURITY_TEST_SCHED_POLICY SCHED_RR

#define SEE_CERTNUM 1

struct pthread_arg {
	int argc;
	char **argv;
};

pthread_addr_t security_sdk_cb(void *args)
{
	int i;
	int test_result = 0;
	int debug_mode = 0;
	int factory_test = 0;
	unsigned int test_select = 0xFFFF;

	int argc;
	char **argv;
	char *p, *q;

	argc = ((struct pthread_arg *)args)->argc;
	argv = ((struct pthread_arg *)args)->argv;

	for (i = 1; i < argc; i++) {
		p = argv[i];
		q = strchr(p, '=');
		if (q == NULL) {
			break;
		}

		*q++ = '\0';

		if ((strcmp(p, "test_select")) == 0) {
			test_select = atoi(q);
		}

		if ((strcmp(p, "fac")) == 0) {
			debug_mode = atoi(q);
			factory_test = 1;
		}
	}

	if (factory_test) {
		security_sdk_init_existence();
		security_sdk_factory_key_test(debug_mode);

		printf("\n\n-----------------\n");
		printf("Key existence\n");
		printf("-----------------\n");
		security_sdk_print_existence();
		printf("\n");
		return 0;
	}

	unsigned char test_name[][50] = {
		"security_sdk_get_certificates",
		"security_sdk_cert_serial",
		"security_sdk_rand",
		"security_sdk_cert_publickey",
		"security_sdk_rsa_encrypt_decrypt",
		"security_sdk_aes_encrypt_decrypt",
		"security_sdk_secure_storage",
		"security_sdk_generate_key",
		"security_sdk_setup_remove_key",
		"security_sdk_get_hash",
		"security_sdk_get_hmac",
		"security_sdk_rsassa_sign_verify",
		"security_sdk_ecdsa_sign_verify",
		"security_sdk_set_compute_dhm_params",
		"security_sdk_set_compute_ecdh_params",
		"security_sdk_get_publickey"
	};

	printf("\n\n");

	if (test_select & 0x1) {
		if (security_sdk_get_certificates()) {
			test_result |= 0x1;
		}
	}
	if (test_select & 0x2) {
		if (security_sdk_cert_serial()) {
			test_result |= 0x2;
		}
	}
	if (test_select & 0x4) {
		if (security_sdk_random()) {
			test_result |= 0x4;
		}
	}
	if (test_select & 0x8) {
		if (security_sdk_cert_publickey()) {
			test_result |= 0x8;
		}
	}
	if (test_select & 0x10) {
		if (security_sdk_rsa_encrypt_decrypt()) {
			test_result |= 0x10;
		}
	}
	if (test_select & 0x20) {
		if (security_sdk_aes_encrypt_decrypt()) {
			test_result |= 0x20;
		}
	}
	if (test_select & 0x40) {
		if (security_sdk_secure_storage()) {
			test_result |= 0x40;
		}
	}
	if (test_select & 0x80) {
		if (security_sdk_generate_key()) {
			test_result |= 0x80;
		}
	}
	if (test_select & 0x100) {
		if (security_sdk_setup_remove_key()) {
			test_result |= 0x100;
		}
	}
	if (test_select & 0x200) {
		if (security_sdk_get_hash()) {
			test_result |= 0x200;
		}
	}
	if (test_select & 0x400) {
		if (security_sdk_get_hmac()) {
			test_result |= 0x400;
		}
	}
	if (test_select & 0x800) {
		if (security_sdk_rsassa_sign_verify()) {
			test_result |= 0x800;
		}
	}
	if (test_select & 0x1000) {
		if (security_sdk_ecdsa_sign_verify()) {
			test_result |= 0x1000;
		}
	}
	if (test_select & 0x2000) {
		if (security_sdk_set_compute_dhm_params()) {
			test_result |= 0x2000;
		}
	}

	if (test_select & 0x4000) {
		if (security_sdk_set_compute_ecdh_params()) {
			test_result |= 0x4000;
		}
	}
	if (test_select & 0x8000) {
		if (security_sdk_get_publickey()) {
			test_result |= 0x8000;
		}
	}
	//Test Result
	for (i = 0; i < ARRAY_SIZE(test_name); i++) {
		if (!(test_select & 0x1)) {
			printf("%40s not tested\n", test_name[i]);
		} else if (test_result & 0x1) {
			printf("%40s fail\n", test_name[i]);
		} else {
			printf("%40s success\n", test_name[i]);
		}
		test_result = test_result >> 1;
		test_select = test_select >> 1;
	}

	return NULL;
}

int security_sdk_main(int argc, char **argv)
{
	pthread_t tid;
	pthread_attr_t attr;
	struct sched_param sparam;
	int r;

	struct pthread_arg args;

	args.argc = argc;
	args.argv = argv;

	/* Initialize the attribute variable */
	r = pthread_attr_init(&attr);
	if (r != 0) {
		printf("%s: pthread_attr_init failed, status=%d\n", __func__, r);
	}

	/* 1. set a priority */
	sparam.sched_priority = SECURITY_TEST_PRIORITY;
	r = pthread_attr_setschedparam(&attr, &sparam);
	if (r != 0) {
		printf("%s: pthread_attr_setschedparam failed, status=%d\n", __func__, r);
	}

	r = pthread_attr_setschedpolicy(&attr, SECURITY_TEST_SCHED_POLICY);
	if (r != 0) {
		printf("%s: pthread_attr_setschedpolicy failed, status=%d\n", __func__, r);
	}

	/* 2. set a stacksize */
	r = pthread_attr_setstacksize(&attr, SECURITY_TEST_STACK_SIZE);
	if (r != 0) {
		printf("%s: pthread_attr_setstacksize failed, status=%d\n", __func__, r);
	}

	/* 3. create pthread with entry function */
	r = pthread_create(&tid, &attr, security_sdk_cb, (void *)&args);
	if (r != 0) {
		printf("%s: pthread_create failed, status=%d\n", __func__, r);
	}

	/* Wait for the threads to stop */
	r = pthread_join(tid, NULL);
	if (r != 0) {
		printf("%s: pthread_join failed, status=%d\n", __func__, r);
	}

	printf("=====SSS Block Test Finish=====\n");

	return 0;
}
