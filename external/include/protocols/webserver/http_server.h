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
 * @file protocols/webserver/http_server.h
 * @brief APIs for HTTP Server.
 */

#ifndef __http_server_h__
#define __http_server_h__

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>

#include <stdio.h>
#include <mqueue.h>
#include <netinet/in.h>

#ifdef CONFIG_NETUTILS_WEBSOCKET
#include <protocols/websocket.h>
#endif

#ifdef CONFIG_NET_SECURITY_TLS
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
#endif

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#if defined(CONFIG_NETUTILS_WEBSERVER_MAX_CLIENT_HANDLER)
#define HTTP_CONF_MAX_CLIENT_HANDLE		(CONFIG_NETUTILS_WEBSERVER_MAX_CLIENT_HANDLER)
#else
#define HTTP_CONF_MAX_CLIENT_HANDLE		1
#endif

#define HTTP_METHOD_UNKNOWN -1
#define HTTP_METHOD_GET     0
#define HTTP_METHOD_PUT     1
#define HTTP_METHOD_POST    2
#define HTTP_METHOD_DELETE  3

#define HTTP_HTTP_VERSION_UNKNOWN 0
#define HTTP_HTTP_VERSION_09      9
#define HTTP_HTTP_VERSION_10      10
#define HTTP_HTTP_VERSION_11      11

#define HTTP_CONTENT_LENGTH       0
#define HTTP_CHUNKED_ENCODING     1

#define HTTP_CONF_MAX_CLIENT                    16
#define HTTP_CONF_CLIENT_STACKSIZE              8192
#define HTTP_CONF_MIN_TLS_MEMORY                80000
#define HTTP_CONF_SOCKET_TIMEOUT_MSEC           50000
#define HTTP_CONF_SERVER_MQ_MAX_MSG             10
#define HTTP_CONF_SERVER_MQ_PRIO                50
#define HTTP_CONF_SERVER_SIGWAKEUP              18

#define HTTP_CONF_MAX_REQUEST_LENGTH            4096
#define HTTP_CONF_MAX_REQUEST_LINE_LENGTH       256
#define HTTP_CONF_MAX_REQUEST_HEADER_URL_LENGTH 128
#define HTTP_CONF_MAX_URL_QUERY_LENGTH          64
#define HTTP_CONF_MAX_URL_PARAMS_LENGTH         256
#define HTTP_CONF_MAX_KEY_LENGTH                32
#define HTTP_CONF_MAX_VALUE_LENGTH              256
#define HTTP_CONF_MAX_DIVIDED_PATH_LENGTH       32
#define HTTP_CONF_MAX_SLASH_COUNT               32
#define HTTP_CONF_MAX_QUERY_HANDLER_COUNT       64
#define HTTP_CONF_MAX_ENTITY_LENGTH             2048

#define HTTP_ERROR_400            "Bad Request"
#define HTTP_ERROR_404            "Not Found"
#define HTTP_ERROR_500            "Internal Server Error"

#ifdef __cplusplus
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif

/****************************************************************************
 * Public Types
 ****************************************************************************/

struct http_client_t;
struct http_keyvalue_list_t;

/**
 * @brief http server ssl config structure.
 */

struct ssl_config_t {
	char *root_ca;
	char *dev_cert;
	char *private_key;
	unsigned int root_ca_len;
	unsigned int dev_cert_len;
	unsigned int private_key_len;
	int auth_mode;
};

typedef enum {
	HTTP_SERVER_INIT,
	HTTP_SERVER_RUN,
	HTTP_SERVER_STOP_REQ,
	HTTP_SERVER_STOP,
} http_server_state_t;

/**
 * @brief http request message.
 */

struct http_req_message {
	char *req_msg;
	int method;
	uint32_t client_ip;
	char *url;
	struct http_keyvalue_list_t *headers;
	char *entity;
	char *query_string;
	int encoding;
};

/**
 * @brief typedef for callback function.
 */

typedef void (*http_cb_t)(struct http_client_t *client, struct http_req_message *msg);

/**
 * @brief http server structure.
 */

struct http_server_t {
	int  port;
	int  listen_fd;
	http_server_state_t state;
	sem_t sem_thread_sync;
	pthread_t tid;
	pthread_t c_tid[HTTP_CONF_MAX_CLIENT_HANDLE];
	mqd_t msg_q;

