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

/// @file tc_net_tcpip.c
/// @brief Test Case Example for tcpip API
#include <net/lwip/api.h>
#include <net/lwip/timers.h>

#include "tc_internal.h"

/**
* @statitcase				: tc_net_tcpip_timeout_p
* @brief					:
* @scenario					:
* @apicovered				: tcpip_timeout
* @precondition				:
* @postcondition			:
*/
static void tc_net_tcpip_timeout_p(void)
{
	u32_t msecs = 10;
	void *arg = NULL;
	err_t result;

	result = tcpip_timeout(msecs, sys_timeout, arg);
	TC_ASSERT_EQ("tcpip_timeout", result, ERR_OK);
	TC_SUCCESS_RESULT();
}

/**
* @statitcase				: tc_net_tcpip_timeout_n
* @brief					:
* @scenario					:
* @apicovered				: tcpip_timeout
* @precondition				:
* @postcondition			:
*/
static void tc_net_tcpip_timeout_n(void)
{
	u32_t msecs = 10;
	void *arg = NULL;
	err_t result;

	result = tcpip_timeout(msecs, NULL, arg);
	TC_ASSERT_EQ("tcpip_timeout", result, ERR_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @statitcase				: tc_net_tcpip_untimeout_p
* @brief					:
* @scenario					:
* @apicovered				: tcpip_untimeout
* @precondition				:
* @postcondition			:
*/
static void tc_net_tcpip_untimeout_p(void)
{
	void *arg = NULL;
	err_t result;

	result = tcpip_untimeout(sys_untimeout, arg);
	TC_ASSERT_EQ("tcpip_untimeout", result, ERR_OK);
	TC_SUCCESS_RESULT();
}

/**
* @statitcase				: tc_net_tcpip_untimeout_n
* @brief					:
* @scenario					:
* @apicovered				: tcpip_untimeout
* @precondition				:
* @postcondition			:
*/
static void tc_net_tcpip_untimeout_n(void)
{
	void *arg = NULL;
	err_t result;

	result = tcpip_untimeout(NULL, arg);
	TC_ASSERT_EQ("tcpip_untimeout", result, ERR_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @statitcase				: tc_net_tcpip_callbackmsg_new_p
* @brief					:
* @scenario					:
* @apicovered				: tcpip_callbackmsg_new
* @precondition				:
* @postcondition			:
*/
void dummy_function(void *ctx)
{
	printf("This is dummy function\n");
}
static void tc_net_tcpip_callbackmsg_new_p(void)
{
	struct tcpip_callback_msg *result;
	result = (struct tcpip_callback_msg *)tcpip_callbackmsg_new(dummy_function, NULL);
	TC_ASSERT_NEQ("tc_net_tcpip_callbackmsg_new_p", result, NULL);
	TC_SUCCESS_RESULT();
}
/**
* @statitcase				: tc_net_tcpip_callbackmsg_new_n
* @brief					:
* @scenario					:
* @apicovered				: tcpip_callbackmsg_new
* @precondition				:
* @postcondition			:
*/
static void tc_net_tcpip_callbackmsg_new_n(void)
{
	struct tcpip_callback_msg *result;

	result = (struct tcpip_callback_msg *)tcpip_callbackmsg_new(NULL, NULL);
	TC_ASSERT_EQ("tc_net_tcpip_callbackmsg_new_n", result, NULL);
	TC_SUCCESS_RESULT();
}

/**
* @statitcase				: tc_net_tcpip_callbackmsg_delete_p
* @brief					:
* @scenario					:
* @apicovered				: tcpip_callbackmsg_delete
* @precondition				:
* @postcondition			:
*/
static void tc_net_tcpip_callbackmsg_delete_p(void)
{
	struct tcpip_callback_msg *msg;

	msg = (struct tcpip_callback_msg *)tcpip_callbackmsg_new(dummy_function, NULL);
	tcpip_callbackmsg_delete(msg);
	TC_SUCCESS_RESULT();
}

/**
* @statitcase				: tc_net_tcpip_callbackmsg_delete_n
* @brief					:
* @scenario					:
* @apicovered				: tcpip_callbackmsg_delete
* @precondition				:
* @postcondition			:
*/
static void tc_net_tcpip_callbackmsg_delete_n(void)
{
	struct tcpip_callback_msg *msg;

	msg = (struct tcpip_callback_msg *)tcpip_callbackmsg_new(NULL, NULL);
	tcpip_callbackmsg_delete(msg);
	TC_SUCCESS_RESULT();
}

/**
* @testcase					: tc_mem_free_callback_n
* @brief					:
* @scenario					:
* @apicovered				: mem_free_callback
* @precondition				:
* @postcondition			:
*/
static void tc_mem_free_callback_n(void)
{
	int *p = NULL;
	int ret = -1;

	ret = mem_free_callback(p);
	TC_ASSERT_NEQ("mem_free_callback_n", ret, 0);
	TC_SUCCESS_RESULT();
}

/**
* @testcase					: tc_mem_free_callback_p
* @brief					:
* @scenario					:
* @apicovered				: mem_free_callback
* @precondition				:
* @postcondition			:
*/
static void tc_mem_free_callback_p(void)
{
	int *p = NULL;
	int ret = -1;

	p = (int *)malloc(sizeof(int) * 8);
	ret = mem_free_callback(p);
	TC_ASSERT_EQ("mem_free_callback_p", ret, 0);
	TC_SUCCESS_RESULT();
}

/**
* @testcase					: tc_pbuf_free_int_p
* @brief					:
* @scenario					:
* @apicovered				: pbuf_free_int
* @precondition				:
* @postcondition			:
*/
void tc_pbuf_free_int_p(void)
{
	struct pbuf *p = NULL;

	p = pbuf_alloc(PBUF_TRANSPORT, 6, PBUF_RAM);
	pbuf_free_int(p);
	TC_SUCCESS_RESULT();
}

/**
* @testcase					: tc_tcpip_trycallback
* @brief					:
* @scenario					:
* @apicovered				: tcpip_trycallback
* @precondition				:
* @postcondition			:
*/
void dummy1_function(void *arg){
	printf("Dummy callback function to test tcp_kill_timewait\n");
	return;
}

void tc_tcpip_trycallback(void)
{
	int ret = -1;
	struct tcpip_callback_msg *msg = (struct tcpip_callback_msg *)tcpip_callbackmsg_new(dummy1_function, NULL);

	ret = tcpip_trycallback(msg);
	TC_ASSERT_EQ("tcpip_trycallback", ret, 0);
	TC_SUCCESS_RESULT();
}

/**
* @statitcase				: net_api_tcpip_main
* @brief					:
* @scenario					:
* @apicovered				:
* @precondition				:
* @postcondition			:
*/
int net_tcpip_main(void)
{
	tc_net_tcpip_timeout_p();
	tc_net_tcpip_timeout_n();
	tc_net_tcpip_untimeout_p();
	tc_net_tcpip_untimeout_n();
	tc_net_tcpip_callbackmsg_new_p();
	tc_net_tcpip_callbackmsg_new_n();
	tc_net_tcpip_callbackmsg_delete_p();
	tc_net_tcpip_callbackmsg_delete_n();
	tc_pbuf_free_int_p();
	tc_mem_free_callback_p();
	tc_mem_free_callback_n();
	tc_tcpip_trycallback();
	return 0;
}
