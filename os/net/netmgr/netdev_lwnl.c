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
#include <stdint.h>
#include <stdbool.h>
#include <errno.h>
#include <net/if.h>
#include <ifaddrs.h>
#include <tinyara/lwnl/lwnl.h>
#include <tinyara/netmgr/netdev_mgr.h>
#include <tinyara/net/if/wifi.h>
#include "netdev_mgr_internal.h"
#include <tinyara/net/netlog.h>

#define TAG "[NETMGR]"

extern int netdev_handle_wifi(struct netdev *dev, lwnl_req cmd, void *data, uint32_t data_len);
extern int netdev_handle_ethernet(struct netdev *dev, lwnl_req cmd, void *data, uint32_t data_len);

struct ifaddr_msg {
	FAR struct ifaddrs *ifa;
	uint8_t idx;
};

static int _netdev_getifaddr(struct netdev *dev, void *arg)
{
	FAR struct ifaddr_msg *msg = (struct ifaddr_msg *)arg;
	struct ifaddrs *addr = NULL;
	int res = ND_NETOPS(dev, get_ifaddrs)(dev, &addr);
	if (res < 0) {
		NET_LOGKE(TAG, "get_ifaddrs fail\n");
		return -1;
	}

	if (msg->idx == 0) {
		msg->ifa = addr;
	} else {
		struct ifaddrs *cursor = msg->ifa;
		while (cursor->ifa_next) {
			cursor = cursor->ifa_next;
		}
		cursor->ifa_next = addr;
	}
	msg->idx++;

	return 0;
}

static int _handle_common(lwnl_msg *msg)
{
	int res = 0;
	if (msg->req_type.type == LWNL_REQ_COMMON_GETADDRINFO) {
		struct ifaddr_msg imsg = {NULL, 0};
		int ret = nm_foreach(_netdev_getifaddr, (void *)&imsg);
		if (ret == OK) {
			msg->data = (void *)imsg.ifa;
		} else {
			res = -1;
		}
	} else {
		NET_LOGKE(TAG, "invalid request type");
		res = -1;
	}

	return res;
}

int netdev_req_handle(const char *msg, size_t msg_len)
{
	lwnl_msg *lmsg = (lwnl_msg *)msg;
	struct netdev *dev = NULL;

	if (!strncmp((const char *)lmsg->name, LWNL_INTF_NAME, strlen(LWNL_INTF_NAME) + 1)) {
		int *res = (int *)lmsg->result;
		*res = _handle_common(lmsg);
		return 0;
	}

	dev = (struct netdev *)nm_get_netdev(lmsg->name);
	if (!dev) {
		return -ENOSYS;
	}

	switch (dev->type) {
	case NM_WIFI: {
		trwifi_result_e *res = (trwifi_result_e *)lmsg->result;
		*res = netdev_handle_wifi(dev, lmsg->req_type, lmsg->data, lmsg->data_len);
	}
		break;
	case NM_ETHERNET: {
		int *res = (int *)lmsg->result;
		*res = netdev_handle_ethernet(dev, lmsg->req_type, lmsg->data, lmsg->data_len);
	}
		break;
	default:
		return -ENOSYS;
	}
	return 0;
}
