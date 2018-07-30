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
/// @file app/netutils/websocket/websocket.c
/// @brief websocket file to support client and server.

/****************************************************************************
 *  Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <fcntl.h>
#include <errno.h>
#include <netdb.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>
#include "mbedtls/sha1.h"
#include "mbedtls/base64.h"
#include <netutils/netlib.h>
#include <protocols/websocket.h>
#include <protocols/wslay/wslay.h>

/****************************************************************************
 * Definitions
 ****************************************************************************/

#define WEBSOCKET_FREE(a) do { if (a != NULL) { free(a); a = NULL; } } while (0)
#define WEBSOCKET_CLOSE(a) do { if (a >= 0) { close(a); a = -1; } } while (0)

/****************************************************************************
 * Private Types
 ****************************************************************************/

/****************************************************************************
 * Private Data
 ****************************************************************************/

websocket_t ws_srv_table[WEBSOCKET_MAX_CLIENT];

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****** websocket common functions *****/

int websocket_tls_handshake(websocket_t *data, char *hostname, int auth_mode)
{
	int r;

	/* set socket file descriptor */
	data->tls_net.fd = data->fd;

	/* set authentication mode */
	mbedtls_ssl_conf_authmode(data->tls_conf, auth_mode);

	/* default setting block mode */
	if ((r = mbedtls_net_set_block(&(data->tls_net))) != 0) {
		WEBSOCKET_DEBUG("Error: mbedtls_net_set_block returned -%4x\n", -r);
		return -1;
	}

	if ((r = mbedtls_ssl_setup(data->tls_ssl, data->tls_conf)) != 0) {
		WEBSOCKET_DEBUG("Error: mbedtls_ssl_setup returned -%4x\n", -r);
		return -1;
	}
#if WEBSOCKET_CONF_CHECK_TLS_HOSTNAME
	if (hostname != NULL) {
		if ((r = mbedtls_ssl_set_hostname(data->tls_ssl, hostname)) != 0) {
			WEBSOCKET_DEBUG("Error: mbedtls_hostname returned -%4x\n", -r);
			return -1;
		}
	}
#endif

	mbedtls_ssl_set_bio(data->tls_ssl, &(data->tls_net), mbedtls_net_send, mbedtls_net_recv, NULL);

	/* Handshake */
	WEBSOCKET_DEBUG("  . Performing the SSL/TLS handshake...");

	while ((r = mbedtls_ssl_handshake(data->tls_ssl)) != 0) {
		if (r != MBEDTLS_ERR_SSL_WANT_READ && r != MBEDTLS_ERR_SSL_WANT_WRITE) {
			WEBSOCKET_DEBUG("Error: mbedtls_ssl_handshake returned -%4x\n", -r);
			return r;
		}
	}

	WEBSOCKET_DEBUG("OK\n");
	return WEBSOCKET_SUCCESS;
}

websocket_return_t websocket_config_socket(int fd)
{
	int flags;
	int opt = 1;
	struct timeval tv;

	flags = fcntl(fd, F_GETFL, 0);
	if (flags == -1) {
		WEBSOCKET_DEBUG("fcntl GET failed\n");
		return WEBSOCKET_SOCKET_ERROR;
	}

	if (fcntl(fd, F_SETFL, flags & (~O_NONBLOCK)) == -1) {
		WEBSOCKET_DEBUG("fcntl SET failed\n");
		return WEBSOCKET_SOCKET_ERROR;
	}

	tv.tv_sec = (WEBSOCKET_SOCK_RCV_TIMEOUT / 1000);
	tv.tv_usec = ((WEBSOCKET_SOCK_RCV_TIMEOUT % 1000) * 1000);
	WEBSOCKET_DEBUG("set socket option(recv timeout sec:%d usec:%d)\n", tv.tv_sec, tv.tv_usec);
	if (setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, (FAR const void *)&tv, (socklen_t) sizeof(struct timeval)) == -1) {
		WEBSOCKET_DEBUG("setsockopt failed\n");
		return WEBSOCKET_SOCKET_ERROR;
	}

	if (setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &opt, sizeof(opt)) == -1) {
		WEBSOCKET_DEBUG("setsockopt failed\n");
		return WEBSOCKET_SOCKET_ERROR;
	}

	return WEBSOCKET_SUCCESS;
}

websocket_return_t websocket_wait_state(websocket_t *websocket, int state, int utime)
{
	if (websocket == NULL) {
		WEBSOCKET_DEBUG("NULL parameter\n");
		return WEBSOCKET_ALLOCATION_ERROR;
	}

	if (state >= WEBSOCKET_STOP && state < WEBSOCKET_MAX_STATE) {
		while ((websocket->state) != state) {
			usleep(utime);
		}
	} else {
		WEBSOCKET_DEBUG("function returned for invalid parameter\n");
	}

	return WEBSOCKET_SUCCESS;
}

int websocket_ping_counter(websocket_t *websocket)
{
	if (websocket->state != WEBSOCKET_STOP) {
		websocket->ping_cnt++;

		if (websocket->ping_cnt >= WEBSOCKET_MAX_PING_IGNORE) {
			WEBSOCKET_DEBUG("ping messages couldn't receive pong messages for %d times, closing.\n", WEBSOCKET_MAX_PING_IGNORE);
			return WEBSOCKET_SOCKET_ERROR;
		}
		websocket_queue_ping(websocket);
	}

	return WEBSOCKET_SUCCESS;
}

