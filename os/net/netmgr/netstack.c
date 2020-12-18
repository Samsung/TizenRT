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

#ifdef CONFIG_NET_LWIP
extern struct netstack *get_netstack_lwip(void);
#endif
#ifdef CONFIG_NET_LOCAL
extern struct netstack *get_netstack_uds(void);
#endif
#ifdef CONFIG_LWNL80211
extern struct netstack *get_netstack_netlink(void);
#endif

static sock_type _get_socktype(int fd)
{
	if (fd < CONFIG_NFILE_DESCRIPTORS) {
		return TR_LWNL;
	}
#ifdef CONFIG_NET_LOCAL
	else if (fd < CONFIG_NFILE_DESCRIPTORS + CONFIG_NUDS_DESCRIPTORS) {
		return TR_UDS;
	}
#endif
	else if (fd < CONFIG_NFILE_DESCRIPTORS + CONFIG_NSOCKET_DESCRIPTORS) {
		return TR_SOCKET;
	}
	return TR_UNKNOWN;
}

struct netstack *get_netstack(sock_type type)
{
	if (type == TR_SOCKET) {
		return get_netstack_lwip();
	} else if (type == TR_UDS) {
#ifdef CONFIG_NET_LOCAL
		return get_netstack_uds();
#endif
	} else if (type == TR_LWNL) {
#ifdef CONFIG_LWNL80211
		return get_netstack_netlink();
#endif
	}
	return NULL;
}

struct netstack *get_netstack_byfd(int fd)
{
	sock_type type = _get_socktype(fd);

	if (type == TR_SOCKET) {
		return get_netstack_lwip();
	} else if (type == TR_UDS) {
#ifdef CONFIG_NET_LOCAL
		return get_netstack_uds();
#endif
	} else if (type == TR_LWNL) {
#ifdef CONFIG_LWNL80211
		return get_netstack_netlink();
#endif
	}
	return NULL;
}
