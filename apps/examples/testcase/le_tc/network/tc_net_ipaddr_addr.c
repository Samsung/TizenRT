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

/// @file tc_net_ipaddr_addr.c
/// @brief Test Case Example for ipaddr_addr() API
#include <sys/stat.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <net/lwip/ipv4/ip_addr.h>
#include <net/lwip/ipv4/inet.h>

#include "tc_internal.h"

/**
* @testcase            :tc_net_ipaddr_ntoa_p
* @brief               :ascii internet address interpretation routine
* @scenario            :none
* @apicovered          :ipaddr_ntoa
* @precondition        :none
* @postcondition       :none
*/
void tc_net_ipaddr_ntoa_p(void)
{
	char *ret;
	struct ip_addr local;
	IP4_ADDR(&local, 127, 0, 0, 1);	/* Set local = "127.0.0.1" */

	ret = ipaddr_ntoa(&local);
	TC_ASSERT_NEQ("ipaddr_ntoa", ret, NULL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            :tc_net_ipaddr_addr_p
* @brief               :ascii internet address interpretation routine
* @scenario            :none
* @apicovered          :ipaddr_addr
* @precondition        :none
* @postcondition       :none
*/
void tc_net_ipaddr_addr_p(void)
{
	unsigned int ret;
	const char *cp = "127.0.0.1";

	ret = ipaddr_addr(cp);
	TC_ASSERT_NEQ("ipaddr_addr", ret, IPADDR_NONE);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            :tc_net_ipaddr_ntoa_r_p
* @brief               :convert numeric IP address into decimal dotted ASCII representation.
* @scenario            :when user supplied buffer is using
* @apicovered          :ipaddr_ntoa_r
* @precondition        :valid buffer
* @postcondition       :none
*/
static void tc_net_ipaddr_ntoa_r_p(void)
{
	char *ret;
	struct ip_addr local;
	static char str[16];

	IP4_ADDR(&local, 127, 0, 0, 1);	/* Set local = "127.0.0.1" */

	ret = ipaddr_ntoa_r(&local, str, 16);
	TC_ASSERT_NEQ("ipaddr_ntoa_r", ret, NULL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            :tc_net_ipaddr_ntoa_r_n
* @brief               :convert numeric IP address into decimal dotted ASCII representation.
* @scenario            :when user supplied buffer is using
* @apicovered          :ipaddr_ntoa_r
* @precondition        :Valid buffer
* @postcondition       :none
*/
static void tc_net_ipaddr_ntoa_r_n(void)
{
	char *ret;
	struct ip_addr local;
	static char str[16];

	IP4_ADDR(&local, 127, 0, 0, 1);	/* Set local = "127.0.0.1" */

	ret = ipaddr_ntoa_r(&local, str, 10);
	TC_ASSERT_EQ("ipaddr_ntoa_r", ret, NULL);
	TC_SUCCESS_RESULT();
}

/****************************************************************************
 * Name:net_ipaddr_addr_main
 ****************************************************************************/

int net_ipaddr_addr_main(void)
{
	tc_net_ipaddr_addr_p();
	tc_net_ipaddr_ntoa_p();
	tc_net_ipaddr_ntoa_r_p();
	tc_net_ipaddr_ntoa_r_n();
	return 0;
}