	int                       tls_init;
#ifdef CONFIG_NET_SECURITY_TLS
	mbedtls_ssl_config        tls_conf;
	mbedtls_entropy_context   tls_entropy;
	mbedtls_ctr_drbg_context  tls_ctr_drbg;
	mbedtls_x509_crt          tls_srvcert;
	mbedtls_pk_context        tls_pkey;
	mbedtls_ssl_cache_context tls_cache;
	mbedtls_net_context       tls_ctx;
#endif

	struct sockaddr_in             servaddr;
	http_cb_t cb[4];
	struct http_query_handler_t
	*query_handlers[HTTP_CONF_MAX_QUERY_HANDLER_COUNT];
#ifdef CONFIG_NETUTILS_WEBSOCKET
	websocket_cb_t ws_cb;
#endif
};

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/**
 * @brief http_server_init() initializes the webserver.
 *
 * @param[in] port number of port that executes the webserver.
 * @return On success, address of created struct http_server_t is returned.
 *         On failure, NULL is returned.
 * @since TizenRT v1.0
 */
struct http_server_t *http_server_init(int port);

/**
 * @brief http_server_start() starts the webserver.
 *
 * @param[in] server http_server_t structure pointer returned by http_server_init().
 * @return On success, HTTP_OK(0) is returned.
 *         On failure, HTTP_ERROR(-1) is returned.
 * @since TizenRT v1.0
 */
int http_server_start(struct http_server_t *server);

/**
 * @brief http_server_stop() stops the webserver.
 *        Both HTTP server and HTTPS server are stoped by this function.
 *
 * @param[in] server pointer of the webserver to be stopped.
 * @return On success, HTTP_OK(0) is returned.
 *         On failure, HTTP_ERROR(-1) is returned.
 * @since TizenRT v1.0
 */
int http_server_stop(struct http_server_t *server);

/**
 * @brief http_server_release() releases the struct http_server_t.
 *
 * @param[in] server http_server_t structure pointer of the webserver.
 * @return none
 * @since TizenRT v1.0
 */
void http_server_release(struct http_server_t **server);

/**
 * @brief http_server_register_cb() registers the cb function to each method on webserver.
 *
 * @param[in] server http_server_t structure pointer of the webserver.
 * @param[in] method number of method to register cb.
 *                   - HTTP_METHOD_GET
 *                   - HTTP_METHOD_PUT
 *                   - HTTP_METHOD_POST
 *                   - HTTP_METHOD_DELETE
 * @param[in] url_format url to register cb.
 * @param[in] func pointer of the callback function.
 * @return On success, HTTP_OK(0) is returned.
 *         On failure, HTTP_ERROR(-1) is returned.
 * @since TizenRT v1.0
 */
int http_server_register_cb(struct http_server_t *server, int method, const char *url_format, http_cb_t func);

/**
 * @brief http_server_deregister_cb() deregisters the cb function to each method on webserver.
 *
 * @param[in] server http_server_t structure pointer of the webserver.
 * @param[in] method number of method to deregister cb.
 *                   - HTTP_METHOD_GET
 *                   - HTTP_METHOD_PUT
 *                   - HTTP_METHOD_POST
 *                   - HTTP_METHOD_DELETE
 * @param[in] url_format url to deregister cb.
 * @return On success, HTTP_OK(0) is returned.
 *         On failure, HTTP_ERROR(-1) is returned.
 * @since TizenRT v1.0
 */
int http_server_deregister_cb(struct http_server_t *server, int method, const char *url_format);

/**
 * @brief http_send_response() sends the response.
 *        If receive request, you must send a response by this function.
 *
 * @param[in] server a pointer of HTTP request.
 * @param[in] status status code of a response.
 * @param[in] body If status is 200, it would be entity.
 *                 Else it would be reason phrase.
 * @param[in] headers HTTP headers of a response.
 * @return On success, HTTP_OK(0) is returned.
 *         On failure, HTTP_ERROR(-1) is returned.
 * @since TizenRT v1.0
 */
int http_send_response(struct http_client_t *client, int status, const char *body, struct http_keyvalue_list_t *headers);

#ifdef CONFIG_NET_SECURITY_TLS
/**
 * @brief http_tls_init() initializes the TLS configuere for webserver.
 *
 * @param[in] server http_server_t structure pointer of the webserver.
 * @param[in] ssl_config ssl_config_t structure pointer of the certification.
 * @return On success, HTTP_OK(0) is returned.
 *         On failure, HTTP_ERROR(-1) is returned.
 * @since TizenRT v1.0
 */
int http_tls_init(struct http_server_t *server, struct ssl_config_t *ssl_config);
#endif


#undef EXTERN
#ifdef __cplusplus
}
#endif

#endif
/** @} */
