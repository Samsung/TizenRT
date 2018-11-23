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
 *  Simple DTLS server demonstration program
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

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "mbedtls/config.h"

#if !defined(MBEDTLS_SSL_SRV_C) || !defined(MBEDTLS_SSL_PROTO_DTLS) ||	\
	!defined(MBEDTLS_SSL_COOKIE_C) || !defined(MBEDTLS_NET_C) ||		\
	!defined(MBEDTLS_ENTROPY_C) || !defined(MBEDTLS_CTR_DRBG_C) ||		\
	!defined(MBEDTLS_X509_CRT_PARSE_C) || !defined(MBEDTLS_RSA_C) ||	\
	!defined(MBEDTLS_CERTS_C) || !defined(MBEDTLS_PEM_PARSE_C) ||		\
	!defined(MBEDTLS_TIMING_C)

int dtls_server_main(int argc, char **argv)
{
	printf("MBEDTLS_SSL_SRV_C and/or MBEDTLS_SSL_PROTO_DTLS and/or\n");
	printf("MBEDTLS_SSL_COOKIE_C and/or MBEDTLS_NET_C and/or\n");
	printf("MBEDTLS_ENTROPY_C and/or MBEDTLS_CTR_DRBG_C and/or\n");
	printf("MBEDTLS_X509_CRT_PARSE_C and/or MBEDTLS_RSA_C and/or\n");
	printf("MBEDTLS_CERTS_C and/or MBEDTLS_PEM_PARSE_C and/or\n");
	printf("MBEDTLS_TIMING_C not defined.\n");
	return 0;
}
#else

#define mbedtls_printf     printf
#define mbedtls_fprintf    fprintf
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/certs.h"
#include "mbedtls/x509.h"
#include "mbedtls/ssl.h"
#include "mbedtls/ssl_cookie.h"
#include "mbedtls/net.h"
#include "mbedtls/error.h"
#include "mbedtls/debug.h"
#include "mbedtls/timing.h"

#if defined(MBEDTLS_SSL_CACHE_C)
#include "mbedtls/ssl_cache.h"
#endif

#define READ_TIMEOUT_MS 10000	/* 5 seconds */
#define DEBUG_LEVEL 0

/*
 * Definition for handling pthread
 */
#define DTLS_SERVER_PRIORITY     100
#define DTLS_SERVER_STACK_SIZE   51200
#define DTLS_SERVER_SCHED_POLICY SCHED_RR

struct pthread_arg {
	int argc;
	char **argv;
};

static void my_debug(void *ctx, int level, const char *file, int line, const char *str)
{
	((void)level);

	mbedtls_fprintf((FILE *)ctx, "%s:%04d: %s", file, line, str);
	fflush((FILE *)ctx);
}

/****************************************************************************
 * dtls_server_main
 ****************************************************************************/

