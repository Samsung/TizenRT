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
#include <net/lwip/api_msg.h>
#include <net/lwip/ipv4/ip_addr.h>
#include <net/lwip/err.h>
#include <net/lwip/sockets.h>
#include <net/lwip/timers.h>
#include <net/lwip/tcpip.h>
#include <net/lwip/pbuf.h>

#include "tc_internal.h"

/**
* @statitcase               : tc_net_tcpip_timeout_p
* @brief                    : call sys_timeout in tcpip_thread.
* @scenario                 :
* @apicovered               : tcpip_timeout
* @precondition             : none
* @postcondition            : none
*/
static void tc_net_tcpip_timeout_p(void)
{
	void *arg = NULL;
	err_t result;

	result = tcpip_timeout(TIME_MS, (sys_timeout_handler) sys_timeout, arg);
	TC_ASSERT_EQ("tc_net_tcpip_timeout_p", result, ZERO);
	TC_SUCCESS_RESULT();
}

/**
* @statitcase               : tc_net_tcpip_timeout_n
* @brief                    : call sys_timeout in tcpip_thread.
* @scenario                 :
* @apicovered               : tcpip_timeout
* @precondition             : none
* @postcondition            : none
*/
static void tc_net_tcpip_timeout_n(void)
{
	void *arg = NULL;
	err_t result;

	result = tcpip_timeout(TIME_MS, NULL, arg);
	TC_ASSERT_EQ("tc_net_tcpip_timeout_n", result, NET_ERR_VAL);
	TC_ASSERT_EQ("tc_net_tcpip_timeout_n", result, NEG_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @statitcase               : tc_net_tcpip_untimeout_p
* @brief                    : call sys_untimeout in tcpip_thread
* @scenario                 :
* @apicovered               : tcpip_untimeout
* @precondition             : none
* @postcondition            : none
*/
static void tc_net_tcpip_untimeout_p(void)
{
	void *arg = NULL;
	err_t result;

	result = tcpip_untimeout((sys_timeout_handler) sys_untimeout, arg);
	TC_ASSERT_EQ("tc_net_tcpip_untimeout_p", result, ZERO);
	TC_SUCCESS_RESULT();
}

/**
* @statitcase               : tc_net_tcpip_untimeout_n
* @brief                    : call sys_untimeout in tcpip_thread
* @scenario                 :
* @apicovered               : tcpip_untimeout
* @precondition             : none
* @postcondition            : none
*/
static void tc_net_tcpip_untimeout_n(void)
{
	void *arg = NULL;
	err_t result;

	result = tcpip_untimeout(NULL, arg);
	TC_ASSERT_EQ("tc_net_tcpip_untimeout_n", result, NET_ERR_VAL);
	TC_ASSERT_EQ("tc_net_tcpip_untimeout_n", result, NEG_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @statitcase               : tc_net_tcpip_callbackmsg_new_p
* @brief                    : Allocate a structure for a static callback message and initialize it.
* @scenario                 : used to send "static" messages from interrupt context.
* @apicovered               : tcpip_callbackmsg_new
* @precondition             : none
* @postcondition            : none
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
* @statitcase               : tc_net_tcpip_callbackmsg_delete_p
* @brief                    : Free a callback message allocated by tcpip_callbackmsg_new().
* @scenario                 :
* @apicovered               : tcpip_callbackmsg_delete, tcpip_callbackmsg_new
* @precondition             : none
* @postcondition            : none
*/
static void tc_net_tcpip_callbackmsg_delete_p(void)
{
	struct tcpip_callback_msg *msg;

	msg = (struct tcpip_callback_msg *)tcpip_callbackmsg_new(dummy_function, NULL);
	tcpip_callbackmsg_delete(msg);
	TC_SUCCESS_RESULT();
}

/**
* @testcase                 : tc_mem_free_callback_n
* @brief                    : allows you to free heap memory from interrupt context.
* @scenario                 :
* @apicovered               : mem_free_callback
* @precondition             : none
* @postcondition            : none
*/
static void tc_mem_free_callback_n(void)
{
	int *p = NULL;
	int result;

	result = mem_free_callback(p);
	TC_ASSERT_NEQ("mem_free_callback_n", result, NEG_VAL);
	TC_ASSERT_NEQ("mem_free_callback_n", result, NET_ERR_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase                 : tc_mem_free_callback_p
* @brief                    : allows you to free heap memory from interrupt context.
* @scenario                 :
* @apicovered               : mem_free_callback, malloc, free
* @precondition             : none
* @postcondition            : none
*/
static void tc_mem_free_callback_p(void)
{
	int *p = NULL;
	err_t result;
	p = (int *)malloc(sizeof(int));

	result = mem_free_callback(p);
	TC_FREE_MEMORY(p);
	TC_ASSERT_EQ("mem_free_callback_p", result, ZERO);
	TC_SUCCESS_RESULT();
}

/**
* @testcase                 : tc_pbuf_free_int_p
* @brief                    : Allocates a pbuf of the given type
* @scenario                 :
* @apicovered               : pbuf_free_int, pbuf_alloc, pbuf_free
* @precondition             : none
* @postcondition            : none
*/
void tc_pbuf_free_int_p(void)
{
	struct pbuf *p = NULL;

	p = pbuf_alloc(PBUF_TRANSPORT, 6, PBUF_RAM);
	pbuf_free_int(p);
	pbuf_free(p);
	TC_SUCCESS_RESULT();
}

/**
* @testcase                 : tc_tcpip_trycallback
* @brief                    : Try to post a callback-message to the tcpip_thread mbox.
* @scenario                 : used to send "static" messages from interrupt context.
* @apicovered               : tcpip_trycallback
* @precondition             : none
* @postcondition            : none
*/
void dummy1_function(void *arg)
{
	printf("Dummy callback function to test tcp_kill_timewait\n");
	return;
}

void tc_tcpip_trycallback(void)
{
	int ret;
	struct tcpip_callback_msg *msg = (struct tcpip_callback_msg *)tcpip_callbackmsg_new(dummy1_function, NULL);

	ret = tcpip_trycallback(msg);
	TC_ASSERT_EQ("tcpip_trycallback", ret, ZERO);
	TC_SUCCESS_RESULT();
}

/**
* @statitcase               : net_api_tcpip_main
* @brief                    :
* @scenario                 :
* @apicovered               :
* @precondition             :
* @postcondition            :
*/
int net_tcpip_main(void)
{
	tc_net_tcpip_timeout_p();
	tc_net_tcpip_timeout_n();
	tc_net_tcpip_untimeout_p();
	tc_net_tcpip_untimeout_n();
	tc_net_tcpip_callbackmsg_new_p();
	tc_net_tcpip_callbackmsg_delete_p();
	tc_pbuf_free_int_p();
	tc_mem_free_callback_p();
	tc_mem_free_callback_n();
	tc_tcpip_trycallback();
	return 0;
}
