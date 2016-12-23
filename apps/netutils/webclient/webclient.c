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

#include <netdb.h>
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

#if defined(CONFIG_HW_RSA_SIGN)
#include "tls/see_api.h"
#include "tls/pk.h"
#include "tls/pk_internal.h"
#endif

#define CONFIG_WEBCLIENT_MAXHTTPLINE 200
#define CONFIG_WEBCLIENT_MAXMIMESIZE 32
#define CONFIG_WEBCLIENT_MAXHOSTNAME 40
#define CONFIG_WEBCLIENT_MAXFILENAME 100

#ifndef CONFIG_WGET_USERAGENT
#define CONFIG_WGET_USERAGENT "TinyARA"
#endif

#ifdef CONFIG_HW_RSA_SIGN
#define HTTPS_SERVERNAME "ARTIK051_dev"
#else
#define HTTPS_SERVERNAME "localhost"
#endif

/****************************************************************************
 * Definitions
 ****************************************************************************/

#ifdef CONFIG_TLS_WITH_SSS
#define CLIENT_REQ_THREAD_STACK    24576
#else
#define CLIENT_REQ_THREAD_STACK    8192
#endif

#define WGET_OK                    0
#define WGET_ERR                   -1

#define WEBCLIENT_STATE_STATUSLINE 0
#define WEBCLIENT_STATE_HEADERS    1
#define WEBCLIENT_STATE_DATA       2
#define WEBCLIENT_STATE_CLOSE      3

#define HTTPSTATUS_NONE            0
#define HTTPSTATUS_OK              1
#define HTTPSTATUS_MOVED           2
#define HTTPSTATUS_ERROR           3

#define ISO_nl                     0x0a
#define ISO_cr                     0x0d
#define ISO_space                  0x20

#define MBED_DEBUG_LEVEL 3

/****************************************************************************
 * Private Types
 ****************************************************************************/

struct wget_s {
	/* Internal status */

	uint8_t state;
	uint8_t httpstatus;

	uint16_t port;				/* The port number to use in the connection */

	/* These describe the just-received buffer of data */

	FAR char *buffer;			/* user-provided buffer */
	int buflen;					/* Length of the user provided buffer */
	int offset;					/* Offset to the beginning of interesting data */
	int datend;					/* Offset+1 to the last valid byte of data in the buffer */

	/* Buffer HTTP header data and parse line at a time */

	char line[CONFIG_WEBCLIENT_MAXHTTPLINE];
	int ndx;

#ifdef CONFIG_WEBCLIENT_GETMIMETYPE
	char mimetype[CONFIG_WEBCLIENT_MAXMIMESIZE];
#endif
	char hostname[CONFIG_WEBCLIENT_MAXHOSTNAME];
	char filename[CONFIG_WEBCLIENT_MAXFILENAME];
};

struct wget_param {
	FAR char *url;
	FAR char *buffer;
	int buflen;
	wget_callback_t callback;
	FAR void *arg;
	FAR char *entity;
	int method;
	int encoding;
	int tls;
	struct http_keyvalue_list_t *headers;
	struct http_client_response_t *response;
#ifdef CONFIG_NET_SECURITY_TLS
	struct http_client_ssl_config_t ssl_config;
#endif
};

/****************************************************************************
 * Private Data
 ****************************************************************************/

static const char g_http10[] = "HTTP/1.0";
static const char g_http11[] = "HTTP/1.1";
#ifdef CONFIG_WEBCLIENT_GETMIMETYPE
static const char g_httpcontenttype[] = "content-type: ";
#endif
static const char g_httphost[] = "host: ";
static const char g_httplocation[] = "location: ";
static const char g_httpget[] = "GET ";
static const char g_httppost[] = "POST ";
static const char g_httpput[] = "PUT ";
static const char g_httpdelete[] = "DELETE ";

static const char g_httpuseragentfields[] = "Connection: close\r\n" "User-Agent: " CONFIG_WGET_USERAGENT "\r\n\r\n";

static const char g_http200[] = "200 ";
static const char g_http301[] = "301 ";
static const char g_http302[] = "302 ";
static const char g_http404[] = "404 ";
static const char g_http500[] = "500 ";

static const char g_httpcrnl[] = "\r\n";

static const char g_httpform[] = "Content-Type: application/x-www-form-urlencoded";
static const char g_httpcontsize[] = "Content-Length: ";
static const char g_httpchunked[] = "Transfer-Encoding: chunked";
const char *tlsname = "araweb_tls_client";

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static void wget_tls_release(struct http_client_tls_t *client);

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: wget_strcpy
 ****************************************************************************/

