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
/****************************************************************************
 * examples/websocket/websocket_main.c
 *
 *   Copyright (C) 2016 SAMSUNG ELECTRONICS CO., LTD. All rights reserved.
 *   Author: Jisuu Kim <jisuu.kim@samsung.com>
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

/**
* @testcase		websocket_ws_01
* @brief		To run websocket example. Test packet size and number can be modified as parameters.
*			parameters:
*			serverip: websocket server IP address
*			80: port number
*			NULL: URI address. Websocket doesn't have regulation for URI.
*			0: TLS disabled
*			128: test packet size
*			10: test number
* @scenario		1. Start websocket server at TASH using the command "websocket server 0"
*			2. Start websocket client at TASH using the command "websocket client [serverip] 80 \ 0 128 10".
* @apicovered
* @precondition		Connect to Wi-Fi. Both ARTIK051 server and ARTIK051 client should be in the same network.
* @postcondition
*/

/**
* @testcase		websocket_wss_01
* @brief		To run websocket example with TLS enabled. Test packet size and number can be modified as parameters.
*			parameters:
*			serverip: ARTIK051 websocket server IP address
*			443: port number. Web Secure Socket(WSS) uses 443 port.
*			NULL: URI address. Websocket doesn't have regulation for URI.
*			0: TLS disabled
*			128: test packet size
*			10: test number
* @scenario		1. Start websocket server at TASH using the command "websocket server 1"
*			2. Start websocket client at TASH using the command "websocket client [serverip] 443 \ 1 128 10".
* @apicovered
* @precondition		Connect to Wi-Fi. Both ARTIK051 server and ARTIK051 client should be in the same network.
* @postcondition
*/

/**
* @testcase		http_ws_01 (client)
* @brief		To run HTTP server and websocket client. Test packet size and number can be modified as parameters.
* @scenario		1. Start webserver at TASH using the command "webserver start".  Refer to webserver_main.c to run HTTP server.
*			2. Start websocket client at TASH using this command "websocket client [serverip] 80 \ 0 128 10".
* @apicovered
* @precondition		Connect to Wi-Fi. Both ARTIK051 server and ARTIK051 client should be in the same network.
* @postcondition
*/

/**
* @testcase		http_wss_01 (client)
* @brief		To run HTTPS server and secure websocket client. Test packet size and number can be modified as parameters.
* @scenario		1. Start webserver at TASH using the command "webserver start".  Refer to webserver_main.c to run HTTP server.
*			2. Start websocket client at TASH using this command "websocket client [serverip] 443 \ 1 128 10".
* @apicovered
* @precondition		Connect to Wi-Fi. Both ARTIK051 server and ARTIK051 client should be in the same network.
* @postcondition
*/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <apps/netutils/websocket.h>

#include <tls/config.h>
#include <tls/entropy.h>
#include <tls/ctr_drbg.h>
#include <tls/certs.h>
#include <tls/x509.h>
#include <tls/ssl.h>
#include <tls/net.h>
#include <tls/error.h>
#include <tls/debug.h>
#include <tls/ssl_cache.h>

#include <sys/socket.h>

#ifdef CONFIG_HW_RSA_SIGN
#include <tls/pk.h>
#include <tls/pk_internal.h>
#include <tls/see_api.h>
#include <tls/sss_key.h>
#endif

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* SSS configure */
#ifdef CONFIG_HW_RSA_SIGN
#define WEBSOCKET_S_CA_KEYINDEX   1
#define WEBSOCKET_S_DEV_KEYINDEX  2
#define WEBSOCKET_S_CA_CERTINDEX  1
#define WEBSOCKET_S_DEV_CERTINDEX 2
#define WEBSOCKET_C_CA_KEYINDEX   3
#define WEBSOCKET_C_DEV_KEYINDEX  4
#define WEBSOCKET_C_CA_CERTINDEX  3
#define WEBSOCKET_C_DEV_CERTINDEX 4
#endif

/* Stack size of examples */
#ifdef CONFIG_HW_RSA_SIGN
#define WEBSOCKET_EXAMPLE_STACKSIZE (1024 * 28)
#else
#define WEBSOCKET_EXAMPLE_STACKSIZE (1024 * 10)
#endif

/* TLS configure */
#define MBEDTLS_DEBUG_LEVEL 2

/****************************************************************************
 * Private Data
 ****************************************************************************/

int g_cnt;

/****************************************************************************
 * Public Functions
 ****************************************************************************/

