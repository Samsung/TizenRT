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

#include <protocols/webserver/http_err.h>

#include "http.h"
#include "http_client.h"
#include "http_log.h"

int http_client_tls_init(struct http_client_t *client)
{
	int result = 0;

	HTTP_LOGD("Client TLS Init start..");

	mbedtls_net_init(&(client->tls_client_fd));
	mbedtls_ssl_init(&(client->tls_ssl));

	client->tls_client_fd.fd = client->client_fd;
	HTTP_LOGD("Ok\n");

	if ((result = mbedtls_ssl_setup(&(client->tls_ssl), &(client->server->tls_conf))) != 0) {
		HTTP_LOGE("Error: mbedtls_ssl_setup returned -%4x\n", -result);
		return HTTP_ERROR;
	}

	mbedtls_ssl_set_bio(&(client->tls_ssl), &(client->tls_client_fd),
						mbedtls_net_send, mbedtls_net_recv, NULL);

	/* Handshake */
	HTTP_LOGD("  . Performing the SSL/TLS handshake...");

	while ((result = mbedtls_ssl_handshake(&(client->tls_ssl))) != 0) {
		if (result != MBEDTLS_ERR_SSL_WANT_READ && result != MBEDTLS_ERR_SSL_WANT_WRITE) {
			HTTP_LOGE("Error: mbedtls_ssl_handshake returned -%4x\n", -result);
			return HTTP_ERROR;
		}
	}

	HTTP_LOGD("Ok\n");

	return HTTP_OK;
}

int http_client_tls_release(struct http_client_t *client)
{
	HTTP_LOGD("Client TLS Release...\n");

	mbedtls_net_free(&(client->tls_client_fd));
	mbedtls_ssl_free(&(client->tls_ssl));

	HTTP_LOGD("Release success Ok\n");

	return HTTP_OK;
}
