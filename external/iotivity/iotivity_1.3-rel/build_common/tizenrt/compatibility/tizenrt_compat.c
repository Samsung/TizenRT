/* *****************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
 *
 *
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************/

#include <tinyara/config.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <net/if.h>
#include <ifaddrs.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <net/lwip/netif.h>
#include <net/lwip/netdb.h>
#if defined(CONFIG_NET_IPv6)
#include <net/lwip/mld6.h>
#endif

#include "tizenrt_compat.h"

/*****************************************
   define
 *****************************************/
#if defined(CONFIG_ARCH_CHIP_S5JT200)
#if defined(CONFIG_NET_LWIP_LOOPBACK_INTERFACE)
#define NETIF_NAME "wl1"
#else
#define NETIF_NAME "wl0"
#endif
#else
#error "undefined NETIF_NAME"
#endif

/*****************************************
   global variables
 *****************************************/
#if defined(CONFIG_NET_IPv6)
const struct in6_addr in6addr_loopback = IN6ADDR_LOOPBACK_INIT;
#endif

/*****************************************
   function related to network interface
 *****************************************/

int getifaddrs(struct ifaddrs **ifap)
{
	static struct ifaddrs ifa;
	static struct sockaddr_in addr, netmask;
	uint8_t flags;

	memset(&ifa, 0, sizeof(ifa));
	memset(&addr, 0, sizeof(addr));
	memset(&netmask, 0, sizeof(netmask));

#if defined(CONFIG_NET_IPv6)
	static struct ifaddrs ifa6;
	static struct sockaddr_in6 addr6, netmask6;

	memset(&ifa6, 0, sizeof(ifa6));
	memset(&addr6, 0, sizeof(addr6));
	memset(&netmask6, 0, sizeof(netmask6));
#endif

	struct netif *curr = netif_find(NETIF_NAME);
	if (curr == NULL) {
		return -1;
	}

	netlib_get_ipv4addr(curr->d_ifname, &addr.sin_addr);
	netlib_get_dripv4addr(curr->d_ifname, &netmask.sin_addr);
	netlib_getifstatus(curr->d_ifname, &flags);

#if defined(CONFIG_NET_IPv6)
	ifa.ifa_next = &ifa6;
#else
	ifa.ifa_next = NULL;
#endif
	ifa.ifa_name = curr->d_ifname;
	ifa.ifa_flags = flags | IFF_RUNNING;
	addr.sin_family = netmask.sin_family = AF_INET;
	ifa.ifa_addr = (struct sockaddr *)&addr;
	ifa.ifa_netmask = (struct sockaddr *)&netmask;

#if defined(CONFIG_NET_IPv6)
	netlib_get_ipv6addr(curr->d_ifname, &addr6.sin6_addr);
	netlib_get_dripv6addr(curr->d_ifname, &netmask6.sin6_addr);
	netlib_getifstatus(curr->d_ifname, &flags);

	ifa6.ifa_next = NULL;
	ifa6.ifa_name = curr->d_ifname;
	ifa6.ifa_flags = flags | IFF_RUNNING;
	addr6.sin6_family = netmask6.sin6_family = AF_INET6;
	ifa6.ifa_addr = (struct sockaddr *)&addr6;
	ifa6.ifa_netmask = (struct sockaddr *)&netmask6;
#endif

	*ifap = &ifa;

	return 0;
}

void freeifaddrs(struct ifaddrs *ifa)
{
	/* do nothing because memory is not allocated in getifaddrs() */
}

const char *gai_strerror(int errcode)
{
	static const char *n_str = "null";
	return n_str;
}

unsigned int if_nametoindex(const char *ifname)
{
	errno = 0;
	return 0; // TODO: Now supports only 1 device in Tizen RT
}

char * if_indextoname(unsigned int ifindex, char *ifname)
{
	struct ifaddrs *ifaddrs, *ifa;
	int error = 0;

	if (getifaddrs(&ifaddrs) < 0) {
		return(NULL); /* getifaddrs properly set errno */
	}

	/* Now supports only 1 device in Tizen RT */
	if (ifa == NULL) {
		error = ENXIO;
		ifname = NULL;
	} else {
		strncpy(ifname, ifa->ifa_name, IFNAMSIZ);
	}

	errno = error;
	return(ifname);
}

unsigned int get_default_ifindex(void)
{
	return if_nametoindex(NETIF_NAME);
}

#if defined(CONFIG_NET_IPv6) && (SUPPORT_SETSOCKOPT_IPV6_JOIN_GROUP == 0)
int do_ipv6_join_group(const ip6_addr_t *groupaddr)
{
	struct netif *netif = NULL;

	netif = netif_find(NETIF_NAME);
	if (netif == NULL) {
		return -1;
	}

	return mld6_joingroup_netif(netif, groupaddr);
}

int do_ipv6_leave_group(const ip6_addr_t *groupaddr)
{
	struct netif *netif = NULL;

	netif = netif_find(NETIF_NAME);
	if (netif == NULL) {
		return -1;
	}

	return mld6_leavegroup_netif(netif, groupaddr);
}
#endif /* CONFIG_NET_IPv6 */