#ifdef CONFIG_HW_RSA_SIGN
int set_key_and_cert_vector(void)
{
	int ret;
	/* Setup post key */
	/* THIS CODE SHOULD BE REMOVED AFTER USING SSS KEY AND CERT */
	if ((ret = see_setup_key(sss_da_rsa_ca, sizeof(sss_da_rsa_ca), SECURE_STORAGE_TYPE_KEY_RSA, WEBSOCKET_S_CA_KEYINDEX)) != 0) {
		printf("Error: set_key fail %d\n", ret);
		return -1;
	}
	if ((ret = see_setup_key(sss_da_rsa_dev, sizeof(sss_da_rsa_dev), SECURE_STORAGE_TYPE_KEY_RSA, WEBSOCKET_S_DEV_KEYINDEX)) != 0) {
		printf("Error: set_key fail %d\n", ret);
		return -1;
	}
	if ((ret = see_set_certificate(sss_ca_crt, sizeof(sss_ca_crt), WEBSOCKET_S_CA_CERTINDEX, CERT_PEM)) != 0) {
		printf("Error: set_cert fail %d\n", ret);
		return -1;
	}
	if ((ret = see_set_certificate(sss_dev_crt, sizeof(sss_dev_crt), WEBSOCKET_S_DEV_CERTINDEX, CERT_PEM)) != 0) {
		printf("Error: set_cert fail %d\n", ret);
		return -1;
	}
	if ((ret = see_setup_key(sss_da_rsa_ca, sizeof(sss_da_rsa_ca), SECURE_STORAGE_TYPE_KEY_RSA, WEBSOCKET_C_CA_KEYINDEX)) != 0) {
		printf("Error: set_key fail %d\n", ret);
		return -1;
	}
	if ((ret = see_setup_key(sss_da_rsa_dev, sizeof(sss_da_rsa_dev), SECURE_STORAGE_TYPE_KEY_RSA, WEBSOCKET_C_DEV_KEYINDEX)) != 0) {
		printf("Error: set_key fail %d\n", ret);
		return -1;
	}
	if ((ret = see_set_certificate(sss_ca_crt, sizeof(sss_ca_crt), WEBSOCKET_C_CA_CERTINDEX, CERT_PEM)) != 0) {
		printf("Error: set_cert fail %d\n", ret);
		return -1;
	}
	if ((ret = see_set_certificate(sss_dev_crt, sizeof(sss_dev_crt), WEBSOCKET_C_DEV_CERTINDEX, CERT_PEM)) != 0) {
		printf("Error: set_cert fail %d\n", ret);
		return -1;
	}
	return 0;
}
#endif

static void websocket_tls_debug(void *ctx, int level, const char *file, int line, const char *str)
{
	printf("%s:%04d: %s", file, line, str);
}

