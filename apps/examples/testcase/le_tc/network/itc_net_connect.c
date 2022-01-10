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

/// @file itc_net_connect.c
/// @brief ITC Scenario Test Case for connect() API

#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <semaphore.h>

#include "tc_internal.h"

#define SERVER_MSG "Hello from server"
#define CLIENT_MSG "Hello from client 0 "
#define BUFF_LEN 64
#define SERVER_WAIT_TIME 3
#define CLIENT_WAIT_TIME 7

static int g_port;
static sem_t g_sig_clitomain;
static sem_t g_sig_srvtomain;

static int _recv(int sd, char *buf, int buf_size)
{
	int recved = 0;
	while (recved < buf_size) {
		int ret = recv(sd, buf + recved, buf_size - recved, MSG_WAITALL);
		if (ret <= 0) {
			printf("recv error %d %s %s:%d\n", ret, strerror(errno), __FUNCTION__, __LINE__);
			return -1;
		}
		recved += ret;
	}
	return 0;
}

static int _send(int sd, char *buf, int buf_size)
{
	int sent = 0;
	while (sent < buf_size) {
		int ret = send(sd, buf + sent, buf_size - sent, 0);
		if (ret <= 0) {
			printf("sent error %d %s %s:%d\n", ret, strerror(errno), __FUNCTION__, __LINE__);
			return -1;
		}
		sent += ret;
	}
	return 0;
}

static void _initialize_test(void)
{
	TC_ASSERT_EQ("sem init", sem_init(&g_sig_clitomain, 0, 0), 0);
	TC_ASSERT_EQ("sem init", sem_init(&g_sig_srvtomain, 0, 0), 0);
}

static void _deinitialize_test(void)
{
	sem_destroy(&g_sig_clitomain);
	sem_destroy(&g_sig_srvtomain);
}

/**
* @fn               :server_connect
* @description      :starts server
* @return           :void
*/
static void *server_connect(void *ptr_num_clients)
{
	int server_socket;
	int client_socket;
	int opt = 1;
	int ret;
	struct sockaddr_in address;
	int addrlen = sizeof(address);
	int num_clients = *((int *)ptr_num_clients);
	char buffer[BUFF_LEN] = {0};
	char *ptr_msg = SERVER_MSG;
	int recv_len = 0;
	int send_len = 0;
	int *pret = malloc(sizeof(int));
	if (NULL == pret) {
		printf("Memory allocation to pret is failed\n");
		return NULL;
	}
	*pret = OK;

	printf("Server thread started...\n");
	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket < 0) {
		*pret = ERROR;
		pthread_exit(pret);
	}
	ret = setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	if (ret < 0) {
		printf("setsockopt API failed \n");
		*pret = ERROR;
		pthread_exit(pret);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(g_port);
	ret = bind(server_socket, (struct sockaddr *)&address, sizeof(address));
	if (ret != OK) {
		close(server_socket);
		*pret = ERROR;
		pthread_exit(pret);
	}

	ret = listen(server_socket, 5);
	if (ret != OK) {
		close(server_socket);
		*pret = ERROR;
		pthread_exit(pret);
	}

	sem_post(&g_sig_srvtomain);

	while (num_clients-- > 0) {
		client_socket = accept(server_socket, (struct sockaddr *)&address, (socklen_t *)&addrlen);
		if (client_socket < 0) {
			close(server_socket);
			*pret = ERROR;
			pthread_exit(pret);
		}

		ret = _recv(client_socket, buffer, sizeof(CLIENT_MSG));
		if (ret != 0) {
			close(client_socket);
			close(server_socket);
			pthread_exit(pret);
		}
		strncpy(buffer + sizeof(CLIENT_MSG), ptr_msg, sizeof(SERVER_MSG));
		if (_send(client_socket, buffer + sizeof(CLIENT_MSG), sizeof(SERVER_MSG)) != 0) {
			close(client_socket);
			close(server_socket);
			*pret = ERROR;
			pthread_exit(pret);
		}

		ret = close(client_socket);
		if (ret != OK) {
			close(server_socket);
			*pret = ERROR;
			pthread_exit(pret);
		}
		printf("Server thread : finished with client\n");
		recv_len = 0;
		send_len = 0;
	}

	ret = close(server_socket);
	if (ret != OK) {
		*pret = ERROR;
		pthread_exit(pret);
	}
	printf("Server completed\n");
	pthread_exit(pret);
}

