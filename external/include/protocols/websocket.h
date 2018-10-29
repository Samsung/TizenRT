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
/**
 * @defgroup Websocket Websocket
 * @brief Provides APIs for Websocket
 * @ingroup NETWORK
 *
 * @{
 */

/**
 * @file protocols/websocket.h
 * @brief websocket header to support WS/WSS server and client.
 */

#ifndef __EXTERNAL_INCLUDE_PROTOCOLS_WEBSOCKET_H
#define __EXTERNAL_INCLUDE_PROTOCOLS_WEBSOCKET_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <sys/types.h>
#include <stdint.h>
#include <netinet/in.h>

#include <protocols/wslay/wslay.h>
#include <tinyara/wqueue.h>

#include "mbedtls/config.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/certs.h"
#include "mbedtls/x509.h"
#include "mbedtls/ssl.h"
#include "mbedtls/net.h"
#include "mbedtls/error.h"
#include "mbedtls/debug.h"
#include "mbedtls/ssl_cache.h"

#define WEBSOCKET_DEBUG_PRINT
/**< websocket debug print option if defined, enables debug print */
#if defined(WEBSOCKET_DEBUG_PRINT)
#define WEBSOCKET_DEBUG ndbg
#else
#define WEBSOCKET_DEBUG(...) do { } while (0)
#endif

/****************************************************************************
 * Pre-prcessor Definitions
 ****************************************************************************/

/**
 * @brief Websocket GUID defined by RFC 6455
 */
#define WS_GUID                                      "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"

/**
 * @brief The maximum stack size of websocket.
 */
#define WEBSOCKET_STACKSIZE                          (1024 * 10)
/**
 * @brief The priority of websocket.
 */
#define WEBSOCKET_PRI                                100
/**
 * @brief The schedule policy of websocket..
 */
#define WEBSOCKET_SCHED_POLICY                       SCHED_RR
/**
 * @brief Websocket timer workqueue
 */
#define WEBSOCKET_WQ                                 LPWORK

/**
 * @brief Websocket http handshake key buffer size
 */
#define WEBSOCKET_CLIENT_KEY_LEN                     (24)
/**
 * @brief Websocket http handshake key buffer size
 */
#define WEBSOCKET_ACCEPT_KEY_LEN                     (29)
/**
 * @brief Websocket http handshake buffer size
 */
#define WEBSOCKET_HANDSHAKE_HEADER_SIZE              (2048)

/**
 * @brief Websocket socket input timeout value, msec.
 */
 #define WEBSOCKET_SOCK_RCV_TIMEOUT                   (60 * 1000)	//mili second

/**
 * @brief Websocket accept server select() timeout value, msec.
 */
#define WEBSOCKET_ACCEPT_TIMEOUT              (500)	//mili second
/**
 * @brief Websocket accept server select() timeout limit, msec.
 */
#define WEBSOCKET_SERVER_TIMEOUT                     (3 * 60 * 1000)	//mili second

/**
 * @brief Websocket event handler select() timeout value, msec.
		  Don't set this 0, it will be busy waiting.
 */
#define WEBSOCKET_HANDLER_TIMEOUT                    (100)	//mili second

/**
 * @brief Ping message limits without pong.
 */
#define WEBSOCKET_MAX_PING_IGNORE                    (3)	//times
/**
 * @brief Websocket ping message interval, MSEC_PER_TICK.
 */
#define WEBSOCKET_PING_INTERVAL                      (20 * 100)	//MSEC_PER_TICK

/**
 * @brief The maximum length of websocket messages to be stored in queue.
 */
#define WEBSOCKET_MAX_LENGTH_QUEUE                      (100 * 1024)
/**
 * @brief The maximum amount of websocket messages to be stored in queue.
 */
#define WEBSOCKET_MAX_NUMBER_QUEUE                      (20)

/**
 * @brief The maximum amount of client to be accepted in server.
 */
#define WEBSOCKET_MAX_CLIENT                         (3)

