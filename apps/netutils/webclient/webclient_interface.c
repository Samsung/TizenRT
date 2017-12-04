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
/****************************************************************************
 * netutils/webclient/webclient_interface.c
 * Implementation of the HTTP client.
 *
 *   Copyright (C) 2007, 2009, 2011-2012, 2014 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Based on uIP which also has a BSD style license:
 *
 *   Author: Adam Dunkels <adam@dunkels.com>
 *   Copyright (c) 2002, Adam Dunkels.
 *   All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above
 *    copyright notice, this list of conditions and the following
 *    disclaimer in the documentation and/or other materials provided
 *    with the distribution.
 * 3. The name of the author may not be used to endorse or promote
 *    products derived from this software without specific prior
 *    written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/* This example shows a HTTP client that is able to download web pages
 * and files from web servers. It requires a number of callback
 * functions to be implemented by the module that utilizes the code:
 * webclient_datahandler().
 */

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <tinyara/compiler.h>
#include <debug.h>

#include <sys/socket.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

#ifdef CONFIG_NET_LWIP_NETDB
#include <netdb.h>
#endif
#include <arpa/inet.h>
#include <netinet/in.h>

#include <tinyara/version.h>
#include <apps/netutils/netlib.h>

#include "../webserver/http_string_util.h"
#include "../webserver/http_client.h"
#include <apps/netutils/webclient.h>
#if defined(CONFIG_NETUTILS_CODECS)
#if defined(CONFIG_CODECS_URLCODE)
#define WGET_USE_URLENCODE 1
#include <apps/netutils/urldecode.h>
#endif
#if defined(CONFIG_CODECS_BASE64)
#include <apps/netutils/base64.h>
#endif
#else
#undef CONFIG_CODECS_URLCODE
#undef CONFIG_CODECS_BASE64
#endif

#include "webclient_common.h"
#include "webclient_core.h"
#include <apps/netutils/webclient.h>

/****************************************************************************
 * Public Functions
 ****************************************************************************/

int client_send_request(struct http_client_request_t *request)
{
	pthread_attr_t attr;
	pthread_t tid;
	pthread_addr_t ret = NULL;

	if (request == NULL) {
		ndbg("Error: Request is null\n");
		return -1;
	}

	if (request->method < WGET_MODE_GET || request->method > WGET_MODE_DELETE) {
		ndbg("Error: Incorrect method value!!\n");
		return -1;
	}

	if (request->buffer == NULL) {
		ndbg("Error: Buffer is NULL!!\n");
		return -1;
	}

	if (request->buflen <= 0) {
		ndbg("Error: Buffer length must be bigger than 0!!\n");
		goto errret;
	}

	if (request->encoding != CONTENT_LENGTH && request->encoding != CHUNKED_ENCODING) {
		ndbg("Error: Incorrect encoding value!!\n");
		goto errret;
	}

	if (request->callback) {
		request->async_flag = 1;
		if (pthread_attr_init(&attr) != 0) {
			printf("Error: Cannot initialize thread attribute\n");
			goto errret;
		}
		pthread_attr_setstacksize(&attr, CLIENT_REQ_THREAD_STACK);
		if (pthread_create(&tid, &attr, wget_base_11, (void *)request) != 0) {
			ndbg("Error: Cannot create thread!!\n");
			goto errret;
		}
		pthread_setname_np(tid, "webclient request");
		pthread_detach(tid);
	} else {
		ret = wget_base_11((void *)request);
	}

	return (int)ret;
errret:
	free(request->buffer);
	return -1;
}

