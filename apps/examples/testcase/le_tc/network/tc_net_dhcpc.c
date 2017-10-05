/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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

/// @file tc_net_dhcpc.c
/// @brief Test Case Example for dhcpc() API

#include <tinyara/config.h>
#include <stdio.h>
#include <errno.h>

#include <sys/stat.h>
#include <string.h>
#include <net/lwip/netif.h>
#include <protocols/dhcpc.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <netutils/ipmsfilter.h>
#include <sys/sockio.h>
#include <netutils/ipmsfilter.h>
#include <protocols/tftp.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netutils/netlib.h>
#include <sys/socket.h>
#include "tc_internal.h"


#define DNS_DEFAULT_PORT        53
#define PORTNUM                 5001
#define MAXRCVLEN               20
#define BACKLOG                 1
#define HOST_NAME_MAX           20
#define DSIZE                   30
#define LEN                     20

#if CONFIG_NSOCKET_DESCRIPTORS > 0
extern struct netif *g_netdevices;
#endif
static int count_wait;
struct sockaddr_in groupSock;
struct sockaddr_in localSock;
char interface_name[LEN];
char host_ip[LEN];

/**
* @fn                    :wait
* @brief                 :function to wait on semaphore.
* @scenario              :use wait function to decrement count_wait value.
* @API's covered         :none
* @Preconditions         :none
* @Postconditions        :none
* @return                :void
*/
static void wait(void)
{
	while (count_wait <= ZERO) {
		printf("");
	}
	count_wait--;
}

/**
* @fn                    :nw_signal
* @brief                 :function to signal semaphore.
* @scenario              :use to increase the count_wait value.
* @API's covered         :none
* @Preconditions         :none
* @Postconditions        :none
* @return                :void
*/
static void nw_signal(void)
{
	count_wait++;
}

/**
* @fn                   :getif_addrs
* @brief                :function creates a linked list of structures describing the network interfaces of the local system
* @scenario             :get the interface
* @API's covered        :netlib_get_ipv4addr,netlib_get_dripv4addr,netlib_getifstatus
* @Preconditions        :none
* @Postconditions       :none
* @return               :int
*/
static int getif_addrs(struct ifaddrs **ifap)
{
	uint8_t flags;
	static struct ifaddrs ifa;
	static struct sockaddr_in addr;
	static struct sockaddr_in netmask;
	struct netif *curr = g_netdevices;

	memset(&ifa, 0, sizeof(ifa));
	memset(&addr, 0, sizeof(addr));
	memset(&netmask, 0, sizeof(netmask));

	netlib_get_ipv4addr(curr->d_ifname, &addr.sin_addr);
	netlib_get_dripv4addr(curr->d_ifname, &netmask.sin_addr);
	netlib_getifstatus(curr->d_ifname, &flags);

	ifa.ifa_next = NULL;
	ifa.ifa_name = curr->d_ifname;
	ifa.ifa_flags = flags | IFF_RUNNING;
	addr.sin_family = netmask.sin_family = AF_INET;
	ifa.ifa_addr = (struct sockaddr *)&addr;
	ifa.ifa_netmask = (struct sockaddr *)&netmask;
	*ifap = &ifa;
	return 0;
}

/**
* @fn                   :gethost_addr
* @brief                :function get the host interface and IP addr
* @scenario             :get the interface and IP
* @API's covered        :socket
* @Preconditions        :none
* @Postconditions       :none
* @return               :void
*/
static void gethost_addr(void)
{
	int family;
	int n;
	int ret;
	char host[LEN];
	struct ifaddrs *ifaddr;
	struct ifaddrs *ifa;
	ret = getif_addrs(&ifaddr);
	TC_ASSERT_NEQ("getifaddrs", ret, NEG_VAL);

	for (ifa = ifaddr, n = ZERO; ifa != NULL; ifa = ifa->ifa_next, n++) {
		if ((struct ifaddrs *)ifa->ifa_addr == NULL) {
			continue;
		}
		family = ifa->ifa_addr->sa_family;
		if (family == AF_INET) {
			ret = getnameinfo(ifa->ifa_addr, (family == AF_INET) ? sizeof(struct sockaddr_in) :sizeof(struct sockaddr_in6), host, LEN, NULL, 0, NI_NUMERICHOST);
			TC_ASSERT_EQ("getnameinfo", ret, ZERO);
		} else if (family == AF_PACKET && ifa->ifa_data != NULL) {
			if (strcmp(ifa->ifa_name, "lo") != ZERO) {
				struct ifreq ifr;
				int fd = socket(AF_INET, SOCK_DGRAM, 0);
				TC_ASSERT_NEQ("socket", fd, NEG_VAL);
				ifr.ifr_addr.sa_family = AF_INET;
				strcpy(interface_name, ifa->ifa_name);
				strncpy(ifr.ifr_name, ifa->ifa_name, IFNAMSIZ-1);
				ret = ioctl(fd, SIOCGIFADDR, &ifr);
				TC_ASSERT_NEQ("ioctl", fd, NEG_VAL);
				close(fd);
				if (ifr.ifr_addr.sa_data) {
					strcpy(host_ip, (FAR char *)ether_ntoa((struct ether_addr *)ifr.ifr_addr.sa_data));
				} else {
					continue;
				}
			}
		}
	}
}


/**
* @fn                   :tc_net_dhcp
* @brief                :a DHCP client is an Internet host using DHCP to obtain configuration parameters such as an IP address.
* @scenario             :get the ip from dhcp server
* @API's covered        :dhcpc_request, dhcpc_close
* @Preconditions        :none
* @Postconditions       :none
* @return               :void
*/
void tc_net_dhcp(void)
{
	int ret_val;
	int ret;
	FAR void *handle;
	struct dhcpc_state ds;

	uint8_t mac[IFHWADDRLEN];

	ret = netlib_ifup(interface_name);
	TC_ASSERT_EQ("netlib_ifup", ret, ZERO);

	ret = netlib_getmacaddr(interface_name, mac);
	TC_ASSERT_EQ("netlib_getmacaddr", ret , ZERO);
	handle = dhcpc_open(interface_name);
	TC_ASSERT_NEQ("dhcpc_open", handle, NULL);

	ret = dhcpc_request(handle, &ds);
	TC_ASSERT_LT_CLEANUP("dhcpc_request", ret, ZERO, dhcpc_close(handle));

	ret = netlib_set_ipv4addr(interface_name, &ds.ipaddr);
	TC_ASSERT_NEQ("netlib_set_ipv4addr", ret, ZERO);

	if (ds.netmask.s_addr != ZERO) {
		ret = netlib_set_ipv4netmask(interface_name, &ds.netmask);
		TC_ASSERT_NEQ("netlib_set_ipv4netmast", ret, ZERO);
	}

	if (ds.default_router.s_addr != ZERO) {
		ret = netlib_set_dripv4addr(interface_name, &ds.default_router);
		TC_ASSERT_NEQ("netlib_set_dripv4addr", ret, ZERO);
	}

	dhcpc_close(handle);
	gethost_addr();
	ret_val = strcmp(host_ip, "0.0.0.0");

	TC_ASSERT_NEQ("dhcp", ret_val, ZERO);
	TC_SUCCESS_RESULT();
}

/****************************************************************************
 * Name: net_dhcpc_main
 ****************************************************************************/
int net_dhcpc_main(void)
{
	gethost_addr();
	tc_net_dhcp();
	return 0;
}
