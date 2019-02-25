/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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
/*
 *  Self-test demonstration program
 *
 *  Copyright (C) 2006-2015, ARM Limited, All Rights Reserved
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  This file is part of mbed TLS (https://tls.mbed.org)
 */

#include <tinyara/config.h>

#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

#include "mbedtls/config.h"
#include "mbedtls/entropy.h"
#include "mbedtls/hmac_drbg.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/dhm.h"
#include "mbedtls/gcm.h"
#include "mbedtls/ccm.h"
#include "mbedtls/md2.h"
#include "mbedtls/md4.h"
#include "mbedtls/md5.h"
#include "mbedtls/ripemd160.h"
#include "mbedtls/sha1.h"
#include "mbedtls/sha256.h"
#include "mbedtls/sha512.h"
#include "mbedtls/arc4.h"
#include "mbedtls/des.h"
#include "mbedtls/aes.h"
#include "mbedtls/camellia.h"
#include "mbedtls/base64.h"
#include "mbedtls/bignum.h"
#include "mbedtls/rsa.h"
#include "mbedtls/x509.h"
#include "mbedtls/xtea.h"
#include "mbedtls/pkcs5.h"
#include "mbedtls/ecp.h"
#include "mbedtls/timing.h"

#define mbedtls_printf     printf
/*
 * Definition for handling pthread
 */
#define TLS_SELFTEST_PRIORITY     100
#define TLS_SELFTEST_STACK_SIZE   51200
#define TLS_SELFTEST_SCHED_POLICY SCHED_RR


#define DO_TLS_TEST(func, v) \
if ((ret = func(v)) != 0) { \
	printf("fail %d\n", ret); \
	fail_cnt++; \
}

