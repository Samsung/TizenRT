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
 * netutils/webclient/webclient.c
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

int wget_msg_construct_11(char *buf, struct http_client_request_t *param, struct wget_s *ws)
{
	int post_len;
	char *dest = buf;
	char post_size[8];
	struct http_keyvalue_t *cur = NULL;

	/* Send method */
	if (param->method == WGET_MODE_GET) {
		dest = wget_strcpy(dest, g_httpget, param);
	} else if (param->method == WGET_MODE_POST) {
		dest = wget_strcpy(dest, g_httppost, param);
	} else if (param->method == WGET_MODE_PUT) {
		dest = wget_strcpy(dest, g_httpput, param);
	} else if (param->method == WGET_MODE_DELETE) {
		dest = wget_strcpy(dest, g_httpdelete, param);
	}

	if (dest == NULL) {
		return WGET_MSG_CONSTRUCT_ERR;
	}

	dest = wget_strcpy(dest, ws->filename, param);
	if (dest == NULL) {
		return WGET_MSG_CONSTRUCT_ERR;
	}

	*dest++ = ISO_space;
	dest = wget_strcpy(dest, g_http11, param);
	if (dest == NULL) {
		return WGET_MSG_CONSTRUCT_ERR;
	}
	dest = wget_strcpy(dest, g_httpcrnl, param);
	if (dest == NULL) {
		return WGET_MSG_CONSTRUCT_ERR;
	}
	dest = wget_strcpy(dest, g_httphost, param);
	if (dest == NULL) {
		return WGET_MSG_CONSTRUCT_ERR;
	}
	dest = wget_strcpy(dest, ws->hostname, param);
	if (dest == NULL) {
		return WGET_MSG_CONSTRUCT_ERR;
	}
	dest = wget_strcpy(dest, g_httpcrnl, param);
	if (dest == NULL) {
		return WGET_MSG_CONSTRUCT_ERR;
	}

	/* header of entity */

	if (param->method == WGET_MODE_POST || param->method == WGET_MODE_PUT) {
		/* Look for Content-Type in the headers */
		cur = param->headers->head->next;
		while (cur != param->headers->tail) {
			if (!strncmp(cur->key, "Content-Type", strlen("Content-Type"))) {
				break;
			}
			cur = cur->next;
		}

		/* Add default Content-Type if not found in headers */
		if (cur == param->headers->tail) {
			dest = wget_strcpy(dest, g_httpform, param);
			if (dest == NULL) {
				return WGET_MSG_CONSTRUCT_ERR;
			}
			dest = wget_strcpy(dest, g_httpcrnl, param);
			if (dest == NULL) {
				return WGET_MSG_CONSTRUCT_ERR;
			}
		}

		/* content length */

		if (!param->encoding) {
			dest = wget_strcpy(dest, g_httpcontsize, param);
			if (dest == NULL) {
				return WGET_MSG_CONSTRUCT_ERR;
			}
			post_len = strlen((char *)param->entity);
			sprintf(post_size, "%d", post_len);
			dest = wget_strcpy(dest, post_size, param);
			if (dest == NULL) {
				return WGET_MSG_CONSTRUCT_ERR;
			}
			dest = wget_strcpy(dest, g_httpcrnl, param);
			if (dest == NULL) {
				return WGET_MSG_CONSTRUCT_ERR;
			}
		}

		/* chuncked param->encoding */

		else {
			dest = wget_strcpy(dest, g_httpchunked, param);
			if (dest == NULL) {
				return WGET_MSG_CONSTRUCT_ERR;
			}
			dest = wget_strcpy(dest, g_httpcrnl, param);
			if (dest == NULL) {
				return WGET_MSG_CONSTRUCT_ERR;
			}
		}
	}

	cur = param->headers->head->next;
	while (cur != param->headers->tail) {
		dest = wget_strcpy(dest, cur->key, param);
		if (dest == NULL) {
			return WGET_MSG_CONSTRUCT_ERR;
		}
		dest = wget_strcpy(dest, ": ", param);
		if (dest == NULL) {
			return WGET_MSG_CONSTRUCT_ERR;
		}
		dest = wget_strcpy(dest, cur->value, param);
		if (dest == NULL) {
			return WGET_MSG_CONSTRUCT_ERR;
		}
		dest = wget_strcpy(dest, g_httpcrnl, param);
		if (dest == NULL) {
			return WGET_MSG_CONSTRUCT_ERR;
		}
		cur = cur->next;
	}
	dest = wget_strcpy(dest, g_httpcrnl, param);
	if (dest == NULL) {
		return WGET_MSG_CONSTRUCT_ERR;
	}

	/* entity is needed POST or PUT method */

	if (param->method == WGET_MODE_POST || param->method == WGET_MODE_PUT) {

		/* content length */

		if (!param->encoding) {
			dest = wget_strcpy(dest, (char *)param->entity, param);
			if (dest == NULL) {
				return WGET_MSG_CONSTRUCT_ERR;
			}
		}

		/* chunked param->encoding */

		else {
			post_len = strlen(param->entity);
			if (post_len > param->buflen) {
				dest = wget_chunksize(dest, param->buflen, param);
				if (dest == NULL) {
					return WGET_MSG_CONSTRUCT_ERR;
				}
				dest = wget_strcpy(dest, g_httpcrnl, param);
				if (dest == NULL) {
					return WGET_MSG_CONSTRUCT_ERR;
				}
				dest = wget_strlencpy(dest, (char *)param->entity, param->buflen, param);
				if (dest == NULL) {
					return WGET_MSG_CONSTRUCT_ERR;
				}
				dest = wget_strcpy(dest, g_httpcrnl, param);
				if (dest == NULL) {
					return WGET_MSG_CONSTRUCT_ERR;
				}
			} else {
				dest = wget_chunksize(dest, post_len, param);
				if (dest == NULL) {
					return WGET_MSG_CONSTRUCT_ERR;
				}
				dest = wget_strcpy(dest, g_httpcrnl, param);
				if (dest == NULL) {
					return WGET_MSG_CONSTRUCT_ERR;
				}
				dest = wget_strcpy(dest, (char *)param->entity, param);
				if (dest == NULL) {
					return WGET_MSG_CONSTRUCT_ERR;
				}
				dest = wget_strcpy(dest, g_httpcrnl, param);
				if (dest == NULL) {
					return WGET_MSG_CONSTRUCT_ERR;
				}
				dest = wget_strlencpy(dest, "0", 1, param);
				if (dest == NULL) {
					return WGET_MSG_CONSTRUCT_ERR;
				}
				dest = wget_strcpy(dest, g_httpcrnl, param);
				if (dest == NULL) {
					return WGET_MSG_CONSTRUCT_ERR;
				}
			}
		}
	}
	return dest - param->buffer;
}

