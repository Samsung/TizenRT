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
* @statitcase				: tc_net_checksd_p
* @brief					:
* @scenario					:   
* @apicovered				: net_checksd
* @precondition				:
* @postcondition			:
*/
static void tc_net_checksd_p(void)
{
	int result;
	
	result = net_checksd(3, 0666);
	TC_ASSERT_EQ("tc_net_checksd_p", result, 0);
	TC_SUCCESS_RESULT();
}

/**
* @statitcase				: tc_net_checksd_n
* @brief					:
* @scenario					:   
* @apicovered				: net_checksd
* @precondition				:
* @postcondition			:
*/
static void tc_net_checksd_n(void)
{
	int result;
	
	result = net_checksd(-1, 0666);
	TC_ASSERT_NEQ("tc_net_checksd_n", result, 0);
	TC_SUCCESS_RESULT();
}

/**
* @statitcase				: tc_net_clone_p
* @brief					:
* @scenario					:   
* @apicovered				: net_clone
* @precondition				:
* @postcondition			:
*/
static void tc_net_clone_p(void)
{
	int sockfd1;
	int sockfd2;
	int result;
	struct socket *sock1 = NULL;
	struct socket *sock2 = NULL;
	sockfd1 = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sockfd1 == -1) {
		return ;
	}
	sockfd2 = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sockfd2 == -1) {
		return ;
	}
	sock1 = get_socket(sockfd1);
	if (!sock1) {
		return ;
	}
	sock2 = get_socket(sockfd2);
	if (!sock2) {
		return ;
	}
	
	result = net_clone(sock1, sock2);
	TC_ASSERT_EQ("tc_net_clone_p", result, 0);
	TC_SUCCESS_RESULT();
}

/**
* @statitcase				: tc_net_clone_n
* @brief					:
* @scenario					:   
* @apicovered				: net_clone
* @precondition				:
* @postcondition			:
*/
static void tc_net_clone_n(void)
{
	struct socket *sock1 = NULL;
	struct socket *sock2 = NULL;
	int result;
	
	result = net_clone(sock1, sock2);
	TC_ASSERT_NEQ("tc_net_clone_n", result, 0);
	TC_SUCCESS_RESULT();
}

/**
* @statitcase				: tc_net_dupsd2_p
* @brief					:
* @scenario					:   
* @apicovered				: net_dupsd2
* @precondition				:
* @postcondition			:
*/
static void tc_net_dupsd2_p(void)
{
	int old_sockfd;
	int new_sockfd;
	int result;
	old_sockfd = 3;
	new_sockfd = 5;

	result = net_dupsd2(old_sockfd, new_sockfd);
	TC_ASSERT_EQ("tc_net_dupsd2_p", result, 0);
	TC_SUCCESS_RESULT();
}

/**
* @statitcase				: tc_net_dupsd2_n
* @brief					:
* @scenario					:   
* @apicovered				: net_dupsd2
* @precondition				:
* @postcondition			:
*/
static void tc_net_dupsd2_n(void)
{
	int old_sockfd;
	int new_sockfd;
	int result;
	old_sockfd = -1;
	new_sockfd = 5;

	result = net_dupsd2(old_sockfd, new_sockfd);
	TC_ASSERT_NEQ("tc_net_dupsd2_n", result, 0);
	TC_SUCCESS_RESULT();
}

/**
* @statitcase				: tc_net_ethernetif_status_callback_p
* @brief					:
* @scenario					:   
* @apicovered				: ethernetif_status_callback
* @precondition				:
* @postcondition			:
*/
static void tc_net_ethernetif_status_callback_p(void)
{
	struct netif *netif = NULL;
	if (!(netif = (struct netif *)malloc(sizeof(struct netif))))
		return;

	ethernetif_status_callback(netif);
	TC_SUCCESS_RESULT();
}

/**
* @statitcase				: tc_net_ethernetif_status_callback_n
* @brief					:
* @scenario					:   
* @apicovered				: ethernetif_status_callback
* @precondition				:
* @postcondition			:
*/
static void tc_net_ethernetif_status_callback_n(void)
{
	struct netif *netif = NULL;
	ethernetif_status_callback(netif);
	TC_SUCCESS_RESULT();
}

/**
* @statitcase				: tc_net_ethernetif_output_p
* @brief					:
* @scenario					:   
* @apicovered				: ethernetif_output
* @precondition				:
* @postcondition			:
*/
static void tc_net_ethernetif_output_p(void)
{
	struct netif *netif;
	struct pbuf *p;
	err_t result;
	if (!(netif = (struct netif *) malloc(sizeof(struct netif))))
		return ;
	if (!(p = (struct pbuf *) malloc(sizeof(struct pbuf))))
		return ;
	
	result = ethernetif_output(netif, p);
	TC_ASSERT_EQ("tc_net_ethernetif_output_p", result, 0);
	TC_SUCCESS_RESULT();
}

/**
* @statitcase				: tc_net_ethernetif_output_n
* @brief					:
* @scenario					:   
* @apicovered				: ethernetif_output
* @precondition				:
* @postcondition			:
*/
static void tc_net_ethernetif_output_n(void)
{
	struct netif *netif = NULL;	
	struct pbuf *p = NULL;
	err_t result;

	result = ethernetif_output(netif, p);
	TC_ASSERT_NEQ("tc_net_ethernetif_output_n", result, 0);
	TC_SUCCESS_RESULT();
}

/**
* @statitcase				: tc_net_ethernetif_init_p
* @brief					:
* @scenario					:   
* @apicovered				: ethernetif_init
* @precondition				:
* @postcondition			:
*/
static void tc_net_ethernetif_init_p(void)
{
	struct netif *netif;
	err_t result;
	if (!(netif = (struct netif *)malloc(sizeof(struct netif))))
		return ;

	result = ethernetif_init(netif);
	TC_ASSERT_EQ("tc_net_ethernetif_init_p", result, 0);
	TC_SUCCESS_RESULT();
}

/**
* @statitcase				: tc_net_ethernetif_init_n
* @brief					:
* @scenario					:   
* @apicovered				: ethernetif_init
* @precondition				:
* @postcondition			:
*/
static void tc_net_ethernetif_init_n(void)
{
	struct netif *netif = NULL;
	err_t result;
	
	result = ethernetif_init(netif);
	TC_ASSERT_NEQ("tc_net_ethernetif_init_n", result, 0);
	TC_SUCCESS_RESULT();
}

/**
* @statitcase				: net_tcp_main
* @brief					:
* @scenario					:   
* @apicovered				:
* @precondition				:
* @postcondition			:
*/
int net_mac_main(void)
{
	tc_net_checksd_p();
	tc_net_checksd_n();
	tc_net_clone_p();
	tc_net_clone_n();
	tc_net_dupsd2_p();
	tc_net_dupsd2_n();
	tc_net_ethernetif_status_callback_p();
	tc_net_ethernetif_status_callback_n();
	tc_net_ethernetif_output_p();
	tc_net_ethernetif_output_n();
	tc_net_ethernetif_init_p();
	tc_net_ethernetif_init_n();
	return 0;
}
