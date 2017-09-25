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

#define PORTNUM        5001
#define MAXRCVLEN      20
#define BACKLOG        1

static int count_wait;

/**
* @fn                    :wait
* @brief                 :function to wait on semaphore.
* @scenario              :use wait function to decrement count value.
* @API's covered         :none
* @Preconditions         :none
* @Postconditions        :none
* @return                :void
*/
static void wait(void)
{
	while (count_wait <= ZERO) {
		printf("");
	}
	count_wait--;
}

/**
* @fn                    :nw_signal
* @brief                 :function to signal semaphore.
* @scenario              :use to increase the count value.
* @API's covered         :none
* @Preconditions         :none
* @Postconditions        :none
* @return                :void
*/
static void nw_signal(void)
{
	count_wait++;
}

/**
* @testcase             :tc_net_accept_p
* @brief                :accept a connection on a socket.
* @scenario             :extracts the first connection request on the queue of pending connections
*                        for the listening socke and creates a new connected socket.
* @apicovered           :accept()
* @precondition         :socket file descriptor.
* @postcondition        :none
* @return               :void
*/
void tc_net_accept_p(int fd)
{
	int ConnectFD = accept(fd, NULL, NULL);

	close(ConnectFD);
	TC_ASSERT_NEQ("accept", ConnectFD, NEG_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase             :tc_net_accept_socket_n
* @brief                :accept a connection on a socket.
* @scenario             :extracts the first connection request on the queue of pending connections
*                        for the listening socke and creates a new connected socket, with invalid fd.
* @apicovered           :accept()
* @precondition         :none
* @postcondition        :none
* @return               :void
*/
void tc_net_accept_socket_n(void)
{
	int ConnectFD = accept(NEG_VAL, NULL, NULL);
	TC_ASSERT_EQ("accept", ConnectFD, NEG_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @fn                   :net_accept_server
* @brief                :create a tcp server.
* @scenario             :create a tcp server to test accept api.
* @API's covered        :socket,bind,listen,close
* @Preconditions        :socket file descriptor.
* @Postconditions       :none
* @return               :void
*/
static void net_accept_server(void)
{
	int ret;
	struct sockaddr_in sa;

	int sock = socket(AF_INET, SOCK_STREAM, 0);
	TC_ASSERT_NEQ("socket", sock, NEG_VAL);

	memset(&sa, 0, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(PORTNUM);
	sa.sin_addr.s_addr = INADDR_LOOPBACK;

	ret = bind(sock, (struct sockaddr *)&sa, sizeof(sa));
	TC_ASSERT_NEQ_CLEANUP("bind", ret, NEG_VAL, close(sock));

	ret = listen(sock, BACKLOG);
	TC_ASSERT_NEQ_CLEANUP("listen", ret, NEG_VAL, close(sock));

	nw_signal();
	tc_net_accept_p(sock);
	tc_net_accept_socket_n();
	close(sock);
}

/**
* @fn                   :net_accept_client
* @brief                :create the client.
* @scenario             :create tcp client.
* @API's covered        :socket,connect,close
* @Preconditions        :socket file descriptor.
* @Postconditions       :none
* @return               :void
*/
static void net_accept_client(void)
{
	int ret;
	struct sockaddr_in dest;

	int sock = socket(AF_INET, SOCK_STREAM, 0);
	TC_ASSERT_NEQ("socket", sock, NEG_VAL);

	memset(&dest, 0, sizeof(dest));
	dest.sin_family = AF_INET;
	dest.sin_addr.s_addr = INADDR_LOOPBACK;
	dest.sin_port = htons(PORTNUM);

	wait();
	ret = connect(sock, (struct sockaddr *)&dest, sizeof(struct sockaddr));
	TC_ASSERT_NEQ_CLEANUP("connect", ret, NEG_VAL, close(sock));
	close(sock);
}

/**
* @fn                   :Server
* @brief                :create a tcp server thread.
* @scenario             :create a tcp server to test accept api.
* @API's covered        :none
* @Preconditions        :none
* @Postconditions       :none
* @return               :void*
*/
void* Server(void *args)
{
	net_accept_server();
	return NULL;
}

/**
* @fn                   :Client
* @brief                :create client thread.
* @scenario             :create tcp client.
* @API's covered        :none
* @Preconditions        :none
* @Postconditions       :none
* @return               :void*
*/
void* Client(void *args)
{
	net_accept_client();
	return NULL;
}

/**
* @fn                   :net_accept
* @brief                :create client and server thread.
* @scenario             :create client and server thread to test accept api.
* @API's covered        :none
* @Preconditions        :none
* @Postconditions       :none
* @return               :void
*/
void net_accept(void)
{
	pthread_t server, client;

	pthread_create(&server, NULL, Server, NULL);
	pthread_create(&client, NULL, Client, NULL);

	pthread_join(server, NULL);
	pthread_join(client, NULL);
}

/****************************************************************************
 * Name: accept
 ****************************************************************************/
int net_accept_main(void)
{
	net_accept();
	return 0;
}
