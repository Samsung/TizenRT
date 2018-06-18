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

// @file tc_net_accept.c
// @brief Test Case Example for accept() API
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

#define PORTNUM 1108
#define MAXRCVLEN 20

static int s = 0;
static int g_client_running = 0; // 0 is wait, 1 is running, -1 is running the client fails
/**
   * @fn                   :wait
   * @brief                :function to wait on semaphore
   * @scenario             :
   * API's covered         :
   * Preconditions         :
   * Postconditions        :
   * @return               :void
   */
static int wait(void)
{
	while (s <= 0) {
		if (s == -2) {
			return -1;
		}
		printf("");
	}
	return 0;
}

/**
   * @fn                   :nw_signal
   * @brief                :function to signal semaphore
   * @scenario             :
   * API's covered         :
   * Preconditions         :
   * Postconditions        :
   * @return               :void
   */
static void nw_signal(void)
{
	s = 1;
}

static void nw_error(void)
{
	s = -2;
}

static int _wait_client(void)
{
	while (g_client_running == 0) {
		if (g_client_running == -1) {
			return -1;
		}
		sleep(1);
	}
	return 1;
}
/**
   * @testcase		   :tc_net_accept_p
   * @brief		   :
   * @scenario		   :
   * @apicovered	   :accept()
   * @precondition	   :
   * @postcondition	   :
   */

void tc_net_accept_p(int fd)
{
	int connect_fd = accept(fd, NULL, NULL);

	TC_ASSERT_GEQ("accept", connect_fd, 0);
	TC_SUCCESS_RESULT();

	close(connect_fd);
}

/**
   * @testcase		   :tc_net_accept_socket_n
   * @brief		   :
   * @scenario		   :
   * @apicovered	   :accept()
   * @precondition	   :
   * @postcondition	   :
   */

void tc_net_accept_socket_n(int fd)
{
	int connect_fd = accept(-1, NULL, NULL);

	TC_ASSERT_NEQ("accept", connect_fd, 0);
	TC_SUCCESS_RESULT();
}

/**
   * @fn                   :Server
   * @brief                :
   * @scenario             :
   * API's covered         :socket,bind,listen,close
   * Preconditions         :
   * Postconditions        :
   * @return               :void *
   */
void *Server(void *args)
{

	struct sockaddr_in sa;
	int socket_fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (socket_fd < 0) {
		nw_error();
		return 0;
	}

	memset(&sa, 0, sizeof(sa));

	sa.sin_family = PF_INET;
	sa.sin_port = htons(PORTNUM);
	sa.sin_addr.s_addr = inet_addr("127.0.0.1");

	int ret = bind(socket_fd, (struct sockaddr *)&sa, sizeof(sa));
	if (ret < 0) {
		nw_error();
		return 0;
	}

	ret = listen(socket_fd, 1);
	if (ret < 0) {
		nw_error();
		return 0;
	}

	nw_signal();
	tc_net_accept_p(socket_fd);
	tc_net_accept_socket_n(socket_fd);

	close(socket_fd);
	return 0;
}

/**
   * @fn                   :Client
   * @brief                :
   * @scenario             :
   * API's covered         :socket,connect,close
   * Preconditions         :
   * Postconditions        :
   * @return               :void *
   */
void *Client(void *args)
{

	int mysocket;
	struct sockaddr_in dest;

	mysocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (mysocket < 0) {
		g_client_running = -1;
		return 0;
	}

	memset(&dest, 0, sizeof(dest));
	dest.sin_family = PF_INET;
	dest.sin_addr.s_addr = inet_addr("127.0.0.1");
	dest.sin_port = htons(PORTNUM);

	g_client_running = 1; // running the client succeed.
	int ret = wait();
	if (ret < 0) {
		close(mysocket);
		return 0;
	}

	ret = connect(mysocket, (struct sockaddr *)&dest, sizeof(struct sockaddr));
	if (ret < 0) {
		close(mysocket);
		return 0;
	}

	close(mysocket);
	return 0;

}

/****************************************************************************
 * Name: accept()
 ****************************************************************************/
int net_accept_main(void)
{
	pthread_t server, client;
	s = 0;
	g_client_running = 0;
	pthread_create(&client, NULL, Client, NULL);
	int ret = _wait_client();
	if (ret < 0) {
		pthread_join(client, NULL);
		return -1;
	}

	pthread_create(&server, NULL, Server, NULL);

	pthread_join(server, NULL);
	pthread_join(client, NULL);

	return 0;
}
