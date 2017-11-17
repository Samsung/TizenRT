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
/****************************************************************************
 * examples/nettest/nettest.c
 *
 *   Copyright (C) 2012 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * This is a leverage of similar logic from uIP:
 *
 *   Author: Adam Dunkels <adam@sics.se>
 *   Copyright (c) 2003, Adam Dunkels.
 *   All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <errno.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <net/if.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>

#define BUF_SIZE 256

#define NUM_PACKETS 100

#define NETTEST_IPV(x) ((x) ? "_V6" : "_V4")

#define IPM_ADDR "225.1.2.100"
#define IPM6_ADDR "FF02:0:0:0:0:0:0:100"

/****************************************************************************
 * Private Data
 ****************************************************************************/

/****************************************************************************
* Public Data
****************************************************************************/

static char *g_app_netif_addr;
static char *g_app_target_addr;
static char *g_app_target_port;

/****************************************************************************
 * Public Functions
 ****************************************************************************/

void ipmcast_sender_thread(int num_packets, int ipver)
{
	char buf[BUF_SIZE];
	int count = 1;
	int fd = -1;
	int ret = 0;
	struct addrinfo *result = NULL;
	struct addrinfo *rp = NULL;
	struct addrinfo hints;

	/* write information for getaddrinfo() */
	memset(&hints, 0, sizeof(hints));
	hints.ai_flags  = AI_NUMERICHOST;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_protocol = IPPROTO_IP;
#if LWIP_IPV6
	if (ipver == 1) {
		hints.ai_family = AF_INET6;
	} else
#endif
	{
		hints.ai_family = AF_INET;
	}

	/* get address information */
	ret = getaddrinfo((ipver == 1) ? IPM6_ADDR : IPM_ADDR, g_app_target_port, &hints, &result);
	if (ret != 0) {
		printf("[IPMSEND%s] getaddrinfo ret: %d\n", NETTEST_IPV(ipver), ret);
		exit(1);
	}

	/* try to find valid socket from the result */
	for (rp = result; rp != NULL; rp = rp->ai_next) {
		fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
		if (fd >= 0) {
			/* success */
			break;
		}
	}
	if (rp == NULL) {
		/* opening socket is totally failed */
		printf("[IPMSEND%s] socket err: %d", NETTEST_IPV(ipver), errno);
		goto err_out;
	}

	printf("[IPMSEND%s] addr: %s netif_addr: %s port: %s\n", NETTEST_IPV(ipver), (ipver == 1) ? IPM6_ADDR : IPM_ADDR, g_app_netif_addr, g_app_target_port);

	memset(buf, 0, BUF_SIZE);
	snprintf(buf, sizeof("hello, IPM_VX"), "hello, IPM%s", NETTEST_IPV(ipver));

	while (1) {
		ret = sendto(fd, buf, BUF_SIZE, 0, result->ai_addr, result->ai_addrlen);
		if (ret <= 0) {
			printf("[IPMSEND%s] sendto err: %d\n", NETTEST_IPV(ipver), errno);
			goto err_out;
		}

		if (num_packets == 0) {
			printf("[IPMSEND%s infinite test #%d] send msg(%s) len(%d)\n", NETTEST_IPV(ipver), count, buf, ret);
		} else {
			printf("[IPMSEND%s test #%d] send msg(%s) len(%d)\n", NETTEST_IPV(ipver), count, buf, ret);
			if (count >= num_packets) {
				printf("[IPMSEND%s test done #%d]\n", NETTEST_IPV(ipver), count);
				break;
			}
		}
		count++;
		sleep(1);
	}

	close(fd);
	freeaddrinfo(result);
	return;
err_out:
	if (fd >= 0) {
		close(fd);
		fd = -1;
	}
	if (result != NULL) {
		freeaddrinfo(result);
		result = NULL;
	}
	exit(1);
}