int websocket_handler(websocket_t *websocket)
{
	int r;
	int fd = websocket->fd;
	int timeout = 0;
	fd_set read_fds;
	fd_set write_fds;
	wslay_event_context_ptr ctx = (wslay_event_context_ptr) websocket->ctx;
	struct timeval tv;

	while (websocket->state != WEBSOCKET_STOP) {
		FD_ZERO(&read_fds);
		FD_ZERO(&write_fds);

		if (wslay_event_want_read(ctx)) {
			FD_SET(fd, &read_fds);
		}
		if (wslay_event_want_write(ctx)) {
			FD_SET(fd, &write_fds);
		}

		tv.tv_sec = (WEBSOCKET_HANDLER_TIMEOUT / 1000);
		tv.tv_usec = ((WEBSOCKET_HANDLER_TIMEOUT % 1000) * 1000);
		r = select(fd + 1, &read_fds, &write_fds, NULL, &tv);
		if (r < 0) {
			if (errno == EAGAIN || errno == EBUSY || errno == EINTR) {
				continue;
			}

			WEBSOCKET_DEBUG("select function returned errno == %d\n", errno);
			return WEBSOCKET_SOCKET_ERROR;
		} else if (r == 0) {
			if (WEBSOCKET_HANDLER_TIMEOUT != 0) {
				timeout++;
				if ((WEBSOCKET_HANDLER_TIMEOUT * timeout) >= (WEBSOCKET_PING_INTERVAL * 10)) {
					timeout = 0;
					if (websocket_ping_counter(websocket) != WEBSOCKET_SUCCESS) {
						return WEBSOCKET_SOCKET_ERROR;
					}
				}
			}

			continue;
		} else {
			timeout = 0;

			if (FD_ISSET(fd, &read_fds)) {
				r = wslay_event_recv(ctx);
				if (r != WEBSOCKET_SUCCESS) {
					WEBSOCKET_DEBUG("fail to process recv event, result : %d\n", r);
					websocket_update_state(websocket, WEBSOCKET_ERROR);
					return WEBSOCKET_SOCKET_ERROR;
				}
			}

			if (FD_ISSET(fd, &write_fds)) {
				r = wslay_event_send(ctx);
				if (r != WEBSOCKET_SUCCESS) {
					WEBSOCKET_DEBUG("fail to process send event, result : %d\n", r);
					websocket_update_state(websocket, WEBSOCKET_ERROR);
					return WEBSOCKET_SOCKET_ERROR;
				}
			}
		}
	}

	return WEBSOCKET_SUCCESS;
}

/***** websocket client oriented sources *****/

int websocket_client_handshake(websocket_t *client, char *host, char *port, char *path)
{
	int r = WEBSOCKET_SUCCESS;
	int fd = client->fd;
	size_t header_length = 0;
	size_t header_sent = 0;
	size_t key_len;
	char *header = NULL;
	char *keyhdstart;
	char *keyhdend;
	unsigned char tmp_buf[16];
	unsigned char client_key[WEBSOCKET_CLIENT_KEY_LEN + 1];
	unsigned char accept_key[WEBSOCKET_ACCEPT_KEY_LEN];
	unsigned char dst[WEBSOCKET_ACCEPT_KEY_LEN];

	header = (char *)calloc(WEBSOCKET_HANDSHAKE_HEADER_SIZE, sizeof(char));
	if (header == NULL) {
		WEBSOCKET_DEBUG("fail to allocate memory for header\n");
		return WEBSOCKET_ALLOCATION_ERROR;
	}

	memset(tmp_buf, rand(), sizeof(tmp_buf));
	memset(client_key, 0, WEBSOCKET_CLIENT_KEY_LEN + 1);
	if ((r = mbedtls_base64_encode(client_key, WEBSOCKET_CLIENT_KEY_LEN + 1, &key_len, tmp_buf, sizeof(tmp_buf))) != 0) {
		WEBSOCKET_DEBUG("mbedtls error %d\n", r);
		goto EXIT_WEBSOCKET_HANDSHAKE_ERROR;
	}
	client_key[WEBSOCKET_CLIENT_KEY_LEN] = '\0';

	snprintf(header, WEBSOCKET_HANDSHAKE_HEADER_SIZE, "GET %s HTTP/1.1\r\n" "Host: %s:%s\r\n" "Upgrade: websocket\r\n" "Connection: Upgrade\r\n" "Sec-WebSocket-Key: %s\r\n" "Sec-WebSocket-Version: 13\r\n" "\r\n", path, host, port, client_key);
	header_length = strlen(header);

	while (header_sent < header_length) {
		if (client->tls_enabled) {
			r = mbedtls_ssl_write(client->tls_ssl, (const unsigned char *)(header + header_sent), header_length - header_sent);
		} else {
			r = write(fd, header + header_sent, header_length - header_sent);
		}
		if (r < 0) {
			WEBSOCKET_DEBUG("fail to write socket errno = %d\n", errno);
			goto EXIT_WEBSOCKET_HANDSHAKE_ERROR;
		} else {
			header_sent += r;
		}
	}

	memset(header, 0, WEBSOCKET_HANDSHAKE_HEADER_SIZE);
	header_length = 0;
	while (1) {
		if (client->tls_enabled) {
			r = mbedtls_ssl_read(client->tls_ssl, (unsigned char *)(header + header_length), WEBSOCKET_HANDSHAKE_HEADER_SIZE - header_length);
		} else {
			r = read(fd, header + header_length, WEBSOCKET_HANDSHAKE_HEADER_SIZE - header_length);
		}
		if (r < 0) {
			WEBSOCKET_DEBUG("fail to read socket errno = %d\n", errno);
			goto EXIT_WEBSOCKET_HANDSHAKE_ERROR;
		} else if (r == 0) {
			WEBSOCKET_DEBUG("Got EOF\n");
			goto EXIT_WEBSOCKET_HANDSHAKE_ERROR;
		} else {
			header_length += r;
			if (header_length >= 4 && memcmp(header + header_length - 4, "\r\n\r\n", 4) == 0) {
				break;
			} else if (header_length >= WEBSOCKET_HANDSHAKE_HEADER_SIZE) {
				WEBSOCKET_DEBUG("Too large HTTP headers\n");
				goto EXIT_WEBSOCKET_HANDSHAKE_ERROR;
			}
		}
	}

	if ((keyhdstart = strstr(header, "Sec-WebSocket-Accept: ")) == NULL) {
		WEBSOCKET_DEBUG("http_upgrade: missing required headers\n");
		goto EXIT_WEBSOCKET_HANDSHAKE_ERROR;
	}
	keyhdstart += 22;
	keyhdend = strstr(keyhdstart, "\r\n");
	if (keyhdend == NULL) {
		WEBSOCKET_DEBUG("http_upgrade: missing required headers\n");
		goto EXIT_WEBSOCKET_HANDSHAKE_ERROR;
	}

	memset(accept_key, 0, WEBSOCKET_ACCEPT_KEY_LEN);
	if (keyhdend - keyhdstart > WEBSOCKET_CLIENT_KEY_LEN) {
		WEBSOCKET_DEBUG("error key length\n");
		goto EXIT_WEBSOCKET_HANDSHAKE_ERROR;
	}
	memcpy(accept_key, keyhdstart, keyhdend - keyhdstart);
	websocket_create_accept_key(dst, WEBSOCKET_ACCEPT_KEY_LEN, client_key, WEBSOCKET_CLIENT_KEY_LEN);
	accept_key[WEBSOCKET_ACCEPT_KEY_LEN - 1] = '\0';

	if (memcmp(accept_key, dst, WEBSOCKET_ACCEPT_KEY_LEN) != 0) {
		WEBSOCKET_DEBUG("invalid key\n");
		goto EXIT_WEBSOCKET_HANDSHAKE_ERROR;
	}
	WEBSOCKET_FREE(header);
	return WEBSOCKET_SUCCESS;
EXIT_WEBSOCKET_HANDSHAKE_ERROR:
	WEBSOCKET_FREE(header);
	return WEBSOCKET_HANDSHAKE_ERROR;
}