websocket_return_t websocket_tls_init(int param, websocket_t *data, mbedtls_ssl_config *conf, mbedtls_x509_crt *cert, mbedtls_pk_context *pkey, mbedtls_entropy_context *entropy, mbedtls_ctr_drbg_context *ctr_drbg, mbedtls_ssl_cache_context *cache)
{
	int r;
#ifdef CONFIG_HW_RSA_SIGN
	unsigned int ca_keyindex = WEBSOCKET_S_CA_KEYINDEX;
	unsigned int dev_keyindex = WEBSOCKET_S_DEV_KEYINDEX;
	unsigned int ca_certindex = WEBSOCKET_S_CA_CERTINDEX;
	unsigned int dev_certindex = WEBSOCKET_S_DEV_CERTINDEX;

	if (param) {
		ca_keyindex = WEBSOCKET_C_CA_KEYINDEX;
		dev_keyindex = WEBSOCKET_C_DEV_KEYINDEX;
		ca_certindex = WEBSOCKET_C_CA_CERTINDEX;
		dev_certindex = WEBSOCKET_C_DEV_CERTINDEX;
	}
#else
	const char *crt = mbedtls_test_srv_crt;
	const char *key = mbedtls_test_srv_key;
	const char *ca_crt = mbedtls_test_cas_pem;
	size_t crt_len = mbedtls_test_srv_crt_len;
	size_t cacrt_len = mbedtls_test_cas_pem_len;
	size_t key_len = mbedtls_test_srv_key_len;

	if (param) {
		crt = mbedtls_test_cli_crt;
		key = mbedtls_test_cli_key;
		ca_crt = mbedtls_test_cas_pem;
		crt_len = mbedtls_test_cli_crt_len;
		cacrt_len = mbedtls_test_cas_pem_len;
		key_len = mbedtls_test_cli_key_len;
	}
#endif

	/* initialize tls context for server */
	mbedtls_ssl_config_init(conf);
	mbedtls_x509_crt_init(cert);
	mbedtls_pk_init(pkey);
	mbedtls_entropy_init(entropy);
	mbedtls_ctr_drbg_init(ctr_drbg);

#ifdef CONFIG_HW_RSA_SIGN
	see_init();

	if (set_key_and_cert_vector()) {
		printf("Error: set key and cert fail\n");
		return WEBSOCKET_INIT_ERROR;
	}

	/* 1. Load the certificates and private key */
	printf("  . [SSS] Loading the cert. and key...");

	unsigned char *cert_buf;
	unsigned int cert_len = 1500;

	cert_buf = malloc(cert_len);
	if (cert_buf == NULL) {
		printf("Error: cert_buf malloc fail\n");
		return WEBSOCKET_INIT_ERROR;
	}

	if ((r = see_get_certificate(cert_buf, &cert_len, dev_certindex, CERT_PEM)) != 0) {
		free(cert_buf);
		printf("Error: see_get_cert returned %d\n", r);
		return WEBSOCKET_INIT_ERROR;
	}

	if ((r = mbedtls_x509_crt_parse(cert, cert_buf, cert_len)) != 0) {
		free(cert_buf);
		printf("Error: cert_parse returned %d\n", r);
		return WEBSOCKET_INIT_ERROR;
	}

	((mbedtls_rsa_context *)(cert->pk.pk_ctx))->key_index = ca_keyindex;

	cert_len = 1500;

	if ((r = see_get_certificate(cert_buf, &cert_len, ca_certindex, CERT_PEM)) != 0) {
		free(cert_buf);
		printf("Error: see_get_cert returned %d\n", r);
		return WEBSOCKET_INIT_ERROR;
	}

	if ((r = mbedtls_x509_crt_parse(cert, cert_buf, cert_len)) != 0) {
		free(cert_buf);
		printf("Error: cert_parse returned %d\n", r);
		return WEBSOCKET_INIT_ERROR;
	}

	((mbedtls_rsa_context *)(cert->next->pk.pk_ctx))->key_index = ca_keyindex;

	free(cert_buf);

	unsigned char rsa_public[292] = { 0x30, 0x82, 0x01, 0x20, 0x30, 0x0d, 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x01,
									  0x01, 0x05, 0x00, 0x03, 0x82, 0x01, 0x0d, 0x00, 0x30, 0x82, 0x01, 0x08, 0x02, 0x82, 0x01, 0x01,
									  0x00, 0xa2, 0x64, 0x21, 0xcf, 0x1c, 0xdb, 0x49, 0x6c, 0x44, 0x01, 0xf8, 0xd5, 0x8b, 0x8d, 0x20,
									  0xfe, 0x2a, 0x46, 0x4d, 0x29, 0xf4, 0x82, 0x3c, 0xa4, 0x29, 0x7d, 0x6b, 0xdc, 0xc4, 0x04, 0xd6,
									  0x0f, 0xf3, 0x6b, 0xa8, 0xb1, 0xad, 0x2b, 0xa1, 0xa5, 0xad, 0xfb, 0x9a, 0xba, 0x72, 0x6e, 0x4e,
									  0x71, 0x93, 0x54, 0x8d, 0x90, 0x02, 0x34, 0x80, 0x1d, 0x8c, 0x83, 0xc9, 0x84, 0xa3, 0xcf, 0x9f,
									  0x80, 0xe9, 0x4f, 0x5b, 0xf6, 0x29, 0x17, 0xf6, 0x7f, 0x5a, 0x79, 0x47, 0x0c, 0x2c, 0xcf, 0x98,
									  0x88, 0x6a, 0x31, 0x4e, 0x0a, 0x2c, 0x8e, 0x8c, 0xe5, 0xa5, 0x9f, 0xd7, 0x8f, 0xd0, 0xc1, 0x04,
									  0x1a, 0xe9, 0x54, 0xa1, 0x36, 0x4e, 0x92, 0x5e, 0x41, 0x9c, 0x07, 0xc8, 0x48, 0xac, 0x9c, 0x7c,
									  0xcb, 0xa0, 0x8a, 0x51, 0x52, 0x4f, 0x47, 0xa2, 0xc8, 0x48, 0xbc, 0xcd, 0x55, 0x85, 0x24, 0xff,
									  0xfa, 0x58, 0xe6, 0x75, 0x61, 0x14, 0x1a, 0x82, 0x4e, 0x6b, 0x40, 0x63, 0x9e, 0xef, 0xbd, 0x70,
									  0x88, 0x9e, 0xc8, 0x59, 0x89, 0x16, 0x0c, 0x4e, 0x71, 0xec, 0x2d, 0xa4, 0x0b, 0xb3, 0x20, 0xca,
									  0x04, 0x5b, 0x37, 0xf6, 0x5c, 0x80, 0x8d, 0x6a, 0xe4, 0x26, 0x95, 0xe4, 0xd5, 0x35, 0xcd, 0xd3,
									  0x90, 0x67, 0x48, 0xef, 0x14, 0x8e, 0xc6, 0xcc, 0x16, 0xdb, 0x7a, 0x96, 0xd6, 0xbf, 0x01, 0xef,
									  0x5f, 0x8d, 0xee, 0x35, 0xd1, 0x66, 0xa3, 0x26, 0x96, 0x5e, 0x73, 0x3b, 0x1e, 0xf6, 0x72, 0xc9,
									  0x78, 0xc8, 0xdd, 0x81, 0x21, 0x0f, 0x0d, 0xdc, 0x3f, 0x63, 0x7a, 0x92, 0xf1, 0x31, 0x53, 0xe6,
									  0x34, 0xd7, 0x70, 0xb0, 0x1d, 0x2f, 0x97, 0xab, 0x44, 0xf1, 0x70, 0x58, 0x0e, 0xca, 0xab, 0x26,
									  0x23, 0x39, 0x6e, 0xdb, 0xf5, 0x5a, 0x15, 0x4a, 0x09, 0x00, 0x7c, 0xe5, 0x82, 0x78, 0xb8, 0xf0,
									  0xd1, 0x02, 0x01, 0x03
									};

	if ((r = mbedtls_pk_parse_public_key(pkey, rsa_public, 292)) != 0) {
		printf("Error: pk_parse_public returned %d\n", r);
		return WEBSOCKET_INIT_ERROR;
	}

	if (pkey->pk_info->type == MBEDTLS_PK_RSA) {
		((mbedtls_rsa_context *)(pkey->pk_ctx))->key_index = dev_keyindex;
	}
#else
	/* 1. Load the certificates and private RSA key */
	printf("  . Loading the cert. and key...");

	/* S/W Certificiate */
	if ((r = mbedtls_x509_crt_parse(cert, (const unsigned char *)crt, crt_len)) != 0) {
		printf("Error: mbedtls_x509_crt_parse when read crt returned %d\n", r);
		return WEBSOCKET_INIT_ERROR;
	}

	if ((r = mbedtls_x509_crt_parse(cert, (const unsigned char *)ca_crt, cacrt_len)) != 0) {
		printf("Error: mbedtls_x509_crt_parse when read caspem returned %d\n", r);
		return WEBSOCKET_INIT_ERROR;
	}

	if ((r = mbedtls_pk_parse_key(pkey, (const unsigned char *)key, key_len, NULL, 0)) != 0) {
		printf("Error: mbedtls_pk_parse_key returned %d\n", r);
		return WEBSOCKET_INIT_ERROR;
	}

	printf("Ok\n");
#endif

	/* 2. Seed the RNG */
	printf("  . Seeding the random number generator...");

	if ((r = mbedtls_ctr_drbg_seed(ctr_drbg, mbedtls_entropy_func, entropy, NULL, 0)) != 0) {
		printf("Error: mbedtls_ctr_drbg_seed returned %d\n", r);
		return WEBSOCKET_INIT_ERROR;
	}

	printf("Ok\n");

	/* 3. Setup ssl stuff */
	printf("  . Setting up the SSL data...");

	if ((r = mbedtls_ssl_config_defaults(conf, param ? MBEDTLS_SSL_IS_CLIENT : MBEDTLS_SSL_IS_SERVER, MBEDTLS_SSL_TRANSPORT_STREAM, MBEDTLS_SSL_PRESET_DEFAULT)) != 0) {
		printf("Error: mbedtls_ssl_config_defaults returned %d\n", r);
		return WEBSOCKET_INIT_ERROR;
	}

	mbedtls_ssl_conf_rng(conf, mbedtls_ctr_drbg_random, ctr_drbg);
	mbedtls_ssl_conf_dbg(conf, websocket_tls_debug, stdout);
	if (!param) {
		mbedtls_ssl_cache_init(cache);
		mbedtls_ssl_conf_session_cache(conf, cache, mbedtls_ssl_cache_get, mbedtls_ssl_cache_set);
	}

	mbedtls_ssl_conf_ca_chain(conf, cert->next, NULL);
	if ((r = mbedtls_ssl_conf_own_cert(conf, cert, pkey)) != 0) {
		printf("Error: mbedtls_ssl_conf_own_cert returned %d\n", r);
		return WEBSOCKET_INIT_ERROR;
	}
	data->tls_conf = conf;

	printf("Ok\n");
	return WEBSOCKET_SUCCESS;
}