int dtls_server_cb(void *args)
{
	int ret;
	int len;
	int ots = 0xFFFF;
	size_t cliip_len;
	unsigned char buf[1024];
	const char *pers = "dtls_server";
	unsigned char client_ip[16] = { 0 };
	struct pthread_arg arg;
	mbedtls_ssl_cookie_ctx cookie_ctx;
	mbedtls_net_context listen_fd;
	mbedtls_net_context client_fd;
	mbedtls_entropy_context entropy;
	mbedtls_ctr_drbg_context ctr_drbg;
	mbedtls_ssl_context ssl;
	mbedtls_ssl_config conf;
	mbedtls_x509_crt srvcert;
	mbedtls_pk_context pkey;
	mbedtls_timing_delay_context timer;
#if defined(MBEDTLS_SSL_CACHE_C)
	mbedtls_ssl_cache_context cache;
#endif

	if (args) {
		arg.argc = ((struct pthread_arg *)args)->argc;
		arg.argv = ((struct pthread_arg *)args)->argv;
		if (arg.argc == 1) {
			ots = 1;
		}
	}

	mbedtls_net_init(&listen_fd);
	mbedtls_net_init(&client_fd);
	mbedtls_ssl_init(&ssl);
	mbedtls_ssl_config_init(&conf);
	mbedtls_ssl_cookie_init(&cookie_ctx);
#if defined(MBEDTLS_SSL_CACHE_C)
	mbedtls_ssl_cache_init(&cache);
#endif
	mbedtls_x509_crt_init(&srvcert);
	mbedtls_pk_init(&pkey);
	mbedtls_entropy_init(&entropy);
	mbedtls_ctr_drbg_init(&ctr_drbg);

#if defined(MBEDTLS_DEBUG_C)
	mbedtls_debug_set_threshold(DEBUG_LEVEL);
#endif

	/*
	 * 1. Load the certificates and private RSA key
	 */
	mbedtls_printf("\n  . Loading the server cert. and key...");
	fflush(stdout);

	/*
	 * This demonstration program uses embedded test certificates.
	 * Instead, you may want to use mbedtls_x509_crt_parse_file() to read the
	 * server and CA certificates, as well as mbedtls_pk_parse_keyfile().
	 */
	ret = mbedtls_x509_crt_parse(&srvcert, (const unsigned char *)mbedtls_test_srv_crt_rsa, mbedtls_test_srv_crt_rsa_len);
	if (ret != 0) {
		mbedtls_printf(" failed\n  !  mbedtls_x509_crt_parse returned %d\n\n", ret);
		goto exit;
	}

	ret = mbedtls_x509_crt_parse(&srvcert, (const unsigned char *)mbedtls_test_ca_crt_rsa, mbedtls_test_ca_crt_rsa_len);
	if (ret != 0) {
		mbedtls_printf(" failed\n  !  mbedtls_x509_crt_parse returned %d\n\n", ret);
		goto exit;
	}

	ret = mbedtls_pk_parse_key(&pkey, (const unsigned char *)mbedtls_test_srv_key_rsa, mbedtls_test_srv_key_rsa_len, NULL, 0);
	if (ret != 0) {
		mbedtls_printf(" failed\n  !  mbedtls_pk_parse_key returned %d\n\n", ret);
		goto exit;
	}

	mbedtls_printf(" ok\n");

	/*
	 * 2. Setup the "listening" UDP socket
	 */
	mbedtls_printf("  . Bind on udp/*/4433 ...");
	fflush(stdout);

	if ((ret = mbedtls_net_bind(&listen_fd, NULL, "4433", MBEDTLS_NET_PROTO_UDP)) != 0) {
		mbedtls_printf(" failed\n  ! mbedtls_net_bind returned %d\n\n", ret);
		goto exit;
	}

	mbedtls_printf(" ok\n");

	/*
	 * 3. Seed the RNG
	 */
	mbedtls_printf("  . Seeding the random number generator...");
	fflush(stdout);

	if ((ret = mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, (const unsigned char *)pers, strlen(pers))) != 0) {
		mbedtls_printf(" failed\n  ! mbedtls_ctr_drbg_seed returned %d\n", ret);
		goto exit;
	}

	mbedtls_printf(" ok\n");

	/*
	 * 4. Setup stuff
	 */
	mbedtls_printf("  . Setting up the DTLS data...");
	fflush(stdout);

	if ((ret = mbedtls_ssl_config_defaults(&conf, MBEDTLS_SSL_IS_SERVER, MBEDTLS_SSL_TRANSPORT_DATAGRAM, MBEDTLS_SSL_PRESET_DEFAULT)) != 0) {
		mbedtls_printf(" failed\n  ! mbedtls_ssl_config_defaults returned %d\n\n", ret);
		goto exit;
	}

	mbedtls_ssl_conf_rng(&conf, mbedtls_ctr_drbg_random, &ctr_drbg);
	mbedtls_ssl_conf_dbg(&conf, my_debug, stdout);

#if defined(MBEDTLS_SSL_CACHE_C)
	mbedtls_ssl_conf_session_cache(&conf, &cache, mbedtls_ssl_cache_get, mbedtls_ssl_cache_set);
