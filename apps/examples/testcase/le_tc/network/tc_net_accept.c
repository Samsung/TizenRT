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
int s = 0;
/**
   * @fn                   :wait
   * @brief                :function to wait on semaphore
   * @scenario             :
   * API's covered         :
   * Preconditions         :
   * Postconditions        :
   * @return               :void
   */
static void wait(void)
{
	while (s <= 0) {

		printf("");
	}
	s--;
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
	s++;
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
	int ConnectFD = accept(fd, NULL, NULL);

	TC_ASSERT_GEQ("accept", ConnectFD, 0);
	TC_SUCCESS_RESULT();

	close(ConnectFD);
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
	int ConnectFD = accept(-1, NULL, NULL);

	TC_ASSERT_NEQ("accept", ConnectFD, 0);
	TC_SUCCESS_RESULT();

	close(ConnectFD);
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
	int SocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	memset(&sa, 0, sizeof(sa));

	sa.sin_family = PF_INET;
	sa.sin_port = htons(PORTNUM);
	sa.sin_addr.s_addr = inet_addr("127.0.0.1");

	bind(SocketFD, (struct sockaddr *)&sa, sizeof(sa));

	listen(SocketFD, 1);

	nw_signal();
	tc_net_accept_p(SocketFD);
	tc_net_accept_socket_n(SocketFD);

	close(SocketFD);
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

	memset(&dest, 0, sizeof(dest));
	dest.sin_family = PF_INET;
	dest.sin_addr.s_addr = inet_addr("127.0.0.1");
	dest.sin_port = htons(PORTNUM);

	wait();

	connect(mysocket, (struct sockaddr *)&dest, sizeof(struct sockaddr));

	close(mysocket);
	return 0;

}

/****************************************************************************
 * Name: accept()
 ****************************************************************************/
int net_accept_main(void)
{

	pthread_t server, client;

	pthread_create(&server, NULL, Server, NULL);
	pthread_create(&client, NULL, Client, NULL);
	pthread_join(server, NULL);

	pthread_join(client, NULL);

	return 0;
}
