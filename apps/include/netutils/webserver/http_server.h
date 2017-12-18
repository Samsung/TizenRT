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
/**
 * @defgroup HTTP HTTP
 * @ingroup NETWORK
 */

/**
 * @defgroup HTTP_SERVER HTTP Server
 * @brief Provides APIs for HTTP Server
 * @ingroup HTTP
 *   The server side of Hypertext Transfer Protocol
 * @{
 */

/**
 * @file http_server.h
 * @brief APIs for HTTP Server.
 */

#ifndef __http_server_h__
#define __http_server_h__

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <stdio.h>
#include <mqueue.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>

#ifdef CONFIG_NETUTILS_WEBSOCKET
#include <apps/netutils/websocket.h>
#endif

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

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/**
 * @cond
 * internal
 */
#define HTTP_METHOD_UNKNOWN        -1
#define HTTP_METHOD_GET             0
#define HTTP_METHOD_PUT             1
#define HTTP_METHOD_POST            2
#define HTTP_METHOD_DELETE          3

#define HTTP_HTTP_VERSION_UNKNOWN   0
#define HTTP_HTTP_VERSION_09        9
#define HTTP_HTTP_VERSION_10        10
#define HTTP_HTTP_VERSION_11        11

#define HTTP_CONTENT_LENGTH         0
#define HTTP_CHUNKED_ENCODING       1

#define HTTP_CONF_SERVER_SIGWAKEUP  18

/**
 * @endcond
 */

/**
 * @brief The number of maximum clients
 */
#define HTTP_CONF_MAX_CLIENT                    16

/**
 * @brief The number of maximum client handler threads
 */
#if defined(CONFIG_NETUTILS_WEBSERVER_MAX_CLIENT_HANDLER)
#define HTTP_CONF_MAX_CLIENT_HANDLE		(CONFIG_NETUTILS_WEBSERVER_MAX_CLIENT_HANDLER)
#else
#define HTTP_CONF_MAX_CLIENT_HANDLE		1
#endif

/**
 * @brief The maximum size of client threads stack
 */
#define HTTP_CONF_CLIENT_STACKSIZE              8192

/**
 * @brief Minimum memory size for tls handshake
 */
#define HTTP_CONF_MIN_TLS_MEMORY                80000

/**
 * @brief Socket recv timeout milisecond
 */
#define HTTP_CONF_SOCKET_TIMEOUT_MSEC           5000

/**
 * @brief The maximum number of messages in message queue
 */
#define HTTP_CONF_SERVER_MQ_MAX_MSG             10

/**
 * @brief The maximum size of request buffer
 */
#define HTTP_CONF_MAX_REQUEST_LENGTH            4096

/**
 * @brief The maximum size of request url
 */
#define HTTP_CONF_MAX_REQUEST_HEADER_URL_LENGTH 128

/**
 * @brief The maximum size of query url
 */
#define HTTP_CONF_MAX_URL_QUERY_LENGTH          64

/**
 * @brief The maximum size of query parameter
 */
#define HTTP_CONF_MAX_URL_PARAMS_LENGTH         256

/**
 * @brief The maximum size of key
 */
#define HTTP_CONF_MAX_KEY_LENGTH                32

/**
 * @brief The maximum size of value
 */
#define HTTP_CONF_MAX_VALUE_LENGTH              256

/**
 * @brief The maximum size of divided path
 */
#define HTTP_CONF_MAX_DIVIDED_PATH_LENGTH       32

/**
 * @brief The maximum slash counts
 */
#define HTTP_CONF_MAX_SLASH_COUNT               32

/**
 * @brief The maximum count of query handler loop
 */
#define HTTP_CONF_MAX_QUERY_HANDLER_COUNT       64

/**
 * @brief The maximum size of entity
 */
#define HTTP_CONF_MAX_ENTITY_LENGTH             2048

/** @cond HIDDEN */
#ifdef __cplusplus
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif
/** @endcond */

/****************************************************************************
 * Public Types
 ****************************************************************************/

struct http_client_t;
struct http_keyvalue_list_t;

/**
 * @brief SSL config structure for TLS handshaking
 */
struct ssl_config_t {
	char *root_ca;                  ///< CA certificate buffer
	char *dev_cert;                 ///< Device certificate buffer
	char *private_key;              ///< Private key buffer
	unsigned int root_ca_len;       ///< Length of ca certificate
	unsigned int dev_cert_len;      ///< Length of device certificate
	unsigned int private_key_len;   ///< Length of private key
	int auth_mode;                  ///< Authentication mode (0:none, 1:optional, 2:mandatory)
};

/**
 * @brief HTTP status enumerations
 */
typedef enum {
	HTTP_SERVER_INIT,       ///< Initialized
	HTTP_SERVER_RUN,        ///< Running
	HTTP_SERVER_STOP_REQ,   ///< Sent stop request
	HTTP_SERVER_STOP,       ///< Stopped
} http_server_state_t;

/**
 * @brief Data structure for handling client requests
 */
struct http_req_message {
	char *req_msg;                          ///< Request message
	int method;                             ///< Types of method
	uint32_t client_ip;                     ///< Source IP address
	char *url;                              ///< Request url
	struct http_keyvalue_list_t *headers;   ///< Request keyvalue structure
	char *entity;                           ///< Request entity
	char *query_string;                     ///< Request query string
	int encoding;                           ///< Encoding type
};

