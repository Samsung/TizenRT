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

// @file tc_net_sendto.c
// @brief Test Case Example for sendto() API
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

#define PORTNUM        7891
#define TCPPORT        7890
#define MAXRCVLEN      20
#define BACKLOG        2

static int count_wait;

void tc_net_sendto_tcp_n(int ConnectFD);
void tc_net_sendto_tcp_shutdown_n(int ConnectFD);

/**
* @testcase            :tc_net_sendto_p
* @brief               :send a message on a socket.
* @scenario            :sendto() function send a message through a connectionless-mode socket.
* @apicovered          :sendto
* @precondition        :socket file descriptor.
* @postcondition       :none
* @return              :void
*/
void tc_net_sendto_p(int fd)
{
	int ret;
	struct sockaddr_in dest;
	socklen_t fromlen;
	char *buffer = "hello";
	int len = strlen(buffer) + ONE;

	memset(&dest, 0, sizeof(dest));
	dest.sin_family = PF_INET;
	dest.sin_addr.s_addr = INADDR_LOOPBACK;
	dest.sin_port = htons(PORTNUM);
	fromlen = sizeof(dest);

	ret = sendto(fd, buffer, len, 0, (struct sockaddr *)&dest, fromlen);
	TC_ASSERT_NEQ("sendto", ret, NEG_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            :tc_net_sendto_n
* @brief               :send a message on a socket.
* @scenario            :sendto() function send a message through a connectionless-mode socket,
*                       test with invalid socket fd.
* @apicovered          :sendto
* @precondition        :none
* @postcondition       :none
* @return              :void
*/
void tc_net_sendto_n(void)
{
	int ret;
	struct sockaddr_in dest;
	socklen_t fromlen;
	char *buffer = "hello";
	int len = strlen(buffer) + ONE;

	memset(&dest, 0, sizeof(dest));
	dest.sin_family = PF_INET;
	dest.sin_addr.s_addr = INADDR_LOOPBACK;
	dest.sin_port = htons(PORTNUM);
	fromlen = sizeof(dest);

	ret = sendto(NEG_VAL, buffer, len, 0, (struct sockaddr *)&dest, fromlen);
	TC_ASSERT_EQ("sendto", ret, NEG_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            :tc_net_sendto_af_unix_n
* @brief               :send a message on a socket.
* @scenario            :sendto() function send a message through a connectionless-mode socket.
* @apicovered          :sendto
* @precondition        :socket file descriptor.
* @postcondition       :none
* @return              :void
*/
void tc_net_sendto_af_unix_n(int fd)
{
	int ret;
	struct sockaddr_in dest;
	socklen_t fromlen;
	char *buffer = "hello";
	int len = strlen(buffer) + ONE;

	memset(&dest, 0, sizeof(dest));
	dest.sin_family = AF_UNIX;
	dest.sin_addr.s_addr = INADDR_LOOPBACK;
	dest.sin_port = htons(PORTNUM);
	fromlen = sizeof(dest);

	ret = sendto(fd, buffer, len, 0, (struct sockaddr *)&dest, fromlen);
	TC_ASSERT_EQ("sendto", ret, NEG_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            :tc_net_sendto_shutdown_n
* @brief               :send a message on a socket.
* @scenario            :sendto() function send a message through a connectionless-mode socket,
*                       test after shutdown.
* @apicovered          :sendto(), shutdown()
* @precondition        :socket file descriptor.
* @postcondition       :none
* @return              :void
*/
void tc_net_sendto_shutdown_n(int fd)
{
	int ret;
	struct sockaddr_in dest;
	socklen_t fromlen;
	char *buffer = "hello";
	int len = strlen(buffer) + ONE;

	memset(&dest, 0, sizeof(dest));
	dest.sin_family = AF_UNIX;
	dest.sin_addr.s_addr = INADDR_LOOPBACK;
	dest.sin_port = htons(PORTNUM);
	fromlen = sizeof(dest);
	shutdown(fd, SHUT_WR);

	ret = sendto(fd, buffer, len, 0, (struct sockaddr *)&dest, fromlen);
	TC_ASSERT_EQ("sendto", ret, NEG_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @fn                   :sendto_udpserver
* @brief                :create a udp server.
* @scenario             :create a tcp server to test sendto api.
* @API's covered        :socket,bind,recvfrom
* @Preconditions        :socket file descriptor.
* @Postconditions       :none
* @return               :void*
*/
void* sendto_udpserver(void *args)
{
	struct sockaddr_in sa;
	char buffer[MAXRCVLEN];
	struct sockaddr_storage serverStorage;
	socklen_t addr_size;

	int sock = socket(AF_INET, SOCK_DGRAM, 0);

	memset(&sa, 0, sizeof(sa));
	sa.sin_family = PF_INET;
	sa.sin_port = htons(PORTNUM);
	sa.sin_addr.s_addr = INADDR_LOOPBACK;

	bind(sock, (struct sockaddr *)&sa, sizeof(sa));
	recvfrom(sock, buffer, MAXRCVLEN, 0, (struct sockaddr *)&serverStorage, &addr_size);
	close(sock);
	return NULL;
}

/**
* @fn                   :sendto_udpclient
* @brief                :create client.
* @scenario             :create tcp client.
* @API's covered        :socket
* @Preconditions        :none
* @Postconditions       :none
* @return               :void*
*/
void* sendto_udpclient(void *args)
{
	int sock = socket(AF_INET, SOCK_DGRAM, 0);

	tc_net_sendto_p(sock);
	tc_net_sendto_n();
	tc_net_sendto_af_unix_n(sock);
	tc_net_sendto_shutdown_n(sock);
	close(sock);
	return NULL;
}

/**
* @fn                   :sendto_wait
* @brief                :function to wait on semaphore
* @scenario             :used to lock the resource
* @API's covered        :none
* @Preconditions        :none
* @Postconditions       :none
* @return               :void
*/
void sendto_wait(void)
{
	while (count_wait <= ZERO) {
		printf("");
	}
	count_wait--;
}

/**
* @fn                   :sendto_signal
* @brief                :function to signal semaphore
* @scenario             :used to release the resource
* @API's covered        :none
* @Preconditions        :none
* @Postconditions       :none
* @return               :void
*/
void sendto_signal(void)
{
	count_wait++;
}

/**
* @testcase             :tc_net_sendto_tcp_p
* @brief                :send a message on a socket.
* @scenario             :sendto() function send a message through a connection-mode socket.
* @apicovered           :accept(), sendto()
* @precondition         :socket file descriptor.
* @postcondition        :none
* @return               :void
*/
void tc_net_sendto_tcp_p(int fd)
{
	int ret;
	int ConnectFD;
	char *msg = "Hello World !\n";
	socklen_t fromlen = ZERO;

	ConnectFD = accept(fd, NULL, NULL);
	TC_ASSERT_NEQ("accept", ConnectFD, ZERO);

	ret = sendto(ConnectFD, msg, strlen(msg), 0, NULL, fromlen);
	TC_ASSERT_NEQ("sendto", ret, NEG_VAL);
	TC_SUCCESS_RESULT();

	tc_net_sendto_tcp_n(ConnectFD);
	tc_net_sendto_tcp_shutdown_n(ConnectFD);
	close(ConnectFD);
}

/**
* @testcase             :tc_net_sendto_tcp_n
* @brief                :send a message on a socket.
* @scenario             :sendto() function send a message through a connection-mode socket,
*                         test with invalid socket fd.
* @apicovered           :sendto(), close()
* @precondition         :none
* @postcondition        :none
* @return               :void
*/
void tc_net_sendto_tcp_n(int ConnectFD)
{
	int ret;
	char *msg = "Hello World !\n";
	socklen_t fromlen = ZERO;

	ret = sendto(NEG_VAL, msg, strlen(msg), 0, NULL, fromlen);
	TC_ASSERT_EQ("sendto", ret, NEG_VAL);
	TC_SUCCESS_RESULT();
	close(ConnectFD);
}

/**
* @testcase             :tc_net_sendto_tcp_shutdown_n
* @brief                :send a message on a socket.
* @scenario             :sendto() function send a message through a connection-mode socket,
                          test after shutdown.
* @apicovered           :sendto(), close()
* @precondition         :socket file descriptor.
* @postcondition        :none
* @return               :void
*/
void tc_net_sendto_tcp_shutdown_n(int ConnectFD)
{
	int ret;
	char *msg = "Hello World !\n";
	socklen_t fromlen = ZERO;

	ret = shutdown(ConnectFD, SHUT_WR);
	TC_ASSERT_NEQ("shutdown", ret, ZERO);

	ret = sendto(ConnectFD, msg, strlen(msg), 0, NULL, fromlen);
	TC_ASSERT_EQ("sendto", ret, NEG_VAL);
	TC_SUCCESS_RESULT();
	close(ConnectFD);
}

/**
* @fn                   :sendto_tcpserver
* @brief                :create a tcp server.
* @scenario             :create a tcp server to test sendto api.
* @API's covered        :socket,bind,listen,close
* @Preconditions        :socket file descriptor.
* @Postconditions       :none
* @return               :void*
*/
void* sendto_tcpserver(void *args)
{
	struct sockaddr_in sa;

	int sock = socket(AF_INET, SOCK_STREAM, 0);

	memset(&sa, 0, sizeof(sa));
	sa.sin_family = PF_INET;
	sa.sin_port = htons(TCPPORT);
	sa.sin_addr.s_addr = INADDR_LOOPBACK;

	bind(sock, (struct sockaddr *)&sa, sizeof(sa));
	listen(sock, BACKLOG);
	sendto_signal();
	tc_net_sendto_tcp_p(sock);
	close(sock);
	return NULL;
}

/**
* @fn                   :sendto_tcpclient
* @brief                :create client.
* @scenario             :create tcp client.
* @API's covered        :socket,connect,recvfrom,close
* @Preconditions        :socket file descriptor.
* @Postconditions       :none
* @return               :void*
*/
void* sendto_tcpclient(void *args)
{
	int len;
	char buffer[MAXRCVLEN];
	struct sockaddr_in dest;

	int sock = socket(AF_INET, SOCK_STREAM, 0);

	memset(&dest, 0, sizeof(dest));
	dest.sin_family = PF_INET;
	dest.sin_addr.s_addr = INADDR_LOOPBACK;
	dest.sin_port = htons(TCPPORT);

	sendto_wait();
	connect(sock, (struct sockaddr *)&dest, sizeof(struct sockaddr));

	while ((len = recvfrom(sock, buffer, MAXRCVLEN, 0, NULL, NULL)) > 0) {
		buffer[len] = '\0';
	}
	close(sock);
	return NULL;
}

/**
* @fn                  :net_sendto
* @brief               :create client and server thread.
* @scenario            :create client and server thread to test sendto api.
* @API's covered       :none
* @Preconditions       :none
* @Postconditions      :none
* @return              :void
*/
void net_sendto(void)
{
	pthread_t Server;
	pthread_t Client;
	pthread_t tcpserver;
	pthread_t tcpclient;

	pthread_create(&Server, NULL, sendto_udpserver, NULL);
	pthread_create(&Client, NULL, sendto_udpclient, NULL);

	pthread_create(&tcpserver, NULL, sendto_tcpserver, NULL);
	pthread_create(&tcpclient, NULL, sendto_tcpclient, NULL);

	pthread_join(Server, NULL);
	pthread_join(Client, NULL);

	pthread_join(tcpserver, NULL);
	pthread_join(tcpclient, NULL);
}

/****************************************************************************
 * Name:net_sendto_main
 ****************************************************************************/

int net_sendto_main(void)
{
	net_sendto();
	return 0;
}
