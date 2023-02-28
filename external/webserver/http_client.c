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

#include <fcntl.h>
#include <protocols/webserver/http_err.h>
#include <protocols/webserver/http_keyvalue_list.h>
#include <protocols/webclient.h>
#include <protocols/websocket.h>
#include <tinyara/fs/fs.h>
#include <tinyara/fs/dirent.h>

#include "http.h"
#include "http_client.h"
#include "http_string_util.h"
#include "http_query.h"
#include "http_arch.h"
#include "http_log.h"

#define MIN_WS_HEADER_FIELD 2
#define MAX_CLIENT_REQUEST 999999 /* it Will be updated if max client request exceeds 999999 */
#define MIN_CLIENT_REQUEST 100

pthread_addr_t http_handle_client(pthread_addr_t arg)
{
	struct http_server_t *server = (struct http_server_t *)arg;
	struct http_msg_t msg;
	int result;
	struct http_keyvalue_list_t request_params;
	int sock_fd;
	struct mallinfo data;
	struct http_client_t *p;
	mqd_t msg_q;
	struct mq_attr mqattr;

	if ((msg_q = http_server_mq_open(server->port)) == NULL) {
		HTTP_LOGE("msg queue open fail in http_handle_client\n");
		return NULL;
	}

	mq_getattr(msg_q, &mqattr);

	while (1) {
		if (mq_receive(msg_q, (char *)&msg, mqattr.mq_msgsize, NULL) < 0) {
			return NULL;
		}

		sock_fd = msg.data;

		if (msg.event == HTTP_STOP_EVENT) {
			close(sock_fd);
			break;
		}

		data = mallinfo();

		if (data.fordblks < HTTP_CONF_MIN_TLS_MEMORY * HTTP_CONF_MAX_CLIENT_HANDLE) {
			HTTP_LOGE("Error: Not enough memory :: %d\n", data.fordblks);
			close(sock_fd);
			continue;

		}

		HTTP_LOGD("Free Mem %d\n", data.fordblks);

		p = http_client_init(server, sock_fd);

		if (p == NULL) {
			HTTP_LOGE("Error: Cannot init client!!\n");
			close(sock_fd);
			continue;
		}

		HTTP_LOGD("Client %d.\n", p->client_fd);

#ifdef CONFIG_NET_SECURITY_TLS
		if (server->tls_init) {
			if (http_client_tls_init(p) != HTTP_OK) {
				HTTP_LOGE("Error: Cannot initialize TLS!! Close client.. %d\n", sock_fd);
				http_client_release(p);
				continue;
			}
		}
#endif

#ifdef CONFIG_NETUTILS_WEBSOCKET
		http_keyvalue_list_init(&request_params);
		result = http_recv_and_handle_request(p, &request_params);
		http_keyvalue_list_release(&request_params);

		if (result != HTTP_OK) {
			HTTP_LOGD("Client %d  in error case.\n", sock_fd);
		} else {
			HTTP_LOGD("Client %d  in normal case.\n", sock_fd);
		}
#else
		do {
			http_keyvalue_list_init(&request_params);
			result = http_recv_and_handle_request(p, &request_params);
			http_keyvalue_list_release(&request_params);

			HTTP_LOGD("Client %d in keep-alive %d.\n", sock_fd, p->keep_alive);

			if (result != HTTP_OK) {
				HTTP_LOGD("Client %d  in error case.\n", sock_fd);
				break;
			} else {
				HTTP_LOGD("Client %d  in normal case.\n", sock_fd);

				// Close client socket if connection is not persistent one.
				if (p->keep_alive == 0) {
					HTTP_LOGD("Client %d closing.\n", sock_fd);
					close(p->client_fd);
					break;
				}
			}
			p->remaining_request--;
		} while (p->keep_alive && p->remaining_request);
#endif

		HTTP_LOGD("Release client....keep_alive[%d], remaining_request[%d] \n", p->keep_alive, p->remaining_request);
		http_client_release(p);
	}

	mq_close(msg_q);

	HTTP_LOGD("Closed client handle %d\n", getpid());
	return NULL;
}


struct http_client_t *http_client_init(struct http_server_t *server, int sock_fd)
{
	struct http_client_t *p = (struct http_client_t *)HTTP_MALLOC(sizeof(struct http_client_t));
	if (p == NULL) {
		return NULL;
	}

	memset(p, 0, sizeof(struct http_client_t));

	p->client_fd = sock_fd;
	p->server = server;
	p->keep_alive = 0;
	p->keep_alive_timeout = HTTP_CONF_SOCKET_TIMEOUT_MSEC / HTTP_CONF_SEC_TO_MSEC;
	p->max_request = MAX_CLIENT_REQUEST;
	p->remaining_request = MAX_CLIENT_REQUEST;
	p->keep_alive_header_flag = 0;

	return p;
}

int http_client_release(struct http_client_t *client)
{
#ifdef CONFIG_NET_SECURITY_TLS
	if (client->server->tls_init && client->ws_state < MIN_WS_HEADER_FIELD) {
		http_client_tls_release(client);
	}
#endif
	HTTP_FREE(client);
	HTTP_LOGD("Free Client\n");
	return HTTP_OK;
}

static int process_trailer_header(char *buf, unsigned int buf_len, struct http_req_message *req,
				struct http_keyvalue_list_t *params,
				struct http_client_t *client, unsigned int sentence_start)
{
	char *header_field_value = NULL;
	int sentence_end;
	char *entity;
	unsigned int trailter_len;

	/* Process the Trailer */
	header_field_value = http_keyvalue_list_find(params, "Trailer");
	if (strncmp(header_field_value, "(null)", 7) == 0) {
		return -1;
	}

	/* match the Trailer field */
	if (strncmp(buf + sentence_start, header_field_value, strlen(header_field_value))
		 || *(buf + sentence_start + strlen(header_field_value)) != ':' ) {
		return -1;
	}

