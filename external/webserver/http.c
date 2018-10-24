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

#include <sys/types.h>
#include <pthread.h>
#include <protocols/webserver/http_err.h>
#include <protocols/webserver/http_server.h>
#include <protocols/webserver/http_keyvalue_list.h>
#include <signal.h>
#include <fcntl.h>

#include "http.h"
#include "http_client.h"
#include "http_log.h"

#define MAX_ACCEPTED_FD    20
#define ACCEPT_TIMEOUT_MS  100
#define HTTP_LISTENING_HANDLER_STACKSIZE (1024 * 4)
#define HTTP_CLIENT_HANDLER_STACKSIZE    (1024 * 4)
#define HTTPS_CLIENT_HANDLER_STACKSIZE    (1024 * 8)

int http_server_mq_flush(mqd_t msg_q)
{
	struct http_msg_t msg;
	struct mq_attr mqattr;
	struct timespec  t;

	mq_getattr(msg_q, &mqattr);
	HTTP_LOGD("msg queue flush start : [%d]\n", mqattr.mq_curmsgs);

	t.tv_sec = 0;
	t.tv_nsec = 1000;

	while (mqattr.mq_curmsgs != 0) {
		mq_timedreceive(msg_q, (char *)&msg, mqattr.mq_msgsize, NULL, &t);
		if (msg.event == HTTP_CONNECT_EVENT) {
			close(msg.data);
		}
		mq_getattr(msg_q, &mqattr);
	}

	HTTP_LOGD("msg queue flush finish : [%d]\n", mqattr.mq_curmsgs);
	return 0;
}

mqd_t http_server_mq_open(int port)
{
	struct mq_attr mqattr;
	char msg_name[8];
	mqd_t msg_q;

	snprintf(msg_name, 8, "mq%05d", port);
	mqattr.mq_maxmsg = HTTP_CONF_SERVER_MQ_MAX_MSG;
	mqattr.mq_msgsize = sizeof(struct http_msg_t);
	mqattr.mq_flags = 0;

	if ((msg_q = mq_open(msg_name, O_RDWR | O_CREAT, 0666, &mqattr)) == (mqd_t)ERROR) {
		msg_q = NULL;
	}

	return msg_q;
}

int http_server_mq_close(int port)
{
	char msg_name[8];

	snprintf(msg_name, 8, "mq%05d", port);

	return mq_unlink(msg_name);
}

pthread_addr_t http_server_handler(pthread_addr_t arg)
{
	fd_set readfds;
	int fdcnt = 0;
	int fdarr[MAX_ACCEPTED_FD] = {0,};
	mqd_t msg_q;
	struct http_msg_t msg;
	socklen_t addrlen;
	int sock_fd, ret, cnt, i, maxfd = 0;
	struct timeval tv, accept_to;
	struct sockaddr_in client_addr;
	struct mq_attr mqattr;
	struct http_server_t *server = (struct http_server_t *)arg;

	if ((msg_q = http_server_mq_open(server->port)) == NULL) {
		HTTP_LOGE("msg queue open fail in http_server_handler %d\n" , server->port);
		goto stop;
	}

	mq_getattr(msg_q, &mqattr);

	/*
	 * Start client accept loop
	 */
	HTTP_LOGD("Accepting connections on port %d began.\n", server->port);

	accept_to.tv_sec = ACCEPT_TIMEOUT_MS / 1000;
	accept_to.tv_usec = (ACCEPT_TIMEOUT_MS % 1000) * 1000;
	if (setsockopt(server->listen_fd, SOL_SOCKET, SO_RCVTIMEO,
				   (struct timeval *)&accept_to, sizeof(struct timeval)) < 0) {
		HTTP_LOGE("Error: Fail to setsockopt\n");
	}

	server->state = HTTP_SERVER_RUN;

	while (server->state == HTTP_SERVER_RUN) {
		addrlen = sizeof(struct sockaddr_in);
		sock_fd = accept(*(volatile int *)&server->listen_fd,
						 (struct sockaddr *)&client_addr,
						 &addrlen);
		if (sock_fd < 0 && (errno != EWOULDBLOCK && errno != ETIMEDOUT)) {
			HTTP_LOGE("Error: Accept client error!!\n");
			continue;
		}

		if (sock_fd > 0) {
			tv.tv_sec = HTTP_CONF_SOCKET_TIMEOUT_MSEC / 1000;
			tv.tv_usec = (HTTP_CONF_SOCKET_TIMEOUT_MSEC % 1000) * 1000;
			HTTP_LOGD("Set timeout to socket (%d.%d)sec\n", tv.tv_sec, tv.tv_usec);
			if (setsockopt(sock_fd, SOL_SOCKET, SO_RCVTIMEO,
						   (struct timeval *)&tv, sizeof(struct timeval)) < 0) {
				HTTP_LOGE("Error: Fail to setsockopt\n");
			}

			HTTP_LOGD("Client %d is accepted ipaddr: %d.%d.%d.%d\n", sock_fd,
					  (int)((client_addr.sin_addr.s_addr & 0xFF)),
					  (int)((client_addr.sin_addr.s_addr & 0xFF00) >> 8),
					  (int)((client_addr.sin_addr.s_addr & 0xFF0000) >> 16),
					  (int)((client_addr.sin_addr.s_addr & 0xFF000000) >> 24));
			for (i = 0; i < MAX_ACCEPTED_FD; ++i) {
				if (!fdarr[i]) {
					if (sock_fd > maxfd) {
						maxfd = sock_fd;
					}
					fdarr[i] = sock_fd;
					++fdcnt;
					break;
				}
				if (i + 1 == MAX_ACCEPTED_FD) {
					HTTP_LOGE("Error: Too many request be piled\n");
				}
			}
		}

		if (fdcnt > 0) {
			cnt = 0;
			FD_ZERO(&readfds);
			for (i = 0; i < MAX_ACCEPTED_FD; ++i) {
				if (fdarr[i]) {
					FD_SET(fdarr[i], &readfds);
					++cnt;
				}
				if (cnt == fdcnt) {
					break;
				}
			}
			ret = select(maxfd + 1, &readfds, (fd_set *)0, (fd_set *)0, NULL);
			if (ret > 0) {
				cnt = 0;
				for (i = 0; i < MAX_ACCEPTED_FD; ++i) {
					if (fdarr[i]) {
						if (FD_ISSET(fdarr[i], &readfds)) {
							++cnt;
							sock_fd = fdarr[i];
							fdarr[i] = 0;
							--fdcnt;

							msg.event = HTTP_CONNECT_EVENT;
							msg.data = sock_fd;

							mq_getattr(msg_q, &mqattr);
							if (mqattr.mq_curmsgs > HTTP_CONF_SERVER_MQ_MAX_MSG - 1) {
								if (sock_fd > 0) {
									close(sock_fd);
								}
								continue;
							}

							if (mq_send(msg_q, (char *)&msg, mqattr.mq_msgsize, 1) != OK) {
								HTTP_LOGE("Send Error %d\n", getpid());
								if (sock_fd > 0) {
									close(sock_fd);
								}
								continue;
							}
						}
						if (cnt == ret) {
							break;
						}
					}
				}
			}
		}
	}
stop:
	if (msg_q != NULL) {
		http_server_mq_flush(msg_q);

		for (i = 0; i < HTTP_CONF_MAX_CLIENT_HANDLE; i++) {
			msg.event = HTTP_STOP_EVENT;
			msg.data = -1;
			mq_send(msg_q, (char *)&msg, mqattr.mq_msgsize, 1);
		}

		mq_getattr(msg_q, &mqattr);

		for (i = 0; i < HTTP_CONF_MAX_CLIENT_HANDLE;  i++) {
			pthread_kill(server->c_tid[i], HTTP_CONF_SERVER_SIGWAKEUP);
		}

		mq_close(msg_q);

		if (http_server_mq_close(server->port) != OK) {
			HTTP_LOGD("mq close error %d\n", server->port);
		}
	}
	HTTP_LOGD("http_server_handler stop :%d\n", server->port);

	server->state = HTTP_SERVER_STOP;
	return NULL;
}