void ipmcast_receiver_thread(int num_packets, int ipver)
{
	char buf[BUF_SIZE];
	int count = 1;
	int fd = -1;
	int ret = 0;
	struct addrinfo *result = NULL;
	struct addrinfo *rp = NULL;
	struct addrinfo hints;
	struct ip_mreq groupaddr;
	struct timeval tv;
#if LWIP_IPV6
	ip6_addr_t ipm6_if;
	ip6_addr_t groupaddr6;
#endif

	/* write information for getaddrinfo() */
	memset(&hints, 0, sizeof(hints));
	hints.ai_flags  = AI_PASSIVE;
	hints.ai_socktype = SOCK_DGRAM;
#if LWIP_IPV6
	if (ipver == 1) {
		hints.ai_family = AF_INET6;
	} else
#endif
	{
		hints.ai_family = AF_INET;
	}

	/* get address information */
	ret = getaddrinfo(NULL, g_app_target_port, &hints, &result);
	if (ret != 0) {
		printf("[IPMRECV%s] getaddrinfo ret: %d\n", NETTEST_IPV(ipver), ret);
		exit(1);
	}

	/* try to find valid socket from the result */
	for (rp = result; rp != NULL; rp = rp->ai_next) {
		fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
		if (fd < 0) {
			/* continue if open is failed */
			continue;
		}

		if (bind(fd, (struct sockaddr *)rp->ai_addr, rp->ai_addrlen) == 0) {
			/* success */
			break;
		}
		close(fd);
	}
	if (rp == NULL) {
		/* opening socket is totally failed */
		printf("[IPMRECV%s] socket err: %d", NETTEST_IPV(ipver), errno);
		goto err_out;
	}

	tv.tv_sec = 10;
	tv.tv_usec = 0;
	ret = setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(struct timeval));
	if (ret != 0) {
		printf("[IPMRECV%s] recvtimeout setsockopt err: %d", NETTEST_IPV(ipver), errno);
		goto err_out;
	}

	/*
	 * Join the multicast groupaddr 225.1.2.100 with netif address
	 * interface.  Note that this IP_ADD_MEMBERSHIP option must be
	 * called for each local interface over which the multicast
	 * datagrams are to be received.
	 */
#if LWIP_IPV6
	if (ipver == 1) {
		(void)groupaddr;
		inet_pton(AF_INET6, IPM6_ADDR, &groupaddr6);
		inet_pton(AF_INET6, g_app_netif_addr, &ipm6_if);
		ret = mld6_joingroup(&ipm6_if, &groupaddr6);
	} else
#endif
	{
#if LWIP_IPV6
		(void)groupaddr6;
#endif
		inet_pton(AF_INET, IPM_ADDR, &groupaddr.imr_multiaddr);
		inet_pton(AF_INET, g_app_netif_addr, &groupaddr.imr_interface);
		ret = setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&groupaddr, sizeof(groupaddr));
	}
	if (ret < 0) {
		printf("[IPMRECV%s] %s err: %d", NETTEST_IPV(ipver), (ipver == 1) ? "mld6" : "igmp", errno);
		goto err_out;
	}

	printf("[IPMRECV%s] addr: %s netif_addr: %s port: %s\n", NETTEST_IPV(ipver), (ipver == 1) ? IPM6_ADDR : IPM_ADDR, g_app_netif_addr, g_app_target_port);

	/* Read from the socket. */
	while (1) {
		ret = read(fd, buf, BUF_SIZE);
		if (ret <= 0) {
			printf("[IPMRECV%s] read err: %d", NETTEST_IPV(ipver), errno);
			goto err_out;
		}

		buf[ret] = '\0';
		if (num_packets == 0) {
			printf("[IPMRECV%s infinite test #%d] read msg(%s) len(%d)\n", NETTEST_IPV(ipver), count, buf, ret);
		} else {
			printf("[IPMRECV%s test #%d] read msg(%s) len(%d)\n", NETTEST_IPV(ipver), count, buf, ret);
			if (count >= num_packets) {
				printf("[IPMRECV%s test done #%d]\n", NETTEST_IPV(ipver), count);
				break;
			}
		}
		count++;
	}

	close(fd);
	freeaddrinfo(result);
	return;
err_out:
	if (fd >= 0) {
		close(fd);
		fd = -1;
	}
	if (result != NULL) {
		freeaddrinfo(result);
		result = NULL;
	}
	exit(1);
}

