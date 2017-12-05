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

#include "tc_internal.h"

#define PORTNUM 1110
#define MAXRCVLEN 20
int s1 = 0;

/**
* @fn                   :wait1
* @brief                :function to wait on semaphore
* @scenario             :None
* API's covered         :None
* Preconditions         :None
* Postconditions        :None
*/
void wait1(void)
{
	while (s1 <= 0) {
		printf("");
	}
	s1--;
}

/**
* @fn                   :signal1
* @brief                :function to signal semaphore
* @scenario             :None
* API's covered         :None
* Preconditions         :None
* Postconditions        :None
*/
void signal1(void)
{
	s1++;
}

/**
* @testcase            :itc_net_send_n_fd
* @brief               :used to transmit a message to another socket
* @scenario            :sending invalid file descriptor
* @apicovered          :send()
* @precondition        :None
* @postcondition       :None
*/
void itc_net_send_n_fd(void)
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
void itc_net_send_n_msg(int socketFD)
{
	char *msg = NULL;
	int ret = send(socketFD, msg, strlen(msg), 0);

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
void *server(void *args)
{

	struct sockaddr_in sa;
	int ret;
	int SocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (SocketFD == -1) {
		printf("[%s]Failed to create socket\n", __func__);
		return NULL;
	}

	memset(&sa, 0, sizeof(sa));

	sa.sin_family = PF_INET;
	sa.sin_port = htons(PORTNUM);
	sa.sin_addr.s_addr = inet_addr("127.0.0.1");

	ret = bind(SocketFD, (struct sockaddr *)&sa, sizeof(sa));
	if (ret == -1) {
		printf("[%s]Failed to bind socket\n", __func__);
		goto end;
	}


	ret = listen(SocketFD, 2);
	if (ret == -1) {
		printf("[%s]Failed to listen socket\n", __func__);
		goto end;
	}

	signal1();
	itc_net_send_n_fd();
	itc_net_send_n_msg(SocketFD);

end:
	ret = close(SocketFD);
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
void *client(void *args)
{

	char buffer[MAXRCVLEN];
	int len;
	int mysocket;
	int ret;
	struct sockaddr_in dest;

	mysocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (mysocket == -1) {
		printf("[%s]Failed to create socket\n", __func__);
		return 0;
	}

	memset(&dest, 0, sizeof(dest));
	dest.sin_family = PF_INET;
	dest.sin_addr.s_addr = inet_addr("127.0.0.1");
	dest.sin_port = htons(PORTNUM);

	wait1();

	ret = connect(mysocket, (struct sockaddr *)&dest, sizeof(struct sockaddr));
	if (ret == -1) {
		printf("[%s]Failed to connect socket\n", __func__);
		goto end;
	}
	len = recv(mysocket, buffer, MAXRCVLEN, 0);
	if (len == -1) {
		printf("[%s]Failed to recv socket\n", __func__);
		goto end;
	}
	buffer[len] = '\0';

end:
	ret = close(mysocket);
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

	pthread_t Server, Client;

	pthread_create(&Server, NULL, server, NULL);
	pthread_create(&Client, NULL, client, NULL);

	pthread_join(Server, NULL);

	pthread_join(Client, NULL);

	return 0;
}
