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
/**
 * @defgroup HTTP_CLIENT HTTP Client
 * @brief Provides APIs for HTTP Client
 * @ingroup HTTP
 *	 The client side of Hypertext Transfer Protocol
 *
 * @{
 */

/**
 * @file webclient_common.h
 * @brief APIs for HTTP Client.
 */

#ifndef __WEBCLIENT_COMMON_H
#define __WEBCLIENT_COMMON_H

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

/****************************************************************************
 * Definitions
 ****************************************************************************/

#define CONFIG_WEBCLIENT_MAXHTTPLINE 200
#define CONFIG_WEBCLIENT_MAXMIMESIZE 32
#define CONFIG_WEBCLIENT_MAXHOSTNAME 40
#define CONFIG_WEBCLIENT_MAXFILENAME 200

#ifndef CONFIG_WGET_USERAGENT
#define CONFIG_WGET_USERAGENT "TinyARA"
#endif

#define CLIENT_REQ_THREAD_STACK    (1024 * 8)

#define WGET_OK					   0
#define WGET_ERR				   -1
#define WGET_MSG_CONSTRUCT_ERR	   -2
#define WGET_SOCKET_CONNECT_ERR    -3

#define WEBCLIENT_STATE_STATUSLINE 0
#define WEBCLIENT_STATE_HEADERS    1
#define WEBCLIENT_STATE_DATA	   2
#define WEBCLIENT_STATE_CLOSE	   3

#define HTTPSTATUS_NONE			   0
#define HTTPSTATUS_OK			   1
#define HTTPSTATUS_MOVED		   2
#define HTTPSTATUS_ERROR		   3

#define ISO_nl					   0x0a
#define ISO_cr					   0x0d
#define ISO_space				   0x20

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
static const char tlsname[] = "araweb_tls_client";

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

#ifdef __cplusplus
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif

char *wget_strcpy(char *dest, const char *src, struct http_client_request_t *ws);

char *wget_strlencpy(char *dest, const char *src, int len, struct http_client_request_t *ws);

char *wget_chunksize(char *dest, int len, struct http_client_request_t *ws);

#ifdef WGET_USE_URLENCODE
char *wget_urlencode_strcpy(char *dest, const char *src);
#endif

int wget_parsestatus(struct wget_s *ws);

int wget_parseheaders(struct wget_s *ws);

int wget_gethostip(FAR char *hostname, in_addr_t *ipv4addr);

#ifdef CONFIG_MBEDTLS
void wget_tls_debug(void *ctx, int level, const char *file, int line, const char *str);

int webclient_tls_init(struct http_client_tls_t *client, struct http_client_ssl_config_t *ssl_config);

void wget_tls_release(struct http_client_tls_t *client);

void wget_tls_ssl_release(struct http_client_tls_t *client);

int wget_tls_handshake(struct http_client_tls_t *client, const char *hostname);
#endif /* CONFIG_MBEDTLS */

int wget_socket_connect(struct wget_s *ws);

#undef EXTERN
#ifdef __cplusplus
}
#endif

#endif							/* __WEBCLIENT_COMMON_H */
/**@} */