/**
 * @brief The maximun retry of tls handshake.
 */
#define WEBSOCKET_MAX_TLS_HANDSHAKE                  (3)

/**
 * @brief Enable and disable the Server name indication(SNI)
 */
#define WEBSOCKET_CONF_CHECK_TLS_HOSTNAME            (0)	// 0- disable, 1- enable

/*
 *  Pre-definition variables from wslay below.
 *  just not to include wslay.h on application side.
 */

/**
 * @brief Websocket context pointer wrapper.
 */
#define websocket_context_ptr                        wslay_event_context_ptr

/**
 * @brief Websocket structure wrapper to carry call back pointers.
 */
#define websocket_cb_t                               struct wslay_event_callbacks

/**
 * @brief Websocket structure wrapper to send a frame.
 *
@verbatim
	The original structure of websocket_frame_t :
		struct wslay_event_msg {
			uint8_t opcode;                 //operation code
			const uint8_t *msg;             //received message
			size_t msg_length;              //message length
		};
@endverbatim
 */
#define websocket_frame_t                            struct wslay_event_msg

/**
 * @brief Websocket structure wrapper to send a fragmented frame.
 *
@verbatim
	The original structure of websocket_fragmented_frame_t :
		struct wslay_event_fragmented_msg {
			uint8_t opcode;                                 //operation code
			union wslay_event_msg_source source;            //source data to make message
			wslay_event_fragmented_msg_cb read_callback;    //callback to read fragmented frame.
		};
@endverbatim
 */
#define websocket_fragmented_frame_t                 struct wslay_event_fragmented_msg

/**
 * @brief Websocket structure wrapper to receive a message.
 *
@verbatim
	The original structure of websocket_on_msg_arg :
		struct wslay_event_on_msg_recv_arg {
			uint8_t rsv;                    //reserved bits: rsv = (RSV1 << 2) | (RSV2 << 1) | RSV3
			uint8_t opcode;                 //operation code
			const uint8_t *msg;             //received message
			size_t msg_length;              //message length
			uint16_t status_code;           //status code
		};
@endverbatim
 */
#define websocket_on_msg_arg                         struct wslay_event_on_msg_recv_arg

/**
 * @brief Websocket structure wrapper to receive a raw frame.
 *
@verbatim
	The original structure of websocket_recv_start_arg :
		struct wslay_event_on_frame_recv_start_arg {
			uint8_t fin;                    //fin bit. 1 is for final frame, or 0
			uint8_t rsv;                    //reserved bits: (RSV1 << 2) | (RSV2 << 1) | RSV3
			uint8_t opcode;                 //operation code
			uint64_t payload_length;        //payload length
		};
@endverbatim
 */
#define websocket_recv_start_arg                     struct wslay_event_on_frame_recv_start_arg

/**
 * @brief Websocket structure wrapper to receive a chunked frame.
 *
@verbatim
	The original structure of websocket_recv_chunk_arg :
		struct wslay_event_on_frame_recv_chunk_arg {
			const uint8_t *data;            //chunk of payload data
			size_t data_length;             //size of data
		};
@endverbatim
 */
#define websocket_recv_chunk_arg                     struct wslay_event_on_frame_recv_chunk_arg

/****************************************************************************
 * Macros
 ****************************************************************************/

/**
 * @brief a macro that checks the websocket frame is not a control frame.
 *
 *        if the websocket frame is not a control frame, then it will return 1, else 0
 */
#define WEBSOCKET_CHECK_NOT_CTRL_FRAME(x)            (!(((x) >> 3) & 1))

/**
 * @brief a macro that checks the websocket frame is a text frame.
 *
 *        if the websocket frame is a text frame, then it will return 1, else 0
 */
#define WEBSOCKET_CHECK_TEXT_FRAME(x)                (((uint8_t)(x)) == ((uint8_t)(WEBSOCKET_TEXT_FRAME)))

