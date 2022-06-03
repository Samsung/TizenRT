/****************************************************************************
 *
 * Copyright 2021 Samsung Electronics All Rights Reserved.
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

#ifndef __webserver_test_util_h__
#define __webserver_test_util_h__

#include <protocols/websocket.h>

#define MBED_DEBUG_LEVEL 1
#define WS_TEST_BUF_SIZE 4600

#define WS_TEST_OK                    0
#define WS_TEST_ERR                   -1
#define WS_TEST_MSG_CONSTRUCT_ERR     -2
#define WS_TEST_SOCKET_CONNECT_ERR    -3

#define HTTP_METHOD_UNKNOWN -1
#define HTTP_METHOD_GET     0
#define HTTP_METHOD_PUT     1
#define HTTP_METHOD_POST    2
#define HTTP_METHOD_DELETE  3

#define HTTP_HTTP_VERSION_UNKNOWN 0
#define HTTP_HTTP_VERSION_09      9
#define HTTP_HTTP_VERSION_10      10
#define HTTP_HTTP_VERSION_11      11

#define HTTP_CONTENT_LENGTH       0
#define HTTP_CHUNKED_ENCODING     1

#define HTTP_CONF_MAX_CLIENT                    16
#define HTTP_CONF_CLIENT_STACKSIZE              8192
#define HTTP_CONF_MIN_TLS_MEMORY                80000
#define HTTP_CONF_SOCKET_TIMEOUT_MSEC           50000
#define HTTP_CONF_SERVER_MQ_MAX_MSG             10
#define HTTP_CONF_SERVER_MQ_PRIO                50
#define HTTP_CONF_SERVER_SIGWAKEUP              18

#define HTTP_CONF_MAX_REQUEST_LENGTH            4096
#define HTTP_CONF_MAX_REQUEST_LINE_LENGTH       256
#define HTTP_CONF_MAX_REQUEST_HEADER_URL_LENGTH 128
#define HTTP_CONF_MAX_URL_QUERY_LENGTH          64
#define HTTP_CONF_MAX_URL_PARAMS_LENGTH         256
#define HTTP_CONF_MAX_KEY_LENGTH                32
#define HTTP_CONF_MAX_VALUE_LENGTH              256
#define HTTP_CONF_MAX_DIVIDED_PATH_LENGTH       32
#define HTTP_CONF_MAX_SLASH_COUNT               32
#define HTTP_CONF_MAX_QUERY_HANDLER_COUNT       64
#define HTTP_CONF_MAX_ENTITY_LENGTH             2048

#define HTTP_ERROR_400            "Bad Request"
#define HTTP_ERROR_404            "Not Found"
#define HTTP_ERROR_500            "Internal Server Error"

#define WS_TEST_CONF_MAX_URL_SIZE       50
#define WS_TEST_CONF_MAX_ENTITY_SIZE    2048
#define WS_TEST_CONF_MAX_PHRASE_SIZE    50
#define WS_TEST_CONF_MAX_MESSAGE_SIZE   6100

#define WS_TEST_MODE_GET                     0
#define WS_TEST_MODE_POST                    1
#define WS_TEST_MODE_PUT                     2
#define WS_TEST_MODE_DELETE                  3

#define HTTPSTATUS_NONE            0
#define HTTPSTATUS_OK              1
#define HTTPSTATUS_MOVED           2
#define HTTPSTATUS_ERROR           3

#define ISO_nl                     0x0a
#define ISO_cr                     0x0d
#define ISO_space                  0x20

#define WS_TEST_MAXHOSTNAME 128

static const char HTTP11[] = "HTTP/1.1";
static const char HTTPHOST[] = "host: ";
static const char HTTPGET[] = "GET ";
static const char HTTPPOST[] = "POST ";
static const char HTTPPUT[] = "PUT ";
static const char HTTPDELETE[] = "DELETE ";
static const char HTTPCRNL[] = "\r\n";

static const char HTTPFORM[] = "Content-Type: application/x-www-form-urlencoded";
static const char HTTPCONTENTSIZE[] = "Content-Length: ";
static const char HTTPCHUNKED[] = "Transfer-Encoding: chunked";
static const char TRAILERHEADER[] = "Trailer: Expires";
static const char EXPIRES[] = "Expires: Fri, 21 Oct 2022 07:28:00 GMT";

static const char HEADERFIELD_CONNECT[] = "Connection";
static const char HEADERFIELD_CLOSE[] = "close";
static const char HEADERFIELD_KEEPALIVE[] = "keep-alive";
static const char HEADERFIELD_KA[] = "Keep-Alive";
static const char HEADERFIELD_KA_PARAMETERS[] = "timeout=300, max=500";
static const char HEADERFIELD_USERAGENT[] = "User-Agent";
static const char HEADERFIELD_TINYARA[] = "TinyARA";

#define PRNT(fmt,args...) printf("\n[WS_TEST]%s():%d: "fmt, __func__, __LINE__,##args); 

#define FUNC_EN PRNT("entry")
#define FUNC_EX PRNT("exit")

enum {
	HTTP_REQUEST_HEADER, 
	HTTP_REQUEST_PARAMETERS, 
	HTTP_REQUEST_BODY
};

typedef struct http_client_response_t *httprsp;
typedef void (*wget_callback_t)(httprsp);

/**
 * @brief HTTP client TLS structure.
 */
