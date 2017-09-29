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
/// @file tc_net_netifapi_add.c
/// @brief Test Case Example for listen() API
#include "tc_internal.h"
#include <net/lwip/netif.h>
#include <net/lwip/netifapi.h>
#include <net/lwip/ipv4/ip_addr.h>

/**
* @testcase              :tc_do_netif_api_netif_add
* @brief                 :call netif_api_add inside.
* @scenario              :take argument as struct netifapi_msg and send it to netif_api_add.
* @apicovered            :do_netifapi_netif_add, netifapi_netif_add, ip_addr_copy
* @precondition          :none
* @postcondition         :none
* @return                :void
*/
static void tc_do_netifapi_netif_add_n(void)
{
	struct netifapi_msg msg;
	struct netif *netif = NULL;

	ip_addr_t remote_ip, local_ip, netmask;
	memset(netif, 0, sizeof(struct netif));

	IP4_ADDR(&local_ip, 192, 168, 1, 1);
	IP4_ADDR(&remote_ip, 192, 168, 1, 2);
	IP4_ADDR(&netmask, 255, 255, 255, 0);

	ip_addr_copy(netif->netmask, netmask);
	ip_addr_copy(netif->ip_addr, local_ip);
	msg.msg.netif = netif;
	do_netifapi_netif_add(msg);
	TC_ASSERT_EQ("do_netifapi_netif_add", msg.msg.err, ZERO);
	TC_SUCCESS_RESULT();
}

/****************************************************************************
 * Name net_netifapi_main
 ****************************************************************************/
int net_netifapi_main(void)
{
	tc_do_netifapi_netif_add_n();
	return 0;
}