	HTTP_LOGD("Trailer header: %s\n", header_field_value);
	sentence_end = http_find_first_crlf(buf, buf_len, sentence_start);
	if (sentence_end < 0 || sentence_end - sentence_start == 0) {
		HTTP_LOGE("Error: Not accord with chunked encoding\n");
		return -1;
	}

	/* copy Trailer data */
	trailter_len = sentence_end - sentence_start;
	entity = (char *)zalloc(trailter_len + 1);
	if (entity == NULL) {
		HTTP_LOGE("Error: Fail to alloc memory\n");
		return -1;
	}
	memcpy(entity, buf + sentence_start, trailter_len);
	req->entity = entity;
	req->entity_len = trailter_len;
	http_dispatch_url(client, req);

	return 1;
}

#if CONFIG_NETUTILS_WEBSERVER_MIN_CLIENT_RCV_TIMEOUT >= CONFIG_NETUTILS_WEBSERVER_MAX_CLIENT_RCV_TIMEOUT
#error "minimum recv time out must be less than maximum timeout"
#endif

static void parse_keep_alive_header(struct http_client_t *client, struct http_keyvalue_list_t *params)
{
	char *token = NULL;
	char str_keep_alive[HTTP_CONF_MAX_VALUE_LENGTH] = { 0, };
	char *header_field_value = NULL;

	if (client->keep_alive_header_flag == 1) {
		HTTP_LOGD("keep alive header is already set: timeout[%d] max[%d] \n",
				client->keep_alive_timeout, client->max_request);
		return;
	}

	header_field_value = http_keyvalue_list_find(params, "Keep-Alive");
	if (strncmp(header_field_value, "(null)", 7) == 0) {
		return;
	}

	HTTP_LOGD("Keep Alive Header [%s] \n", header_field_value);

	strncpy(str_keep_alive, header_field_value, HTTP_CONF_MAX_VALUE_LENGTH);

	token = strtok(str_keep_alive, ", ");
	while (token != NULL) {
		if (strncmp(token, "timeout=", 8) == 0) {
			client->keep_alive_timeout = atoi(token + 8);
			if (client->keep_alive_timeout < CONFIG_NETUTILS_WEBSERVER_MIN_CLIENT_RCV_TIMEOUT) {
				client->keep_alive_timeout = CONFIG_NETUTILS_WEBSERVER_MIN_CLIENT_RCV_TIMEOUT;
			} else if (client->keep_alive_timeout > CONFIG_NETUTILS_WEBSERVER_MAX_CLIENT_RCV_TIMEOUT) {
				client->keep_alive_timeout = CONFIG_NETUTILS_WEBSERVER_MAX_CLIENT_RCV_TIMEOUT;
			}

			HTTP_LOGD("keep_alive_timeout : %d \n", client->keep_alive_timeout);
		} else if (strncmp(token, "max=", 4) == 0) {
			client->max_request = atoi(token + 4);
			if (client->max_request < MIN_CLIENT_REQUEST) {
				client->max_request =  MIN_CLIENT_REQUEST;
			}

			HTTP_LOGD("max request : %d \n", client->max_request);
		}

		token = strtok(NULL, ", ");
	}
}