/**
 * @brief a macro that checks the websocket frame is a binary frame.
 *
 *        if the websocket frame is a binary frame, then it will return 1, else 0
 */
#define WEBSOCKET_CHECK_BINARY_FRAME(x)              (((uint8_t)(x)) == ((uint8_t)(WEBSOCKET_BINARY_FRAME)))

/**
 * @brief a macro that checks the websocket frame is a close frame.
 *
 *        if the websocket frame is a close frame, then it will return 1, else 0
 */
#define WEBSOCKET_CHECK_CTRL_CLOSE(x)                (((uint8_t)(x)) == ((uint8_t)(WEBSOCKET_CONNECTION_CLOSE)))

/**
 * @brief a macro that checks the websocket frame is a ping frame.
 *
 *        if the websocket frame is a ping frame, then it will return 1, else 0
 */
#define WEBSOCKET_CHECK_CTRL_PING(x)                 (((uint8_t)(x)) == ((uint8_t)(WEBSOCKET_PING)))

/**
 * @brief a macro that checks the websocket frame is a pong frame.
 *
 *        if the websocket frame is a pong frame, then it will return 1, else 0
 */
#define WEBSOCKET_CHECK_CTRL_PONG(x)                 (((uint8_t)(x)) == ((uint8_t)(WEBSOCKET_PONG)))

/****************************************************************************
 * Public Types
 ****************************************************************************/
/**
 * @brief websocket return definitions
 */
typedef enum {
	WEBSOCKET_SUCCESS,			///< SUCCESS
	WEBSOCKET_ALLOCATION_ERROR,	///< malloc fail
	WEBSOCKET_INIT_ERROR,		///< websocket context init fail
	WEBSOCKET_SOCKET_ERROR,		///< socket setting fail
	WEBSOCKET_CONNECT_ERROR,	///< socket connecting fail
	WEBSOCKET_HANDSHAKE_ERROR,	///< http handshake fail
	WEBSOCKET_SEND_ERROR,		///< message sending fail
	WEBSOCKET_RECEIVE_ERROR,	///< message reading fail
	WEBSOCKET_TLS_INIT_ERROR,	///< TLS context init fail
	WEBSOCKET_TLS_HANDSHAKE_ERROR	///< TLS handshake fail
} websocket_return_t;

/**
 * @brief websocket states
 */
enum websocket_state {
	WEBSOCKET_STOP,				///< stopped
	WEBSOCKET_ERROR,			///< websocket handler is stopped for an error.
	WEBSOCKET_RUN_CLIENT,		///< client is running
	WEBSOCKET_RUN_SERVER,		///< server is running
	WEBSOCKET_MAX_STATE			///< websocket number of max state.
};

/**
 * @brief websocket io flags
 */
enum websocket_io_flag {
	WEBSOCKET_MSG_MORE = 1
};

/**
 * @brief websocket socket error codes
 *
 * this error codes are used in websocket_set_error function
 */
enum websocket_socket_error {
	WEBSOCKET_ERR_WANT_READ = -100,
	WEBSOCKET_ERR_WANT_WRITE = -101,
	WEBSOCKET_ERR_PROTO = -200,
	WEBSOCKET_ERR_INVALID_ARGUMENT = -300,
	WEBSOCKET_ERR_INVALID_CALLBACK = -301,
	WEBSOCKET_ERR_NO_MORE_MSG = -302,
	WEBSOCKET_ERR_CALLBACK_FAILURE = -400,
	WEBSOCKET_ERR_WOULDBLOCK = -401,
	WEBSOCKET_ERR_NOMEM = -500
};

/**
 * @brief websocket operation code.
 *
 * Operation codes are sent with a message
 */
enum websocket_opcode {
	WEBSOCKET_CONTINUATION_FRAME = 0x0,	///< a fragmented frame
	WEBSOCKET_TEXT_FRAME = 0x1,	///< a normal websocket frame
	WEBSOCKET_BINARY_FRAME = 0x2,	///< a binary websocket frame
	WEBSOCKET_CONNECTION_CLOSE = 0x8,	///< a control frame to close websocket
	WEBSOCKET_PING = 0x9,		///< a control frame to ping
	WEBSOCKET_PONG = 0xa		///< a control frame to pong
};

