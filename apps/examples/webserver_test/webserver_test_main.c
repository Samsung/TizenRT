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
#include <sys/types.h>
#include <sys/socket.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include "webserver_test_util.h"
#include "webserver_test_tls.h"

typedef struct ws_test_s {
	/* Internal status */
	uint8_t state;
	uint8_t httpstatus;
	uint16_t port; /* The port number to use in the connection */
	char *buffer; /* user-provided buffer */
	int buflen; /* Length of the user provided buffer */
	int offset; /* Offset to the beginning of interesting data */
	int datend; /* Offset+1 to the last valid byte of data in the buffer */
	/* Buffer HTTP header data and parse line at a time */
	char hostname[WS_TEST_MAXHOSTNAME];
} ws_test_s;

static int g_normal_clen = 0;
static int g_last_clen = 0;
static int g_tc_pass = 0;

//format
static void ws_test_dump_usage(void)
{
	FUNC_EN;
	printf("\n  webserver_test usage:\n");
	printf("   $ webserver_test <iterations> <method> <uri> [options...] \n");
	printf("\n");
	printf(" <method>   : %%s (GET, PUT, POST, DELETE)\n");
	printf(" <uri>      : %%s (Host address : should be started with http:// or https://)\n");
	printf("\n [options...] \n");
	printf(" test_entity=SIZE      Test entity size (default is 0)\n");
	printf(" chunk-count=Y    	   Turns on Chunked-Transfer and sends Y chunks of test_entity (default Chunked-Transfer is OFF)\n");
	printf(" keep-alive=X          Turns on keep-alive and sends X requests (default Keep-Alive is OFF)\n");
	printf("\n  example:\n");
	printf("   $ webserver_test 3 GET https://127.0.0.1/\n");
	printf("   $ webserver_test 3 GET https://127.0.0.1/ test_entity=5\n");
	FUNC_EX;
}

//retrives IP address
static int ws_test_gethostip(char *hostname, in_addr_t *ipv4addr)
{
	FUNC_EN;
	struct hostent *he;
	he = gethostbyname(hostname);
	if (he == NULL) {
		PRNT("gethostbyname failed");
		return WS_TEST_ERR;
	} else if (he->h_addrtype != AF_INET) {
		PRNT("gethostbyname returned an address of type: %d", he->h_addrtype);
		return WS_TEST_OK;
	}

	memcpy(ipv4addr, he->h_addr, sizeof(in_addr_t));
	FUNC_EX;
	return WS_TEST_OK;
}