#endif

	mbedtls_ssl_conf_ca_chain(&conf, srvcert.next, NULL);
	if ((ret = mbedtls_ssl_conf_own_cert(&conf, &srvcert, &pkey)) != 0) {
		mbedtls_printf(" failed\n  ! mbedtls_ssl_conf_own_cert returned %d\n\n", ret);
		goto exit;
	}

	if ((ret = mbedtls_ssl_cookie_setup(&cookie_ctx, mbedtls_ctr_drbg_random, &ctr_drbg)) != 0) {
		mbedtls_printf(" failed\n  ! mbedtls_ssl_cookie_setup returned %d\n\n", ret);
		goto exit;
	}

	mbedtls_ssl_conf_dtls_cookies(&conf, mbedtls_ssl_cookie_write, mbedtls_ssl_cookie_check, &cookie_ctx);

	if ((ret = mbedtls_ssl_setup(&ssl, &conf)) != 0) {
		mbedtls_printf(" failed\n  ! mbedtls_ssl_setup returned %d\n\n", ret);
		goto exit;
	}

	mbedtls_ssl_set_timer_cb(&ssl, &timer, mbedtls_timing_set_delay, mbedtls_timing_get_delay);

	mbedtls_printf(" ok\n");

reset:
#ifdef MBEDTLS_ERROR_C
	if (ret != 0) {
		char error_buf[100];
		mbedtls_strerror(ret, error_buf, 100);
		mbedtls_printf("Last error was: %d - %s\n\n", ret, error_buf);
	}
#endif

	mbedtls_net_free(&client_fd);

	mbedtls_ssl_session_reset(&ssl);

	/*
	 * 3. Wait until a client connects
	 */
	mbedtls_printf("  . Waiting for a remote connection ...");
	fflush(stdout);

	if (listen_fd.fd < 0) {
		goto exit;
	}
	if ((ret = mbedtls_net_accept(&listen_fd, &client_fd, client_ip, sizeof(client_ip), &cliip_len)) != 0) {
		mbedtls_printf(" failed\n  ! mbedtls_net_accept returned %d\n\n", ret);
		goto exit;
	}

	/* For HelloVerifyRequest cookies */
	if ((ret = mbedtls_ssl_set_client_transport_id(&ssl, client_ip, cliip_len)) != 0) {
		mbedtls_printf(" failed\n  ! " "mbedtls_ssl_set_client_transport_id() returned -0x%x\n\n", -ret);
		goto exit;
	}

	mbedtls_ssl_set_bio(&ssl, &client_fd, mbedtls_net_send, mbedtls_net_recv, mbedtls_net_recv_timeout);

	mbedtls_printf(" ok\n");

	/*
	 * 5. Handshake
	 */
	mbedtls_printf("  . Performing the DTLS handshake...");
	fflush(stdout);

	do {
		ret = mbedtls_ssl_handshake(&ssl);
	} while (ret == MBEDTLS_ERR_SSL_WANT_READ || ret == MBEDTLS_ERR_SSL_WANT_WRITE);

	if (ret == MBEDTLS_ERR_SSL_HELLO_VERIFY_REQUIRED) {
		mbedtls_printf(" hello verification requested\n");
		ret = 0;
		goto reset;
	} else if (ret != 0) {
		mbedtls_printf(" failed\n  ! mbedtls_ssl_handshake returned -0x%x\n\n", -ret);
		if (ots == 1) {
			goto exit;
		}
		goto reset;
	}

	mbedtls_printf(" ok\n");

	/*
	 * 6. Read the echo Request
	 */
	len = sizeof(buf) - 1;
	memset(buf, 0, sizeof(buf));

	do {
		ret = mbedtls_ssl_read(&ssl, buf, len);
	} while (ret == MBEDTLS_ERR_SSL_WANT_READ || ret == MBEDTLS_ERR_SSL_WANT_WRITE);

	if (ret <= 0) {
		switch (ret) {
		case MBEDTLS_ERR_SSL_TIMEOUT:
			mbedtls_printf(" timeout\n\n");
			if (ots != 1) {
				goto reset;
			}
			goto exit;

		case MBEDTLS_ERR_SSL_PEER_CLOSE_NOTIFY:
			mbedtls_printf(" connection was closed gracefully\n");
			ret = 0;
			goto close_notify;

		default:
			mbedtls_printf(" mbedtls_ssl_read returned -0x%x\n", -ret);
			if (ots != 1) {
				goto reset;
			}
			goto exit;
		}
	}

	len = ret;
	mbedtls_printf(" < READ FROM CLIENT %d bytes read : %s\n", len, buf);

	/*
	 * 7. Write the 200 Response
	 */

	do {
		ret = mbedtls_ssl_write(&ssl, buf, len);
	} while (ret == MBEDTLS_ERR_SSL_WANT_READ || ret == MBEDTLS_ERR_SSL_WANT_WRITE);

	if (ret < 0) {
		mbedtls_printf(" failed\n  ! mbedtls_ssl_write returned %d\n", ret);
		goto exit;
	}

	len = ret;
	mbedtls_printf(" > WRITE TO CLIENT %d bytes written : %s\n", len, buf);

	/*
	 * 8. Done, cleanly close the connection
	 */