/****************************************************************************
 * Name: wget_base_11
 *
 * Description:
 *   Obtain the requested file from an HTTP server using the GET method.
 *
 *   Note: If the function is passed a host name, it must already be in
 *   the resolver cache in order for the function to connect to the web
 *   server. It is therefore up to the calling module to implement the
 *   resolver calls and the signal handler used for reporting a resolv
 *   query answer.
 *
 * Input Parameters
 *   url      - A pointer to a string containing either the full URL to
 *              the file to get (e.g., http://www.nutt.org/index.html, or
 *              http://192.168.23.1:80/index.html).
 *   buffer   - A user provided buffer to receive the file data (also
 *              used for the outgoing GET request
 *   buflen   - The size of the user provided buffer
 *   callback - As data is obtained from the host, this function is
 *              to dispose of each block of file data as it is received.
 *   mode     - Indicates GET or POST modes
 *
 * Returned Value:
 *   0: if the GET operation completed successfully;
 *  -1: On a failure with errno set appropriately
 *
 ****************************************************************************/
#ifdef CONFIG_NET_SECURITY_TLS
pthread_addr_t wget_base_11(void *arg)
{
	int sockfd = -1;
	int ret;
	int buf_len, sndlen, len;
	int remain = WEBCLIENT_CONF_MAX_MESSAGE_SIZE;
	int encoding = CONTENT_LENGTH;
	int state = HTTP_REQUEST_HEADER;
	struct http_message_len_t mlen = { 0, };
	struct wget_s ws;
	struct http_client_request_t *param = (struct http_client_request_t *)arg;
	struct http_client_response_t response = { 0, };
	bool read_finish = false;
	bool keep_connect = false;
	int callback_count = 0;

	struct http_client_tls_t *client_tls = NULL;
	int handshake_retry = WEBCLIENT_CONF_HANDSHAKE_RETRY;
	if (param->tls) {
#ifdef CONFIG_ENABLE_HTTP20
		if (param->http2->init) {
			client_tls = param->http2->client_tls;
		} else
#endif
		{
			client_tls = (struct http_client_tls_t *)malloc(sizeof(struct http_client_tls_t));
		}
		if (client_tls == NULL) {
			free(param->buffer);
			return (pthread_addr_t) WGET_ERR;
		}
	}

	/* Initialize the state structure */
	memset(&ws, 0, sizeof(struct wget_s));
	ws.buffer = param->buffer;
	ws.buflen = param->buflen;

	/* Parse the hostname (with optional port number) and filename from the URL */
	ret = netlib_parsehttpurl(param->url, &ws.port, ws.hostname, CONFIG_WEBCLIENT_MAXHOSTNAME, ws.filename, CONFIG_WEBCLIENT_MAXFILENAME);
	if (ret != 0) {
		ndbg("ERROR: Malformed HTTP URL: %s\n", param->url);
		free(param->buffer);
		free(client_tls);
		return (pthread_addr_t) WGET_ERR;
	}

	nvdbg("hostname='%s' filename='%s'\n", ws.hostname, ws.filename);

	if (param->tls && webclient_tls_init(client_tls, &param->ssl_config)) {
		ndbg("Fail to client tls init\n");
		goto errout_before_tlsinit;
	}

	/* Re-initialize portions of the state structure that could have
	 * been left from the previous time through the loop and should not
	 * persist with the new connection.
	 */
	ws.httpstatus = HTTPSTATUS_NONE;
	ws.offset = 0;
	ws.datend = 0;
	ws.ndx = 0;

	sndlen = wget_msg_construct_11(ws.buffer, param, &ws);
	if (sndlen <= 0) {
		ndbg("ERROR: construction message failed\n");
		goto errout_before_tlsinit;
	}
#ifdef CONFIG_ENABLE_HTTP20
	if (param->http2->init && param->http2->sock_fd != -1) {
		goto send_message;
	}
#endif

retry:
	if ((sockfd = wget_socket_connect(&ws)) < 0) {
		ndbg("ERROR: socket failed: %d\n", errno);
		goto errout_before_tlsinit;
	}

	if (client_tls != NULL) {
		client_tls->client_fd = sockfd;
		ret = wget_tls_handshake(client_tls, ws.hostname);
		if (param->tls && ret) {
			if (handshake_retry-- > 0) {
				if (ret == MBEDTLS_ERR_NET_SEND_FAILED || ret == MBEDTLS_ERR_NET_RECV_FAILED || ret == MBEDTLS_ERR_SSL_CONN_EOF) {
					ndbg("Handshake again....\n");
					mbedtls_net_free(&(client_tls->tls_client_fd));
					mbedtls_ssl_free(&(client_tls->tls_ssl));
					goto retry;
				}
			}
			goto errout;
		}
	}
#ifdef CONFIG_ENABLE_HTTP20
	if (param->http2->init) {
		keep_connect = true;
		param->http2->sock_fd = sockfd;
		if (param->tls) {
			param->http2->client_tls = client_tls;
			param->http2->ishandshaked = true;
		}
	}

send_message:
#endif
	buf_len = 0;
	while (sndlen > 0) {
		if (param->tls) {
			ret = mbedtls_ssl_write(&(client_tls->tls_ssl), (const unsigned char *)param->buffer + buf_len, sndlen);
		} else {
			ret = send(sockfd, param->buffer + buf_len, sndlen, 0);
		}
		if (ret < 1) {
			ndbg("ERROR: send failed: %d\n", ret);
			goto errout;
		} else {
			sndlen -= ret;
			buf_len += ret;
			ndbg("SEND SUCCESS: send %d bytes\n", ret);
		}
	}

#ifdef CONFIG_ENABLE_HTTP20
	/* if a server supports HTTP/2, it must return HTTP/1.1 response with 101 status.
	 * but nghttp2 server don't send a HTTP/1.1 response.
	 * so this codes may be removed when communicate with other HTTP/2 server.
	 */
	if (param->http2->init) {
		free(param->buffer);
		return (pthread_addr_t) WGET_OK;
	}
#endif

	if (param->callback && param->response == NULL) {
		param->response = &response;
		if (http_client_response_init(param->response) < 0) {
			ndbg("ERROR: response init failed: %d\n", ret);
			param->response = NULL;
			goto errout;
		}
	}

	buf_len = 0;
	while (!read_finish) {
		if (param->tls) {
			len = mbedtls_ssl_read(&(client_tls->tls_ssl), (unsigned char *)param->response->message + buf_len, WEBCLIENT_CONF_MAX_MESSAGE_SIZE - buf_len);
		} else {
			len = recv(sockfd, param->response->message + buf_len, WEBCLIENT_CONF_MAX_MESSAGE_SIZE - buf_len, 0);
		}

		if (len < 0) {
			ndbg("Error: Receive Fail\n");
			goto errout;
		} else if (len == 0) {
			ndbg("Finish read (got EOF)\n");
			break;
		}

		buf_len += len;
		remain -= len;
		usleep(1);
		read_finish = http_parse_message(param->response->message, len, NULL, param->response->url, &param->response->entity, &encoding, &state, &mlen, param->response->headers, NULL, param->response, NULL);

		param->response->entity_len = mlen.entity_len;

		if (remain <= 0) {

			if (param->callback) {
				callback_count++;
				param->callback(param->response);
			}

			memset(param->response->message, 0, WEBCLIENT_CONF_MAX_MESSAGE_SIZE);
			remain = WEBCLIENT_CONF_MAX_MESSAGE_SIZE;
			buf_len = 0;
			mlen.sentence_start = 0;
			mlen.entity_len = 0;
			param->response->entity_len = 0;
		}
	}

	param->response->method = param->method;
	param->response->url = param->url;

	if (param->callback) {
		if (mlen.entity_len > 0) {
			param->callback(param->response);
		}
		//If message is over buffer size of message, already sent all message.
		if (callback_count > 0) {
			memset(param->response->entity, 0, WEBCLIENT_CONF_MAX_MESSAGE_SIZE);
			param->response->entity_len = 0;
			param->callback(param->response);
		}
		http_client_response_release(param->response);
	}
	if (!keep_connect) {
		if (param->tls) {
			wget_tls_release(client_tls);
			wget_tls_ssl_release(client_tls);
		}
		if (client_tls) {
			free(client_tls);
		}
		if (!param->tls) {
			close(sockfd);
		}
	}
	free(param->buffer);
	param->async_flag = WGET_OK;
	return (pthread_addr_t) WGET_OK;

errout:
	if (param->callback && param->response) {
		http_client_response_release(param->response);
	}
#ifdef CONFIG_ENABLE_HTTP20
	if (!param->http2->init)
#endif
	{
		if (param->tls) {
			wget_tls_ssl_release(client_tls);
		}
	}
errout_before_tlsinit:
#ifdef CONFIG_ENABLE_HTTP20
	if (!param->http2->init)
#endif
	{
		if (param->tls) {
			wget_tls_release(client_tls);
		}
		if (client_tls) {
			free(client_tls);
		}
	}
	if (!param->tls && sockfd > 0) {
		close(sockfd);
	}
	free(param->buffer);
	param->async_flag = WGET_ERR;
#ifdef CONFIG_ENABLE_HTTP20
	if (param->http2->init) {
		param->http2->sock_fd = -1;
		if (param->tls) {
			param->http2->client_tls = NULL;
			param->http2->ishandshaked = false;
		}
	}
#endif
	return (pthread_addr_t) WGET_ERR;
}
#endif

