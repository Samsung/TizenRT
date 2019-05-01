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

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <stdio.h>
#include <errno.h>
#include <pthread.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <netinet/in.h>

//#include <arch/board/board.h>
#include <netutils/netlib.h>

#include "tc_internal.h"

#define PORTNUM 1115
#define MAXRCVLEN 20
static int g_sem = 0;
static int g_running_client = 0;

/**
   * @fn                   :getpeername_wait
   * @brief                :function to wait on semaphore
   * @scenario             :
   * API's covered         :
   * Preconditions         :
   * Postconditions        :
   * @return               :void
   */
static int getpeername_wait(void)
{
	while (g_sem <= 0) {
		if (g_sem == -1) {
			return -1;
		}
		sleep(1);
	}
	return 1;
}

/**
   * @fn                   :getpeername_signal
   * @brief                :function to signal semaphore
   * @scenario             :
   * API's covered         :
   * Preconditions         :
   * Postconditions        :
   * @return               :void
   */
static void getpeername_signal(void)
{
	g_sem = 1;
}

static void getpeername_error(void)
{
	g_sem = -1;
}
static int _wait_client(void)
{
	while (g_running_client <= 0) {
		if (g_running_client == -1) {
			return -1;
		}
	}
	return 1;
}
/**
   * @testcase		   :tc_net_getpeername_sock_n
   * @brief		   :negative test case wthout client server model
   * @scenario		   :
   * @apicovered	   :getpeername()
   * @precondition	   :
   * @postcondition	   :
   */
static void tc_net_getpeername_sock_n(void)
{
	int len = sizeof(struct sockaddr);
	struct sockaddr foo;
	int ret = getpeername(-1, &foo, (socklen_t *)&len);

	TC_ASSERT_EQ("getpeername", ret, -1);
	TC_SUCCESS_RESULT();
}


/**
   * @testcase		   :tc_net_getpeername_close_n
   * @brief		   :negative test case without client server model
   * @scenario		   :
   * @apicovered	   :getpeername()
   * @precondition	   :
   * @postcondition	   :
   */
static void tc_net_getpeername_close_n(void)
{
	int sock;
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		printf("socket creation error (%s) line:%d\n", __FUNCTION__, __LINE__);
		return;
	}
	int len = sizeof(struct sockaddr);
	struct sockaddr foo;
	close(sock);
	int ret = getpeername(sock, &foo, (socklen_t *)&len);

	TC_ASSERT_EQ("getpeername", ret, -1);
	TC_SUCCESS_RESULT();
}
#ifdef AF_UNIX
/**
   * @testcase		   :tc_net_getpeername_unix_p
   * @brief		   :
   * @scenario		   :
   * @apicovered	   :getpeername()
   * @precondition	   :
   * @postcondition	   :
   */
static void tc_net_getpeername_unix_p(void)
{
	int sock;
	int len = sizeof(struct sockaddr);
	struct sockaddr foo;

	sock = socket(AF_UNIX, SOCK_STREAM, 0);
	int ret = getpeername(sock, &foo, (socklen_t *)&len);

	close(sock);

	TC_ASSERT_NEQ("getpeername", ret, -1);
	TC_SUCCESS_RESULT();
}
#endif

/**
   * @testcase		   :tc_net_getpeername_p
   * @brief		   :positive testcase for  getpeername api with client server model
   * @scenario		   :
   * @apicovered	   :getpeername()
   * @precondition	   :
   * @postcondition	   :
   */
static void tc_net_getpeername_p(int fd)
{
	socklen_t len;
	struct sockaddr_storage addr;
	len = sizeof(addr);

	int ret = getpeername(fd, (struct sockaddr *)&addr, &len);

	TC_ASSERT_NEQ("getpeername", ret, -1);
	TC_SUCCESS_RESULT();
}