// adds method, headers and entity to the request body
static int ws_test_msg_construct(char *buf, struct http_client_request_t *param, struct ws_test_s *ws)
{
	FUNC_EN;
	int post_len;
	char *dest = buf;
	char *temp;
	char post_size[8];
	struct http_keyvalue_t *cur = NULL;

	/*----------------- Send method --------------------*/
	if (param->method == WS_TEST_MODE_GET) {
		dest = ws_test_strcpy(dest, HTTPGET, param);
	} else if (param->method == WS_TEST_MODE_POST) {
		dest = ws_test_strcpy(dest, HTTPPOST, param);
	} else if (param->method == WS_TEST_MODE_PUT) {
		dest = ws_test_strcpy(dest, HTTPPUT, param);
	} else if (param->method == WS_TEST_MODE_DELETE) {
		dest = ws_test_strcpy(dest, HTTPDELETE, param);
	}
	if (dest == NULL) {
		return WS_TEST_MSG_CONSTRUCT_ERR;
	}

	dest = ws_test_strcpy(dest, "/\0", param);
	if (dest == NULL) {
		return WS_TEST_MSG_CONSTRUCT_ERR;
	}

	*dest++ = ISO_space;
	dest = ws_test_strcpy(dest, HTTP11, param);
	if (dest == NULL) {
		return WS_TEST_MSG_CONSTRUCT_ERR;
	}
	dest = ws_test_strcpy(dest, HTTPCRNL, param);
	if (dest == NULL) {
		return WS_TEST_MSG_CONSTRUCT_ERR;
	}
	dest = ws_test_strcpy(dest, HTTPHOST, param);
	if (dest == NULL) {
		return WS_TEST_MSG_CONSTRUCT_ERR;
	}
	dest = ws_test_strcpy(dest, ws->hostname, param);
	if (dest == NULL) {
		return WS_TEST_MSG_CONSTRUCT_ERR;
	}
	dest = ws_test_strcpy(dest, HTTPCRNL, param);
	if (dest == NULL) {
		return WS_TEST_MSG_CONSTRUCT_ERR;
	}

	/*------------------------- Header of entity -----------------------*/

	if (param->method == WS_TEST_MODE_POST || param->method == WS_TEST_MODE_PUT) {
		/* Look for Content-Type in the headers */
		cur = param->headers->head->next;
		while (cur != param->headers->tail) {
			if (!strncmp(cur->key, "Content-Type", strlen("Content-Type")+1)) {
				break;
			}
			cur = cur->next;
		}

		/* Add default Content-Type if not found in headers */
		if (cur == param->headers->tail) {
			dest = ws_test_strcpy(dest, HTTPFORM, param);
			if (dest == NULL) {
				return WS_TEST_MSG_CONSTRUCT_ERR;
			}
			dest = ws_test_strcpy(dest, HTTPCRNL, param);
			if (dest == NULL) {
				return WS_TEST_MSG_CONSTRUCT_ERR;
			}
		}

		/* content length */
		if (!param->encoding) {
			dest = ws_test_strcpy(dest, HTTPCONTENTSIZE, param);
			if (dest == NULL) {
				return WS_TEST_MSG_CONSTRUCT_ERR;
			}
			post_len = strlen((char *)param->entity);
			sprintf(post_size, "%d", post_len);
			dest = ws_test_strcpy(dest, post_size, param);
			if (dest == NULL) {
				return WS_TEST_MSG_CONSTRUCT_ERR;
			}
			dest = ws_test_strcpy(dest, HTTPCRNL, param);
			if (dest == NULL) {
				return WS_TEST_MSG_CONSTRUCT_ERR;
			}
		} else { /* chunked param->encoding */
			dest = ws_test_strcpy(dest, HTTPCHUNKED, param);
			if (dest == NULL) {
				return WS_TEST_MSG_CONSTRUCT_ERR;
			}
			dest = ws_test_strcpy(dest, HTTPCRNL, param);
			if (dest == NULL) {
				return WS_TEST_MSG_CONSTRUCT_ERR;
			}
			if (param->trailer) {
				dest = ws_test_strcpy(dest, TRAILERHEADER, param);
				if (dest == NULL) {
					return WS_TEST_MSG_CONSTRUCT_ERR;
				}
				dest = ws_test_strcpy(dest, HTTPCRNL, param);
				if (dest == NULL) {
					return WS_TEST_MSG_CONSTRUCT_ERR;
				}
			}
		}
	}

	/*------------------------ Adding the remaining headers ---------------------------*/

	cur = param->headers->head->next;
	while (cur != param->headers->tail) {
		dest = ws_test_strcpy(dest, cur->key, param);
		if (dest == NULL) {
			return WS_TEST_MSG_CONSTRUCT_ERR;
		}
		dest = ws_test_strcpy(dest, ": ", param);
		if (dest == NULL) {
			return WS_TEST_MSG_CONSTRUCT_ERR;
		}
		dest = ws_test_strcpy(dest, cur->value, param);
		if (dest == NULL) {
			return WS_TEST_MSG_CONSTRUCT_ERR;
		}
		dest = ws_test_strcpy(dest, HTTPCRNL, param);
		if (dest == NULL) {
			return WS_TEST_MSG_CONSTRUCT_ERR;
		}
		cur = cur->next;
	}
	dest = ws_test_strcpy(dest, HTTPCRNL, param);
	if (dest == NULL) {
		return WS_TEST_MSG_CONSTRUCT_ERR;
	}

	/*----------------------------- Adding Entity for POST & PUT -------------------------------------*/

	if (param->method == WS_TEST_MODE_POST || param->method == WS_TEST_MODE_PUT) {
		/* content length */
		if (!param->encoding) {
			dest = ws_test_strcpy(dest, (char *)param->entity, param);
			if (dest == NULL) {
				return WS_TEST_MSG_CONSTRUCT_ERR;
			}
		} else { /* chunked param->encoding */
			post_len = strlen(param->entity);
			if (post_len > param->buflen) {
				dest = ws_test_chunksize(dest, param->buflen, param);
				if (dest == NULL) {
					return WS_TEST_MSG_CONSTRUCT_ERR;
				}
				dest = ws_test_strcpy(dest, HTTPCRNL, param);
				if (dest == NULL) {
					return WS_TEST_MSG_CONSTRUCT_ERR;
				}
				dest = ws_test_strlencpy(dest, (char *)param->entity, param->buflen, param);
				if (dest == NULL) {
					return WS_TEST_MSG_CONSTRUCT_ERR;
				}
				dest = ws_test_strcpy(dest, HTTPCRNL, param);
				if (dest == NULL) {
					return WS_TEST_MSG_CONSTRUCT_ERR;
				}
			} else {
				temp = dest;
				dest = ws_test_chunksize(dest, post_len, param);
				if (dest == NULL) {
					return WS_TEST_MSG_CONSTRUCT_ERR;
				}
				dest = ws_test_strcpy(dest, HTTPCRNL, param);
				if (dest == NULL) {
					return WS_TEST_MSG_CONSTRUCT_ERR;
				}
				dest = ws_test_strcpy(dest, (char *)param->entity, param);
				if (dest == NULL) {
					return WS_TEST_MSG_CONSTRUCT_ERR;
				}
				dest = ws_test_strcpy(dest, HTTPCRNL, param);
				if (dest == NULL) {
					return WS_TEST_MSG_CONSTRUCT_ERR;
				}
				g_normal_clen = dest - temp;
				dest = ws_test_strlencpy(dest, "0", 1, param);
				if (dest == NULL) {
					return WS_TEST_MSG_CONSTRUCT_ERR;
				}
				dest = ws_test_strcpy(dest, HTTPCRNL, param);
				if (dest == NULL) {
					return WS_TEST_MSG_CONSTRUCT_ERR;
				}
				if (param->trailer) {
					dest = ws_test_strcpy(dest, EXPIRES, param);
					if (dest == NULL) {
						return WS_TEST_MSG_CONSTRUCT_ERR;
					}
				}
				dest = ws_test_strcpy(dest, HTTPCRNL, param);
				if (dest == NULL) {
					return WS_TEST_MSG_CONSTRUCT_ERR;
				}
				g_last_clen = dest - temp;
			}
		}
	}
	FUNC_EX;
	return dest - param->buffer;
}

