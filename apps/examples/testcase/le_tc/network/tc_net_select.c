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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <pthread.h>

#include "tc_internal.h"

#define PORTNUM        5004
#define MAXRCVLEN      20
#define BUFSIZE        20

static int count_wait;

/**
* @fn                   : wait1
* @brief                : function to wait on semaphore.
* @scenario             : waitine wait function to decrement count value.
* API's covered         : none
* Preconditions         : none
* Postconditions        : none
* @return               : void
*/
static void wait1(void)
{
	while (count_wait <= ZERO) {
		printf("");
	}
	count_wait--;
}

/**
* @fn                   : tc_net_select_server
* @brief                : server thread.
* @scenario             : server thread.
* API's covered         : socket,bind,listen,select,close
* Preconditions         : socket file descriptor.
* Postconditions        : none
* @return               : void
*/
static void tc_net_select_server(void)
{
	int newfd, yes = 1, addrlen, result, ret;
	fd_set master;
	fd_set read_fds;
	struct sockaddr_in sa;
	struct sockaddr_in ca;

	int sock_tcp = socket(AF_INET, SOCK_STREAM, 0);
	TC_ASSERT_NEQ("socket", sock_tcp, NEG_VAL);

	FD_ZERO(&master);
	ret = setsockopt(sock_tcp, SOL_SOCKET, 0, &yes, sizeof(int));
	TC_ASSERT_NEQ_CLEANUP("setsockopt", ret, NEG_VAL, close(sock_tcp));

	memset(&sa, 0, sizeof(sa));
	sa.sin_family = PF_INET;
	sa.sin_port = htons(PORTNUM);
	sa.sin_addr.s_addr = INADDR_LOOPBACK;

	ret = bind(sock_tcp, (struct sockaddr *)&sa, sizeof(sa));
	TC_ASSERT_NEQ("bind", ret, NEG_VAL);

	listen(sock_tcp, 2);
	newfd = accept(sock_tcp, (struct sockaddr *)&ca, (socklen_t *)&addrlen);
	close(sock_tcp);
	TC_ASSERT_GEQ("accept", ret, ZERO);

	FD_ZERO(&read_fds);
	FD_SET(newfd, &read_fds);

	result = select(newfd + 1, &read_fds, NULL, NULL, NULL);
	TC_ASSERT_NEQ_CLEANUP("select", result, NEG_VAL, close(newfd));
	close(newfd);
	TC_SUCCESS_RESULT();
}

/**
* @fn                   : tc_net_select_client
* @brief                : clinet thread.
* @scenario             : clinet thread.
* API's covered         : socket,connect,send,close
* Preconditions         : socket file descriptor.
* Postconditions        : none
* @return               : void
*/
static void tc_net_select_client(void)
{
	int ret;
	struct sockaddr_in dest;

	int sock_tcp = socket(AF_INET, SOCK_STREAM, 0);
	TC_ASSERT_NEQ("socket", sock_tcp, NEG_VAL);

	memset(&dest, 0, sizeof(dest));
	dest.sin_family = PF_INET;
	dest.sin_addr.s_addr = INADDR_LOOPBACK;
	dest.sin_port = htons(PORTNUM);

	wait1();
	ret = connect(sock_tcp, (struct sockaddr *)&dest, sizeof(struct sockaddr));
	TC_ASSERT_NEQ_CLEANUP("connect", ret, NEG_VAL, close(sock_tcp));
	close(sock_tcp);
	TC_SUCCESS_RESULT();
}

/**
* @fn                   : server
* @brief                : executing the server thread
* @scenario             : server program
* API's covered         : socket,bind,listen,select,close
* Preconditions         : socket file descriptor.
* Postconditions        : none
* @return               : void*
*/
static void* server(void *args)
{
	tc_net_select_server();
	return NULL;
}

/**
* @fn                   : client
* @brief                : executing the client thread
* @scenario             : clinet program
* API's covered         : socket,connect,send,close
* Preconditions         : socket file descriptor.
* Postconditions        : none
* @return               : void*
*/
static void* client(void *args)
{
	tc_net_select_client();
	return NULL;
}

/**
* @fn                   : tc_net_select
* @brief                : created client and server threads.
* @scenario             : create client and server thread to test select api.
* API's covered         : none
* Preconditions         : none
* Postconditions        : none
* @return               : void
*/
void net_select(void)
{
	pthread_t Server, Client;

	pthread_create(&Server, NULL, &server, NULL);
	pthread_create(&Client, NULL, &client, NULL);

	pthread_join(Server, NULL);
	pthread_join(Client, NULL);
}

/****************************************************************************
 * Name: select()
 ****************************************************************************/
int net_select_main(void)
{
	net_select();
	return 0;
}