#ifndef CONFIG_NET_SECURITY_TLS
pthread_addr_t wget_base_11(void *arg)
{
	int sockfd = -1;
	int ret;
	int buf_len, sndlen, len;
	int remain = WEBCLIENT_CONF_MAX_MESSAGE_SIZE;
	int encoding = CONTENT_LENGTH;
	int state = HTTP_REQUEST_HEADER;
	struct http_message_len_t mlen = { 0, };
	struct wget_s ws;
	struct http_client_request_t *param = (struct http_client_request_t *)arg;
	struct http_client_response_t response;
	bool read_finish = false;
	bool keep_connect = false;
	int callback_count = 0;

	/* Initialize the state structure */
	memset(&ws, 0, sizeof(struct wget_s));
	ws.buffer = param->buffer;
	ws.buflen = param->buflen;

	/* Parse the hostname (with optional port number) and filename from the URL */
	ret = netlib_parsehttpurl(param->url, &ws.port, ws.hostname, CONFIG_WEBCLIENT_MAXHOSTNAME, ws.filename, CONFIG_WEBCLIENT_MAXFILENAME);
	if (ret != 0) {
		ndbg("ERROR: Malformed HTTP URL: %s\n", param->url);
		free(param->buffer);
		return (pthread_addr_t) WGET_ERR;
	}

	nvdbg("hostname='%s' filename='%s'\n", ws.hostname, ws.filename);

	/* Re-initialize portions of the state structure that could have
	 * been left from the previous time through the loop and should not
	 * persist with the new connection.
	 */
	ws.httpstatus = HTTPSTATUS_NONE;
	ws.offset = 0;
	ws.datend = 0;
	ws.ndx = 0;

	sndlen = wget_msg_construct_11(ws.buffer, param, &ws);
	if (sndlen <= 0) {
		ndbg("ERROR: construction message failed\n");
		goto errout_before_init;
	}
#ifdef CONFIG_ENABLE_HTTP20
	if (param->http2->init && param->http2->sock_fd != -1) {
		goto send_message;
	}
#endif

	if ((sockfd = wget_socket_connect(&ws)) < 0) {
		ndbg("ERROR: socket failed: %d\n", errno);
		goto errout_before_init;
	}
#ifdef CONFIG_ENABLE_HTTP20
	if (param->http2->init) {
		keep_connect = true;
		param->http2->sock_fd = sockfd;
	}

send_message:
#endif
	buf_len = 0;
	while (sndlen > 0) {
		ret = send(sockfd, param->buffer + buf_len, sndlen, 0);
		if (ret < 1) {
			ndbg("ERROR: send failed: %d\n", ret);
			goto errout;
		} else {
			sndlen -= ret;
			buf_len += ret;
			ndbg("SEND SUCCESS: send %d bytes\n", ret);
		}
	}

	if (param->callback && param->response == NULL) {
		param->response = &response;
		if (http_client_response_init(param->response) < 0) {
			ndbg("ERROR: response init failed: %d\n", ret);
			param->response = NULL;
			goto errout;
		}
	}

	buf_len = 0;
	while (!read_finish) {
		len = recv(sockfd, param->response->message + buf_len, WEBCLIENT_CONF_MAX_MESSAGE_SIZE - buf_len, 0);

		if (len < 0) {
			ndbg("Error: Receive Fail\n");
			goto errout;
		} else if (len == 0) {
			ndbg("Finish read (got EOF)\n");
			break;
		}

		buf_len += len;
		remain -= len;
		usleep(1);
		read_finish = http_parse_message(param->response->message, len, NULL, param->response->url, &param->response->entity, &encoding, &state, &mlen, param->response->headers, NULL, param->response, NULL);

		param->response->entity_len = mlen.entity_len;

		if (remain <= 0) {

			if (param->callback) {
				callback_count++;
				param->callback(param->response);
			}

			memset(param->response->message, 0, WEBCLIENT_CONF_MAX_MESSAGE_SIZE);
			remain = WEBCLIENT_CONF_MAX_MESSAGE_SIZE;
			buf_len = 0;
			mlen.sentence_start = 0;
			mlen.entity_len = 0;
			param->response->entity_len = 0;
		}
	}

	param->response->method = param->method;
	param->response->url = param->url;

	if (param->callback) {
		if (mlen.entity_len > 0) {
			param->callback(param->response);
		}
		//If message is over buffer size of message, already sent all message.
		if (callback_count > 0) {
			memset(param->response->entity, 0, WEBCLIENT_CONF_MAX_MESSAGE_SIZE);
			param->response->entity_len = 0;
			param->callback(param->response);
		}
		http_client_response_release(param->response);
	}

	if (!keep_connect) {
		close(sockfd);
	}

	if (param->buffer) {
		free(param->buffer);
	}
	param->async_flag = WGET_OK;
	return (pthread_addr_t) WGET_OK;

errout:
	if (param->callback && param->response) {
		http_client_response_release(param->response);
	}
errout_before_init:
	if (sockfd > 0) {
		close(sockfd);
	}
	free(param->buffer);
	param->async_flag = WGET_ERR;
#ifdef CONFIG_ENABLE_HTTP20
	if (param->http2->init) {
		param->http2->sock_fd = -1;
	}
#endif
	return (pthread_addr_t) WGET_ERR;
}
#endif
