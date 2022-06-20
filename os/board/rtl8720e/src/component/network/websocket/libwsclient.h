#ifndef EASYWSCLIENT_H
#define EASYWSCLIENT_H
#include <platform_stdlib.h>
#include "osdep_service.h"


#define WSCLIENT_TLS_POLARSSL       0    /*!< Use PolarSSL for TLS when WSCLIENT */
#define WSCLIENT_TLS_MBEDTLS        1    /*!< Use mbedTLS for TLS when WSCLIENT */

#if CONFIG_USE_POLARSSL
#define WSCLIENT_USE_TLS            WSCLIENT_TLS_POLARSSL
#elif CONFIG_USE_MBEDTLS
#define WSCLIENT_USE_TLS            WSCLIENT_TLS_MBEDTLS
#endif

/****************Define the debug message level*********************/
#define DEBUG_WSCLIENT    1

#define WSCLIENT_LOG(level, fmt, ...) printf("\n\r[WSCLIENT %s] %s: " fmt "\n", level, __FUNCTION__, ##__VA_ARGS__)
#if DEBUG_WSCLIENT == 2
#define WSCLIENT_DEBUG(fmt, ...) WSCLIENT_LOG("DEBUG", fmt, ##__VA_ARGS__)
#else
#define WSCLIENT_DEBUG(fmt, ...)
#endif
#if DEBUG_WSCLIENT
#define WSCLIENT_ERROR(fmt, ...) WSCLIENT_LOG("ERROR", fmt, ##__VA_ARGS__)
#else
#define WSCLIENT_ERROR(fmt, ...)
#endif
/*******************************************************************/

/****************Define the structures used*************************/
typedef enum {
	CLOSING,
	CLOSED,
	CONNECTING,
	OPEN
} readyStateValues;

struct wsheader_type {
	unsigned header_size;
	int fin;
	int mask;
	enum opcode_type {
		CONTINUATION = 0x0,
		TEXT_FRAME = 0x1,
		BINARY_FRAME = 0x2,
		CLOSE = 8,
		PING = 9,
		PONG = 0xa,
	} opcode;
	int N0;
	uint64_t N;
	uint8_t masking_key[4];
};

struct rsv_bits_field {
	uint8_t RSV1 : 1;
	uint8_t RSV2 : 1;
	uint8_t RSV3 : 1;
};

typedef struct send_buf_t {
	uint8_t *txbuf;
	int tx_len;
} send_buf;

struct _wsclient_context;

struct ws_fun_ops {
	int (*hostname_connect)(struct _wsclient_context *wsclient);
	void (*client_close)(struct _wsclient_context *wsclient);
	int (*client_send)(struct _wsclient_context *wsclient, unsigned char *data, size_t data_len);
	int (*client_read)(struct _wsclient_context *wsclient, unsigned char *data, size_t data_len);
};

typedef struct _wsclient_context {
	char *host;
	char *path;
	char *origin;
	int port;
	char *protocol;
	int protocol_len;
	char *version;
	int version_len;
	char *custom_token;
	int custom_token_len;
	uint8_t use_ssl;
	int sockfd;
	readyStateValues readyState;
	int tx_len;
	int rx_len;
	void *tls;
	int max_queue_size;
	int stable_buf_num;
	int ready_send_buf_num;
	int recycle_send_buf_num;
	_xqueue ready_send_buf; //tx message ready to send
	_xqueue recycle_send_buf; //usable buf to load tx message
	uint8_t *txbuf;
	struct rsv_bits_field txRsvBits;
	uint8_t *rxbuf;
	struct rsv_bits_field rxRsvBits;
	uint8_t *receivedData;
	struct ws_fun_ops fun_ops;
} wsclient_context;
/*******************************************************************/

/****************General functions used by wsclient*****************/
void ws_get_random_bytes(void *buf, size_t len);
void *ws_malloc(unsigned int size);
void ws_free(void *buf);
int ws_client_handshake(wsclient_context *wsclient);
int ws_check_handshake(wsclient_context *wsclient);
int ws_sendData(uint8_t type, size_t message_size, uint8_t *message, int useMask, wsclient_context *wsclient);
/*******************************************************************/

/*************Functions used by wsclient without SSL****************/

int ws_hostname_connect(wsclient_context *wsclient);
int ws_client_read(wsclient_context *wsclient, unsigned char *data, size_t data_len);
int ws_client_send(wsclient_context *wsclient, unsigned char *data, size_t data_len);
void ws_client_close(wsclient_context *wsclient);
/*******************************************************************/

/***************Functions used by wsclient with SSL*****************/
int wss_hostname_connect(wsclient_context *wsclient);
int wss_client_read(wsclient_context *wsclient, unsigned char *data, size_t data_len);
int wss_client_send(wsclient_context *wsclient, unsigned char *data, size_t data_len);
void wss_client_close(wsclient_context *wsclient);
void *wss_tls_connect(int *sock, char *host, int port);
int wss_tls_handshake(void *tls_in);
void wss_tls_close(void *tls_in, int *sock);
int wss_tls_write(void *tls_in, char *request, int request_len);
int wss_tls_read(void *tls_in, char *buffer, int buf_len);
/*******************************************************************/

#endif
