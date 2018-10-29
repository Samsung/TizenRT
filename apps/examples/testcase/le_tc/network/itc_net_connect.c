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

#include "tc_internal.h"

#define SERVER_MSG "Hello from server"
#define CLIENT_MSG "Hello from client 0 "
#define BUFF_LEN 64
#define SERVER_WAIT_TIME 3
#define CLIENT_WAIT_TIME 7

static int g_port;
/**
* @fn               :server_connect
* @description      :starts server
* @return           :void
*/
static void *server_connect(void *ptr_num_clients)
{
	int server_socket;
	int client_socket;
	int valrecv = 0;
	int valsend = 0;
	int opt = 1;
	int ret;
	struct sockaddr_in address;
	int addrlen = sizeof(address);
	int num_clients = *((int *)ptr_num_clients);
	char buffer[BUFF_LEN] = {0};
	char *ptr_msg = SERVER_MSG;
	int total_recv;
	int total_send;
	int *pret = malloc(sizeof(int));
	int recv_len = 0;
	int send_len = 0;
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
	int nRet = setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	if (nRet < 0) {
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
	while (num_clients-- > 0) {
		client_socket = accept(server_socket, (struct sockaddr *)&address, (socklen_t*)&addrlen);
		if (client_socket < 0) {
			close(server_socket);
			*pret = ERROR;
			pthread_exit(pret);
		}
		total_recv = strlen(CLIENT_MSG);
		memset(buffer, 0, BUFF_LEN);
		while (total_recv) {
			valrecv = recv(client_socket, buffer + recv_len, BUFF_LEN - recv_len, MSG_WAITALL);
			printf("server recv: %s\n", buffer);
			if (valrecv == -1) {
				close(client_socket);
				close(server_socket);
				pthread_exit(pret);
			}
			total_recv -= valrecv;
			if (total_recv < 0) {
				close(client_socket);
				close(server_socket);
				*pret = ERROR;
				pthread_exit(pret);
			}
			recv_len += valrecv;
		}
		strncpy(buffer + recv_len, ptr_msg, sizeof(SERVER_MSG));
		total_send = strlen(buffer);
		while (total_send) {
			valsend = send(client_socket, buffer + send_len, strlen(buffer) - send_len, 0);
			printf("server send: %s\n", buffer);
			if (valsend == -1) {
				close(client_socket);
				close(server_socket);
				*pret = ERROR;
				pthread_exit(pret);
			}
			total_send -= valsend;
			if (total_send < 0) {
				close(client_socket);
				close(server_socket);
				*pret = ERROR;
				pthread_exit(pret);
			}
			send_len += valsend;
		}
		sleep(SERVER_WAIT_TIME);
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
	int valrecv = 0;
	int valsend = 0;
	struct sockaddr_in serv_addr;
	char client_msg[BUFF_LEN];
	char buffer[BUFF_LEN] = {0};
	int total_recv;
	int total_send;
	unsigned int recv_len = 0;
	int send_len = 0;
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
		close(sock);
		*pret = ret;
		pthread_exit(pret);
	}
	strncpy(client_msg, CLIENT_MSG, sizeof(CLIENT_MSG));
	client_msg[strlen(client_msg) - 2] = '0' + (*id);
	total_send = strlen(client_msg);
	while (total_send) {
		valsend = send(sock, client_msg + send_len, strlen(client_msg) - send_len, 0);
		printf("client send: %s\n", client_msg);
		if (valsend == -1) {
			close(sock);
			pthread_exit(pret);
		}
		total_send -= valsend;
		if (total_send < 0) {
			close(sock);
			*pret = ERROR;
			pthread_exit(pret);
		}
		send_len += valsend;
	}
	total_recv = strlen(client_msg) + strlen(SERVER_MSG);
	memset(buffer, 0, BUFF_LEN);
	while (total_recv) {
		valrecv = recv(sock, buffer + recv_len, BUFF_LEN - recv_len, MSG_WAITALL);
		printf("client recv: %s\n", buffer);
		if (valrecv <= 0) {
			close(sock);
			*pret = ERROR;
			pthread_exit(pret);
		}
		total_recv -= valrecv;
		if (total_recv < 0) {
			close(sock);
			*pret = ERROR;
			pthread_exit(pret);
		}
		recv_len += valrecv;
	}
	if (strncmp(buffer, client_msg, strlen(client_msg)) != 0) {
		close(sock);
		*pret = ERROR;
		pthread_exit(pret);
	}
	if (strncmp(buffer + strlen(client_msg), SERVER_MSG, strlen(SERVER_MSG)) != 0) {
		close(sock);
		*pret = ERROR;
		pthread_exit(pret);
	}
	sleep(CLIENT_WAIT_TIME);
	ret = close(sock);
	if (ret != OK) {
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
	int *ptr = &num_clients;
	int *ptr_id = &id;
	int *pret = OK;
	g_port = 8880;

	pthread_create(&server_thread, NULL, server_connect, ptr);
	sleep(SERVER_WAIT_TIME);
	pthread_create(&client_thread, NULL, client_connect, ptr_id);

	sleep(CLIENT_WAIT_TIME);

	ret = pthread_join(server_thread, (void *)&pret);
	TC_ASSERT_EQ_CLEANUP("pthread_join", ret, OK, free(pret); pthread_join(client_thread, NULL));
	TC_ASSERT_EQ_CLEANUP("pthread_join", *pret, OK, free(pret); pthread_join(client_thread, NULL));
	ret = pthread_join(client_thread, (void *)&pret);
	TC_ASSERT_EQ_CLEANUP("pthread_join", ret, OK, free(pret));
	TC_ASSERT_EQ_CLEANUP("pthread_join", *pret, OK, free(pret));

	free(pret);
	TC_SUCCESS_RESULT();
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
	int *ptr_num_clients = &num_clients;
	int *ptr_id1 = &id1;
	int *ptr_id2 = &id2;
	int *ptr_id3 = &id3;
	int *pret = OK;

	g_port = 8890;

	pthread_create(&server_thread, NULL, server_connect, ptr_num_clients);
	sleep(SERVER_WAIT_TIME);
	pthread_create(&client1_thread, NULL, client_connect, ptr_id1);
	sleep(SERVER_WAIT_TIME);
	pthread_create(&client2_thread, NULL, client_connect, ptr_id2);
	sleep(SERVER_WAIT_TIME);
	pthread_create(&client3_thread, NULL, client_connect, ptr_id3);

	sleep(CLIENT_WAIT_TIME);

	ret = pthread_join(server_thread, (void *)&pret);
	TC_ASSERT_EQ_CLEANUP("pthread_join", ret, OK, free(pret); pthread_join(client3_thread, NULL); pthread_join(client2_thread, NULL); pthread_join(client1_thread, NULL));
	TC_ASSERT_EQ_CLEANUP("pthread_join", *pret, OK, free(pret); pthread_join(client3_thread, NULL); pthread_join(client2_thread, NULL); pthread_join(client1_thread, NULL));
	ret = pthread_join(client3_thread, (void *)&pret);
	TC_ASSERT_EQ_CLEANUP("pthread_join", ret, OK, free(pret); pthread_join(client2_thread, NULL); pthread_join(client1_thread, NULL));
	TC_ASSERT_EQ_CLEANUP("pthread_join", *pret, OK, free(pret); pthread_join(client2_thread, NULL); pthread_join(client1_thread, NULL));

	ret = pthread_join(client2_thread, (void *)&pret);
	TC_ASSERT_EQ_CLEANUP("pthread_join", ret, OK, free(pret); pthread_join(client1_thread, NULL));
	TC_ASSERT_EQ_CLEANUP("pthread_join", *pret, OK, free(pret); pthread_join(client1_thread, NULL));

	ret = pthread_join(client1_thread, NULL);
	TC_ASSERT_EQ_CLEANUP("pthread_join", ret, OK, free(pret));

	free(pret);
	TC_SUCCESS_RESULT();
}

int itc_net_connect_main(void)
{
	itc_net_connect_p();
	itc_net_connect_p_multiple_clients();

	return 0;
}
