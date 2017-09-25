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

// @file tc_net_shutdown.c
// @brief Test Case Example for shutdown() API
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

#define PORTNUM        5010
#define MAXRCVLEN      20
#define BACKLOG        1

static int count_wait;

/**
* @fn                   : shutdown_wait
* @brief                : Function to wait on semaphore.
* @scenario             : use wait function to decrement count value.
* @API's covered        : none
* @Preconditions        : none
* @Postconditions       : none
* @return               : void
*/
void shutdown_wait(void)
{
	while (count_wait <= ZERO) {

		printf("");
	}
	count_wait--;
}

/**
* @fn                  : shutdown_signal
* @brief               : Function to signal semaphore.
* @scenario            : use to increase the count value.
* @API's covered       : none
* @Preconditions       : none
* @Postconditions      : none
* @return              : void
*/
void shutdown_signal(void)
{
	count_wait++;
}

/**
* @fn                  : tc_net_shutdown_recv_p
* @brief               : Close one end of a full-duplex connection.
* @scenario            : Close one end of a full-duplex connection with disables further receive operations.
* @API's covered       : shutdown()
* @Preconditions       : socket file descriptor.
* @Postconditions      : none
* @return              : void
*/

void tc_net_shutdown_recv_p(int fd)
{
	int ret = shutdown(fd, SHUT_RD);

	TC_ASSERT_NEQ("shutdown", ret, NEG_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_shutdown_send_p
* @brief               : Close one end of a full-duplex connection.
* @scenario            : Close one end of a full-duplex connection with disables further send operations.
* @apicovered          : shutdown()
* @precondition        : socket file descriptor.
* @postcondition       : none
* @return              : void
*/
void tc_net_shutdown_send_p(int fd)
{
	int ret = shutdown(fd, SHUT_WR);

	TC_ASSERT_NEQ("shutdown", ret, NEG_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_shutdown_sendrecv_p
* @brief               : Close one end of a full-duplex connection.
* @scenario            : Close one end of a full-duplex connection with disables further send and receive operations.
* @apicovered          : shutdown()
* @precondition        : socket file descriptor.
* @postcondition       : none
* @return              : void
*/
void tc_net_shutdown_sendrecv_p(int fd)
{
	int ret = shutdown(fd, SHUT_RDWR);

	TC_ASSERT_NEQ("shutdown", ret, NEG_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_net_shutdown_n
* @brief               : Close one end of a full-duplex connection.
* @scenario            : Close one end of a full-duplex connection with invalid socket fd.
* @apicovered          : shutdown()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
void tc_net_shutdown_n(void)
{
	int ret = shutdown(NEG_VAL, SHUT_RDWR);

	TC_ASSERT_EQ("shutdown", ret, NEG_VAL);
	TC_SUCCESS_RESULT();

}

/**
* @testcase            : tc_net_shutdown_sock_n
* @brief               : Close one end of a full-duplex connection.
* @scenario            : Close one end of a full-duplex connection with given socket fd.
* @apicovered          : shutdown()
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
void tc_net_shutdown_sock_n(int fd)
{
	int ret = shutdown(fd, SHUT_RD);

	TC_ASSERT_EQ("shutdown", ret, NEG_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @fn                   :net_shutdown_server
* @brief                :create a tcp server.
* @scenario             :create a tcp server to test shutdown api.
* @API's covered        :socket,bind,listen,send,accept,close
* @Preconditions        :socket file descriptor.
* @Postconditions       :none
* @return               :void
*/
static void net_shutdown_server(void)
{
	int ConnectFD;
	int ret;
	char *msg = "Hello World !\n";
	char buf[MAXRCVLEN];
	struct sockaddr_in sa;

	int sock = socket(AF_INET, SOCK_STREAM, 0);
	TC_ASSERT_NEQ("socket", sock, NEG_VAL);
	memset(&sa, 0, sizeof(sa));
	sa.sin_family = PF_INET;
	sa.sin_port = htons(PORTNUM);
	sa.sin_addr.s_addr = INADDR_LOOPBACK;

	ret = bind(sock, (struct sockaddr *)&sa, sizeof(sa));
	TC_ASSERT_NEQ_CLEANUP("bind", ret, NEG_VAL, close(sock));
	ret = listen(sock, BACKLOG);
	TC_ASSERT_NEQ_CLEANUP("listen", ret, NEG_VAL, close(sock));
	shutdown_signal();

	ConnectFD = accept(sock, NULL, NULL);
	TC_ASSERT_NEQ_CLEANUP("accept", ConnectFD, NEG_VAL, close(sock));
	tc_net_shutdown_send_p(ConnectFD);

	send(ConnectFD, msg, sizeof(msg), 0);
	tc_net_shutdown_recv_p(ConnectFD);

	recv(ConnectFD, buf, sizeof(buf), 0);
	tc_net_shutdown_n();

	tc_net_shutdown_sock_n(sock);
	close(ConnectFD);
	close(sock);
}

/**
* @fn                   :net_shutdown_client
* @brief                :create client.
* @scenario             :create tcp client.
* @API's covered        :socket,connect,recvfrom,close
* @Preconditions        :socket file descriptor.
* @Postconditions       :none
* @return               :void
*/
static void net_shutdown_client(void)
{
	int len;
	char buffer[MAXRCVLEN];
	struct sockaddr_in dest;

	int sock = socket(AF_INET, SOCK_STREAM, 0);
	TC_ASSERT_NEQ("socket", sock, NEG_VAL);

	memset(&dest, 0, sizeof(dest));
	dest.sin_family = PF_INET;
	dest.sin_addr.s_addr = INADDR_LOOPBACK;
	dest.sin_port = htons(PORTNUM);

	shutdown_wait();
	int ret = connect(sock, (struct sockaddr *)&dest, sizeof(struct sockaddr));
	TC_ASSERT_NEQ_CLEANUP("connect", ret, NEG_VAL, close(sock));

	len = recv(sock, buffer, MAXRCVLEN, 0);
	buffer[len] = '\0';
	tc_net_shutdown_recv_p(sock);

	recv(sock, buffer, MAXRCVLEN, 0);
	tc_net_shutdown_sendrecv_p(sock);

	recv(sock, buffer, MAXRCVLEN, 0);
	tc_net_shutdown_sendrecv_p(sock);

	send(sock, buffer, sizeof(buffer), 0);
	tc_net_shutdown_sendrecv_p(sock);

	tc_net_shutdown_n();
	close(sock);
}

/**
* @fn                   :shutdown_server
* @brief                :create a tcp server.
* @scenario             :create a tcp server to test shutdown api.
* @API's covered        :none.
* @Preconditions        :none.
* @Postconditions       :none
* @return               :void*
*/
void* shutdown_server(void *args)
{
	net_shutdown_server();
	return NULL;
}

/**
* @fn                   :shutdown_client
* @brief                :create client.
* @scenario             :create tcp client.
* @API's covered        :none.
* @Preconditions        :none.
* @Postconditions       :none
* @return               :void*
*/
void* shutdown_client(void *args)
{
	net_shutdown_client();
	return NULL;
}

/**
* @fn                   : net_shutdown
* @brief                : This api create client and server thread.
* @scenario             : Create client and server thread to test shutdown api.
* API's covered         : none
* Preconditions         : none
* Postconditions        : none
* @return               : void
*/
void net_shutdown(void)
{
	pthread_t Server, Client;

	pthread_create(&Server, NULL, shutdown_server, NULL);
	pthread_create(&Client, NULL, shutdown_client, NULL);

	pthread_join(Server, NULL);
	pthread_join(Client, NULL);
}

/****************************************************************************
 * Name: shutdown()
 ****************************************************************************/
int net_shutdown_main(void)
{
	net_shutdown();
	return 0;
}
