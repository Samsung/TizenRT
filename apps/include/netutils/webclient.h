/****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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

/**
 * @defgroup HTTP_CLIENT HTTP Client
 * @brief Provides APIs for HTTP Client
 * @ingroup HTTP
 *   The client side of Hypertext Transfer Protocol
 *
 * @{
 */

/**
 * @file webclient.h
 * @brief APIs for HTTP Client.
 */

#ifndef __APPS_INCLUDE_NETUTILS_WEBCLIENT_H
#define __APPS_INCLUDE_NETUTILS_WEBCLIENT_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <sys/types.h>

#ifdef CONFIG_NET_SECURITY_TLS
#include "tls/config.h"
#include "tls/entropy.h"
#include "tls/ctr_drbg.h"
#include "tls/certs.h"
#include "tls/x509.h"
#include "tls/ssl.h"
#include "tls/net.h"
#include "tls/error.h"
#include "tls/debug.h"
#include "tls/ssl_cache.h"
#endif

#include <apps/netutils/webserver/http_keyvalue_list.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/** @cond HIDDEN */
#define WGET_MODE_GET                     0
#define WGET_MODE_POST                    1
#define WGET_MODE_PUT                     2
#define WGET_MODE_DELETE                  3

#define CONTENT_LENGTH                    0
#define CHUNKED_ENCODING                  1
/** @endcond */

/**
 * @brief The maximum length of url
 */
#define WEBCLIENT_CONF_MAX_URL_SIZE       50

/**
 * @brief The maximum buffer size of entity
 */
#define WEBCLIENT_CONF_MAX_ENTITY_SIZE    2048

/**
 * @brief The maximum buffer size of phrase
 */
#define WEBCLIENT_CONF_MAX_PHRASE_SIZE    50

/**
 * @brief The maximum buffer size of entire message
 */
#define WEBCLIENT_CONF_MAX_MESSAGE_SIZE   2100

/**
 * @brief Socket recv timeout milisecond
 */
#ifdef CONFIG_NETUTILS_WEBCLIENT_RX_TIMEOUT
#define WEBCLIENT_CONF_TIMEOUT_MSEC       (CONFIG_NETUTILS_WEBCLIENT_RX_TIMEOUT * 1000)
#else
#define WEBCLIENT_CONF_TIMEOUT_MSEC       5000
#endif

/**
 * @brief Minimum memory size for tls handshake
 */
#define WEBCLIENT_CONF_MIN_TLS_MEMORY     100000

/**
 * @brief The maximum retry count of handshake
 */
#define WEBCLIENT_CONF_HANDSHAKE_RETRY    3

/**
 * @brief List of ALPN (Application-Layer Protocol Negotiation)
 */
#define WEBCLIENT_CONF_ALPN_LIST          "h2\0h2-14\0h2-16\0"

/**
 * @brief Number of ALPN list
 */
#define WEBCLIENT_CONF_ALPN_LIST_NUM      3

/**
 * @brief SSL/TLS hostname check option
 */
#define WEBCLIENT_CONF_CHECK_TLS_HOSTNAME 0

/**
 * @brief SSL/TLS certificate verify option
 */
#define WEBCLIENT_CONF_SSL_VERIFY_LEVEL   MBEDTLS_SSL_VERIFY_NONE

/**
 * @brief Magic string for http2
 */
#define HTTP2_MAGIC_STRING                "PRI * HTTP/2.0\r\n\r\nSM\r\n\r\n"

/**
 * @brief Magic string length for http2
 */
#define HTTP2_MAGIC_STRING_LEN            24

/**
 * @brief The number of setting parameters
 */
#define SETTINGS_PARAMS                   6

/****************************************************************************
 * Public types
 ****************************************************************************/

/** @cond HIDDEN */
typedef struct http_client_response_t *httprsp;
typedef void (*wget_callback_t)(httprsp);
/** @endcond */

