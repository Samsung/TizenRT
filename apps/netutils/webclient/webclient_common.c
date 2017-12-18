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
 * netutils/webclient/webclient_common.c
 * Implementation of the HTTP client.
 *
 *	 Copyright (C) 2007, 2009, 2011-2012, 2014 Gregory Nutt. All rights reserved.
 *	 Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Based on uIP which also has a BSD style license:
 *
 *	 Author: Adam Dunkels <adam@dunkels.com>
 *	 Copyright (c) 2002, Adam Dunkels.
 *	 All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *	  notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above
 *	  copyright notice, this list of conditions and the following
 *	  disclaimer in the documentation and/or other materials provided
 *	  with the distribution.
 * 3. The name of the author may not be used to endorse or promote
 *	  products derived from this software without specific prior
 *	  written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.	IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
****************************************************************************/

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
#	 define WGET_USE_URLENCODE 1
#	 include <apps/netutils/urldecode.h>
#endif
#if defined(CONFIG_CODECS_BASE64)
#	 include <apps/netutils/base64.h>
#endif
#else
#undef CONFIG_CODECS_URLCODE
#undef CONFIG_CODECS_BASE64
#endif

#include "webclient_common.h"

/****************************************************************************
 * Private Functions
 ****************************************************************************/

#ifdef CONFIG_NET_SECURITY_TLS
void wget_tls_release(struct http_client_tls_t *client);
#endif

/****************************************************************************
 * public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: wget_strcpy
 ****************************************************************************/

char *wget_strcpy(char *dest, const char *src, struct http_client_request_t *ws)
{
	int len = strlen(src);

	if (dest + len - ws->buffer >= ws->buflen) {
		printf("Error: buffer is too small\n");
		return NULL;
	}

	memcpy(dest, src, len);
	dest[len] = '\0';
	return dest + len;
}

char *wget_strlencpy(char *dest, const char *src, int len, struct http_client_request_t *ws)
{
	if (dest + len - ws->buffer >= ws->buflen) {
		printf("Error: buffer is too small\n");
		return NULL;
	}

	memcpy(dest, src, len);
	dest[len] = '\0';
	return dest + len;
}

char *wget_chunksize(char *dest, int len, struct http_client_request_t *ws)
{
	char size[10] = { 0, };

	if (!dest) {
		return NULL;
	}

	snprintf(size, sizeof(size), "%x", len);
	len = strlen(size);
	if (dest + len - ws->buffer >= ws->buflen) {
		printf("Error: buffer is too small\n");
		return NULL;
	}
	memcpy(dest, size, len);
	return dest + len;
}

/****************************************************************************
 * Name: wget_urlencode_strcpy
 ****************************************************************************/

#ifdef WGET_USE_URLENCODE
char *wget_urlencode_strcpy(char *dest, const char *src)
{
	int len = strlen(src);
	int d_len;

	d_len = urlencode_len(src, len);
	urlencode(src, len, dest, &d_len);
	return dest + d_len;
}
#endif

/****************************************************************************
 * Name: wget_parsestatus
 ****************************************************************************/

inline int wget_parsestatus(struct wget_s *ws)
{
	int offset;
	int ndx;
	char *dest;

	offset = ws->offset;
	ndx = ws->ndx;

	while (offset < ws->datend) {
		ws->line[ndx] = ws->buffer[offset];
		if (ws->line[ndx] == ISO_nl) {
			ws->line[ndx] = '\0';
			if ((strncmp(ws->line, g_http10, strlen(g_http10)) == 0)
				|| (strncmp(ws->line, g_http11, strlen(g_http11)) == 0)) {
				dest = &(ws->line[9]);
				ws->httpstatus = HTTPSTATUS_NONE;

				/* Check for 200 OK */

				if (strncmp(dest, g_http200, strlen(g_http200)) == 0) {
					ws->httpstatus = HTTPSTATUS_OK;
				}

				/* Check for 301 Moved permanently or 302 Found. Location: header line
				 * will contain the new location.
				 */

				else if (strncmp(dest, g_http301, strlen(g_http301)) == 0 || strncmp(dest, g_http302, strlen(g_http302)) == 0 || strncmp(dest, g_http404, strlen(g_http404)) == 0 || strncmp(dest, g_http500, strlen(g_http500)) == 0) {

					ws->state = WEBCLIENT_STATE_DATA;
					break;
				}
			} else {
				return -ECONNABORTED;
			}

			/* We're done parsing the status line, so start parsing the HTTP headers. */

			ws->state = WEBCLIENT_STATE_HEADERS;
			break;
		} else {
			offset++;
			ndx++;
		}
	}

	ws->offset = offset;
	ws->ndx = ndx;
	return OK;
}

