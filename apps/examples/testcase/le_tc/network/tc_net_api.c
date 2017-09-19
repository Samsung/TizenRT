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

/// @file tc_net_api.c
/// @brief Test Case Example for api() API
#include "tc_internal.h"
#include <net/lwip/api.h>
#include <net/lwip/api_msg.h>
#include <net/lwip/ipv4/ip_addr.h>
#include <net/lwip/err.h>
#include <net/lwip/raw.h>
#include <net/lwip/sockets.h>
#include <net/lwip/tcp.h>
#define SERVER_PORT  5000

#define TRUE             1
#define FALSE            0

static int count_wait;
/**
* @fn                    : sig_wait
* @brief                 : function to wait on semaphore
* @scenario              :
* @API's covered         :
* @Preconditions         :
* @Postconditions        :
* @return                : void
*/
static void sig_wait(void)
{
	while (count_wait <= 0) {
		printf("");
	}
	count_wait--;
}

/**
* @fn                    : sig_call
* @brief                 : function to signal semaphore
* @scenario              : none
* @API's covered         : none
* @Preconditions         : none
* @Postconditions        : none
* @return                : void
*/
static void sig_call(void)
{
	count_wait++;
}

/**
* @fn                    : tc_netconn_server
* @brief                 : function to signal semaphore
* @scenario              : none
* @API's covered         : none
* @Preconditions         : none
* @Postconditions        : none
* @return                : void
*/
void tc_netconn_server(void)
{
	int i = ZERO, rc = ZERO, flags, ret;
	int new_sd;
	char buffer[80];
	struct sockaddr_in addr;

	int sock = socket(AF_INET, SOCK_STREAM, 0);
	TC_ASSERT_EQ("socket", sock, ZERO);

	flags = fcntl(sock, F_GETFL, 0);
	TC_ASSERT_NEQ("fcntl", flags, NEG_VAL);

	flags = fcntl(sock, F_SETFL, flags | O_NONBLOCK);
	TC_ASSERT_NEQ("fcntl", flags, NEG_VAL);

	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	addr.sin_port = htons(SERVER_PORT);

	ret = bind(sock, (struct sockaddr *)&addr, sizeof(addr));
	TC_ASSERT_NEQ("bind", ret, NEG_VAL);

	ret = listen(sock, 3);
	TC_ASSERT_NEQ("listen", ret, NEG_VAL);

	sig_call();
	new_sd = accept(sock, NULL, NULL);
	TC_ASSERT_NEQ("accept", new_sd, NEG_VAL);

	while (i++ < 10 && rc < 0) {
		rc = recv(new_sd, buffer, sizeof(buffer), 0);
	}
	TC_ASSERT_NEQ("recv", rc, NEG_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @fn                    : tc_netconn_client
* @brief                 :
* @scenario              : none
* @API's covered         : none
* @Preconditions         : none
* @Postconditions        : none
* @return                : void
*/
void tc_netconn_client(void)
{
	int ret, len;
	char buffer[200] = "HELLOWORLD\n";
	struct sockaddr_in dest;

	int sock = socket(AF_INET, SOCK_STREAM, 0);
	TC_ASSERT_EQ("socket", sock, ZERO);

	memset(&dest, 0, sizeof(dest));
	dest.sin_family = AF_INET;
	dest.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	dest.sin_port = htons(5000);

	sig_wait();
	ret = connect(sock, (struct sockaddr *)&dest, sizeof(struct sockaddr));
	TC_ASSERT_EQ("connect", ret, ZERO);

	len = write(sock, buffer, strlen(buffer));
	TC_ASSERT_NEQ("write", len, ZERO);
}

/**
* @fn                    : netconn_api_server
* @brief                 :
* @scenario              : none
* @API's covered         : none
* @Preconditions         : none
* @Postconditions        : none
* @return                : void*
*/
void *netconn_api_server(void *args)
{
	tc_netconn_server();
	return NULL;
}

/**
* @fn                    : netconn_api_client
* @brief                 :
* @scenario              : none
* @API's covered         : none
* @Preconditions         : none
* @Postconditions        : none
* @return                : void*
*/
void *netconn_api_client(void *args)
{
	tc_netconn_client();
	return NULL;
}

/**
* @testcase              : net_api_main
* @brief                 :
* @scenario              :
* @apicovered            :
* @precondition          :
* @postcondition         :
*/
int net_api_main(void)
{
	pthread_t Server, Client;
	pthread_create(&Server, NULL, netconn_api_server, NULL);
	pthread_create(&Client, NULL, netconn_api_client, NULL);
	pthread_join(Server, NULL);
	pthread_join(Client, NULL);
	return 0;
}
