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

// @file tc_net_sendto.c
// @brief Test Case Example for sendto() API
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

#define PORTNUM        7891
#define TCPPORT        7890
#define MAXRCVLEN      20

void tc_net_sendto_tcp_n(int ConnectFD);
void tc_net_sendto_tcp_shutdown_n(int ConnectFD);

/**
* @testcase           : tc_net_sendto_p
* @brief              : positive testcase for sendto api using udp
* @scenario           : used for udp connection
* @apicovered         : sendto()
* @precondition       : valid file descriptor
* @postcondition      : none
*/
void tc_net_sendto_p(int fd)
{
	int ret;
	char *buffer = "hello";
	int len = strlen(buffer) + ONE;

	struct sockaddr_in dest;
	socklen_t fromlen;
	memset(&dest, 0, sizeof(dest));
	dest.sin_family = PF_INET;
	dest.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	dest.sin_port = htons(PORTNUM);
	fromlen = sizeof(dest);

	ret = sendto(fd, buffer, len, 0, (struct sockaddr *)&dest, fromlen);
	TC_ASSERT_NEQ("sendto", ret, NEG_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase           : tc_net_sendto_n
* @brief              : negative testcase for sendto api using udp
* @scenario           : used for udp connection
* @apicovered         : sendto()
* @precondition       : valid file descriptor
* @postcondition      : none
*/
void tc_net_sendto_n(void)
{
	int ret;
	char *buffer = "hello";
	int len = strlen(buffer) + ONE;
	struct sockaddr_in dest;
	socklen_t fromlen;

	memset(&dest, 0, sizeof(dest));
	dest.sin_family = PF_INET;
	dest.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	dest.sin_port = htons(PORTNUM);
	fromlen = sizeof(dest);

	ret = sendto(NEG_VAL, buffer, len, 0, (struct sockaddr *)&dest, fromlen);
	TC_ASSERT_EQ("sendto", ret, NEG_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase           : tc_net_sendto_af_unix_n
* @brief              : negative testcase for sendto api using AF_UNIX
* @scenario           : used for udp connection
* @apicovered         : sendto()
* @precondition       : valid file descriptor
* @postcondition      : none
*/
void tc_net_sendto_af_unix_n(int fd)
{
	char *buffer = "hello";
	int len = strlen(buffer) + ONE;
	struct sockaddr_in dest;
	socklen_t fromlen;
	memset(&dest, 0, sizeof(dest));
	dest.sin_family = AF_UNIX;
	dest.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	dest.sin_port = htons(PORTNUM);
	fromlen = sizeof(dest);

	int ret = sendto(fd, buffer, len, 0, (struct sockaddr *)&dest, fromlen);
	TC_ASSERT_EQ("sendto", ret, NEG_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase           : tc_sendto_udpserver
* @brief              : udpserver
* @scenario           : used for udp connection
* @apicovered         : socket, bind, recvfrom
* @precondition       : valid file descriptor
* @postcondition      : none
*/
void tc_sendto_udpserver(void)
{
	int ret;
	struct sockaddr_in sa;
	
	int SocketFD = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	TC_ASSERT_EQ("socket", SocketFD, ZERO);

	char buffer[MAXRCVLEN];
	memset(&sa, 0, sizeof(sa));

	sa.sin_family = PF_INET;
	sa.sin_port = htons(PORTNUM);
	sa.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

	ret = bind(SocketFD, (struct sockaddr *)&sa, sizeof(sa));
	TC_ASSERT_NEQ("bind", SocketFD, NEG_VAL);

	struct sockaddr_storage serverStorage;
	socklen_t addr_size;

	ret = recvfrom(SocketFD, buffer, MAXRCVLEN, 0, (struct sockaddr *)&serverStorage, &addr_size);
	close(SocketFD);
	TC_ASSERT_EQ("recvfrom", ret, ZERO);
	TC_SUCCESS_RESULT();
}

/**
* @testcase           : tc_sendto_udpclient
* @brief              : udp client
* @scenario           : used for udp connection
* @apicovered         : socket
* @precondition       : valid file descriptor
* @postcondition      : none
*/
void tc_sendto_udpclient(void)
{
	int ret;
	int sock = socket(AF_INET, SOCK_DGRAM, ZERO);
	TC_ASSERT_NEQ("socket", sock, NEG_VAL);

   	char *buffer = "hello";
	int len = strlen(buffer) + ONE;
	struct sockaddr_in dest;
	socklen_t fromlen;

	memset(&dest, 0, sizeof(dest));
	dest.sin_family = PF_INET;
	dest.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	dest.sin_port = htons(PORTNUM);
	fromlen = sizeof(dest);

	ret = sendto(sock, buffer, len, 0, (struct sockaddr *)&dest, fromlen);
	close(sock);
	TC_ASSERT_EQ("sendto", ret, ZERO);
	TC_SUCCESS_RESULT();
}

/**
* @testcase           : sendto_udpserver
* @brief              : udpserver
* @scenario           : used for udp connection
* @apicovered         : socket, bind, recvfrom
* @precondition       : valid file descriptor
* @postcondition      : none
*/
void* sendto_udpserver(void *args)
{
	tc_sendto_udpserver();
	return NULL;
}


/**
* @testcase           : sendto_udpclient
* @brief              : udp client
* @scenario           : used for udp connection
* @apicovered         : socket
* @precondition       : valid file descriptor
* @postcondition      : none
*/
void* sendto_udpclient(void *args)
{
	tc_sendto_udpclient();
	return NULL;
}

/****************************************************************************
 * Name: sendto()
 ****************************************************************************/

int net_sendto_main(void)
{
	pthread_t Server, Client;

	pthread_create(&Server, NULL, sendto_udpserver, NULL);
	pthread_create(&Client, NULL, sendto_udpclient, NULL);

	pthread_join(Server, NULL);
	pthread_join(Client, NULL);
	return 0;
}
