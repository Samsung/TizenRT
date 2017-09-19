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
#include <net/lwip/udp.h>
#include <net/lwip/netif/etharp.h>
#include <net/lwip/stats.h>
#include <net/lwip/ipv4/igmp.h>
#include <stdlib.h>
#include "tc_internal.h"

static struct netif test_netif;
static ip_addr_t test_ipaddr, test_netmask, test_gw;
struct eth_addr test_ethaddr = { {1, 1, 1, 1, 1, 1} };

static int linkoutput_ctr;

/**
* @fn                : etharp_remove_all
* @brief             : Helper function
* @scenario          : call etharp_tmr often enough to have all entries cleaned
* @API's covered     : etharp_tmr
* @Preconditions     : none
* @Postconditions    : none
*/
static void etharp_remove_all(void)
{
	int i;
	/* call etharp_tmr often enough to have all entries cleaned */
	for (i = 0; i < 0xff; i++) {
		etharp_tmr();
	}
}

/**
* @fn                : default_netif_linkoutput
* @brief             : Helper function
* @scenario          : linkout the netif
* @API's covered     : none
* @Preconditions     : none
* @Postconditions    : return 0 on success
*/
static err_t default_netif_linkoutput(struct netif *netif, struct pbuf *p)
{
	linkoutput_ctr++;
	return ERR_OK;
}

/**
* @fn                  : default_netif_init
* @brief               : Helper function
* @scenario            : initilize the default value of netif interface
* @API's covered       : none
* @Preconditions       : none
* @Postconditions      : return 0
*/
static err_t default_netif_init(struct netif *netif)
{
	netif->linkoutput = default_netif_linkoutput;
	netif->output = etharp_output;
	netif->mtu = 1500;
	netif->flags = NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP | NETIF_FLAG_LINK_UP;
	netif->hwaddr_len = ETHARP_HWADDR_LEN;
	return 0;
}

/**
* @testcase            : default_netif_add
* @brief               : Helper function
* @scenario            : Add a default netif interface
* @apicovered          : netif_set_default
* @precondition        : none
* @postcondition       : none
*/
static void default_netif_add(void)
{
	IP4_ADDR(&test_gw, 192, 168, 0, 1);
	IP4_ADDR(&test_ipaddr, 192, 168, 0, 1);
	IP4_ADDR(&test_netmask, 255, 255, 0, 0);
	netif_set_default(netif_add(&test_netif, &test_ipaddr, &test_netmask, &test_gw, NULL, default_netif_init, NULL));
	netif_set_up(&test_netif);
}

/**
* @testcase            : default_netif_remove
* @brief               : Helper function
* @scenario            : Remove netif interface
* @apicovered          : netif_remove
* @precondition        : call to remove neif interface
* @postcondition       : remove neif interface
*/
static void default_netif_remove(void)
{
	netif_remove(&test_netif);
}

/**
* @testcase            : etharp_setup
* @brief               : Helper function
* @scenario            : It setup the netif interface
* @apicovered          : etharp_remove_all
* @precondition        : none
* @postcondition       : add netif interface
*/
static void etharp_setup(void)
{
	etharp_remove_all();
	default_netif_add();
}

/**
* @testcase            : etharp_teardown
* @brief               : Helper function
* @scenario            :
* @apicovered          : etharp_remove_all
* @precondition        :
* @postcondition       :
*/
static void etharp_teardown(void)
{
	etharp_remove_all();
	default_netif_remove();
}

