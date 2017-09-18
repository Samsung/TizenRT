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
//#include <arch/board/board.h>
#include <netutils/netlib.h>

#include <sys/socket.h>
#include <pthread.h>

#include "tc_internal.h"

#define PORTNUM 1115
#define MAXRCVLEN 20
int sem = 0;

/**
* @fn					: getpeername_wait
* @brief				: function to wait on semaphore
* @scenario				:
* API's covered			:
* Preconditions			:
* Postconditions		:
* @return				: void
*/
void getpeername_wait(void)
{
	while (sem <= 0) {
		printf("");
	}

	sem--;
}

/**
* @fn					: getpeername_signal
* @brief				: function to signal semaphore
* @scenario				:
* API's covered			:
* Preconditions			:
* Postconditions		:
* @return				: void
*/
void getpeername_signal(void)
{
	sem++;
}

/**
* @testcase				: tc_net_getpeername_p1
* @brief				: positive test cases without client server model
* @scenario				:
* @apicovered			: getpeername()
* @precondition			:
* @postcondition		:
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
* @testcase				: tc_net_getpeername_sock_n
* @brief				: negative test case wthout client server model
* @scenario				:
* @apicovered			: getpeername()
* @precondition			:
* @postcondition		:
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
* @testcase				: tc_net_getpeername_close_n
* @brief				: negative test case without client server model
* @scenario				:
* @apicovered			: getpeername()
* @precondition			:
* @postcondition		:
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
* @testcase				: tc_net_getpeername_unix_p
* @brief				:
* @scenario				:
* @apicovered			: getpeername()
* @precondition			:
* @postcondition		:
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
* @testcase				: tc_net_getpeername_udp_p
* @brief				:
* @scenario				:
* @apicovered			: getpeername()
* @precondition			:
* @postcondition		:
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
* @testcase				: tc_net_getpeername_p
* @brief				: positive testcase for  getpeername api with client server model
* @scenario				:
* @apicovered			: getpeername()
* @precondition			:
* @postcondition		:
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
* @testcase				: tc_net_getpeername_n
* @brief				: negative testcase for  getpeername api with client server model
* @scenario				:
* @apicovered			: getpeername()
* @precondition			:
* @postcondition		:
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
* @fn					: getpeername_server
* @brief				:
* @scenario				:
* API's covered			: socket,bind,listen,accept,close
* Preconditions			:
* Postconditions		:
* @return				: void *
*/
void *getpeername_server(void *args)
{
	int ConnectFD, ret;
	struct sockaddr_in sa;

	int sock = socket(AF_INET, SOCK_STREAM, 0);
	memset(&sa, 0, sizeof(sa));

	sa.sin_family = PF_INET;
	sa.sin_port = htons(PORTNUM);
	sa.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

	ret = bind(sock, (struct sockaddr *)&sa, sizeof(sa));

	ret = listen(sock, 1);

	getpeername_signal();
	ConnectFD = accept(sock, NULL, NULL);

	close(ConnectFD);
	close(sock);
	return NULL;
}

/**
* @fn					: getpeername_client
* @brief				:
* @scenario				:
* API's covered			: socket,connect,close
* Preconditions			:
* Postconditions		:
* @return				: void *
*/
void *getpeername_client(void *args)
{
	int ret;
	struct sockaddr_in dest;
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	memset(&dest, 0, sizeof(dest));
	dest.sin_family = PF_INET;
	dest.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	dest.sin_port = htons(PORTNUM);

	getpeername_wait();
	ret = connect(sock, (struct sockaddr *)&dest, sizeof(struct sockaddr));

	tc_net_getpeername_p(sock);
	tc_net_getpeername_n(sock);
	close(sock);

	return NULL;
}

void tc_client_server(void)
{
	int ret;
	pthread_t Server, Client;

	ret = pthread_create(&Server, NULL, getpeername_server, NULL);
	ret = pthread_create(&Client, NULL, getpeername_client, NULL);

	ret = pthread_join(Server, NULL);

	ret = pthread_join(Client, NULL);

}

/****************************************************************************
 * Name: getpeername()
 ****************************************************************************/
int net_getpeername_main(void)
{
	tc_client_server();
	return 0;
}