void udp_server_thread(int num_packets, int ipver)
{
	char buf[BUF_SIZE];
	int count = 1;
	int fd = -1;
	int have_received = 0;
	int maxfd;
	int ret = 0;
	fd_set fds;
	fd_set rfds;
	fd_set sfds;
	socklen_t fromlen;
	struct sockaddr *from = NULL;
	struct addrinfo *result = NULL;
	struct addrinfo *rp = NULL;
	struct addrinfo hints;
	struct timeval tv;

	/* write information for getaddrinfo() */
	memset(&hints, 0, sizeof(hints));
	hints.ai_flags = AI_PASSIVE;
	hints.ai_socktype = SOCK_DGRAM;
#if LWIP_IPV6
	if (ipver == 1) {
		hints.ai_family = AF_INET6;
		fromlen = sizeof(struct sockaddr_in6);
	} else
#endif
	{
		hints.ai_family = AF_INET;
		fromlen = sizeof(struct sockaddr_in);
	}

	/* recvfrom() function will return target address in this variable */
	from = malloc(fromlen);
	if (from == NULL) {
		printf("[UDPSERV%s] malloc err: %d\n", NETTEST_IPV(ipver), errno);
		goto err_out;
	}

	/* get address information */
	ret = getaddrinfo(NULL, g_app_target_port, &hints, &result);
	if (ret != 0) {
		printf("[UDPSERV%s] getaddrinfo ret: %d\n", NETTEST_IPV(ipver), ret);
		goto err_out;
	}

	/* try to find a valid socket from the result */
	for (rp = result; rp != NULL; rp = rp->ai_next) {
		fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
		if (fd < 0) {
			/* continue if socket open is failed */
			continue;
		}

		if (bind(fd, rp->ai_addr, rp->ai_addrlen) == 0) {
			/* success */
			break;
		}
		close(fd);
	}
	if (rp == NULL) {
		/* binding socket is totally failed */
		printf("[UDPSERV%s] bind err: %d\n", NETTEST_IPV(ipver), errno);
		goto err_out;
	}

	tv.tv_sec = 10;
	tv.tv_usec = 0;
	ret = setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(struct timeval));
	if (ret != 0) {
		printf("[UDPSERV%s] recvtimeout setsockopt err: %d", NETTEST_IPV(ipver), errno);
		goto err_out;
	}

	printf("[UDPSERV%s] waiting on port %s\n", NETTEST_IPV(ipver), g_app_target_port);

	/* fds is used to initialize rfds and sfds */
	FD_ZERO(&fds);
	FD_SET(fd, &fds);

	maxfd = fd + 1;
	while (1) {
		rfds = fds;
		FD_ZERO(&sfds);
		if (have_received) {
			/* sfds should be set when it has something to send */
			sfds = fds;
		}

		ret = select(maxfd, &rfds, &sfds, NULL, &tv);
		if (ret <= 0) {
			printf("[UDPSERV%s] select err: %d\n", NETTEST_IPV(ipver), errno);
			goto err_out;
		}
		if (ret > 0) {
			if (FD_ISSET(fd, &sfds)) {
				ret = sendto(fd, buf, BUF_SIZE, 0, from, fromlen);
				if (ret <= 0) {
					printf("[UDPSERV%s] sendto err: %d\n", NETTEST_IPV(ipver), errno);
					break;
				}

				if (num_packets == 0) {
					/* infinite test */
					if (!(count % 1000)) {
						printf("[UDPSERV%s infinite test #%d] echoback msg(%s) len(%d)\n", NETTEST_IPV(ipver), count, buf, ret);
					}
				} else {
					printf("[UDPSERV%s test #%d] echoback msg(%s) len(%d)\n", NETTEST_IPV(ipver), count, buf, ret);
					if (count >= num_packets) {
						printf("[UDPSERV%s test done #%d]\n", NETTEST_IPV(ipver), count);
						break;
					}
				}
				count++;
				have_received--;
			}
			if (FD_ISSET(fd, &rfds)) {
				ret = recvfrom(fd, buf, BUF_SIZE, 0, from, &fromlen);
				if (ret <= 0) {
					printf("[UDPSERV%s] recvfrom err: %d\n", NETTEST_IPV(ipver), errno);
					break;
				}

				have_received++;
			}
		}
	}

	free(from);
	close(fd);
	freeaddrinfo(result);
	return;
err_out:
	if (from != NULL) {
		free(from);
		from = NULL;
	}
	if (fd >= 0) {
		close(fd);
		fd = -1;
	}
	if (result != NULL) {
		freeaddrinfo(result);
		result = NULL;
	}
	exit(1);
}