void websocket_tls_release(int param, mbedtls_ssl_config *conf, mbedtls_x509_crt *cert, mbedtls_pk_context *pkey, mbedtls_entropy_context *entropy, mbedtls_ctr_drbg_context *ctr_drbg, mbedtls_ssl_cache_context *cache)
{
	if (!param) {
		mbedtls_ssl_cache_free(cache);
	}
	mbedtls_ctr_drbg_free(ctr_drbg);
	mbedtls_entropy_free(entropy);
	mbedtls_pk_free(pkey);
	mbedtls_x509_crt_free(cert);
	mbedtls_ssl_config_free(conf);

#ifdef CONFIG_HW_RSA_SIGN
	see_free();
#endif
}

/****************************************************************************
 * websocket_main
 ****************************************************************************/

/* receive packets from TCP socket */
ssize_t recv_cb(websocket_context_ptr ctx, uint8_t *buf, size_t len, int flags, void *user_data)
{
	ssize_t r;
	int fd;
	int retry_cnt = 3;
	struct websocket_info_t *info = user_data;

	fd = info->data->fd;
RECV_RETRY:
	if (info->data->tls_enabled) {
		r = mbedtls_ssl_read(info->data->tls_ssl, buf, len);
	} else {
		r = recv(fd, buf, len, 0);
		if (r == 0) {
			websocket_set_error(info->data, WEBSOCKET_ERR_CALLBACK_FAILURE);
		} else if (r < 0) {
			printf("recv err : %d\n", errno);
			if (errno == EAGAIN || errno == EBUSY) {
				if (retry_cnt == 0) {
					return r;
				}
				retry_cnt--;
				goto RECV_RETRY;
			}
		}
	}

	return r;
}

