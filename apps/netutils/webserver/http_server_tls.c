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

#include <apps/netutils/webserver/http_err.h>
#include <apps/netutils/webserver/http_server.h>

#include "http_client.h"
#include "http_arch.h"
#include "http_log.h"

#if defined(CONFIG_HW_RSA_SIGN)
#include "tls/see_api.h"
#include "tls/pk.h"
#include "tls/pk_internal.h"
#endif

const char *pers = "http_tls_server";

#define MBED_DEBUG_LEVEL 3

static void http_tls_debug(void *ctx, int level, const char *file, int line, const char *str)
{
	HTTP_LOGD("%s:%04d: %s", file, line, str);
}

int http_tls_init(struct http_server_t *server, struct ssl_config_t *ssl_config)
{
	int result = 0;

	server->tls_init = 1;
	mbedtls_ssl_config_init(&(server->tls_conf));
	mbedtls_x509_crt_init(&(server->tls_srvcert));
	mbedtls_pk_init(&(server->tls_pkey));
	mbedtls_entropy_init(&(server->tls_entropy));
	mbedtls_ctr_drbg_init(&(server->tls_ctr_drbg));
	mbedtls_net_init(&(server->tls_ctx));
	mbedtls_ssl_cache_init(&(server->tls_cache));

#ifdef MBEDTLS_DEBUG_C
	mbedtls_debug_set_threshold(MBED_DEBUG_LEVEL);
#endif

	/* 0. Initialize the RNG */

	HTTP_LOGD("  . Seeding the random number generator...");

	if ((result = mbedtls_ctr_drbg_seed(&(server->tls_ctr_drbg), mbedtls_entropy_func, &(server->tls_entropy), (const unsigned char *)pers, strlen(pers))) != 0) {
		HTTP_LOGE("Error: mbedtls_ctr_drbg_seed returned %d\n", result);
		return HTTP_ERROR;
	}

	HTTP_LOGD("Ok\n");

#ifdef CONFIG_HW_RSA_SIGN
	/* 1. Load the certificates and private key */
	HTTP_LOGD("  . [SSS] Loading the cert. and key...");

	if (ssl_config == NULL) {
		result = -1;
		HTTP_LOGE("Error: ssl_config is Null %d\n", result);
		return HTTP_ERROR;
	}

	unsigned char *cert_buf;
//  unsigned char *pubkey_buf;
//  unsigned int pubkey_len = 292;
	unsigned int cert_len = 1500;

	cert_buf = malloc(cert_len);
	if (cert_buf == NULL) {
		return HTTP_ERROR;
	}

	result = see_get_certificate(cert_buf, &cert_len, ssl_config->dev_cert_index, CERT_PEM);
	if (result) {
		HTTP_LOGE("Error: dev_cert get fail, return %d\n", result);
		goto http_exit;
	}

	result = mbedtls_x509_crt_parse(&(server->tls_srvcert), (const unsigned char *)cert_buf, cert_len);

	if (result) {
		HTTP_LOGE("Error: dev_cert parse fail, return %d\n", result);
		goto http_exit;
	}

	cert_len = 1500;

	result = see_get_certificate(cert_buf, &cert_len, ssl_config->ca_cert_index, CERT_PEM);
	if (result) {
		HTTP_LOGE("Error: CA_cert get fail, return %d\n", result);
		goto http_exit;
	}

	result = mbedtls_x509_crt_parse(&(server->tls_srvcert), (const unsigned char *)cert_buf, cert_len);

	if (result) {
		HTTP_LOGE("Error: CA_cert parse fail, return %d\n", result);
		goto http_exit;
	}
#if 1
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

	if ((result = mbedtls_pk_parse_public_key(&server->tls_pkey, rsa_public, 292)) != 0) {
		HTTP_LOGE("Error: parse pubkey fail, return %d\n", result);
		goto http_exit;
	}
#else
	pubkey_buf = malloc(pubkey_len);
	if (pubkey_buf == NULL) {
		result = -1;
		goto http_exit;
	}

	if ((result = see_get_publickey(pubkey_buf, &pubkey_len)) != 0) {
		HTTP_LOGE("Error: get pubkey fail, return %d\n", result);
		goto http_exit;
	}
	if ((result = mbedtls_pk_parse_public_key(&server->tls_pkey, pubkey_buf, pubkey_len)) != 0) {
		HTTP_LOGE("Error: parse pubkey fail, return %d\n", result);
		goto http_exit;
	}
#endif
	if (server->tls_pkey.pk_info->type == MBEDTLS_PK_RSA) {
		((mbedtls_rsa_context *)(server->tls_pkey.pk_ctx))->key_index = ssl_config->dev_key_index;
	}
	HTTP_LOGD("Ok\n");

 http_exit:
	if (cert_buf) {
		free(cert_buf);
	}
	if (result) {
		return result;
	}
#else

	/* 1. Load the certificates */

	HTTP_LOGD("  . Loading the server cert. and key...");

	result = mbedtls_x509_crt_parse(&(server->tls_srvcert), (const unsigned char *)ssl_config->dev_cert, ssl_config->dev_cert_len);
	if (result) {
		HTTP_LOGE("Error: srv_cert parse fail, return %d\n", result);
		return HTTP_ERROR;
	}

	/* 2. Load the private key */
	result = mbedtls_pk_parse_key(&(server->tls_pkey), (const unsigned char *)ssl_config->private_key, ssl_config->private_key_len, NULL, 0);

	if (result) {
		HTTP_LOGE("Error: srv_key parse fail, return %d\n", result);
		return HTTP_ERROR;
	}

	HTTP_LOGD("Ok\n");

	/* 3. Load the root CA certificate */

	HTTP_LOGD("  . Loading the CA cert...");

	result = mbedtls_x509_crt_parse(&(server->tls_srvcert), (const unsigned char *)ssl_config->root_ca, ssl_config->root_ca_len);
	if (result) {
		HTTP_LOGE("Error: CA_cert parse fail, return %d\n", result);
		return HTTP_ERROR;
	}

	HTTP_LOGD("Ok\n");

#endif							/* CONFIG_HW_RSA_SIGN */

	/* 4. Setup ssl stuff */

	HTTP_LOGD("  . Setting up the SSL data...");

	if ((result = mbedtls_ssl_config_defaults(&(server->tls_conf), MBEDTLS_SSL_IS_SERVER, MBEDTLS_SSL_TRANSPORT_STREAM, MBEDTLS_SSL_PRESET_DEFAULT))
		!= 0) {
		HTTP_LOGE("Error: mbedtls_ssl_config_defaults returned %d\n", result);
		return HTTP_ERROR;
	}

	mbedtls_ssl_conf_rng(&(server->tls_conf), mbedtls_ctr_drbg_random, &(server->tls_ctr_drbg));
	mbedtls_ssl_conf_dbg(&(server->tls_conf), http_tls_debug, stdout);
	mbedtls_ssl_conf_session_cache(&(server->tls_conf), &(server->tls_cache), mbedtls_ssl_cache_get, mbedtls_ssl_cache_set);
	mbedtls_ssl_conf_ca_chain(&(server->tls_conf), server->tls_srvcert.next, NULL);

	if ((result = mbedtls_ssl_conf_own_cert(&(server->tls_conf), &(server->tls_srvcert), &(server->tls_pkey))) != 0) {
		HTTP_LOGE("Error: mbedtls_ssl_conf_own_cert returned %d\n", result);
		return HTTP_ERROR;
	}
	HTTP_LOGD("Ok\n");

	return HTTP_OK;
}

int http_server_tls_release(struct http_server_t *server)
{
	HTTP_LOGD("Server TLS Release...\n");

	mbedtls_ssl_cache_free(&(server->tls_cache));
	mbedtls_x509_crt_free(&(server->tls_srvcert));
	mbedtls_pk_free(&(server->tls_pkey));
	mbedtls_ssl_config_free(&(server->tls_conf));
	mbedtls_ctr_drbg_free(&(server->tls_ctr_drbg));
	mbedtls_entropy_free(&(server->tls_entropy));

	HTTP_LOGD("Ok\n");

	return HTTP_OK;
}
