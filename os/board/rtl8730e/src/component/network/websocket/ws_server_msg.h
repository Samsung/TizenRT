#ifndef _WS_SERVER_MSG_H_
#define _WS_SERVER_MSG_H_
#include <websocket/wsserver_api.h>

struct ws_data_header_type {
	size_t header_size;
	int fin;
	int mask;
	enum opcode_type opcode;
	int N0;
	uint64_t N;
	uint8_t masking_key[4];
};

#define WS_SERVER_SEND_BY_POLL			0    /*!< WS server send data when polling */
#define WS_SERVER_SEND_DIRECTLY			1    /*!< WS server send data directly */

void *ws_server_malloc(size_t size);

void ws_server_free(void *ptr);

int ws_server_write(ws_conn *conn, uint8_t *buf, size_t buf_len);

int ws_server_read(ws_conn *conn, uint8_t *buf, size_t buf_len);

void ws_server_response_too_many_requests(ws_conn *conn, char *msg);

void ws_server_response_bad_request(ws_conn *conn, char *msg);

int ws_server_handshake_read_header(ws_conn *conn);

int ws_server_handshake_response(ws_conn *conn);

void ws_server_sendData(uint8_t type, size_t message_size, uint8_t *message, int useMask, uint8_t send_mode, ws_conn *conn);

void ws_server_dispatchBinary(ws_conn *conn);

void ws_server_conn_remove(ws_conn *conn);

#endif
