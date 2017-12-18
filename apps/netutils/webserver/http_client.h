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

#include <apps/netutils/webserver/http_server.h>
#include <apps/netutils/webclient.h>
#include <apps/netutils/websocket.h>

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

enum {
	HTTP_REQUEST_HEADER, HTTP_REQUEST_PARAMETERS, HTTP_REQUEST_BODY
};

struct http_client_t {
	int client_fd;
	struct http_server_t *server;
	int ws_state;

#ifdef CONFIG_NETUTILS_WEBSOCKET
	unsigned char ws_key[WEBSOCKET_CLIENT_KEY_LEN];
#endif

#ifdef CONFIG_NET_SECURITY_TLS
	mbedtls_ssl_context tls_ssl;
	mbedtls_net_context tls_client_fd;
#endif
};

struct http_message_len_t {
	int chunked_remain;
	int sentence_start;
	int message_len;
	int entity_len;
	int content_len;
};

void *http_handle_client(void *arg /* struct http_client_t *client */);

int http_parse_message(char *buf, int buf_len, int *method, char *url, char **body, int *enc, int *state, struct http_message_len_t *len, struct http_keyvalue_list_t *params, struct http_client_t *client, struct http_client_response_t *response, struct http_req_message *req);

#ifdef CONFIG_NET_SECURITY_TLS
int http_client_tls_init(struct http_client_t *client);
int http_client_tls_release(struct http_client_t *client);
int http_server_tls_release(struct http_server_t *server);
#endif

#endif
