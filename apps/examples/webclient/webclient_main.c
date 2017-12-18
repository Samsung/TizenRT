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
/****************************************************************************
 * examples/webclient/webclient_main.c
 *
 *   Copyright (C) 2009, 2011, 2015 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name Gregory Nutt nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/**
* @testcase            lwip_http_01 (client)
* @brief               To send HTTP request messages and receive response messages.
*                      supported methods: GET, POST, PUT, DELETE
*                      Entity and encoding are needed only for POST and PUT methods. If an encoding is not for "C", it always uses the content-length.
* @scenario            1. Start webserver at TASH using the command "webserver start". Refer to webserver_main.c to run HTTP server.
*                      2. Start webclient at TASH using the command "webclient GET http://[serverip]/".
* @apicovered
* @precondition                Connect to Wi-Fi. Both ARTIK05x server and ARTIK05x client should be in the same network.
* @postcondition
*/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <apps/netutils/webclient.h>

#include "webclient.h"

/****************************************************************************
 * Name: callback
 ****************************************************************************/
static void callback(struct http_client_response_t *response)
{
	printf("----------async response----------\n");
	printf("status %d %s\n", response->status, response->phrase);
	printf("%s\n", response->entity);
	printf("---------------------------------\n");
}

/****************************************************************************
 * Private Functions
 ****************************************************************************/
static void dump_webclient_usage(void)
{
	printf("\n  webclient usage:\n");
	printf("   $ webclient <method> <uri> [options...]\n");
	printf("\n");
	printf(" <method>   : %%s (GET, PUT, POST, DELETE)\n");
	printf(" <uri>      : %%s (Host address : should be started with http:// or https://)\n");
	printf("\n [options...]\n");
	printf(" async=1               Enable asynchronous mode (default is synchronous)\n");
	printf(" chunked=1             Enable chunked encoding (default is disabled)\n");
	printf(" entity=DATA           Set entity data (default is NULL)\n");
	printf(" test_entity=SIZE      Test entity size (default is 0)\n");
	printf("\n  example:\n");
	printf("   $ webclient GET https://127.0.0.1/\n");
	printf("   $ webclient GET https://127.0.0.1/ async=1 entity=data\n");
}

static int webclient_init_request(int argc, char *argv[], struct http_client_request_t *request)
{
	int i;
	char *p, *q;

	g_async = 0;
	g_testentity = 0;
	memset(request, 0, sizeof(struct http_client_request_t));

	if (argc < 3) {
		return -1;
	}

	if (!strcmp(argv[1], "GET")) {
		request->method = WGET_MODE_GET;
	} else if (!strcmp(argv[1], "POST")) {
		request->method = WGET_MODE_POST;
	} else if (!strcmp(argv[1], "PUT")) {
		request->method = WGET_MODE_PUT;
	} else if (!strcmp(argv[1], "DELETE")) {
		request->method = WGET_MODE_DELETE;
	} else {
		dump_webclient_usage();
		return -1;
	}

	/* argument2 is url. */
	request->url = argv[2];
#ifdef CONFIG_NET_SECURITY_TLS
	if (!strncmp(request->url, "https", 5)) {
		g_https = 1;
	} else
#endif
		if (!strncmp(request->url, "http", 4)) {
			g_https = 0;
		} else {
			return -1;
		}

	for (i = 3; i < argc; i++) {
		p = argv[i];
		if ((q = strchr(p, '=')) == NULL) {
			return -1;
		}
		*q++ = '\0';

		if (strcmp(p, "async") == 0) {
			g_async = atoi(q);
		} else if (strcmp(p, "chunked") == 0) {
			request->encoding = atoi(q);
		} else if (strcmp(p, "entity") == 0) {
			request->entity = q;
		} else if (strcmp(p, "test_entity") == 0) {
			int t = atoi(q);

			if (t > 0 && t <= WEBCLIENT_CONF_MAX_ENTITY_SIZE) {
				request->entity = (char *)malloc(t + 1);
				if (request->entity == NULL) {
					return -1;
				}
				g_testentity = 1;
				memset(request->entity, '1', t);
				(request->entity)[t] = '\0';
			} else {
				printf("entity is too big\n");
				return -1;
			}
		} else {
			return -1;
		}
	}

	request->buflen = WEBCLIENT_BUF_SIZE;
	return 0;
}

/****************************************************************************
 * Name: webclient_main
 ****************************************************************************/
#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int webclient_main(int argc, char *argv[])
#endif
{
	struct http_client_request_t request;
	struct http_keyvalue_list_t headers;
	struct http_client_response_t response;
	struct http_client_ssl_config_t *ssl_config = NULL;

	if (g_running) {
		printf("Previous request is in process, Please wait.\n");
		return -1;
	}
	g_running = 1;

	if (webclient_init_request(argc, argv, &request)) {
		dump_webclient_usage();
		if (g_testentity && request.entity) {
			free(request.entity);
		}
		g_running = 0;
		return -1;
	}

	ssl_config = g_https ? &g_config : NULL;

	/* before sending request,
	 * must initialize keyvalue list for request headers
	 */
	http_keyvalue_list_init(&headers);
	http_keyvalue_list_add(&headers, headerfield_connect, headerfield_close);
	http_keyvalue_list_add(&headers, headerfield_useragent, headerfield_tinyara);
	request.headers = &headers;

	/* before sending request by sync function,
	 * must initialize response structure
	 */
	if (g_async) {
		if (http_client_send_request_async(&request, ssl_config, (wget_callback_t) callback)) {
			printf("fail to send request\n");
			goto release_out;
		}
		/* sleep for end request */
		while (request.async_flag > 0) {
			usleep(100000);
		}
		if (request.async_flag < 0) {
			printf("fail to send request\n");
		}
	} else {
		if (http_client_response_init(&response) < 0) {
			printf("fail to response init\n");
			goto release_out;
		}
		if (http_client_send_request(&request, ssl_config, &response)) {
			printf("fail to send request\n");
			goto release_out;
		}
		printf("----------sync response----------\n");
		printf("status %d %s\n", response.status, response.phrase);
		int i;

		for (i = 0; i < response.entity_len; i++) {
			printf("%c", (response.entity)[i]);
		}
		printf("\n---------------------------------\n");
	}

release_out:
	if (g_testentity) {
		free(request.entity);
		g_testentity = 0;
	}
	/* after sending request by sync function,
	 * must release response structure
	 */
	if (g_async == 0) {
		http_client_response_release(&response);
	}
	/* before finish of app,
	 * must release keyvalue list for request headers
	 */
	http_keyvalue_list_release(&headers);
	printf("end request\n");

	g_running = 0;
	return 0;
}
