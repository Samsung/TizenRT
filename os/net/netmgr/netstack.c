/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
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

#include <tinyara/config.h>
#include <poll.h>
#include <sys/socket.h>
#include <net/if.h>
#include <tinyara/net/net.h>
#include "netstack.h"
#include <tinyara/net/netlog.h>

#define TAG "[NETMGR]"
#ifdef CONFIG_NET_LWIP
extern struct netstack *get_netstack_lwip(void);
#endif
#ifdef CONFIG_LWNL80211
extern struct netstack *get_netstack_netlink(void);
#endif

static sock_type _get_socktype(int fd)
{
	if (fd < CONFIG_NFILE_DESCRIPTORS) {
		return TR_LWNL;
	} else if (fd < CONFIG_NFILE_DESCRIPTORS + CONFIG_NSOCKET_DESCRIPTORS) {
		return TR_SOCKET;
	}
	NET_LOGKE(TAG, "not supported socket type\n");
	return TR_UNKNOWN;
}

struct netstack *get_netstack(sock_type type)
{
	if (type == TR_SOCKET) {
		return get_netstack_lwip();
	} else if (type == TR_LWNL) {
#ifdef CONFIG_LWNL80211
		return get_netstack_netlink();
#endif
	}
	NET_LOGKE(TAG, "not supported stack type\n");
	return NULL;
}

struct netstack *get_netstack_byfd(int fd)
{
	sock_type type = _get_socktype(fd);

	if (type == TR_SOCKET) {
		return get_netstack_lwip();
	} else if (type == TR_LWNL) {
#ifdef CONFIG_LWNL80211
		return get_netstack_netlink();
#endif
	}
	NET_LOGKE(TAG, "not supported stack type\n");
	return NULL;
}
