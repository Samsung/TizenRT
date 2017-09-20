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

/// @file tc_net_ipv4_netmask_valid.c
/// @brief Test Case Example for ip4_addr_netmask_valid() API

#include <net/lwip/ipv4/ip_addr.h>

#include "tc_internal.h"

/**
* @testcase             : tc_net_ipv4_addr_netmask_valid_p
* @brief                : Checks if a netmask is valid (starting with ones, then only zeros)
* @scenario             : netmask the IPv4 netmask to check (in network byte order!)
* @apicovered           : ip4_addr_netmask_valid
* @precondition         : none
* @postcondition        : none
* @return               : void
*/
void tc_net_ipv4_addr_netmask_valid_p(void)
{
	int ret;
	int netmask = 0x0000ffff;	/* Value for checking */

	ret = ip4_addr_netmask_valid(netmask);
	TC_ASSERT_EQ("ip4_addr_netmask_valid", ret, ONE);
	TC_SUCCESS_RESULT();
}

/**
* @testcase             : tc_net_ipv4_addr_netmask_valid_n
* @brief                : Checks if a netmask is valid (starting with ones, then only zeros)
* @scenario             : netmask the IPv4 netmask to check (in network byte order!)
* @apicovered           : ip4_addr_netmask_valid
* @precondition         : none
* @postcondition        : none
* @return               : void
*/
void tc_net_ipv4_addr_netmask_valid_n(void)
{
	int ret;
	int netmask = 0xffffff90;	/* Value for checking */

	ret = ip4_addr_netmask_valid(netmask);
	TC_ASSERT_EQ("ip4_addr_netmask_valid", ret, ONE);
	TC_SUCCESS_RESULT();
}

/****************************************************************************
 * Name: ip4_addr_netmask_valid()
 ****************************************************************************/
int net_ipv4_addr_netmask_valid_main(void)
{
	tc_net_ipv4_addr_netmask_valid_p();
	tc_net_ipv4_addr_netmask_valid_n();

	return 0;
}