pthread_addr_t tls_selftest_cb(void *args)
{
	int fail_cnt = 0;
	int ret = 0;
	int v = 1;
	void *pointer;

	/*
	 * The C standard doesn't guarantee that all-bits-0 is the representation
	 * of a NULL pointer. We do however use that in our code for initializing
	 * structures, which should work on every modern platform. Let's be sure.
	 */
	memset(&pointer, 0, sizeof(void *));
	if (pointer != NULL) {
		printf("all-bits-zero is not a NULL pointer1\n");
		mbedtls_printf("all-bits-zero is not a NULL pointer2\n");
		// return( 1 );
	}

#if defined(MBEDTLS_ECP_C)
	DO_TLS_TEST(mbedtls_ecp_self_test, v)
#endif
#if defined(MBEDTLS_MEMORY_BUFFER_ALLOC_C)
	mbedtls_memory_buffer_alloc_init(buf, sizeof(buf));
#endif
#if defined(MBEDTLS_MD2_C)
	DO_TLS_TEST(mbedtls_md2_self_test, v);
#endif
#if defined(MBEDTLS_MD4_C)
	DO_TLS_TEST(mbedtls_md4_self_test, v);
#endif
#if defined(MBEDTLS_MD5_C)
	DO_TLS_TEST(mbedtls_md5_self_test, v);
#endif
#if defined(MBEDTLS_RIPEMD160_C)
	DO_TLS_TEST(mbedtls_ripemd160_self_test, v);
#endif
#if defined(MBEDTLS_SHA1_C)
	DO_TLS_TEST(mbedtls_sha1_self_test, v);
#endif
#if defined(MBEDTLS_SHA256_C)
	DO_TLS_TEST(mbedtls_sha256_self_test, v);
#endif
#if defined(MBEDTLS_SHA512_C)
	DO_TLS_TEST(mbedtls_sha512_self_test, v);
#endif
#if defined(MBEDTLS_ARC4_C)
	DO_TLS_TEST(mbedtls_arc4_self_test, v);
#endif
#if defined(MBEDTLS_DES_C)
	DO_TLS_TEST(mbedtls_des_self_test, v);
#endif
#if defined(MBEDTLS_AES_C)
	DO_TLS_TEST(mbedtls_aes_self_test, v);
#endif
#if defined(MBEDTLS_GCM_C) && defined(MBEDTLS_AES_C)
	DO_TLS_TEST(mbedtls_gcm_self_test, v);
#endif
#if defined(MBEDTLS_CCM_C) && defined(MBEDTLS_AES_C)
	DO_TLS_TEST(mbedtls_ccm_self_test, v);
#endif
#if defined(MBEDTLS_BASE64_C)
	DO_TLS_TEST(mbedtls_base64_self_test, v);
#endif
#if defined(MBEDTLS_BIGNUM_C)
	DO_TLS_TEST(mbedtls_mpi_self_test, v);
#endif
#if defined(MBEDTLS_RSA_C)
	DO_TLS_TEST(mbedtls_rsa_self_test, v);
#endif
#if defined(MBEDTLS_X509_USE_C)
	DO_TLS_TEST(mbedtls_x509_self_test, v);
#endif
#if defined(MBEDTLS_XTEA_C)
	DO_TLS_TEST(mbedtls_xtea_self_test, v);
#endif
#if defined(MBEDTLS_CAMELLIA_C)
	DO_TLS_TEST(mbedtls_camellia_self_test, v);
#endif
#if defined(MBEDTLS_CTR_DRBG_C)
	DO_TLS_TEST(mbedtls_ctr_drbg_self_test, v);
#endif
#if defined(MBEDTLS_HMAC_DRBG_C)
	DO_TLS_TEST(mbedtls_hmac_drbg_self_test, v);
#endif
#if defined(MBEDTLS_ECJPAKE_C)
	DO_TLS_TEST(mbedtls_ecjpake_self_test, v);
#endif
#if defined(MBEDTLS_DHM_C)
	DO_TLS_TEST(mbedtls_dhm_self_test, v);
#endif

/*
 * Without HW entropy, there is no strong entropy source and
 * it will make failure the selftest.
 */
#if defined(MBEDTLS_ENTROPY_C)
	DO_TLS_TEST(mbedtls_entropy_self_test, v);
#endif
#if defined(MBEDTLS_PKCS5_C)
	DO_TLS_TEST(mbedtls_pkcs5_self_test, v);
#endif

	if (v != 0) {
#if defined(MBEDTLS_MEMORY_BUFFER_ALLOC_C) && defined(MBEDTLS_MEMORY_DEBUG)
		mbedtls_memory_buffer_alloc_status();
#endif
	}

#if defined(MBEDTLS_MEMORY_BUFFER_ALLOC_C)
	mbedtls_memory_buffer_alloc_free();

	DO_TLS_TEST(mbedtls_memory_buffer_alloc_self_test, v);
#endif
	if (v != 0) {
		if (fail_cnt) {
			printf("  [ Failed ]\n\n");
		} else {
			printf("  [ All tests passed ]\n\n");
		}
	}
	return NULL;
}

int tls_selftest_main(int argc, char **argv)
{
	pthread_t tid;
	pthread_attr_t attr;
	struct sched_param sparam;
	int r;

	/* Initialize the attribute variable */
	if ((r = pthread_attr_init(&attr)) != 0) {
		printf("%s: pthread_attr_init failed, status=%d\n", __func__, r);
	}

	/* 1. set a priority */
	sparam.sched_priority = TLS_SELFTEST_PRIORITY;
	if ((r = pthread_attr_setschedparam(&attr, &sparam)) != 0) {
		printf("%s: pthread_attr_setschedparam failed, status=%d\n", __func__, r);
	}

	if ((r = pthread_attr_setschedpolicy(&attr, TLS_SELFTEST_SCHED_POLICY)) != 0) {
		printf("%s: pthread_attr_setschedpolicy failed, status=%d\n", __func__, r);
	}

	/* 2. set a stacksize */
	if ((r = pthread_attr_setstacksize(&attr, TLS_SELFTEST_STACK_SIZE)) != 0) {
		printf("%s: pthread_attr_setstacksize failed, status=%d\n", __func__, r);
	}

	/* 3. create pthread with entry function */
	if ((r = pthread_create(&tid, &attr, tls_selftest_cb, (void *)0)) != 0) {
		printf("%s: pthread_create failed, status=%d\n", __func__, r);
	}

	/* Wait for the threads to stop */
	pthread_join(tid, NULL);

	return 0;
}

