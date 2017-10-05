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

#define PORTNUM 7891
#define TCPPORT 7890
#define MAXRCVLEN 20
int sp = 0;
void tc_net_sendto_tcp_n(int ConnectFD);
void tc_net_sendto_tcp_shutdown_n(int ConnectFD);

/**
   * @testcase		   :tc_net_sendto_p
   * @brief		   :positive testcase for sendto api using udp
   * @scenario		   :
   * @apicovered	   :sendto()
   * @precondition	   :
   * @postcondition	   :
   */
void tc_net_sendto_p(int fd)
{
	char *buffer = "hello";
	int len = strlen(buffer) + 1;
	struct sockaddr_in dest;
	socklen_t fromlen;
	memset(&dest, 0, sizeof(dest));
	dest.sin_family = PF_INET;
	dest.sin_addr.s_addr = inet_addr("127.0.0.1");
	dest.sin_port = htons(PORTNUM);
	fromlen = sizeof(dest);
	int ret = sendto(fd, buffer, len, 0, (struct sockaddr *)&dest, fromlen);

	TC_ASSERT_NEQ("sendto", ret, -1);
	TC_SUCCESS_RESULT();

}

/**
   * @testcase		   :tc_net_sendto_n
   * @brief		   :negative testcase for sendto api using udp
   * @scenario		   :
   * @apicovered	   :sendto()
   * @precondition	   :
   * @postcondition	   :
   */
void tc_net_sendto_n(void)
{
	char *buffer = "hello";
	int len = strlen(buffer) + 1;
	struct sockaddr_in dest;
	socklen_t fromlen;
	memset(&dest, 0, sizeof(dest));
	dest.sin_family = PF_INET;
	dest.sin_addr.s_addr = inet_addr("127.0.0.1");
	dest.sin_port = htons(PORTNUM);
	fromlen = sizeof(dest);
	int ret = sendto(-1, buffer, len, 0, (struct sockaddr *)&dest, fromlen);

	TC_ASSERT_EQ("sendto", ret, -1);
	TC_SUCCESS_RESULT();

}

/**
   * @testcase		   :tc_net_sendto_af_unix_n
   * @brief		   :negative testcase for sendto api using udp
   * @scenario		   :
   * @apicovered	   :sendto()
   * @precondition	   :
   * @postcondition	   :
   */
void tc_net_sendto_af_unix_n(int fd)
{
	char *buffer = "hello";
	int len = strlen(buffer) + 1;
	struct sockaddr_in dest;
	socklen_t fromlen;
	memset(&dest, 0, sizeof(dest));
	dest.sin_family = AF_UNIX;
	dest.sin_addr.s_addr = inet_addr("127.0.0.1");
	dest.sin_port = htons(PORTNUM);
	fromlen = sizeof(dest);
	int ret = sendto(fd, buffer, len, 0, (struct sockaddr *)&dest, fromlen);

	TC_ASSERT_EQ("sendto", ret, -1);
	TC_SUCCESS_RESULT();

}

/**
   * @testcase		   :tc_net_sendto_shutdown_n
   * @brief		   :negative testcase for sendto api using udp
   * @scenario		   :
   * @apicovered	   :sendto()
   * @precondition	   :
   * @postcondition	   :
   */
void tc_net_sendto_shutdown_n(int fd)
{
	char *buffer = "hello";
	int len = strlen(buffer) + 1;
	struct sockaddr_in dest;
	socklen_t fromlen;
	memset(&dest, 0, sizeof(dest));
	dest.sin_family = AF_UNIX;
	dest.sin_addr.s_addr = inet_addr("127.0.0.1");
	dest.sin_port = htons(PORTNUM);
	fromlen = sizeof(dest);
	shutdown(fd, SHUT_WR);
	int ret = sendto(fd, buffer, len, 0, (struct sockaddr *)&dest, fromlen);

	TC_ASSERT_EQ("sendto", ret, -1);
	TC_SUCCESS_RESULT();

}

/**
   * @fn                   :sendto_server
   * @brief                :udp server
   * @scenario             :
   * API's covered         :socket,bind,recvfrom
   * Preconditions         :
   * Postconditions        :
   * @return               :void *
   */

void *sendto_udpserver(void *args)
{
	struct sockaddr_in sa;
	int SocketFD = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	char buffer[MAXRCVLEN];
	memset(&sa, 0, sizeof(sa));

	sa.sin_family = PF_INET;
	sa.sin_port = htons(PORTNUM);
	sa.sin_addr.s_addr = inet_addr("127.0.0.1");

	bind(SocketFD, (struct sockaddr *)&sa, sizeof(sa));
	struct sockaddr_storage serverStorage;
	socklen_t addr_size;

	recvfrom(SocketFD, buffer, MAXRCVLEN, 0, (struct sockaddr *)&serverStorage, &addr_size);

	close(SocketFD);

	return 0;

}

/**
   * @fn                   :sendto_client
   * @brief                :udp client
   * @scenario             :
   * API's covered         :socket
   * Preconditions         :
   * Postconditions        :
   * @return               :void *
   */
void *sendto_udpclient(void *args)
{
	int mysocket;
	mysocket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	tc_net_sendto_p(mysocket);
	tc_net_sendto_n();
	tc_net_sendto_af_unix_n(mysocket);
	tc_net_sendto_shutdown_n(mysocket);
	close(mysocket);

	return 0;

}

