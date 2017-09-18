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

/// @file tc_net_tcp.c
/// @brief Test Case Example for tcp API
#include <net/lwip/api.h>
#include <net/lwip/api_msg.h>
#include <net/lwip/ipv4/ip_addr.h>
#include <net/lwip/err.h>
#include <net/lwip/tcp.h>
#include <net/lwip/sockets.h>
#include <net/lwip/pbuf.h>

#include "tc_internal.h"
/**
* @testcase             : tc_tcp_rexmit
* @brief                : Requeue the first unacked segment for retransmission.
* @scenario             :
* @apicovered           : tcp_rexmit, tcp_new
* @precondition         : none
* @postcondition        : none
*/
void tc_tcp_rexmit(void)
{
	struct tcp_pcb *p;

	p = tcp_new();
	tcp_rexmit(p);
	TC_SUCCESS_RESULT();
}

/**
* @testcase             : tc_pbuf_free_callback_p
* @brief                : Function that allows you to free a pbuf from interrupt context.
* @scenario             :
* @apicovered           : pbuf_free_callback, pbuf_alloc, pbuf_free
* @precondition         : none
* @postcondition        : none
*/
void tc_pbuf_free_callback_p(void)
{
	struct pbuf *p = NULL;
	int ret;

	p = pbuf_alloc(PBUF_TRANSPORT, 6, PBUF_RAM);
	TC_ASSERT_NEQ("pbuf_alloc", p, NULL);
	ret = pbuf_free_callback(p);
	pbuf_free(p);
	TC_ASSERT_EQ("tc_pbuf_free_callback_p", ret, ZERO);
	TC_SUCCESS_RESULT();
}

/**
* @testcase             : tc_pbuf_free_callback_n
* @brief                : Function that allows you to free a pbuf from interrupt context.
* @scenario             :
* @apicovered           : tc_pbuf_free_callback, pbuf_alloc, pbuf_free
* @precondition         : none
* @postcondition        : none
*/
void tc_pbuf_free_callback_n(void)
{
	int ret = -1;
	ret = pbuf_free_callback(NULL);
	TC_ASSERT_EQ("pbuf_free_callback", ret, 0);
	TC_SUCCESS_RESULT();
}

/**
* @statitcase			: tc_net_tcp_accept_null_n
* @brief				:
* @scenario				:
* @apicovered			: tcp_accept_null
* @precondition			:
* @postcondition		:
*/
static void tc_net_tcp_accept_null_n(void)
{
	void *arg = NULL;
	struct tcp_pcb *pcb = NULL;
	err_t result;
	result = tcp_accept_null(arg, pcb, 0);
	TC_ASSERT_NEQ("tc_net_tcp_accept_null_n", result, ERR_ABRT);
	TC_SUCCESS_RESULT();
}

/**
* @statitcase           : tc_net_tcp_accept_null_p
* @brief                :
* @scenario             :
* @apicovered           : tcp_accept_null, malloc, free
* @precondition         : none
* @postcondition        : none
*/
static void tc_net_tcp_accept_null_p(void)
{
	void *arg = NULL;
	struct tcp_pcb *pcb = NULL;
	err_t result;

	pcb = (struct tcp_pcb *)malloc(sizeof(struct tcp_pcb));
	TC_ASSERT_NEQ("malloc", pcb, NULL);

	result = tcp_accept_null(arg, pcb, 0);
	TC_FREE_MEMORY(pcb);
	TC_ASSERT_EQ("tc_net_tcp_accept_null_p", result, NET_ERR_ABRT);
	TC_SUCCESS_RESULT();
}

/**
* @statitcase           : tc_net_tcp_process_refused_data_p
* @brief                :
* @scenario             :
* @apicovered           : tcp_process_refused_data, malloc, free
* @precondition         : none
* @postcondition        : none
*/
static void tc_net_tcp_process_refused_data_p(void)
{
	struct tcp_pcb *pcb;
	struct pbuf *refused_data;
	err_t result;

	pcb = (struct tcp_pcb *)malloc(sizeof(struct tcp_pcb));
	TC_ASSERT_NEQ("malloc", pcb, NULL);
	refused_data = pcb->refused_data;
	result = tcp_process_refused_data(pcb);
	TC_FREE_MEMORY(pcb);
	TC_ASSERT_EQ("tc_net_tcp_process_refused_data_p", result, ZERO);
	TC_SUCCESS_RESULT();
}

/**
* @statitcase           : tc_net_tcp_setprio_n
* @brief                : Sets the priority of a connection.
* @scenario             :
* @apicovered           : tcp_setprio, malloc, free
* @precondition         : none
* @postcondition        : none
*/

static void tc_net_tcp_setprio_n(void)
{
	struct tcp_pcb *pcb;
	u8_t prio = TCP_PRIO_MAX + 1;
	pcb = (struct tcp_pcb *)malloc(sizeof(struct tcp_pcb));
	TC_ASSERT_NEQ("malloc", pcb, NULL);

	tcp_setprio(pcb, prio);
	TC_FREE_MEMORY(pcb);
	TC_SUCCESS_RESULT();
}

