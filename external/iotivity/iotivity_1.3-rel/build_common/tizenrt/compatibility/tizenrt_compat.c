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

#include "tizenrt_compat.h"

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

	struct netif *curr = g_netdevices;

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
