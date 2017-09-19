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
static int count_wait = 0;
/**
* @fn                    : wait
* @brief                 : function to wait on semaphore
* @scenario              :
* @API's covered         :
* @Preconditions         :
* @Postconditions        :
* @return                : void
*/
static void wait(void)
{
	while (count_wait <= 0) {
		printf("");
	}
	count_wait--;
}

/**
* @fn                    : nw_signal
* @brief                 : function to signal semaphore
* @scenario              : none
* @API's covered         : none
* @Preconditions         : none
* @Postconditions        : none
* @return                : void
*/
static void nw_signal(void)
{
	count_wait++;
}

/**
* @testcase             : tc_net_accept_p
* @brief                : for accepting socket connection
* @scenario             : creates a new connected socket, and returns a new file
                          descriptor referring to that socket
* @apicovered           : accept()
* @precondition         : create a socket and need sock fd
* @postcondition        : void
*/
void tc_net_accept_p(int fd)
{
	int ConnectFD = accept(fd, NULL, NULL);

	close(ConnectFD);
	TC_ASSERT_GEQ("accept", ConnectFD, 0);
	TC_SUCCESS_RESULT();
}

/**
* @testcase             : tc_net_accept_socket_n
* @brief                : for accepting socket connection
* @scenario             : Take an invalid sock fd and return -1 on error
* @apicovered           : accept()
* @precondition         : none
* @postcondition        : void
*/
void tc_net_accept_socket_n(void)
{
	int ConnectFD = accept(NEG_VAL, NULL, NULL);
	close(ConnectFD);
	TC_ASSERT_NEQ("accept", ConnectFD, ZERO);
	TC_SUCCESS_RESULT();
}

/**
* @fn                   : Server
* @brief                : Create a Tcp server
* @scenario             : create a tcp server for checking accept
* @API's covered        : socket,bind,listen,close
* @Preconditions        : none
* @Postconditions       : none
* @return               : void *
*/
void *Server(void *args)
{
	struct sockaddr_in sa;

	int sock = socket(AF_INET, SOCK_STREAM, 0);
	memset(&sa, 0, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(PORTNUM);
	sa.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

	bind(sock, (struct sockaddr *)&sa, sizeof(sa));
	listen(sock, 1);
	nw_signal();
	tc_net_accept_p(sock);
	tc_net_accept_socket_n();
	close(sock);
	return NULL;
}

/**
* @fn                   : Client
* @brief                : This api create client
* @scenario             : Create tcp client
* API's covered         : socket,connect,close
* Preconditions         : none
* Postconditions        : none
* @return               : void *
*/
void *Client(void *args)
{
	struct sockaddr_in dest;

	int sock = socket(AF_INET, SOCK_STREAM, 0);

	memset(&dest, 0, sizeof(dest));
	dest.sin_family = AF_INET;
	dest.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	dest.sin_port = htons(PORTNUM);

	wait();
	connect(sock, (struct sockaddr *)&dest, sizeof(struct sockaddr));
	close(sock);
	TC_SUCCESS_RESULT();
	return NULL;
}

/**
* @fn                   : tc_net_accept
* @brief                : This api create client and server thread
* @scenario             : test accept, create client server
* API's covered         : socket,connect,close
* Preconditions         : none
* Postconditions        : none
* @return               : int
*/
void tc_net_accept(void)
{
	pthread_t server, client;

	pthread_create(&server, NULL, Server, NULL);
	pthread_create(&client, NULL, Client, NULL);

	pthread_join(server, NULL);
	pthread_join(client, NULL);
}

/****************************************************************************
 * Name: accept()
 ****************************************************************************/
int net_accept_main(void)
{
	tc_net_accept();
	return 0;
}
