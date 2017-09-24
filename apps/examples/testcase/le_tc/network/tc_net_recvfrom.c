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

#define PORTNUM        5001
#define MAXRCVLEN      20
#define CNT            4
#define BACKLOG        1

static int count_wait;

/**
* @testcase             : tc_net_recvfrom_p
* @brief                : receive a message from a socket.
* @scenario             : recvfrom() function receive a message from a connectionless-mode socket.
* @apicovered           : recvfrom()
* @precondition         : socket file descriptor.
* @postcondition        : none
* @return               : void
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
* @testcase             : tc_net_recvfrom_sock_n
* @brief                : receive a message from a socket.
* @scenario             : recvfrom() function receive a message from a connectionless-mode socket.
* @apicovered           : recvfrom()
* @precondition         : socket file descriptor.
* @postcondition        : none
* @return               : void
*/
void tc_net_recvfrom_sock_n(void)
{
	char buffer[MAXRCVLEN];
	struct sockaddr_storage serverStorage;
	socklen_t addr_size;

	int ret = recvfrom(NEG_VAL, buffer, MAXRCVLEN, 0, (struct sockaddr *)&serverStorage, &addr_size);
	TC_ASSERT_EQ("recvfrom", ret, NEG_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase             : tc_net_recvfrom_n
* @brief                : receive a message from a socket.
* @scenario             : recvfrom() function receive a message from a connectionless-mode socket,
                          test with invalid socket fd.
* @apicovered           : recvfrom()
* @precondition         : socket file descriptor.
* @postcondition        : none
* @return               : void
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
* @fn                 : recvfrom_udpserver
* @brief              : create a Udp server.
* @scenario           : create a udp server to test recvfrom api.
* @API's covered      : socket,bind
* @Preconditions      : socket file descriptor.
* @Postconditions     : none
* @return             : void
*/
void *recvfrom_udpserver(void *args)
{
	struct sockaddr_in sa;

	int sock = socket(AF_INET, SOCK_DGRAM, 0);

	memset(&sa, 0, sizeof(sa));
	sa.sin_family = PF_INET;
	sa.sin_port = htons(PORTNUM);
	sa.sin_addr.s_addr = INADDR_LOOPBACK;

	bind(sock, (struct sockaddr *)&sa, sizeof(sa));

	tc_net_recvfrom_p(sock);
	tc_net_recvfrom_sock_n();
	tc_net_recvfrom_n(sock);
	close(sock);
	return NULL;
}

/**
* @fn                   : recvfrom_udpclient
* @brief                : create client.
* @scenario             : create udp client.
* @API's covered        : socket,sendto
* @Preconditions        : socket file descriptor.
* @Postconditions       : none
* @return               : void
*/
void *recvfrom_udpclient(void *args)
{
	int ret;
	int sock = socket(AF_INET, SOCK_DGRAM, 0);

	char *buffer = "hello";

	int len = strlen(buffer) + ONE;
	struct sockaddr_in dest;
	socklen_t fromlen;

	memset(&dest, 0, sizeof(dest));
	dest.sin_family = PF_INET;
	dest.sin_addr.s_addr = INADDR_LOOPBACK;
	dest.sin_port = htons(PORTNUM);
	fromlen = sizeof(dest);

	ret = sendto(sock, buffer, len, 0, (struct sockaddr *)&dest, fromlen);
	close(sock);
	return NULL;
}

/**
* @fn                   : recvfrom_wait
* @brief                : function to wait on semaphore
* @scenario             : use wait function to decrement count value.
* @API's covered        : none
* @Preconditions        : none
* @Postconditions       : none
* @return               : void
*/
void recvfrom_wait(void)
{
	while (count_wait <= ZERO) {
		printf("");
	}
	count_wait--;
}

/**
* @fn                   : recvfrom_signal
* @brief                : function to signal semaphore
* @scenario             : use to increase the count value.
* @API's covered        : none
* @Preconditions        : none
* @Postconditions       : none
* @return               : void
*/
void recvfrom_signal(void)
{
	count_wait++;
}

/**
* @testcase             : tc_net_recvfrom_tcp_p
* @brief                : receive a message from a socket.
* @scenario             : recvfrom() function receive a message from a connection-mode socket.
* @apicovered           : recvfrom()
* @precondition         : socket file descriptor.
* @postcondition        : none
* @return               : void
*/
void tc_net_recvfrom_tcp_p(int fd)
{
	char buffer[MAXRCVLEN];

	int ret = recvfrom(fd, buffer, MAXRCVLEN, 0, NULL, NULL);
	TC_ASSERT_NEQ("recvfrom", ret, NEG_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase             : tc_net_recvfrom_tcp_conn_n
* @brief                : receive a message from a socket.
* @scenario             : recvfrom() function receive a message from a connection-mode socket.
                          test after shutdown.
* @apicovered           : recvfrom()
* @precondition         : none
* @postcondition        : none
* @return               : void
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
* @testcase             : tc_net_recvfrom_tcp_sock_n
* @brief                : negative testcase for recvfrom api
* @scenario             : used for tcp connection
* @apicovered           : recvfrom()
* @precondition         : none
* @postcondition        : none
* @return               : void
*/
void tc_net_recvfrom_tcp_sock_n(int fd)
{
	char buffer[MAXRCVLEN];

	int ret = recvfrom(fd, buffer, MAXRCVLEN, 0, NULL, NULL);

	TC_ASSERT_EQ("recvfrom", ret, NEG_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_recvfrom_tcp_n
* @brief               : receive a message from a socket.
* @scenario            : recvfrom() function receive a message from a connection-mode socket,
                         test with invalid socket fd.
* @apicovered          : recvfrom()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
void tc_net_recvfrom_tcp_n(void)
{
	char buffer[MAXRCVLEN];

	int ret = recvfrom(NEG_VAL, buffer, MAXRCVLEN, 0, NULL, NULL);
	TC_ASSERT_EQ("recvfrom", ret, NEG_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @fn                   : recvfrom_server
* @brief                : create a tcp server.
* @scenario             : create a tcp server to test recvfrom api.
* @API's covered        : socket,bind,listen,accept,sendto,close
* @Preconditions        : socket file descriptor.
* @Postconditions       : none
* @return               : void
*/
void* recvfrom_tcpserver(void *args)
{
	int i;
	int ConnectFD;
	char *msg = "Hello World !\n";
	struct sockaddr_in sa;

	int sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	memset(&sa, 0, sizeof(sa));
	sa.sin_family = PF_INET;
	sa.sin_port = htons(PORTNUM);
	sa.sin_addr.s_addr = INADDR_LOOPBACK;

	bind(sock, (struct sockaddr *)&sa, sizeof(sa));
	listen(sock, BACKLOG);

	recvfrom_signal();
	ConnectFD = accept(sock, NULL, NULL);

	for (i = 0; i < CNT; i++) {
		sendto(ConnectFD, msg, strlen(msg), 0, (struct sockaddr *)&sa, sizeof(sa));
	}
	close(ConnectFD);
	close(sock);
	return NULL;
}

/**
* @fn                   : recvfrom_tcpclient
* @brief                : create client.
* @scenario             : create tcp client.
* @API's covered        : socket,connect,close
* @Preconditions        : socket file descriptor.
* @Postconditions       : none
* @return               : void
*/
void* recvfrom_tcpclient(void *args)
{
	struct sockaddr_in dest;

	int sock = socket(AF_INET, SOCK_STREAM, 0);

	memset(&dest, 0, sizeof(dest));
	dest.sin_family = PF_INET;
	dest.sin_addr.s_addr = INADDR_LOOPBACK;
	dest.sin_port = htons(PORTNUM);

	recvfrom_wait();
	connect(sock, (struct sockaddr *)&dest, sizeof(struct sockaddr));

	tc_net_recvfrom_tcp_p(sock);
	tc_net_recvfrom_tcp_n();
	tc_net_recvfrom_tcp_sock_n(sock);
	tc_net_recvfrom_tcp_conn_n(sock);
	close(sock);
	return NULL;
}

/**
* @fn                  : net_recvfrom
* @brief               : create client and server thread.
* @scenario            : create client and server thread to test recvfrom api.
* @API's covered       : none
* @Preconditions       : none
* @Postconditions      : none
* @return              : void
*/
void net_recvfrom(void)
{
	pthread_t Server, Client, tcpserver, tcpclient;

	pthread_create(&Server, NULL, recvfrom_udpserver, NULL);
	pthread_create(&Client, NULL, recvfrom_udpclient, NULL);

	pthread_join(Server, NULL);
	pthread_join(Client, NULL);

	pthread_create(&tcpserver, NULL, recvfrom_tcpserver, NULL);
	pthread_create(&tcpclient, NULL, recvfrom_tcpclient, NULL);

	pthread_join(tcpserver, NULL);
	pthread_join(tcpclient, NULL);
}

/****************************************************************************
 * Name: recvfrom()
 ****************************************************************************/
int net_recvfrom_main(void)
{
	net_recvfrom();
	return 0;
}
