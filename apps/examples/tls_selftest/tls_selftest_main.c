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

#include "tls/config.h"
#include "tls/entropy.h"
#include "tls/hmac_drbg.h"
#include "tls/ctr_drbg.h"
#include "tls/dhm.h"
#include "tls/gcm.h"
#include "tls/ccm.h"
#include "tls/md2.h"
#include "tls/md4.h"
#include "tls/md5.h"
#include "tls/ripemd160.h"
#include "tls/sha1.h"
#include "tls/sha256.h"
#include "tls/sha512.h"
#include "tls/arc4.h"
#include "tls/des.h"
#include "tls/aes.h"
#include "tls/camellia.h"
#include "tls/base64.h"
#include "tls/bignum.h"
#include "tls/rsa.h"
#include "tls/x509.h"
#include "tls/xtea.h"
#include "tls/pkcs5.h"
#include "tls/ecp.h"
#include "tls/timing.h"

#include <stdio.h>
#include <string.h>

#define mbedtls_printf     printf
/*
 * Definition for handling pthread
 */
#define TLS_SELFTEST_PRIORITY     100
#define TLS_SELFTEST_STACK_SIZE   51200
#define TLS_SELFTEST_SCHED_POLICY SCHED_RR

pthread_addr_t tls_selftest_cb(void *args)
{
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
	if ((ret = mbedtls_ecp_self_test(v)) != 0) {
		printf("FAIL %d\n", ret);
	}
#endif
#if defined(MBEDTLS_MEMORY_BUFFER_ALLOC_C)
	mbedtls_memory_buffer_alloc_init(buf, sizeof(buf));
#endif
#if defined(MBEDTLS_MD2_C)
	if ((ret = mbedtls_md2_self_test(v)) != 0) {
		printf("FAIL %d\n", ret);
	}
#endif

#if defined(MBEDTLS_MD4_C)
	if ((ret = mbedtls_md4_self_test(v)) != 0) {
		printf("FAIL %d\n", ret);
	}
#endif

#if defined(MBEDTLS_MD5_C)
	if ((ret = mbedtls_md5_self_test(v)) != 0) {
		printf("FAIL %d\n", ret);
	}
#endif

#if defined(MBEDTLS_RIPEMD160_C)
	if ((ret = mbedtls_ripemd160_self_test(v)) != 0) {
		printf("FAIL %d\n", ret);
	}
#endif

#if defined(MBEDTLS_SHA1_C)
	if ((ret = mbedtls_sha1_self_test(v)) != 0) {
		printf("FAIL %d\n", ret);
	}
#endif
#if defined(MBEDTLS_SHA256_C)
	if ((ret = mbedtls_sha256_self_test(v)) != 0) {
		printf("FAIL %d\n", ret);
	}
#endif

#if defined(MBEDTLS_SHA512_C)
	if ((ret = mbedtls_sha512_self_test(v)) != 0) {
		printf("FAIL %d\n", ret);
	}
#endif

#if defined(MBEDTLS_ARC4_C)
	if ((ret = mbedtls_arc4_self_test(v)) != 0) {
		printf("FAIL %d\n", ret);
	}
#endif

#if defined(MBEDTLS_DES_C)
	if ((ret = mbedtls_des_self_test(v)) != 0) {
		printf("FAIL %d\n", ret);
	}
#endif

#if defined(MBEDTLS_AES_C)
	if ((ret = mbedtls_aes_self_test(v)) != 0) {
		printf("FAIL %d\n", ret);
	}
#endif

#if defined(MBEDTLS_GCM_C) && defined(MBEDTLS_AES_C)
	if ((ret = mbedtls_gcm_self_test(v)) != 0) {
		printf("FAIL %d\n", ret);
	}
#endif

#if defined(MBEDTLS_CCM_C) && defined(MBEDTLS_AES_C)
	if ((ret = mbedtls_ccm_self_test(v)) != 0) {
		printf("FAIL %d\n", ret);
	}
#endif

#if defined(MBEDTLS_BASE64_C)
	if ((ret = mbedtls_base64_self_test(v)) != 0) {
		printf("FAIL %d\n", ret);
	}
#endif

#if defined(MBEDTLS_BIGNUM_C)
	if ((ret = mbedtls_mpi_self_test(v)) != 0) {
		printf("FAIL %d\n", ret);
	}
#endif

#if defined(MBEDTLS_RSA_C)
	if ((ret = mbedtls_rsa_self_test(v)) != 0) {
		printf("FAIL %d\n", ret);
	}
#endif

#if defined(MBEDTLS_X509_USE_C)
	if ((ret = mbedtls_x509_self_test(v)) != 0) {
		printf("FAIL %d\n", ret);
	}
#endif
#if defined(MBEDTLS_XTEA_C)
	if ((ret = mbedtls_xtea_self_test(v)) != 0) {
		printf("FAIL %d\n", ret);
	}
#endif

#if defined(MBEDTLS_CAMELLIA_C)
	if ((ret = mbedtls_camellia_self_test(v)) != 0) {
		printf("FAIL %d\n", ret);
	}
#endif

#if defined(MBEDTLS_CTR_DRBG_C)
	if ((ret = mbedtls_ctr_drbg_self_test(v)) != 0) {
		printf("FAIL %d\n", ret);
	}
#endif

#if defined(MBEDTLS_HMAC_DRBG_C)
	if ((ret = mbedtls_hmac_drbg_self_test(v)) != 0) {
		printf("FAIL %d\n", ret);
	}
#endif


#if defined(MBEDTLS_ECJPAKE_C)
	if ((ret = mbedtls_ecjpake_self_test(v)) != 0) {
		printf("FAIL %d\n", ret);
	}
#endif

#if defined(MBEDTLS_DHM_C)
	if ((ret = mbedtls_dhm_self_test(v)) != 0) {
		printf("FAIL %d\n", ret);
	}
#endif

#if defined(MBEDTLS_ENTROPY_C)
	if ((ret = mbedtls_entropy_self_test(v)) != 0) {
		printf("FAIL %d\n", ret);
	}
#endif

#if defined(MBEDTLS_PKCS5_C)
	if ((ret = mbedtls_pkcs5_self_test(v)) != 0) {
		printf("FAIL %d\n", ret);
	}
#endif

	if (v != 0) {
#if defined(MBEDTLS_MEMORY_BUFFER_ALLOC_C) && defined(MBEDTLS_MEMORY_DEBUG)
		mbedtls_memory_buffer_alloc_status();
#endif
	}

#if defined(MBEDTLS_MEMORY_BUFFER_ALLOC_C)
	mbedtls_memory_buffer_alloc_free();

	if ((ret = mbedtls_memory_buffer_alloc_self_test(v)) != 0) {
		return NULL;
	}
#endif
	if (v != 0) {
		printf("  [ All tests passed ]\n\n");
	}
	printf("  [ fail ]\n\n");

	sleep(2);
	if (ret != 0) {
		printf("FAIL %x\n", ret);
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