int connect_socket(websocket_t *client, const char *host, const char *port)
{
	int fd;
	socklen_t addrlen;
	struct sockaddr_in serveraddr;

#ifdef CONFIG_NET_LWIP_NETDB
	struct hostent *he = NULL;
	char ip_str[INET6_ADDRSTRLEN];

	/* Resolve hostname to IP address */
	he = gethostbyname(host);
	if (!he || (he->h_addrtype != AF_INET)) {
		WEBSOCKET_DEBUG("failed to resolve hostname\n");
		return WEBSOCKET_CONNECT_ERROR;
	}
	if (inet_ntop(he->h_addrtype, he->h_addr, ip_str, sizeof(ip_str)) == NULL) {
		WEBSOCKET_DEBUG("inet_ntop failed (errno=%d)\n", errno);
		return WEBSOCKET_CONNECT_ERROR;
	}
#endif
	fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (fd == -1) {
		WEBSOCKET_DEBUG("fail to open socket\n");
		return WEBSOCKET_CONNECT_ERROR;
	}

	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(atoi(port));
#ifdef CONFIG_NET_LWIP_NETDB
	serveraddr.sin_addr.s_addr = inet_addr(ip_str);
#else
	serveraddr.sin_addr.s_addr = inet_addr(host);
#endif
	addrlen = sizeof(struct sockaddr);
	if (connect(fd, (struct sockaddr *)&serveraddr, addrlen) < 0) {
		WEBSOCKET_DEBUG("fail to connect socket (errno=%d)\n", errno);
		WEBSOCKET_CLOSE(fd);
		return WEBSOCKET_CONNECT_ERROR;
	}
	client->fd = fd;
	WEBSOCKET_DEBUG("connect server, fd == %d\n", fd);

	return WEBSOCKET_SUCCESS;
}

int websocket_connect(websocket_t *client, char *host, const char *port)
{
	int r;
	int tls_hs_retry = WEBSOCKET_MAX_TLS_HANDSHAKE;

TLS_HS_RETRY:
	if ((r = connect_socket(client, host, port)) != WEBSOCKET_SUCCESS) {
		return r;
	}

	if (websocket_config_socket(client->fd) != WEBSOCKET_SUCCESS) {
		WEBSOCKET_CLOSE(client->fd);
		return WEBSOCKET_SOCKET_ERROR;
	}

	if (client->tls_enabled) {
		if ((r = websocket_tls_handshake(client, host, client->auth_mode)) != WEBSOCKET_SUCCESS) {
			if (r == MBEDTLS_ERR_NET_SEND_FAILED || r == MBEDTLS_ERR_NET_RECV_FAILED || r == MBEDTLS_ERR_SSL_CONN_EOF) {
				if (tls_hs_retry-- > 0) {
					WEBSOCKET_DEBUG("Handshake again.... \n");
					mbedtls_net_free(&(client->tls_net));
					mbedtls_ssl_free(client->tls_ssl);
					mbedtls_ssl_init(client->tls_ssl);
					goto TLS_HS_RETRY;
				}
			}
			return WEBSOCKET_TLS_HANDSHAKE_ERROR;
		}
	}
	return r;
}

