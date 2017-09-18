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
#include <net/lwip/tcp.h>

#include "tc_internal.h"
/**
* @testcase				: tc_tcp_rexmit
* @brief				:
* @scenario				:
* @apicovered			: tcp_rexmit
* @precondition			:
* @postcondition		:
*/
void tc_tcp_rexmit(void)
{
	struct tcp_pcb *p;

	p = tcp_new();
	tcp_rexmit(p);
	TC_SUCCESS_RESULT();
}

/**
* @testcase				: tc_pbuf_free_callback_p
* @brief				:
* @scenario				:
* @apicovered			: pbuf_free_callback_p
* @precondition			:
* @postcondition		:
*/
void tc_pbuf_free_callback_p(void)
{
	struct pbuf *p = NULL;
	int ret = -1;

	p = pbuf_alloc(PBUF_TRANSPORT, 6, PBUF_RAM);
	ret = pbuf_free_callback(p);
	TC_ASSERT_EQ("pbuf_free_callback", ret, 0);
	TC_SUCCESS_RESULT();
}

/**
* @testcase				: tc_pbuf_free_callback_n
* @brief				:
* @scenario				:
* @apicovered			: tc_pbuf_free_callback_n
* @precondition			:
* @postcondition		:
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
* @statitcase			: tc_net_tcp_accept_null_p
* @brief				:
* @scenario				:
* @apicovered			: tcp_accept_null
* @precondition			:
* @postcondition		:
*/
static void tc_net_tcp_accept_null_p(void)
{
	void *arg = NULL;
	struct tcp_pcb *pcb = NULL;
	err_t result;

	if (!(pcb = (struct tcp_pcb *)malloc(sizeof(struct tcp_pcb))))
		return;
	result = tcp_accept_null(arg, pcb, 0);
	TC_ASSERT_EQ("tc_net_tcp_accept_null_p", result, ERR_ABRT);
	TC_SUCCESS_RESULT();
}

/**
* @statitcase			: tc_net_tcp_process_refused_data_p
* @brief				:
* @scenario				:
* @apicovered			: tcp_process_refused_data
* @precondition			:
* @postcondition		:
*/
static void tc_net_tcp_process_refused_data_p(void)
{
	struct tcp_pcb *pcb;
	struct pbuf *refused_data;
	err_t result;

	if (!(pcb = (struct tcp_pcb *)malloc(sizeof(struct tcp_pcb))))
		return ;
	refused_data = pcb->refused_data;
	result = tcp_process_refused_data(pcb);
	TC_ASSERT_EQ("tc_net_tcp_process_refused_data_p", result, ERR_OK);
	TC_SUCCESS_RESULT();
}

/**
* @statitcase			: tc_net_tcp_setprio_n
* @brief				:
* @scenario				:
* @apicovered			: tcp_setprio
* @precondition			:
* @postcondition		:
*/

static void tc_net_tcp_setprio_n(void)
{
	struct tcp_pcb *pcb;
	u8_t prio = 129;

	if (!(pcb = (struct tcp_pcb *)malloc(sizeof(struct tcp_pcb))))
		return;
	tcp_setprio(pcb, prio);
	TC_SUCCESS_RESULT();
}

/**
* @statitcase			: tc_net_tcp_setprio_p
* @brief				:
* @scenario				:
* @apicovered			: tcp_setprio
* @precondition			:
* @postcondition		:
*/
static void tc_net_tcp_setprio_p(void)
{
	struct tcp_pcb *pcb;
	u8_t prio = 5;
	if (!(pcb = (struct tcp_pcb *)malloc(sizeof(struct tcp_pcb))))
		return ;
	tcp_setprio(pcb, prio);
	TC_SUCCESS_RESULT();
}

/**
* @statitcase			: tc_net_tcp_kill_prio_p
* @brief				:
* @scenario				:
* @apicovered			: tcp_kill_prio
* @precondition			:
* @postcondition		:
*/
static void tc_net_tcp_kill_prio_p(void)
{
	u8_t prio;
	prio = 5;
	tcp_kill_prio(prio);
	TC_SUCCESS_RESULT();
}

/**
* @statitcase			: tc_net_tcp_kill_prio_n
* @brief				:
* @scenario				:
* @apicovered			: tcp_kill_prio
* @precondition			:
* @postcondition		:
*/
static void tc_net_tcp_kill_prio_n(void)
{
	u8_t prio;
	prio = 0;
	tcp_kill_prio(prio);
	TC_SUCCESS_RESULT();
}

/**
* @statitcase			: tc_net_tcp_recv_null_n
* @brief				:
* @scenario				:
* @apicovered			: tcp_recv_null
* @precondition			:
* @postcondition		:
*/
static void tc_net_tcp_recv_null_n(void)
{
	struct tcp_pcb *pcb = NULL;
	struct pbuf *p = NULL;
	err_t result;
	result = tcp_recv_null(NULL, pcb, p, 0);
	TC_ASSERT_EQ("tc_net_tcp_recv_null_n", result, ERR_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @statitcase			: tc_net_tcp_recv_null_p
* @brief				:
* @scenario				:
* @apicovered			: tcp_recv_null
* @precondition			:
* @postcondition		:
*/
static void tc_net_tcp_recv_null_p(void)
{
	struct tcp_pcb *pcb = NULL;
	struct pbuf *p = NULL;
	err_t result;
	if (!(pcb = (struct tcp_pcb *)malloc(sizeof(struct tcp_pcb))))
		return;
	if (!(p = (struct pbuf *)malloc(sizeof(struct pbuf))))
		return;
	result = tcp_recv_null(NULL, pcb, p, 0);
	TC_ASSERT_EQ("tc_net_tcp_recv_null_p", result, ERR_OK);
	TC_SUCCESS_RESULT();
}

/**
* @statitcase			: tcp_kill_timewait_p
* @brief				:
* @scenario				:
* @apicovered			: tcp_kill_timewait
* @precondition			:
* @postcondition		:
*/
static void tc_net_tcp_kill_timewait_p(void)
{
	tcp_kill_timewait();
	TC_SUCCESS_RESULT();
}

/**
* @statitcase			: tcp_debug_state_str_p
* @brief				:
* @scenario				:
* @apicovered			: tcp_debug_state_str
* @precondition			:
* @postcondition		:
*/
static void tc_net_tcp_debug_state_str_p(void)
{
	const char *result;
	result = tcp_debug_state_str(0);
	TC_ASSERT_EQ("tc_net_tcp_debug_state_str_p", result, "CLOSED");
	TC_SUCCESS_RESULT();
}

/**
* @statitcase			: tc_net_tcp_debug_state_str_n
* @brief				:
* @scenario				:
* @apicovered			: tcp_debug_state_str
* @precondition			:
* @postcondition		:
*/
static void tc_net_tcp_debug_state_str_n(void)
{
	const char *result;
	result = tcp_debug_state_str(0);
	TC_ASSERT_NEQ("tc_net_tcp_debug_state_str_n", result, "LISTEN");
	TC_SUCCESS_RESULT();
}

/**
* @statitcase			: net_tcp_main
* @brief				:
* @scenario				:
* @apicovered			:
* @precondition			:
* @postcondition		:
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
	tc_net_tcp_kill_timewait_p();
	tc_net_tcp_debug_state_str_p();
	tc_net_tcp_debug_state_str_n();
	tc_tcp_rexmit();
	tc_pbuf_free_callback_p();
	tc_pbuf_free_callback_n();
	return 0;
}