/**
   * @fn                   :sendto_wait
   * @brief                :function to wait on semaphore
   * @scenario             :
   * API's covered         :
   * Preconditions         :
   * Postconditions        :
   * @return               :void
   */
void sendto_wait(void)
{
	while (sp <= 0) {

		printf("");
	}
	sp--;
}

/**
   * @fn                   :sendto_signal
   * @brief                :function to signal semaphore
   * @scenario             :
   * API's covered         :
   * Preconditions         :
   * Postconditions        :
   * @return               :void
   */
void sendto_signal(void)
{
	sp++;
}

/**
   * @testcase		   :tc_net_sendto_tcp_p
   * @brief		   :positive testcase for sendto api using tcp
   * @scenario		   :
   * @apicovered	   :accept(), sendto()
   * @precondition	   :
   * @postcondition	   :
   */
void tc_net_sendto_tcp_p(int fd)
{

	char *msg = "Hello World !\n";
	socklen_t fromlen = 0;
	int ConnectFD = accept(fd, NULL, NULL);
	int ret = sendto(ConnectFD, msg, strlen(msg), 0, NULL, fromlen);

	TC_ASSERT_NEQ("sendto", ret, -1);
	TC_SUCCESS_RESULT();

	tc_net_sendto_tcp_n(ConnectFD);
	tc_net_sendto_tcp_shutdown_n(ConnectFD);

}

/**
   * @testcase		   :tc_net_sendto_tcp_n
   * @brief		   :negative testcase for sendto api using tcp
   * @scenario		   :
   * @apicovered	   :sendto(), close()
   * @precondition	   :
   * @postcondition	   :
   */
void tc_net_sendto_tcp_n(int ConnectFD)
{

	char *msg = "Hello World !\n";
	socklen_t fromlen = 0;

	int ret = sendto(-1, msg, strlen(msg), 0, NULL, fromlen);

	TC_ASSERT_EQ("sendto", ret, -1);
	TC_SUCCESS_RESULT();

	close(ConnectFD);

}

/**
   * @testcase		   :tc_net_sendto_tcp_shutdown_n
   * @brief		   :negative testcase for sendto api using tcp
   * @scenario		   :
   * @apicovered	   :sendto(), close()
   * @precondition	   :
   * @postcondition	   :
   */
void tc_net_sendto_tcp_shutdown_n(int ConnectFD)
{

	char *msg = "Hello World !\n";
	socklen_t fromlen = 0;
	shutdown(ConnectFD, SHUT_WR);
	int ret = sendto(ConnectFD, msg, strlen(msg), 0, NULL, fromlen);

	TC_ASSERT_EQ("sendto", ret, -1);
	TC_SUCCESS_RESULT();

	close(ConnectFD);

}

/**
   * @fn                   :sendto_server
   * @brief                :tcp server
   * @scenario             :
   * API's covered         :socket,bind,listen,close
   * Preconditions         :
   * Postconditions        :
   * @return               :void *
   */

void *sendto_tcpserver(void *args)
{

	struct sockaddr_in sa;
	int SocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	memset(&sa, 0, sizeof(sa));

	sa.sin_family = PF_INET;
	sa.sin_port = htons(TCPPORT);
	sa.sin_addr.s_addr = inet_addr("127.0.0.1");

	bind(SocketFD, (struct sockaddr *)&sa, sizeof(sa));

	listen(SocketFD, 2);

	sendto_signal();
	tc_net_sendto_tcp_p(SocketFD);

	close(SocketFD);
	pthread_exit(NULL);

}

/**
   * @fn                   :sendto_client
   * @brief                :tcp client
   * @scenario             :
   * API's covered         :socket,connect,recvfrom,close
   * Preconditions         :
   * Postconditions        :
   * @return               :void *
   */
void *sendto_tcpclient(void *args)
{

	char buffer[MAXRCVLEN];
	int len, mysocket;
	struct sockaddr_in dest;

	mysocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	memset(&dest, 0, sizeof(dest));
	dest.sin_family = PF_INET;
	dest.sin_addr.s_addr = inet_addr("127.0.0.1");
	dest.sin_port = htons(TCPPORT);

	sendto_wait();

	connect(mysocket, (struct sockaddr *)&dest, sizeof(struct sockaddr));
	while ((len = recvfrom(mysocket, buffer, MAXRCVLEN, 0, NULL, NULL)) > 0) {
		buffer[len] = '\0';
	}

	close(mysocket);
	pthread_exit(NULL);

}

/****************************************************************************
 * Name: sendto()
 ****************************************************************************/

int net_sendto_main(void)
{

	pthread_t Server, Client, tcpserver, tcpclient;

	pthread_create(&Server, NULL, sendto_udpserver, NULL);
	pthread_create(&Client, NULL, sendto_udpclient, NULL);
	pthread_create(&tcpserver, NULL, sendto_tcpserver, NULL);
	pthread_create(&tcpclient, NULL, sendto_tcpclient, NULL);

	pthread_join(Server, NULL);

	pthread_join(Client, NULL);
	pthread_join(tcpserver, NULL);

	pthread_join(tcpclient, NULL);
	return 0;
}