/**
 * @brief structure to manage websocket client/server
 */
typedef struct {
	int state;
///< 0 - closed / 1 - running client / 2 - running server
	int fd;
///< Each server or client has own socket fd
	int tls_enabled;
///< TLS enable flag (1 - enable, 0 - disable)
	int ping_cnt;
///< Counter for ping message without receiving pong
	websocket_context_ptr ctx;
///< Websocket context to manage event queue, errors and configurations
	websocket_cb_t *cb;
///< Each server of client should define callbacks to use websocket functions
	mbedtls_net_context tls_net;
///< User data set by the calling application
	void *user_data;
///< Contain network file descriptor
	mbedtls_ssl_context *tls_ssl;
///< Handling all tls context informations
	mbedtls_ssl_config *tls_conf;
///< Authentication mode to use when doing TLS
	int auth_mode;
///< TLS Initial configuration structure
	struct work_s time_worker;
///< Ping timer structure
	pthread_t thread_id;
///< Websocket event handler thread ID
	pthread_attr_t thread_attr;
///< Websocket event handler thread attribute
} websocket_t;

/**
 * @brief data structure to use in callbacks.
 *
 *        websocket callbacks get this structure as a parameter.
 *        so users should modify this structure if necessary.
 */
struct websocket_info_t {
	websocket_t *data;
};

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

#ifdef __cplusplus
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif

/**
 * @brief websocket_create_accept_key
 *
 *        This function makes encrypted key for websocket http handshake.
 * @param[out] dst created accept key is stored to destination buffer.
 * @param[in] dlen destination buffer length.
 * @param[in] src source to create accept key.
 * @param[in] slen source buffer length.
 * @return none
 * @since TizenRT v1.0
 */
void websocket_create_accept_key(unsigned char *dst, size_t dlen, const unsigned char *src, size_t slen);

/**
 * @brief websocket_count_table() returns amount of working websocket server.
 * @param[in] none.
 * @return return amount of working websocket server
 * @since TizenRT v1.0
 */
int websocket_count_table(void);

/**
 * @brief websocket_find_table() returns websocket_t * to manage websocket server.
 *
 *        This function returns websocket_t * to manage websocket server.\n
 *        The maximum number of client that websocket can handle is defined WEBSOCKET_MAX_CLIENT.
 * @param[in] none.
 * @return On success, return websocket_t *. On failure, return NULL.
 * @since TizenRT v1.0
 */
websocket_t *websocket_find_table(void);

/**
 * @brief websocket_client_open() initiates websocket client ready to send and receive websocket message.
 *
 *        This function offers steps to connect socket, enable TLS by option,
 *        http handshake and init websocket context.
 * @param[in] client websocket structure manages file descriptor, websocket context and TLS context.
 *               users must give a pointer of websocket callback structure in websocket_t *client
 * @param[in] addr websocket server ip address.
 * @param[in] port websocket server port number. usually 80 is normal websocket, 443 is secured websocket.
 * @param[in] path uri path.
 * @return On success, return WEBSOCKET_SUCCESS. On failure, return values defined in websocket_return_t.
 * @since TizenRT v1.0
 */
websocket_return_t websocket_client_open(websocket_t *client, char *addr, char *port, char *path);

/**
 * @brief websocket_server_open() initiates websocket server.
 *
 *        This function offers steps to accept client socket, enable TLS by option,
 *        http handshake and calls websocket_server_init() to start message handling loop.\n
 *        websocket server uses callbacks that users defined to initiate websocket server.
 * @param[in] server websocket strucutre manages file descriptor, websocket context and TLS context.
 *               users must give a pointer of websocket callback structure in websocket_t *server
 * @return On success, return WEBSOCKET_SUCCESS. On failure, return values defined in websocket_return_t.
 * @since TizenRT v1.0
 */