// to make socket connection
static int ws_test_socket_connect(struct ws_test_s *ws)
{
	FUNC_EN;
	int sockfd, ret;
	struct timeval tv;
	struct sockaddr_in server;

	/* Create a socket */
	sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sockfd < 0) {
		return WS_TEST_SOCKET_CONNECT_ERR;
	}

	/* Set send and receive timeout values */
	tv.tv_sec = WS_TEST_CONF_TIMEOUT_MSEC / 1000;
	tv.tv_usec = (WS_TEST_CONF_TIMEOUT_MSEC % 1000) * 1000;
	PRNT("recv timeout is set to (%d.%d)sec", tv.tv_sec, tv.tv_usec);
	ret = setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO,
				   (struct timeval *)&tv, sizeof(struct timeval));
	if (ret < 0) {
		PRNT("ERROR: setsockopt failed %d", ret);
		return WS_TEST_SOCKET_CONNECT_ERR;
	}

	/* Get the server address from the host name */
	server.sin_family = AF_INET;
	server.sin_port = htons(ws->port);
	ret = ws_test_gethostip(ws->hostname, &server.sin_addr.s_addr);
	if (ret < 0) {
		/* Could not resolve host (or malformed IP address) */
		PRNT("ERROR: Failed to resolve hostname %d", ret);
		close(sockfd);
		return WS_TEST_SOCKET_CONNECT_ERR;
	}

	/* Connect to server.  First we have to set some fields in the
	 * 'server' address structure.  The system will assign me an arbitrary
	 * local port that is not in use.
	 */
	ret = connect(sockfd, (struct sockaddr *)&server, sizeof(struct sockaddr_in));
	if (ret < 0) {
		PRNT("ERROR: connect failed: %d errno: %d", ret, errno);
		close(sockfd);
		return WS_TEST_SOCKET_CONNECT_ERR;
	}
	FUNC_EX;
	return sockfd;
}

