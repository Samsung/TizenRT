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
#include "tc_internal.h"

#define SERVER_MSG "Hello from server"
#define CLIENT_MSG "Hello from client 0 "
#define BUFF_LEN 64
#define SEC_2 2
#define SEC_3 3
#define SEC_7 7

static int g_flag;
static int g_port;

/**
* @fn               :server_connect
* @description      :starts server
* @return           :void
*/
void *server_connect(void *ptr_num_clients)
{
	int server_socket;
	int client_socket;
	int valread;
	int opt = 1;
	int ret;
	struct sockaddr_in address;
	int addrlen = sizeof(address);
	int num_clients = *((int *)ptr_num_clients);
	char buffer[BUFF_LEN] = {0};
	char *ptr_msg = SERVER_MSG;

	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket < 0) {
		g_flag = ERROR;
		return NULL;
	}
	setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(g_port);
	ret = bind(server_socket, (struct sockaddr *)&address, sizeof(address));
	if (ret != OK) {
		g_flag = ERROR;
		close(server_socket);
		return NULL;
	}
	ret = listen(server_socket, 5);
	if (ret != OK) {
		g_flag = ERROR;
		close(server_socket);
		return NULL;
	}
	while (num_clients-- > 0) {
		client_socket = accept(server_socket, (struct sockaddr *)&address, (socklen_t*)&addrlen);
		if (client_socket < 0) {
			g_flag = ERROR;
			close(server_socket);
			return NULL;
		}
		memset(buffer, 0, BUFF_LEN);
		valread = read(client_socket, buffer, BUFF_LEN);
		if (valread != strlen(CLIENT_MSG)) {
			g_flag = ERROR;
			close(client_socket);
			close(server_socket);
			return NULL;
		}
		strncpy(buffer + valread, ptr_msg, sizeof(SERVER_MSG));
		ret = send(client_socket, buffer, strlen(buffer), 0);
		if (ret != strlen(CLIENT_MSG) + strlen(SERVER_MSG)) {
			g_flag = ERROR;
			close(client_socket);
			close(server_socket);
			return NULL;
		}
		sleep(SEC_2);
		ret = close(client_socket);
		if (ret != OK) {
			g_flag = ERROR;
			close(server_socket);
			return NULL;
		}
	}
	ret = close(server_socket);
	if (ret != OK) {
		g_flag = ERROR;
		return NULL;
	}
	return NULL;
}

/**
* @fn               :client_connect
* @description      :starts client
* @return           :void
*/
void *client_connect(void *ptr_id)
{
	int *id;
	id = (int *)ptr_id;
	int sock = 0;
	int ret;
	int valread;
	struct sockaddr_in serv_addr;
	char client_msg[BUFF_LEN];
	char buffer[BUFF_LEN] = {0};

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		g_flag = ERROR;
		return NULL;
	}
	memset(&serv_addr, '0', sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serv_addr.sin_port = htons(g_port);
	ret = connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
	if (ret != OK) {
		g_flag = ERROR;
		close(sock);
		return NULL;
	}
	strncpy(client_msg, CLIENT_MSG, sizeof(CLIENT_MSG));
	client_msg[strlen(client_msg) - 2] = '0' + (*id);
	ret = send(sock, client_msg, strlen(client_msg), 0);
	if (ret != strlen(client_msg)) {
		g_flag = ERROR;
		close(sock);
		return NULL;
	}
	valread = read(sock, buffer, BUFF_LEN);
	if (valread != strlen(client_msg) + strlen(SERVER_MSG)) {
		g_flag = ERROR;
		close(sock);
		return NULL;
	}
	if (strncmp(buffer, client_msg, strlen(client_msg)) != 0) {
		g_flag = ERROR;
		close(sock);
		return NULL;
	}
	if (strncmp(buffer + strlen(client_msg), SERVER_MSG, strlen(SERVER_MSG)) != 0) {
		g_flag = ERROR;
		close(sock);
		return NULL;
	}
	sleep(SEC_2);
	ret = close(sock);
	if (ret != OK) {
		g_flag = ERROR;
		return NULL;
	}
	return NULL;
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
	g_flag = OK;
	g_port = 8080;

	pthread_create(&server_thread, NULL, server_connect, ptr);
	sleep(SEC_2);
	pthread_create(&client_thread, NULL, client_connect, ptr_id);

	sleep(SEC_3);

	ret = pthread_cancel(server_thread);
	TC_ASSERT_NEQ_CLEANUP("pthread_cancel", ret, OK, pthread_cancel(client_thread));
	ret = pthread_cancel(client_thread);
	TC_ASSERT_NEQ("pthread_cancel", ret, OK);
	TC_ASSERT_EQ("connect", g_flag, OK);

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
	g_flag = OK;
	g_port = 8090;

	pthread_create(&server_thread, NULL, server_connect, ptr_num_clients);
	sleep(SEC_2);
	pthread_create(&client1_thread, NULL, client_connect, ptr_id1);
	pthread_create(&client2_thread, NULL, client_connect, ptr_id2);
	pthread_create(&client3_thread, NULL, client_connect, ptr_id3);

	sleep(SEC_7);

	ret = pthread_cancel(server_thread);
	TC_ASSERT_NEQ_CLEANUP("pthread_cancel", ret, OK, pthread_cancel(client1_thread); pthread_cancel(client2_thread); pthread_cancel(client3_thread));

	ret = pthread_cancel(client1_thread);
	TC_ASSERT_NEQ_CLEANUP("pthread_cancel", ret, OK, pthread_cancel(client2_thread); pthread_cancel(client3_thread));

	ret = pthread_cancel(client2_thread);
	TC_ASSERT_NEQ_CLEANUP("pthread_cancel", ret, OK, pthread_cancel(client3_thread));

	ret = pthread_cancel(client3_thread);
	TC_ASSERT_NEQ("pthread_cancel", ret, OK);

	TC_ASSERT_EQ("connect", g_flag, OK);

	TC_SUCCESS_RESULT();
}

void itc_net_connect_main(void)
{
	itc_net_connect_p();
	itc_net_connect_p_multiple_clients();

	return;
}