/**
* @testcase            : tc_etharp_add_static_n
* @brief               :
* @scenario            :
* @apicovered          : etharp_add_static
* @precondition        :
* @postcondition       :
*/
static void tc_etharp_find_addr_n(void)
{
	int ret = 0;
	etharp_find_addr(NULL, NULL, NULL, NULL);
	TC_ASSERT_EQ("etharp_find_addr", ret, NEG_VAL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_etharp_add_static_p
* @brief               : Add one static entry to ARP table
* @scenario            : Configure static arp.
* @apicovered          : etharp_find_addr
* @precondition        : ARP entry need to be know
* @postcondition       : return 0 on success
*/
static void tc_etharp_find_addr_p(void)
{
	ip_addr_t *unused_ipaddr;
	struct eth_addr *unused_ethaddr;
	int ret;
	ip_addr_t adrs[ARP_TABLE_SIZE + 2];
	int i;

	for (i = 0; i < ARP_TABLE_SIZE + 2; i++) {
		IP4_ADDR(&adrs[i], 192, 168, 0, i + 2);
	}

	ret = etharp_add_static_entry(&adrs[ARP_TABLE_SIZE], &test_ethaddr);
	etharp_find_addr(NULL, &adrs[ARP_TABLE_SIZE], &unused_ethaddr, &unused_ipaddr);
	TC_ASSERT_NEQ("etharp_find_addr", ret, ONE);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_igmp_removegroup_n
* @brief               : Remove a group in the global igmp_group_list
* @scenario            : Checking if there is no igmp_group
* @apicovered          : igmp_removegroup_n
* @precondition        : none
* @postcondition       : return error no.
*/
static void tc_igmp_removegroup_n(void)
{
	int ret;

	ret = igmp_remove_group(NULL);
	TC_ASSERT_EQ("igmp_removegroup", ret, ZERO);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_igmp_removegroup_p
* @brief               : Remove a group in the global igmp_group_list
* @scenario            : It remmove igmp group if any have
* @apicovered          : igmp_removegroup_p
* @precondition        : Shold have one igmp group
* @postcondition       : 0 on succes
*/
static void tc_igmp_removegroup_p(void)
{
	ip_addr_t *groupaddr = NULL;
	ip_addr_t ifaddr;
	int ret;

	etharp_setup();
	groupaddr = (ip_addr_t *) igmp_lookup_group(&test_netif, &ifaddr);
	ret = igmp_remove_group(groupaddr);
	etharp_teardown();
	TC_ASSERT_EQ("igmp_removeroup", ret, ZERO);
	TC_SUCCESS_RESULT();
}

/**
* @testcasu            : tc_igmp_leavegroup_n
* @brief               : Leave a group on one network interface.
* @scenario            : If there is no networkk interface
* @apicovered          : igmp_leavegroup
* @precondition        : none
* @postcondition       : erro on return
*/
void tc_igmp_leavegroup_n(void)
{
	int ret;

	ret = igmp_leavegroup(NULL, NULL);
	TC_ASSERT_NEQ("igmp_leavegroup", ret, ZERO);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_igmp_leavegroup_p
* @brief               : Leave a group on one network interface.
* @scenario            : If there is igmp group is should left on netif
* @apicovered          : igmp_leavegroup_p
* @precondition        : should have igmp group
* @postcondition       : return 0 on success
*/
void tc_igmp_leavegroup_p(void)
{
	ip_addr_t *ifaddr = NULL;
	ip_addr_t *groupaddr = NULL;
	int ret;

	etharp_setup();
	ifaddr = (ip_addr_t *)malloc(sizeof(ip_addr_t));
	groupaddr = (ip_addr_t *) igmp_lookup_group(&test_netif, ifaddr);
	ipaddr_aton("198.51.100.4", ifaddr);
	ret = igmp_leavegroup(ifaddr, groupaddr);
	etharp_teardown();
	TC_FREE_MEMORY(ifaddr);
	TC_ASSERT_EQ("igmp_leavegroup", ret, ZERO);
	TC_SUCCESS_RESULT();
}

/**
* @testcase            : tc_igmp_joingroup_n
* @brief               : Join one igmp group on one network interface
* @scenario            : If it not able to join group on interface
* @apicovered          : igmp_joingroup
* @precondition        : nothing
* @postcondition       : return error
*/
static void tc_igmp_joingroup_n(void)
{
	int ret;

	ret = igmp_joingroup(NULL, NULL);
	TC_ASSERT_NEQ("igmp_joingroup", ret, ZERO);
	TC_SUCCESS_RESULT();
}

/**
* @testcasu            : tc_igmp_joingroup_p
* @brief               : Join one igmp group on one network interface
* @scenario            : Check for group and add the interface
* @apicovered          : igmp_joingroup
* @precondition        : Should have interface adder and group adder.
* @postcondition       : return success
*/
static void tc_igmp_joingroup_p(void)
{
	ip_addr_t *ifaddr = NULL;
	ip_addr_t *groupaddr = NULL;
	int ret;

	etharp_setup();
	ifaddr = (ip_addr_t *)malloc(sizeof(ip_addr_t));
	groupaddr = (ip_addr_t *) igmp_lookup_group(&test_netif, ifaddr);
	ipaddr_aton("198.51.100.4", ifaddr);
	ret = igmp_joingroup(ifaddr, groupaddr);
	etharp_teardown();
	TC_FREE_MEMORY(ifaddr);
	TC_ASSERT_EQ("igmp_joingroup", ret, ZERO);
	TC_SUCCESS_RESULT();
}

/****************************************************************************
 * Name: igmp()
 ****************************************************************************/
int net_igmp_main(void)
{
	tc_igmp_joingroup_p();
	tc_igmp_joingroup_n();
	tc_igmp_leavegroup_p();
	tc_igmp_leavegroup_n();
	tc_igmp_removegroup_p();
	tc_igmp_removegroup_n();
	tc_etharp_find_addr_p();
	tc_etharp_find_addr_n();
	return 0;
}