int http_parse_message(char *buf, int buf_len, int *method, char *url,
					   char **body, int *enc, int *state,
					   struct http_message_len_t *len,
					   struct http_keyvalue_list_t *params,
					   struct http_client_t *client,
					   struct http_client_response_t *response,
					   struct http_req_message *req,
					   int *chunk_processed)
{
	int protocol = 0;
	int sentence_end = 0;
	char key[HTTP_CONF_MAX_KEY_LENGTH] = { 0, };
	char value[HTTP_CONF_MAX_VALUE_LENGTH] = { 0, };
	int process_finish = false;
	int read_finish = false;
	char *entity = *body;
	bool is_chunked = false;
	int ret = 0;

	while (!process_finish) {
		/* At this point, we read a line of http request */
		switch (*state) {
		case HTTP_REQUEST_HEADER:
			len->sentence_start = 0;
			len->message_len = 0;
			/* Search CR and LF */
			sentence_end = http_find_first_crlf(buf, buf_len, len->sentence_start);
			if (sentence_end != -1) {
				buf[sentence_end] = '\0';

				/* If this is called by webserver */
				if (client) {
					/* Header Examples
					 * ---------------
					 * 1. GET / HTTP/1.1
					 * 2. GET /cgi-bin/http_trace.pl HTTP/1.1\r\n
					 */
					http_separate_header(buf + len->sentence_start, method, url, &protocol);

					HTTP_LOGD(
						"Request Method : %s\n",
						(*method == HTTP_METHOD_GET) ? "GET" :
						(*method == HTTP_METHOD_PUT) ? "PUT" :
						(*method == HTTP_METHOD_POST) ? "POST" :
						(*method == HTTP_METHOD_DELETE) ?
						"DELETE" : "UNKNOWN");
					req->method = *method;
					HTTP_LOGD("Request URI : %s\n", url);
					HTTP_LOGD("Request Protocol : ");
					if (protocol == HTTP_HTTP_VERSION_09) {
						HTTP_LOGD("HTTP/0.9\n");
					} else if (protocol == HTTP_HTTP_VERSION_10) {
						HTTP_LOGD("HTTP/1.0\n");
					} else if (protocol == HTTP_HTTP_VERSION_11) {
						HTTP_LOGD("HTTP/1.1\n");
					} else {
						HTTP_LOGD("unknown version\n");
					}
				} else if(response) { /* If this is called by webclient */
					http_separate_status_line(buf + len->sentence_start, &protocol, &response->status, response->phrase);
					HTTP_LOGD("Response Status : %d\n", response->status);
					HTTP_LOGD("Response Phrase : %s\n", response->phrase);
				}
				len->sentence_start = sentence_end + 2;
				*state = HTTP_REQUEST_PARAMETERS;
			} else {
				read_finish = true;
				process_finish = true;
			}
			break;
		case HTTP_REQUEST_PARAMETERS:
			/* Search CR and LF */
			sentence_end = http_find_first_crlf(buf, buf_len, len->sentence_start);
			if (sentence_end >= 0) {
				buf[sentence_end] = '\0';
				if (strlen(buf + len->sentence_start) > 0) {
					/* Read parameters */
					int result = http_separate_keyvalue(buf + len->sentence_start, key, value);
					if (result == HTTP_ERROR) {
						HTTP_LOGE("Error: Fail to separate keyvalue\n");
						return HTTP_ERROR;
					}
					HTTP_LOGD("[HTTP Parameter] Key: %s / Value: %s\n", key, value);

					http_keyvalue_list_add(params, key, value);
					if (client) {
						if (strcmp(key, "Connection") == 0 && strcmp(value, "Upgrade") == 0) {
							++client->ws_state;
						}
						if (strcmp(key, "Upgrade") == 0 && strcmp(value, "websocket") == 0) {
							++client->ws_state;
						}
						if (strcmp(key, "Sec-WebSocket-Key") == 0) {
							strncpy((char *)client->ws_key, value, WEBSOCKET_CLIENT_KEY_LEN);
						} else if (strcmp(key, "Keep-Alive") == 0) {
							parse_keep_alive_header(client, params);
						}
					}
					if (strcmp(key, "Content-Length") == 0) {
						len->content_len = HTTP_ATOI(value);
						if (response) {
							response->total_len = len->content_len;
						}

						HTTP_LOGD("This request contains contents, length : %d\n", len->content_len);
					}
					if (strcmp(key, "Transfer-Encoding") == 0 && strcmp(value, "chunked") == 0) {
						if (client) {
							len->chunked_remain = 0;
							len->entity_len = 0;
							entity = HTTP_MALLOC(HTTP_CONF_MAX_ENTITY_LENGTH);
							if (entity == NULL) {
								HTTP_LOGE("Error: Fail to alloc memory\n");
								return HTTP_ERROR;
							}
							*enc = HTTP_CHUNKED_ENCODING;
							req->encoding = *enc;
							*body = entity;
							HTTP_LOGD("This request contains chunked encoding contents\n");
						} else {
							is_chunked = true;
							HTTP_LOGD("Transfer-Encoding is chunked!!!!!\n");
						}
					}
				} else {
					*state = HTTP_REQUEST_BODY;
				}
				len->sentence_start = sentence_end + 2;
			} else {
				process_finish = true;
			}
			break;

		case HTTP_REQUEST_BODY:
			if (client && *method != HTTP_METHOD_POST && *method != HTTP_METHOD_PUT) {
				read_finish = true;
				return read_finish;
			}
			/* Content length */
			if (len->sentence_start < 0 || sentence_end < 0) {
				HTTP_LOGE("Error: Wrong sentence arrange\n");
				break;
			}
			if (*enc == HTTP_CONTENT_LENGTH) {
				bool is_header = false;
				if (!len->message_len) {
					is_header = true;
					*body = buf + len->sentence_start;
					HTTP_LOGD("[HEADER] buf_len : %d len->sentence_start : %d len->message_len : %d len->content_len %d\n", buf_len, len->sentence_start, len->message_len, len->content_len);
				}
				if (buf_len - len->sentence_start + len->message_len == len->content_len) {
					buf[buf_len] = '\0';
					HTTP_LOGD("[BODY_END]buf_len : %d len->sentence_start : %d len->message_len : %d len->content_len %d\n", buf_len, len->sentence_start, len->message_len, len->content_len);
					//HTTP_LOGD("All body readed : \n%s\n", *body);

					if (response) {
						response->total_len = len->content_len;

						if (is_header == true) {
							response->entity_len = buf_len - len->sentence_start;
							response->entity = buf + len->sentence_start;
						} else {
							response->entity_len = buf_len;
							response->entity = buf;
						}
					}

					read_finish = true;
				} else {
					if (is_chunked == true) {
						int i, j, cha;
						len->content_len = 0;
						sentence_end = http_find_first_crlf(buf, buf_len, len->sentence_start);

						if (sentence_end < 0) {
							HTTP_LOGE("Error: Cannot find body\n");
							process_finish = true;
							read_finish = true;
							break;
						}

						/* calculate chunked size */
						for (i = 1; sentence_end - i >= len->sentence_start; ++i) {
							cha = (int)*(buf + sentence_end - i);
							if (cha > '9') {
								cha = cha - 'a' + 10;
							} else {
								cha = cha - '0';
							}
							for (j = 1; j < i; ++j) {
								cha *= 16;
							}
							len->content_len += cha;
						}

						*body = buf + len->sentence_start + 5;
						buf[buf_len + len->message_len - 2] = '\0';

						HTTP_LOGD("buf_len : %d len->sentence_start : %d len->message_len : %d len->content_len %d\n", buf_len, len->sentence_start, len->message_len, len->content_len);
						read_finish = true;
					}
					len->message_len += buf_len;

					if (response) {
						response->total_len = len->content_len;

						if (is_chunked) {
							if (response->entity) {
								response->entity = *body;
								response->entity_len = len->content_len;
							}
						} else if (is_header == true && !is_chunked) {
							response->entity_len = buf_len - len->sentence_start;
							response->entity = buf + len->sentence_start;
							HTTP_LOGD("[HEADER_BODY]buf_len : %d len->sentence_start : %d len->message_len : %d len->content_len %d entity_len : %d\n", buf_len, len->sentence_start, len->message_len, len->content_len, response->entity_len);
						} else {
							response->entity = buf;
							response->entity_len = buf_len;
						}
					}
					
					HTTP_LOGD("[BODY]buf_len : %d len->sentence_start : %d len->message_len : %d len->content_len %d\n", buf_len, len->sentence_start, len->message_len, len->content_len);
					HTTP_LOGD("Not all body readed\n");
				}
				process_finish = true;
			}
			/* Chunked encoding */
			else {
				int i, j, cha;
				if (!len->chunked_remain) {
					len->content_len = 0;
					sentence_end = http_find_first_crlf(buf, buf_len, len->sentence_start);

					if (sentence_end < 0) {
						HTTP_LOGE("Error: Cannot find body - Need more data\n");
						process_finish = true;
						read_finish = false;
						*chunk_processed = len->sentence_start;
						break;
					}

					/* calculate chunked size */
					for (i = 1; sentence_end - i >= len->sentence_start; ++i) {
						cha = (int)*(buf + sentence_end - i);
						if (cha > '9') {
							cha = cha - 'a' + 10;
						} else {
							cha = cha - '0';
						}
						for (j = 1; j < i; ++j) {
							cha *= 16;
						}
						len->content_len += cha;
					}
					len->chunked_remain = len->content_len;
					if (!len->chunked_remain) {
						HTTP_LOGD("Chunked size is zero\n");
						entity[len->entity_len] = '\0';
						req->entity = entity + len->entity_len;
						req->entity_len = 0;
						http_dispatch_url(client, req);
						read_finish = true;
						return read_finish;
					}
					len->sentence_start = sentence_end + 2;
				}
				i = 0;
				if (len->chunked_remain > 0) {
					for (i = 0; len->chunked_remain > 0; ++i) {
						if (len->sentence_start >= buf_len + len->message_len
                                                    || i + len->entity_len >= HTTP_CONF_MAX_ENTITY_LENGTH) {
							len->message_len = len->sentence_start;
							len->entity_len += i;
							read_finish = false;
							return read_finish;
						}
						entity[i + len->entity_len] = *(buf + len->sentence_start++);
						--len->chunked_remain;
					}
					len->entity_len += i;
				}

				if (!len->chunked_remain) {
					entity[len->entity_len] = '\0';
					req->entity = entity + len->entity_len - len->content_len;
					req->entity_len = len->content_len;
					http_dispatch_url(client, req);

					// Reset entity length. Next chunk will be processed from begining of entity buffer.
					len->entity_len = 0;

					len->sentence_start += 2;

					// Save number bytes of data is processed.
					*chunk_processed = len->sentence_start;

					sentence_end = http_find_first_crlf(buf, buf_len + len->message_len, len->sentence_start);
					if (sentence_end < 0) {
						HTTP_LOGE("Error: Not accord with chunked encoding - need more data\n");
						read_finish = false;
						return read_finish;
					}

					// Check last chunk is in buffer
					if (*(buf + sentence_end - 1) == '0'
					    && (sentence_end - len->sentence_start) == 1) {
						entity[len->entity_len] = '\0';
						req->entity = entity + len->entity_len;
						req->entity_len = 0;
						http_dispatch_url(client, req);
						process_finish = true;
						read_finish = true;

						/* sentence start is next statement after \r\n */
						len->sentence_start = sentence_end + 2;
						ret = process_trailer_header(buf, buf_len, req, params, client, len->sentence_start);
						if (ret < 0) {
							HTTP_LOGD("Trailer header is not present or failed to process it \n");
						}
					}
				}
			}
			break;
		}
	}
	return read_finish;
}