close_notify:
	mbedtls_printf("  . Closing the connection...");

	/* No error checking, the connection might be closed already */
	do {
		ret = mbedtls_ssl_close_notify(&ssl);
	} while (ret == MBEDTLS_ERR_SSL_WANT_WRITE);
	ret = 0;

	mbedtls_printf(" done\n");

	if (--ots) {
		goto reset;
	}

	/*
	 * Final clean-ups and exit
	 */
exit:

#ifdef MBEDTLS_ERROR_C
	if (ret != 0) {
		char error_buf[100];
		mbedtls_strerror(ret, error_buf, 100);
		mbedtls_printf("Last error was: %d - %s\n\n", ret, error_buf);
	}
#endif

	mbedtls_net_free(&client_fd);
	mbedtls_net_free(&listen_fd);

	mbedtls_x509_crt_free(&srvcert);
	mbedtls_pk_free(&pkey);
	mbedtls_ssl_free(&ssl);
	mbedtls_ssl_config_free(&conf);
	mbedtls_ssl_cookie_free(&cookie_ctx);
#if defined(MBEDTLS_SSL_CACHE_C)
	mbedtls_ssl_cache_free(&cache);
#endif
	mbedtls_ctr_drbg_free(&ctr_drbg);
	mbedtls_entropy_free(&entropy);

	return ret;
}

int dtls_server_main(int argc, char **argv)
{
	pthread_t tid;
	pthread_attr_t attr;
	struct sched_param sparam;
	int r;

	struct pthread_arg args;
	args.argc = argc;
	args.argv = argv;

	/* Initialize the attribute variable */
	if ((r = pthread_attr_init(&attr)) != 0) {
		mbedtls_printf("%s: pthread_attr_init failed, status=%d\n", __func__, r);
	}

	/* 1. set a priority */
	sparam.sched_priority = DTLS_SERVER_PRIORITY;
	if ((r = pthread_attr_setschedparam(&attr, &sparam)) != 0) {
		mbedtls_printf("%s: pthread_attr_setschedparam failed, status=%d\n", __func__, r);
	}

	if ((r = pthread_attr_setschedpolicy(&attr, DTLS_SERVER_SCHED_POLICY)) != 0) {
		mbedtls_printf("%s: pthread_attr_setschedpolicy failed, status=%d\n", __func__, r);
	}

	/* 2. set a stacksize */
	if ((r = pthread_attr_setstacksize(&attr, DTLS_SERVER_STACK_SIZE)) != 0) {
		mbedtls_printf("%s: pthread_attr_setstacksize failed, status=%d\n", __func__, r);
	}

	/* 3. create pthread with entry function */
	if ((r = pthread_create(&tid, &attr, (pthread_startroutine_t)dtls_server_cb, (void *)&args)) != 0) {
		mbedtls_printf("%s: pthread_create failed, status=%d\n", __func__, r);
	}

	/* Wait for the threads to stop */
	pthread_join(tid, NULL);

	return 0;
}
#endif