int http_server_start(struct http_server_t *server)
{
	pthread_attr_t attr;
	unsigned int cli_handle_stack = HTTP_CLIENT_HANDLER_STACKSIZE;
	int reuse = 1;
	int i;

	if (server == NULL) {
		HTTP_LOGE("Error: Server must be initialized before start");
		return HTTP_ERROR;
	}
	/*
	 * Initialize socket and bind, start listening
	 */

	server->listen_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server->listen_fd < 0) {
		HTTP_LOGE("Error: Cannot create socket!!\n");
		return HTTP_ERROR;
	}

	if (setsockopt(server->listen_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
		HTTP_LOGE("Error: Cannot set socket option!!\n");
	}

	server->servaddr.sin_family = AF_INET;
	server->servaddr.sin_port = HTTP_HTONS(server->port);
	server->servaddr.sin_addr.s_addr = INADDR_ANY;

	if (bind(server->listen_fd, (struct sockaddr *)&(server->servaddr), sizeof(struct sockaddr_in)) < 0) {
		HTTP_LOGE("Error: Cannot socket bind!!\n");
		close(server->listen_fd);
		return HTTP_ERROR;
	}

	if (listen(server->listen_fd, HTTP_CONF_MAX_CLIENT) < 0) {
		HTTP_LOGE("Error: Cannot listen!!\n");
		close(server->listen_fd);
		return HTTP_ERROR;
	}

	if (pthread_attr_init(&attr) != 0) {
		HTTP_LOGE("Error: Cannot initialize ptread attribute\n");
		return HTTP_ERROR;
	}
	pthread_attr_setschedpolicy(&attr, SCHED_RR);
	pthread_attr_setstacksize(&attr, HTTP_LISTENING_HANDLER_STACKSIZE);

	if (pthread_create(&server->tid, &attr, http_server_handler, (void *)server) != 0) {
		HTTP_LOGE("Error: Cannot create server thread!!\n");
		return HTTP_ERROR;
	}
	pthread_setname_np(server->tid, "listening webserver");
	pthread_detach(server->tid);

#ifdef CONFIG_NET_SECURITY_TLS
	if (server->tls_init) {
		cli_handle_stack = HTTPS_CLIENT_HANDLER_STACKSIZE;
	}
#endif

	for (i = 0; i < HTTP_CONF_MAX_CLIENT_HANDLE; i++) {
		if (pthread_attr_init(&attr) != 0) {
			HTTP_LOGE("Error: Cannot initialize thread attribute\n");
			return HTTP_ERROR;
		}
		pthread_attr_setschedpolicy(&attr, SCHED_RR);
		pthread_attr_setstacksize(&attr, cli_handle_stack);
		if (pthread_create(&server->c_tid[i], &attr, http_handle_client, (void *)server) != 0) {
			HTTP_LOGE("Error: Cannot create server thread!!\n");
			return HTTP_ERROR;
		}
		pthread_setname_np(server->c_tid[i], "client handler");
		pthread_detach(server->c_tid[i]);
	}

	return HTTP_OK;
}