struct http_client_tls_t {
	int client_fd;
	mbedtls_ssl_context       tls_ssl;
	mbedtls_net_context       tls_client_fd;

	int                       tls_init;
	mbedtls_ssl_config        tls_conf;
	mbedtls_entropy_context   tls_entropy;
	mbedtls_ctr_drbg_context  tls_ctr_drbg;
	mbedtls_x509_crt          tls_rootca;
	mbedtls_x509_crt          tls_clicert;
	mbedtls_pk_context        tls_pkey;
	mbedtls_ssl_session       tls_session;
};

/**
 * @brief SSL configure structure.
 */
struct http_client_ssl_config_t {
	char *root_ca;
	char *dev_cert;
	char *private_key;
	unsigned int root_ca_len;
	unsigned int dev_cert_len;
	unsigned int private_key_len;
	int auth_mode;
};

/**
 * @brief HTTP request structure.
 */
struct http_client_request_t {
	int method;
	char *url;
	char *buffer;
	int buflen;
	int encoding;
	int tls;
	char *entity;
	wget_callback_t callback;
	struct http_keyvalue_list_t *headers;
	struct http_client_response_t *response;
	struct http_client_ssl_config_t ssl_config;
	int keep_alive_count;
	int chunk_count;
	int trailer;
	int is_test_entity;
	int https;
};

/**
 * @brief HTTP response structure.
 */
struct http_client_response_t {
	int method;
	char *url;
	int status;
	char *phrase;
	struct http_keyvalue_list_t *headers;
	char *message;
	char *entity;
	unsigned int entity_len;
	unsigned int total_len;
};

struct http_req_message {
	char *req_msg;
	int method;
	uint32_t client_ip;
	char *url;
	struct http_keyvalue_list_t *headers;
	char *entity;
	int entity_len;
	char *query_string;
	int encoding;
};

struct http_keyvalue_t {
	char key[HTTP_CONF_MAX_KEY_LENGTH];
	char value[HTTP_CONF_MAX_VALUE_LENGTH];

	struct http_keyvalue_t *prev;
	struct http_keyvalue_t *next;
};

/**
 * @brief HTTP keyvalue linked list structure.
 */
struct http_keyvalue_list_t {
	struct http_keyvalue_t *head;
	struct http_keyvalue_t *tail;
};

struct http_message_len_t {
	int chunked_remain;
	int sentence_start;
	int message_len;
	int entity_len;
	int content_len;
};

char* ws_test_strcpy(char *dest, const char *src, struct http_client_request_t *ws);
char* ws_test_strlencpy(char *dest, const char *src, int len, struct http_client_request_t *ws);
int ws_test_http_client_response_init(struct http_client_response_t *response);
void ws_test_http_client_response_release(struct http_client_response_t *response);
char* ws_test_chunksize(char *dest, int len, struct http_client_request_t *ws);

int find_first_crlf(const char *src, int len, int start);
int separate_status_line(const char *src, int *status, char *phrase);
int separate_keyvalue(const char *src, char *key, char *value);

int parsehttpurl(const char *url, uint16_t *port, char *hostname, int hostlen);
void parse_header(struct http_message_len_t *len, char *buf, int buf_len,
					 struct http_client_response_t *response, int *state,
					 int *read_finish, int *process_finish);
int parse_parameter(struct http_message_len_t *len, char *buf, int buf_len,
					 struct http_client_response_t *response, int *state,
					 int *process_finish);
void parse_body(struct http_message_len_t *len, char *buf, int buf_len,
					 struct http_client_response_t *response, int *state,
					 char **body, int *read_finish, int *process_finish);
int parse_message(char *buf, int buf_len, char *url,
				  char **body, int *state,
				  struct http_message_len_t *len,
				  struct http_client_response_t *response);


#endif