void udp_client_thread(int num_packets, int ipver)
{
	char sbuf[BUF_SIZE];
	char rbuf[BUF_SIZE];
	int count = 1;
	int fd = -1;
	int ret = 0;
	struct addrinfo *result;
	struct addrinfo *rp;
	struct addrinfo hints;
	struct timeval tv;

	/* write information for getaddrinfo() */
	memset(&hints, 0, sizeof(hints));
	hints.ai_socktype = SOCK_DGRAM;
#if LWIP_IPV6
	if (ipver == 1) {
		hints.ai_family = AF_INET6;
	} else
#endif
	{
		hints.ai_family = AF_INET;
	}

	/* get address information */
	ret = getaddrinfo(g_app_target_addr, g_app_target_port, &hints, &result);
	if (ret != 0) {
		printf("[UDPCLI%s] getaddrinfo ret: %d\n", NETTEST_IPV(ipver), ret);
		goto err_out;
	}

	/* try to find valid socket from the result */
	for (rp = result; rp != NULL; rp = rp->ai_next) {
		fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
		if (fd >= 0) {
			/* success */
			break;
		}
	}
	if (rp == NULL) {
		printf("[UDPCLI%s] fail to open socket\n", NETTEST_IPV(ipver));
		goto err_out;
	}

	tv.tv_sec = 10;
	tv.tv_usec = 0;
	ret = setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(struct timeval));
	if (ret != 0) {
		printf("[UDPCLI%s] recvtimeout setsockopt err: %d", NETTEST_IPV(ipver), errno);
		goto err_out;
	}

	printf("[UDPCLI%s] addr: %s port: %s\n", NETTEST_IPV(ipver), g_app_target_addr, g_app_target_port);

	memset(rbuf, 0, BUF_SIZE);
	memset(sbuf, 0, BUF_SIZE);
	snprintf(sbuf, sizeof("hello, UDP_VX"), "hello, UDP%s", NETTEST_IPV(ipver));

	while (1) {
		ret = sendto(fd, sbuf, BUF_SIZE, 0, rp->ai_addr, rp->ai_addrlen);
		if (ret <= 0) {
			printf("[UDPCLI%s] sendto err: %d\n", NETTEST_IPV(ipver), errno);
			break;
		}

		ret = recvfrom(fd, rbuf, BUF_SIZE, 0, NULL, NULL);
		if (ret <= 0) {
			printf("[UDPCLI%s] recvfrom err: %d\n", NETTEST_IPV(ipver), errno);
			break;
		}

		if (num_packets == 0) {
			/* infinite test */
			if (!(count % 1000)) {
				printf("[UDPCLI%s infinite test #%d] msg(%s) len(%d)\n", NETTEST_IPV(ipver), count, rbuf, ret);
			}
		} else {
			printf("[UDPCLI%s test #%d] msg(%s) len(%d)\n", NETTEST_IPV(ipver), count, rbuf, ret);
			if (count >= num_packets) {
				printf("[UDPCLI%s test done #%d]\n", NETTEST_IPV(ipver), count);
				break;
			}
		}
		count++;
	}

	close(fd);
	freeaddrinfo(result);
	return;
err_out:
	if (fd >= 0) {
		close(fd);
		fd = -1;
	}
	if (result != NULL) {
		freeaddrinfo(result);
		result = NULL;
	}
	exit(1);
}

