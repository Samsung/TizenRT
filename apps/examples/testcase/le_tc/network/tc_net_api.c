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

#define PORT_NUMBER		2000

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
	ip_addr_t addr;
	u16_t port = PORT_NUMBER;
	IP4_ADDR(&addr, 127, 0, 0, 1);
	err_t result;

	result = netconn_sendto(conn, buf, &addr, port);
	TC_ASSERT_EQ("tc_net_netconn_sendto_n", result, NEG_SIX);
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
	TC_ASSERT_EQ("tc_net_netconn_close_n", result, NEG_SIX);
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
	int sock_tcp = socket(AF_INET, SOCK_STREAM, 0);
	struct socket *sock;
	err_t result;
	sock = get_socket(sock_tcp);
	TC_ASSERT_NEQ("get_socket", sock, NULL);

	result = netconn_close(sock->conn);
	TC_ASSERT_EQ("tc_net_netconn_close_p", result, ZERO);
	close(sock_tcp);
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
	TC_ASSERT_EQ("tc_net_netconn_disconnect_n", result, NEG_SIX);
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
	int ret;

	ip_addr_t multiaddr;
	ip_addr_t netif_addr;

	ret = ipaddr_aton("239.255.255.255", &multiaddr);
	TC_ASSERT_NEQ("ipaddr_aton", ret, ONE);

	ret = ipaddr_aton("127.0.0.1", &netif_addr);
	TC_ASSERT_NEQ("ipaddr_aton", ret, ONE);

	result = netconn_join_leave_group(conn, &multiaddr, &netif_addr, NETCONN_JOIN);	// 0 -> join, 1 -> leave

	TC_ASSERT_EQ("tc_net_netconn_join_leave_group_n", result, NEG_FOURTEEN);
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
	err_t result;
	struct api_msg_msg *msg = NULL;
	msg = (struct api_msg_msg *)malloc(sizeof(struct api_msg_msg));
	TC_ASSERT_NEQ("malloc", msg, NULL);
	msg->conn->type = NETCONN_UDP;

	do_disconnect(msg);
	result = msg->err;
	TC_FREE_MEMORY(msg);
	TC_ASSERT_EQ("tc_net_do_disconnect_p", result, ZERO);
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
	err_t result;
	struct api_msg_msg *msg;
	msg = (struct api_msg_msg *)malloc(sizeof(struct api_msg_msg));
	TC_ASSERT_NEQ("malloc", msg, NULL);
	msg->conn->type = NETCONN_TCP;

	do_disconnect(msg);
	result = msg->err;
	TC_FREE_MEMORY(msg);
	TC_ASSERT_EQ("tc_net_do_disconnect_n", result, NET_ERR_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @statitcase			: tc_net_recv_raw
* @brief				:
* @scenario				:
* @apicovered			: recv_raw
* @precondition			:
* @postcondition		:
*/
static void tc_net_recv_raw(void)
{
	struct pbuf *p = NULL;
	struct raw_pcb *pcb = NULL;
	ip_addr_t addr;
	u8_t result;
	void *arg = NULL;
	IP4_ADDR(&addr, 127, 0, 0, 1);
	p = (struct pbuf *)malloc(sizeof(struct pbuf));
	TC_ASSERT_NEQ("malloc", p, NULL);
	pcb = (struct raw_pcb *)malloc(sizeof(struct raw_pcb));
	TC_ASSERT_NEQ("malloc", pcb, NULL);

	result = recv_raw(arg, pcb, p, &addr);
	TC_FREE_MEMORY(p);
	TC_FREE_MEMORY(pcb);
	TC_ASSERT_EQ("tc_net_recv_raw", result, ZERO);
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
	struct api_msg_msg *msg;
	err_t result;
	msg = (struct api_msg_msg *)malloc(sizeof(struct api_msg_msg));
	TC_ASSERT_NEQ("malloc", msg, NULL);
	msg->conn->last_err = 0;
	msg->conn->pcb.tcp = NULL;

	do_join_leave_group(msg);
	result = msg->err;
	TC_FREE_MEMORY(msg);
	TC_ASSERT_EQ("tc_net_do_join_leave_group_n", result, NET_ERR_CONN);
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
	err_t result;

	msg = (struct api_msg_msg *)malloc(sizeof(struct api_msg_msg));
	TC_ASSERT_NEQ("malloc", msg, NULL);
	msg->conn->last_err = 0;
	msg->conn->pcb.tcp = (struct tcp_pcb *)malloc(sizeof(struct tcp_pcb));
	TC_ASSERT_NEQ("malloc", msg->conn->pcb.tcp, NULL);
	msg->conn->type = NETCONN_UDP;
	msg->msg.jl.join_or_leave = NETCONN_JOIN;

	do_join_leave_group(msg);
	result = msg->err;
	TC_FREE_MEMORY(msg->conn->pcb.tcp);
	TC_FREE_MEMORY(msg);
	TC_ASSERT_EQ("tc_net_do_join_leave_group_p", result, ZERO);
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
	tc_net_netconn_disconnect_n();
	tc_net_netconn_join_leave_group_n();
	tc_net_do_disconnect_n();
	tc_net_do_disconnect_p();
	tc_net_recv_raw();
	tc_net_do_join_leave_group_n();
	tc_net_do_join_leave_group_p();
	return 0;
}