int http_recv_and_handle_request(struct http_client_t *client, struct http_keyvalue_list_t *request_params)
{
	char *buf;
	int len = 0;
	char *body = NULL;
	int read_finish = false;

	int method = HTTP_METHOD_UNKNOWN;
	char url[HTTP_CONF_MAX_REQUEST_HEADER_URL_LENGTH] = { 0, };
	int buf_len = 0;
	int remain = HTTP_CONF_MAX_REQUEST_LENGTH;
	int enc = HTTP_CONTENT_LENGTH;
	struct http_req_message req = {0, };
	int state = HTTP_REQUEST_HEADER;
	struct http_message_len_t mlen = {0,};
	struct sockaddr_in addr;
	socklen_t addr_len;
	char *conn_type = NULL;
	int chunk_processed = 0;
	int unprocessed = 0;
	int i = 0;

	client->ws_state = 0;

	buf = HTTP_MALLOC(HTTP_CONF_MAX_REQUEST_LENGTH);
	if (buf == NULL) {
		HTTP_LOGE("Error: Fail to malloc buf\n");
		close(client->client_fd);
		return HTTP_ERROR;
	}

	memset(buf, 0, HTTP_CONF_MAX_REQUEST_LENGTH);

	if (getpeername(client->client_fd, (struct sockaddr *)&addr, &addr_len) < 0) {
		HTTP_LOGE("Error: Fail to getpeername\n");
		goto errout;
	}
	req.req_msg = buf;
	req.url = url;
	req.headers = request_params;
	req.client_ip = addr.sin_addr.s_addr;
	req.encoding = HTTP_CONTENT_LENGTH;

	while (!read_finish) {
		if (remain <= 0) {
			HTTP_LOGE("Error: Request size is too large!!\n");
			goto err_large_request;
		}
#ifdef CONFIG_NET_SECURITY_TLS
		if (client->server->tls_init) {
			len = mbedtls_ssl_read(&(client->tls_ssl), (unsigned char *)buf + buf_len, HTTP_CONF_MAX_REQUEST_LENGTH - buf_len);
		} else
#endif
		{
			len = recv(client->client_fd, buf + buf_len, HTTP_CONF_MAX_REQUEST_LENGTH - buf_len, 0);
		}
		if (len < 0) {
			HTTP_LOGE("Error: Receive Fail %d errno:[%s-%d] \n", len, strerror(errno), errno);
			goto errout;
		} else if (len == 0) {
			HTTP_LOGD("Finish read\n");
			goto errout;
		}
		buf_len += len;
		remain -= len;

		read_finish = http_parse_message(buf, buf_len, &method, url, &body, &enc, &state, &mlen, request_params, client, NULL, &req, &chunk_processed);
		if (read_finish == HTTP_ERROR) {
			goto errout;
		}

		/* TODO: hot fix
		   If buf contains partial chunk then copy that partial chunk
		   to the beginning of buf for next socket read and send it to
                   http parse along with newly received data from socket */
		if (enc == HTTP_CHUNKED_ENCODING && !read_finish) {
			HTTP_LOGD("chunk_processed=%d\n", chunk_processed);
			unprocessed = buf_len - chunk_processed;
			if (unprocessed > 0) {
				for (i=chunk_processed; i<buf_len; i++) {
					buf[i-chunk_processed] = buf[i];
				}
			}

			//reset chunk_processed
			chunk_processed = 0;

                        buf_len = unprocessed;
                        remain = HTTP_CONF_MAX_REQUEST_LENGTH - buf_len;
                        memset(&mlen, 0x0, sizeof(struct http_message_len_t));
		}
	}

	// Check "Connection" header value
	conn_type = http_keyvalue_list_find(request_params, "Connection");
	if (!strncasecmp(conn_type, "Keep-Alive", strlen("Keep-Alive")+1)) {
		client->keep_alive = 1;

		if (client->keep_alive_timeout != (HTTP_CONF_SOCKET_TIMEOUT_MSEC / HTTP_CONF_SEC_TO_MSEC) &&
				client->keep_alive_header_flag == 0) {
			struct timeval tv;
			tv.tv_sec = client->keep_alive_timeout;
			tv.tv_usec = 0;
			HTTP_LOGD("Keep-alive case, change timeout to (%u.%d)sec\n", tv.tv_sec, tv.tv_usec);
			if (setsockopt(client->client_fd, SOL_SOCKET, SO_RCVTIMEO, (struct timeval *)&tv, sizeof(struct timeval)) < 0) {
				HTTP_LOGE("Error: set timeout to socket fails \n");
			} else {
				HTTP_LOGD("Timeout modified done\n");
				client->remaining_request = client->max_request;
				client->keep_alive_header_flag = 1;
			}
		}
	} else {
		client->keep_alive = 0;
	}

	if (method == HTTP_METHOD_UNKNOWN) {
		goto errout;
	}

	if (enc == HTTP_CONTENT_LENGTH) {
		req.entity = body;
		http_dispatch_url(client, &req);
	}

#ifdef CONFIG_NETUTILS_WEBSOCKET
	/* open websocket */
	if (client->ws_state >= MIN_WS_HEADER_FIELD) {
		websocket_t *ws = NULL;
		ws = websocket_find_table();
		if (ws == NULL) {
			goto errout;
		}
		ws->fd = client->client_fd;
		ws->cb = &client->server->ws_cb;
#ifdef CONFIG_NET_SECURITY_TLS
		if (client->server->tls_init) {
			ws->tls_enabled = 1;
			ws->tls_net.fd = client->tls_client_fd.fd;
			ws->tls_ssl = (mbedtls_ssl_context *)malloc(sizeof(mbedtls_ssl_context));
			memcpy(ws->tls_ssl, &client->tls_ssl, sizeof(mbedtls_ssl_context));
			ws->tls_conf = &client->server->tls_conf;
			mbedtls_ssl_set_bio(ws->tls_ssl, &ws->tls_net, mbedtls_net_send, mbedtls_net_recv, NULL);
		}
#endif
		if (pthread_attr_init(&ws->thread_attr) != 0) {
			HTTP_LOGE("Error: Cannot initialize thread attribute\n");
			goto errout;
		}
		pthread_attr_setstacksize(&ws->thread_attr, WEBSOCKET_STACKSIZE);
		pthread_attr_setschedpolicy(&ws->thread_attr, SCHED_RR);
		if (pthread_create(&ws->thread_id, &ws->thread_attr,
						   (pthread_startroutine_t)websocket_server_init,
						   (pthread_addr_t)ws) != 0) {
			HTTP_LOGE("Error: Cannot create websocket thread!!\n");
			goto errout;
		}
		pthread_setname_np(ws->thread_id, "websocket handle server");
		pthread_detach(ws->thread_id);
	} else {
		close(client->client_fd);
	}
#endif

	HTTP_FREE(buf);
	if (enc == HTTP_CHUNKED_ENCODING) {
		HTTP_FREE(body);
	}
	return HTTP_OK;

err_large_request:
	http_send_response(client, 413, "Payload Too Large\r\n", NULL);

errout:
	close(client->client_fd);
	HTTP_FREE(buf);
	if (enc == HTTP_CHUNKED_ENCODING) {
		HTTP_FREE(body);
	}
	return HTTP_ERROR;
}