static char *wget_strcpy(char *dest, const char *src, struct wget_param *ws)
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

static char *wget_strlencpy(char *dest, const char *src, int len, struct wget_param *ws)
{
	if (dest + len - ws->buffer >= ws->buflen) {
		printf("Error: buffer is too small\n");
		return NULL;
	}

	memcpy(dest, src, len);
	dest[len] = '\0';
	return dest + len;
}

static char *wget_chunksize(char *dest, int len, struct wget_param *ws)
{
	char size[10] = { 0, };

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
static char *wget_urlencode_strcpy(char *dest, const char *src)
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

static inline int wget_parsestatus(struct wget_s *ws)
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
 * Name: wget_parsestatus
 ****************************************************************************/

static inline int wget_parseheaders(struct wget_s *ws)
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
 *   Call gethostbyname() to get the IPv4 address associated with a hostname.
 *
 * Input Parameters
 *   hostname - The host name to use in the nslookup.
 *   ipv4addr - The location to return the IPv4 address.
 *
 * Returned Value:
 *   Zero (OK) on success; a negated errno value on failure.
 *
 ****************************************************************************/

static int wget_gethostip(FAR char *hostname, in_addr_t * ipv4addr)
{
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
}

#ifdef CONFIG_NET_SECURITY_TLS
static void wget_tls_debug(void *ctx, int level, const char *file, int line, const char *str)
{
	ndbg("%s:%04d: %s", file, line, str);
}

int webclient_tls_init(struct http_client_tls_t *client, struct http_client_ssl_config_t *ssl_config)
{
	int result = 0;

	mbedtls_ssl_config_init(&(client->tls_conf));
	mbedtls_x509_crt_init(&(client->tls_clicert));
	mbedtls_pk_init(&(client->tls_pkey));
	mbedtls_entropy_init(&(client->tls_entropy));
	mbedtls_ctr_drbg_init(&(client->tls_ctr_drbg));
	mbedtls_ssl_session_init(&(client->tls_session));
#ifdef MBEDTLS_DEBUG_C
	mbedtls_debug_set_threshold(MBED_DEBUG_LEVEL);
#endif

	/* 0. Initialize the RNG */

	ndbg("  . Seeding the random number generator...");

	if ((result = mbedtls_ctr_drbg_seed(&(client->tls_ctr_drbg), mbedtls_entropy_func, &(client->tls_entropy), (const unsigned char *)tlsname, strlen(tlsname))) != 0) {
		ndbg("Error: mbedtls_ctr_drbg_seed returned %d\n", result);
		goto TLS_INIT_EXIT;
	}

	ndbg("Ok\n");

#ifdef CONFIG_HW_RSA_SIGN
	/* 1. Load the certificates and private key */
	ndbg("  . [SSS] Loading the cert. and key...");

	if (ssl_config == NULL) {
		result = -1;
		ndbg("Error: ssl_config is Null %d\n", result);
		goto TLS_INIT_EXIT;
	}
#if 1
	unsigned char rsa_public[292] = { 0x30, 0x82, 0x01, 0x20, 0x30, 0x0d, 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x01,
		0x01, 0x05, 0x00, 0x03, 0x82, 0x01, 0x0d, 0x00, 0x30, 0x82, 0x01, 0x08, 0x02, 0x82, 0x01, 0x01,
		0x00, 0xa2, 0x64, 0x21, 0xcf, 0x1c, 0xdb, 0x49, 0x6c, 0x44, 0x01, 0xf8, 0xd5, 0x8b, 0x8d, 0x20,
		0xfe, 0x2a, 0x46, 0x4d, 0x29, 0xf4, 0x82, 0x3c, 0xa4, 0x29, 0x7d, 0x6b, 0xdc, 0xc4, 0x04, 0xd6,
		0x0f, 0xf3, 0x6b, 0xa8, 0xb1, 0xad, 0x2b, 0xa1, 0xa5, 0xad, 0xfb, 0x9a, 0xba, 0x72, 0x6e, 0x4e,
		0x71, 0x93, 0x54, 0x8d, 0x90, 0x02, 0x34, 0x80, 0x1d, 0x8c, 0x83, 0xc9, 0x84, 0xa3, 0xcf, 0x9f,
		0x80, 0xe9, 0x4f, 0x5b, 0xf6, 0x29, 0x17, 0xf6, 0x7f, 0x5a, 0x79, 0x47, 0x0c, 0x2c, 0xcf, 0x98,
		0x88, 0x6a, 0x31, 0x4e, 0x0a, 0x2c, 0x8e, 0x8c, 0xe5, 0xa5, 0x9f, 0xd7, 0x8f, 0xd0, 0xc1, 0x04,
		0x1a, 0xe9, 0x54, 0xa1, 0x36, 0x4e, 0x92, 0x5e, 0x41, 0x9c, 0x07, 0xc8, 0x48, 0xac, 0x9c, 0x7c,
		0xcb, 0xa0, 0x8a, 0x51, 0x52, 0x4f, 0x47, 0xa2, 0xc8, 0x48, 0xbc, 0xcd, 0x55, 0x85, 0x24, 0xff,
		0xfa, 0x58, 0xe6, 0x75, 0x61, 0x14, 0x1a, 0x82, 0x4e, 0x6b, 0x40, 0x63, 0x9e, 0xef, 0xbd, 0x70,
		0x88, 0x9e, 0xc8, 0x59, 0x89, 0x16, 0x0c, 0x4e, 0x71, 0xec, 0x2d, 0xa4, 0x0b, 0xb3, 0x20, 0xca,
		0x04, 0x5b, 0x37, 0xf6, 0x5c, 0x80, 0x8d, 0x6a, 0xe4, 0x26, 0x95, 0xe4, 0xd5, 0x35, 0xcd, 0xd3,
		0x90, 0x67, 0x48, 0xef, 0x14, 0x8e, 0xc6, 0xcc, 0x16, 0xdb, 0x7a, 0x96, 0xd6, 0xbf, 0x01, 0xef,
		0x5f, 0x8d, 0xee, 0x35, 0xd1, 0x66, 0xa3, 0x26, 0x96, 0x5e, 0x73, 0x3b, 0x1e, 0xf6, 0x72, 0xc9,
		0x78, 0xc8, 0xdd, 0x81, 0x21, 0x0f, 0x0d, 0xdc, 0x3f, 0x63, 0x7a, 0x92, 0xf1, 0x31, 0x53, 0xe6,
		0x34, 0xd7, 0x70, 0xb0, 0x1d, 0x2f, 0x97, 0xab, 0x44, 0xf1, 0x70, 0x58, 0x0e, 0xca, 0xab, 0x26,
		0x23, 0x39, 0x6e, 0xdb, 0xf5, 0x5a, 0x15, 0x4a, 0x09, 0x00, 0x7c, 0xe5, 0x82, 0x78, 0xb8, 0xf0,
		0xd1, 0x02, 0x01, 0x03
	};

	if ((result = mbedtls_pk_parse_public_key(&client->tls_pkey, rsa_public, 292)) != 0) {
		ndbg("Error: parse public_key fail, return %d\n", result);
		goto TLS_INIT_EXIT;
	}
#else
	unsigned char *pubkey_buf = NULL;
	unsigned int pubkey_len = 292;

	pubkey_buf = malloc(pubkey_len);

	if (pubkey_buf == NULL) {
		goto TLS_INIT_EXIT;
	}

	if ((result = see_get_publickey(pubkey_buf, &pubkey_len)) != 0) {
		ndbg("Error: get_publickey fail, return %d\n", result);
		free(pubkey_buf);
		goto TLS_INIT_EXIT;
	}
	if ((result = mbedtls_pk_parse_public_key(&client->tls_pkey, pubkey_buf, pubkey_len)) != 0) {
		ndbg("Error: parse public_key fail, return %d\n", result);
		free(pubkey_buf);
		goto TLS_INIT_EXIT;
	}
#endif
	unsigned char *cert_buf = NULL;
	unsigned int cert_len = 1500;

	cert_buf = malloc(cert_len);

	if (cert_buf == NULL) {
		goto TLS_INIT_EXIT;
	}

	result = see_get_certificate(cert_buf, &cert_len, ssl_config->dev_cert_index, CERT_PEM);
	if (result) {
		ndbg("Error: dev_cert get fail, return %d\n", result);
		free(cert_buf);
		goto TLS_INIT_EXIT;
	}

	result = mbedtls_x509_crt_parse(&(client->tls_clicert), (const unsigned char *)cert_buf, cert_len);
	if (result) {
		ndbg("Error: dev_cert parse fail, return %d\n", result);
		free(cert_buf);
		goto TLS_INIT_EXIT;
	}

	cert_len = 1500;

	result = see_get_certificate(cert_buf, &cert_len, ssl_config->ca_cert_index, CERT_PEM);
	if (result) {
		ndbg("Error: ca_cert get fail, return %d\n", result);
		free(cert_buf);
		goto TLS_INIT_EXIT;
	}

	result = mbedtls_x509_crt_parse(&(client->tls_clicert), (const unsigned char *)cert_buf, cert_len);

	if (result) {
		ndbg("Error: ca_cert parse fail, return %d\n", result);
		free(cert_buf);
		goto TLS_INIT_EXIT;
	}

	free(cert_buf);
	if (client->tls_pkey.pk_info->type == MBEDTLS_PK_RSA) {
		((mbedtls_rsa_context *)(client->tls_pkey.pk_ctx))->key_index = ssl_config->dev_key_index;
	}
	ndbg("Ok\n");
#else

	/* 1. Load the certificates and private key */

	ndbg("  . Loading the client cert. and key...");

	result = mbedtls_x509_crt_parse(&(client->tls_clicert), (const unsigned char *)ssl_config->dev_cert, ssl_config->dev_cert_len);
	if (result) {
		ndbg("Error: cli_cert parse fail, return %d\n", result);
		goto TLS_INIT_EXIT;
	}

	result = mbedtls_pk_parse_key(&(client->tls_pkey), (const unsigned char *)ssl_config->private_key, ssl_config->private_key_len, NULL, 0);
	if (result) {
		ndbg("Error: cli_key parse fail, return %d\n", result);
		goto TLS_INIT_EXIT;
	}

	ndbg("Ok\n");

	/* 2. Load the CA certificate */

	ndbg("  . Loading the CA cert...");

	result = mbedtls_x509_crt_parse(&(client->tls_clicert), (const unsigned char *)ssl_config->root_ca, ssl_config->root_ca_len);
	if (result) {
		ndbg("Error: CA_cert parse fail, return %d\n", result);
		goto TLS_INIT_EXIT;
	}

	ndbg("Ok\n");

#endif							/* CONFIG_HW_RSA_SIGN */

	/* 3. Setup ssl stuff */

	ndbg("  . Setting up the SSL data...");

	if ((result = mbedtls_ssl_config_defaults(&(client->tls_conf), MBEDTLS_SSL_IS_CLIENT, MBEDTLS_SSL_TRANSPORT_STREAM, MBEDTLS_SSL_PRESET_DEFAULT)) != 0) {
		ndbg("Error: mbedtls_ssl_config_defaults returned %d\n", result);
		goto TLS_INIT_EXIT;
	}

	mbedtls_ssl_conf_rng(&(client->tls_conf), mbedtls_ctr_drbg_random, &(client->tls_ctr_drbg));
	mbedtls_ssl_conf_dbg(&(client->tls_conf), wget_tls_debug, stdout);
	mbedtls_ssl_conf_ca_chain(&(client->tls_conf), client->tls_clicert.next, NULL);

	if ((result = mbedtls_ssl_conf_own_cert(&(client->tls_conf), &(client->tls_clicert), &(client->tls_pkey))) != 0) {
		ndbg("Error: mbedtls_ssl_conf_own_cert returned %d\n", result);
		goto TLS_INIT_EXIT;
	}
	ndbg("Ok\n");

	return 0;

 TLS_INIT_EXIT:
	wget_tls_release(client);
	return result;
}

static void wget_tls_release(struct http_client_tls_t *client)
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

int wget_tls_handshake(struct http_client_tls_t *client)
{
	int result = 0;

	ndbg("Client TLS Init start..");

	mbedtls_ssl_conf_authmode(&(client->tls_conf), MBEDTLS_SSL_VERIFY_REQUIRED);

	mbedtls_net_init(&(client->tls_client_fd));
	mbedtls_ssl_init(&(client->tls_ssl));

	client->tls_client_fd.fd = client->client_fd;

	if (mbedtls_net_set_block(&(client->tls_client_fd)) < 0) {
		ndbg("Error: mbedtls_net_set_block fail\n");
		goto HANDSHAKE_FAIL;
	}

	ndbg("Ok\n");

	if ((result = mbedtls_ssl_setup(&(client->tls_ssl), &(client->tls_conf))) != 0) {
		ndbg("Error: mbedtls_ssl_setup returned %d\n", result);
		goto HANDSHAKE_FAIL;
	}

	if ((result = mbedtls_ssl_set_hostname(&(client->tls_ssl), HTTPS_SERVERNAME)) != 0) {
		ndbg("Error: mbedtls_hostname fail %d\n", result);
		goto HANDSHAKE_FAIL;
	}
	mbedtls_ssl_set_bio(&(client->tls_ssl), &(client->tls_client_fd), mbedtls_net_send, mbedtls_net_recv, NULL);

	/* Handshake */
	ndbg("  . Performing the SSL/TLS handshake...");

	while ((result = mbedtls_ssl_handshake(&(client->tls_ssl))) != 0) {
		if (result != MBEDTLS_ERR_SSL_WANT_READ && result != MBEDTLS_ERR_SSL_WANT_WRITE) {
			if (result == MBEDTLS_ERR_SSL_CONN_EOF) {
				ndbg("Error: webserver MQ is FULL %d\n", result);
			} else if ((result == MBEDTLS_ERR_NET_RECV_FAILED) || (result == MBEDTLS_ERR_NET_SEND_FAILED)) {
				ndbg("Error: mbedtls socket send/recv fail %d\n", result);
			} else {
				ndbg("Error: mbedtls_ssl_handshake returned %d\n", result);
			}
			goto HANDSHAKE_FAIL;
		}
	}

	ndbg("Ok\n");

	return 0;

 HANDSHAKE_FAIL:
	wget_tls_release(client);
	wget_tls_ssl_release(client);
	return result;
}
#endif

/****************************************************************************
 * Name: wget_base
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

static pthread_addr_t wget_base(void *arg)
{
	int sockfd;
	int len;
	int post_len;
	int ret;
	int buf_len, sndlen;
	int remain = WEBCLIENT_CONF_MAX_MESSAGE_SIZE;
	int encoding = CONTENT_LENGTH;
	int state = HTTP_REQUEST_HEADER;
	struct http_message_len_t mlen = { 0, };
	struct sockaddr_in server;
	struct wget_s ws;
	struct timeval tv;
	struct wget_param *param = (struct wget_param *)arg;
	struct http_keyvalue_t *cur = NULL;
	struct http_client_response_t response;
	bool redirected = false;
	bool read_finish = false;
	char *dest, post_size[8];

#ifdef CONFIG_NET_SECURITY_TLS
	struct http_client_tls_t *client_tls = (struct http_client_tls_t *)malloc(sizeof(struct http_client_tls_t));
	if (client_tls == NULL) {
		free(param->buffer);
		free(param);

		return (pthread_addr_t)WGET_ERR;
	}
#endif

	/* Initialize the state structure */

	memset(&ws, 0, sizeof(struct wget_s));
	ws.buffer = param->buffer;
	ws.buflen = param->buflen;

	/* Parse the hostname (with optional port number) and filename from the URL */

	ret = netlib_parsehttpurl(param->url, &ws.port, ws.hostname, CONFIG_WEBCLIENT_MAXHOSTNAME, ws.filename, CONFIG_WEBCLIENT_MAXFILENAME);
	if (ret != 0) {
		ndbg("ERROR: Malformed HTTP URL: %s\n", param->url);
		free(param->buffer);
		free(param);
#ifdef CONFIG_NET_SECURITY_TLS
		free(client_tls);
#endif
		return (pthread_addr_t)WGET_ERR;
	}

	nvdbg("hostname='%s' filename='%s'\n", ws.hostname, ws.filename);

#ifdef CONFIG_NET_SECURITY_TLS
	if (param->tls && webclient_tls_init(client_tls, &param->ssl_config)) {
		ndbg("Fail to client tls init\n");
		goto errout;
	}
#endif

	/* The following sequence may repeat indefinitely if we are redirected */

	do {

		/* Re-initialize portions of the state structure that could have
		 * been left from the previous time through the loop and should not
		 * persist with the new connection.
		 */
		ws.httpstatus = HTTPSTATUS_NONE;
		ws.offset = 0;
		ws.datend = 0;
		ws.ndx = 0;

		/* Create a socket */
		sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (sockfd < 0) {

			/* socket failed.  It will set the errno appropriately */
			ndbg("ERROR: socket failed: %d\n", errno);
			free(param->buffer);
			free(param);
#ifdef CONFIG_NET_SECURITY_TLS
			free(client_tls);
#endif
			return (pthread_addr_t)WGET_ERR;
		}

		/* Set send and receive timeout values */

		tv.tv_sec = WEBCLIENT_CONF_TIMEOUT_MSEC / 1000;
		tv.tv_usec = (WEBCLIENT_CONF_TIMEOUT_MSEC % 1000) * 1000;
		if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (struct timeval *)&tv, sizeof(struct timeval)) < 0) {
			ndbg("ERROR: setsockopt failed\n");
		}

		/* Get the server address from the host name */

		server.sin_family = AF_INET;
		server.sin_port = htons(ws.port);
		ret = wget_gethostip(ws.hostname, &server.sin_addr.s_addr);
		if (ret < 0) {

			/* Could not resolve host (or malformed IP address) */

			ndbg("ERROR: Failed to resolve hostname\n");
			ret = -EHOSTUNREACH;
			goto errout;
		}

		/* Connect to server.  First we have to set some fields in the
		 * 'server' address structure.  The system will assign me an arbitrary
		 * local port that is not in use.
		 */

		ret = connect(sockfd, (struct sockaddr *)&server, sizeof(struct sockaddr_in));
		if (ret < 0) {
			ndbg("ERROR: connect failed: %d\n", ret);
			goto errout;
		}

		/* Send method */

		dest = ws.buffer;
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
			goto errout;
		}

		dest = wget_strcpy(dest, ws.filename, param);
		if (dest == NULL) {
			goto errout;
		}

		*dest++ = ISO_space;
		dest = wget_strcpy(dest, g_http10, param);
		if (dest == NULL) {
			goto errout;
		}
		dest = wget_strcpy(dest, g_httpcrnl, param);
		if (dest == NULL) {
			goto errout;
		}
		dest = wget_strcpy(dest, g_httphost, param);
		if (dest == NULL) {
			goto errout;
		}
		dest = wget_strcpy(dest, ws.hostname, param);
		if (dest == NULL) {
			goto errout;
		}
		dest = wget_strcpy(dest, g_httpcrnl, param);
		if (dest == NULL) {
			goto errout;
		}

		/* header of entity */

		if (param->method == WGET_MODE_POST || param->method == WGET_MODE_PUT) {
			dest = wget_strcpy(dest, g_httpform, param);
			if (dest == NULL) {
				goto errout;
			}
			dest = wget_strcpy(dest, g_httpcrnl, param);
			if (dest == NULL) {
				goto errout;
			}

			/* content length */

			if (!param->encoding) {
				dest = wget_strcpy(dest, g_httpcontsize, param);
				if (dest == NULL) {
					goto errout;
				}
				post_len = strlen((char *)param->entity);
				sprintf(post_size, "%d", post_len);
				dest = wget_strcpy(dest, post_size, param);
				if (dest == NULL) {
					goto errout;
				}
				dest = wget_strcpy(dest, g_httpcrnl, param);
				if (dest == NULL) {
					goto errout;
				}
			}

			/* chuncked param->encoding */

			else {
				dest = wget_strcpy(dest, g_httpchunked, param);
				if (dest == NULL) {
					goto errout;
				}
				dest = wget_strcpy(dest, g_httpcrnl, param);
				if (dest == NULL) {
					goto errout;
				}
			}
		}

		cur = param->headers->head->next;
		while (cur != param->headers->tail) {
			dest = wget_strcpy(dest, cur->key, param);
			if (dest == NULL) {
				goto errout;
			}
			dest = wget_strcpy(dest, ": ", param);
			if (dest == NULL) {
				goto errout;
			}
			dest = wget_strcpy(dest, cur->value, param);
			if (dest == NULL) {
				goto errout;
			}
			dest = wget_strcpy(dest, g_httpcrnl, param);
			if (dest == NULL) {
				goto errout;
			}
			cur = cur->next;
		}
		dest = wget_strcpy(dest, g_httpcrnl, param);
		if (dest == NULL) {
			goto errout;
		}

		/* entity is needed POST or PUT method */

		if (param->method == WGET_MODE_POST || param->method == WGET_MODE_PUT) {

			/* content length */

			if (!param->encoding) {
				dest = wget_strcpy(dest, (char *)param->entity, param);
				if (dest == NULL) {
					goto errout;
				}
			}

			/* chunked param->encoding */

			else {
				post_len = strlen(param->entity);
				if (post_len > param->buflen) {
					dest = wget_chunksize(dest, param->buflen, param);
					if (dest == NULL) {
						goto errout;
					}
					dest = wget_strcpy(dest, g_httpcrnl, param);
					if (dest == NULL) {
						goto errout;
					}
					dest = wget_strlencpy(dest, (char *)param->entity, param->buflen, param);
					if (dest == NULL) {
						goto errout;
					}
					dest = wget_strcpy(dest, g_httpcrnl, param);
					if (dest == NULL) {
						goto errout;
					}
					post_len -= param->buflen;
				} else {
					dest = wget_chunksize(dest, post_len, param);
					if (dest == NULL) {
						goto errout;
					}
					dest = wget_strcpy(dest, g_httpcrnl, param);
					if (dest == NULL) {
						goto errout;
					}
					dest = wget_strcpy(dest, (char *)param->entity, param);
					if (dest == NULL) {
						goto errout;
					}
					dest = wget_strcpy(dest, g_httpcrnl, param);
					if (dest == NULL) {
						goto errout;
					}
					dest = wget_strlencpy(dest, "\0", 1, param);
					if (dest == NULL) {
						goto errout;
					}
					dest = wget_strcpy(dest, g_httpcrnl, param);
					if (dest == NULL) {
						goto errout;
					}
				}
			}
		}

		len = dest - param->buffer;