void tcp_server_thread(int num_packets, int ipver)
{
	char buf[BUF_SIZE];
	int connfd = -1;
	int count = 1;
	int listenfd = -1;
	int ret = 0;
	socklen_t addrlen;
	struct sockaddr *cliaddr = NULL;
	struct addrinfo *result = NULL;
	struct addrinfo *rp = NULL;
	struct addrinfo hints;
	struct timeval tv;

	/* write information for getaddrinfo() */
	memset(&hints, 0, sizeof(hints));
	hints.ai_flags = AI_PASSIVE;
	hints.ai_socktype = SOCK_STREAM;
#if LWIP_IPV6
	if (ipver == 1) {
		hints.ai_family = AF_INET6;
		addrlen = sizeof(struct sockaddr_in6);
	} else
#endif
	{
		hints.ai_family = AF_INET;
		addrlen = sizeof(struct sockaddr_in);
	}

	cliaddr = malloc(addrlen);
	if (cliaddr == NULL) {
		printf("[TCPSERV%s] malloc err: %d\n", NETTEST_IPV(ipver), errno);
		exit(1);
	}

	/* get address information */
	ret = getaddrinfo(NULL, g_app_target_port, &hints, &result);
	if (ret != 0) {
		printf("[TCPSERV%s] getaddrinfo ret: %d\n", NETTEST_IPV(ipver), ret);
		goto err_out;
	}

	/* try to find a valid socket from the result */
	for (rp = result; rp != NULL; rp = rp->ai_next) {
		listenfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
		if (listenfd < 0) {
			/* continue if socket open is failed */
			continue;
		}

		if (bind(listenfd, rp->ai_addr, rp->ai_addrlen) == 0) {
			/* success */
			break;
		}
		close(listenfd);
	}
	if (rp == NULL) {
		printf("[TCPSERV%s] bind err: %d\n", NETTEST_IPV(ipver), errno);
		goto err_out;
	}

	/* Enable SO_REUSEADDR to allow multiple instances */
	int reuse = 1;

	ret = setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(reuse));
	if (ret < 0) {
		printf("[TCPSERV%s] reuse setsockopt err: %d\n", NETTEST_IPV(ipver), errno);
		goto err_out;
	}

	/* start listening socket fd */
	ret = listen(listenfd, 1024);
	if (ret < 0) {
		printf("[TCPSERV%s] listen err: %d\n", NETTEST_IPV(ipver), errno);
		goto err_out;
	}

	printf("[TCPSERV%s] waiting on port: %s\n", NETTEST_IPV(ipver), g_app_target_port);

	/* accept socket addrlen specifies ipv4 and ipv6 socket */
	connfd = accept(listenfd, cliaddr, &addrlen);
	if (connfd < 0) {
		printf("[TCPSERV%s] accept err: %d\n", NETTEST_IPV(ipver), errno);
		goto err_out;
	}

	tv.tv_sec = 10;
	tv.tv_usec = 0;
	ret = setsockopt(connfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(struct timeval));
	if (ret != 0) {
		printf("[TCPSERV%s] recvtimeout setsockopt err: %d", NETTEST_IPV(ipver), errno);
		goto err_out;
	}

	while (1) {
		/* echo back loop */
		ret = recv(connfd, buf, BUF_SIZE, 0);
		if (ret <= 0) {
			printf("[TCPSERV%s] recv err: %d\n", NETTEST_IPV(ipver), errno);
			break;
		}

		ret = send(connfd, buf, BUF_SIZE, 0);
		if (ret <= 0) {
			printf("[TCPSERV%s] send err: %d\n", NETTEST_IPV(ipver), errno);
			break;
		}

		if (num_packets == 0) {
			/* Infinite echo back */
			if (!(count % 1000)) {
				printf("[TCPSERV%s infinite test #%d] echoback msg(%s) len(%d)\n", NETTEST_IPV(ipver), count, buf, ret);
			}
		} else {
			printf("[TCPSERV%s test #%d] echoback msg(%s) len(%d)\n", NETTEST_IPV(ipver), count, buf, ret);
			if (count >= num_packets) {
				printf("[TCPSERV%s test done #%d]\n", NETTEST_IPV(ipver), count);
				break;
			}
		}
		count++;
	}

	free(cliaddr);
	close(connfd);
	close(listenfd);
	freeaddrinfo(result);
	return;
err_out:
	if (cliaddr != NULL) {
		free(cliaddr);
		cliaddr = NULL;
	}
	if (listenfd >= 0) {
		close(listenfd);
		listenfd = -1;
	}
	if (result != NULL) {
		freeaddrinfo(result);
		result = NULL;
	}
	exit(1);
}

