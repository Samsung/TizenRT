/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
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
/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <netdb.h>
#include <net/if.h>
#include <ifaddrs.h>

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: getifaddrs
 *
 * Description:
 *   Return active NIC information whereas linux implementation
 *   returns all NIC information. it's temporary API for supporting Iotivity
 *   It should be fixed later when requirements is changed.
 *
 * Input Parameters:
 *   ifap - The list  consists of ifaddrs structures
 *
 * Returned Value:
 *   0 on success, non-zero on failure
 *
 ****************************************************************************/

/****************************************************************************
 * Name: getifaddrs
 ****************************************************************************/

int getifaddrs(struct ifaddrs **ifap)
{
	// iotivity doesn't call freeifaddr, so it's defined to static
	static struct ifaddrs ifa;
	static struct sockaddr_in addr, netmask;
	static char ifname[10] = {0,};
	uint8_t flags;

	// get interface name to fetch ip address
	int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd < 0) {
		printf("socket() failed with errno: %d\n", errno);
		return -1;
	}

	struct ifreq req;
	memset(&req, 0, sizeof(req));

	int ret = ioctl(sockfd, SIOCGIFNAME, (unsigned long)&req);
	if (ret == ERROR) {
		printf("ioctl() failed with errno: %d\n", errno);
		close(sockfd);
		return ret;
	}
	close(sockfd);
	strncpy(ifname, req.ifr_name, IFNAMSIZ);

	memset(&ifa, 0, sizeof(ifa));
	memset(&addr, 0, sizeof(addr));
	memset(&netmask, 0, sizeof(netmask));

	netlib_get_ipv4addr(ifname, &addr.sin_addr);
	netlib_get_dripv4addr(ifname, &netmask.sin_addr);
	netlib_getifstatus(ifname, &flags);

	ifa.ifa_next = NULL;
	ifa.ifa_name = ifname;
	ifa.ifa_flags = flags | IFF_RUNNING;
	addr.sin_family = netmask.sin_family = AF_INET;
	ifa.ifa_addr = (struct sockaddr *)&addr;
	ifa.ifa_netmask = (struct sockaddr *)&netmask;

	*ifap = &ifa;

	return ret;
}
