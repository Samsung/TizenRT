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

// @file tc_net_recvfrom.c
// @brief Test Case Example for recvfrom() API
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

#define PORTNUM 1111
#define MAXRCVLEN 20
int mutex1 = 0;

/**
* @testcase				: tc_net_recvfrom_p
* @brief				: positive testcase using udp
* @scenario				:
* @apicovered			: recvfrom()
* @precondition			:
* @postcondition		:
*/
void tc_net_recvfrom_p(int fd)
{
	char buffer[MAXRCVLEN];
	struct sockaddr_storage serverStorage;
	socklen_t addr_size;
	int ret = recvfrom(fd, buffer, MAXRCVLEN, 0, (struct sockaddr *)&serverStorage, &addr_size);

	TC_ASSERT_NEQ("recvfrom", ret, NEG_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase				: tc_net_recvfrom_sock_n
* @brief				: negative testcase using udp
* @scenario				:
* @apicovered			: recvfrom()
* @precondition			:
* @postcondition		:
*/
void tc_net_recvfrom_sock_n(int sock)
{
	char buffer[MAXRCVLEN];
	struct sockaddr_storage serverStorage;
	socklen_t addr_size;
	int ret = recvfrom(NEG_VAL, buffer, MAXRCVLEN, 0, (struct sockaddr *)&serverStorage, &addr_size);

	TC_ASSERT_EQ("recvfrom", ret, NEG_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase				: tc_net_recvfrom_n
* @brief				: negative testcase using udp
* @scenario				:
* @apicovered			: recvfrom()
* @precondition			:
* @postcondition		:
*/
void tc_net_recvfrom_n(int fd)
{
	char buffer[MAXRCVLEN];
	struct sockaddr_storage serverStorage;
	socklen_t addr_size;
	int ret = recvfrom(NEG_VAL, buffer, MAXRCVLEN, 0, (struct sockaddr *)&serverStorage, &addr_size);

	TC_ASSERT_EQ("recvfrom", ret, NEG_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @fn					: recvfrom_server
* @brief				: udp server
* @scenario				:
* API's covered			: socket,bind
* Preconditions			:
* Postconditions		:
* @return				: void *
*/

void *recvfrom_udpserver(void *args)
{
	struct sockaddr_in sa;
	int sock = socket(AF_INET, SOCK_DGRAM, 0);

	memset(&sa, 0, sizeof(sa));

	sa.sin_family = PF_INET;
	sa.sin_port = htons(PORTNUM);
	sa.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

	bind(sock, (struct sockaddr *)&sa, sizeof(sa));

	tc_net_recvfrom_p(sock);
	tc_net_recvfrom_sock_n(sock);
	tc_net_recvfrom_n(sock);
	close(sock);
	return NULL;
}

/**
* @fn					: recvfrom_client
* @brief				: udp client
* @scenario				:
* API's covered			: socket,sendto
* Preconditions			:
* Postconditions		:
* @return				: void *
*/
void *recvfrom_udpclient(void *args)
{
	int ret;
	int sock = socket(AF_INET, SOCK_DGRAM, 0);

	char *buffer = "hello";

	int len = strlen(buffer) + 1;
	struct sockaddr_in dest;
	socklen_t fromlen;

	memset(&dest, 0, sizeof(dest));
	dest.sin_family = PF_INET;
	dest.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	dest.sin_port = htons(PORTNUM);
	fromlen = sizeof(dest);

	ret = sendto(sock, buffer, len, 0, (struct sockaddr *)&dest, fromlen);
	close(sock);
	return NULL;
}

/**
* @fn					: recvfrom_wait
* @brief				: function to wait on semaphore
* @scenario				:
* API's covered			:
* Preconditions			:
* Postconditions		:
* @return				: void
*/
void recvfrom_wait(void)
{
	while (mutex1 <= 0) {

		printf("");
	}
	mutex1--;
}

/**
* @fn					: recvfrom_signal
* @brief				: function to signal semaphore
* @scenario				:
* API's covered			:
* Preconditions			:
* Postconditions		:
* @return				: void
*/
void recvfrom_signal(void)
{
	mutex1++;
}

/**
* @testcase				: tc_net_recvfrom_tcp_p
* @brief				: positive testcase for recvfrom api
* @scenario				:
* @apicovered			: recvfrom()
* @precondition			:
* @postconditions		:
*/
void tc_net_recvfrom_tcp_p(int fd)
{
	char buffer[MAXRCVLEN];

	int ret = recvfrom(fd, buffer, MAXRCVLEN, 0, NULL, NULL);

	TC_ASSERT_NEQ("recvfrom", ret, NEG_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase				: tc_net_recvfrom_tcp_conn_n
* @brief				: negative testcase for recvfrom api
* @scenario				:
* @apicovered			: recvfrom()
* @precondition			:
* @postcondition		:
*/
void tc_net_recvfrom_tcp_conn_n(int fd)
{
	char buffer[MAXRCVLEN];

	shutdown(fd, SHUT_RD);
	int ret = recvfrom(fd, buffer, MAXRCVLEN, 0, NULL, NULL);

	TC_ASSERT_EQ("recvfrom", ret, NEG_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase				: tc_net_recvfrom_tcp_sock_n
* @brief				: negative testcase for recvfrom api
* @scenario				:
* @apicovered			: recvfrom()
* @precondition			:
* @postcondition		:
*/
void tc_net_recvfrom_tcp_sock_n(int fd)
{
	char buffer[MAXRCVLEN];

	int ret = recvfrom(fd, buffer, MAXRCVLEN, 0, NULL, NULL);

	TC_ASSERT_EQ("recvfrom", ret, NEG_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase				: tc_net_recvfrom_tcp_n
* @brief				: negative testcase using tcp
* @scenario				:
* @apicovered			: recvfrom()
* @precondition			:
* @postcondition		:
*/
void tc_net_recvfrom_tcp_n(void)
{
	char buffer[MAXRCVLEN];

	int ret = recvfrom(NEG_VAL, buffer, MAXRCVLEN, 0, NULL, NULL);

	TC_ASSERT_EQ("recvfrom", ret, NEG_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @fn					: recvfrom_server
* @brief				:
* @scenario				:
* API's covered			: socket,bind,listen,accept,sendto,close
* Preconditions			:
* Postconditions		:
* @return				: void
*/
void *recvfrom_tcpserver(void *args)
{
	int i, ret;
	int ConnectFD;
	char *msg = "Hello World !\n";
	struct sockaddr_in sa;
	int sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	memset(&sa, 0, sizeof(sa));

	sa.sin_family = PF_INET;
	sa.sin_port = htons(PORTNUM);
	sa.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

	ret = bind(sock, (struct sockaddr *)&sa, sizeof(sa));

	ret = listen(sock, 1);

	recvfrom_signal();
	ConnectFD = accept(sock, NULL, NULL);

	for (i = 0; i < 4; i++) {
		ret = sendto(ConnectFD, msg, strlen(msg), 0, (struct sockaddr *)&sa, sizeof(sa));

	}
	close(ConnectFD);
	close(sock);
	return NULL;
}

/**
* @fn					: recvfrom_client
* @brief				:
* @scenario				:
* API's covered			: socket,connect,close
* Preconditions			:
* Postconditions		:
* @return				: void *
*/
void *recvfrom_tcpclient(void *args)
{
	int newsoc = 0;
	int ret;
	struct sockaddr_in dest;

	int sock = socket(AF_INET, SOCK_STREAM, 0);

	memset(&dest, 0, sizeof(dest));
	dest.sin_family = PF_INET;
	dest.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	dest.sin_port = htons(PORTNUM);

	recvfrom_wait();
	ret = connect(sock, (struct sockaddr *)&dest, sizeof(struct sockaddr));

	tc_net_recvfrom_tcp_p(sock);

	tc_net_recvfrom_tcp_n();
	tc_net_recvfrom_tcp_sock_n(newsoc);
	tc_net_recvfrom_tcp_conn_n(sock);

	close(sock);
	return NULL;
}

void tc_net_recvfrom(int sock)
{
	pthread_t Server, Client, tcpserver, tcpclient;
	int ret;
	ret = pthread_create(&Server, NULL, recvfrom_udpserver, &sock);
	TC_ASSERT_EQ("pthread_create", ret, ZERO);

	ret = pthread_create(&Client, NULL, recvfrom_udpclient, NULL);
	TC_ASSERT_EQ("pthread_create", ret, ZERO);
	ret = pthread_join(Server, NULL);
	TC_ASSERT_EQ("pthread_join", ret, ZERO);
	ret = pthread_join(Client, NULL);
	TC_ASSERT_EQ("pthread_join", ret, ZERO);
	ret = pthread_create(&tcpserver, NULL, recvfrom_tcpserver, NULL);
	TC_ASSERT_EQ("pthread_create", ret, ZERO);
	ret = pthread_create(&tcpclient, NULL, recvfrom_tcpclient, NULL);
	TC_ASSERT_EQ("pthread_create", ret, ZERO);
	ret = pthread_join(tcpserver, NULL);
	TC_ASSERT_EQ("pthread_join", ret, ZERO);
	ret = pthread_join(tcpclient, NULL);
	TC_ASSERT_EQ("pthread_join", ret, ZERO);
}

/****************************************************************************
 * Name: recvfrom()
 ****************************************************************************/
int net_recvfrom_main(void)
{
	tc_client_server();
	return 0;
}