void tcp_client_thread(int num_packets, int ipver)
{
	char sbuf[BUF_SIZE];
	char rbuf[BUF_SIZE];
	int count = 1;
	int fd = -1;
	int ret;
	struct addrinfo *result;
	struct addrinfo *rp;
	struct addrinfo hints;
	struct timeval tv;

	/* write information for getaddrinfo() */
	memset(&hints, 0, sizeof(hints));
	hints.ai_socktype = SOCK_STREAM;
#if LWIP_IPV6
	if (ipver == 1) {
		hints.ai_family = AF_INET6;
	} else
#endif
	{
		hints.ai_family = AF_INET;
	}

	/* get address information */
	ret = getaddrinfo(g_app_target_addr, g_app_target_port, &hints, &result);
	if (ret != 0) {
		printf("[TCPCLI%s] getaddrinfo ret: %d\n", NETTEST_IPV(ipver), ret);
		goto err_out;
	}

	/* try to find a valid socket from the result */
	for (rp = result; rp != NULL; rp = rp->ai_next) {
		fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
		if (fd < 0) {
			/* continue if socket open is failed */
			continue;
		}

		if (connect(fd, rp->ai_addr, rp->ai_addrlen) == 0) {
			/* success */
			break;
		}
		close(fd);
	}
	if (rp == NULL) {
		printf("[TCPCLI%s] connection err\n", NETTEST_IPV(ipver), errno);
		goto err_out;
	}

	tv.tv_sec = 10;
	tv.tv_usec = 0;
	ret = setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(struct timeval));
	if (ret != 0) {
		printf("[TCPCLI%s] recvtimeout setsockopt err: %d", NETTEST_IPV(ipver), errno);
		goto err_out;
	}

	printf("[TCPCLI%s] addr: %s port:%s\n", NETTEST_IPV(ipver), g_app_target_addr, g_app_target_port);

	memset(sbuf, 0, BUF_SIZE);
	snprintf(sbuf, sizeof("hello, TCP_VX"), "hello, TCP%s", NETTEST_IPV(ipver));
	memset(rbuf, 0, BUF_SIZE);

	while (1) {
		ret = send(fd, sbuf, BUF_SIZE, 0);
		if (ret <= 0) {
			printf("[TCPCLI%s] send err: %d\n", NETTEST_IPV(ipver), errno);
			goto err_out;
		}

		ret = recv(fd, rbuf, BUF_SIZE, 0);
		if (ret <= 0) {
			printf("[TCPCLI%s] recv err: %d\n", NETTEST_IPV(ipver), errno);
			goto err_out;
		}

		if (num_packets == 0) {
			if (!(count % 1000)) {
				printf("[TCPCLI%s infinite test #%d] msg(%s) len(%d)\n", NETTEST_IPV(ipver), count, rbuf, ret);
			}
		} else {
			printf("[TCPCLI%s test #%d] msg(%s) len(%d)\n", NETTEST_IPV(ipver), count, rbuf, ret);
			if (count >= num_packets) {
				printf("[TCPCLI%s test done #%d]\n", NETTEST_IPV(ipver), count);
				break;
			}
		}
		count++;
	}

	close(fd);
	freeaddrinfo(result);
	return;
err_out:
	if (fd >= 0) {
		close(fd);
		fd = -1;
	}
	if (result != NULL) {
		freeaddrinfo(result);
		result = NULL;
	}
	exit(1);
}

void nettest_print_usage(int index)
{
	switch (index) {
	case 1:
		printf("\nUsage1: nettest proto target_port option1\n");
		printf("\tUsage1 is for echoback SERVER\n");
		printf("Parameter:\n");
		printf("\tproto - 0(tcp ipv4 echoback), 1(tcp ipv6 echoback)\n");
		printf("\t        2(udp ipv4 echoback), 3(udp ipv6 echoback)\n");
		printf("\ttarget_port - port address to bind\n");
		printf("\toption1 - number of pakcets to test, default: %d\n", NUM_PACKETS);
		printf("Sample Command(tcp ipv4 echoback server): nettest 0 8888 1000(option1)\n\n");
		break;
	case 2:
		printf("\nUsage2: nettest proto target_addr target_port option1\n");
		printf("\tUsage2 is for CLIENT that expects echoing back from the server\n");
		printf("Parameter:\n");
		printf("\tproto - 10(tcp ipv4 client), 11(tcp ipv6 client)\n");
		printf("\t        12(udp ipv4 client), 13(udp ipv6 client)\n");
		printf("\ttarget_addr - target IP address to send or connect\n");
		printf("\ttarget_port - target port address to send or connect\n");
		printf("\toption1 - number of packets to test, default: %d\n", NUM_PACKETS);
		printf("Sample Command(udp ipv6 client): nettest 13 fe80::dcc3:3084:cb9b:38ce%%wl1 8888 1000(option1)\n\n");
		break;
	case 3:
		printf("\nUsage3: nettest proto netif_addr target_port option1\n");
		printf("\tUsage3 is for sending and receiving ipmulticast message\n");
		printf("\tipmulticast address is fixed as definition in source file\n");
		printf("\tipv4 multicast address: %s\n", IPM_ADDR);
		printf("\tipv6 multicast address: %s\n", IPM6_ADDR);
		printf("Parameter:\n");
		printf("\tproto - 20(ipmulticast ipv4 receiver), 21(ipmulticast ipv6 receiver)\n");
		printf("\t        22(ipmulticast ipv4 sender), 23(ipmulticast ipv6 sender)\n");
		printf("\tnetif_addr - network interface address to send or receive ipmulticast message\n");
		printf("\ttarget_port - target port address for ipmulticast\n");
		printf("\toption1 - number of packets to test, default: %d\n", NUM_PACKETS);
		printf("Sample Command(ipmulticast ipv6 sender): nettest 23 fe80::dcc3:3084:cb9b:38ce 8888 1000(option1)\n\n");
		break;
	default:
		printf("invalid parameter\n");
		break;
	}
}

