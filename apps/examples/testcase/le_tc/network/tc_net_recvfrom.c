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

// @file tc_net_recvfrom.c
// @brief Test Case Example for recvfrom() API
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

#define PORTNUM 1111
#define MAXRCVLEN 20
int mutex1 = 0;

/**
   * @testcase		   :tc_net_recvfrom_p
   * @brief		   :positive testcase using udp
   * @scenario		   :
   * @apicovered	   :recvfrom()
   * @precondition	   :
   * @postcondition	   :
   */
void tc_net_recvfrom_p(int fd)
{
	char buffer[MAXRCVLEN];
	struct sockaddr_storage serverStorage;
	socklen_t addr_size;
	int ret = recvfrom(fd, buffer, MAXRCVLEN, 0, (struct sockaddr *)&serverStorage, &addr_size);

	TC_ASSERT_NEQ("recvfrom", ret, -1);
	TC_SUCCESS_RESULT();

}

/**
   * @testcase		   :tc_net_recvfrom_n
   * @brief		   :negative testcase using udp
   * @scenario		   :
   * @apicovered	   :recvfrom()
   * @precondition	   :
   * @postcondition	   :
   */
void tc_net_recvfrom_n(int fd)
{
	char buffer[MAXRCVLEN];
	struct sockaddr_storage serverStorage;
	socklen_t addr_size;
	int ret = recvfrom(-1, buffer, MAXRCVLEN, 0, (struct sockaddr *)&serverStorage, &addr_size);

	TC_ASSERT_EQ("recvfrom", ret, -1);
	TC_SUCCESS_RESULT();

}

/**
   * @fn                   :recvfrom_server
   * @brief                :udp server
   * @scenario             :
   * API's covered         :socket,bind
   * Preconditions         :
   * Postconditions        :
   * @return               :void *
   */
void *recvfrom_udpserver(void *args)
{
	struct sockaddr_in sa;
	int socket_fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (socket_fd < 0) {
		printf("socket fail %s :%d\n", __FUNCTION__, __LINE__);
		return 0;
	}

	memset(&sa, 0, sizeof(sa));

	sa.sin_family = PF_INET;
	sa.sin_port = htons(PORTNUM);
	sa.sin_addr.s_addr = inet_addr("127.0.0.1");

	int ret = bind(socket_fd, (struct sockaddr *)&sa, sizeof(sa));
	if (ret < 0) {
		printf("bind fail %s:%d", __FUNCTION__, __LINE__);
		close(socket_fd);
		return 0;
	}

	tc_net_recvfrom_p(socket_fd);
	tc_net_recvfrom_n(socket_fd);
	close(socket_fd);

	return 0;

}

/**
   * @fn                   :recvfrom_client
   * @brief                :udp client
   * @scenario             :
   * API's covered         :socket,sendto
   * Preconditions         :
   * Postconditions        :
   * @return               :void *
   */
void *recvfrom_udpclient(void *args)
{
	int mysocket;
	mysocket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (mysocket < 0) {
		printf("socket creation error (%s) line:%d\n", __FUNCTION__, __LINE__);
		return 0;
	}

	char *buffer = "hello";
	int len = strlen(buffer) + 1;
	struct sockaddr_in dest;
	socklen_t fromlen;
	memset(&dest, 0, sizeof(dest));
	dest.sin_family = PF_INET;
	dest.sin_addr.s_addr = inet_addr("127.0.0.1");
	dest.sin_port = htons(PORTNUM);
	fromlen = sizeof(dest);
	int ret = sendto(mysocket, buffer, len, 0, (struct sockaddr *)&dest, fromlen);
	if (ret < 0) {
		printf("sendto fail %s:%d\n", __FUNCTION__, __LINE__);
	}
	close(mysocket);

	return 0;
}

/**
   * @fn                   :recvfrom_wait
   * @brief                :function to wait on semaphore
   * @scenario             :
   * API's covered         :
   * Preconditions         :
   * Postconditions        :
   * @return               :void
   */
void recvfrom_wait(void)
{
	while (mutex1 <= 0) {

		printf("");
	}
	mutex1--;
}

/**
   * @fn                   :recvfrom_signal
   * @brief                :function to signal semaphore
   * @scenario             :
   * API's covered         :
   * Preconditions         :
   * Postconditions        :
   * @return               :void
   */
void recvfrom_signal(void)
{
	mutex1++;
}

/**
   * @testcase		   :tc_net_recvfrom_tcp_p
   * @brief		   :positive testcase for recvfrom api
   * @scenario		   :
   * @apicovered	   :recvfrom()
   * @precondition	   :
   * @postcondition	   :
   */
void tc_net_recvfrom_tcp_p(int fd)
{
	char buffer[MAXRCVLEN];

	int ret = recvfrom(fd, buffer, MAXRCVLEN, 0, NULL, NULL);

	TC_ASSERT_NEQ("recvfrom", ret, -1);
	TC_SUCCESS_RESULT();

}

/**
   * @testcase		   :tc_net_recvfrom_tcp_conn_n
   * @brief		   :negative testcase for recvfrom api
   * @scenario		   :
   * @apicovered	   :recvfrom()
   * @precondition	   :
   * @postcondition	   :
   */