void http_handle_file(struct http_client_t *client, int method, const char *url, char *entity)
{
	FILE *f;
	char path[HTTP_CONF_MAX_REQUEST_HEADER_URL_LENGTH + 1] = ".";
	int ref;
	int valid = 1;

	switch (method) {
	case HTTP_METHOD_GET:
		if ((f = fopen(url, "r")) != NULL) {
			fgets(entity, HTTP_CONF_MAX_ENTITY_LENGTH, f);
			if (http_send_response(client, 200, entity, NULL) == HTTP_ERROR) {
				HTTP_LOGE("Error: Fail to send response\n");
			}
			fclose(f);
		} else {
			if (http_send_response(client, 404, HTTP_ERROR_404, NULL) == HTTP_ERROR) {
				HTTP_LOGE("Error: Fail to send response\n");
			}
		}
		break;
	case HTTP_METHOD_POST:
		ref = strlen(url) + 12;
		if (ref > HTTP_CONF_MAX_REQUEST_HEADER_URL_LENGTH) {
			valid = 0;
			HTTP_LOGE("ERROR: URL length is too long. Cannot send response\n");
		} else
			valid = 1;
		/* Need to create file with unique file name */
		strncat(path, url, HTTP_CONF_MAX_REQUEST_HEADER_URL_LENGTH - strlen(path));
		strncat(path, "index.shtml", HTTP_CONF_MAX_REQUEST_HEADER_URL_LENGTH - strlen(path));
		if (valid && (f = fopen(path, "w"))) {
			if (fputs(entity, f) < 0) {
				HTTP_LOGE("Error: Fail to execute fputs\n");
				fclose(f);
				break;
			}
			if (http_send_response(client, 200, path, NULL) == HTTP_ERROR) {
				HTTP_LOGE("Error: Fail to send response\n");
			}
			fclose(f);
		} else {
			HTTP_LOGE("fail to open %s\n", path);
			if (http_send_response(client, 500, HTTP_ERROR_500, NULL) == HTTP_ERROR) {
				HTTP_LOGE("Error: Fail to send response\n");
			}
		}
		break;
	case HTTP_METHOD_PUT:
		ref = strlen(url) + 1;
		if (ref > HTTP_CONF_MAX_REQUEST_HEADER_URL_LENGTH) {
			valid = 0;
			HTTP_LOGE("ERROR: URL length is too long. Cannot send response\n");
		} else
			valid = 1;
		if (valid && (f = fopen(strncat(path, url, HTTP_CONF_MAX_REQUEST_HEADER_URL_LENGTH - strlen(path)), "w"))) {
			if (fputs(entity, f) < 0) {
				HTTP_LOGE("Error: Fail to execute fputs\n");
				fclose(f);
				break;
			}
			if (http_send_response(client, 200, url, NULL) == HTTP_ERROR) {
				HTTP_LOGE("Error: Fail to send response\n");
			}
			fclose(f);
		} else {
			HTTP_LOGE("fail to open %s\n", url);
			if (http_send_response(client, 500, HTTP_ERROR_500, NULL) == HTTP_ERROR) {
				HTTP_LOGE("Error: Fail to send response\n");
			}
		}
		break;
	case HTTP_METHOD_DELETE:
		ref = strlen(url) + 1;
		if (ref > HTTP_CONF_MAX_REQUEST_HEADER_URL_LENGTH) {
			valid = 0;
			HTTP_LOGE("ERROR: URL length is too long. Cannot send response\n");
		} else
			valid = 1;
		if (valid && unlink(strncat(path, url, HTTP_CONF_MAX_REQUEST_HEADER_URL_LENGTH - strlen(path)))) {
			HTTP_LOGE("fail to delete %s\n", url);
			if (http_send_response(client, 500, HTTP_ERROR_500, NULL) == HTTP_ERROR) {
				HTTP_LOGE("Error: Fail to send response\n");
			}
		} else {
			HTTP_LOGD("success to delete %s\n", url);
			if (http_send_response(client, 200, url, NULL) == HTTP_ERROR) {
				HTTP_LOGE("Error: Fail to send response\n");
			}
		}
		break;
	}
}