// to perform chunk transfer
static int ws_test_chunk_transfer(struct http_client_request_t *param, 
							int send_len, int sockfd, 
							struct http_client_tls_t *client_tls) {
	FUNC_EN;
	int ret, cbuf, chunks = param->chunk_count - 1;
	//send headers
	int buf_len = 0;
	int sndlen = send_len-g_last_clen;
	while (sndlen > 0) {
		if (param->tls) {
			ret = mbedtls_ssl_write(&(client_tls->tls_ssl),
									(const unsigned char *)param->buffer + buf_len,
									sndlen);
		} else {
			ret = send(sockfd, param->buffer + buf_len, sndlen, 0);
		}
		if (ret < 1) {
			PRNT("ERROR: send failed: %d", ret);
			return WS_TEST_ERR;
		} else {
			sndlen -= ret;
			buf_len += ret;
			PRNT("SEND SUCCESS: send %d bytes", ret);
		}
	}
	//send chunks
	cbuf = buf_len;
	while(chunks--) {
		buf_len = cbuf;
		sndlen = g_normal_clen;
		while (sndlen > 0) {
			if (param->tls) {
				ret = mbedtls_ssl_write(&(client_tls->tls_ssl),
										(const unsigned char *)param->buffer + buf_len,
										sndlen);
			} else {
				ret = send(sockfd, param->buffer + buf_len, sndlen, 0);
			}
			if (ret < 1) {
				PRNT("ERROR: send failed: %d", ret);
				return WS_TEST_ERR;
			} else {
				sndlen -= ret;
				buf_len += ret;
				PRNT("SEND SUCCESS: send %d bytes", ret);
			}
		}
	}
	//last chunk
	buf_len = cbuf;
	sndlen = g_last_clen;
	while (sndlen > 0) {
		if (param->tls) {
			ret = mbedtls_ssl_write(&(client_tls->tls_ssl),
									(const unsigned char *)param->buffer + buf_len,
									sndlen);
		} else {
			ret = send(sockfd, param->buffer + buf_len, sndlen, 0);
		}
		if (ret < 1) {
			PRNT("ERROR: send failed: %d", ret);
			return WS_TEST_ERR;
		} else {
			sndlen -= ret;
			buf_len += ret;
			PRNT("SEND SUCCESS: send %d bytes", ret);
		}
	}
	return WS_TEST_OK;
	FUNC_EX;
}

