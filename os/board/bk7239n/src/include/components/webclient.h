/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2013-05-05     Bernard      the first version
 * 2013-06-10     Bernard      fix the slow speed issue when download file.
 * 2015-11-14     aozima       add content_length_remainder.
 * 2017-12-23     aozima       update gethostbyname to getaddrinfo.
 * 2018-01-04     aozima       add ipv6 address support.
 * 2018-07-26     chenyong     modify log information
 * 2018-08-07     chenyong     modify header processing
 */

/*******************************************************************
 * 组件名称：http 组件
 * 组件说明：http库
 *******************************************************************/

#ifndef     __WEBCLIENT_H__
#define     __WEBCLIENT_H__

#include <common/bk_typedef.h>
#include <os/mem.h>
#include <os/str.h>
#include <os/os.h>
#if defined(CONFIG_MBEDTLS) && !defined(CONFIG_HTTPS_CLIENT_AT_ENABLE)
#define WEBCLIENT_USING_MBED_TLS
#endif
#if defined(WEBCLIENT_USING_MBED_TLS) || defined(WEBCLIENT_USING_SAL_TLS)
#include <tls_client.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifndef web_malloc
#define web_malloc                     os_malloc
#endif

#ifndef web_calloc
#define web_calloc                     os_calloc
#endif

#ifndef web_realloc
#define web_realloc                    os_realloc
#endif

#ifndef web_free
#define web_free                       os_free
#endif

#ifndef web_strdup
#define web_strdup                    os_strdup
#endif

#define WEBCLIENT_SW_VERSION           "2.2.0"
#define WEBCLIENT_SW_VERSION_NUM       0x20200

#define WEBCLIENT_HEADER_BUFSZ         4096
#define WEBCLIENT_RESPONSE_BUFSZ       4096

enum WEBCLIENT_STATUS
{
    WEBCLIENT_OK,
    WEBCLIENT_ERROR,
    WEBCLIENT_TIMEOUT,
    WEBCLIENT_NOMEM,
    WEBCLIENT_NOSOCKET,
    WEBCLIENT_NOBUFFER,
    WEBCLIENT_CONNECT_FAILED,
    WEBCLIENT_DISCONNECT,
    WEBCLIENT_FILE_ERROR,
};

enum WEBCLIENT_METHOD
{
    WEBCLIENT_USER_METHOD,
    WEBCLIENT_GET,
    WEBCLIENT_POST,
};

struct  webclient_header
{
    char *buffer;
    size_t length;                      /* content header buffer size */

    size_t size;                        /* maximum support header size */
};

struct webclient_session
{
    struct webclient_header *header;    /* webclient response header information */
    int socket;
    int resp_status;

    char *host;                         /* server host */
    char *req_url;                      /* HTTP request address*/

    int chunk_sz;
    int chunk_offset;

    int content_length;
    char *Cookie;
    size_t content_remainder;           /* remainder of content length */

    BOOL is_tls;                   /* HTTPS connect */
#ifdef WEBCLIENT_USING_MBED_TLS
    MbedTLSSession *tls_session;        /* mbedtls connect session */
#endif
};

typedef enum {
	HTTP_EVENT_ERROR = 0,
	HTTP_EVENT_ON_CONNECTED,
	HTTP_EVENT_HEADERS_SENT,
	HTTP_EVENT_ON_HEADER,
	HTTP_EVENT_ON_DATA,
	HTTP_EVENT_ON_FINISH,
	HTTP_EVENT_DISCONNECTED,
} bk_webclient_event_id_t;

typedef struct {
	bk_webclient_event_id_t event_id;
	void *data;
	int data_len;
	void *user_data;
	char *header_key;
	char *header_value;
} bk_webclient_event_t;
typedef bk_err_t (*http_event_handle_cb)(bk_webclient_event_t *evt);

typedef struct {
	const char					*url;
	const char					*cert_pem;
	int 						header_size;
	int 						rx_buffer_size;
	int 						timeout_ms;
	char						*post_data;
	http_event_handle_cb		event_handler;
	bk_webclient_event_t	    event;
} bk_webclient_input_t;
int bk_webclient_ota_get_comm(bk_webclient_input_t *input);
int bk_webclient_get(bk_webclient_input_t *input);
int bk_webclient_post(bk_webclient_input_t *input);

/* create webclient session and set header response size */
struct webclient_session *webclient_session_create(size_t header_sz);

/* send HTTP GET request */
int webclient_get(struct webclient_session *session, const char *URI);
int webclient_get_position(struct webclient_session *session, const char *URI, int position);

/* send HTTP POST request */
int webclient_post(struct webclient_session *session, const char *URI, const void *post_data, size_t data_len);

/* close and release wenclient session */
int webclient_close(struct webclient_session *session);

int webclient_set_timeout(struct webclient_session *session, int millisecond);

/* send or receive data from server */
int webclient_read(struct webclient_session *session, void *buffer, size_t size);
int webclient_write(struct webclient_session *session, const void *buffer, size_t size);

/* webclient GET/POST header buffer operate by the header fields */
int webclient_header_fields_add(struct webclient_session *session, const char *fmt, ...);
const char *webclient_header_fields_get(struct webclient_session *session, const char *fields);

/* send HTTP POST/GET request, and get response data */
int webclient_response(struct webclient_session *session, void **response, size_t *resp_len);
int webclient_request(const char *URI, const char *header, const void *post_data, size_t data_len, void **response, size_t *resp_len);
int webclient_request_header_add(char **request_header, const char *fmt, ...);
int webclient_resp_status_get(struct webclient_session *session);
int webclient_content_length_get(struct webclient_session *session);


#ifdef  __cplusplus
    }
#endif


int test_http(void);


#endif