static int prepare_chunk_body(char *buf, unsigned int len, const char *body, unsigned int body_len, bool is_last_msg)
{
	int chunk_size;
	char httpcrnl[] = "\r\n";
	int buflen = 0;

	if (buf == NULL) {
		HTTP_LOGE("buf is NULL \n");
		return -1;
	}

	buflen = len;

	if (is_last_msg) {
		chunk_size = 0;
	} else {
		if (body == NULL) {
			HTTP_LOGE("msg body is NULL \n");
			return -1;
		}
		chunk_size = body_len;
	}

	//append length of message
	buflen += snprintf(buf + buflen, HTTP_CONF_MAX_REQUEST_LENGTH - buflen, "%x", chunk_size);

	//append httpcrnl
	buflen += snprintf(buf + buflen, HTTP_CONF_MAX_REQUEST_LENGTH - buflen, "%s", httpcrnl);

	if (is_last_msg) {
		//append httpcrnl and no body
		buflen += snprintf(buf + buflen, HTTP_CONF_MAX_REQUEST_LENGTH - buflen, "%s", httpcrnl);
		return buflen;
	}

	if (HTTP_CONF_MAX_REQUEST_LENGTH - buflen - 2 <= chunk_size) {
		HTTP_LOGE("msg body is larger than buffer can hold \n");
		return -1;
	}

	//append body
	memcpy(buf + buflen, body, body_len); 
	buflen += body_len;

	//append httpcrnl
	buflen += snprintf(buf + buflen, HTTP_CONF_MAX_REQUEST_LENGTH - buflen, "%s", httpcrnl);

	return buflen;
}

static int http_send_chunk_buffer(struct http_client_t *client, char *buf, int len)
{
	int ret = 0;
	int sndlen = 0;
	int buflen = 0;

	if (client == NULL) {
		HTTP_LOGE("client is NULL \n");
		return -1;
	}

	sndlen = len;

	while (sndlen > 0) {
#ifdef CONFIG_NET_SECURITY_TLS
		if (client->server->tls_init) {
			ret = mbedtls_ssl_write(&(client->tls_ssl), (unsigned char *)buf + buflen, sndlen);
		} else
#endif
		{
			ret = send(client->client_fd, buf + buflen, sndlen, 0);
		}

		if (ret < 1) {
			HTTP_LOGE("Fail to send buffer ret[%d] \n", ret);
			return -1;
		} else {
			sndlen -= ret;
			buflen += ret;
		}
	}

	return 0;
}

