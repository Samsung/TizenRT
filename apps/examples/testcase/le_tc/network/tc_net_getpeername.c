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

// @file tc_net_getpeername.c
// @brief Test Case Example for getpeername() API
#include <tinyara/config.h>
#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netutils/netlib.h>
#include <sys/socket.h>
#include <pthread.h>

#include "tc_internal.h"

#define PORTNUM        5002
#define MAXRCVLEN      20
#define BACKLOG        1

static int count_wait;

/**
* @fn                   : getpeername_wait
* @brief                : function to wait on semaphore
* @scenario             : use wait function to decrement sem value.
* API's covered         : none
* Preconditions         : none
* Postconditions        : none
* @return               : void
*/
void getpeername_wait(void)
{
	while (count_wait <= ZERO) {
		printf("");
	}

	count_wait--;
}

/**
* @fn                   : getpeername_signal
* @brief                : function to signal semaphore
* @scenario             : use to increase the sem value.
* API's covered         : none
* Preconditions         : none
* Postconditions        : none
* @return               : void
*/
void getpeername_signal(void)
{
	count_wait++;
}

/**
* @testcase            : tc_net_getpeername_p1
* @brief               : get name of connected peer socket.
* @scenario            : get the peer address of the specified socket.
* @apicovered          : getpeername()
* @precondition        : socket file descriptor.
* @postcondition       : none
* @return              : void
*/
static void tc_net_getpeername_p1(int sock)
{
	int len, ret;
	struct sockaddr foo;

	len = sizeof(struct sockaddr);
	ret = getpeername(sock, &foo, (socklen_t *)&len);
	TC_ASSERT_NEQ("getpeername", ret, NEG_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_getpeername_sock_n
* @brief               : get name of connected peer socket.
* @scenario            : get the peer address of the specified socket, with invalid socket fd.
* @apicovered          : getpeername()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_getpeername_sock_n(void)
{
	int len, ret;
	struct sockaddr foo;

	len = sizeof(struct sockaddr);
	ret = getpeername(NEG_VAL, &foo, (socklen_t *)&len);
	TC_ASSERT_EQ("getpeername", ret, NEG_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_getpeername_close_n
* @brief               : get name of connected peer socket.
* @scenario            : get the peer address of the specified socket.
* @apicovered          : getpeername()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_getpeername_close_n(int sock)
{
	int len, ret;
	struct sockaddr foo;

	len = sizeof(struct sockaddr);
	ret = getpeername(sock, &foo, (socklen_t *)&len);
	TC_ASSERT_EQ("getpeername", ret, NEG_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_getpeername_unix_p
* @brief               : get name of connected peer socket.
* @scenario            : get the peer address of the specified socket.
* @apicovered          : getpeername()
* @precondition        : socket file descriptor.
* @postcondition       : none
* @return              : void
*/
static void tc_net_getpeername_unix_p(int sock)
{
	int len, ret;
	struct sockaddr foo;

	len = sizeof(struct sockaddr);
	ret = getpeername(sock, &foo, (socklen_t *)&len);
	TC_ASSERT_NEQ("getpeername", ret, NEG_VAL);
	TC_SUCCESS_RESULT();

}

/**
* @testcase            : tc_net_getpeername_udp_p
* @brief               : get name of connected peer socket.
* @scenario            : get the peer address of the specified socket.
* @apicovered          : getpeername()
* @precondition        : socket file descriptor.
* @postcondition       : none
* @return              : void
*/
static void tc_net_getpeername_udp_p(int sock)
{
	int ret, len;
	struct sockaddr foo;

	len = sizeof(struct sockaddr);
	ret = getpeername(sock, &foo, (socklen_t *)&len);
	TC_ASSERT_NEQ("getpeername", ret, NEG_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_getpeername_p
* @brief               : get name of connected peer socket.
* @scenario            : get the peer address of the specified socket.
* @apicovered          : getpeername()
* @precondition        : socket file descriptor.
* @postcondition       : none
* @return              : void
*/
static void tc_net_getpeername_p(int fd)
{
	int ret;
	socklen_t len;
	struct sockaddr_storage addr;

	len = sizeof(addr);
	ret = getpeername(fd, (struct sockaddr *)&addr, &len);
	TC_ASSERT_NEQ("getpeername", ret, NEG_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_getpeername_n
* @brief               : get name of connected peer socket.
* @scenario            : get the peer address of the specified socket, with invalid socket fd.
* @apicovered          : getpeername()
* @precondition        : socket file descriptor.
* @postcondition       : none
* @return              : void
*/
static void tc_net_getpeername_n(int fd)
{
	int ret;
	socklen_t len;
	struct sockaddr_storage addr;

	len = sizeof(addr);

	ret = getpeername(NEG_VAL, (struct sockaddr *)&addr, &len);
	TC_ASSERT_EQ("getpeername", ret, NEG_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : net_getpeername_server
* @brief               : create a tcp server.
* @scenario            : create a tcp server to test getpeername api.
* @apicovered          : socket,bind,listen,accept,close
* @precondition        : socket file descriptor.
* @postcondition       : none
* @return              : void
*/
void net_getpeername_server(void)
{
	int ConnectFD;
	int ret;
	struct sockaddr_in sa;

	int sock = socket(AF_INET, SOCK_STREAM, 0);
	TC_ASSERT_NEQ("socket", sock, NEG_VAL);

	memset(&sa, 0, sizeof(sa));
	sa.sin_family = PF_INET;
	sa.sin_port = htons(PORTNUM);
	sa.sin_addr.s_addr = INADDR_LOOPBACK;

	ret = bind(sock, (struct sockaddr *)&sa, sizeof(sa));
	TC_ASSERT_NEQ("bind", ret, NEG_VAL);

	ret = listen(sock, BACKLOG);
	TC_ASSERT_NEQ("listen", ret, NEG_VAL);

	getpeername_signal();
	ConnectFD = accept(sock, NULL, NULL);
	close(sock);
	TC_ASSERT_NEQ("accept", ret, NEG_VAL);
	close(ConnectFD);
}

/**
* @testcase            : net_getpeername_client
* @brief               : create client.
* @scenario            : create tcp client.
* @apicovered          : socket,connect,close
* @precondition        : socket file descriptor.
* @postcondition       : none
* @return              : void
*/
void net_getpeername_client(void)
{
	int ret;
	struct sockaddr_in dest;

	int sock = socket(AF_INET, SOCK_STREAM, 0);
	TC_ASSERT_NEQ("socket", sock, NEG_VAL);

	memset(&dest, 0, sizeof(dest));
	dest.sin_family = PF_INET;
	dest.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	dest.sin_port = htons(PORTNUM);

	getpeername_wait();
	ret = connect(sock, (struct sockaddr *)&dest, sizeof(struct sockaddr));
	TC_ASSERT_NEQ("connect", ret, NEG_VAL);

	tc_net_getpeername_p(sock);
	tc_net_getpeername_n(sock);
	close(sock);
}

/**
* @testcase            : getpeername_server
* @brief               : invoking getpeername server
* @scenario            : none
* @apicovered          : none
* @precondition        : none
* @postcondition       : none
* @return              : void*
*/
void* getpeername_server(void *args)
{
	net_getpeername_server();
	return NULL;
}

/**
* @testcase            : getpeername_client
* @brief               : invoking getpeername client
* @scenario            : none
* @apicovered          : none
* @precondition        : none
* @postcondition       : none
* @return              : void*
*/
void* getpeername_client(void *args)
{
	net_getpeername_client();
	return NULL;
}

/**
* @testcase            : client_server
* @brief               : This api create client and server thread.
* @scenario            : Create client and server thread to test getpeername api.
* @apicovered          : none
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void client_server(void)
{
	pthread_t Server, Client;

	pthread_create(&Server, NULL, getpeername_server, NULL);
	pthread_create(&Client, NULL, getpeername_client, NULL);

	pthread_join(Server, NULL);
	pthread_join(Client, NULL);
}

/****************************************************************************
 * Name: getpeername()
 ****************************************************************************/
int net_getpeername_main(void)
{
	client_server();
	return 0;
}