static int http_client_send_requests(struct http_client_request_t *request, void *ssl_config, struct http_client_response_t *response, wget_callback_t cb)
{
#ifdef CONFIG_NET_SECURITY_TLS
	struct mallinfo data;
	struct http_client_ssl_config_t *ssl_conf = ssl_config;
#endif

	if (request == NULL) {
		ndbg("Error: Request is null\n");
		return -1;
	}

	request->callback = NULL;
	request->tls = false;
	request->response = response;
	request->callback = cb;

	if (request->url == NULL) {
		ndbg("Error: URL is NULL!!\n");
		return -1;
	}

	/* Entity is only for PUT/POST methods */
	if ((request->method == WGET_MODE_PUT) || (request->method == WGET_MODE_POST)) {
		if (request->entity == NULL) {
			ndbg("Error: PUT, POST method without entity\n");
			return -1;
		}
		if (strlen(request->entity) > WEBCLIENT_CONF_MAX_ENTITY_SIZE) {
			ndbg("Error: Too small buffer size\n");
			return -1;
		}
	}
#ifdef CONFIG_NET_SECURITY_TLS
	if (ssl_conf) {
		request->tls = true;
		memcpy(&request->ssl_config, ssl_conf, sizeof(struct http_client_ssl_config_t));

		data = mallinfo();
		if (data.fordblks < WEBCLIENT_CONF_MIN_TLS_MEMORY) {
			ndbg("Error: Not enough memory!!\n");
			return -1;
		}
	}
#endif

	request->buffer = (char *)malloc(request->buflen);
	if (request->buffer == NULL) {
		ndbg("Error: Fail to malloc buffer\n");
		return -1;
	}

	return client_send_request(request);
}

int http_client_send_request(struct http_client_request_t *request, void *ssl_config, struct http_client_response_t *response)
{
	if (response == NULL) {
		ndbg("Error: Response is null\n");
		return -1;
	}

	return http_client_send_requests(request, ssl_config, response, NULL);
}

int http_client_send_request_async(struct http_client_request_t *request, void *ssl_config, wget_callback_t cb)
{
	if (cb == NULL) {
		ndbg("Error: Callback is Null\n");
		return -1;
	}

	return http_client_send_requests(request, ssl_config, NULL, cb);
}

int http_client_response_init(struct http_client_response_t *response)
{
	response->phrase = malloc(WEBCLIENT_CONF_MAX_PHRASE_SIZE);
	if (response->phrase == NULL) {
		ndbg("Error: Fail to init\n");
		return -1;
	}
	response->message = malloc(WEBCLIENT_CONF_MAX_MESSAGE_SIZE);
	if (response->message == NULL) {
		ndbg("Error: Fail to init\n");
		free(response->phrase);
		return -1;
	}
	response->headers = malloc(sizeof(struct http_keyvalue_list_t));
	if (response->headers == NULL || http_keyvalue_list_init(response->headers) < 0) {
		ndbg("Error: Fail to init\n");
		free(response->phrase);
		free(response->message);
		return -1;
	}
	return 0;
}

void http_client_response_release(struct http_client_response_t *response)
{
	free(response->phrase);
	free(response->message);
	http_keyvalue_list_release(response->headers);
	free(response->headers);
}

#ifdef CONFIG_ENABLE_HTTP20
/****************************************************************************
 * Public Functions for http2.0
 ****************************************************************************/

int client_send_request_20(struct http_client_request_t *request)
{
	int ret;

	if (request == NULL) {
		ndbg("Error: Request is null\n");
		return -1;
	}
	if (request->method < WGET_MODE_GET || request->method > WGET_MODE_DELETE) {
		ndbg("Error: Incorrect method value!!\n");
		return -1;
	}

	if (request->buffer == NULL) {
		ndbg("Error: Buffer is NULL!!\n");
		return -1;
	}

	if (request->buflen <= 0) {
		ndbg("Error: Buffer length must be bigger than 0!!\n");
		goto errret;
	}

	if (request->encoding != CONTENT_LENGTH && request->encoding != CHUNKED_ENCODING) {
		ndbg("Error: Incorrect encoding value!!\n");
		goto errret;
	}

	ret = wget_base_20((void *)request);

	return ret;
errret:
	free(request->buffer);
	return -1;
}

static int http20_client_send_requests(struct http_client_request_t *request, void *ssl_config, wget_callback_t cb)
{
#ifdef CONFIG_NET_SECURITY_TLS
	struct mallinfo data;
	struct http_client_ssl_config_t *ssl_conf = ssl_config;
#endif

	if (request == NULL) {
		ndbg("Error: Request is null\n");
		return -1;
	}

	request->callback = NULL;
	request->tls = false;
	request->callback = cb;

	if (request->url == NULL) {
		ndbg("Error: URL is NULL!!\n");
		return -1;
	}

	/* Entity is only for PUT/POST methods */
	if ((request->method == WGET_MODE_PUT) || (request->method == WGET_MODE_POST)) {
		if (request->entity == NULL) {
			ndbg("Error: PUT, POST method without entity\n");
			return -1;
		}
		if (strlen(request->entity) > WEBCLIENT_CONF_MAX_ENTITY_SIZE) {
			ndbg("Error: Too small buffer size\n");
			return -1;
		}
	}

#ifdef CONFIG_NET_SECURITY_TLS
	if (ssl_conf) {
		request->tls = true;
		memcpy(&request->ssl_config, ssl_conf, sizeof(struct http_client_ssl_config_t));

		data = mallinfo();
		if (data.fordblks < WEBCLIENT_CONF_MIN_TLS_MEMORY) {
			ndbg("Error: Not enough memory!!\n");
			return -1;
		}
	}
#endif

	request->buffer = (char *)malloc(request->buflen);
	if (request->buffer == NULL) {
		ndbg("Error: Fail to malloc buffer\n");
		return -1;
	}

	return client_send_request_20(request);
}

