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

/// @file tc_netdev_carrier.c
/// @brief Test Case Example for netdev() API
#include <tinyara/net/netdev.h>
#include <net/lwip/netif.h>
#include <net/if.h>

#include "tc_internal.h"

/**
* @testcase            : tc_netdev_carrier_on_p
* @brief               : Notifies the networking layer about an available carrier.
* @scenario            : Get the inforamation about available carrier.
* @apicovered          : netdev_carrier_on
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_netdev_carrier_on_p(void)
{
	int ret;
	struct net_driver_s *dev;

	dev = (struct net_driver_s *)malloc(sizeof(struct net_driver_s));
	TC_ASSERT_NEQ("malloc", dev, NULL);

	ret = netdev_carrier_on(dev);

	TC_FREE_MEMORY(dev);
	TC_ASSERT_EQ("netdev", ret, ZERO);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_netdev_carrier_on_n
* @brief               : Notifies the networking layer about an available carrier.
* @scenario            : Get the inforamation about available carrier.
* @apicovered          : netdev_carrier_on
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_netdev_carrier_on_n(void)
{
	int ret;

	ret = netdev_carrier_on(NULL);
	TC_ASSERT_NEQ("netdev", ret, ZERO);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_netdev_carrier_off_p
* @brief               : Notifies the networking layer about an disappeared carrier.
* @scenario            : Get the inforamation about disappeared carrier.
* @apicovered          : netdev_carrier_off
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_netdev_carrier_off_p(void)
{
	int ret;
	struct net_driver_s *dev;

	dev = (struct net_driver_s *)malloc(sizeof(struct net_driver_s));
	TC_ASSERT_NEQ("malloc", dev, NULL);
	dev->d_flags = 4;

	ret = netdev_carrier_off(dev);

	TC_FREE_MEMORY(dev);
	TC_ASSERT_EQ("netdev", ret, ZERO);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_netdev_carrier_off_n
* @brief               : Notifies the networking layer about an disappeared carrier.
* @scenario            : Get the inforamation about disappeared carrier.
* @apicovered          : netdev_carrier_off
* @precondition        : none
* @postcondition       : none
* @return              : void
*/
static void tc_netdev_carrier_off_n(void)
{
	int ret;

	ret = netdev_carrier_off(NULL);
	TC_ASSERT_NEQ("netdev", ret, ZERO);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_test_function
* @brief               : Helper function.
* @scenario            : none
* @apicovered          : none
* @precondition        : none
* @postcondition       : none
* @return              : int
*/
static int tc_test_function(struct netif *dev)
{
	return ZERO;
}

/**
* @testcase            : tc_netdev_ifdown
* @brief               : Bring the interface down.
* @scenario            : Bring the interface down.
* @apicovered          : netdev_ifdown
* @precondition        : up the interface
* @postcondition       : none
* @return              : void
*/
static void tc_netdev_ifdown(void)
{
	struct netif dev;

	dev.d_flags = 0;
	dev.d_flags |= IFF_UP;
	dev.d_ifdown = tc_test_function;

	netdev_ifdown(&dev);
	TC_ASSERT_EQ("netdev", dev.d_flags, ZERO)
	TC_SUCCESS_RESULT();
}

/****************************************************************************
 * Name: netdev()
 ****************************************************************************/
int netdev_carrier_main(void)
{
	tc_netdev_carrier_on_p();
	tc_netdev_carrier_on_n();
	tc_netdev_carrier_off_p();
	tc_netdev_carrier_off_n();
	tc_netdev_ifdown();
	return 0;
}