/* send packets from TCP socket */
ssize_t send_cb(websocket_context_ptr ctx, const uint8_t *buf, size_t len, int flags, void *user_data)
{
	ssize_t r;
	int fd;
	int retry_cnt = 3;
	struct websocket_info_t *info = user_data;

	fd = info->data->fd;
SEND_RETRY:
	if (info->data->tls_enabled) {
		r = mbedtls_ssl_write(info->data->tls_ssl, buf, len);
	} else {
		r = send(fd, buf, len, flags);
		if (r < 0) {
			printf("send err : %d\n", errno);
			if (errno == EAGAIN || errno == EBUSY) {
				if (retry_cnt == 0) {
					return r;
				}
				retry_cnt--;
				goto SEND_RETRY;
			}
		}
	}

	return r;
}

int genmask_cb(websocket_context_ptr ctx, uint8_t *buf, size_t len, void *user_data)
{
	memset(buf, rand(), len);
	return 0;
}

void print_on_msg_cb(websocket_context_ptr ctx, const websocket_on_msg_arg *arg, void *user_data)
{
	websocket_frame_t msgarg = {
		arg->opcode, arg->msg, arg->msg_length
	};

	/* Echo back non-closing message */
	if (WEBSOCKET_CHECK_NOT_CTRL_FRAME(arg->opcode)) {
		g_cnt++;				//to check communication is done.
		printf("on_msg length : %d print : %s, [#%d]\n", msgarg.msg_length, msgarg.msg, g_cnt);
	} else if (WEBSOCKET_CHECK_CTRL_CLOSE(arg->opcode)) {
		printf("print_on_msg recevied close message\n");
	} else if (WEBSOCKET_CHECK_CTRL_PING(arg->opcode)) {
		printf("cli got ping\n");
	} else if (WEBSOCKET_CHECK_CTRL_PONG(arg->opcode)) {
		printf("cli got pong\n");
	}
}

