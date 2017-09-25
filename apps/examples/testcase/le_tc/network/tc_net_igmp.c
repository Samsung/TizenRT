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

// @file tc_net_igmp.c
// @brief Test Case Example for igmp() API
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
#include <netdb.h>

#include "tc_internal.h"

#define PORTNUM        5001
#define MAXRCVLEN      20
#define BACKLOG        1
#define HOST_NAME_MAX  20
#define DSIZE          30

static int count_wait;
struct sockaddr_in groupSock;
struct sockaddr_in localSock;

/**
* @fn                    :wait
* @brief                 :function to wait on semaphore.
* @scenario              :use wait function to decrement count_wait value.
* @API's covered         :none
* @Preconditions         :none
* @Postconditions        :none
* @return                :void
*/
static void wait(void)
{
	while (count_wait <= ZERO) {
		printf("");
	}
	count_wait--;
}

/**
* @fn                    :nw_signal
* @brief                 :function to signal semaphore.
* @scenario              :use to increase the count_wait value.
* @API's covered         :none
* @Preconditions         :none
* @Postconditions        :none
* @return                :void
*/
static void nw_signal(void)
{
	count_wait++;
}

/**
* @fn                   :tc_net_igmp_server
* @brief                :create a udp server for igmp
* @scenario             :create a udp server to test igmp api
* @API's covered        :socket,bind,listen,close
* @Preconditions        :none
* @Postconditions       :none
* @return               :void
*/
static void tc_net_igmp_server(void)
{
	int fd, ret;
	struct in_addr localInterface;
	struct ip_mreq group;
	struct hostent *thehost, *retGethost;
	char databuf[DSIZE] = "Multicast test message";
	int datalen = sizeof(databuf);
	char *clientHostName = malloc(HOST_NAME_MAX);

	/* Create a datagram socket on which to send. */
	fd = socket(AF_INET, SOCK_DGRAM, 0);
	TC_ASSERT_NEQ("socket", fd, NEG_VAL);

	/* Initialize the group sockaddr structure with a */
	/* group address of 225.1.1.1 and port 5555. */
	memset((char *)&groupSock, 0, sizeof(groupSock));
	groupSock.sin_family = AF_INET;
	groupSock.sin_addr.s_addr = inet_addr("226.1.1.1");
	groupSock.sin_port = htons(5555);

	/* Set local interface for outbound multicast datagrams. */
	/* The IP address specified must be associated with a local, */
	/* multicast capable interface. */
	retGethost = (struct hostent*)gethostbyname(clientHostName);
	TC_ASSERT_NEQ_CLEANUP("gethostbyname", retGethost, NULL, close(fd));
	thehost = (struct hostent*)gethostbyname(clientHostName);
	memcpy(&group.imr_interface.s_addr, thehost->h_addr_list[ZERO], thehost->h_length);

	ret = setsockopt(fd, IPPROTO_IP, IP_MULTICAST_IF, (char *)&localInterface, sizeof(localInterface));
	TC_ASSERT_NEQ_CLEANUP("setsockopt", ret, NEG_VAL, close(fd));

	/* Send a message to the multicast group specified by the */
	/* groupSock sockaddr structure. */
	ret = sendto(fd, databuf, datalen, 0, (struct sockaddr *)&groupSock, sizeof(groupSock));
	TC_ASSERT_NEQ_CLEANUP("sendto", ret, NEG_VAL, close(fd));
	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_net_igmp_client
* @brief                :create the client for igmp
* @scenario             :create udp client
* @API's covered        :socket,connect,close
* @Preconditions        :none
* @Postconditions       :none
* @return               :void
*/
static void tc_net_igmp_client(void)
{
	int fd, ret;
	int datalen;
	char databuf[DSIZE];
	struct ip_mreq group;
	struct hostent *thehost, *retGethost;
	char *clientHostName = malloc(HOST_NAME_MAX);

	retGethost = (struct hostent*)gethostbyname(clientHostName);
	TC_ASSERT_NEQ("gethostbyname", retGethost, NULL);
	thehost = (struct hostent*)gethostbyname(clientHostName);

	/* Create a datagram socket on which to receive. */
	fd = socket(AF_INET, SOCK_DGRAM, 0);
	TC_ASSERT_NEQ("socket", fd, NEG_VAL);

	/* Enable SO_REUSEADDR to allow multiple instances of this */
	/* application to receive copies of the multicast datagrams. */
	{
		int reuse = 1;
		ret = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(reuse));
		TC_ASSERT_NEQ_CLEANUP("setsockopt", ret, NEG_VAL, close(fd));
	}

	/* Bind to the proper port number with the IP address */
	/* specified as INADDR_ANY. */
	memset((char *)&localSock, 0, sizeof(localSock));
	localSock.sin_family = AF_INET;
	localSock.sin_port = htons(5555);
	localSock.sin_addr.s_addr = INADDR_ANY;
	ret = bind(fd, (struct sockaddr *)&localSock, sizeof(localSock));
	TC_ASSERT_NEQ_CLEANUP("bind", ret, NEG_VAL, close(fd));

	/* Join the multicast group 226.1.1.1 on the local 203.106.93.94 */
	/* interface. Note that this IP_ADD_MEMBERSHIP option must be */
	/* called for each local interface over which the multicast */
	/* datagrams are to be received. */
	group.imr_multiaddr.s_addr = inet_addr("226.1.1.1");
	memcpy(&group.imr_interface.s_addr, thehost->h_addr_list[ZERO], thehost->h_length);
	ret = setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&group, sizeof(group));
	TC_ASSERT_NEQ_CLEANUP("setsockopt", ret, NEG_VAL, close(fd));

	/* Read from the socket. */
	datalen = sizeof(databuf);
	ret = read(fd, databuf, datalen);
	TC_ASSERT_NEQ_CLEANUP("read", ret, ZERO, close(fd));
	close(fd);
	TC_SUCCESS_RESULT();
}

/**
* @fn                   :Server
* @brief                :invokes igmp server
* @scenario             :invokes igmp server
* @API's covered        :none
* @Preconditions        :none
* @Postconditions       :none
* @return               :void*
*/
static void* server(void *args)
{
	tc_net_igmp_server();
	return NULL;
}

/**
* @fn                   :Client
* @brief                :invokes igmp client.
* @scenario             :invokes igmp client
* @API's covered        :none
* @Preconditions        :none
* @Postconditions       :none
* @return               :void*
*/
static void* client(void *args)
{
	tc_net_igmp_client();
	return NULL;
}

/**
* @fn                   :net_igmp
* @brief                :create client and server thread.
* @scenario             :create client and server thread to test igmp api.
* @API's covered        :none
* @Preconditions        :none
* @Postconditions       :none
* @return               :void
*/
void net_igmp(void)
{
	pthread_t Server, Client;

	pthread_create(&Server, NULL, server, NULL);
	pthread_create(&Client, NULL, client, NULL);

	pthread_join(Server, NULL);
	pthread_join(Client, NULL);
}

/****************************************************************************
 * Name: net_igmp_main
 ****************************************************************************/

int net_igmp_main(void)
{
	net_igmp();
	return 0;
}
