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

// @file tc_net_send.c
// @brief Test Case Example for send() API
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
#include <net/lwip/udp.h>
#include <net/lwip/ipv4/ip.h>
#include "tc_internal.h"

#define PORTNUM        5007
#define MAXRCVLEN      20
#define BACKLOG        2
#define UDP_SRC_PORT   21
#define UDP_DEST_PORT  5004

static int count_wait;

/**
* @fn                   : sig_wait
* @brief                : Function to wait on semaphore
* @scenario             : use wait function to decrement count value.
* @API's covered        : none
* @Preconditions        : none
* @Postconditions       : none
* @return               : void
*/
static void sig_wait(void)
{
	while (count_wait <= ZERO) {
		printf("");
	}
	count_wait--;
}

/**
* @fn                   : sig_call
* @brief                : Function to signal semaphore
* @scenario             : use to increase the count value.
* @API's covered        : none
* @Preconditions        : none
* @Postconditions       : none
* @return               : void
*/
static void sig_call(void)
{
	count_wait++;
}

/**
* @testcase             : tc_net_send_p
* @brief                : This send API send a message on a socket.
* @scenario             : used to transmit a message to another socket.
* @apicovered           : accept(),send()
* @precondition         : socket file descriptor.
* @postcondition        : none
* @return               : void
*/
static void tc_net_send_p(int fd)
{
	char *msg = "Hello World !\n";

	int ConnectFD = accept(fd, NULL, NULL);
	TC_ASSERT_NEQ_CLEANUP("accept", ConnectFD, NEG_VAL, close(fd));

	int ret = send(ConnectFD, msg, strlen(msg), 0);
	TC_ASSERT_NEQ_CLEANUP("send", ret, NEG_VAL, close(ConnectFD));
	close(ConnectFD);
	TC_SUCCESS_RESULT();
}

static void tc_net_raw_server(void)
{
	int ret;
	struct sockaddr_in sa;

	int sock = socket(AF_INET, SOCK_RAW, 0);
	TC_ASSERT_NEQ("socket", sock, NEG_VAL);

	memset(&sa, 0, sizeof(sa));
	sa.sin_family = PF_INET;
	sa.sin_port = htons(PORTNUM);
	sa.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

	ret = bind(sock, (struct sockaddr *)&sa, sizeof(sa));
	TC_ASSERT_NEQ_CLEANUP("bind", ret, NEG_VAL, close(sock));

	ret = listen(sock, BACKLOG);
	TC_ASSERT_NEQ_CLEANUP("listen", ret, NEG_VAL, close(sock));
	sig_call();
	tc_net_send_p(sock);
	close(sock);
}

static void tc_net_raw_client(void)
{
	int len, ret;
	char buffer[] = "Hello World!";
	struct sockaddr_in dest;
	struct ip_addr dst;
	struct ip_addr src;
	struct ip_hdr iphdr;
	struct udp_hdr udp;
	int one = 1;

	int sock = socket(AF_INET, SOCK_RAW, IP_PROTO_UDP);
	TC_ASSERT_NEQ("socket", sock, NEG_VAL);

	len = sizeof(struct ip_hdr) + sizeof(struct udp_hdr);

	IP4_ADDR(&dst, 127, 0, 0, 1);
	IP4_ADDR(&src, 127, 0, 0, 1);

	memset(&dest, 0, sizeof(dest));
	dest.sin_family = PF_INET;
	dest.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	dest.sin_port = htons(PORTNUM);
	IPH_VHL_SET(&iphdr, 4, 0);
	IPH_TOS_SET(&iphdr, 16);
	IPH_LEN_SET(&iphdr, sizeof(struct ip_hdr) + sizeof(struct udp_hdr));
	IPH_ID_SET(&iphdr, htons(54321));
	IPH_TTL_SET(&iphdr, 64);
	IPH_PROTO_SET(&iphdr, 17);
	ip_addr_copy(iphdr.src, src);
	ip_addr_copy(iphdr.dest, dst);
	udp.src = htons(UDP_SRC_PORT);
	udp.dest = htons(UDP_DEST_PORT);
	udp.len = htons(sizeof(struct udp_hdr));

	ret = setsockopt(sock, IPPROTO_IP, 0, &one, sizeof(one));
	TC_ASSERT_NEQ_CLEANUP("setsockopt", ret, NEG_VAL, close(sock));
	ret = sendto(sock, buffer, len, 0, (struct sockaddr *)&dest, sizeof(dest));
	TC_ASSERT_NEQ_CLEANUP("sendto", ret, ZERO, close(sock));
	close(sock);
}

/**
* @fn                   : server
* @brief                : Create a Tcp server.
* @scenario             : Create a tcp server for checking send api.
* API's covered         : socket,bind,listen,close
* Preconditions         : socket file descriptor.
* Postconditions        : none
* @return               : void*
*/
static void* server(void *args)
{
	return NULL;
}

/**
* @fn                   : client
* @brief                : This api create client.
* @scenario             : Create tcp client.
* API's covered         : socket,connect,recv,close
* Preconditions         : socket file descriptor.
* Postconditions        : none
* @return               : void*
*/
static void* client(void *args)
{
	tc_net_raw_client();
	return NULL;
}

/**
* @fn                  : net_raw_client_server
* @brief               : This api create client and server thread.
* @scenario            : Create client and server thread to test send api.
* API's covered        : none
* Preconditions        : none
* Postconditions       : none
* @return              : void
*/
static void net_raw_client_server(void)
{
	pthread_t Server, Client;

	pthread_create(&Server, NULL, server, NULL);
	pthread_create(&Client, NULL, client, NULL);

	pthread_join(Server, NULL);
	pthread_join(Client, NULL);
}

/****************************************************************************
 * Name: net_raw_main()
 ****************************************************************************/
int net_raw_main(void)
{
	net_raw_client_server();
	return 0;
}