/***** websocket server oriented sources *****/

int websocket_server_handshake(websocket_t *server)
{
	int fd = server->fd;
	size_t header_length = 0;
	size_t header_sent = 0;
	ssize_t r;
	char *header = NULL;
	char *keyhdstart, *keyhdend;
	unsigned char client_key[WEBSOCKET_CLIENT_KEY_LEN];
	unsigned char accept_key[WEBSOCKET_ACCEPT_KEY_LEN];

	header = calloc(WEBSOCKET_HANDSHAKE_HEADER_SIZE, sizeof(char));
	if (header == NULL) {
		WEBSOCKET_DEBUG("fail to allocate memory for header\n");
		return WEBSOCKET_HANDSHAKE_ERROR;
	}

	while (1) {
		if (server->tls_enabled) {
			r = mbedtls_ssl_read(server->tls_ssl, (unsigned char *)(header + header_length), WEBSOCKET_HANDSHAKE_HEADER_SIZE - header_length);
		} else {
			r = read(fd, header + header_length, WEBSOCKET_HANDSHAKE_HEADER_SIZE - header_length);
		}
		if (r < 0) {
			WEBSOCKET_DEBUG("fail to read socket errno = %d\n", errno);
			goto EXIT_WEBSOCKET_HANDSHAKE_ERROR;
		} else if (r == 0) {
			WEBSOCKET_DEBUG("HTTP Handshake: Got EOF\n");
			goto EXIT_WEBSOCKET_HANDSHAKE_ERROR;
		} else {
			header_length += r;
			if (header_length >= 4 && memcmp(header + header_length - 4, "\r\n\r\n", 4) == 0) {
				break;
			} else if (header_length >= WEBSOCKET_HANDSHAKE_HEADER_SIZE) {
				WEBSOCKET_DEBUG("HTTP Handshake: Too large HTTP headers\n");
				goto EXIT_WEBSOCKET_HANDSHAKE_ERROR;
			}
		}
	}

	if (strstr(header, "Upgrade: websocket") == NULL || strstr(header, "Connection: Upgrade") == NULL) {
		WEBSOCKET_DEBUG("HTTP handshake: Missing required header fields\n");
		goto EXIT_WEBSOCKET_HANDSHAKE_ERROR;
	}
	if ((keyhdstart = strstr(header, "Sec-WebSocket-Key: ")) == NULL) {
		WEBSOCKET_DEBUG("http_upgrade: missing required headers\n");
		goto EXIT_WEBSOCKET_HANDSHAKE_ERROR;
	}
	keyhdstart += 19;

	keyhdend = strstr(keyhdstart, "\r\n");
	if (keyhdend == NULL) {
		WEBSOCKET_DEBUG("http_upgrade: missing required headers\n");
		goto EXIT_WEBSOCKET_HANDSHAKE_ERROR;
	}

	memset(client_key, 0, WEBSOCKET_CLIENT_KEY_LEN);
	if (keyhdend - keyhdstart > WEBSOCKET_CLIENT_KEY_LEN) {
		WEBSOCKET_DEBUG("error key length\n");
		goto EXIT_WEBSOCKET_HANDSHAKE_ERROR;
	}
	memcpy(client_key, keyhdstart, keyhdend - keyhdstart);
	memset(accept_key, 0, WEBSOCKET_ACCEPT_KEY_LEN);
	websocket_create_accept_key(accept_key, WEBSOCKET_ACCEPT_KEY_LEN, client_key, WEBSOCKET_CLIENT_KEY_LEN);

	memset(header, 0, WEBSOCKET_HANDSHAKE_HEADER_SIZE);
	snprintf(header, WEBSOCKET_HANDSHAKE_HEADER_SIZE, "HTTP/1.1 101 Switching Protocols\r\n" "Upgrade: websocket\r\n" "Connection: Upgrade\r\n" "Sec-WebSocket-Accept: %s\r\n" "\r\n", accept_key);
	header_length = strlen(header);

	while (header_sent < header_length) {
		if (server->tls_enabled) {
			r = mbedtls_ssl_write(server->tls_ssl, (const unsigned char *)(header + header_sent), header_length - header_sent);
		} else {
			r = write(fd, header + header_sent, header_length - header_sent);
		}
		if (r < 0) {
			WEBSOCKET_DEBUG("fail to write socket errno = %d\n", errno);
			goto EXIT_WEBSOCKET_HANDSHAKE_ERROR;
		} else {
			header_sent += r;
		}
	}
	WEBSOCKET_FREE(header);
	return WEBSOCKET_SUCCESS;

EXIT_WEBSOCKET_HANDSHAKE_ERROR:
	WEBSOCKET_FREE(header);
	return WEBSOCKET_HANDSHAKE_ERROR;
}