static int http_send_chunk(struct http_client_t *client, char *buf, int buflen,
					const char *body, int body_len, bool is_last_msg)
{
	int ret = 0;
	int buf_len = 0;

	if(client == NULL || buf == NULL) {
		HTTP_LOGE("client or buf is NULL \n");
		return -1;
	}

	ret = prepare_chunk_body(buf, buflen, body, body_len, is_last_msg);
	if (ret < 0) {
		HTTP_LOGE("Failed to prepare chunk body \n");
		return ret;
	}

	buf_len = ret;

	ret = http_send_chunk_buffer(client, buf, buf_len);
	if (ret < 0) {
		HTTP_LOGE("Failed to send chunk buffer \n");
		return ret;
	}

	return 0;
}

int http_send_response_chunk(struct http_client_t *client, int status, const char* status_message,
			const char *body, int body_len, struct http_keyvalue_list_t *headers, data_type_e data_type)
{
	char *buf;
	int buflen = 0;
	struct http_keyvalue_t *cur = NULL;
	int ret = 0;

	if (body == NULL) {
		HTTP_LOGE("There is empty body \n");
		return HTTP_ERROR;
	}

	buf = HTTP_MALLOC(HTTP_CONF_MAX_REQUEST_LENGTH);
	if (buf == NULL) {
		HTTP_LOGE("Fail to malloc buffer\n");
		return HTTP_ERROR;
	}
	memset(buf, 0, HTTP_CONF_MAX_REQUEST_LENGTH);

	if (data_type == FIRST_DATA) {
		//add header and data
		buflen = snprintf(buf, HTTP_CONF_MAX_REQUEST_LENGTH, "HTTP/1.1 %d %s\r\n",
					  status, status_message);
		if (headers && headers->head) {
			cur = headers->head->next;
			while (cur != headers->tail) {
				if (strcmp(cur->key, "Content-Length") == 0 || strcmp(cur->key, "Content-Type") == 0
					|| strcmp(cur->key, "Keep-Alive") == 0 || strcmp(cur->key, "Connection") == 0
					|| strcmp(cur->key, "Transfer-Encoding") == 0)  {
					cur = cur->next;
					continue;
				}

				buflen += snprintf(buf + buflen, HTTP_CONF_MAX_REQUEST_LENGTH - buflen,
								   "%s: %s\r\n", cur->key, cur->value);
				cur = cur->next;
			}
		}

		buflen += snprintf(buf + buflen,
					   HTTP_CONF_MAX_REQUEST_LENGTH - buflen,
					   "Content-Type: text/html\r\n");

		buflen += snprintf(buf + buflen,
					   HTTP_CONF_MAX_REQUEST_LENGTH - buflen,
					   "Transfer-Encoding: chunked\r\n");

		if (client->keep_alive == 0) {
			buflen += snprintf(buf + buflen, HTTP_CONF_MAX_REQUEST_LENGTH - buflen,
							"Connection: close\r\n");
		} else {
			buflen += snprintf(buf + buflen, HTTP_CONF_MAX_REQUEST_LENGTH - buflen,
							"Connection: Keep-Alive\r\n");
		}

		buflen += snprintf(buf + buflen, HTTP_CONF_MAX_REQUEST_LENGTH - buflen,
					"Keep-Alive: timeout=%d, max=%d\r\n",
					client->keep_alive_timeout, client->max_request);

		// Append extra CRLF to mark headers done
		buflen += snprintf(buf + buflen,
					HTTP_CONF_MAX_REQUEST_LENGTH - buflen, "\r\n");
	}


	// Include response body for chunk
	ret = http_send_chunk(client, buf, buflen, body, body_len, false);
	if (ret < 0) {
		HTTP_FREE(buf);
		return HTTP_ERROR;
	}

	if (data_type == LAST_DATA) {
		//last chunk with no body
		memset (buf, 0, HTTP_CONF_MAX_REQUEST_LENGTH);
		buflen = 0;

		ret = http_send_chunk(client, buf, buflen, NULL, 0, true);
		if (ret < 0) {
			HTTP_FREE(buf);
			return HTTP_ERROR;
		}
	}

	HTTP_FREE(buf);
	return HTTP_OK;
}

static int http_send_buffer(struct http_client_t *client, const char *buf, int len)
{
	int send_byte = 0;
	int sndlen = 0;
	int buflen = 0;

	if ((client == NULL) || (buf == NULL) || (len <= 0)) {
		HTTP_LOGE("Invalid arguments  \n");
		return -1;
	}

	sndlen = len;
	while (sndlen > 0) {
#ifdef CONFIG_NET_SECURITY_TLS
		if (client->server->tls_init) {
			send_byte = mbedtls_ssl_write(&(client->tls_ssl), (unsigned char *)buf + buflen, sndlen);
		} else
#endif
		{
			send_byte = send(client->client_fd, buf + buflen, sndlen, 0);
		}

		if (send_byte < 1) {
			HTTP_LOGE("Fail to send buffer send_byte[%d] errno[%d] \n", send_byte, errno);
			return -1;
		} else {
			sndlen -= send_byte;
			buflen += send_byte;
		}
	}

	return 0;
}

