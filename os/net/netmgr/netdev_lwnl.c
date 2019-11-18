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
#include <tinyara/lwnl/lwnl.h>
#include <tinyara/netmgr/netdev_mgr.h>
#include "netdev_mgr_internal.h"

extern int netdev_handle_wifi(struct netdev *dev, lwnl_req cmd, void *data, uint32_t data_len);
extern int netdev_handle_ethernet(struct netdev *dev, lwnl_req cmd, void *data, uint32_t data_len);

int netdev_req_handle(const char *msg, size_t msg_len)
{
	lwnl_msg *lmsg = (lwnl_msg *)msg;
	lwnl_result_e *res = &lmsg->res;

	struct netdev *dev = NULL;
	dev = (struct netdev *)nm_get_netdev(lmsg->name);
	if (!dev) {
		return -ENOTTY;
	}

	switch (dev->type) {
	case NM_WIFI:
		*res = netdev_handle_wifi(dev, lmsg->req_type, lmsg->data, lmsg->data_len);
	case NM_ETHERNET:
		*res = netdev_handle_ethernet(dev, lmsg->req_type, lmsg->data, lmsg->data_len);
		break;
	default:
		return -ENOSYS;
	}
	if (*res != LWNL_SUCCESS) {
		return -ENOSYS;
	}
	return 0;
}