void echoback_on_msg_cb(websocket_context_ptr ctx, const websocket_on_msg_arg *arg, void *user_data)
{
	struct websocket_info_t *info = user_data;
	websocket_frame_t msgarg = {
		arg->opcode, arg->msg, arg->msg_length
	};

	/* Echo back non-closing message */
	if (WEBSOCKET_CHECK_NOT_CTRL_FRAME(arg->opcode)) {
		websocket_queue_msg(info->data, &msgarg);
		//echo back the message
	} else if (WEBSOCKET_CHECK_CTRL_CLOSE(arg->opcode)) {
		printf("echoback_on_msg recevied close message\n");
	} else if (WEBSOCKET_CHECK_CTRL_PING(arg->opcode)) {
		printf("srv got ping\n");
	} else if (WEBSOCKET_CHECK_CTRL_PONG(arg->opcode)) {
		printf("srv got pong\n");
	}
}

/*
 *  recv_start_cb for example
 *  this cb will be invoked when a new frame starts to be received.
 */
void recv_start_cb(websocket_context_ptr ctx, const websocket_recv_start_arg *arg, void *user_data)
{
	if (arg->fin == 1) {
		//this frame is the last.
	} else if (arg->fin == 0) {
		//this frame is fragmented.
	}
	if (WEBSOCKET_CHECK_NOT_CTRL_FRAME(arg->opcode)) {
		//it's not a control frame. You can find opcodes from apps/include/netutils/websocket.h
	}
}

/*
 *  recv_chunk_cb for example
 *  this cb will be invoked when a chunk of frame payload is received.
 */
void recv_chunk_cb(websocket_context_ptr ctx, const websocket_recv_chunk_arg *arg, void *user_data)
{
	if (arg->data_length > 0) {
		//you can handle the payload (arg->data)
	}
}

/*
 *  recv_end_cb for example
 *  this cb will be invoked when a frame is completely received.
 */
void recv_end_cb(websocket_context_ptr ctx, void *user_data)
{
	printf("receive done\n");
	//receive done.
}

