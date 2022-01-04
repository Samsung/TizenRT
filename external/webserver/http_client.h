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

#ifndef __http_client_h__
#define __http_client_h__

#include <protocols/webserver/http_server.h>
#include <protocols/webclient.h>
#include <protocols/websocket.h>

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

enum {
	HTTP_REQUEST_HEADER, HTTP_REQUEST_PARAMETERS, HTTP_REQUEST_BODY
};

struct http_client_t {
	int client_fd;
	struct http_server_t *server;
	int ws_state;
	unsigned char ws_key[WEBSOCKET_CLIENT_KEY_LEN];

#ifdef CONFIG_NET_SECURITY_TLS
	mbedtls_ssl_context       tls_ssl;
	mbedtls_net_context       tls_client_fd;
#endif

	int keep_alive;
	int keep_alive_timeout;
	uint32_t max_request;
	uint32_t remaining_request;
	int keep_alive_header_flag;
};

struct http_message_len_t {
	int chunked_remain;
	int sentence_start;
	int message_len;
	int entity_len;
	int content_len;
};

int   http_accept_client(struct http_server_t *server);
void  http_close_client(struct http_client_t *client);
void *http_handle_client(void *arg /* struct http_client_t *client */);

struct http_client_t *http_client_init(struct http_server_t *server, int sock_fd);
int   http_client_release(struct http_client_t *client);

/**
 * @brief http_parse_message parse the http request and resonse message
 * @param[in] buf of http request and response message
 * @param[in] buf_len : http request and response buffer len
 * @param[out] method : http methods HTTP_METHOD_GET HTTP_METHOD_PUT HTTP_METHOD_POST HTTP_METHOD_DELETE 
 * @param[out] url : http url
 * @param[in] body : http entity mesage
 * @param[out] enc : HTTP_CHUNKED_ENCODING or HTTP_CONTENT_LENGTH
 * @param[in] state : http state machine HTTP_REQUEST_HEADER, HTTP_REQUEST_PARAMETERS, HTTP_REQUEST_BODY
 * @param[in] len : http various message len of type http_message_len_t
 * @param[in] params : http headers 
 * @param[in] client : client instance managed by webserver
 * @param[in] response : HTTP response structure called by webclient
 * @param[in] req : http request message
 * @param[out] chunk_processed : Number of byte prcessed
 * @return false when parsing and reading of http message is finished 
 * @API_type: synchronous
 * @callback: none
 */
int http_parse_message(char *buf, int buf_len, int *method, char *url,
					   char **body, int *enc, int *state,
					   struct http_message_len_t *len,
					   struct http_keyvalue_list_t *params,
					   struct http_client_t *client,
					   struct http_client_response_t *response,
					   struct http_req_message *req,
					   int *chunk_processed);
int   http_recv_and_handle_request(struct http_client_t *client, struct http_keyvalue_list_t *request_params);

#ifdef CONFIG_NET_SECURITY_TLS
int   http_client_tls_init(struct http_client_t *client);
int   http_client_tls_release(struct http_client_t *client);
int   http_server_tls_release(struct http_server_t *server);
#endif

#endif
