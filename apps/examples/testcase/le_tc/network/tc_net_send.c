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
#include <pthread.h>

#include "tc_internal.h"

#define PORTNUM		5007
#define MAXRCVLEN	20

static int count_wait;

/**
* @fn                   : sig_wait
* @brief                : Function to wait on semaphore
* @scenario             : use wait function to decrement count value.
* @API's covered        : none
* @Preconditions        : none
* @Postconditions       : none
* @return               : void
*/
void sig_wait(void)
{
	while (count_wait <= ZERO) {
		printf("");
	}
	count_wait--;
}

/**
* @fn                   : sig_call
* @brief                : Function to signal semaphore
* @scenario             : use to increase the count value.
* @API's covered        : none
* @Preconditions        : none
* @Postconditions       : none
* @return               : void
*/
void sig_call(void)
{
	count_wait++;
}

/**
* @testcase             : tc_net_send_p
* @brief                : This send API send a message on a socket.
* @scenario             : used to transmit a message to another socket.
* @apicovered           : accept(),send()
* @precondition         : socket file descriptor.
* @postcondition        : none
* @return               : void
*/
void tc_net_send_p(int fd)
{
	char *msg = "Hello World !\n";

	int ConnectFD = accept(fd, NULL, NULL);

	int ret = send(ConnectFD, msg, strlen(msg), ZERO);
	close(ConnectFD);
	TC_ASSERT_NEQ("send", ret, NEG_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @fn                   : server
* @brief                : Create a Tcp server.
* @scenario             : Create a tcp server for checking send api.
* API's covered         : socket,bind,listen,close
* Preconditions         : socket file descriptor.
* Postconditions        : none
* @return               : void*
*/
void *server(void *args)
{
	struct sockaddr_in sa;

	int sock = socket(AF_INET, SOCK_STREAM, ZERO);

	memset(&sa, 0, sizeof(sa));
	sa.sin_family = PF_INET;
	sa.sin_port = htons(PORTNUM);
	sa.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

	bind(sock, (struct sockaddr *)&sa, sizeof(sa));
	listen(sock, 2);

	sig_call();
	tc_net_send_p(sock);
	close(sock);
	return NULL;
}

/**
* @fn                   : client
* @brief                : This api create client.
* @scenario             : Create tcp client.
* API's covered         : socket,connect,recv,close
* Preconditions         : socket file descriptor.
* Postconditions        : none
* @return               : void*
*/
void *client(void *args)
{
	int len, ret;
	char buffer[MAXRCVLEN];
	struct sockaddr_in dest;

	int sock = socket(AF_INET, SOCK_STREAM, 0);

	memset(&dest, 0, sizeof(dest));
	dest.sin_family = PF_INET;
	dest.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	dest.sin_port = htons(PORTNUM);

	sig_wait();
	ret = connect(sock, (struct sockaddr *)&dest, sizeof(struct sockaddr));
	len = recv(sock, buffer, MAXRCVLEN, ZERO);
	buffer[len] = '\0';
	close(sock);
	return NULL;
}

/**
* @fn                  : net_send
* @brief               : This api create client and server thread.
* @scenario            : Create client and server thread to test send api.
* API's covered        : none
* Preconditions        : none
* Postconditions       : none
* @return              : void
*/
static void net_send(void)
{
	pthread_t Server, Client;

	pthread_create(&Server, NULL, server, NULL);
	pthread_create(&Client, NULL, client, NULL);

	pthread_join(Server, NULL);
	pthread_join(Client, NULL);
}

/****************************************************************************
 * Name: send()
 ****************************************************************************/
int net_send_main(void)
{
	net_send();
	return 0;
}