void tc_net_recvfrom_tcp_conn_n(int fd)
{
	char buffer[MAXRCVLEN];

	shutdown(fd, SHUT_RD);
	int ret = recvfrom(fd, buffer, MAXRCVLEN, 0, NULL, NULL);

	TC_ASSERT_EQ("recvfrom", ret, 0);
	TC_SUCCESS_RESULT();

}

/**
   * @testcase		   :tc_net_recvfrom_tcp_sock_n
   * @brief		   :negative testcase for recvfrom api
   * @scenario		   :
   * @apicovered	   :recvfrom()
   * @precondition	   :
   * @postcondition	   :
   */
void tc_net_recvfrom_tcp_sock_n(int fd)
{
	char buffer[MAXRCVLEN];

	int ret = recvfrom(fd, buffer, MAXRCVLEN, 0, NULL, NULL);

	TC_ASSERT_EQ("recvfrom", ret, -1);
	TC_SUCCESS_RESULT();

}

/**
   * @testcase		   :tc_net_recvfrom_tcp_n
   * @brief		   :negative testcase using tcp
   * @scenario		   :
   * @apicovered	   :recvfrom()
   * @precondition	   :
   * @postcondition	   :
   */
void tc_net_recvfrom_tcp_n(void)
{
	char buffer[MAXRCVLEN];

	int ret = recvfrom(-1, buffer, MAXRCVLEN, 0, NULL, NULL);

	TC_ASSERT_EQ("recvfrom", ret, -1);
	TC_SUCCESS_RESULT();

}

/**
   * @fn                   :recvfrom_server
   * @brief                :
   * @scenario             :
   * API's covered         :socket,bind,listen,accept,sendto,close
   * Preconditions         :
   * Postconditions        :
   * @return               :void *
   */
void *recvfrom_tcpserver(void *args)
{

	char *msg = "Hello World !\n";
	struct sockaddr_in sa;
	int socket_fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (socket_fd < 0) {
		printf("Socket creation fail %s:%d\n", __FUNCTION__,  __LINE__);
		return 0;
	}

	if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &(int){ 1 }, sizeof(int)) < 0) {
		printf("setsockopt(SO_REUSEADDR) failed %s:%d:%d\n", __FUNCTION__, __LINE__, errno);
		close(socket_fd);
		return 0;
	}

	memset(&sa, 0, sizeof(sa));

	sa.sin_family = PF_INET;
	sa.sin_port = htons(PORTNUM);
	sa.sin_addr.s_addr = inet_addr("127.0.0.1");

	int ret = bind(socket_fd, (struct sockaddr *)&sa, sizeof(sa));
	if (ret < 0) {
		close(socket_fd);
		printf("fail %s:%d\n", __FUNCTION__, __LINE__);
		return 0;
	}

	ret = listen(socket_fd, 1);
	if (ret < 0) {
		close(socket_fd);
		printf("fail %s:%d\n", __FUNCTION__, __LINE__);
		return 0;
	}

	recvfrom_signal();
	int connect_fd = accept(socket_fd, NULL, NULL);
	if (connect_fd < 0) {
		printf("accept error (%s) line:%d\n", __FUNCTION__, __LINE__);
		close(socket_fd);
		return 0;
	}

	int i;
	for (i = 0; i < 4; i++) {
		ret = sendto(connect_fd, msg, strlen(msg), 0, (struct sockaddr *)&sa, sizeof(sa));
		if (ret < 0) {
			printf("sendto fail %s:%d", __FUNCTION__, __LINE__);
		}
	}

	close(connect_fd);

	close(socket_fd);
	return 0;
}

/**
   * @fn                   :recvfrom_client
   * @brief                :
   * @scenario             :
   * API's covered         :socket,connect,close
   * Preconditions         :
   * Postconditions        :
   * @return               :void *
   */
void *recvfrom_tcpclient(void *args)
{
	int mysocket = 0;
	int newsoc = 0;
	struct sockaddr_in dest;

	mysocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (mysocket < 0) {
		printf("socket() fail %s:%d", __FUNCTION__, __LINE__);
		return 0;
	}
	memset(&dest, 0, sizeof(dest));
	dest.sin_family = PF_INET;
	dest.sin_addr.s_addr = inet_addr("127.0.0.1");
	dest.sin_port = htons(1111);

	recvfrom_wait();
	int ret = connect(mysocket, (struct sockaddr *)&dest, sizeof(struct sockaddr));
	if (ret < 0) {
		printf("connect fail %s:%d", __FUNCTION__, __LINE__);
		close(mysocket);
		return 0;
	}
	tc_net_recvfrom_tcp_p(mysocket);

	tc_net_recvfrom_tcp_n();
	tc_net_recvfrom_tcp_sock_n(newsoc);
	tc_net_recvfrom_tcp_conn_n(mysocket);

	close(mysocket);
	return 0;
}

/****************************************************************************
 * Name: recvfrom()
 ****************************************************************************/
int net_recvfrom_main(void)
{

	pthread_t Server, Client, tcpserver, tcpclient;

	pthread_create(&Server, NULL, recvfrom_udpserver, NULL);
	pthread_create(&Client, NULL, recvfrom_udpclient, NULL);

	pthread_join(Server, NULL);

	pthread_join(Client, NULL);

	pthread_create(&tcpserver, NULL, recvfrom_tcpserver, NULL);
	pthread_create(&tcpclient, NULL, recvfrom_tcpclient, NULL);

	pthread_join(tcpserver, NULL);

	pthread_join(tcpclient, NULL);

	return 0;
}