/**
* @statitcase           : tc_net_tcp_setprio_p
* @brief                : Sets the priority of a connection.
* @scenario             :
* @apicovered           : tcp_setprio, malloc, free
* @precondition         : none
* @postcondition        : none
*/
static void tc_net_tcp_setprio_p(void)
{
	struct tcp_pcb *pcb;
	u8_t prio = TCP_PRIO_NORMAL;
	pcb = (struct tcp_pcb *)malloc(sizeof(struct tcp_pcb));
	TC_ASSERT_NEQ("malloc", pcb, NULL);

	tcp_setprio(pcb, prio);
	TC_FREE_MEMORY(pcb);
	TC_SUCCESS_RESULT();
}

/**
* @statitcase           : tc_net_tcp_kill_prio_p
* @brief                : Kills the oldest active connection that has the same or lower priority than prio.
* @scenario             :
* @apicovered           : tcp_kill_prio
* @precondition         : none
* @postcondition        : none
*/
static void tc_net_tcp_kill_prio_p(void)
{
	u8_t prio;
	prio = TCP_PRIO_NORMAL;
	tcp_kill_prio(prio);
	TC_SUCCESS_RESULT();
}

/**
* @statitcase           : tc_net_tcp_kill_prio_n
* @brief                : Kills the oldest active connection that has the same or lower priority than prio.
* @scenario             :
* @apicovered           : tcp_kill_prio
* @precondition         : none
* @postcondition        : none
*/
static void tc_net_tcp_kill_prio_n(void)
{
	u8_t prio;
	prio = TCP_PRIO_MIN - 1;
	tcp_kill_prio(prio);
	TC_SUCCESS_RESULT();
}

/**
* @statitcase           : tc_net_tcp_recv_null_n
* @brief                : Default receive callback that is called if the user didn't register a recv callback for the pcb.
* @scenario             :
* @apicovered           : tcp_recv_null
* @precondition         : none
* @postcondition        : none
*/
static void tc_net_tcp_recv_null_n(void)
{
	struct tcp_pcb *pcb = NULL;
	struct pbuf *p = NULL;
	err_t result;
	result = tcp_recv_null(NULL, pcb, p, NEG_VAL);
	TC_ASSERT_EQ("tc_net_tcp_recv_null_n", result, ZERO);
	TC_SUCCESS_RESULT();
}

/**
* @statitcase           : tc_net_tcp_recv_null_p
* @brief                : Default receive callback that is called if the user didn't register a recv callback for the pcb.
* @scenario             :
* @apicovered           : tcp_recv_null
* @precondition         : none
* @postcondition        : none
*/
static void tc_net_tcp_recv_null_p(void)
{
	struct tcp_pcb *pcb = NULL;
	struct pbuf *p = NULL;
	err_t result;
	pcb = (struct tcp_pcb *)malloc(sizeof(struct tcp_pcb));
	TC_ASSERT_NEQ("malloc", pcb, NULL);
	p = (struct pbuf *)malloc(sizeof(struct pbuf));
	TC_ASSERT_NEQ("malloc", p, NULL);

	result = tcp_recv_null(NULL, pcb, p, 0);
	TC_FREE_MEMORY(pcb);
	TC_FREE_MEMORY(p);
	TC_ASSERT_EQ("tc_net_tcp_recv_null_p", result, ZERO);
	TC_SUCCESS_RESULT();
}

/**
* @statitcase           : tcp_debug_state_str_p
* @brief                : Tells the connection state.
* @scenario             :
* @apicovered           : tcp_debug_state_str, strcmp
* @precondition         : none
* @postcondition        : none
*/
static void tc_net_tcp_debug_state_str_p(void)
{
	const char *result;
	int ret;
	result = tcp_debug_state_str(CLOSED);
	ret = strcmp(result, "CLOSED");
	TC_ASSERT_EQ("tc_net_tcp_debug_state_str_p", ret, ZERO);
	TC_SUCCESS_RESULT();
}

/**
* @statitcase           : tc_net_tcp_debug_state_str_n
* @brief                : Tells the connection state.
* @scenario             :
* @apicovered           : tcp_debug_state_str, strcmp
* @precondition         : none
* @postcondition        : none
*/
static void tc_net_tcp_debug_state_str_n(void)
{
	const char *result;
	int ret;
	result = tcp_debug_state_str(CLOSED);
	ret = strcmp(result, "LISTEN");
	TC_ASSERT_NEQ("tc_net_tcp_debug_state_str_n", ret, ZERO);
	TC_SUCCESS_RESULT();
}

/**
* @statitcase           : net_tcp_main
* @brief                :
* @scenario             :
* @apicovered           :
* @precondition         :
* @postcondition        :
*/
int net_tcp_main(void)
{
	tc_net_tcp_accept_null_n();
	tc_net_tcp_accept_null_p();
	tc_net_tcp_process_refused_data_p();
	tc_net_tcp_setprio_p();
	tc_net_tcp_setprio_n();
	tc_net_tcp_kill_prio_p();
	tc_net_tcp_kill_prio_n();
	tc_net_tcp_recv_null_n();
	tc_net_tcp_recv_null_p();
	tc_net_tcp_debug_state_str_p();
	tc_net_tcp_debug_state_str_n();
	tc_tcp_rexmit();
	tc_pbuf_free_callback_p();
	tc_pbuf_free_callback_n();
	return 0;
}
