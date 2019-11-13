/* ***************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
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
/*******************************************************************************
 * Copyright (c) 2014, 2015 IBM Corp.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v10.html
 * and the Eclipse Distribution License is available at
 *   http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    Allan Stockdill-Mander - initial API and implementation and/or initial documentation
 *    Ian Craggs - convert to FreeRTOS
 *******************************************************************************/

#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "iot_os_net.h"
#include "iot_debug.h"

void iot_os_net_print_status(iot_net_socket *n)
{
	struct timeval tv, timeout = {0,};
	int sock_err = 0;
	socklen_t err_len = sizeof(sock_err);
	fd_set rfdset, wfdset;

	FD_ZERO(&rfdset);
	FD_ZERO(&wfdset);
	FD_SET(n->socket, &rfdset);
	FD_SET(n->socket, &wfdset);

	select(n->socket + 1, &rfdset, &wfdset, NULL, &timeout);
	getsockopt(n->socket, SOL_SOCKET, SO_ERROR, &sock_err, &err_len);
	gettimeofday(&tv, NULL);

	IOT_INFO("[%ld] Socket Network Status readable %d writable %d sock_err %d errno %d", tv.tv_sec,
			FD_ISSET(n->socket, &rfdset), FD_ISSET(n->socket, &wfdset), sock_err, errno);
}

int iot_os_net_select(iot_net_socket *n, unsigned int timeout_ms)
{
	int ret = 0;
	struct timeval timeout;
	fd_set fdset;

	FD_ZERO(&fdset);
	FD_SET(n->socket, &fdset);

	timeout.tv_sec = timeout_ms / 1000;
	timeout.tv_usec = (timeout_ms % 1000) * 1000;

#ifdef CONFIG_STDK_MQTT_USE_SSL
	if ((n->ssl && SSL_pending(n->ssl) > 0) || select(n->socket + 1, &fdset, NULL, NULL, &timeout) > 0) {
		ret = 1;
	}
#else
	ret = select(n->socket + 1, &fdset, NULL, NULL, &timeout);
#endif

	return ret;
}

static int iot_os_net_read(iot_net_socket *n, unsigned char *buffer, unsigned int len, iot_os_timer timer)
{
	int recvLen = 0;
	int rc = 0;
	int ret = 0;

	struct timeval timeout;
	fd_set fdset;

	FD_ZERO(&fdset);
	FD_SET(n->socket, &fdset);

	timeout.tv_sec = 0;
	timeout.tv_usec = iot_os_timer_left_ms(timer) * 1000;

	ret = select(n->socket + 1, &fdset, NULL, NULL, &timeout);

	if (ret <= 0) {
		IOT_WARN("iot_os_net_read:select return %d\n", ret);
		return ret;
	}

	if (FD_ISSET(n->socket, &fdset)) {
		do {
			rc = recv(n->socket, buffer + recvLen, len - recvLen, MSG_DONTWAIT);

			if (rc > 0) {
				recvLen += rc;
			} else if (rc <= 0) {
				int error = 0;
				socklen_t err_len = sizeof(error);
				getsockopt(n->socket, SOL_SOCKET, SO_ERROR, &error, &err_len);
				if (error != 0) {
					recvLen = -1;
					IOT_WARN("recv error %d\n", error);
					break;
				}
			}
		} while (recvLen < len && !iot_os_timer_isexpired(timer));
	}

	return recvLen;
}

static int iot_os_net_write(iot_net_socket *n, unsigned char *buffer, unsigned int len, iot_os_timer timer)
{
	int sentLen = 0;
	int rc = 0;
	int ret = 0;

	struct timeval timeout;
	fd_set fdset;

	FD_ZERO(&fdset);
	FD_SET(n->socket, &fdset);

	timeout.tv_sec = 0;
	timeout.tv_usec = iot_os_timer_left_ms(timer) * 1000;

	ret = select(n->socket + 1, NULL, &fdset, NULL, &timeout);

	if (ret <= 0) {
		IOT_WARN("iot_os_net_write:select return %d\n", ret);
		return ret;
	}

	if (FD_ISSET(n->socket, &fdset)) {
		do {
			rc = send(n->socket, buffer + sentLen, len - sentLen, 0);

			if (rc > 0) {
				sentLen += rc;
			} else if (rc <= 0) {
				int error = 0;
				socklen_t err_len = sizeof(error);
				getsockopt(n->socket, SOL_SOCKET, SO_ERROR, &error, &err_len);
				if (error != 0) {
					sentLen = -1;
					IOT_WARN("send error %d\n", error);
					break;
				}
			}
		} while (sentLen < len && !iot_os_timer_isexpired(timer));
	}

	return sentLen;
}