/* Sample App to test Transport Layer (TCP / UDP) / IP Multicast Functionality */
#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int nettest_main(int argc, char *argv[])
#endif
{
	int proto;
	int num_packets_to_process;

	if (argc == 1) {
		nettest_print_usage(1);
		nettest_print_usage(2);
		nettest_print_usage(3);
		return 0;
	}

	proto = atoi(argv[1]);
	num_packets_to_process = NUM_PACKETS;

	if (proto >= 0 && proto < 10) {
		if (argc < 3 || argc > 4) {
			nettest_print_usage(1);
			return 0;
		}

		g_app_target_port = argv[2];

		if (argc == 4) {
			num_packets_to_process = atoi(argv[3]);
		}
	}

	if (proto >= 10 && proto < 20) {
		if (argc < 4 || argc > 5) {
			nettest_print_usage(2);
			return 0;
		}

		g_app_target_addr = argv[2];
		g_app_target_port = argv[3];

		if (argc == 5) {
			num_packets_to_process = atoi(argv[4]);
		}
	}

	if (proto >= 20 && proto < 30) {
		if (argc < 4 || argc > 5) {
			nettest_print_usage(3);
			return 0;
		}

		g_app_netif_addr = argv[2];
		g_app_target_port = argv[3];

		if (argc == 5) {
			num_packets_to_process = atoi(argv[4]);
		}
	}

	switch (proto) {
	case 0:
		/* IPV4 TCP echo back server test */
		tcp_server_thread(num_packets_to_process, 0);
		break;
	case 1:
		/* IPV6 TCP echo back server test */
#if LWIP_IPV6
		tcp_server_thread(num_packets_to_process, 1);
#else
		printf("ipv6 is not supported\n");
#endif
		break;
	case 2:
		/* IPV4 UDP echo back server test */
		udp_server_thread(num_packets_to_process, 0);
		break;
	case 3:
		/* IPV6 UDP echo back server test */
#if LWIP_IPV6
		udp_server_thread(num_packets_to_process, 1);
#else
		printf("ipv6 is not supported\n");
#endif
		break;

	case 10:
		/* IPV4 TCP client test */
		tcp_client_thread(num_packets_to_process, 0);
		break;
	case 11:
		/* IPV6 TCP client test */
#if LWIP_IPV6
		tcp_client_thread(num_packets_to_process, 1);
#else
		printf("ipv6 is not supported\n");
#endif
		break;
	case 12:
		/* IPV4 UDP client test */
		udp_client_thread(num_packets_to_process, 0);
		break;
	case 13:
		/* IPV6 UDP client test */
#if LWIP_IPV6
		udp_client_thread(num_packets_to_process, 1);
#else
		printf("ipv6 is not supported\n");
#endif
		break;

	case 20:
		/* IPV4 ipmulticast receive */
		ipmcast_receiver_thread(num_packets_to_process, 0);
		break;
	case 21:
		/* IPV6 ipmulticast receive */
#if LWIP_IPV6
		ipmcast_receiver_thread(num_packets_to_process, 1);
#else
		printf("ipv6 is not supported\n");
#endif
		break;
	case 22:
		/* IPV4 ipmulticast send */
		ipmcast_sender_thread(num_packets_to_process, 0);
		break;
	case 23:
		/* IPV6 ipmulticast send */
#if LWIP_IPV6
		ipmcast_sender_thread(num_packets_to_process, 1);
#else
		printf("ipv6 is not supported\n");
#endif
		break;

	default:
		printf("[NETTEST APP] Invalid proto type\n");
		break;
	}
	printf("\nExiting nettest_main thread, job finished\n");
	return 0;
}