#ifdef CONFIG_NET_SECURITY_TLS
/**
 * @brief HTTP client TLS structure
 */
struct http_client_tls_t {
	int client_fd;                          ///< Socket file descriptor
	mbedtls_ssl_context tls_ssl;            ///< TLS context
	mbedtls_net_context tls_client_fd;      ///< TLS network context

	int tls_init;                           ///< Initialized status of TLS
	mbedtls_ssl_config tls_conf;            ///< TLS configurations
	mbedtls_entropy_context tls_entropy;    ///< TLS entropy context
	mbedtls_ctr_drbg_context tls_ctr_drbg;  ///< TLS deterministic random generator structure
	mbedtls_x509_crt tls_clicert;           ///< TLS client certificate context
	mbedtls_pk_context tls_pkey;            ///< TLS private key context
	mbedtls_ssl_session tls_session;        ///< TLS session information
};
#endif

/**
 * @brief SSL configure structure
 */
struct http_client_ssl_config_t {
	char *root_ca;                  ///< CA certificate buffer
	char *dev_cert;                 ///< Device certificate buffer
	char *private_key;              ///< Private key buffer
	unsigned int root_ca_len;       ///< Length of ca certificate
	unsigned int dev_cert_len;      ///< Length of device certificate
	unsigned int private_key_len;   ///< Length of private key
	int auth_mode;                  ///< Authentication mode (0:none, 1:optional, 2:mandatory)
	mbedtls_ssl_config * tls_conf;  ///< Custom SSL configuration (useful when using Secure Element)
};

/**
 * @brief HTTP request structure
 */
struct http_client_request_t {
	int method;                                  ///< Request method
	char *url;                                   ///< Request url
	char *buffer;                                ///< Request buffer to recv/send data
	int buflen;                                  ///< Request buffer length
	int encoding;                                ///< Request encoding type (content-length/chunked)
	int tls;                                     ///< Request TLS mode (disable:0 enable:1)
	char *entity;                                ///< Request entity buffer
	wget_callback_t callback;                    ///< Callback function pointer to response
	struct http_keyvalue_list_t *headers;        ///< Keyvalue list structure
	struct http_client_response_t *response;     ///< Buffer for response message
#ifdef CONFIG_NET_SECURITY_TLS
	struct http_client_ssl_config_t ssl_config;  ///< SSL configure structure
#endif
	int async_flag;                              ///< Async-request status flag
};

/**
 * @brief HTTP response structure
 */
struct http_client_response_t {
	int method;                             ///< Response method, same with request method
	char *url;                              ///< Response url, same with request url
	int status;                             ///< Response status
	char *phrase;                           ///< Response phrase
	struct http_keyvalue_list_t *headers;   ///< Response keyvalue list buffer
	char *message;                          ///< Response message buffer
	char *entity;                           ///< Response entity buffer
	unsigned int entity_len;                ///< Response entity length
};

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/** @cond HIDDEN */
#ifdef __cplusplus
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif
/** @endcond */

/**
 * @brief Send the HTTP request to HTTP server with asynchronous function.
 *
 * @param[in] request Structure pointer of request
 * @param[in] ssl_config Structure pointer of TLS config
 * @param[in] cb Callback function pointer for response
 * @return On success, OK(0) is returned.
 *         On failure, negative value is returned.
 */

int http_client_send_request_async(struct http_client_request_t *request, void *ssl_config, wget_callback_t cb);

/**
 * @brief Initialize the response structure.
 *
 * @param[in] response Structure pointer of response message
 * @return On success, OK(0) is returned.
 *         On failure, negative value is returned.
 */

int http_client_response_init(struct http_client_response_t *response);

/**
 * @brief Release the response structure.
 *
 * @param[in] response Structure pointer of response message
 */
void http_client_response_release(struct http_client_response_t *response);

#undef EXTERN
#ifdef __cplusplus
}
#endif

#endif							/* __APPS_INCLUDE_NETUTILS_WEBCLIENT_H */
/**@} */