/**
   * @testcase		   :tc_net_getpeername_n
   * @brief		   :negative testcase for  getpeername api with client server model
   * @scenario		   :
   * @apicovered	   :getpeername()
   * @precondition	   :
   * @postcondition	   :
   */
static void tc_net_getpeername_n(int fd)
{
	socklen_t len;
	struct sockaddr_storage addr;
	len = sizeof(addr);

	int ret = getpeername(-1, (struct sockaddr *)&addr, &len);

	TC_ASSERT_EQ("getpeername", ret, -1);
	TC_SUCCESS_RESULT();
}

/**
   * @fn                   :getpeername_server
   * @brief                :
   * @scenario             :
   * API's covered         :socket,bind,listen,accept,close
   * Preconditions         :
   * Postconditions        :
   * @return               :void *
   */
void *getpeername_server(void *args)
{
	struct sockaddr_in sa;
	int socket_fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (socket_fd < 0) {
		printf("socket creation fail %s :%d", __FUNCTION__, __LINE__);
		getpeername_error();
		return 0;
	}
	memset(&sa, 0, sizeof(sa));

	sa.sin_family = PF_INET;
	sa.sin_port = htons(PORTNUM);
	sa.sin_addr.s_addr = inet_addr("127.0.0.1");

	int ret = bind(socket_fd, (struct sockaddr *)&sa, sizeof(sa));
	if (ret < 0) {
		printf("bind fail %s :%d", __FUNCTION__, __LINE__);
		getpeername_error();
		close(socket_fd);
		return 0;
	}

	ret = listen(socket_fd, 1);
	if (ret < 0) {
		printf("listen fail %s :%d", __FUNCTION__, __LINE__);
		getpeername_error();
		close(socket_fd);
		return 0;
	}
	getpeername_signal();
	int connect_fd = accept(socket_fd, NULL, NULL);
	if (connect_fd == -1) {
		printf("accept fail\n");
		close(socket_fd);
		return 0;
	}

	close(connect_fd);
	close(socket_fd);
	return 0;
}

/**
   * @fn                   :getpeername_client
   * @brief                :
   * @scenario             :
   * API's covered         :socket,connect,close
   * Preconditions         :
   * Postconditions        :
   * @return               :void *
   */
void *getpeername_client(void *args)
{
	int mysocket;
	struct sockaddr_in dest;
	mysocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (mysocket < 0) {
		printf("socket creation fail %s :%d\n", __FUNCTION__, __LINE__);
		g_running_client = -1;
		return 0;
	}

	memset(&dest, 0, sizeof(dest));
	dest.sin_family = PF_INET;
	dest.sin_addr.s_addr = inet_addr("127.0.0.1");
	dest.sin_port = htons(PORTNUM);

	g_running_client = 1;
	int ret = getpeername_wait();
	if (ret < 0) {
		printf("running server fail\n");
		close(mysocket);
		return 0;
	}
	ret = connect(mysocket, (struct sockaddr *)&dest, sizeof(struct sockaddr));
	if (ret < 0) {
		printf("connect fail %s %d\n", __FUNCTION__, __LINE__);
		close(mysocket);
		return 0;
	}

	tc_net_getpeername_p(mysocket);
	tc_net_getpeername_n(mysocket);

	close(mysocket);
	return 0;
}

/****************************************************************************
 * Name: getpeername()
 ****************************************************************************/
int net_getpeername_main(void)
{

	pthread_t Server, Client;
	g_sem = 0;
	g_running_client = 0;

	pthread_create(&Client, NULL, getpeername_client, NULL);
	int ret = _wait_client();
	if (ret < 0) {
		printf("running client fail %s :%d\n", __FUNCTION__, __LINE__);
		pthread_join(Client, NULL);
		return -1;
	}

	pthread_create(&Server, NULL, getpeername_server, NULL);

	pthread_join(Server, NULL);
	pthread_join(Client, NULL);

	tc_net_getpeername_sock_n();
	tc_net_getpeername_close_n();

	return 0;
}