int websocket_server_authenticate(websocket_t *server)
{
	int r;

	if (server->tls_enabled) {
		server->tls_ssl = malloc(sizeof(mbedtls_ssl_context));
		if (server->tls_ssl == NULL) {
			WEBSOCKET_DEBUG("fail to allocate memory for server\n");
			r = WEBSOCKET_ALLOCATION_ERROR;
			goto EXIT_SERVER_START;
		}

		mbedtls_ssl_init(server->tls_ssl);
		mbedtls_net_init(&(server->tls_net));

		if ((r = websocket_tls_handshake(server, NULL, server->auth_mode)) != WEBSOCKET_SUCCESS) {
			WEBSOCKET_DEBUG("fail to tls handshake\n");
			r = WEBSOCKET_TLS_HANDSHAKE_ERROR;
			goto EXIT_SERVER_START;
		}
	}

	if (websocket_server_handshake(server) != WEBSOCKET_SUCCESS) {
		WEBSOCKET_DEBUG("fail to handshake\n");
		r = WEBSOCKET_HANDSHAKE_ERROR;
		goto EXIT_SERVER_START;
	}

	return websocket_server_init(server);

EXIT_SERVER_START:
	WEBSOCKET_CLOSE(server->fd);

	if (server->tls_enabled) {
		mbedtls_ssl_free(server->tls_ssl);
		mbedtls_net_free(&(server->tls_net));
		WEBSOCKET_FREE(server->tls_ssl);
	}

	websocket_update_state(server, WEBSOCKET_STOP);

	return r;
}

int websocket_accept_handler(websocket_t *init_server)
{
	int i;
	int r = WEBSOCKET_SUCCESS;
	int timeout_cnt = 0;
	int listen_fd = init_server->fd;
	fd_set init_server_read_fds;
	socklen_t addrlen = sizeof(struct sockaddr);
	struct timeval tv;
	struct sockaddr_in clientaddr;
	struct sched_param ws_sparam;

	for (i = 0; i < WEBSOCKET_MAX_CLIENT; i++) {
		ws_srv_table[i].state = WEBSOCKET_STOP;
	}

	init_server->state = WEBSOCKET_RUN_SERVER;
	while (init_server->state != WEBSOCKET_STOP) {
		FD_ZERO(&init_server_read_fds);
		FD_SET(listen_fd, &init_server_read_fds);

		tv.tv_sec = WEBSOCKET_ACCEPT_TIMEOUT / 1000;
		tv.tv_usec = (WEBSOCKET_ACCEPT_TIMEOUT & 1000) * 1000;
		r = select(listen_fd + 1, &init_server_read_fds, NULL, NULL, &tv);
		if (r == -1) {
			if (errno == EINVAL) {
				WEBSOCKET_DEBUG("socket fd is not exist, init_server closing\n");
				break;
			}
			if (errno == EINTR || errno == 0) {
				continue;
			}
			WEBSOCKET_DEBUG("init_server select function returned errno == %d\n", errno);
			continue;
		} else if (r == 0) {
			for (i = 0; i < WEBSOCKET_MAX_CLIENT; i++) {
				if (ws_srv_table[i].state) {
					break;
				}
			}
			if (i == WEBSOCKET_MAX_CLIENT) {
				timeout_cnt++;
				if (timeout_cnt * WEBSOCKET_ACCEPT_TIMEOUT >= WEBSOCKET_SERVER_TIMEOUT) {
					WEBSOCKET_DEBUG("websocket server is inactive for %d msec, closing.\n", WEBSOCKET_SERVER_TIMEOUT);
					r = WEBSOCKET_SUCCESS;
					goto EXIT_INIT_SERVER;
				}
			} else {
				timeout_cnt = 0;
				continue;
			}
		} else {
			int accept_fd = -1;
			websocket_t *server_handler = NULL;

			timeout_cnt = 0;
			r = WEBSOCKET_SUCCESS;

			/* finds empty websocket server structure */
			server_handler = websocket_find_table();
			if (server_handler == NULL) {
				WEBSOCKET_DEBUG("fail to find empty server table\n");
				r = WEBSOCKET_INIT_ERROR;
				goto EXIT_ACCEPT;
			}

			/* To copy TLS context and websocket_call backs from init_server to server_handler */
			memcpy(server_handler, init_server, sizeof(websocket_t));

			if (pthread_attr_init(&server_handler->thread_attr) != 0) {
				WEBSOCKET_DEBUG("fail to init attribute\n");
				r = WEBSOCKET_INIT_ERROR;
				goto EXIT_ACCEPT;
			}

			if (pthread_attr_setstacksize(&server_handler->thread_attr, WEBSOCKET_STACKSIZE) != 0) {
				WEBSOCKET_DEBUG("fail to set stack size\n");
				r = WEBSOCKET_INIT_ERROR;
				goto EXIT_ACCEPT;
			}

			ws_sparam.sched_priority = WEBSOCKET_PRI;
			if (pthread_attr_setschedparam(&server_handler->thread_attr, &ws_sparam) != 0) {
				WEBSOCKET_DEBUG("fail to setschedparam\n");
				r = WEBSOCKET_INIT_ERROR;
				goto EXIT_ACCEPT;
			}

			if (pthread_attr_setschedpolicy(&server_handler->thread_attr, WEBSOCKET_SCHED_POLICY) != 0) {
				WEBSOCKET_DEBUG("fail to set scheduler policy\n");
				r = WEBSOCKET_INIT_ERROR;
				goto EXIT_ACCEPT;
			}

			accept_fd = accept(listen_fd, (struct sockaddr *)&clientaddr, &addrlen);
			if (accept_fd < 0) {
				WEBSOCKET_DEBUG("Error in accept err == %d\n", errno);
				r = WEBSOCKET_SOCKET_ERROR;
				goto EXIT_ACCEPT;
			}

			if (websocket_config_socket(accept_fd) != WEBSOCKET_SUCCESS) {
				WEBSOCKET_DEBUG("fail to config socket\n");
				r = WEBSOCKET_SOCKET_ERROR;
				goto EXIT_ACCEPT;
			}

			WEBSOCKET_DEBUG("accept client, fd == %d\n", accept_fd);
			server_handler->fd = accept_fd;

			if (pthread_create(&server_handler->thread_id, &server_handler->thread_attr, (pthread_startroutine_t) websocket_server_authenticate, (pthread_addr_t) server_handler) != 0) {
				WEBSOCKET_DEBUG("fail to create thread, fd == %d\n", accept_fd);
				r = WEBSOCKET_INIT_ERROR;
				goto EXIT_ACCEPT;
			}

			if (pthread_setname_np(server_handler->thread_id, "websocket server handler") != 0) {
				WEBSOCKET_DEBUG("fail to set thread name\n");
				r = WEBSOCKET_INIT_ERROR;
				goto EXIT_ACCEPT;
			}

			/* Detach thread in order to avoid memory leaks. */
			if (pthread_detach(server_handler->thread_id) != 0) {
				WEBSOCKET_DEBUG("fail to detach thread\n");
				r = WEBSOCKET_INIT_ERROR;
				goto EXIT_ACCEPT;
			}
EXIT_ACCEPT:
			if (r != WEBSOCKET_SUCCESS) {
				WEBSOCKET_CLOSE(accept_fd);
				websocket_update_state(server_handler, WEBSOCKET_STOP);
				goto EXIT_INIT_SERVER;
			}
		}
	}

EXIT_INIT_SERVER:
	WEBSOCKET_CLOSE(init_server->fd);
	return r;
}