/**
 * @brief Response callback prototype
 */
typedef void (*http_cb_t)(struct http_client_t *client, struct http_req_message *msg);

/**
 * @brief Webserver context structure
 */
struct http_server_t {
	int  port;                                      ///< Port number
	int  listen_fd;                                 ///< Listening fd
	http_server_state_t state;                      ///< Server status
	pthread_t tid;                                  ///< Thread id for handling asynchronous mode
	pthread_t c_tid[HTTP_CONF_MAX_CLIENT_HANDLE];   ///< Client thread id
	mqd_t msg_q;                                    ///< Message queue descriptor

	int                       tls_init;             ///< TLS init flag
#ifdef CONFIG_NET_SECURITY_TLS
	mbedtls_ssl_config        tls_conf;             ///< TLS configuration structure
	mbedtls_entropy_context   tls_entropy;          ///< TLS entropy structure
	mbedtls_ctr_drbg_context  tls_ctr_drbg;         ///< TLS deterministic random generator structure
	mbedtls_x509_crt          tls_srvcert;          ///< Server certificate
	mbedtls_pk_context        tls_pkey;             ///< Server private key
	mbedtls_ssl_cache_context tls_cache;            ///< TLS cache structure
	mbedtls_net_context       tls_ctx;              ///< TLS context structure
#endif

	struct sockaddr_in             servaddr;        ///< IP address of server
	http_cb_t cb[4];                                ///< Callback handle
	struct http_query_handler_t
	*query_handlers[HTTP_CONF_MAX_QUERY_HANDLER_COUNT]; ///< Query handler structure
#ifdef CONFIG_NETUTILS_WEBSOCKET
	websocket_cb_t ws_cb;                           ///< Callback for websocket
#endif
};

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/**
 * @brief Initialize the webserver context structure.
 *
 * @param[in] port Port number for running webserver
 * @return On success, address of created struct http_server_t is returned.
 *         On failure, NULL is returned.
 */
struct http_server_t *http_server_init(int port);

/**
 * @brief Start the webserver daemon.
 *
 * @param[in] server Webserver context structure to use
 * @return On success, HTTP_OK(0) is returned.
 *         On failure, HTTP_ERROR(-1) is returned.
 */
int http_server_start(struct http_server_t *server);

/**
 * @brief Stop the webserver daemon.
 *
 * @param[in] server Webserver context structure to use
 * @return On success, HTTP_OK(0) is returned.
 *         On failure, HTTP_ERROR(-1) is returned.
 */
int http_server_stop(struct http_server_t *server);

/**
 * @brief Release the webserver context structure.
 *
 * @param[in] server Webserver context structure to be released
 */
void http_server_release(struct http_server_t **server);

/**
 * @brief Register the cb function to each method on webserver.
 *
 * @param[in] server Webserver context structure to use
 * @param[in] method Number of method type to register cb
 *                   - HTTP_METHOD_GET
 *                   - HTTP_METHOD_PUT
 *                   - HTTP_METHOD_POST
 *                   - HTTP_METHOD_DELETE
 * @param[in] url_format Url to register cb
 * @param[in] func Pointer of the callback function
 * @return On success, HTTP_OK(0) is returned.
 *         On failure, HTTP_ERROR(-1) is returned.
 */
int http_server_register_cb(struct http_server_t *server, int method, const char *url_format, http_cb_t func);

/**
 * @brief Deregister the cb function to each method on webserver.
 *
 * @param[in] server Webserver context structure to use
 * @param[in] method Number of method type to deregister cb
 *                   - HTTP_METHOD_GET
 *                   - HTTP_METHOD_PUT
 *                   - HTTP_METHOD_POST
 *                   - HTTP_METHOD_DELETE
 * @param[in] url_format Url to deregister cb
 * @return On success, HTTP_OK(0) is returned.
 *         On failure, HTTP_ERROR(-1) is returned.
 */
int http_server_deregister_cb(struct http_server_t *server, int method, const char *url_format);

/**
 * @brief Send the response.
 *        If receive request, you must send a response by this function.
 *
 * @param[in] client Webserver sub-context structure for handling request
 * @param[in] status Status code of response
 * @param[in] body Entity or phrase pointer depens on status
 * @param[in] headers HTTP header of response
 * @return On success, HTTP_OK(0) is returned.
 *         On failure, HTTP_ERROR(-1) is returned.
 */
int http_send_response(struct http_client_t *client, int status, const char *body, struct http_keyvalue_list_t *headers);

#ifdef CONFIG_NET_SECURITY_TLS
/**
 * @brief Initialize TLS context for webserver.
 *
 * @param[in] server Webserver context structure to use
 * @param[in] ssl_config SSL configuration including certificates and private key
 * @return On success, HTTP_OK(0) is returned.
 *         On failure, HTTP_ERROR(-1) is returned.
 */
int http_tls_init(struct http_server_t *server, struct ssl_config_t *ssl_config);
#endif

#undef EXTERN
#ifdef __cplusplus
}
#endif

#endif
/** @} */