// to make connections and perform tls handshakes
static int ws_test_connect_func(struct ws_test_s *ws, int handshake_retry,
							struct http_client_tls_t *client_tls, 
							struct http_client_request_t *param) {
	FUNC_EN;
	int sockfd = -1;
	int ret = 0;						
	while(handshake_retry-- > 0) {
		if ((sockfd = ws_test_socket_connect(ws)) < 0) {
			PRNT("ERROR: socket failed: %d", errno);
			if (param->tls) {
				ws_test_tls_release(client_tls);
			}

			free(client_tls);
			if (!param->tls && sockfd > 0) {
				close(sockfd);
			}
			return WS_TEST_ERR;
		}
		client_tls->client_fd = sockfd;
		if (param->tls && (ret = ws_test_tls_handshake(client_tls, ws->hostname))) {
			if (ret == MBEDTLS_ERR_NET_SEND_FAILED ||
				ret == MBEDTLS_ERR_NET_RECV_FAILED ||
				ret == MBEDTLS_ERR_SSL_CONN_EOF) {
				PRNT("Handshake again.... ");
				mbedtls_net_free(&(client_tls->tls_client_fd));
				mbedtls_ssl_free(&(client_tls->tls_ssl));
				close(sockfd);
			}
		}
		if (ret == 0) {
			return sockfd;
		}
	}

	if (param->callback && param->response) {
		ws_test_http_client_response_release(param->response);
	}
	if (param->tls) {
		ws_test_tls_ssl_release(client_tls);
	}
	if (param->tls) {
		ws_test_tls_release(client_tls);
	}

	free(client_tls);
	if (!param->tls && sockfd > 0) {
		close(sockfd);
	}
	FUNC_EX;
	return WS_TEST_ERR;
}

