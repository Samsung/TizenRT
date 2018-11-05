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

#include <sys/types.h>
#include <sys/socket.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <errno.h>
#include <fcntl.h>
#include <debug.h>
#include <pthread.h>

#ifdef CONFIG_NET_LWIP_NETDB
#include <netdb.h>
#endif
#include <arpa/inet.h>
#include <netinet/in.h>

#include <tinyara/version.h>
#include <netutils/netlib.h>

#include "../webserver/http_string_util.h"
#include "../webserver/http_client.h"
#include <protocols/webserver/http_err.h>
#include <protocols/webclient.h>
#if defined(CONFIG_NETUTILS_CODECS)
#  if defined(CONFIG_CODECS_URLCODE)
#    define WGET_USE_URLENCODE 1
#    include <codecs/urldecode.h>
#  endif
#  if defined(CONFIG_CODECS_BASE64)
#    include <codecs/base64.h>
#  endif
#else
#  undef CONFIG_CODECS_URLCODE
#  undef CONFIG_CODECS_BASE64
#endif

#ifndef CONFIG_WEBCLIENT_MAXHTTPLINE
#define CONFIG_WEBCLIENT_MAXHTTPLINE 512
#endif

#ifndef CONFIG_WEBCLIENT_MAXMIMESIZE
#define CONFIG_WEBCLIENT_MAXMIMESIZE 128
#endif

#ifndef CONFIG_WEBCLIENT_MAXHOSTNAME
#define CONFIG_WEBCLIENT_MAXHOSTNAME 128
#endif

#ifndef CONFIG_WEBCLIENT_MAXFILENAME
#define CONFIG_WEBCLIENT_MAXFILENAME 384
#endif

#ifndef CONFIG_WGET_USERAGENT
# define CONFIG_WGET_USERAGENT "TinyARA"
#endif

/****************************************************************************
 * Definitions
 ****************************************************************************/

#define CLIENT_REQ_THREAD_STACK    (1024 * 8)

#define WGET_OK                    0
#define WGET_ERR                   -1
#define WGET_MSG_CONSTRUCT_ERR     -2
#define WGET_SOCKET_CONNECT_ERR    -3

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

#define MBED_DEBUG_LEVEL 0

/****************************************************************************
 * Private Types
 ****************************************************************************/

struct wget_s {
	/* Internal status */

	uint8_t state;
	uint8_t httpstatus;

	uint16_t port; /* The port number to use in the connection */

	/* These describe the just-received buffer of data */

	FAR
	char *buffer; /* user-provided buffer */
	int buflen; /* Length of the user provided buffer */
	int offset; /* Offset to the beginning of interesting data */
	int datend; /* Offset+1 to the last valid byte of data in the buffer */

	/* Buffer HTTP header data and parse line at a time */

