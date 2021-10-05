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
#include <errno.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <ifaddrs.h>
#include <tinyara/netmgr/netdev_mgr.h>
#include "netstack.h"
#include "netdev_mgr_internal.h"

/****************************************************************************
 * Name: netdev_nmioctl
 *
 * Description:
 *   Perform network monitor specific operations.
 *
 * Parameters:
 *   sock    Socket structure
 *   cmd     The ioctl command
 *   arg    The argument of ioctl command
 *
 * Return:
 *   >=0 on success (positive non-zero values are cmd-specific)
 *   Negated errno returned on failure.
 *
 ****************************************************************************/
int netdev_nmioctl(FAR struct socket *sock, int cmd, void  *arg)
{
	int ret = -EINVAL;
	if (cmd == SIOCGETSOCK) {
		struct netstack *st = get_netstack(TR_SOCKET);
		if (!st) {
			return -1;
		}
		ret = st->ops->getstats(arg);
	}
#if defined(CONFIG_NET_NETMON) && defined(CONFIG_NET_STATS)
	else if (cmd == SIOCGDSTATS) {
		struct netmon_netdev_stats *stats = (struct netmon_netdev_stats *)arg;
		struct netdev *dev = nm_get_netdev((uint8_t *)stats->devname);
		if (!dev) {
			ret = -ENOTTY;
		} else {
			ret = ((struct netdev_ops *)(dev->ops))->get_stats(dev, stats);
		}
	}
#endif
	else {
		ret = -ENOTTY;
	}
	return ret;
}