// makes connection with server and sends requests accordingly
static int ws_test_base(void *arg)
{
	FUNC_EN;
	int sockfd = -1;
	int ret;
	int buf_len, sndlen, len;
	int state = HTTP_REQUEST_HEADER;
	struct http_message_len_t mlen = {0,};
	struct ws_test_s ws;
	struct http_client_request_t *param = (struct http_client_request_t *)arg;
	struct http_client_response_t response = {0, };
	int read_finish = false;
	int send_len;

	struct http_client_tls_t *client_tls = (struct http_client_tls_t *)malloc(sizeof(struct http_client_tls_t));
	int handshake_retry = WS_TEST_CONF_HANDSHAKE_RETRY;

	if (param->tls && client_tls == NULL) {
		PRNT("memory not allocated");
		return WS_TEST_ERR;
	}

	/* Initialize the state structure */
	memset(&ws, 0, sizeof(struct ws_test_s));
	ws.buffer = param->buffer;
	ws.buflen = param->buflen;

	/* Parse the hostname from the URL */
	ret = parsehttpurl(param->url, &ws.port, ws.hostname, WS_TEST_MAXHOSTNAME);
	if (ret != 0) {
		PRNT("ERROR: Malformed HTTP URL: %s", param->url);
		free(client_tls);
		return WS_TEST_ERR;
	}

	PRNT("hostname='%s'", ws.hostname);

	if (param->tls && ws_test_tls_init(client_tls, &param->ssl_config)) {
		PRNT("Fail to client tls init");
		goto errout_before_tlsinit;
	}

	ws.httpstatus = HTTPSTATUS_NONE;
	ws.offset = 0;

	if ((sndlen = ws_test_msg_construct(ws.buffer, param, &ws)) < 0) {
		PRNT("ERROR: construction message failed");
		goto errout_before_tlsinit;
	}

	PRNT("sndlen:%d , param->buffer is:\n%s", sndlen,param->buffer);

	if((sockfd = ws_test_connect_func(&ws, handshake_retry, client_tls, param)) < 0) {
		return WS_TEST_ERR;
	}

	send_len = sndlen;
	for (int i = 0; i < param->keep_alive_count; i++) {
		read_finish = false;
		memset(&mlen, 0, sizeof(mlen));
		state = HTTP_REQUEST_HEADER;

		if (!param->encoding) {
			buf_len = 0;
			sndlen = send_len;
			while (sndlen > 0) {
				if (param->tls) {
					ret = mbedtls_ssl_write(&(client_tls->tls_ssl),
											(const unsigned char *)param->buffer + buf_len,
											sndlen);
				} else {
					ret = send(sockfd, param->buffer + buf_len, sndlen, 0);
				}
				if (ret < 1) {
					PRNT("ERROR: send failed: %d", ret);
					goto errout;
				} else {
					sndlen -= ret;
					buf_len += ret;
					PRNT("SEND SUCCESS: send %d bytes", ret);
				}
			}
		} else {
			if(ws_test_chunk_transfer(param, send_len, sockfd, client_tls) == WS_TEST_ERR) {
				goto errout;
			}
		}

		if (param->callback && param->response == NULL) {
			param->response = &response;
			if (ws_test_http_client_response_init(param->response) < 0) {
				PRNT("ERROR: response init failed: %d", ret);
				param->response = NULL;
				goto errout;
			}
		}
		int loopcount = 0;
		int buf_msg_len = 0;

		len = 0;

		if (param->response->message == NULL) {
			PRNT("Error: message is NULL \n");
			goto errout;
		}
		memset(param->response->message, 0, WS_TEST_CONF_MAX_MESSAGE_SIZE);

		while (!read_finish) {

			if (WS_TEST_CONF_MAX_MESSAGE_SIZE - buf_msg_len <= 0) {
				PRNT("Error: Received message size is too large!!\n");
				goto errout;
			}

			PRNT("Receive start \n");
			if (param->tls) {
				len = mbedtls_ssl_read(&(client_tls->tls_ssl),
									   (unsigned char *)param->response->message,
									   WS_TEST_CONF_MAX_MESSAGE_SIZE);
			} else {
				len = recv(sockfd, param->response->message + buf_msg_len,
						   WS_TEST_CONF_MAX_MESSAGE_SIZE - buf_msg_len, 0);
			}

			if (len < 0) {
				PRNT("Error: Receive Fail %d", len);
				goto errout;
			} else if (len == 0) {
				PRNT("Finish read");
				if (mlen.message_len - mlen.sentence_start == mlen.content_len) {
					PRNT("download completed successfully");
					break;
				} else {
					PRNT("Error: Receive Fail");
					goto errout;
				}
			}

			buf_msg_len += len;
			read_finish = parse_message(param->response->message, buf_msg_len, param->response->url,
							 			&param->response->entity, &state, &mlen, param->response);
			++loopcount;
			PRNT("====== loopcount : %d read_finish : %d=====", loopcount, read_finish);
			if (read_finish == WS_TEST_ERR) {
				PRNT("Error: Parse message Fail");
				goto errout;
			}

			param->response->method = param->method;
			param->response->url = param->url;

			if (param->callback && param->response->entity_len != 0) {
				param->callback(param->response);
			}
		}
	}
	if (param->callback) {
		ws_test_http_client_response_release(param->response);
	}
	
	if (param->tls) {
		ws_test_tls_release(client_tls);
		ws_test_tls_ssl_release(client_tls);
	}
	free(client_tls);
	if (!param->tls) {
		close(sockfd);
	}

	FUNC_EX;
	return WS_TEST_OK;

errout:
	if (param->callback && param->response) {
		ws_test_http_client_response_release(param->response);
	}
	if (param->tls) {
		ws_test_tls_ssl_release(client_tls);
	}
errout_before_tlsinit:
	if (param->tls) {
		ws_test_tls_release(client_tls);
	}

	free(client_tls);
	if (!param->tls && sockfd > 0) {
		close(sockfd);
	}

	FUNC_EX;
	return WS_TEST_ERR;
}