int http_send_response_helper(struct http_client_t *client, int status, const char* status_message,
				const char* body, int body_len, struct http_keyvalue_list_t *headers)
{
	char *buf;
	int buflen = 0;
	struct http_keyvalue_t *cur = NULL;
	int len = 0;
	int rem_body_len = 0;
	int ret = 0;

	buf = HTTP_MALLOC(HTTP_CONF_MAX_REQUEST_LENGTH);
	if (buf == NULL) {
		HTTP_LOGE("Error: Fail to malloc buffer\n");
		return HTTP_ERROR;
	}

	memset(buf, 0, HTTP_CONF_MAX_REQUEST_LENGTH);

#ifdef CONFIG_NETUTILS_WEBSOCKET
	if (client->ws_state >= MIN_WS_HEADER_FIELD) {
		unsigned char accept_key[WEBSOCKET_ACCEPT_KEY_LEN] = {0, };
		websocket_create_accept_key(accept_key, WEBSOCKET_ACCEPT_KEY_LEN, client->ws_key, WEBSOCKET_CLIENT_KEY_LEN);
		buflen = snprintf(buf, HTTP_CONF_MAX_REQUEST_LENGTH,
						  "HTTP/1.1 101 Switching Protocols\r\n"
						  "Upgrade: websocket\r\n"
						  "Connection: Upgrade\r\n"
						  "Sec-WebSocket-Accept: %s\r\n\r\n",
						  accept_key);
	} else
#endif
	{
		buflen = snprintf(buf, HTTP_CONF_MAX_REQUEST_LENGTH, "HTTP/1.1 %d %s\r\n",
						  status, status_message);
		if (headers) {
			cur = headers->head->next;
			while (cur != headers->tail) {
				if (strcmp(cur->key, "Content-Length") == 0 || strcmp(cur->key, "Content-Type") == 0
					|| strcmp(cur->key, "Keep-Alive") == 0) {
					cur = cur->next;
					continue;
				}

				buflen += snprintf(buf + buflen, HTTP_CONF_MAX_REQUEST_LENGTH - buflen,
								   "%s: %s\r\n", cur->key, cur->value);
				cur = cur->next;
			}

			// Add content type and content length headers
			if (body) {
				len = snprintf(buf + buflen,
                                                           HTTP_CONF_MAX_REQUEST_LENGTH - buflen,
                                                           "Content-Type: text/html\r\n"
                                                           "Content-Length: %d\r\n",
                                                           body_len);
				if (len < 0) {
					HTTP_LOGE("Error: snprintf failed \n");
					HTTP_FREE(buf);
					return HTTP_ERROR;
				}

				buflen += len;
			}

			// Add keep alive header
			len = snprintf(buf + buflen, HTTP_CONF_MAX_REQUEST_LENGTH - buflen,
						"Keep-Alive: timeout=%d, max=%d\r\n",
						client->keep_alive_timeout, client->max_request);

			if (len < 0) {
				HTTP_LOGE("Error: snprintf failed \n");
				HTTP_FREE(buf);
				return HTTP_ERROR;
			}

			buflen += len;
		} else {
			// Add content header
			if (client->keep_alive == 0) {
				buflen += snprintf(buf + buflen, HTTP_CONF_MAX_REQUEST_LENGTH - buflen,
                                                                "Connection: close\r\n");
			} else {
				buflen += snprintf(buf + buflen, HTTP_CONF_MAX_REQUEST_LENGTH - buflen,
                                                                "Connection: Keep-Alive\r\n");
			}

			// Add content type and content length headers
			if (body) {
				buflen += snprintf(buf + buflen,
                                                           HTTP_CONF_MAX_REQUEST_LENGTH - buflen,
                                                           "Content-type: text/html\r\n"
                                                           "Content-Length: %d\r\n",
                                                           body_len);
			}

			// Add keep alive header
			len = snprintf(buf + buflen, HTTP_CONF_MAX_REQUEST_LENGTH - buflen,
						"Keep-Alive: timeout=%d, max=%d\r\n",
						client->keep_alive_timeout, client->max_request);
			if (len < 0) {
				HTTP_LOGE("Error: snprintf failed \n");
				HTTP_FREE(buf);
				return HTTP_ERROR;
			}

			buflen += len;
		}

		// Append extra CRLF to mark headers done
		buflen += snprintf(buf + buflen,
					HTTP_CONF_MAX_REQUEST_LENGTH - buflen, "\r\n");
		// Include response body
		if (body) {
			memcpy(buf + buflen, body, HTTP_CONF_MAX_REQUEST_LENGTH - buflen);
			if (body_len <= HTTP_CONF_MAX_REQUEST_LENGTH - buflen) {
				len = body_len;
			} else {
				len = HTTP_CONF_MAX_REQUEST_LENGTH - buflen;
				rem_body_len = body_len - len;
			}
			buflen += len;
		}
	}

	ret = http_send_buffer(client, buf, buflen);
	if (ret < 0) {

		HTTP_LOGE("Error: failed to send buffer \n");
		HTTP_FREE(buf);
		return HTTP_ERROR;
	}

	//send remaining body
	if (rem_body_len > 0) {
		ret = http_send_buffer(client, body + len, rem_body_len);
		if (ret < 0) {

			HTTP_LOGE("Error: failed to send buffer \n");
			HTTP_FREE(buf);
			return HTTP_ERROR;
		}
	}

	HTTP_FREE(buf);
	return HTTP_OK;
}

int http_send_response(struct http_client_t *client, int status, const char *body, struct http_keyvalue_list_t *headers)
{
	const char* status_message = (status == 200) ? "OK" : body;

	int body_len = 0;
	if (body) {
		body_len = strlen(body);
	}
	return http_send_response_helper(client, status, status_message, body, body_len, headers);
}

int http_send_response_with_status_msg(struct http_client_t *client, int status, const char* status_message, const char* body, struct http_keyvalue_list_t *headers)
{
	int body_len = 0;
	if (body) {
		body_len = strlen(body);
	}
	return http_send_response_helper(client, status, status_message, body, body_len, headers);
}

int http_send_response_with_status(struct http_client_t *client, int status, const char* status_message, const char* body, int body_len, struct http_keyvalue_list_t *headers)
{
	return http_send_response_helper(client, status, status_message, body, body_len, headers);
}