int http20_client_send_request(struct http_client_request_t *request, void *ssl_config, wget_callback_t cb)
{
	if (cb == NULL) {
		ndbg("Error: Callback is null\n");
		return -1;
	}

	return http20_client_send_requests(request, ssl_config, cb);
}

int http20_client_response_init(struct http_client_response_t *response)
{
	response->phrase = malloc(WEBCLIENT_CONF_MAX_PHRASE_SIZE);
	if (response->phrase == NULL) {
		ndbg("Error: Fail to init\n");
		return -1;
	}
	response->message = malloc(WEBCLIENT_CONF_MAX_MESSAGE_SIZE);
	if (response->message == NULL) {
		ndbg("Error: Fail to init\n");
		free(response->phrase);
		return -1;
	}
	response->headers = malloc(sizeof(struct http_keyvalue_list_t));
	if (response->headers == NULL || http_keyvalue_list_init(response->headers) < 0) {
		ndbg("Error: Fail to init\n");
		free(response->phrase);
		free(response->message);
		return -1;
	}
	return 0;
}

void http20_client_response_release(struct http_client_response_t *response)
{
	free(response->phrase);
	free(response->message);
	http_keyvalue_list_release(response->headers);
	free(response->headers);
}

int http20_client_init(struct http_client_http20_t *http2, struct http_client_ssl_config_t *ssl_config)
{
	int i;
	struct mq_attr mqattr;

	http2->mem = nghttp2_mem_default();
	nghttp2_hd_deflate_init(&http2->deflater, http2->mem);
	nghttp2_hd_inflate_init(&http2->inflater, http2->mem);
	http2->msg.sndlen = 0;
	http2->sock_fd = -1;
	for (i = 0; i < SETTINGS_PARAMS; ++i) {
		http2->iv[i].settings_id = 0;
		http2->iv[i].value = 0;
	}
	mqattr.mq_maxmsg = 5;
	mqattr.mq_msgsize = sizeof(struct http_client_queued_frame_t);
	mqattr.mq_flags = 0;
	http2->msg_q = mq_open("wc_mq", O_RDWR | O_CREAT | O_NONBLOCK, 0666, &mqattr);
	if (http2->msg_q == (mqd_t) ERROR) {
		ndbg("ERROR: mq_open failed\n");
		return -1;
	}
#ifdef CONFIG_NET_SECURITY_TLS
	http2->ishandshaked = 0;
	if (ssl_config != NULL) {
		http2->client_tls = (struct http_client_tls_t *)malloc(sizeof(struct http_client_tls_t));
		if (http2->client_tls == NULL) {
			return -1;
		}
		if (webclient_tls_init(http2->client_tls, ssl_config)) {
			free(http2->client_tls);
			return -1;
		}
	} else {
		http2->client_tls = NULL;
	}
#endif
	http2->init = true;
	return 0;
}

void http20_client_release(struct http_client_http20_t *http2)
{
	if (http2->init == false) {
		return;
	}
	nghttp2_hd_inflate_free(&http2->inflater);
	nghttp2_hd_deflate_free(&http2->deflater);
	mq_unlink("wc_mq");
#ifdef CONFIG_NET_SECURITY_TLS
	if (http2->client_tls) {
		if (http2->ishandshaked) {
			wget_tls_ssl_release(http2->client_tls);
		}
		wget_tls_release(http2->client_tls);
		http2->sock_fd = -1;
		free(http2->client_tls);
	}
#endif
	if (http2->sock_fd > 0) {
		close(http2->sock_fd);
		http2->sock_fd = -1;
	}
}
#endif							/* CONFIG_ENABLE_HTTP20 */
