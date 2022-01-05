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

/// @file itc_net_setsockopt.c
/// @brief ITC Test Case Example for setsockopt() API

#include <tinyara/config.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <semaphore.h>
#include <errno.h>
#include <sys/stat.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netutils/netlib.h>
#include "tc_internal.h"

#define SERVER_PORT 9098
#define SERVER_IP "127.0.0.1"
#define BUF_SIZE 1024

static sem_t g_signal;
static sem_t g_conn_signal;
static int g_listenfd = -1;

static void *accept_handler(void *arg)
{
	char msg[BUF_SIZE];
	int connfd = -1;
	int remain = 0;
	struct sockaddr_in cliaddr;
	socklen_t clilen = sizeof(cliaddr);
	int nbytes = 0;

	printf("send connection signal to client to send data\n");
	sem_post(&g_conn_signal);

	connfd = accept(g_listenfd, (struct sockaddr *)&cliaddr, &clilen);
	if (connfd < 0) {
		perror("accept fail\n");
		goto out_with_socket;
	}

	printf("Accepted\n");
	remain = sizeof(msg);
	while (remain > 0) {
		nbytes = recv(connfd, msg, remain, 0);
		if (nbytes == 0) {
			/* connection closed */
			printf("connection is broken %d\n", errno);
			goto out_with_socket;
		} else if (nbytes < 0) {
			printf("recv fail %s\n", strerror(errno));
			continue;
		}
		remain -= nbytes;
		printf("nbytes received %d\n", nbytes);
	}
out_with_socket:
	if (connfd > 0) {
		close(connfd);
		printf("Closed connfd successfully \n");
	}

	close(g_listenfd);
	printf("Closed g_listenfd successfully \n");

	sem_post(&g_signal);

	return NULL;
}

static void *run_server(void *arg)
{
	pid_t accept_pid;
	struct sockaddr_in servaddr;
	int ret = 0;

	g_listenfd = socket(PF_INET, SOCK_STREAM, 0);
	if (g_listenfd < 0) {
		printf("TCP socket failure %d\n", errno);
		return NULL;
	}

	int reuse = 1;
	ret = setsockopt(g_listenfd, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(reuse));
	if (ret < 0) {
		printf("ERR: setting SO_REUSEADDR\n");
		close(g_listenfd);
		return NULL;
	}

	/* Connect the socket to the server */
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = PF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = HTONS(SERVER_PORT);

	ret = bind(g_listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
	if (ret < 0) {
		perror("bind fail\n");
		close(g_listenfd);
		return NULL;
	}

	printf("Listening... port %d\n", SERVER_PORT);
	ret = listen(g_listenfd, 1024);
	if (ret < 0) {
		perror("listen fail\n");
		close(g_listenfd);
		return NULL;
	}

	TC_ASSERT_EQ_RETURN("create accept handler", 0, pthread_create(&accept_pid, NULL, accept_handler, NULL), NULL);
	pthread_detach(accept_pid);
	return NULL;
}

static void *run_client(void *arg)
{
	struct sockaddr_in myaddr;
	char buf[BUF_SIZE];
	socklen_t addrlen;

	sem_wait(&g_conn_signal);

	int sockfd = socket(PF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		printf("TCP socket failure %d\n", errno);
		return NULL;
	}

	/* Connect the socket to the server */
	myaddr.sin_family = AF_INET;
	myaddr.sin_port = htons(SERVER_PORT);
	inet_pton(AF_INET, SERVER_IP, &(myaddr.sin_addr));

	addrlen = sizeof(struct sockaddr_in);

	printf("Connecting...\n");
	if (connect(sockfd, (struct sockaddr *)&myaddr, addrlen) < 0) {
		printf("connect fail: %d\n", errno);
		goto out_with_socket;
	}
	printf("Connected\n");

	/* Then send num_packets number of messages */

	sprintf(buf, "Test Data: TCP send from TinyAra Node to Linux");
	int remain = sizeof(buf);
	while (remain > 0) {
		int res = send(sockfd, buf, remain, 0);
		if (res < 0) {
			printf("send fail %s\n", strerror(errno));
			continue;
		}
		if (res == 0) {
			printf("client connection is broken\n");
			goto out_with_socket;
		}
		remain -= res;
	}
	printf("Terminating tcpclient after sending sufficient messages (%lu)\n", sizeof(buf));
out_with_socket:
	close(sockfd);
	return NULL;
}

int net_socket_share_main(void)
{
	pthread_t server, client;
	TC_ASSERT_EQ_RETURN("create signal", 0, sem_init(&g_signal, 0, 0), -1);
	TC_ASSERT_EQ_RETURN("create conn signal", 0, sem_init(&g_conn_signal, 0, 0), -1);
	TC_ASSERT_EQ_RETURN("create server", 0, pthread_create(&server, NULL, run_server, NULL), -1);
	TC_ASSERT_EQ_RETURN("create client", 0, pthread_create(&client, NULL, run_client, NULL), -1);
	sem_wait(&g_signal);
	pthread_join(server, NULL);
	pthread_join(client, NULL);

	TC_ASSERT_EQ_RETURN("destroy signal", 0, sem_destroy(&g_signal), -1);
	TC_ASSERT_EQ_RETURN("destroy signal", 0, sem_destroy(&g_conn_signal), -1);
	return 0;
}