/* websocket client prints received messages from a server using recv message cb */
int websocket_client(void *arg)
{
	int i;
	int r = WEBSOCKET_SUCCESS;
	int tmp;
	char **argv = arg;
	char *addr = NULL;
	char *port = NULL;
	char *path = NULL;
	int tls = atoi(argv[3]);
	int size = atoi(argv[4]);
	int num = atoi(argv[5]);
	websocket_frame_t *tx_frame = NULL;
	websocket_t *websocket_cli = NULL;
	char *test_message = NULL;
	static websocket_cb_t cb = {
		recv_cb,				/* recv callback */
		send_cb,				/* send callback */
		genmask_cb,				/* gen mask callback */
		NULL,					/* recv frame start callback */
		NULL,					/* recv frame chunk callback */
		NULL,					/* recv frame end callback */
		print_on_msg_cb			/* recv message callback */
	};

	mbedtls_ssl_config conf;
	mbedtls_x509_crt cert;
	mbedtls_pk_context pkey;
	mbedtls_entropy_context entropy;
	mbedtls_ctr_drbg_context ctr_drbg;
	mbedtls_ssl_cache_context cache;

	addr = malloc(strlen(argv[0]) + 1);
	if (addr == NULL) {
		printf("fail to allocate memory\n");
		goto WEB_CLI_EXIT;
	}
	port = malloc(strlen(argv[1]) + 1);
	if (port == NULL) {
		printf("fail to allocate memory\n");
		goto WEB_CLI_EXIT;
	}
	path = malloc(strlen(argv[2]) + 1);
	if (path == NULL) {
		printf("fail to allocate memory\n");
		goto WEB_CLI_EXIT;
	}

	memset(addr, 0, strlen(argv[0]) + 1);
	memset(port, 0, strlen(argv[1]) + 1);
	memset(path, 0, strlen(argv[2]) + 1);

	strncpy(addr, argv[0], strlen(argv[0]));
	strncpy(port, argv[1], strlen(argv[1]));
	strncpy(path, argv[2], strlen(argv[2]));

	memset(&conf, 0, sizeof(mbedtls_ssl_config));
	memset(&cert, 0, sizeof(mbedtls_x509_crt));
	memset(&pkey, 0, sizeof(mbedtls_pk_context));
	memset(&entropy, 0, sizeof(mbedtls_entropy_context));
	memset(&ctr_drbg, 0, sizeof(mbedtls_ctr_drbg_context));
	memset(&cache, 0, sizeof(mbedtls_ssl_cache_context));

	g_cnt = 0;
	websocket_cli = malloc(sizeof(websocket_t));
	if (websocket_cli == NULL) {
		printf("fail to allocate memory\n");
		goto WEB_CLI_EXIT;
	}
	memset(websocket_cli, 0, sizeof(websocket_t));

	websocket_cli->cb = &cb;
	websocket_cli->tls_enabled = tls;

	/* TLS init routine */
	if (tls) {
#ifdef MBEDTLS_DEBUG_C
		mbedtls_debug_set_threshold(MBEDTLS_DEBUG_LEVEL);
#endif
		if ((r = websocket_tls_init(1, websocket_cli, &conf, &cert, &pkey, &entropy, &ctr_drbg, &cache)) != WEBSOCKET_SUCCESS) {
			printf("fail to init TLS, error: %d\n", r);
			goto WEB_CLI_EXIT;
		}
		websocket_cli->tls_ssl = malloc(sizeof(mbedtls_ssl_context));
		if (websocket_cli->tls_ssl == NULL) {
			printf("fail to allocate memory\n");
			goto WEB_CLI_EXIT;
		}

		mbedtls_net_init(&(websocket_cli->tls_net));
		mbedtls_ssl_init(websocket_cli->tls_ssl);
	}

	printf("start opening websocket client to addr: %s, port: %s, Security: %s\n", addr, port, tls ? "enabled" : "disabled");

	r = websocket_client_open(websocket_cli, addr, port, path);
	if (r != WEBSOCKET_SUCCESS) {
		printf("websocket_client_open returned error: %d\n", r);
		goto WEB_CLI_EXIT;
	}

	websocket_register_cb(websocket_cli, &cb);

	i = 0;
	tmp = size;
	while (tmp) {
		tmp = tmp / 10;
		i++;
	}
	test_message = malloc(size + 1);
	if (test_message == NULL) {
		goto WEB_CLI_EXIT;
	}
	memset(test_message, '.', size);
	snprintf(&test_message[size - i - 1], i + 1, "%d", size);
	strncpy(test_message, "hello websocket", 15);
	test_message[size - 1] = '\0';

	tx_frame = malloc(sizeof(websocket_frame_t));
	if (tx_frame == NULL) {
		printf("fail to allocate memory\n");
		goto WEB_CLI_EXIT;
	}
	memset(tx_frame, 0, sizeof(websocket_frame_t));

	tx_frame->opcode = WEBSOCKET_TEXT_FRAME;
	tx_frame->msg = (const uint8_t *)test_message;
	tx_frame->msg_length = strlen((const char *)tx_frame->msg) + 1;

	printf("websocket ping message test\n");
	sleep(60);					//if there's no I/O for 20 sec, websocket starts heart-beat ping-pong.

	/* send and receive small message num times */
	for (i = 0; i < num; i++) {
		tmp = g_cnt + 1;

		r = websocket_queue_msg(websocket_cli, tx_frame);
		if (r != WEBSOCKET_SUCCESS) {
			printf("websocket_queue_msg returned error : %d\n", r);
		}

		/* wait for server echoes back */
		while (g_cnt != tmp) {
			usleep(100000);
		}
	}

	/* wait until every message is tested. */
	while (g_cnt < num) {
		if (websocket_cli->state == WEBSOCKET_STOP) {
			printf("test is not finished and websocket client is expired\n");
			break;
		}
		usleep(100000);
	};
WEB_CLI_EXIT:
	r = websocket_queue_close(websocket_cli, NULL);
	if (r != WEBSOCKET_SUCCESS) {
		printf("websocket_close returned error : %d\n", r);
	}
	printf("websocket client %s [%d]\n", (r ? "error" : "finished"), r);

	if (tx_frame) {
		free(tx_frame);
	}

	if (tls) {
		websocket_tls_release(1, &conf, &cert, &pkey, &entropy, &ctr_drbg, &cache);
		if (websocket_cli->tls_ssl) {
			mbedtls_ssl_free(websocket_cli->tls_ssl);
			free(websocket_cli->tls_ssl);
		}
	}

	if (websocket_cli) {
		free(websocket_cli);
	}
	if (addr) {
		free(addr);
	}
	if (port) {
		free(port);
	}
	if (path) {
		free(path);
	}
	if (test_message) {
		free(test_message);
	}

	return r;
}

