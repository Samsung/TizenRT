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

#define PORTNUM 1112
#define MAXRCVLEN 20
int mut = 0;
/**
   * @fn                   :shutdown_wait
   * @brief                :function to wait on semaphore
   * @scenario             :
   * API's covered         :
   * Preconditions         :
   * Postconditions        :
   * @return               :void
   */
void shutdown_wait(void)
{
	while (mut <= 0) {

		printf("");
	}
	mut--;
}

/**
   * @fn                   :shutdown_signal
   * @brief                :function to signal semaphore
   * @scenario             :
   * API's covered         :
   * Preconditions         :
   * Postconditions        :
   * @return               :void
   */
void shutdown_signal(void)
{
	mut++;
}

/**
   * @testcase		   :tc_net_shutdown_recv_p
   * @brief		   :
   * @scenario		   :
   * @apicovered	   :shutdown()
   * @precondition	   :
   * @postcondition	   :
   */
void tc_net_shutdown_recv_p(int fd)
{
	int ret = shutdown(fd, SHUT_RD);

	TC_ASSERT_NEQ("shutdown", ret, -1);
	TC_SUCCESS_RESULT();

}

/**
   * @testcase		   :tc_net_shutdown_send_p
   * @brief		   :
   * @scenario		   :
   * @apicovered	   :shutdown()
   * @precondition	   :
   * @postcondition	   :
   */
void tc_net_shutdown_send_p(int fd)
{
	int ret = shutdown(fd, SHUT_WR);

	TC_ASSERT_NEQ("shutdown", ret, -1);
	TC_SUCCESS_RESULT();

}

/**
   * @testcase		   :tc_net_shutdown_sendrecv_p
   * @brief		   :
   * @scenario		   :
   * @apicovered	   :shutdown()
   * @precondition	   :
   * @postcondition	   :
   */
void tc_net_shutdown_sendrecv_p(int fd)
{
	int ret = shutdown(fd, SHUT_RDWR);

	TC_ASSERT_NEQ("shutdown", ret, -1);
	TC_SUCCESS_RESULT();

}

/**
   * @testcase		   :tc_net_shutdown_n
   * @brief		   :
   * @scenario		   :
   * @apicovered	   :shutdown()
   * @precondition	   :
   * @postcondition	   :
   */
void tc_net_shutdown_n(void)
{
	int ret = shutdown(-1, SHUT_RDWR);

	TC_ASSERT_EQ("shutdown", ret, -1);
	TC_SUCCESS_RESULT();

}

/**
   * @testcase		   :tc_net_shutdown_sock_n
   * @brief		   :
   * @scenario		   :
   * @apicovered	   :shutdown()
   * @precondition	   :
   * @postcondition	   :
   */
void tc_net_shutdown_sock_n(int fd)
{
	int ret = shutdown(fd, SHUT_RD);

	TC_ASSERT_EQ("shutdown", ret, -1);
	TC_SUCCESS_RESULT();

}

/**
   * @fn                   :shutdown_server
   * @brief                :
   * @scenario             :
   * API's covered         :socket,bind,listen,send,accept,close
   * Preconditions         :
   * Postconditions        :
   * @return               :void *
   */
void *shutdown_server(void *args)
{
	char *msg = "Hello World !\n";
	char buf[64] = {0,};
	struct sockaddr_in sa;
	int socket_fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (socket_fd < 0) {
		printf("socket error %s:%d\n", __FUNCTION__, __LINE__);
		return 0;
	}

	if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &(int){ 1 }, sizeof(int)) < 0) {
		printf("socket error %s:%d:%d\n", __FUNCTION__, __LINE__, errno);
		close(socket_fd);
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

	ret = listen(socket_fd, 1);
	if (ret < 0) {
		printf("listen fail %s:%d", __FUNCTION__, __LINE__);
		close(socket_fd);
		return 0;
	}

	shutdown_signal();

	int connect_fd = accept(socket_fd, NULL, NULL);
	if (connect_fd < 0) {
		printf("fail %s:%d", __FUNCTION__, __LINE__);
		close(socket_fd);
		return 0;
	}
	tc_net_shutdown_send_p(connect_fd);
	int val = send(connect_fd, msg, strlen(msg), 0);
	if (val == -1)
		printf("\nShutdown send successful %d\n", errno);
	tc_net_shutdown_recv_p(connect_fd);
	val = recv(connect_fd, buf, 64, 0);
	if (val == -1)
		printf("\nShutdown recv successful %d\n", errno);
	tc_net_shutdown_n();
	tc_net_shutdown_sock_n(socket_fd);
	close(connect_fd);
	close(socket_fd);

	return 0;

}

/**
   * @fn                   :shut_client
   * @brief                :
   * @scenario             :
   * API's covered         :socket,connect,recvfrom,close
   * Preconditions         :
   * Postconditions        :
   * @return               :void *
   */
void *shutdown_client(void *args)
{
	char buffer[MAXRCVLEN];
	int len, mysocket;
	struct sockaddr_in dest;

	mysocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (mysocket < 0) {
		printf("fail %s:%d", __FUNCTION__, __LINE__);
		return 0;
	}
	memset(&dest, 0, sizeof(dest));
	dest.sin_family = PF_INET;
	dest.sin_addr.s_addr = inet_addr("127.0.0.1");
	dest.sin_port = htons(PORTNUM);

	shutdown_wait();

	int ret = connect(mysocket, (struct sockaddr *)&dest, sizeof(struct sockaddr));
	if (ret < 0) {
		printf("fail %s:%d", __FUNCTION__, __LINE__);
		close(mysocket);
		return 0;
	}
	len = recv(mysocket, buffer, MAXRCVLEN, 0);
	if (len < 0) {
		printf("recv fail %s:%d\n", __FUNCTION__, __LINE__);
		close(mysocket);
		return 0;
	}
	buffer[len] = '\0';
	tc_net_shutdown_recv_p(mysocket);
	int val = recv(mysocket, buffer, MAXRCVLEN, 0);
	if (val == -1)
		printf("\nShutdown recv successful %d\n", errno);
	tc_net_shutdown_sendrecv_p(mysocket);
	val = recv(mysocket, buffer, MAXRCVLEN, 0);
	if (val == -1)
		printf("\nShutdown recv successful %d\n", errno);
	val = send(mysocket, buffer, strlen(buffer), 0);
	if (val == -1)
		printf("\nShutdown recv successful %d\n", errno);
	tc_net_shutdown_n();
	close(mysocket);
	return 0;

}

/****************************************************************************
 * Name: shutdown()
 ****************************************************************************/
int net_shutdown_main(void)
{

	pthread_t Server, Client;

	pthread_create(&Server, NULL, shutdown_server, NULL);
	pthread_create(&Client, NULL, shutdown_client, NULL);

	pthread_join(Server, NULL);

	pthread_join(Client, NULL);

	return 0;
}
