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
#if defined(CONFIG_NET_IPv4) && CONFIG_NSOCKET_DESCRIPTORS > 0

#include <sys/socket.h>
#include <sys/ioctl.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <netinet/in.h>
#include <net/if.h>
#include <ifaddrs.h>
#include <netutils/netlib.h>
#include <tinyara/lwnl/lwnl.h>
#include <tinyara/net/netlog.h>

#define TAG "[NETLIB]"

static inline int _send_msg(lwnl_msg *msg)
{
	int ret = 0;
	int fd = socket(AF_LWNL, SOCK_RAW, LWNL_ROUTE);
	if (fd < 0) {
		NET_LOGE(TAG, "create socket %d\n", errno);
		ret = -1;
	} else {
		if (write(fd, msg, sizeof(*msg)) < 0) {
			NET_LOGE(TAG, "write %d\n", errno);
			ret = -2;
		}
		close(fd);
	}
	return ret;
}

int netlib_getifaddrs(struct ifaddrs **ifap)
{
	int res = 0;
	lwnl_msg msg = {LWNL_INTF_NAME, {LWNL_REQ_COMMON_GETADDRINFO},
					sizeof(*ifap), NULL, (void *)&res};
	int lres = _send_msg(&msg);
	if (lres < 0) {
		NET_LOGE(TAG, "send request msg fail %d %d\n", lres, res);
		return -1;
	}
	*ifap = (struct ifaddrs *)msg.data;
	return res;
}

void netlib_freeifaddrs(struct ifaddrs *ifa)
{
	struct ifaddrs *cursor = ifa, *next = NULL;
	if (cursor) {
		next = cursor->ifa_next;
		if (cursor->ifa_name) {
			free(cursor->ifa_name);
		}
		if (cursor->ifa_addr) {
			free(cursor->ifa_addr);
		}
		if (cursor->ifa_netmask) {
			free(cursor->ifa_netmask);
		}
		if (cursor->ifa_dstaddr) {
			free(cursor->ifa_dstaddr);
		}
		free(cursor);
		cursor = next;
	}
}
#endif //#if defined(CONFIG_NET_IPv4) && CONFIG_NSOCKET_DESCRIPTORS > 0