int websocket_listen(int *listen_fd, int port)
{
	int val = 1;
	socklen_t addrlen;
	struct sockaddr_in serveraddr;

	*listen_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (*listen_fd == -1) {
		WEBSOCKET_DEBUG("fail to open socket\n");
		return WEBSOCKET_SOCKET_ERROR;
	}

	if (setsockopt(*listen_fd, SOL_SOCKET, SO_REUSEADDR, &val, (socklen_t) sizeof(int)) == -1) {
		WEBSOCKET_DEBUG("setsockopt fail\n");
		WEBSOCKET_CLOSE(*listen_fd);
		return WEBSOCKET_SOCKET_ERROR;
	}

	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(port);
	serveraddr.sin_addr.s_addr = INADDR_ANY;
	addrlen = sizeof(struct sockaddr);
	if (bind(*listen_fd, (struct sockaddr *)&serveraddr, addrlen) == -1) {
		WEBSOCKET_DEBUG("fail to bind socket\n");
		WEBSOCKET_CLOSE(*listen_fd);
		return WEBSOCKET_SOCKET_ERROR;
	}

	if (listen(*listen_fd, 16) == -1) {
		WEBSOCKET_DEBUG("fail to listen socket\n");
		WEBSOCKET_CLOSE(*listen_fd);
		return WEBSOCKET_SOCKET_ERROR;
	}

	return WEBSOCKET_SUCCESS;
}

/****************************************************************************
 * Global Functions
 ****************************************************************************/

void websocket_create_accept_key(unsigned char *dst, size_t dlen, const unsigned char *src, size_t slen)
{
	size_t olen;
	unsigned char sha1buf[20];
	unsigned char key_src[60];

	memcpy(key_src, src, slen);
	memcpy(key_src + slen, WS_GUID, 36);
	mbedtls_sha1(key_src, sizeof(key_src), sha1buf);
	mbedtls_base64_encode(dst, dlen, &olen, sha1buf, sizeof(sha1buf));
	dst[olen] = '\0';
}

int websocket_count_table(void)
{
	int i;
	int cnt = 0;

	for (i = 0; i < WEBSOCKET_MAX_CLIENT; i++) {
		if (ws_srv_table[i].state != WEBSOCKET_STOP) {
			cnt++;
		}
	}

	return cnt;
}

websocket_t *websocket_find_table(void)
{
	int i;

	for (i = 0; i < WEBSOCKET_MAX_CLIENT; i++) {
		if (ws_srv_table[i].state == WEBSOCKET_STOP) {
			break;
		}
	}

	if (i == WEBSOCKET_MAX_CLIENT) {
		WEBSOCKET_DEBUG("websocket clients are too many. limit : %d\n", WEBSOCKET_MAX_CLIENT);
		return NULL;
	}

	memset(&ws_srv_table[i], 0, sizeof(websocket_t));
	websocket_update_state(&ws_srv_table[i], WEBSOCKET_RUN_SERVER);
	ws_srv_table[i].fd = -1;

	return &ws_srv_table[i];
}

