/****************************************************************************
*
* Copyright 2017 Samsung Electronics All Rights Reserved.
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

// @file itc_net_send.c
// @brief ITC Test Case Example for send() API
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>
#include <arpa/inet.h>

#include "tc_internal.h"

#define PORTNUM 1110

static pthread_mutex_t g_lock = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t g_cond;

/**
* @testcase            :itc_net_send_n_fd
* @brief               :used to transmit a message to another socket
* @scenario            :sending invalid file descriptor
* @apicovered          :send()
* @precondition        :None
* @postcondition       :None
*/
static void itc_net_send_n_fd(void)
{
	char *msg = "Hello World !\n";
	int ret = send(-1, msg, strlen(msg), 0);

	TC_ASSERT_EQ("send", ret, -1);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            :itc_net_send_n_msg
* @brief               :used to transmit a message to another socket
* @scenario            :sending NULL msg
* @apicovered          :send()
* @precondition        :None
* @postcondition       :None
*/
static void itc_net_send_n_msg(int client_socket)
{
	char *msg = NULL;
	int ret = send(client_socket, msg, strlen(msg), 0);

	TC_ASSERT_EQ("send", ret, -1);
	TC_SUCCESS_RESULT();
}

/**
* @fn                   :server
* @brief                :For client server scenario creating thread function
* @scenario             :None
* API's covered         :socket,bind,listen,close
* Preconditions         :None
* Postconditions        :None
*/
static void *server(void *args)
{
	struct sockaddr_in sa, client;
	int ret;
	int client_socket;
	int addr_len = sizeof(client);
	int server_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (server_socket == -1) {
		printf("[%s]Failed to create socket\n", __func__);
		return NULL;
	}

	if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &(int){ 1 }, sizeof(int)) < 0) {
		printf("setsockopt(SO_REUSEADDR) failed %s:%d:%d\n", __FUNCTION__, __LINE__, errno);
		close(server_socket);
		return 0;
	}

	memset(&sa, 0, sizeof(sa));

	sa.sin_family = PF_INET;
	sa.sin_port = htons(PORTNUM);
	sa.sin_addr.s_addr = inet_addr("127.0.0.1");

	ret = bind(server_socket, (struct sockaddr *)&sa, sizeof(sa));
	if (ret == -1) {
		printf("[%s]Failed to bind socket\n", __func__);
		goto end;
	}

	ret = listen(server_socket, 2);
	if (ret == -1) {
		printf("[%s]Failed to listen socket\n", __func__);
		goto end;
	}

	pthread_mutex_lock(&g_lock);
	pthread_cond_signal(&g_cond);
	pthread_mutex_unlock(&g_lock);

	client_socket = accept(server_socket, (struct sockaddr *)&client, (socklen_t *)&addr_len);
	if (client_socket < 0) {
		goto end;
	}

	itc_net_send_n_fd();
	itc_net_send_n_msg(client_socket);

	ret = close(client_socket);
	if (ret == -1) {
		printf("[%s]Failed to close socket\n", __func__);
	}

end:
	ret = close(server_socket);
	if (ret == -1) {
		printf("[%s]Failed to close socket\n", __func__);
	}
	return 0;
}

/**
* @fn                   :client
* @brief                :For client server scenario creating thread function
* @scenario             :None
* API's covered         :socket,connect,recv,close
* Preconditions         :None
* Postconditions        :None
*/
static void *client(void *args)
{
	int client_socket;
	int ret;
	struct sockaddr_in dest;

	client_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (client_socket == -1) {
		printf("[%s]Failed to create socket\n", __func__);
		return 0;
	}

	memset(&dest, 0, sizeof(dest));
	dest.sin_family = PF_INET;
	dest.sin_addr.s_addr = inet_addr("127.0.0.1");
	dest.sin_port = htons(PORTNUM);

	ret = connect(client_socket, (struct sockaddr *)&dest, sizeof(struct sockaddr));
	if (ret == -1) {
		printf("[%s]Failed to connect socket\n", __func__);
		goto end;
	}
	pthread_cond_wait(&g_cond, &g_lock);

end:
	ret = close(client_socket);
	if (ret == -1) {
		printf("[%s]Failed to close socket\n", __func__);
	}
	return 0;
}

/****************************************************************************
* Name: send()
****************************************************************************/
int itc_net_send_main(void)
{
	pthread_cond_init(&g_cond, NULL);
	pthread_t Server, Client;

	pthread_create(&Server, NULL, server, NULL);
	pthread_create(&Client, NULL, client, NULL);

	pthread_join(Client, NULL);
	pthread_join(Server, NULL);

	pthread_cond_destroy(&g_cond);

	return 0;
}
