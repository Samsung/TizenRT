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

// @file tc_net_shutdown.c
// @brief Test Case Example for shutdown() API
#include <tinyara/config.h>
#include <errno.h>
#include <sys/stat.h>
#include <net/if.h>
#include <netutils/netlib.h>
#include "tc_internal.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <pthread.h>

#define PORTNUM 1112
#define MAXRCVLEN 20
int mut = 0;
/**
* @fn					: shutdown_wait
* @brief				: function to wait on semaphore
* @scenario				:
* API's covered			:
* Preconditions			:
* Postconditions		:
* @return				: void
*/
void shutdown_wait(void)
{
	while (mut <= 0) {

		printf("");
	}
	mut--;
}

/**
* @fn					: shutdown_signal
* @brief				: function to signal semaphore
* @scenario				:
* API's covered			:
* Preconditions			:
* Postconditions		:
* @return				: void
*/
void shutdown_signal(void)
{
	mut++;
}

/**
* @testcase				: tc_net_shutdown_recv_p
* @brief				:
* @scenario				:
* @apicovered			: shutdown()
* @precondition			:
* @postcondition		:
*/
void tc_net_shutdown_recv_p(int fd)
{
	int ret = shutdown(fd, SHUT_RD);

	TC_ASSERT_NEQ("shutdown", ret, NEG_VAL);
	TC_SUCCESS_RESULT();

}

/**
* @testcase				: tc_net_shutdown_send_p
* @brief				:
* @scenario				:
* @apicovered			: shutdown()
* @precondition			:
* @postcondition		:
*/
void tc_net_shutdown_send_p(int fd)
{
	int ret = shutdown(fd, SHUT_WR);

	TC_ASSERT_NEQ("shutdown", ret, NEG_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase				: tc_net_shutdown_sendrecv_p
* @brief				:
* @scenario				:
* @apicovered			: shutdown()
* @precondition			:
* @postcondition		:
*/
void tc_net_shutdown_sendrecv_p(int fd)
{
	int ret = shutdown(fd, SHUT_RDWR);

	TC_ASSERT_NEQ("shutdown", ret, NEG_VAL);
	TC_SUCCESS_RESULT();

}

/**
* @testcase				: tc_net_shutdown_n
* @brief				:
* @scenario				:
* @apicovered			: shutdown()
* @precondition			:
* @postcondition		:
*/
void tc_net_shutdown_n(void)
{
	int ret = shutdown(NEG_VAL, SHUT_RDWR);

	TC_ASSERT_EQ("shutdown", ret, NEG_VAL);
	TC_SUCCESS_RESULT();

}

/**
* @testcase				: tc_net_shutdown_sock_n
* @brief				:
* @scenario				:
* @apicovered			: shutdown()
* @precondition			:
* @postcondition		:
*/
void tc_net_shutdown_sock_n(int fd)
{
	int ret = shutdown(fd, SHUT_RD);

	TC_ASSERT_EQ("shutdown", ret, NEG_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @fn					: shutdown_server
* @brief				:
* @scenario				:
* API's covered			: socket,bind,listen,send,accept,close
* Preconditions			:
* Postconditions		:
* @return				: void *
*/
void *shutdown_server(void *args)
{
	int ConnectFD;
	char *msg = "Hello World !\n";
	struct sockaddr_in sa;

	memset(&sa, 0, sizeof(sa));
	int sock = socket(AF_INET, SOCK_STREAM, 0);

	sa.sin_family = PF_INET;
	sa.sin_port = htons(PORTNUM);
	sa.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

	bind(sock, (struct sockaddr *)&sa, sizeof(sa));
	listen(sock, 1);
	shutdown_signal();

	ConnectFD = accept(sock, NULL, NULL);
	tc_net_shutdown_send_p(ConnectFD);
	send(ConnectFD, msg, strlen(msg), ZERO);
	tc_net_shutdown_recv_p(ConnectFD);
	recv(ConnectFD, msg, 1024, 0);
	tc_net_shutdown_n();
	tc_net_shutdown_sock_n(sock);
	close(ConnectFD);
	close(sock);
	return NULL;
}

/**
* @fn					: shut_client
* @brief				:
* @scenario				:
* API's covered			: socket,connect,recvfrom,close
* Preconditions			:
* Postconditions		:
* @return				: void *
*/
void *shutdown_client(void *args)
{
	char buffer[MAXRCVLEN];
	int len;
	struct sockaddr_in dest;

	int sock = socket(AF_INET, SOCK_STREAM, 0);

	memset(&dest, 0, sizeof(dest));
	dest.sin_family = PF_INET;
	dest.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	dest.sin_port = htons(PORTNUM);

	shutdown_wait();

	connect(sock, (struct sockaddr *)&dest, sizeof(struct sockaddr));

	len = recv(sock, buffer, MAXRCVLEN, ZERO);
	buffer[len] = '\0';
	tc_net_shutdown_recv_p(sock);

	recv(sock, buffer, MAXRCVLEN, ZERO);
	tc_net_shutdown_sendrecv_p(sock);
	recv(sock, buffer, MAXRCVLEN, ZERO);
	tc_net_shutdown_sendrecv_p(sock);
	send(sock, buffer, strlen(buffer), ZERO);
	tc_net_shutdown_sendrecv_p(sock);
	tc_net_shutdown_n();
	close(sock);
	return NULL;
}

void tc_net_shutdown(void)
{
	pthread_t Server, Client;

	pthread_create(&Server, NULL, shutdown_server, NULL);
	pthread_create(&Client, NULL, shutdown_client, NULL);

	pthread_join(Server, NULL);
	pthread_join(Client, NULL);
}

/****************************************************************************
 * Name: shutdown()
 ****************************************************************************/
int net_shutdown_main(void)
{

	tc_net_shutdown();

	return 0;
}
