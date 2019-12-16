/****************************************************************************
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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <time.h>
#include <errno.h>
#include <semaphore.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <signal.h>

#define AF_INETX AF_INET
#define PF_INETX PF_INET

#define PKT_NUM 10
#define BUF_SIZE 301
static int g_target_port = 5555;

static void server_app(void)
{
	struct sockaddr_in servaddr;
	struct sockaddr_in cliaddr;
	int count = 0;
	int listenfd = -1;
	int connfd = -1;
	socklen_t clilen;
	int ret = 0;
	int recv_len = 0;
	int nbytes = 0;
	char msg[BUF_SIZE];

	uint32_t sbuf_size = 0;
	char sbuf[BUF_SIZE];
	memset(sbuf, 0xff, BUF_SIZE);
	sbuf_size = BUF_SIZE;
	int sret;

	listenfd = socket(PF_INET, SOCK_STREAM, 0);
	if (listenfd < 0) {
		printf("[TCPSERV] TCP socket failure %d\n", errno);
		return;
	}

	/*
	 *	 * Enable SO_REUSEADDR to allow multiple instances of this
	 *		 * application to receive copies of the multicast datagrams.
	 *			 */

	int reuse = 1;
	ret = setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(reuse));
	if (ret < 0) {
		printf("[TCPSERV] ERR: setting SO_REUSEADDR\n");
		goto out_with_socket;
	}
	printf("[TCPSERV] set reusable success\n");

	/* Connect the socket to the server */
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = PF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(g_target_port);

	ret = bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
	if (ret < 0) {
		perror("[TCPSERV] bind fail\n");
		goto out_with_socket;
	}

	printf("[TCPSERV] Listening... port %d\n", g_target_port);

	ret = listen(listenfd, 1024);
	if (ret < 0) {
		perror("[TCPSERV] listen fail\n");
		goto out_with_socket;
	}

	clilen = sizeof(cliaddr);

	connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen);
	if (connfd < 0) {
		perror("[TCPSERV] accept fail\n");
		goto out_with_socket;
	}
	printf("[TCPSERV] Accepted\n");

	recv_len = sizeof(msg);
	int trial = 1;
	while (1) {
		printf("\n[TCPSERV] TRIAL=%d wait for receive\n", trial++);
		int nbytes_sum = 0;
		while (nbytes_sum < BUF_SIZE) {
			nbytes = recv(connfd, msg + nbytes_sum, recv_len - nbytes_sum, 0);
			if (nbytes <= 0) {
				/* connection closed */
				printf("[TCPSERV] selectserver: socket hung up err nbytes? %d\n", nbytes);
				close(connfd);
				goto out_with_socket;
			}
			nbytes_sum += nbytes;
		}
		count++;
		printf("[TCPSERV] - Received Msg # %d] Received Msg (%d)\n", count, nbytes_sum);
		if (count == PKT_NUM) {
			count = 0;
			for (int i = 0; i < PKT_NUM; i++) {
				int send_sum = 0;
				while (send_sum < BUF_SIZE) {
					sret = send(connfd, sbuf + send_sum, sbuf_size - send_sum, 0);
					if (sret < 0 || sret != sbuf_size) {
						printf("[TCPSERV] send #%d error ret(%d) err(%d)\n", i+1, sret, errno);
						close(connfd);
						goto out_with_socket;
					}
					send_sum += sret;
				}
				printf("[TCPSERV] send #%d  %d bytes\n", i+1, send_sum);
			}
		}
	}
	if (connfd > 0) {
		close(connfd);
		printf("[TCPSERV] Closed connfd successfully \n");
	}
	printf("[TCPSERV] Closed listenfd successfully \n");

out_with_socket:
	close(listenfd);
	return;
}

void main(void)
{
	signal(SIGPIPE, SIG_IGN);
	while (1) {
		server_app();
	}
	return;
}