websocket_return_t websocket_client_open(websocket_t *client, char *host, char *port, char *path)
{
	int fd = -1;
	int r = WEBSOCKET_SUCCESS;
	struct websocket_info_t *socket_data = NULL;
	struct sched_param ws_sparam;

	if (client == NULL || host == NULL || port == NULL || path == NULL) {
		WEBSOCKET_DEBUG("NULL parameter\n");
		return WEBSOCKET_ALLOCATION_ERROR;
	}

	websocket_update_state(client, WEBSOCKET_RUN_CLIENT);

	//TODO : all TLS initializing code will be moved from example to here

	if (websocket_connect(client, host, port) != WEBSOCKET_SUCCESS) {
		r = WEBSOCKET_CONNECT_ERROR;
		goto EXIT_CLIENT_OPEN;
	}
	fd = client->fd;

	if (websocket_client_handshake(client, host, port, path) != WEBSOCKET_SUCCESS) {
		WEBSOCKET_DEBUG("fail to http handshake\n");
		r = WEBSOCKET_HANDSHAKE_ERROR;
		goto EXIT_CLIENT_OPEN;
	}

	socket_data = malloc(sizeof(struct websocket_info_t));
	if (socket_data == NULL) {
		WEBSOCKET_DEBUG("fail to allocate memory\n");
		r = WEBSOCKET_ALLOCATION_ERROR;
		goto EXIT_CLIENT_OPEN;
	}
	memset(socket_data, 0, sizeof(struct websocket_info_t));
	socket_data->data = client;

	if (wslay_event_context_client_init(&client->ctx, client->cb, socket_data) != WEBSOCKET_SUCCESS) {
		WEBSOCKET_DEBUG("fail to init websocket client context\n");
		WEBSOCKET_FREE(socket_data);
		r = WEBSOCKET_INIT_ERROR;
		goto EXIT_CLIENT_OPEN;
	}

	WEBSOCKET_DEBUG("start websocket client handling thread\n");

	if (pthread_attr_init(&client->thread_attr) != 0) {
		WEBSOCKET_DEBUG("fail to init pthread attribute\n");
		r = WEBSOCKET_ALLOCATION_ERROR;
		goto EXIT_CLIENT_OPEN;
	}

	if (pthread_attr_setstacksize(&client->thread_attr, WEBSOCKET_STACKSIZE) != 0) {
		WEBSOCKET_DEBUG("fail to set stack size\n");
		r = WEBSOCKET_ALLOCATION_ERROR;
		goto EXIT_CLIENT_OPEN;
	}
	ws_sparam.sched_priority = WEBSOCKET_PRI;

	if (pthread_attr_setschedparam(&client->thread_attr, &ws_sparam) != 0) {
		WEBSOCKET_DEBUG("fail to set priority\n");
		r = WEBSOCKET_ALLOCATION_ERROR;
		goto EXIT_CLIENT_OPEN;
	}

	if (pthread_attr_setschedpolicy(&client->thread_attr, WEBSOCKET_SCHED_POLICY) != 0) {
		WEBSOCKET_DEBUG("fail to set scheduler policy\n");
		r = WEBSOCKET_ALLOCATION_ERROR;
		goto EXIT_CLIENT_OPEN;
	}

	if (pthread_create(&client->thread_id, &client->thread_attr, (pthread_startroutine_t) websocket_handler, (pthread_addr_t) client) != 0) {
		WEBSOCKET_DEBUG("fail to create websocket client thread\n");
		r = WEBSOCKET_ALLOCATION_ERROR;
		goto EXIT_CLIENT_OPEN;
	}

	if (pthread_setname_np(client->thread_id, "websocket client handler") != 0) {
		WEBSOCKET_DEBUG("fail to set thread name\n");
		r = WEBSOCKET_ALLOCATION_ERROR;
		goto EXIT_CLIENT_OPEN;
	}

	if (pthread_detach(client->thread_id) != 0) {
		WEBSOCKET_DEBUG("fail to detach websocket handler thread\n");
		r = WEBSOCKET_ALLOCATION_ERROR;
		goto EXIT_CLIENT_OPEN;
	}

	return r;

EXIT_CLIENT_OPEN:
	WEBSOCKET_CLOSE(fd);

	if (client->ctx) {
		wslay_event_context_free(client->ctx);
		client->ctx = NULL;
	}

	websocket_update_state(client, WEBSOCKET_STOP);

	return r;
}

/*
 * websocket_server_open function includes:
 * 1. allocating socket fd
 * 2. accepting client
 * 3. authenticating with client
 * those 3 are not needed when websocket is initiated from http(s).
 */
websocket_return_t websocket_server_open(websocket_t *init_server)
{
	int port = 0;
	int r = WEBSOCKET_SUCCESS;

	if (init_server == NULL) {
		WEBSOCKET_DEBUG("NULL parameter\n");
		return WEBSOCKET_ALLOCATION_ERROR;
	}

	//TODO : all TLS initializing code will be moved from example to here

	port = init_server->tls_enabled ? 443 : 80;

	if (websocket_listen(&(init_server->fd), port) != WEBSOCKET_SUCCESS) {
		return WEBSOCKET_SOCKET_ERROR;
	}

	WEBSOCKET_DEBUG("websocket listen on fd == %d, port == %d\n", init_server->fd, port);

	r = websocket_accept_handler(init_server);
	if (r != WEBSOCKET_SUCCESS) {
		return r;
	}

	return WEBSOCKET_SUCCESS;
}

/*
 * if websocket server is initiated from http(s), you just can call this function.
 * see the comment of websocket_server_open to know what is different.
 */