	char line[CONFIG_WEBCLIENT_MAXHTTPLINE];
	int ndx;

#ifdef CONFIG_WEBCLIENT_GETMIMETYPE
	char mimetype[CONFIG_WEBCLIENT_MAXMIMESIZE];
#endif
	char hostname[CONFIG_WEBCLIENT_MAXHOSTNAME];
	char filename[CONFIG_WEBCLIENT_MAXFILENAME];
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
static const char g_http200[] = "200 ";
static const char g_http301[] = "301 ";
static const char g_http302[] = "302 ";
static const char g_http404[] = "404 ";
static const char g_http500[] = "500 ";

static const char g_httpcrnl[] = "\r\n";

static const char g_httpform[] =
	"Content-Type: application/x-www-form-urlencoded";
static const char g_httpcontsize[] = "Content-Length: ";
static const char g_httpchunked[] = "Transfer-Encoding: chunked";
const char *tlsname = "araweb_tls_client";

/****************************************************************************
 * Private Functions
 ****************************************************************************/

#ifdef CONFIG_NET_SECURITY_TLS
static void wget_tls_release(struct http_client_tls_t *client);
#endif

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: wget_strcpy
 ****************************************************************************/

static char *wget_strcpy(char *dest, const char *src, struct http_client_request_t *ws)
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

static char *wget_strlencpy(char *dest, const char *src, int len, struct http_client_request_t *ws)
{
	if (dest + len - ws->buffer >= ws->buflen) {
		printf("Error: buffer is too small\n");
		return NULL;
	}

	memcpy(dest, src, len);
	dest[len] = '\0';
	return dest + len;
}

static char *wget_chunksize(char *dest, int len, struct http_client_request_t *ws)
{
	char size[10] = {0,};

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

				else if (strncmp(dest, g_http301, strlen(g_http301)) == 0
						 || strncmp(dest, g_http302, strlen(g_http302)) == 0
						 || strncmp(dest, g_http404, strlen(g_http404)) == 0
						 || strncmp(dest, g_http500, strlen(g_http500)) == 0) {

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

			if (ndx > 0) { /* Should always be true */
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

					strncpy(ws->mimetype, ws->line + strlen(g_httpcontenttype),
							sizeof(ws->mimetype));
				} else
#endif
					if (strncasecmp(ws->line, g_httplocation,
									strlen(g_httplocation)) == 0) {
						/* Parse the new HTTP host and filename from the URL.  Note that
						 * the return value is ignored.  In the event of failure, we
						 * retain the current location.
						 */

						(void) netlib_parsehttpurl(
							ws->line + strlen(g_httplocation), &ws->port,
							ws->hostname, CONFIG_WEBCLIENT_MAXHOSTNAME,
							ws->filename, CONFIG_WEBCLIENT_MAXFILENAME);
						nvdbg("New hostname='%s' filename='%s'\n", ws->hostname,
							  ws->filename);
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

static int wget_gethostip(FAR char *hostname, in_addr_t *ipv4addr)
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
	ndbg("NETDB is not supported\n");
	return ERROR;
#endif
}

#ifdef CONFIG_NET_SECURITY_TLS
static void wget_tls_debug(void *ctx, int level, const char *file, int line, const char *str)
{
	ndbg("%s:%04d: %s", file, line, str);
}

int webclient_tls_init(struct http_client_tls_t *client, struct http_client_ssl_config_t *ssl_config)
{
	int result = 0;

	if (!client || !ssl_config) {
		return -1;
	}

	mbedtls_ssl_config_init(&(client->tls_conf));
	mbedtls_x509_crt_init(&(client->tls_rootca));
	mbedtls_x509_crt_init(&(client->tls_clicert));
	mbedtls_pk_init(&(client->tls_pkey));
	mbedtls_entropy_init(&(client->tls_entropy));
	mbedtls_ctr_drbg_init(&(client->tls_ctr_drbg));
	mbedtls_ssl_session_init(&(client->tls_session));
	mbedtls_ssl_conf_authmode(&(client->tls_conf), ssl_config->auth_mode);
#ifdef MBEDTLS_DEBUG_C
	mbedtls_debug_set_threshold(MBED_DEBUG_LEVEL);
#endif

	/* 0. Initialize the RNG */
	ndbg("  . Seeding the random number generator...");

	if ((result = mbedtls_ctr_drbg_seed(&(client->tls_ctr_drbg), mbedtls_entropy_func, &(client->tls_entropy), (const unsigned char *)tlsname, strlen(tlsname))) != 0) {
		ndbg("Error: mbedtls_ctr_drbg_seed returned -%4x\n", -result);
		goto TLS_INIT_EXIT;
	}

	ndbg("Ok\n");

	/* 1. Setup ssl stuff */
	ndbg("  . Setting up the SSL data...");

	if ((result = mbedtls_ssl_config_defaults(&(client->tls_conf),
				  MBEDTLS_SSL_IS_CLIENT,
				  MBEDTLS_SSL_TRANSPORT_STREAM,
				  MBEDTLS_SSL_PRESET_DEFAULT)) != 0) {
		ndbg("Error: mbedtls_ssl_config_defaults returned -%4x\n", -result);
		goto TLS_INIT_EXIT;
	}

	mbedtls_ssl_conf_rng(&(client->tls_conf), mbedtls_ctr_drbg_random,
						 &(client->tls_ctr_drbg));
	mbedtls_ssl_conf_dbg(&(client->tls_conf), wget_tls_debug, stdout);

	ndbg("Ok\n");

	if (ssl_config->dev_cert && ssl_config->private_key) {
		/* 2. Load the certificates and private key */

		ndbg("  . Loading the client cert. and key...");

		if ((result = mbedtls_x509_crt_parse(&(client->tls_clicert),
											 (const unsigned char *)ssl_config->dev_cert,
											 ssl_config->dev_cert_len)) != 0) {
			ndbg("Error: cli_cert parse fail, returned -%4x\n", -result);
			goto TLS_INIT_EXIT;
		}

		if ((result = mbedtls_pk_parse_key(&(client->tls_pkey),
										   (const unsigned char *)ssl_config->private_key,
										   ssl_config->private_key_len, NULL, 0)) != 0) {
			ndbg("Error: cli_key parse fail, returned -%4x\n", -result);
			goto TLS_INIT_EXIT;
		}

		if ((result = mbedtls_ssl_conf_own_cert(&(client->tls_conf),
												&(client->tls_clicert),
												&(client->tls_pkey))) != 0) {
			ndbg("Error: mbedtls_ssl_conf_own_cert returned -%4x\n", -result);
			goto TLS_INIT_EXIT;
		}

		ndbg("Ok\n");
	}

	if (ssl_config->root_ca) {

		/* 3. Load the CA certificate */
		ndbg("  . Loading the CA cert...");

		if ((result = mbedtls_x509_crt_parse(&(client->tls_rootca),
											 (const unsigned char *)ssl_config->root_ca,
											 ssl_config->root_ca_len)) != 0) {
			ndbg("Error: CA_cert parse fail, returned -%4x\n", -result);
			goto TLS_INIT_EXIT;
		}

		/* CA cert may be first or second in chain depending if client cert was loaded */
		mbedtls_ssl_conf_ca_chain(&(client->tls_conf), &(client->tls_rootca), NULL);

		ndbg("Ok\n");
	}

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

	mbedtls_x509_crt_free(&(client->tls_rootca));
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

	mbedtls_net_init(&(client->tls_client_fd));
	mbedtls_ssl_init(&(client->tls_ssl));

	client->tls_client_fd.fd = client->client_fd;

	if (mbedtls_net_set_block(&(client->tls_client_fd)) < 0) {
		ndbg("Error: mbedtls_net_set_block fail\n");
		goto HANDSHAKE_FAIL;
	}

	ndbg("TLS Init Success\n");

	if ((result = mbedtls_ssl_setup(&(client->tls_ssl),
									&(client->tls_conf))) != 0) {
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
	if ((result = mbedtls_ssl_set_hostname(&(client->tls_ssl), hostname)) != 0) {
		ndbg("Error: mbedtls_hostname returned -%4x\n", -result);
		goto HANDSHAKE_FAIL;
	}
#endif

	mbedtls_ssl_set_bio(&(client->tls_ssl), &(client->tls_client_fd),
						mbedtls_net_send, mbedtls_net_recv, NULL);

	/* Handshake */
	while ((result = mbedtls_ssl_handshake(&(client->tls_ssl))) != 0) {
		if (result != MBEDTLS_ERR_SSL_WANT_READ &&
			result != MBEDTLS_ERR_SSL_WANT_WRITE) {
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

static int wget_msg_construct(char *buf, struct http_client_request_t *param, struct wget_s *ws)
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

static int wget_socket_connect(struct wget_s *ws)
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
	nvdbg("webclient recv timeout(%d.%d)sec\n", tv.tv_sec, tv.tv_usec);
	if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO,
				   (struct timeval *)&tv, sizeof(struct timeval)) < 0) {
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
#ifdef CONFIG_NET_SECURITY_TLS
static pthread_addr_t wget_base(void *arg)
{
	int sockfd = -1;
	int ret;
	int buf_len, sndlen, len;
	int encoding = CONTENT_LENGTH;
	int state = HTTP_REQUEST_HEADER;
	struct http_message_len_t mlen = {0,};
	struct wget_s ws;
	struct http_client_request_t *param = (struct http_client_request_t *)arg;
	struct http_client_response_t response = {0, };
	int read_finish = false;

	struct http_client_tls_t *client_tls = (struct http_client_tls_t *)malloc(sizeof(
			struct http_client_tls_t));
	int handshake_retry = WEBCLIENT_CONF_HANDSHAKE_RETRY;

	if (client_tls == NULL) {
		free(param->buffer);
		return (pthread_addr_t)WGET_ERR;
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
		return (pthread_addr_t)WGET_ERR;
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

	if ((sndlen = wget_msg_construct(ws.buffer, param, &ws)) <= 0) {
		ndbg("ERROR: construction message failed\n");
		goto errout_before_tlsinit;
	}

retry:
	if ((sockfd = wget_socket_connect(&ws)) < 0) {
		ndbg("ERROR: socket failed: %d\n", errno);
		goto errout_before_tlsinit;
	}

	client_tls->client_fd = sockfd;
	if (param->tls && (ret = wget_tls_handshake(client_tls, ws.hostname))) {
		if (handshake_retry-- > 0) {
			if (ret == MBEDTLS_ERR_NET_SEND_FAILED ||
				ret == MBEDTLS_ERR_NET_RECV_FAILED ||
				ret == MBEDTLS_ERR_SSL_CONN_EOF) {
				ndbg("Handshake again.... \n");
				mbedtls_net_free(&(client_tls->tls_client_fd));
				mbedtls_ssl_free(&(client_tls->tls_ssl));
				goto retry;
			}
		}
		goto errout;
	}

	buf_len = 0;
	while (sndlen > 0) {
		if (param->tls) {
			ret = mbedtls_ssl_write(&(client_tls->tls_ssl),
									(const unsigned char *)param->buffer + buf_len,
									sndlen);
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

	if (param->callback && param->response == NULL) {
		param->response = &response;
		if (http_client_response_init(param->response) < 0) {
			ndbg("ERROR: response init failed: %d\n", ret);
			param->response = NULL;
			goto errout;
		}
	}

	int loopcount = 0;
	while (!read_finish) {
		ndbg("Receive start\n");
		memset(param->response->message, 0, WEBCLIENT_CONF_MAX_MESSAGE_SIZE);
		if (param->tls) {
			len = mbedtls_ssl_read(&(client_tls->tls_ssl),
								   (unsigned char *)param->response->message,
								   WEBCLIENT_CONF_MAX_MESSAGE_SIZE);
		} else {
			len = recv(sockfd, param->response->message,
					   WEBCLIENT_CONF_MAX_MESSAGE_SIZE, 0);
		}

		if (len < 0) {
			ndbg("Error: Receive Fail\n");
			goto errout;
		} else if (len == 0) {
			ndbg("Finish read\n");
			if (mlen.message_len - mlen.sentence_start == mlen.content_len) {
				ndbg("download completed successfully\n");
				break;
			} else {
				ndbg("Error: Receive Fail\n");
				goto errout;
			}
		}

		usleep(1);
		read_finish = http_parse_message(param->response->message,
						 len, NULL, param->response->url,
						 &param->response->entity,
						 &encoding, &state, &mlen,
						 param->response->headers,
						 NULL, param->response, NULL);

		++loopcount;
		nvdbg("====== loopcount : %d read_finish : %d=====\n", loopcount, read_finish);
		if (read_finish == HTTP_ERROR) {
			ndbg("Error: Parse message Fail\n");
			goto errout;
		}

		param->response->method = param->method;
		param->response->url = param->url;

		if (param->callback && param->response->entity_len != 0) {
			param->callback(param->response);
		}
	}

	if (param->callback) {
		http_client_response_release(param->response);
	}
	
	if (param->tls) {
		wget_tls_release(client_tls);
		wget_tls_ssl_release(client_tls);
	}
	free(client_tls);
	if (!param->tls) {
		close(sockfd);
	}
	free(param->buffer);
	param->async_flag = WGET_OK;
	return (pthread_addr_t)WGET_OK;

errout:
	if (param->callback && param->response) {
		http_client_response_release(param->response);
	}
	if (param->tls) {
		wget_tls_ssl_release(client_tls);
	}
errout_before_tlsinit:
	if (param->tls) {
		wget_tls_release(client_tls);
	}
	if (client_tls) {
		free(client_tls);
	}
	if (!param->tls && sockfd > 0) {
		close(sockfd);
	}
	free(param->buffer);
	param->async_flag = WGET_ERR;
	return (pthread_addr_t)WGET_ERR;
}
#endif

#ifndef CONFIG_NET_SECURITY_TLS
static pthread_addr_t wget_base(void *arg)
{
	int sockfd = -1;
	int ret;
	int buf_len, sndlen, len;
	int encoding = CONTENT_LENGTH;
	int state = HTTP_REQUEST_HEADER;
	struct http_message_len_t mlen = {0,};
	struct wget_s ws;
	struct http_client_request_t *param = (struct http_client_request_t *)arg;
	struct http_client_response_t response;
	int read_finish = false;

	/* Initialize the state structure */
	memset(&ws, 0, sizeof(struct wget_s));
	ws.buffer = param->buffer;
	ws.buflen = param->buflen;

	/* Parse the hostname (with optional port number) and filename from the URL */
	ret = netlib_parsehttpurl(param->url, &ws.port, ws.hostname, CONFIG_WEBCLIENT_MAXHOSTNAME, ws.filename, CONFIG_WEBCLIENT_MAXFILENAME);
	if (ret != 0) {
		ndbg("ERROR: Malformed HTTP URL: %s\n", param->url);
		free(param->buffer);
		return (pthread_addr_t)WGET_ERR;
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

	if ((sndlen = wget_msg_construct(ws.buffer, param, &ws)) <= 0) {
		ndbg("ERROR: construction message failed\n");
		goto errout_before_init;
	}

	nvdbg("Length Info: sndlen : %d buf_len : %d param->buglen : %d\n", strlen(ws.buffer), strlen(param->buffer), param->buflen);
	nvdbg("Send Data: %s\n", param->buffer);
	if ((sockfd = wget_socket_connect(&ws)) < 0) {
		ndbg("ERROR: socket failed: %d\n", errno);
		goto errout_before_init;
	}

	buf_len = 0;
	while (sndlen > 0) {
		nvdbg("INFO: sndlen : %d buf_len : %d\n", sndlen, buf_len);
		ret = send(sockfd, param->buffer + buf_len, sndlen, 0);
		if (ret < 1) {
			ndbg("ERROR: send failed: %d\n", ret);
			goto errout;
		} else {
			sndlen -= ret;
			buf_len += ret;
			nvdbg("SEND SUCCESS: send %d bytes\n", ret);
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

	int loopcount = 0;
	while (!read_finish) {
		ndbg("Receive start\n");
		memset(param->response->message, 0, WEBCLIENT_CONF_MAX_MESSAGE_SIZE);
		len = recv(sockfd, param->response->message,
				   WEBCLIENT_CONF_MAX_MESSAGE_SIZE, 0);

		if (len < 0) {
			ndbg("Error: Receive Fail\n");
			goto errout;
		} else if (len == 0) {
			ndbg("Finish read\n");
			if (mlen.message_len - mlen.sentence_start == mlen.content_len) {
				ndbg("download completed successfully\n");
				break;
			} else {
				ndbg("Error: Receive Fail\n");
				goto errout;
			}
		}

		usleep(1);
		read_finish = http_parse_message(param->response->message,
						 len, NULL, param->response->url,
						 &param->response->entity,
						 &encoding, &state, &mlen,
						 param->response->headers,
						 NULL, param->response, NULL);

		++loopcount;
		nvdbg("====== loopcount : %d read_finish : %d=====\n", loopcount, read_finish);
		if (read_finish == HTTP_ERROR) {
			ndbg("Error: Parse message Fail %d \n", read_finish);
			goto errout;
		}

		param->response->method = param->method;
		param->response->url = param->url;

		if (param->callback && param->response->entity_len != 0) {
			param->callback(param->response);
		}
	}

	if (param->callback) {
		http_client_response_release(param->response);
	}

	close(sockfd);

	free(param->buffer);
	param->async_flag = WGET_OK;
	return (pthread_addr_t)WGET_OK;

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
	return (pthread_addr_t)WGET_ERR;
}
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

int client_send_request(struct http_client_request_t *request)
{
	pthread_attr_t attr;
	pthread_t tid;
	pthread_addr_t ret = NULL;

	if (request == NULL) {
		printf("Error: Request is null\n");
		return -1;
	}

	if (request->method < WGET_MODE_GET || request->method > WGET_MODE_DELETE) {
		printf("Error: Incorrect method value!!\n");
		return -1;
	}

	if (request->buffer == NULL) {
		printf("Error: Buffer is NULL!!\n");
		return -1;
	}

	if (request->buflen <= 0) {
		printf("Error: Buffer length must be bigger than 0!!\n");
		goto errret;
	}

	if (request->encoding != CONTENT_LENGTH && request->encoding != CHUNKED_ENCODING) {
		printf("Error: Incorrect encoding value!!\n");
		goto errret;
	}

	if (request->callback) {
		request->async_flag = 1;
		if (pthread_attr_init(&attr) != 0) {
			printf("Error: Cannot initialize thread attribute\n");
			goto errret;
		}

		pthread_attr_setstacksize(&attr, CLIENT_REQ_THREAD_STACK);

		if (pthread_create(&tid, &attr, wget_base, (void *)request) != 0) {
			printf("Error: Cannot create thread!!\n");
			goto errret;
		}
		pthread_setname_np(tid, "webclient request");
		pthread_detach(tid);
	} else {
		ret = wget_base((void *)request);
	}

	return (int)ret;
errret:
	free(request->buffer);
	return -1;
}

static int http_client_send_requests(struct http_client_request_t *request,
		 void *ssl_config,
		 struct http_client_response_t *response,
		 wget_callback_t cb)
{
#ifdef CONFIG_NET_SECURITY_TLS
	struct mallinfo data;
	struct http_client_ssl_config_t *ssl_conf = ssl_config;
#endif

	if (request == NULL) {
		printf("Error: Request is null\n");
		return -1;
	}

	request->callback = NULL;
	request->tls = false;
	request->response = response;
	request->callback = cb;

	if (request->url == NULL) {
		printf("Error: URL is NULL!!\n");
		return -1;
	}
	if (WEBCLIENT_CONF_MAX_ENTITY_SIZE < strlen(request->entity)) {
		printf("Error: Too small buffer size\n");
		return -1;
	}
#ifdef CONFIG_NET_SECURITY_TLS
	if (ssl_conf) {
		request->tls = true;
		memcpy(&request->ssl_config, ssl_conf, sizeof(struct http_client_ssl_config_t));

		data = mallinfo();
		if (data.fordblks < WEBCLIENT_CONF_MIN_TLS_MEMORY) {
			printf("Error: Not enough memory!!\n");
			return -1;
		}
	}
#endif

	request->buffer = (char *)malloc(request->buflen);
	if (request->buffer == NULL) {
		printf("Error: Fail to malloc buffer\n");
		return -1;
	}

	return client_send_request(request);
}

int http_client_send_request(struct http_client_request_t *request,
							 void *ssl_config,
							 struct http_client_response_t *response)
{
	if (response == NULL) {
		printf("Error: Response is null\n");
		return -1;
	}

	return http_client_send_requests(request, ssl_config, response, NULL);
}

int http_client_send_request_async(struct http_client_request_t *request,
								   void *ssl_config,
								   wget_callback_t cb)
{
	if (cb == NULL) {
		printf("Error: Callback is Null\n");
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
