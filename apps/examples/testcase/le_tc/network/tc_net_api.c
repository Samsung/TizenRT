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

/**
* @statitcase			: tc_net_netconn_sendto_n
* @brief				:
* @scenario				:
* @apicovered			: netconn_sendto
* @precondition			:
* @postcondition		:
*/
static void tc_net_netconn_sendto_n(void)
{
	struct netconn *conn = NULL;
	struct netbuf *buf = NULL;
	ip_addr_t *addr = NULL;
	u16_t port = 0;
	err_t result;

	result = netconn_sendto(conn, buf, addr, port);
	TC_ASSERT_EQ("tc_net_netconn_sendto_n", result, ERR_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @statitcase			: tc_net_netconn_sendto_p
* @brief				:
* @scenario				:
* @apicovered			: netconn_sendto
* @precondition			:
* @postcondition		:
*/
static void tc_net_netconn_sendto_p(void)
{
	struct socket *sock;
	struct netbuf *buf = NULL;
	int sock_fd;
	ip_addr_t *addr;
	u16_t port;
	err_t result;

	addr = malloc(4);
	sock_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	sock = get_socket(sock_fd);
	if (sock == NULL) {
		return ;
	}
	addr->addr = (u32_t)"127.0.0.1";
	port = 2000;
	buf = (struct netbuf *)mem_trim(buf, 2 * sizeof(struct netbuf));
	result = netconn_sendto(sock->conn, buf, addr, port);
	TC_ASSERT_EQ("tc_net_netconn_sendto_p", result, ERR_OK);
	TC_SUCCESS_RESULT();
}

/**
* @statitcase			: tc_net_netconn_close_n
* @brief				:
* @scenario				:
* @apicovered			: netconn_close
* @precondition			:
* @postcondition		:
*/
static void tc_net_netconn_close_n(void)
{
	struct netconn *conn = NULL;
	err_t result;

	result = netconn_close(conn);
	TC_ASSERT_EQ("tc_net_netconn_close_n", result, ERR_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @statitcase			: tc_net_netconn_close_p
* @brief				:
* @scenario				:
* @apicovered			: netconn_close
* @precondition			:
* @postcondition		:
*/
static void tc_net_netconn_close_p(void)
{
	struct socket *sock;
	int sock_fd;
	err_t result;

	sock_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	sock = get_socket(sock_fd);
	if (sock == NULL) {
		return ;
	}
	result = netconn_close(sock->conn);
	TC_ASSERT_EQ("tc_net_netconn_close_p", result, ERR_OK);
	TC_SUCCESS_RESULT();
}

/**
* @statitcase			: tc_net_netconn_disconnect_n
* @brief				:
* @scenario				:
* @apicovered			: netconn_disconnect
* @precondition			:
* @postcondition		:
*/
static void tc_net_netconn_disconnect_n(void)
{
	struct netconn *conn = NULL;
	err_t result;

	result = netconn_disconnect(conn);
	TC_ASSERT_EQ("tc_net_netconn_disconnect_n", result, ERR_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @statitcase			: tc_net_netconn_disconnect_p
* @brief				:
* @scenario				:
* @apicovered			: netconn_disconnect
* @precondition			:
* @postcondition		:
*/
static void tc_net_netconn_disconnect_p(void)
{
	struct netconn *conn= NULL;
	err_t result;
	if (!(conn = (struct netconn *)malloc(sizeof(struct netconn))))
		return ;

	result = netconn_disconnect(conn);
	TC_ASSERT_EQ("tc_net_netconn_disconnect_p", result, ERR_OK);
	TC_SUCCESS_RESULT();
}

/**
* @statitcase			: tc_net_netconn_join_leave_group_n
* @brief				:
* @scenario				:
* @apicovered			: netconn_join_leave_group
* @precondition			:
* @postcondition		:
*/
static void tc_net_netconn_join_leave_group_n(void)
{
	struct netconn *conn = NULL;
	err_t result;
	ip_addr_t *multiaddr;
	ip_addr_t *netif_addr;
	multiaddr = malloc(4);
	netif_addr = malloc(4);
	multiaddr->addr = (u32_t)"239.255.255.255";
	netif_addr->addr = (u32_t)"127.0.0.1";

	result = netconn_join_leave_group(conn, multiaddr, netif_addr, 0);// 0 -> join, 1 -> leave
	TC_ASSERT_EQ("tc_net_netconn_join_leave_group_n", result, ERR_ARG);
	TC_SUCCESS_RESULT();
}

/**
* @statitcase			: tc_net_netconn_join_leave_group_p
* @brief				:
* @scenario				:
* @apicovered			: netconn_join_leave_group
* @precondition			:
* @postcondition		:
*/
static void tc_net_netconn_join_leave_group_p(void)
{
	struct netconn *conn = NULL;
	err_t result;
	ip_addr_t *multiaddr;
	ip_addr_t *netif_addr;
	multiaddr = malloc(4);
	netif_addr = malloc(4);
	multiaddr->addr = (u32_t)"239.255.255.255";
	netif_addr->addr = (u32_t)"127.0.0.1";
	if (!(conn = (struct netconn *)malloc(sizeof(struct netconn))))
		return;

	result = netconn_join_leave_group(conn, multiaddr, netif_addr, 0);// 0 -> join, 1 -> leave
	TC_ASSERT_EQ("tc_net_netconn_join_leave_group_p", result, ERR_OK);
	TC_SUCCESS_RESULT();
}

/**
* @statitcase			: tc_net_do_disconnect_p
* @brief				:
* @scenario				:
* @apicovered			: do_disconnect
* @precondition			:
* @postcondition		:
*/
static void tc_net_do_disconnect_p(void)
{
	struct api_msg_msg *msg;

	if (!(msg = malloc(sizeof(struct api_msg_msg))))
		return;
	msg->conn->type = 0x20;
	do_disconnect(msg);
	TC_SUCCESS_RESULT();
}

/**
* @statitcase			: tc_net_do_disconnect_n
* @brief				:
* @scenario				:
* @apicovered			: do_disconnect
* @precondition			:
* @postcondition		:
*/
static void tc_net_do_disconnect_n(void)
{
	struct api_msg_msg *msg = NULL;

	do_disconnect(msg);
	TC_SUCCESS_RESULT();
}

/**
* @statitcase			: tc_net_recv_raw_n
* @brief				:
* @scenario				:
* @apicovered			: recv_raw
* @precondition			:
* @postcondition		:
*/
static void tc_net_recv_raw_n(void)
{
	struct pbuf *p = NULL;
	struct raw_pcb *pcb = NULL;
	ip_addr_t *addr;
	u8_t result;
	void *arg = NULL;
	addr = malloc(4);
	addr->addr = (u32_t)"127.0.0.1";
	result = recv_raw(arg, pcb, p, addr);
	TC_ASSERT_NEQ("tc_net_recv_raw_n", result, 0);
	TC_SUCCESS_RESULT();
}

/**
* @statitcase			: tc_net_recv_raw_p
* @brief				:
* @scenario				:
* @apicovered			: recv_raw
* @precondition			:
* @postcondition		:
*/
static void tc_net_recv_raw_p(void)
{
	struct pbuf *p = NULL;
	struct raw_pcb *pcb = NULL;
	ip_addr_t *addr;
	u8_t result;
	void *arg = NULL;
	addr = malloc(4);
	addr->addr = (u32_t)"127.0.0.1";
	if (!(p = (struct pbuf *)malloc(sizeof(struct pbuf))))
		return;
	if (!(pcb = (struct raw_pcb *)malloc(sizeof(struct raw_pcb))))
		return;

	result = recv_raw(arg, pcb, p, addr);
	TC_ASSERT_EQ("tc_net_recv_raw_p", result, 0);
	TC_SUCCESS_RESULT();
}

/**
* @statitcase			: tc_net_do_join_leave_group_n
* @brief				:
* @scenario				:
* @apicovered			: do_join_leave_group
* @precondition			:
* @postcondition		:
*/
static void tc_net_do_join_leave_group_n(void)
{
	struct api_msg_msg *msg = NULL;

	do_join_leave_group(msg);
	TC_SUCCESS_RESULT();
}

/**
* @statitcase			: tc_net_do_join_leave_group_p
* @brief				:
* @scenario				:
* @apicovered			: do_join_leave_group
* @precondition			:
* @postcondition		:
*/
static void tc_net_do_join_leave_group_p(void)
{
	struct api_msg_msg *msg;

	if (!(msg = malloc(sizeof(struct api_msg_msg))))
		return;

	do_join_leave_group(msg);
	TC_SUCCESS_RESULT();
}

/**
* @statitcase			: net_api_main
* @brief				:
* @scenario				:
* @apicovered			:
* @precondition			:
* @postcondition		:
*/
int net_api_main(void)
{
	tc_net_netconn_close_n();
	tc_net_netconn_close_p();
	tc_net_netconn_sendto_n();
	tc_net_netconn_sendto_p();
	tc_net_netconn_disconnect_n();
	tc_net_netconn_disconnect_p();
	tc_net_netconn_join_leave_group_n();
	tc_net_netconn_join_leave_group_p();
	tc_net_do_disconnect_n();
	tc_net_do_disconnect_p();
	tc_net_recv_raw_n();
	tc_net_recv_raw_p();
	tc_net_do_join_leave_group_n();
	tc_net_do_join_leave_group_p();
	return 0;
}