void iot_os_net_disconnect(iot_net_socket *n)
{
	close(n->socket);
}

int iot_os_net_connet(iot_net_socket *n, char *addr, int port)
{
	struct sockaddr_in sAddr;
	int retVal = -1;
	struct hostent *ipAddress;

	if ((ipAddress = gethostbyname(addr)) == 0) {
		goto exit;
	}

	sAddr.sin_family = AF_INET;
	sAddr.sin_addr.s_addr = ((struct in_addr *)(ipAddress->h_addr))->s_addr;
	sAddr.sin_port = htons(port);

	if ((n->socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		goto exit;
	}

	if ((retVal = connect(n->socket, (struct sockaddr *)&sAddr, sizeof(sAddr))) < 0) {
		close(n->socket);
		goto exit;
	}

exit:
	return retVal;
}

void iot_os_net_init(iot_net_socket *n)
{
	n->socket = 0;
	n->read = iot_os_net_read;
	n->write = iot_os_net_write;
#ifdef CONFIG_STDK_MQTT_USE_SSL
	n->ctx = NULL;
	n->ssl = NULL;
#endif
}

#ifdef CONFIG_STDK_MQTT_USE_SSL

static int iot_os_net_ssl_read(iot_net_socket *n, unsigned char *buffer, unsigned int len, iot_os_timer timer)
{
	int recvLen = 0;
	int rc = 0;

	struct timeval timeout;
	fd_set fdset;

	FD_ZERO(&fdset);
	FD_SET(n->socket, &fdset);

	timeout.tv_sec = 0;
	timeout.tv_usec = iot_os_timer_left_ms(timer) * 1000;

	if (SSL_pending(n->ssl) > 0 || select(n->socket + 1, &fdset, NULL, NULL, &timeout) > 0) {
		do {
			rc = SSL_read(n->ssl, buffer + recvLen, len - recvLen);

			if (rc > 0) {
				recvLen += rc;
			} else if (rc <= 0) {
				int error = 0;
				error = SSL_get_error(n->ssl, rc);
				IOT_WARN("recv error %d %d %d\n", rc, error, errno);
				switch (error) {
				case SSL_ERROR_WANT_READ:
					if (errno == ECONNABORTED) {
						recvLen = -1;
						goto exit;
					}
					iot_os_delay(1000);
					break;
				default:
					recvLen = -1;
					goto exit;
				}
			}
		} while (recvLen < len && !iot_os_timer_isexpired(timer));
	}

exit:
	return recvLen;
}

static int iot_os_net_ssl_write(iot_net_socket *n, unsigned char *buffer, unsigned int len, iot_os_timer timer)
{
	int sentLen = 0;
	int rc = 0;
	int ret = 0;

	struct timeval timeout;
	fd_set fdset;

	FD_ZERO(&fdset);
	FD_SET(n->socket, &fdset);

	timeout.tv_sec = 0;
	timeout.tv_usec = iot_os_timer_left_ms(timer) * 1000;

	errno = 0;
	ret = select(n->socket + 1, NULL, &fdset, NULL, &timeout);

	if (ret <= 0) {
		struct timeval tv;
		int error = 0;
		socklen_t err_len = sizeof(error);
		getsockopt(n->socket, SOL_SOCKET, SO_ERROR, &error, &err_len);
		gettimeofday(&tv, NULL);
		IOT_ERROR("[%ld] Socket Network Error write_sel_rc %d sock_err %d errno %d select timeout=%ld",
			tv.tv_sec, ret, error, errno, timeout.tv_usec);
		return ret;
	}

	if (FD_ISSET(n->socket, &fdset)) {
		do {
			rc = SSL_write(n->ssl, buffer + sentLen, len - sentLen);

			if (rc > 0) {
				sentLen += rc;
			} else if (rc <= 0) {
				int error = 0;
				error = SSL_get_error(n->ssl, rc);
				IOT_WARN("write error %d %d %d\n", rc, error, errno);
				sentLen = -1;
				break;
			}
		} while (sentLen < len && !iot_os_timer_isexpired(timer));
	}

	return sentLen;
}

void iot_os_net_ssl_disconnect(iot_net_socket *n)
{
	close(n->socket);
	SSL_free(n->ssl);
	SSL_CTX_free(n->ctx);
	n->read_count = 0;
}

int iot_os_net_ssl_connet(iot_net_socket *n, char *addr, int port, ssl_ca_crt_key_t *ssl_cck, const SSL_METHOD *method, int verify_mode, unsigned int frag_len)
{
	struct sockaddr_in sAddr;
	int retVal = -1;
	struct hostent *ipAddress;
	#if defined(LWIP_SO_SNDRCVTIMEO_NONSTANDARD) && (LWIP_SO_SNDRCVTIMEO_NONSTANDARD == 0)
	struct timeval sock_timeout = {30, 0};
	#else
	unsigned int sock_timeout = 30 * 1000;
	#endif

	if ((ipAddress = gethostbyname(addr)) == 0) {
		goto exit;
	}

	n->ctx = SSL_CTX_new(method);

	if (!n->ctx) {
		goto exit;
	}

	if (ssl_cck->cacrt) {
		retVal = SSL_CTX_load_verify_buffer(n->ctx, ssl_cck->cacrt, ssl_cck->cacrt_len);

		if (retVal != 1) {
			goto exit1;
		}
	}

	if (ssl_cck->cert && ssl_cck->key) {
		retVal = SSL_CTX_use_certificate_ASN1(n->ctx, ssl_cck->cert_len, ssl_cck->cert);

		if (!retVal) {
			goto exit1;
		}

		retVal = SSL_CTX_use_PrivateKey_ASN1(0, n->ctx, ssl_cck->key, ssl_cck->key_len);

		if (!retVal) {
			goto exit1;
		}
	}

	if (ssl_cck->cacrt) {
		SSL_CTX_set_verify(n->ctx, verify_mode, NULL);
	} else {
		SSL_CTX_set_verify(n->ctx, SSL_VERIFY_NONE, NULL);
	}

	sAddr.sin_family = AF_INET;
	sAddr.sin_addr.s_addr = ((struct in_addr *)(ipAddress->h_addr))->s_addr;
	sAddr.sin_port = htons(port);

	if ((n->socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		goto exit1;
	}
	setsockopt(n->socket, SOL_SOCKET, SO_RCVTIMEO, &sock_timeout, sizeof(sock_timeout));
	setsockopt(n->socket, SOL_SOCKET, SO_SNDTIMEO, &sock_timeout, sizeof(sock_timeout));
	if ((retVal = connect(n->socket, (struct sockaddr *)&sAddr, sizeof(sAddr))) < 0) {
		goto exit2;
	}

	n->ssl = SSL_new(n->ctx);

	if (!n->ssl) {
		goto exit2;
	}

	SSL_set_fd(n->ssl, n->socket);

	if ((retVal = SSL_connect(n->ssl)) <= 0) {
		goto exit3;
	} else {
		retVal = 0;
		goto exit;
	}

exit3:
	SSL_free(n->ssl);
exit2:
	close(n->socket);
exit1:
	SSL_CTX_free(n->ctx);
	retVal = -1;
exit:
	return retVal;
}

void iot_os_net_ssl_init(iot_net_socket *n)
{
	n->socket = 0;
	n->read = iot_os_net_ssl_read;
	n->write = iot_os_net_ssl_write;
	n->read_count = 0;
	n->ctx = NULL;
	n->ssl = NULL;
}

#endif //CONFIG_STDK_MQTT_USE_SSL