websocket_return_t websocket_server_open(websocket_t *server);

/**
 * @brief websocket_server_init
 *
 *        This function start message handling loop.\n
 *        It initiates websocket context structure and select() fd to handle the messages.
 * @param[in] server websocket structure manages file descriptor, websocket context and TLS context.
 *               users must give a pointer of websocket callback structure in websocket_t *server
 * @return On success, return WEBSOCKET_SUCCESS. On failure, return values defined in websocket_return_t.
 * @since TizenRT v1.0
 */
websocket_return_t websocket_server_init(websocket_t *server);

/**
 * @brief websocket_register_cb() changes a websocket callback structure in a websocket context.
 *
 *        Websocket callbacks are basically registered when initiation,
 *        but users can change callbacks with this function.
 * @param[in] websocket websocket structure to store callbacks.
 * @param[in] cb callbacks structure pointer to change
 * @return On success, return WEBSOCKET_SUCCESS. On failure, return values defined in websocket_return_t.
 * @since TizenRT v1.0
 */
websocket_return_t websocket_register_cb(websocket_t *websocket, websocket_cb_t *cb);

/*
 * @brief websocket_queue_msg() queues a message into websocket context.
 *
 *        Websocket has a event handler thread and it keeps watching the queue to send or recv events.
 * @param[in] ctx message queue is in websocket context
 * @param[in] frame message frame to be sent
 * @return On success return WEBSOCKET_SUCCESS, On failure return values defined in websocket_return_t
 * @since TizenRT v1.0
 */
websocket_return_t websocket_queue_msg(websocket_t *websocket, websocket_frame_t *tx_frame);

/**
 * @brief websocket_queue_ping() is used to send a websocket ping message.
 *
 *        This function has following steps to send websocket frame.\n
 *        It contains queueing a message, triggering the websocket send event
 *        and sending through socket file descriptor.
 * @param[in] websocket websocket structure manages websocket context.
 * @return On success, return WEBSOCKET_SUCCESS. On failure, return values defined in websocket_return_t.
 * @since TizenRT v1.0
 */
websocket_return_t websocket_queue_ping(websocket_t *websocket);

/**
 * @brief websocket_queue_close() closes the websocket.
 *
 *        This function doesn't guarantee the current packet transfer.\n
 *        As it is control frame, it has more priority than non-control frames.\n
 *        This means users need to manage the packet transfer status before calling this function.
 * @param[in] websocket websocket structure manages websocket context.
 * @param[in] close_message close message to be sent.
 * @return On success, return WEBSOCKET_SUCCESS. On failure, return values defined in websocket_return_t.
 * @since TizenRT v1.0
 */
websocket_return_t websocket_queue_close(websocket_t *websocket, const char *close_message);

/**
 * @brief websocket_update_state() changes websocket state.
 *
 *        This function is used to control websocket state.\n
 *        websocket state are defined at websocket_state.
 * @param[in] websocket websocket structure.
 * @param[in] state websocket state.
 * @return On success, return WEBSOCKET_SUCCESS. On failure, return values defined in websocket_return_t.
 * @since TizenRT v1.0
 */
websocket_return_t websocket_update_state(websocket_t *websocket, int state);

/**
 * @brief websocket_set_error() sets an error number to websocket context.
 *
 *        This function is used to notice serious error to websocket context.\n
 *        After this function triggered, websocket context will be closed.
 * @param[in] websocket websocket structure manages websocket context.
 * @param[in] val error value to be set.
 * @return On success, return WEBSOCKET_SUCCESS. On failure, return values defined in websocket_return_t.
 * @since TizenRT v1.0
 */
websocket_return_t websocket_set_error(websocket_t *websocket, int val);

#undef EXTERN
#ifdef __cplusplus
}
#endif

/** @} */// end of Websocket group

#endif							/* __EXTERNAL_INCLUDE_PROTOCOLS_WEBSOCKET_H */