//validates the request and passes it to ws_test_base() for sending
static int ws_test_http_client_send_request(struct http_client_request_t *request,
		 void *ssl_config, struct http_client_response_t *response, wget_callback_t cb)
{
	FUNC_EN;
	int res = WS_TEST_ERR;
	struct mallinfo data;
	struct http_client_ssl_config_t *ssl_conf = ssl_config;

	if (request == NULL) {
		PRNT("Error: Request is null");
		goto ret_error;
	}

	if (request->url == NULL) {
		PRNT("Error: URL is NULL!!");
		goto ret_error;
	}

	if (request->method < WS_TEST_MODE_GET || request->method > WS_TEST_MODE_DELETE) {
		PRNT("Error: Incorrect method value!!");
		goto ret_error;
	}

	if (request->encoding != HTTP_CONTENT_LENGTH && request->encoding != HTTP_CHUNKED_ENCODING) {
		PRNT("Error: Incorrect encoding value!!");
		goto ret_error;
	}

	request->buffer = (char *)zalloc(request->buflen);
	if (request->buffer == NULL) {
		PRNT("Error: Fail to allocate buffer");
		goto ret_error;
	}

	if (request->buflen <= 0) {
		PRNT("Error: Buffer length must be bigger than 0!!");
		return WS_TEST_ERR;
	}

	request->tls = false;
	request->response = response;
	request->callback = cb;


	if (ssl_conf) {
		request->tls = true;
		memcpy(&request->ssl_config, ssl_conf, sizeof(struct http_client_ssl_config_t));

		data = mallinfo();
		if (data.fordblks < WS_TEST_CONF_MIN_TLS_MEMORY) {
			PRNT("Error: Not enough memory!!");
			goto ret_error;
		}
	}

	res = ws_test_base((void *)request);
	if (res != 0) {
		PRNT("ws_test_base() failed %d", res);
	}

	free(request->buffer);
	FUNC_EX;
	return res;

ret_error:
	FUNC_EX;
	return WS_TEST_ERR;
}

//takes in user arguments and accordingly creates the request
static int ws_test_prepare_request(int argc, char** argv, struct http_client_request_t *request)
{
	FUNC_EN;
	char *p, *q;
	memset(request, 0, sizeof(struct http_client_request_t));
	request->keep_alive_count = 1;

	if (!strncmp(argv[2], "GET", 4)) {
		request->method = WS_TEST_MODE_GET;
	} else if (!strncmp(argv[2], "PUT", 4)) {
		request->method = WS_TEST_MODE_PUT;
	} else if (!strncmp(argv[2], "POST", 5)) {
		request->method = WS_TEST_MODE_POST;
	} else if (!strncmp(argv[2], "DELETE", 7)) {
		request->method = WS_TEST_MODE_DELETE;
	} else {
		PRNT("invalid req_type");
		return WS_TEST_ERR;
	}

	request->url = argv[3];

	if (!strncmp(request->url, "https", 5)) {
		request->https = 1;
	} else

	if (!strncmp(request->url, "http", 4)) {
		request->https = 0;
	} else {
		PRNT("issue with url");
		return WS_TEST_ERR;
	}

	for (int i = 4; i < argc; i++) {
		p = argv[i];
		if ((q = strchr(p, '=')) == NULL) {
			PRNT("issue in optional args, '=' not found");
			return WS_TEST_ERR;
		}
		q++;

		if (strncmp(p, "test_entity", 11) == 0) {
			int t = atoi(q);
			if (t > 0 && t <= WS_TEST_CONF_MAX_ENTITY_SIZE) {
				request->entity = (char *)malloc(t+1);
				if (request->entity == NULL) {
					PRNT("no memory allocated");
					return WS_TEST_ERR;
				}
				request->is_test_entity = 1;
				memset(request->entity, '1', t);
				request->entity[t] = '\0';
			} else {
				PRNT("%d is out of range",t);
				return WS_TEST_ERR;
			}
		}
		else  if (strncmp(p, "keep-alive", 10) == 0) {
			request->keep_alive_count = atoi(q);
		}
		else if (strncmp(p, "chunk-count", 11) == 0) {
			request->encoding = 1;
			request->chunk_count = atoi(q);
		}
		else if (strncmp(p, "trailer", 7) == 0) {
			request->trailer = atoi(q);
		}
		else {
			PRNT("issue with optional args");
			return WS_TEST_ERR;
		}
	}

	request->buflen = WS_TEST_BUF_SIZE;
	FUNC_EX;
	return 0;
}

