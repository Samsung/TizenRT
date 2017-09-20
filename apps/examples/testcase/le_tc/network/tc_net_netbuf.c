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

#include <sys/socket.h>

#include "tc_internal.h"

/**
* @testcase              : tc_net_netbuf_start_p
* @brief                 : Move the current data pointer of a packet buffer contained in a netbuf to frist
* @scenario              : the netbuf to modify
* @apicovered            : tc_net_netbuf_start_p
* @precondition          : none
* @postcondition         : none
* @return                : void
*/
static void tc_net_netbuf_start_p(void)
{
	struct netbuf *buf1 = NULL;
	u16_t smallsize = 0;

	buf1 = netbuf_new();
	TC_ASSERT_EQ("netbuf_new", buf1, NULL);
	netbuf_alloc(buf1, smallsize);
	netbuf_alloc(buf1, smallsize);

	netbuf_first(buf1);
	TC_SUCCESS_RESULT();
}

/**
* @testcase              : tc_netbuf_next_more_buff
* @brief                 : Move the current data pointer of a packet buffer contained in a netbuf to next
* @scenario              : the netbuf to modify
* @apicovered            : netbuf_next
* @precondition          : none
* @postcondition         : none
* @return                : void
*/
static void tc_netbuf_next_more_buff(void)
{
	struct netbuf *buf1 = NULL;
	struct netbuf *buf2 = NULL;
	struct netbuf *buf3 = NULL;
	struct netbuf *buf4 = NULL;
	struct netbuf *buf5 = NULL;

	int res = -1;
	buf1 = netbuf_new();
	buf2 = netbuf_new();
	buf3 = netbuf_new();
	buf4 = netbuf_new();
	buf5 = netbuf_new();
	res = netbuf_next(buf1);
	netbuf_delete(buf1);
	buf1 = NULL;
	netbuf_delete(buf2);
	buf2 = NULL;
	netbuf_delete(buf3);
	buf3 = NULL;
	netbuf_delete(buf4);
	buf4 = NULL;
	netbuf_delete(buf5);
	buf5 = NULL;
	TC_ASSERT_NEQ("netbuf_next", res, ONE);
	TC_SUCCESS_RESULT();
}

/**
* @testcase              : tc_netbuf_next_move_no_next
* @brief                 : Deallocate a netbuf allocated by netbuf_new()
* @scenario              : Deallocating the created netbuff
* @apicovered            : netbuf_delete
* @precondition          : none
* @postcondition         : none
* @return                : void
*/
static void tc_netbuf_next_move_no_next(void)
{
	struct netbuf *buf1 = NULL;
	int ret;

	buf1 = netbuf_new();
	ret = netbuf_next(buf1);
	netbuf_delete(buf1);
	TC_ASSERT_NEQ("netbuf_next", ret, ZERO);
	TC_SUCCESS_RESULT();
}

/**
* @testcase              : tc_netbuf_next_no_next
* @brief                 : Deallocate a netbuf allocated by netbuf_new()
* @scenario              : Deallocating the created netbuff
* @apicovered            : netbuf_delete
* @precondition          : create netbuf.
* @postcondition         : none
* @return                : void
*/
static void tc_netbuf_next_no_next(void)
{
	struct netbuf *buf = NULL;
	int ret;
	buf = netbuf_new();

	ret = netbuf_next(buf);
	netbuf_delete(buf);
	TC_ASSERT_EQ("netbuf_next", ret, ZERO);
	TC_SUCCESS_RESULT();
}

/**
* @testcase              : tc_net_netbuf_next_n
* @brief                 : Move the current data pointer of a packet buffer contained in a netbuf to next
* @scenario              : the netbuff to modify
* @apicovered            : netbuf_next
* @precondition          : none
* @postcondition         : none
* @return                : void
*/
static void tc_net_netbuf_next_n(void)
{
	netbuf_next(NULL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase              : tc_net_netbuf_chain_n
* @brief                 : Chain one netbuf to another
* @scenario              : none
* @apicovered            : netbuf_chain
* @precondition          : none
* @postcondition         : none
* @return                : void
*/
static void tc_net_netbuf_chain_n(void)
{
	netbuf_chain(NULL, NULL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase              : tc_net_netbuf_chain_p
* @brief                 : Chain one netbuf to another
* @scenario              : none
* @apicovered            : netbuf_chain
* @precondition          : none
* @postcondition         : none
* @return                : void
*/
static void tc_net_netbuf_chain_p(void)
{
	struct netbuf *buf1 = NULL;
	struct netbuf *buf2 = NULL;
	struct netbuf *buf3 = NULL;
	struct netbuf *buf4 = NULL;
	struct netbuf *buf5 = NULL;
	struct netbuf *check = NULL;
	buf1 = netbuf_new();
	buf2 = netbuf_new();
	buf3 = netbuf_new();
	buf4 = netbuf_new();
	buf5 = netbuf_new();
	check = buf1;
	netbuf_chain(buf1, buf5);
	netbuf_delete(buf1);
	buf1 = NULL;
	netbuf_delete(buf2);
	buf2 = NULL;
	netbuf_delete(buf3);
	buf3 = NULL;
	netbuf_delete(buf4);
	buf4 = NULL;
	netbuf_delete(buf5);
	buf5 = NULL;
	TC_SUCCESS_RESULT();
}

/**
* @testcase              : tc_net_netbuf_new_p
* @brief                 : Create (allocate) and initialize a new netbuf
* @scenario              : to create the new netbuf
* @apicovered            : netbuf_new
* @precondition          : new
* @postcondition         : new
* @return                : void
*/
static void tc_net_netbuf_new_p(void)
{
	struct netbuf *buf = NULL;

	buf = netbuf_new();
	TC_ASSERT_NEQ("netbuf_new", buf, NULL);
	netbuf_delete(buf);
	TC_ASSERT_EQ("tc_net_netbuf_new_p", buf->p, NULL);
	TC_SUCCESS_RESULT();
}

/****************************************************************************
 * Name: netbuf_api
 ****************************************************************************/
int tc_net_netbuf_main(void)
{
	tc_net_netbuf_new_p();
	tc_net_netbuf_chain_p();
	tc_net_netbuf_chain_n();
	tc_net_netbuf_next_n();
	tc_net_netbuf_start_p();
	return 0;
}