#ifdef CONFIG_NET_SECURITY_TLS
		client_tls->client_fd = sockfd;
		if (param->tls && wget_tls_handshake(client_tls)) {
			ndbg("Fail to client tls handshake\n");
			goto errout;
		}
#endif
		sndlen = len;
		buf_len = 0;
		while (sndlen > 0) {
#ifdef CONFIG_NET_SECURITY_TLS
			if (param->tls) {
				ret = mbedtls_ssl_write(&(client_tls->tls_ssl), (const unsigned char *)param->buffer + buf_len, sndlen);
			} else
#endif
			{
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

		if (param->callback) {
			param->response = &response;
			if (http_client_response_init(param->response) < 0) {
				ndbg("ERROR: response init failed: %d\n", ret);
				goto errout;
			}
		}

		buf_len = 0;
		while (!read_finish) {
			if (remain <= 0) {
				ndbg("Error: Response Size is too large\n");
				goto errout;
			}
#ifdef CONFIG_NET_SECURITY_TLS
			if (param->tls) {
				len = mbedtls_ssl_read(&(client_tls->tls_ssl), (unsigned char *)param->response->message + buf_len, WEBCLIENT_CONF_MAX_MESSAGE_SIZE - buf_len);
			} else
#endif
			{
				len = recv(sockfd, param->response->message + buf_len, WEBCLIENT_CONF_MAX_MESSAGE_SIZE - buf_len, 0);
			}

			if (len < 0) {
				ndbg("Error: Receive Fail\n");
				goto errout;
			} else if (len == 0) {
				ndbg("Finish read\n");
				goto errout;
			}

			buf_len += len;
			remain -= len;
			usleep(1);
			read_finish = http_parse_message(param->response->message, len, NULL, param->response->url, &param->response->entity, &encoding, &state, &mlen, param->response->headers, NULL, param->response);
		}
	} while (redirected);
	param->response->method = param->method;
	param->response->url = param->url;
	param->response->entity_len = strlen(param->response->entity);
	if (param->callback) {
		param->callback(param->response);
		http_client_response_release(param->response);
	}
#ifdef CONFIG_NET_SECURITY_TLS
	if (param->tls) {
		wget_tls_release(client_tls);
		wget_tls_ssl_release(client_tls);
	}
	free(client_tls);
#endif
	if (!param->tls) {
		close(sockfd);
	}
	free(param->buffer);
	free(param);

	return (pthread_addr_t)WGET_OK;

 errout:
	if (!param->tls) {
		close(sockfd);
	}
#ifdef CONFIG_NET_SECURITY_TLS
	if (client_tls) {
		free(client_tls);
	}
#endif
	free(param->buffer);
	free(param);
	return (pthread_addr_t)WGET_ERR;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

int client_send_request(struct wget_param *p)
{
	pthread_attr_t attr;
	pthread_t tid;
	struct wget_param *param = (struct wget_param *)malloc(sizeof(struct wget_param));
	pthread_addr_t ret = NULL;

	if (param == NULL) {
		printf("Error: Cannot alloc memory!!\n");
		return -1;
	}

	if (p->method < WGET_MODE_GET || p->method > WGET_MODE_DELETE) {
		printf("Error: Incorrect method value!!\n");
		goto errret;
	}

	if (p->buffer == NULL) {
		printf("Error: Buffer is NULL!!\n");
		goto errret;
	}

	if (p->buflen <= 0) {
		printf("Error: Buffer length must be bigger than 0!!\n");
		goto errret;
	}

	if (p->encoding != CONTENT_LENGTH && p->encoding != CHUNKED_ENCODING) {
		printf("Error: Incorrect encoding value!!\n");
		goto errret;
	}

	memcpy(param, p, sizeof(struct wget_param));

	if (param->callback) {
		pthread_attr_init(&attr);
		pthread_attr_setstacksize(&attr, CLIENT_REQ_THREAD_STACK);

		if (pthread_create(&tid, &attr, wget_base, (void *)param) != 0) {
			printf("Error: Cannot create thread!!\n");
			goto errret;
		}
		pthread_setname_np(tid, "webclient request");
		pthread_detach(tid);
	} else {
		ret = wget_base((void *)param);
	}

	return (int)ret;
 errret:
	free(p->buffer);
	free(param);
	return -1;
}

int http_client_send_request(struct http_client_request_t *request, struct http_client_ssl_config_t *ssl_config, struct http_client_response_t *response)
{
	char *buffer;
	struct wget_param p;
#ifdef CONFIG_NET_SECURITY_TLS
	struct mallinfo data;
#endif

	if (request == NULL) {
		printf("Error: Request is null\n");
		return -1;
	} else {
		buffer = (char *)malloc(request->buflen);
		if (buffer == NULL) {
			printf("Error: Fail to malloc buffer\n");
			return -1;
		}

		p.buffer = buffer;
		p.buflen = request->buflen;
		p.method = request->method;
		p.encoding = request->encoding;
		p.headers = request->headers;
		p.callback = NULL;
		p.tls = false;
		p.response = response;
	}
	if (response == NULL) {
		printf("Error: Response is null\n");
		free(buffer);
		return -1;
	}
	if (request->url == NULL) {
		printf("Error: URL is NULL!!\n");
		free(buffer);
		return -1;
	}
	if (WEBCLIENT_CONF_MAX_ENTITY_SIZE < strlen(request->entity)) {
		printf("Error: Too small buffer size\n");
		free(buffer);
		return -1;
	}
	p.url = request->url;
	p.entity = request->entity;

#ifdef CONFIG_NET_SECURITY_TLS
	if (ssl_config) {
		p.tls = true;
		memcpy(&p.ssl_config, ssl_config, sizeof(struct http_client_ssl_config_t));

		data = mallinfo();
		if (data.fordblks < WEBCLIENT_CONF_MIN_TLS_MEMORY) {
			printf("Error: Not enough memory!!\n");
			free(buffer);
			return -1;
		}
	}
#endif

	return client_send_request(&p);
}

int http_client_send_request_async(struct http_client_request_t *request, struct http_client_ssl_config_t *ssl_config, wget_callback_t cb)
{
	char *buffer;
	struct wget_param p;
#ifdef CONFIG_NET_SECURITY_TLS
	struct mallinfo data;
#endif

	if (cb == NULL) {
		printf("Error: Callback is Null\n");
		return -1;
	}
	if (request == NULL) {
		printf("Error: Request is null\n");
		return -1;
	} else {
		buffer = (char *)malloc(request->buflen);
		if (buffer == NULL) {
			printf("Error: Fail to malloc buffer\n");
			return -1;
		}

		p.buffer = buffer;
		p.buflen = request->buflen;
		p.method = request->method;
		p.encoding = request->encoding;
		p.headers = request->headers;
		p.callback = cb;
		p.tls = false;
		p.response = NULL;
	}
	if (request->url == NULL) {
		printf("Error: URL is NULL!!\n");
		free(buffer);
		return -1;
	}
	if (WEBCLIENT_CONF_MAX_ENTITY_SIZE < strlen(request->entity)) {
		printf("Error: Too small buffer size\n");
		free(buffer);
		return -1;
	}
	p.url = request->url;
	p.entity = request->entity;

#ifdef CONFIG_NET_SECURITY_TLS
	if (ssl_config) {
		p.tls = true;
		memcpy(&p.ssl_config, ssl_config, sizeof(struct http_client_ssl_config_t));

		data = mallinfo();
		if (data.fordblks < WEBCLIENT_CONF_MIN_TLS_MEMORY) {
			printf("Error: Not enough memory!!\n");
			free(buffer);
			return -1;
		}
	}
#endif

	return client_send_request(&p);
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
	if (http_keyvalue_list_init(response->headers) < 0) {
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
