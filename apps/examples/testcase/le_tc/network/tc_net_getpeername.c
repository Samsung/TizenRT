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
#include <netutils/netlib.h>
#include <semaphore.h>

#include "tc_internal.h"

#define PORTNUM 1115
#define MAXRCVLEN 20

static sem_t g_sig_clitomain;
static sem_t g_sig_srvtocli;

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
		assert(0);
	}

	if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &(int){ 1 }, sizeof(int)) < 0) {
		printf("setsockopt(SO_REUSEADDR) failed %s:%d:%d\n", __FUNCTION__, __LINE__, errno);
		close(socket_fd);
		return 0;
	}

	memset(&sa, 0, sizeof(sa));
	sa.sin_family = PF_INET;
	sa.sin_port = htons(PORTNUM);
	sa.sin_addr.s_addr = inet_addr("127.0.0.1");

	int ret = bind(socket_fd, (struct sockaddr *)&sa, sizeof(sa));
	if (ret < 0) {
		printf("bind fail %s :%d", __FUNCTION__, __LINE__);
		assert(0);
	}

	ret = listen(socket_fd, 1);
	if (ret < 0) {
		printf("listen fail %s :%d", __FUNCTION__, __LINE__);
		assert(0);
	}

	sem_post(&g_sig_srvtocli);

	int connect_fd = accept(socket_fd, NULL, NULL);
	if (connect_fd == -1) {
		printf("accept fail %s %s %d\n", strerror(errno), __FUNCTION__, __LINE__);
		assert(0);
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
		assert(0);
	}

	memset(&dest, 0, sizeof(dest));
	dest.sin_family = PF_INET;
	dest.sin_addr.s_addr = inet_addr("127.0.0.1");
	dest.sin_port = htons(PORTNUM);

	sem_post(&g_sig_clitomain);
	int ret = sem_wait(&g_sig_srvtocli);
	if (ret != 0) {
		assert(0);
	}

	ret = connect(mysocket, (struct sockaddr *)&dest, sizeof(struct sockaddr));
	if (ret < 0) {
		printf("connect fail %s %s %d\n", strerror(errno), __FUNCTION__, __LINE__);
		assert(0);
	}
	tc_net_getpeername_p(mysocket);
	tc_net_getpeername_n(mysocket);

	close(mysocket);

	return 0;
}

static void _initialize(void)
{
	int ret = sem_init(&g_sig_clitomain, 0, 0);
	if (ret != 0) {
		assert(0);
	}
	ret = sem_init(&g_sig_srvtocli, 0, 0);
	if (ret != 0) {
		assert(0);
	}
}

static void _deinitialize(void)
{
	sem_destroy(&g_sig_clitomain);
	sem_destroy(&g_sig_srvtocli);
}

/****************************************************************************
 * Name: getpeername()
 ****************************************************************************/
int net_getpeername_main(void)
{
	pthread_t Server;
	pthread_t Client;

	_initialize();
	pthread_create(&Client, NULL, getpeername_client, NULL);
	int res = sem_wait(&g_sig_clitomain);
	if (res != 0) {
		assert(0);
	}
	pthread_create(&Server, NULL, getpeername_server, NULL);

	pthread_join(Server, NULL);
	pthread_join(Client, NULL);

	tc_net_getpeername_sock_n();
	tc_net_getpeername_close_n();

	_deinitialize();

	return 0;
}