/****************************************************************************
 * Name: wget_parseheaders
 ****************************************************************************/

inline int wget_parseheaders(struct wget_s *ws)
{
	int offset;
	int ndx;

	offset = ws->offset;
	ndx = ws->ndx;

	while (offset < ws->datend) {
		ws->line[ndx] = ws->buffer[offset];
		if (ws->line[ndx] == ISO_nl) {
			/* We have an entire HTTP header line in s.line, so
			 * we parse it.
			 */

			if (ndx > 0) {		/* Should always be true */
				if (ws->line[0] == ISO_cr) {
					/* This was the last header line (i.e., and empty "\r\n"), so
					 * we are done with the headers and proceed with the actual
					 * data.
					 */

					ws->state = WEBCLIENT_STATE_DATA;
					goto exit;
				}

				/* Truncate the trailing \r\n */

				ws->line[ndx - 1] = '\0';

				/* Check for specific HTTP header fields. */

#ifdef CONFIG_WEBCLIENT_GETMIMETYPE
				if (strncasecmp(ws->line, g_httpcontenttype, strlen(g_httpcontenttype)) == 0) {
					/* Found Content-type field. */

					char *dest = strchr(ws->line, ';');

					if (dest != NULL) {
						*dest = 0;
					}

					strncpy(ws->mimetype, ws->line + strlen(g_httpcontenttype), sizeof(ws->mimetype));
				} else
#endif
					if (strncasecmp(ws->line, g_httplocation, strlen(g_httplocation)) == 0) {
						/* Parse the new HTTP host and filename from the URL.  Note that
						 * the return value is ignored.  In the event of failure, we
						 * retain the current location.
						 */

						(void)netlib_parsehttpurl(ws->line + strlen(g_httplocation), &ws->port, ws->hostname, CONFIG_WEBCLIENT_MAXHOSTNAME, ws->filename, CONFIG_WEBCLIENT_MAXFILENAME);
						nvdbg("New hostname='%s' filename='%s'\n", ws->hostname, ws->filename);
					}
			}

			/* We're done parsing this line, so we reset the index to the start
			 * of the next line.
			 */

			ndx = 0;
		} else {
			ndx++;
		}

		offset++;
	}

exit:
	ws->offset = ++offset;
	ws->ndx = ndx;
	return OK;
}

/****************************************************************************
 * Name: wget_gethostip
 *
 * Description:
 *	 Call gethostbyname() to get the IPv4 address associated with a hostname.
 *
 * Input Parameters
 *	 hostname - The host name to use in the nslookup.
 *	 ipv4addr - The location to return the IPv4 address.
 *
 * Returned Value:
 *	 Zero (OK) on success; a negated errno value on failure.
 *
 ****************************************************************************/

int wget_gethostip(FAR char *hostname, in_addr_t *ipv4addr)
{
#ifdef CONFIG_NET_LWIP_NETDB
	FAR struct hostent *he;

	he = gethostbyname(hostname);
	if (he == NULL) {
		ndbg("gethostbyname failed\n");
		return -ENOENT;
	} else if (he->h_addrtype != AF_INET) {
		ndbg("gethostbyname returned an address of type: %d\n", he->h_addrtype);
		return -ENOEXEC;
	}

	memcpy(ipv4addr, he->h_addr, sizeof(in_addr_t));
	return OK;
#else
	ndbg("LIBC NETDB is not supported\n");
	return ERROR;
#endif
}

#ifdef CONFIG_NET_SECURITY_TLS
void wget_tls_debug(void *ctx, int level, const char *file, int line, const char *str)
{
	printf("%s:%04d: %s", file, line, str);
}