websocket_return_t websocket_server_init(websocket_t *server)
{
	int r = WEBSOCKET_SUCCESS;
	struct websocket_info_t *socket_data = NULL;

	if (server == NULL) {
		WEBSOCKET_DEBUG("NULL parameter\n");
		return WEBSOCKET_ALLOCATION_ERROR;
	}

	socket_data = calloc(1, sizeof(struct websocket_info_t));
	if (socket_data == NULL) {
		WEBSOCKET_DEBUG("fail to allocate memory\n");
		r = WEBSOCKET_ALLOCATION_ERROR;
		goto EXIT_SERVER_INIT;
	}
	socket_data->data = server;

	if (wslay_event_context_server_init(&(server->ctx), server->cb, socket_data) != WEBSOCKET_SUCCESS) {
		WEBSOCKET_DEBUG("fail to initiate websocket server\n");
		r = WEBSOCKET_INIT_ERROR;
		goto EXIT_SERVER_INIT;
	}

	if (websocket_config_socket(server->fd) != WEBSOCKET_SUCCESS) {
		r = WEBSOCKET_SOCKET_ERROR;
		goto EXIT_SERVER_INIT;
	}

	WEBSOCKET_DEBUG("start websocket server handling loop\n");
	r = websocket_handler(server);

EXIT_SERVER_INIT:
	WEBSOCKET_CLOSE(server->fd);

	if (server->ctx) {
		wslay_event_context_free(server->ctx);
		server->ctx = NULL;
	}

	if (server->tls_enabled) {
		mbedtls_net_free(&(server->tls_net));
		mbedtls_ssl_free(server->tls_ssl);
		WEBSOCKET_FREE(server->tls_ssl);
	}

	websocket_update_state(server, WEBSOCKET_STOP);

	return r;
}

websocket_return_t websocket_register_cb(websocket_t *websocket, websocket_cb_t *cb)
{
	if (websocket == NULL) {
		WEBSOCKET_DEBUG("NULL parameter\n");
		return WEBSOCKET_ALLOCATION_ERROR;
	}

	wslay_event_config_set_callbacks(websocket->ctx, cb);

	return WEBSOCKET_SUCCESS;
}

websocket_return_t websocket_queue_msg(websocket_t *websocket, websocket_frame_t *tx_frame)
{
	if (websocket == NULL || tx_frame == NULL) {
		WEBSOCKET_DEBUG("NULL parameter\n");
		return WEBSOCKET_ALLOCATION_ERROR;
	}

	if (websocket->state == WEBSOCKET_STOP) {
		WEBSOCKET_DEBUG("websocket is not running state.\n");
		return WEBSOCKET_INIT_ERROR;
	}

	return wslay_event_queue_msg(websocket->ctx, tx_frame);
}

websocket_return_t websocket_queue_ping(websocket_t *websocket)
{
	websocket_frame_t tx_frame;

	if (websocket == NULL) {
		WEBSOCKET_DEBUG("NULL parameter\n");
		return WEBSOCKET_ALLOCATION_ERROR;
	}

	if (websocket->state == WEBSOCKET_STOP) {
		WEBSOCKET_DEBUG("websocket is not running state.\n");
		return WEBSOCKET_INIT_ERROR;
	}

	tx_frame.opcode = WEBSOCKET_PING;
	tx_frame.msg = (uint8_t *) "\0";
	tx_frame.msg_length = strlen((const char *)tx_frame.msg);

	return wslay_event_queue_msg(websocket->ctx, &tx_frame);
}

websocket_return_t websocket_queue_close(websocket_t *websocket, const char *close_message)
{
	int r = WEBSOCKET_SUCCESS;

	if (websocket == NULL) {
		WEBSOCKET_DEBUG("NULL parameter\n");
		return WEBSOCKET_ALLOCATION_ERROR;
	}

	if (close_message == NULL) {
		close_message = "\0";
	}

	if (websocket->ctx != NULL && websocket->state != WEBSOCKET_STOP) {
		if (wslay_event_queue_close(websocket->ctx, 1000, (const uint8_t *)close_message, strlen(close_message)) != WEBSOCKET_SUCCESS) {
			WEBSOCKET_DEBUG("fail to queue close message\n");
			r = WEBSOCKET_SEND_ERROR;
			goto EXIT_QUEUE_CLOSE;
		}
		websocket_wait_state(websocket, WEBSOCKET_STOP, 100000);
		WEBSOCKET_DEBUG("websocket handler successfully stopped, closing\n");
	}

EXIT_QUEUE_CLOSE:
	WEBSOCKET_CLOSE(websocket->fd);

	if (websocket->ctx) {
		wslay_event_context_free(websocket->ctx);
		websocket->ctx = NULL;
	}

	websocket_update_state(websocket, WEBSOCKET_STOP);

	return r;
}

websocket_return_t websocket_update_state(websocket_t *websocket, int state)
{
	if (websocket == NULL) {
		WEBSOCKET_DEBUG("NULL parameter\n");
		return WEBSOCKET_ALLOCATION_ERROR;
	}

	if (state >= WEBSOCKET_STOP && state < WEBSOCKET_MAX_STATE) {
		websocket->state = state;
	} else {
		WEBSOCKET_DEBUG("function returned for invalid parameter\n");
	}

	return WEBSOCKET_SUCCESS;
}

websocket_return_t websocket_set_error(websocket_t *websocket, int val)
{
	if (websocket == NULL) {
		WEBSOCKET_DEBUG("NULL parameter\n");
		return WEBSOCKET_ALLOCATION_ERROR;
	}

	wslay_event_set_error(websocket->ctx, val);

	return WEBSOCKET_SUCCESS;
}
