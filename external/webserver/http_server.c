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

#include <stdlib.h>
#include <protocols/webserver/http_err.h>
#include <protocols/webserver/http_server.h>

#include "http_client.h"
#include "http_arch.h"
#include "http_log.h"

struct http_server_t *http_server_init(int port)
{
	struct http_server_t *p = (struct http_server_t *)HTTP_MALLOC(sizeof(struct http_server_t));
	if (p == NULL) {
		HTTP_LOGE("Error: Failed to malloc server\n");
		return NULL;
	}

	if (port < 0 || port > 65535) {
		HTTP_LOGE("Error: Port num is not valid\n");
		HTTP_FREE(p);
		return NULL;
	}

	HTTP_MEMSET(p, 0, sizeof(struct http_server_t));

	/* Initialize default values */
	p->port = port;
	p->listen_fd = -1;
	p->tls_init = 0;
	p->state = HTTP_SERVER_INIT;

	/* Init server query handler */
	HTTP_MEMSET(p->query_handlers, 0, sizeof(struct http_query_handler_t *) * HTTP_CONF_MAX_QUERY_HANDLER_COUNT);

	return p;
}

void http_server_release(struct http_server_t **server)
{
	if (*server) {
#ifdef CONFIG_NET_SECURITY_TLS
#ifdef CONFIG_NETUTILS_WEBSOCKET
		while (websocket_count_table()) {
			usleep(100000);
		}
#endif

		if ((*server)->tls_init) {
			http_server_tls_release(*server);
		}
#endif
		HTTP_FREE(*server);
		*server = NULL;
	}
}

void http_close_client(struct http_client_t *client)
{
	close(client->client_fd);
	http_client_release(client);
}

int http_server_stop(struct http_server_t *server)
{
	if (server == NULL) {
		HTTP_LOGE("Error: Server must be started before stop\n");
		return HTTP_ERROR;
	}

	server->state = HTTP_SERVER_STOP_REQ;

	while (server->state != HTTP_SERVER_STOP) {
		usleep(100000);
	}

	close(server->listen_fd);

	return HTTP_OK;
}
