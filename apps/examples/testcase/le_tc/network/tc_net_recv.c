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

// @file tc_net_recv.c
// @brief Test Case Example for recv() API
#include <tinyara/config.h>
#include <errno.h>
#include <sys/stat.h>
#include <net/if.h>
#include <netutils/netlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <pthread.h>

#include "tc_internal.h"

#define PORTNUM        5010
#define MAXRCVLEN      20
#define BACKLOG        2

static int count_wait;

/**
* @fn                   : recv_wait
* @brief                : Function to wait on semaphore
* @scenario             : use wait function to decrement count value.
* @API's covered        : none
* @Preconditions        : none
* @Postconditions       : none
* @return               : void
*/
void recv_wait(void)
{
	while (count_wait <= ZERO) {
		printf("");
	}
	count_wait--;
}

/**
* @fn                  : recv_signal
* @brief               : Function to signal semaphore
* @scenario            : use to increase the count value.
* @API's covered       : none
* @Preconditions       : none
* @Postconditions      : none
* @return              : void
*/
void recv_signal(void)
{
	count_wait++;
}

/**
* @testcase            : tc_net_recv_p
* @brief               : This recv API receive a message from a socket.
* @scenario            : recv is used to receive messages from a socket.
* @apicovered          : recv()
* @precondition        : socket file descriptor.
* @postcondition       : none
* @return              : void
*/
void tc_net_recv_p(int fd)
{
	char buffer[MAXRCVLEN];
	int ret = recv(fd, buffer, MAXRCVLEN, 0);
	buffer[ret] = '\0';

	TC_ASSERT_NEQ("recv", ret, NEG_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_recv_n
* @brief               : This recv API receive a message from a socket.
* @scenario            : recv is used to receive messages from a socket,
                         with invalid fd.
* @apicovered          : recv()
* @precondition        : socket file descriptor.
* @postcondition       : none
* @return              : void
*/
void tc_net_recv_n(int fd)
{
	char buffer[MAXRCVLEN];

	int ret = recv(-1, buffer, MAXRCVLEN, 0);
	buffer[ret] = '\0';

	TC_ASSERT_EQ("recv", ret, NEG_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_recv_shutdown_n
* @brief               : This recv API receive a message from a socket.
* @scenario            : recv is used to receive messages from a socket,
                         test after shutdown.
* @apicovered          : recv()
* @precondition        : socket file descriptor.
* @postcondition       : none
* @return              : void
*/
void tc_net_recv_shutdown_n(int fd)
{
	char buffer[MAXRCVLEN];
	shutdown(fd, SHUT_RD);
	int ret = recv(fd, buffer, MAXRCVLEN, 0);
	buffer[ret] = '\0';

	TC_ASSERT_EQ("net_recv_shutdown", ret, NEG_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_recv_close_n
* @brief               : This recv API receive a message from a socket.
* @scenario            : recv is used to receive messages from a socket,
                         test after close.
* @apicovered          : recv()
* @precondition        : socket file descriptor.
* @postcondition       : none
* @return              : void
*/
void tc_net_recv_close_n(int fd)
{
	char buffer[MAXRCVLEN];
	close(fd);
	int ret = recv(fd, buffer, MAXRCVLEN, 0);

	TC_ASSERT_EQ("recv_close", ret, NEG_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @fn                   : recv_server
* @brief                : Create a Tcp server.
* @scenario             : Create a tcp server for checking receive api.
* @API's covered        : socket,bind,listen,accept,send,close
* @Preconditions        : socket file descriptor.
* @Postconditions       : none
* @return               : void
*/
void* recv_server(void *args)
{
	int ConnectFD;
	struct sockaddr_in sa;
	int sock = socket(AF_INET, SOCK_STREAM, 0);

	memset(&sa, 0, sizeof(sa));
	sa.sin_family = PF_INET;
	sa.sin_port = htons(PORTNUM);
	sa.sin_addr.s_addr = INADDR_LOOPBACK;

	bind(sock, (struct sockaddr *)&sa, sizeof(sa));
	listen(sock, BACKLOG);
	recv_signal();
	ConnectFD = accept(sock, NULL, NULL);

	close(ConnectFD);
	return NULL;
}

/**
* @fn                  : recv_client
* @brief               : This api create client.
* @scenario            : Create tcp client.
* @API's covered       : socket,connect,close
* @Preconditions       : socket file descriptor.
* @Postconditions      : none
* @return              : void
*/
void* recv_client(void *args)
{
	struct sockaddr_in dest;
	int ret;

	int sock = socket(AF_INET, SOCK_STREAM, 0);
	memset(&dest, 0, sizeof(dest));
	dest.sin_family = PF_INET;
	dest.sin_addr.s_addr = INADDR_LOOPBACK;
	dest.sin_port = htons(PORTNUM);

	recv_wait();
	ret = connect(sock, (struct sockaddr *)&dest, sizeof(struct sockaddr));
	tc_net_recv_p(sock);
	tc_net_recv_n(sock);
	tc_net_recv_shutdown_n(sock);
	tc_net_recv_close_n(NEG_VAL);
	close(sock);
	return NULL;
}

/**
* @fn                  : net_recv
* @brief               : This api create client and server thread.
* @scenario            : Create client and server thread to test accept api.
* API's covered        : none
* Preconditions        : none
* Postconditions       : none
* @return              : void
*/
void net_recv(void)
{
	pthread_t Server, Client;

	pthread_create(&Server, NULL, recv_server, NULL);
	pthread_create(&Client, NULL, recv_client, NULL);
	pthread_join(Server, NULL);
	pthread_join(Client, NULL);
}

/****************************************************************************
 * Name: recv()
 ****************************************************************************/
int net_recv_main(void)
{
	net_recv();
	return 0;
}
