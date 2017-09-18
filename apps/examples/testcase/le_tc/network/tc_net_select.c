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
#include <sys/select.h>
#include <pthread.h>

#define PORTNUM 1110
#define MAXRCVLEN 20
int s = 0;
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
	while (s <= 0) {

		printf("");
	}
	s--;
}

/**
* @fn                   :server
* @brief                :
* @scenario             :
* API's covered         :socket,bind,listen,select,close
* Preconditions         :
* Postconditions        :
* @return               :void
*/
static void *server(void *args)
{
	fd_set master;
	fd_set read_fds;
	struct sockaddr_in sa;
	struct sockaddr_in ca;
	int newfd;
	int yes = 1;
	int addrlen;
	int result;
	int sock_tcp = socket(AF_INET, SOCK_STREAM, 0);

	FD_ZERO(&master);
	if (setsockopt(sock_tcp, SOL_SOCKET, 0, &yes, sizeof(int)) == -1) {
		perror("setsockopt error\n");
		return 0;
	}
	memset(&sa, 0, sizeof(sa));

	sa.sin_family = PF_INET;
	sa.sin_port = htons(PORTNUM);
	sa.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

	bind(sock_tcp, (struct sockaddr *)&sa, sizeof(sa));

	listen(sock_tcp, 2);
	newfd = accept(sock_tcp, (struct sockaddr *)&ca, (socklen_t *)&addrlen);
	do {
		FD_ZERO(&read_fds);
		FD_SET(newfd, &read_fds);
		result = select(newfd + 1, &read_fds, NULL, NULL, NULL);
	} while (result == -1);
	close(sock_tcp);
	return NULL;
}

/**
* @fn                   :client
* @brief                :
* @scenario             :
* API's covered         :socket,connect,send,close
* Preconditions         :
* Postconditions        :
* @return               :void
*/
static void *client(void *args)
{
	int len;
	struct sockaddr_in dest;
	char buf[10] = "samsung";

	int sock_tcp = socket(AF_INET, SOCK_STREAM, 0);
	memset(&dest, 0, sizeof(dest));
	dest.sin_family = PF_INET;
	dest.sin_addr.s_addr = inet_addr("127.0.0.1");
	dest.sin_port = htons(PORTNUM);

	wait1();
	connect(sock_tcp, (struct sockaddr *)&dest, sizeof(struct sockaddr));
	len = send(sock_tcp, buf, sizeof(buf), 0);
	close(sock_tcp);
	return NULL;
}

/****************************************************************************
 * Name: send()
 ****************************************************************************/
int net_select_main()
{

	pthread_t Server, Client;

	pthread_create(&Server, NULL, &server, NULL);
	pthread_create(&Client, NULL, &client, NULL);

	pthread_join(Server, NULL);
	pthread_join(Client, NULL);

	return 0;
}
