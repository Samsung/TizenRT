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
#include <tinyara/netmgr/netdev_mgr.h>
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
#if CONFIG_NET_NETMON && CONFIG_NET_STATS
static int netdev_getstats(void *arg)
{
	int ret = -EINVAL;
	struct netif *dev;
	struct netmon_netdev_stats *stats = (struct netmon_netdev_stats *)arg;
	if (stats) {
		char *intf = stats->devname;
		dev = netdev_findbyname(intf);
		if (dev) {
			netdev_semtake();
			stats->devinpkts = dev->mib2_counters.ifinucastpkts +
				dev->mib2_counters.ifinnucastpkts +
				dev->mib2_counters.ifindiscards +
				dev->mib2_counters.ifinerrors +
				dev->mib2_counters.ifinunknownprotos;

			stats->devinoctets = dev->mib2_counters.ifinoctets;
			stats->devoutpkts = dev->mib2_counters.ifoutucastpkts +
				dev->mib2_counters.ifoutnucastpkts +
				dev->mib2_counters.ifoutdiscards +
				dev->mib2_counters.ifouterrors;

			stats->devoutoctets = dev->mib2_counters.ifoutoctets;
			netdev_semgive();
			ret = OK;
		}
	}
	return ret;
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
	switch (cmd) {
	case SIOCGETSOCK:          /* Get socket info. */
		num_copy = copy_socket(arg);
		/* num_copy shoud be larger than 0 (this socket) */
		if (num_copy > 0) {
			ret = OK;
		} else {
			ret = -EINVAL;
		}
		break;
#if CONFIG_NET_NETMON && CONFIG_NET_STATS
	case SIOCGDSTATS:          /* Get netdev info. */
		ret = netdev_getstats(arg);
		break;
#endif
	default:
		ret = -ENOTTY;
		break;
	} /* end switch */

	return ret;
}
