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

// @file tc_net_send.c
// @brief Test Case Example for send() API
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

#define PORTNUM 1110
#define MAXRCVLEN 20
static int s1 = 0;
/**
   * @fn                   :wait1
   * @brief                :function to wait on semaphore
   * @scenario             :
   * API's covered         :
   * Preconditions         :
   * Postconditions        :
   * @return               :void
   */
static void wait1(void)
{
	while (s1 <= 0) {

		printf("");
	}
	s1--;
}

/**
   * @fn                   :signal1
   * @brief                :function to signal semaphore
   * @scenario             :
   * API's covered         :
   * Preconditions         :
   * Postconditions        :
   * @return               :void
   */
void signal1(void)
{
	s1++;
}

/**
   * @testcase		   :tc_net_send_p
   * @brief		   :
   * @scenario		   :
   * @apicovered	   :accept(),send()
   * @precondition	   :
   * @postcondition	   :
   */
void tc_net_send_p(int fd)
{

	char *msg = "Hello World !\n";
	int connect_fd = accept(fd, NULL, NULL);
	if (connect_fd < 0) {
		printf("connect fail %s:%d", __FUNCTION__, __LINE__);
		return;
	}
	int ret = send(connect_fd, msg, strlen(msg), 0);

	TC_ASSERT_NEQ_CLEANUP("send", ret, -1, close(connect_fd))
	TC_SUCCESS_RESULT()

	close(connect_fd);

}

/**
   * @fn                   :server
   * @brief                :
   * @scenario             :
   * API's covered         :socket,bind,listen,close
   * Preconditions         :
   * Postconditions        :
   * @return               :void *
   */
void *server(void *args)
{
	struct sockaddr_in sa;
	int socket_fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (socket_fd < 0) {
		printf("socket fail %s:%d", __FUNCTION__, __LINE__);
		return 0;
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
		printf("bind fail %s:%d", __FUNCTION__, __LINE__);
		close(socket_fd);
		return 0;
	}

	ret = listen(socket_fd, 2);
	if (ret < 0) {
		printf("listen fail %s:%d", __FUNCTION__, __LINE__);
		close(socket_fd);
		return 0;
	}
	signal1();
	tc_net_send_p(socket_fd);

	close(socket_fd);
	return 0;
}

/**
   * @fn                   :client
   * @brief                :
   * @scenario             :
   * API's covered         :socket,connect,recv,close
   * Preconditions         :
   * Postconditions        :
   * @return               :void *
   */
void *client(void *args)
{

	char buffer[MAXRCVLEN];
	int len, mysocket;
	struct sockaddr_in dest;

	mysocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (mysocket < 0) {
		printf("socket fail %s:%d", __FUNCTION__, __LINE__);
		return 0;
	}
	memset(&dest, 0, sizeof(dest));
	dest.sin_family = PF_INET;
	dest.sin_addr.s_addr = inet_addr("127.0.0.1");
	dest.sin_port = htons(PORTNUM);

	wait1();

	int ret = connect(mysocket, (struct sockaddr *)&dest, sizeof(struct sockaddr));
	if (ret < 0) {
		printf("connect fail %s:%d", __FUNCTION__, __LINE__);
		close(mysocket);
		return 0;
	}

	len = recv(mysocket, buffer, MAXRCVLEN, 0);
	if (len <= 0) {
		printf("recv fail %s:%d\n", __FUNCTION__, __LINE__);
		close(mysocket);
		return 0;
	}
	buffer[len] = '\0';

	close(mysocket);
	return 0;

}

/****************************************************************************
 * Name: send()
 ****************************************************************************/
int net_send_main(void)
{

	pthread_t Server, Client;
	pthread_create(&Server, NULL, server, NULL);
	pthread_create(&Client, NULL, client, NULL);

	pthread_join(Server, NULL);

	pthread_join(Client, NULL);

	return 0;
}