int webclient_tls_init(struct http_client_tls_t *client, struct http_client_ssl_config_t *ssl_config)
{
	int result = 0;

	if (!client || !ssl_config) {
		return -1;
	}

	/* If caller provides SSL configuration, just copy the config */
	if (ssl_config->tls_conf) {
		memcpy(&(client->tls_conf), ssl_config->tls_conf, sizeof(mbedtls_ssl_config));
		return 0;
	}

	mbedtls_ssl_config_init(&(client->tls_conf));
	mbedtls_x509_crt_init(&(client->tls_clicert));
	mbedtls_pk_init(&(client->tls_pkey));
	mbedtls_entropy_init(&(client->tls_entropy));
	mbedtls_ctr_drbg_init(&(client->tls_ctr_drbg));
	mbedtls_ssl_session_init(&(client->tls_session));

	/* 0. Initialize the RNG */
	ndbg("	. Seeding the random number generator...");

	result = mbedtls_ctr_drbg_seed(&(client->tls_ctr_drbg), mbedtls_entropy_func, &(client->tls_entropy), (const unsigned char *)tlsname, strlen(tlsname));
	if (result != 0) {
		ndbg("Error: mbedtls_ctr_drbg_seed returned -%4x\n", -result);
		goto TLS_INIT_EXIT;
	}

	ndbg("Ok\n");

	/* 1. Setup ssl stuff */
	ndbg("	. Setting up the SSL data...");

	result = mbedtls_ssl_config_defaults(&(client->tls_conf), MBEDTLS_SSL_IS_CLIENT, MBEDTLS_SSL_TRANSPORT_STREAM, MBEDTLS_SSL_PRESET_DEFAULT);
	if (result != 0) {
		ndbg("Error: mbedtls_ssl_config_defaults returned -%4x\n", -result);
		goto TLS_INIT_EXIT;
	}

	mbedtls_ssl_conf_rng(&(client->tls_conf), mbedtls_ctr_drbg_random, &(client->tls_ctr_drbg));
	mbedtls_ssl_conf_dbg(&(client->tls_conf), wget_tls_debug, stdout);

	ndbg("Ok\n");

	if (ssl_config->dev_cert && ssl_config->private_key) {
		/* 2. Load the certificates and private key */

		ndbg("	. Loading the client cert. and key...");

		result = mbedtls_x509_crt_parse(&(client->tls_clicert), (const unsigned char *)ssl_config->dev_cert, ssl_config->dev_cert_len);
		if (result != 0) {
			ndbg("Error: cli_cert parse fail, returned -%4x\n", -result);
			goto TLS_INIT_EXIT;
		}

		result = mbedtls_pk_parse_key(&(client->tls_pkey), (const unsigned char *)ssl_config->private_key, ssl_config->private_key_len, NULL, 0);
		if (result != 0) {
			ndbg("Error: cli_key parse fail, return -%4x\n", -result);
			goto TLS_INIT_EXIT;
		}

		result = mbedtls_ssl_conf_own_cert(&(client->tls_conf), &(client->tls_clicert), &(client->tls_pkey));
		if (result != 0) {
			ndbg("Error: mbedtls_ssl_conf_own_cert returned -%4x\n", -result);
			goto TLS_INIT_EXIT;
		}

		ndbg("Ok\n");
	}

	if (ssl_config->root_ca) {
		mbedtls_x509_crt *chain;

		/* 3. Load the CA certificate */
		ndbg("	. Loading the CA cert...");

		result = mbedtls_x509_crt_parse(&(client->tls_clicert), (const unsigned char *)ssl_config->root_ca, ssl_config->root_ca_len);
		if (result != 0) {
			ndbg("Error: CA_cert parse fail, return %d\n", result);
			goto TLS_INIT_EXIT;
		}

		/* CA cert may be first or second in chain depending if client cert was loaded */
		chain = client->tls_clicert.next ? client->tls_clicert.next : &client->tls_clicert;
		mbedtls_ssl_conf_ca_chain(&(client->tls_conf), chain, NULL);

		ndbg("Ok\n");
	}

	return 0;
TLS_INIT_EXIT:
	wget_tls_release(client);
	return result;
}

void wget_tls_release(struct http_client_tls_t *client)
{
	if (client == NULL) {
		return;
	}

	mbedtls_x509_crt_free(&(client->tls_clicert));
	mbedtls_pk_free(&(client->tls_pkey));
	mbedtls_ssl_config_free(&(client->tls_conf));
	mbedtls_ctr_drbg_free(&(client->tls_ctr_drbg));
	mbedtls_entropy_free(&(client->tls_entropy));
	mbedtls_ssl_session_free(&(client->tls_session));
}

void wget_tls_ssl_release(struct http_client_tls_t *client)
{
	if (client == NULL) {
		return;
	}

	mbedtls_net_free(&(client->tls_client_fd));
	mbedtls_ssl_session_reset(&(client->tls_ssl));
	mbedtls_ssl_free(&(client->tls_ssl));
}

