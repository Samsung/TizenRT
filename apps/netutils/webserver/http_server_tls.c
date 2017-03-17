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

#if defined(CONFIG_HW_RSA)
#include "tls/see_api.h"
#include "tls/sss_key.h"
#include "tls/pk.h"
#include "tls/pk_internal.h"
#endif

const char *pers = "http_tls_server";

#define MBED_DEBUG_LEVEL 0

static void http_tls_debug(void *ctx, int level, const char *file, int line, const char *str)
{
	HTTP_LOGD("%s:%04d: %s", file, line, str);
}

int http_tls_init(struct http_server_t *server, struct ssl_config_t *ssl_config)
{
	int result = 0;

	/*
	 * 1. Check and Initialize input parameter
	 */
	if (!ssl_config) {
		return HTTP_ERROR;
	}

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

	/*
	 * 2. Initialize the Random number generator
	 */
	HTTP_LOGD("  . Seeding the random number generator...");

	if ((result = mbedtls_ctr_drbg_seed(&(server->tls_ctr_drbg), mbedtls_entropy_func, &(server->tls_entropy), (const unsigned char *) pers, strlen(pers))) != 0) {
		HTTP_LOGE("Error: mbedtls_ctr_drbg_seed returned %d\n", result);
		return HTTP_ERROR;
	}

	HTTP_LOGD("Ok\n");

	/*
	 * 2. Load the certificates and private key
	 */
#ifdef CONFIG_HW_RSA
	HTTP_LOGD("  . [SSS] Loading the cert. and key...");

	/* 2-1 Load device cert */
	if ((result = mbedtls_x509_crt_parse(&(server->tls_srvcert), (const unsigned char *) sss_dev_crt, sizeof(sss_dev_crt))) != 0) {
		HTTP_LOGE("Error: dev_cert parse fail, return %d\n", result);
		goto http_exit;
	}

	/* 2-2 Load CA cert */
	if ((result = mbedtls_x509_crt_parse(&(server->tls_srvcert), (const unsigned char *) sss_ca_crt, sizeof(sss_ca_crt))) != 0) {
		HTTP_LOGE("Error: CA_cert parse fail, return %d\n", result);
		goto http_exit;
	}

	/* 2-3 Setup device key */
	const mbedtls_pk_info_t *pk_info;

	if ((pk_info = mbedtls_pk_info_from_type(MBEDTLS_PK_RSA)) == NULL) {
		return WEBSOCKET_INIT_ERROR;
	}

	if ((result = mbedtls_pk_setup(&server->tls_pkey, pk_info)) != 0) {
		return WEBSOCKET_INIT_ERROR;
	}

	mbedtls_rsa_init((mbedtls_rsa_context *) server->tls_pkey.pk_ctx,
					 MBEDTLS_RSA_PKCS_V15, MBEDTLS_MD_SHA256);

	((mbedtls_rsa_context *)(server->tls_pkey.pk_ctx))->key_index = ssl_config->dev_key_index;
	((mbedtls_rsa_context *)(server->tls_pkey.pk_ctx))->len = 256;

	HTTP_LOGD("Ok\n");

http_exit:
	if (result) {
		return result;
	}
#else
	HTTP_LOGD("  . Loading the server cert. and key...");

	if (ssl_config->dev_cert && ssl_config->private_key) {
		/* 2-1 Load device cert */
		if ((result = mbedtls_x509_crt_parse(&(server->tls_srvcert), (const unsigned char *) ssl_config->dev_cert, ssl_config->dev_cert_len)) != 0) {
			HTTP_LOGE("Error: srv_cert parse fail, return %d\n", result);
			return HTTP_ERROR;
		}

		/* 2-2 Setup device key */
		if ((result = mbedtls_pk_parse_key(&(server->tls_pkey), (const unsigned char *) ssl_config->private_key, ssl_config->private_key_len, NULL, 0)) != 0) {
			HTTP_LOGE("Error: srv_key parse fail, return %d\n", result);
			return HTTP_ERROR;
		}

		if ((result = mbedtls_ssl_conf_own_cert(&(server->tls_conf), &(server->tls_srvcert), &(server->tls_pkey))) != 0) {
			HTTP_LOGE("Error: mbedtls_ssl_conf_own_cert returned %d\n", result);
			return HTTP_ERROR;
		}
	}

	/* 2-3 Load CA cert */
	if (ssl_config->root_ca) {
		mbedtls_x509_crt *chain;

		if ((result = mbedtls_x509_crt_parse(&(server->tls_srvcert), (const unsigned char *) ssl_config->root_ca, ssl_config->root_ca_len)) != 0) {
			HTTP_LOGE("Error: CA_cert parse fail, return %d\n", result);
			return HTTP_ERROR;
		}

		/* CA cert may be first or second in chain depending if server cert was loaded */
		chain = server->tls_srvcert.next ? server->tls_srvcert.next : &server->tls_srvcert;
		mbedtls_ssl_conf_ca_chain(&(server->tls_conf), chain, NULL);
	}

	HTTP_LOGD("Ok\n");

	mbedtls_ssl_conf_rng(&(server->tls_conf), mbedtls_ctr_drbg_random, &(server->tls_ctr_drbg));
	mbedtls_ssl_conf_dbg(&(server->tls_conf), http_tls_debug, stdout);
	mbedtls_ssl_conf_session_cache(&(server->tls_conf), &(server->tls_cache), mbedtls_ssl_cache_get, mbedtls_ssl_cache_set);

#endif /* CONFIG_HW_RSA */

	/*
	 * 3. Setup ssl stuffs
	 */
	HTTP_LOGD("  . Setting up the SSL data...");

	if ((result = mbedtls_ssl_config_defaults(&(server->tls_conf),
				  MBEDTLS_SSL_IS_SERVER,
				  MBEDTLS_SSL_TRANSPORT_STREAM,
				  MBEDTLS_SSL_PRESET_DEFAULT))
		!= 0) {
		HTTP_LOGE("Error: mbedtls_ssl_config_defaults returned %d\n", result);
		return HTTP_ERROR;
	}

	HTTP_LOGD("Ok\n");

	mbedtls_ssl_conf_authmode(&server->tls_conf, ssl_config->auth_mode);

	server->tls_init = 1;
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
