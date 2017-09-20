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

#define PORTNUM 1115
#define MAXRCVLEN 20
int sem = 0;

/**
* @fn                   : getpeername_wait
* @brief                : Function to wait on semaphore
* @scenario             : use wait function to decrement sem value.
* API's covered         : none
* Preconditions         : none
* Postconditions        : none
* @return               : void
*/
void getpeername_wait(void)
{
	while (sem <= 0) {
		printf("");
	}

	sem--;
}

/**
* @fn                   : getpeername_signal
* @brief                : Function to signal semaphore
* @scenario             : use to increase the sem value.
* API's covered         : none
* Preconditions         : none
* Postconditions        : none
* @return               : void
*/
void getpeername_signal(void)
{
	sem++;
}

/**
* @testcase            : tc_net_getpeername_p1
* @brief               : This getpeername API get name of connected peer socket.
* @scenario            : Get the peer address of the specified socket.
* @apicovered          : getpeername()
* @precondition        : socket file descriptor.
* @postcondition       : none
* @return              : void
*/
static void tc_net_getpeername_p1(int sock)
{
	int len = sizeof(struct sockaddr);
	struct sockaddr foo;

	int ret = getpeername(sock, &foo, (socklen_t *)&len);
	TC_ASSERT_NEQ("getpeername", ret, NEG_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_getpeername_sock_n
* @brief               : This getpeername API get name of connected peer socket.
* @scenario            : Get the peer address of the specified socket, with invalid socket fd.
* @apicovered          : getpeername()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_getpeername_sock_n(void)
{
	int len = sizeof(struct sockaddr);
	struct sockaddr foo;
	int ret = getpeername(NEG_VAL, &foo, (socklen_t *)&len);
	TC_ASSERT_EQ("getpeername", ret, NEG_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_getpeername_close_n
* @brief               : This getpeername API get name of connected peer socket.
* @scenario            : Get the peer address of the specified socket.
* @apicovered          : getpeername()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_net_getpeername_close_n(int sock)
{
	int len = sizeof(struct sockaddr);
	struct sockaddr foo;

	int ret = getpeername(sock, &foo, (socklen_t *)&len);
	TC_ASSERT_EQ("getpeername", ret, NEG_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_getpeername_unix_p
* @brief               : This getpeername API get name of connected peer socket.
* @scenario            : Get the peer address of the specified socket.
* @apicovered          : getpeername()
* @precondition        : socket file descriptor.
* @postcondition       : none
* @return              : void
*/
static void tc_net_getpeername_unix_p(int sock)
{
	int len = sizeof(struct sockaddr);
	struct sockaddr foo;

	int ret = getpeername(sock, &foo, (socklen_t *)&len);
	TC_ASSERT_NEQ("getpeername", ret, NEG_VAL);
	TC_SUCCESS_RESULT();

}

/**
* @testcase            : tc_net_getpeername_udp_p
* @brief               : This getpeername API get name of connected peer socket.
* @scenario            : Get the peer address of the specified socket.
* @apicovered          : getpeername()
* @precondition        : socket file descriptor.
* @postcondition       : none
* @return              : void
*/
static void tc_net_getpeername_udp_p(int sock)
{
	int len = sizeof(struct sockaddr);
	struct sockaddr foo;

	int ret = getpeername(sock, &foo, (socklen_t *)&len);
	TC_ASSERT_NEQ("getpeername", ret, NEG_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_getpeername_p
* @brief               : This getpeername API get name of connected peer socket.
* @scenario            : Get the peer address of the specified socket.
* @apicovered          : getpeername()
* @precondition        : socket file descriptor.
* @postcondition       : none
* @return              : void
*/
static void tc_net_getpeername_p(int fd)
{
	socklen_t len;
	struct sockaddr_storage addr;
	len = sizeof(addr);

	int ret = getpeername(fd, (struct sockaddr *)&addr, &len);
	TC_ASSERT_NEQ("getpeername", ret, NEG_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_getpeername_n
* @brief               : This getpeername API get name of connected peer socket.
* @scenario            : Get the peer address of the specified socket, with invalid socket fd.
* @apicovered          : getpeername()
* @precondition        : socket file descriptor.
* @postcondition       : none
* @return              : void
*/
static void tc_net_getpeername_n(int fd)
{
	socklen_t len;
	struct sockaddr_storage addr;
	len = sizeof(addr);

	int ret = getpeername(NEG_VAL, (struct sockaddr *)&addr, &len);
	TC_ASSERT_EQ("getpeername", ret, NEG_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : getpeername_server
* @brief               : Create a Tcp server.
* @scenario            : Create a tcp server for checking getpeername api.
* @apicovered          : socket,bind,listen,accept,close
* @precondition        : socket file descriptor.
* @postcondition       : none
* @return              : void
*/
void *getpeername_server(void *args)
{
	int ConnectFD;
	struct sockaddr_in sa;

	int sock = socket(AF_INET, SOCK_STREAM, 0);
	memset(&sa, 0, sizeof(sa));

	sa.sin_family = PF_INET;
	sa.sin_port = htons(PORTNUM);
	sa.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

	bind(sock, (struct sockaddr *)&sa, sizeof(sa));
	listen(sock, 1);

	getpeername_signal();
	ConnectFD = accept(sock, NULL, NULL);
	close(ConnectFD);
	close(sock);
	return NULL;
}

/**
* @testcase            : getpeername_client
* @brief               : This api create client.
* @scenario            : Create tcp client.
* @apicovered          : socket,connect,close
* @precondition        : socket file descriptor.
* @postcondition       : none
* @return              : void
*/
void *getpeername_client(void *args)
{
	struct sockaddr_in dest;
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	memset(&dest, 0, sizeof(dest));
	dest.sin_family = PF_INET;
	dest.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	dest.sin_port = htons(PORTNUM);

	getpeername_wait();
	connect(sock, (struct sockaddr *)&dest, sizeof(struct sockaddr));

	tc_net_getpeername_p(sock);
	tc_net_getpeername_n(sock);
	close(sock);
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