int wget_tls_handshake(struct http_client_tls_t *client, const char *hostname)
{
	int result = 0;
#if defined(MBEDTLS_SSL_ALPN) && defined(CONFIG_ENABLE_HTTP20)
	const char *alpn_list[WEBCLIENT_CONF_ALPN_LIST_NUM + 1];
	char *p = WEBCLIENT_CONF_ALPN_LIST;
	int i;
#endif

	mbedtls_ssl_conf_authmode(&(client->tls_conf), WEBCLIENT_CONF_SSL_VERIFY_LEVEL);
#if defined(MBEDTLS_SSL_ALPN) && defined(CONFIG_ENABLE_HTTP20)

	memset(alpn_list, 0, sizeof(const char *) * (WEBCLIENT_CONF_ALPN_LIST_NUM + 1));

	if (p != NULL) {
		i = 0;

		/* Leave room for a final NULL in alpn_list */
		while ((i < WEBCLIENT_CONF_ALPN_LIST_NUM) && (*p != '\0')) {
			alpn_list[i++] = p;

			/* Terminate the current string and move on to next one */
			while (*p != '\0') {
				p++;
			}
			p++;
		}
	}

	if (alpn_list[0] != NULL) {
		result = mbedtls_ssl_conf_alpn_protocols(&(client->tls_conf), alpn_list);
		if (result != 0) {
			ndbg(" failed\n  ! mbedtls_ssl_conf_alpn_protocols returned %d\n\n", result);
		}
	}
#endif

	mbedtls_net_init(&(client->tls_client_fd));
	mbedtls_ssl_init(&(client->tls_ssl));

	client->tls_client_fd.fd = client->client_fd;

	if (mbedtls_net_set_block(&(client->tls_client_fd)) < 0) {
		ndbg("Error: mbedtls_net_set_block fail\n");
		goto HANDSHAKE_FAIL;
	}

	ndbg("TLS Init Success\n");

	result = mbedtls_ssl_setup(&(client->tls_ssl), &(client->tls_conf));
	if (result != 0) {
		ndbg("Error: mbedtls_ssl_setup returned -%4x\n", -result);
		goto HANDSHAKE_FAIL;
	}

	/*
	 * Server name intication is an extension to the TLS networking protocol
	 * If server presents multiple certificates on the same IP address,
	 * client could make multiple secure session depends on hostname.
	 *
	 * Note : Hostname in TLS is a subject's common name(CN) of certificates.
	 */
#if WEBCLIENT_CONF_CHECK_TLS_HOSTNAME
	result = mbedtls_ssl_set_hostname(&(client->tls_ssl), hostname);
	if (result != 0) {
		ndbg("Error: mbedtls_hostname returned -%4x\n", -result);
		goto HANDSHAKE_FAIL;
	}
#endif

	mbedtls_ssl_set_bio(&(client->tls_ssl), &(client->tls_client_fd), mbedtls_net_send, mbedtls_net_recv, NULL);

	/* Handshake */
	while ((result = mbedtls_ssl_handshake(&(client->tls_ssl))) != 0) {
		if (result != MBEDTLS_ERR_SSL_WANT_READ && result != MBEDTLS_ERR_SSL_WANT_WRITE) {
			ndbg("Error: TLS Handshake fail returned -%4x\n", -result);
			goto HANDSHAKE_FAIL;
		}
	}

	ndbg("TLS Handshake Success\n");

	return 0;
HANDSHAKE_FAIL:
	return result;
}
#endif

int wget_socket_connect(struct wget_s *ws)
{
	int sockfd, ret;
	struct timeval tv;
	struct sockaddr_in server;

	/* Create a socket */
	sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sockfd < 0) {
		return WGET_SOCKET_CONNECT_ERR;
	}

	/* Set send and receive timeout values */
	tv.tv_sec = WEBCLIENT_CONF_TIMEOUT_MSEC / 1000;
	tv.tv_usec = (WEBCLIENT_CONF_TIMEOUT_MSEC % 1000) * 1000;
	if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (struct timeval *)&tv, sizeof(struct timeval)) < 0) {
		ndbg("ERROR: setsockopt failed\n");
	}

	/* Get the server address from the host name */
	server.sin_family = AF_INET;
	server.sin_port = htons(ws->port);
	ret = wget_gethostip(ws->hostname, &server.sin_addr.s_addr);
	if (ret < 0) {
		/* Could not resolve host (or malformed IP address) */
		ndbg("ERROR: Failed to resolve hostname\n");
		close(sockfd);
		return WGET_SOCKET_CONNECT_ERR;
	}

	/* Connect to server.  First we have to set some fields in the
	 * 'server' address structure.  The system will assign me an arbitrary
	 * local port that is not in use.
	 */
	ret = connect(sockfd, (struct sockaddr *)&server, sizeof(struct sockaddr_in));
	if (ret < 0) {
		ndbg("ERROR: connect failed: %d errno: %d\n", ret, errno);
		close(sockfd);
		return WGET_SOCKET_CONNECT_ERR;
	}
	return sockfd;
}