/* websocket server echoes back messages from a client using recv message cb */
int websocket_server(void *arg)
{
	int r;
	int tls = atoi((const char *)arg);
	static websocket_cb_t cb = {
		recv_cb,				/* recv callback */
		send_cb,				/* send callback */
		NULL,					/* gen mask callback */
		NULL,					/* recv frame start callback */
		NULL,					/* recv frame chunk callback */
		NULL,					/* recv frame end callback */
		echoback_on_msg_cb		/* recv message callback */
	};

	mbedtls_ssl_config conf;
	mbedtls_x509_crt cert;
	mbedtls_pk_context pkey;
	mbedtls_entropy_context entropy;
	mbedtls_ctr_drbg_context ctr_drbg;
	mbedtls_ssl_cache_context cache;

	g_cnt = 0;
	websocket_t *websocket_srv = malloc(sizeof(websocket_t));
	if (websocket_srv == NULL) {
		printf("fail to allocate memory\n");
		return WEBSOCKET_ALLOCATION_ERROR;
	}

	memset(websocket_srv, 0, sizeof(websocket_t));

	websocket_srv->cb = &cb;
	websocket_srv->tls_enabled = tls;

	/* TLS init routine */
	if (tls) {
#ifdef MBEDTLS_DEBUG_C
		mbedtls_debug_set_threshold(MBEDTLS_DEBUG_LEVEL);
#endif
		if ((r = websocket_tls_init(0, websocket_srv, &conf, &cert, &pkey, &entropy, &ctr_drbg, &cache)) != WEBSOCKET_SUCCESS) {
			printf("fail to init TLS\n");
			goto WEB_SRV_EXIT;
		}
	}

	/* Start Websocket server program */
	if ((r = websocket_server_open(websocket_srv)) != WEBSOCKET_SUCCESS) {
		printf("websocket_server_open returned error : %d\n", r);
		goto WEB_SRV_EXIT;
	}

WEB_SRV_EXIT:
	if (tls) {
		websocket_tls_release(0, &conf, &cert, &pkey, &entropy, &ctr_drbg, &cache);
	}

	if (websocket_srv) {
		free(websocket_srv);
	}

	return 0;
}

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int websocket_main(int argc, char *argv[])
#endif
{
	int status;
	pthread_attr_t attr;
	pthread_t tid;

	if (argc == 3 || argc == 8) {
		if (memcmp(argv[1], "client", strlen("client")) == 0) {
			if (argc != 8) {
				printf("not enough arguments\n  usage: %s %s [addr] [port] [path] [tls_enable] [size] [num]\n", argv[0], argv[1]);
				return -1;
			} else {
				if (atoi(argv[6]) < 16) {
					printf("size too small, minimum 16\n");
					return -1;
				}
				if (atoi(argv[7]) < 1) {
					printf("test number too small, minimum 1\n");
					return -1;
				}
				status = pthread_attr_init(&attr);
				if (status != 0) {
					printf("fail to init thread\n");
				}
				pthread_attr_setstacksize(&attr, WEBSOCKET_EXAMPLE_STACKSIZE);
				pthread_attr_setschedpolicy(&attr, WEBSOCKET_SCHED_POLICY);
				status = pthread_create(&tid, &attr, (pthread_startroutine_t)websocket_client, (void *)(argv + 2));
				if (status != 0) {
					printf("fail to create thread\n");
					return -1;
				}
				pthread_setname_np(tid, "websocket client");
				sleep(1);
				pthread_detach(tid);
			}
		} else if (memcmp(argv[1], "server", strlen("server")) == 0) {
			if (argc != 3) {
				printf("not enough arguments\n	usage: %s %s [tls_enable]\n", argv[0], argv[1]);
				return -1;
			} else {
				status = pthread_attr_init(&attr);
				if (status != 0) {
					printf("fail to init thread\n");
				}
				pthread_attr_setstacksize(&attr, WEBSOCKET_EXAMPLE_STACKSIZE);
				pthread_attr_setschedpolicy(&attr, WEBSOCKET_SCHED_POLICY);
				status = pthread_create(&tid, &attr, (pthread_startroutine_t)websocket_server, (void *)argv[2]);
				if (status != 0) {
					printf("fail to create thread\n");
					return -1;
				}
				pthread_setname_np(tid, "websocket server");
				sleep(1);
				pthread_detach(tid);
			}
		} else {
			printf("choose client or server\n");
			return -1;
		}
	} else {
		printf("not enough arguments\n");
		return -1;
	}
	return 0;
}
