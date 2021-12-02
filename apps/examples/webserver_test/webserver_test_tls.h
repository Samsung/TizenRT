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
#include "webserver_test_util.h"

#include "mbedtls/config.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/certs.h"
#include "mbedtls/x509.h"
#include "mbedtls/ssl.h"
#include "mbedtls/net.h"
#include "mbedtls/error.h"
#include "mbedtls/debug.h"
#include "mbedtls/ssl_cache.h"

#define WS_TEST_CONF_TIMEOUT_MSEC       50000
#define WS_TEST_CONF_MIN_TLS_MEMORY     100000

#define WS_TEST_CONF_HANDSHAKE_RETRY    3
#define WS_TEST_CONF_CHECK_TLS_HOSTNAME 0

#define WS_TEST_SSL_VERIFY_NODE         0
#define WS_TEST_SSL_VERIFY_OPTIONAL     1
#define WS_TEST_SSL_VERIFY_REQUIRED     2
#define WS_TEST_SSL_VERIFY_UNSET        3

int ws_test_tls_handshake(struct http_client_tls_t *client, const char *hostname);
void ws_test_tls_debug(void *ctx, int level, const char *file, int line, const char *str);
void ws_test_tls_release(struct http_client_tls_t *client);
int ws_test_tls_init(struct http_client_tls_t *client, struct http_client_ssl_config_t *ssl_config);
void ws_test_tls_ssl_release(struct http_client_tls_t *client);
int ws_test_tls_handshake(struct http_client_tls_t *client, const char *hostname);

