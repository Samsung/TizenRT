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
 * Function: netdev_getstats
 *
 * Description:
 *   Return the specific interface stats
 *
 * Parameters:
 *   arg Type of information to get
 *
 * Returned Value:
 *   0:Success; negated errno on failure
 *
 ****************************************************************************/
#if defined(CONFIG_NET_NETMON) && defined(CONFIG_NET_STATS)
static int _copy_socket(void *arg)
{
	int num_copy = 0;
	sq_queue_t *qsock = (sq_queue_t *) arg;
	struct netstack *st = get_netstack(TR_SOCKET);
	if (!st) {
		return -1;
	}
	int res = 0;
	for (int i = 0; i < CONFIG_NSOCKET_DESCRIPTORS; i++) {
		struct netmon_sock *sock_info = NULL;
		res = st->ops->getstats(i, &sock_info);
		if (res < 0) {
			continue;
		}
		num_copy++;
		sq_addlast((sq_entry_t *)sock_info, qsock);
	}
	return num_copy;
}

#endif  //CONFIG_NET_NETMON & CONFIG_NET_STATS

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
	int num_copy;
	if (cmd == SIOCGETSOCK) {
		num_copy = _copy_socket(arg);
		/* num_copy shoud be larger than 0 (this socket) */
		if (num_copy > 0) {
			ret = OK;
		} else {
			ret = -EINVAL;
		}
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
