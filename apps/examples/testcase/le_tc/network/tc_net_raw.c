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
#include <net/lwip/ipv4/ip.h>

#include "tc_internal.h"

/**
* @testcase				: tc_net_raw_send_n
* @brief				:
* @scenario				:
* @apicovered			: raw_send
* @precondition			:
* @postcondition		:
*/
static void tc_net_raw_send_n(void)
{
	struct raw_pcb *raw = NULL;
	struct pbuf *pbuf = NULL;
	ip_addr_t loop_ipaddr;
	int ret;

	raw = (struct raw_pcb *)raw_new(IP_PROTO_TCP);
	IP4_ADDR(&loop_ipaddr, 127, 0, 0, 1);
	ret = raw_bind(raw, &loop_ipaddr);
	TC_ASSERT_EQ("raw_bind", ret, ZERO);
	ret = raw_send(raw, pbuf);
	TC_ASSERT_NEQ("raw_connect", ret, 0);
	TC_SUCCESS_RESULT();
}

/**
* @testcase				: tc_net_raw_connect_n
* @brief				:
* @scenario				:
* @apicovered			: raw_connect
* @precondition			:
* @postcondition		:
*/
static void tc_net_raw_connect_n(void)
{
	struct raw_pcb *raw = NULL;
	int ret;

	raw = (struct raw_pcb *)raw_new(IP_PROTO_TCP);
	ret = raw_connect(raw, NULL);
	raw_remove(raw);

	TC_ASSERT_EQ("raw_connect", ret, 0);
	TC_SUCCESS_RESULT();
}

/**
* @testcase				: tc_net_raw_connect_p
* @brief				:
* @scenario				:
* @apicovered			: raw_connect
* @precondition			:
* @postcondition		:
*/
static void tc_net_raw_connect_p(void)
{
	struct raw_pcb *raw = NULL;
	ip_addr_t loop_ipaddr;
	int ret;

	raw = (struct raw_pcb *)raw_new(IP_PROTO_TCP);
	IP4_ADDR(&loop_ipaddr, 127, 0, 0, 1);
	ret = raw_connect(raw, loop_ipaddr);

	raw_remove(raw);
	TC_ASSERT_EQ("raw_connect", ret, 0);
	TC_SUCCESS_RESULT();
}

/**
* @testcase				: tc_net_raw_bind_n
* @brief				:
* @scenario				:
* @apicovered			: raw_bind
* @precondition			:
* @postcondition		:
*/
static void tc_net_raw_bind_n(void)
{
	int ret = -1;

	ret = raw_bind(NULL, NULL);
	TC_ASSERT_NEQ("raw_bind", ret, ZERO);
	TC_SUCCESS_RESULT();
}

/**
* @testcase				: tc_net_raw_bind_p
* @brief				:
* @scenario				:
* @apicovered			: raw_bind
* @precondition			:
* @postcondition		:
*/
static void tc_net_raw_bind_p(void)
{
	struct raw_pcb *raw = NULL;
	ip_addr_t loop_ipaddr;
	int ret;

	raw = (struct raw_pcb *)raw_new(IP_PROTO_TCP);
	IP4_ADDR(&loop_ipaddr, 127, 0, 0, 1);
	ret = raw_bind(raw, &loop_ipaddr);

	raw_remove(raw);
	TC_ASSERT_EQ("raw_bind", ret, 0);
	TC_SUCCESS_RESULT();
}

/****************************************************************************
 * Name: raw api
 ****************************************************************************/
int tc_net_raw_main(void)
{
	tc_net_raw_bind_p();
	tc_net_raw_bind_n();
	tc_net_raw_connect_p();
	tc_net_raw_connect_n();
	tc_net_raw_send_n();
	return 0;
}
