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

/// @file tc_net_mac.c
/// @brief Test Case Example for ethernet and socket APIs

#include "tc_internal.h"
#include <stdio.h>
#include <net/lwip/err.h>
#include <net/lwip/sockets.h>
#include <tinyara/net/net.h>
#include <tinyara/net/ethernet.h>

/**
* @statitcase            : tc_net_checksd_p
* @brief                 : check if the socket descriptor is valid for the provided TCB and if it supports the requested access.
* @scenario              : none
* @apicovered            : net_checksd
* @precondition          : none
* @postcondition         : none
* @return                : void
*/
static void tc_net_checksd_p(int sock)
{
	int result;

	result = net_checksd(sock, 0666);
	TC_ASSERT_EQ("net_checksd_p", result, ZERO);
	TC_SUCCESS_RESULT();
}

/**
* @statitcase            : tc_net_checksd_n
* @brief                 : check if the socket descriptor is valid for the provided TCB and if it supports the requested access.
* @scenario              : none
* @apicovered            : net_checksd
* @precondition          : none
* @postcondition         : none
* @return                : void
*/
static void tc_net_checksd_n(void)
{
	int result;

	result = net_checksd(NEG_VAL, FLAGS);
	TC_ASSERT_EQ("tc_net_checksd_n", result, NET_EBADF);
	TC_SUCCESS_RESULT();
}

/**
* @statitcase            : tc_net_clone_p
* @brief                 : performs the low level, common portion of net_dupsd() and net_dupsd2().
* @scenario              : none
* @apicovered            : net_clone, socket, get_socket
* @precondition          : socket file descriptor.
* @postcondition         : none
* @return                : void
*/
static void tc_net_clone_p(int sockfd1, int sockfd2)
{
	int result;
	struct socket *sock1 = NULL;
	struct socket *sock2 = NULL;

	sock1 = get_socket(sockfd1);
	TC_ASSERT_NEQ("get_socket", sock1, NULL);
	sock2 = get_socket(sockfd2);
	TC_ASSERT_NEQ("get_socket", sock2, NULL);

	result = net_clone(sock1, sock2);
	TC_ASSERT_EQ("tc_net_clone_p", result, ZERO);
	TC_SUCCESS_RESULT();
}

/**
* @statitcase            : tc_net_dupsd2_p
* @brief                 : clone a socket descriptor to an arbitray descriptor number.
* @scenario              : none
* @apicovered            : net_dupsd2
* @precondition          : socket file descriptor.
* @postcondition         : none
* @return                : void
*/
static void tc_net_dupsd2_p(int old_fd, int new_fd)
{
	int result;

	result = net_dupsd2(old_fd, new_fd);
	TC_ASSERT_EQ("tc_net_dupsd2_p", result, ZERO);
	TC_SUCCESS_RESULT();
}

/**
* @statitcase            : tc_net_dupsd2_n
* @brief                 : clone a socket descriptor to an arbitray descriptor number.
* @scenario              : none
* @apicovered            : net_dupsd2
* @precondition          : socket file descriptors.
* @postcondition         : none
* @return                : void
*/
static void tc_net_dupsd2_n(int new_fd)
{
	int result;

	result = net_dupsd2(NEG_VAL, new_fd);
	TC_ASSERT_EQ("tc_net_dupsd2_n", result, NEG_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @statitcase            : tc_net_ethernetif_status_callback_p
* @brief                 : prints the IP address in dotted decimal format
* @scenario              : none
* @apicovered            : ethernetif_status_callback
* @precondition          : none
* @postcondition         : none
* @return                : void
*/
static void tc_net_ethernetif_status_callback_p(void)
{
	struct netif *netif = NULL;

	netif = (struct netif *)malloc(sizeof(struct netif));
	TC_ASSERT_NEQ("malloc", netif, NULL);

	ethernetif_status_callback(netif);
	TC_FREE_MEMORY(netif);
	TC_SUCCESS_RESULT();
}

/**
* @statitcase            : tc_net_ethernetif_init
* @brief                 : set up the network interface at the beginning.
* @scenario              : none
* @apicovered            : ethernetif_init
* @precondition          : none
* @postcondition         : none
* @return                : void
*/
static void tc_net_ethernetif_init(void)
{
	struct netif *netif;
	err_t result;

	netif = (struct netif *)malloc(sizeof(struct netif));
	TC_ASSERT_NEQ("malloc", netif, NULL);

	result = ethernetif_init(netif);
	TC_FREE_MEMORY(netif);
	TC_ASSERT_EQ("tc_net_ethernetif_init", result, ZERO);
	TC_SUCCESS_RESULT();
}

/****************************************************************************
 * Name: net_mac_main()
 ****************************************************************************/
int net_mac_main(int sock, int sock1)
{
	tc_net_checksd_p(sock);
	tc_net_checksd_n();
	tc_net_clone_p(sock, sock1);
	tc_net_dupsd2_p(sock, sock1);
	tc_net_dupsd2_n(sock1);
	tc_net_ethernetif_status_callback_p();
	tc_net_ethernetif_init();
	return 0;
}