//calls for sending the request and checks if response has status 200
static int ws_test_process_query(int argc, char** argv, int result,
								struct http_client_request_t request,
								struct http_keyvalue_list_t headers,
								struct http_client_ssl_config_t *ssl_config)
{
	FUNC_EN;
	struct http_client_response_t response;

	// before sending request, must initialize response
	if (ws_test_http_client_response_init(&response) < 0) {
		PRNT("fail to init response");
		goto release_out;
	}
	if (ws_test_http_client_send_request(&request, ssl_config, &response, NULL)) {
		PRNT("fail to send request");
		goto release_out;
	}
	if (response.status == 200) {
		g_tc_pass++;
	}

release_out:
	ws_test_http_client_response_release(&response);
	PRNT("request ended");

	FUNC_EX;
	return 0;
}

int webserver_test_main(int argc, char *argv[])
{
	FUNC_EN;
	int iters = 1;

	if (argc < 4) {
		PRNT("invalid input");
		ws_test_dump_usage();
		return WS_TEST_ERR;
	}
	iters = atoi(argv[1]);
	if (iters < 1 || iters > 10000) {
		PRNT("iters < 1 || iters > 10000");
		return WS_TEST_ERR;
	}

	//generate report
	g_tc_pass = 0;

	/*------- Creating the request --------*/

	struct http_client_request_t request;
	struct http_keyvalue_list_t headers;
	struct http_client_ssl_config_t *ssl_config = NULL;

	int result = ws_test_prepare_request(argc, argv, &request);
	if (result != 0) {
		ws_test_dump_usage();
		if (request.is_test_entity) {
			free(request.entity);
		}
		return result;
	}

	if(request.https) {
		struct http_client_ssl_config_t temp_ssl_config = {
			(char *)mbedtls_test_ca_crt, NULL, NULL,
			mbedtls_test_ca_crt_len, 0, 0, WS_TEST_SSL_VERIFY_REQUIRED
		};
		ssl_config = &temp_ssl_config;
	}

	// before sending request, initialize keyvalue list for request headers
	result = http_keyvalue_list_init(&headers);
	if (result != 0) {
		PRNT("http_keyvalue_list_init() failed %d", result);
		goto release_out;
	}

	//	keep alive header
	if (request.keep_alive_count > 0) {
		http_keyvalue_list_add(&headers, HEADERFIELD_CONNECT, HEADERFIELD_KEEPALIVE);
		http_keyvalue_list_add(&headers, HEADERFIELD_KA, HEADERFIELD_KA_PARAMETERS);
	} else {
		http_keyvalue_list_add(&headers, HEADERFIELD_CONNECT, HEADERFIELD_CLOSE);
	}
	http_keyvalue_list_add(&headers, HEADERFIELD_USERAGENT, HEADERFIELD_TINYARA);
	request.headers = &headers;

	/*-------------- Running the iterations -----------*/

	for (int i = 1; i <= iters; i++) {
		result = ws_test_process_query(argc, argv, result, request, headers, ssl_config);
		if (result != 0) {
			PRNT("ws_test_process_query() failed %d, ending test", result);
			return result;
		}
		printf("\n------------++++++++++++++++++++++++++++-------------\n\n");
	}
	PRNT("---------------------------");
	PRNT("Result: %d/%d Passed", g_tc_pass, iters);
	PRNT("---------------------------");

release_out:
	if (request.is_test_entity) {
		free(request.entity);
	}
	result = http_keyvalue_list_release(&headers);
	if (result != 0) {
		PRNT("http_keyvalue_list_release() failed %d", result);
	}

	FUNC_EX;
	return 0;
}