/**
* @fn               :client_connect
* @description      :starts client
* @return           :void
*/
static void *client_connect(void *ptr_id)
{
	int *id;
	id = (int *)ptr_id;
	int sock = 0;
	int ret;
	struct sockaddr_in serv_addr;
	char buffer[BUFF_LEN] = {0};
	int *pret = malloc(sizeof(int));
	if (NULL == pret) {
		printf("Memory allocation to pret is failed\n");
		return NULL;
	}
	*pret = OK;

	printf("Client thread started...\n");
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		pthread_exit(pret);
	}

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serv_addr.sin_port = htons(g_port);
	ret = connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
	if (ret != OK) {
		printf("connect error %s:%d\n", __FUNCTION__, __LINE__);
		close(sock);
		*pret = ret;
		pthread_exit(pret);
	}

	strncpy(buffer, CLIENT_MSG, sizeof(CLIENT_MSG));
	buffer[sizeof(CLIENT_MSG) - 3] = '0' + (*id);
	printf("client send: %s\n", buffer);
	ret = _send(sock, buffer, sizeof(CLIENT_MSG));
	if (ret != 0) {
		printf("send error %s %s:%d\n", strerror(errno), __FUNCTION__, __LINE__);
		close(sock);
		pthread_exit(pret);
	}

	ret = _recv(sock, buffer + sizeof(CLIENT_MSG), sizeof(SERVER_MSG));
	if (ret != 0) {
		printf("recv error %s %s:%d\n", strerror(errno), __FUNCTION__, __LINE__);
		close(sock);
		*pret = ERROR;
		pthread_exit(pret);
	}

	if (strncmp(buffer + sizeof(CLIENT_MSG), SERVER_MSG, strlen(SERVER_MSG)) != 0) {
		printf("integrity error %s %s:%d\n", strerror(errno), __FUNCTION__, __LINE__);
		close(sock);
		*pret = ERROR;
		pthread_exit(pret);
	}
	ret = close(sock);
	if (ret != OK) {
		printf("close error %s %s:%d\n", strerror(errno), __FUNCTION__, __LINE__);
		*pret = ERROR;
		pthread_exit(pret);
	}
	printf("Client completed\n");
	pthread_exit(pret);
}

/**
* @testcase         :itc_net_connect_p
* @brief            :connects the socket referred to by the file descriptor
* @scenario         :start server and client thread, connect client to server
* @apicovered       :connect()
* @precondition     :none
* @postcondition    :none
*/
static void itc_net_connect_p(void)
{
	pthread_t server_thread;
	pthread_t client_thread;
	int num_clients = 1;
	int id = 1;
	int ret;
	int *pret = OK;

	g_port = 8880;
	_initialize_test();

	pthread_create(&server_thread, NULL, server_connect, (void *)&num_clients);
	sem_wait(&g_sig_srvtomain);
	pthread_create(&client_thread, NULL, client_connect, (void *)&id);

	ret = pthread_join(server_thread, (void *)&pret);
	TC_ASSERT_EQ_CLEANUP("pthread_join", ret, OK, free(pret); pthread_join(client_thread, NULL));
	TC_ASSERT_EQ_CLEANUP("pthread_join", *pret, OK, free(pret); pthread_join(client_thread, NULL));

	ret = pthread_join(client_thread, (void *)&pret);
	TC_ASSERT_EQ_CLEANUP("pthread_join", ret, OK, free(pret));
	TC_ASSERT_EQ_CLEANUP("pthread_join", *pret, OK, free(pret));

	free(pret);
	TC_SUCCESS_RESULT();
	_deinitialize_test();
}

/**
* @testcase         :itc_net_connect_p_multiple_clients
* @brief            :connects the socket referred to by the file descriptor
* @scenario         :start server and 3 client thread, connect client thread to server one by one
* @apicovered       :connect()
* @precondition     :none
* @postcondition    :none
*/
static void itc_net_connect_p_multiple_clients(void)
{
	pthread_t server_thread;
	pthread_t client1_thread;
	pthread_t client2_thread;
	pthread_t client3_thread;
	int num_clients = 3;
	int id1 = 1;
	int id2 = 2;
	int id3 = 3;
	int ret;
	int *pret = OK;

	_initialize_test();
	g_port = 8890;
	_initialize_test();

	pthread_create(&server_thread, NULL, server_connect, (void *)&num_clients);
	sem_wait(&g_sig_srvtomain);
	pthread_create(&client1_thread, NULL, client_connect, (void *)&id1);
	pthread_create(&client2_thread, NULL, client_connect, (void *)&id2);
	pthread_create(&client3_thread, NULL, client_connect, (void *)&id3);

	ret = pthread_join(server_thread, (void *)&pret);
	TC_ASSERT_EQ_CLEANUP("pthread_join", ret, OK,
						 free(pret);
						 pthread_join(client3_thread, NULL);
						 pthread_join(client2_thread, NULL);
						 pthread_join(client1_thread, NULL));
	TC_ASSERT_EQ_CLEANUP("pthread_join", *pret, OK,
						 free(pret);
						 pthread_join(client3_thread, NULL);
						 pthread_join(client2_thread, NULL);
						 pthread_join(client1_thread, NULL));
	ret = pthread_join(client3_thread, (void *)&pret);
	TC_ASSERT_EQ_CLEANUP("pthread_join", ret, OK,
						 free(pret);
						 pthread_join(client2_thread, NULL);
						 pthread_join(client1_thread, NULL));
	TC_ASSERT_EQ_CLEANUP("pthread_join", *pret, OK,
						 free(pret);
						 pthread_join(client2_thread, NULL);
						 pthread_join(client1_thread, NULL));

	ret = pthread_join(client2_thread, (void *)&pret);
	TC_ASSERT_EQ_CLEANUP("pthread_join", ret, OK,
						 free(pret);
						 pthread_join(client1_thread, NULL));
	TC_ASSERT_EQ_CLEANUP("pthread_join", *pret, OK,
						 free(pret);
						 pthread_join(client1_thread, NULL));

	ret = pthread_join(client1_thread, NULL);
	TC_ASSERT_EQ_CLEANUP("pthread_join", ret, OK, free(pret));

	free(pret);
	TC_SUCCESS_RESULT();
	_deinitialize_test();
}

int itc_net_connect_main(void)
{
	itc_net_connect_p();
	itc_net_connect_p_multiple_clients();

	return 0;
}
