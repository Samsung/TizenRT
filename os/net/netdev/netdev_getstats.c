/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
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
#if defined(CONFIG_NET) && CONFIG_NSOCKET_DESCRIPTORS > 0 && defined(CONFIG_NET_NETMON) && defined(CONFIG_NET_STATS)

#include <errno.h>
#include <net/lwip/netif.h>
#include <net/if.h>
#include "netdev/netdev.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Types
 ****************************************************************************/

/****************************************************************************
 * Private Data
 ****************************************************************************/

/****************************************************************************
 * Public Data
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/
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

int netdev_getstats(void *arg)
{
	int ret = -EINVAL;
	struct netif *dev;
	struct netmon_netdev_stats *stats = (struct netmon_netdev_stats *)arg;
	if (stats) {
		char *intf = stats->devname;
		dev = netdev_findbyname(intf);
		if (dev) {
			netdev_semtake();
			stats->devinpkts = dev->mib2_counters.ifinucastpkts + dev->mib2_counters.ifinnucastpkts + dev->mib2_counters.ifindiscards + dev->mib2_counters.ifinerrors + dev->mib2_counters.ifinunknownprotos;
			stats->devinoctets = dev->mib2_counters.ifinoctets;
			stats->devoutpkts = dev->mib2_counters.ifoutucastpkts + dev->mib2_counters.ifoutnucastpkts + dev->mib2_counters.ifoutdiscards + dev->mib2_counters.ifouterrors;
			stats->devoutoctets = dev->mib2_counters.ifoutoctets;
			netdev_semgive();
			ret = OK;
		}
	}
	return ret;
}

#endif							/* CONFIG_NET && CONFIG_NSOCKET_DESCRIPTORS && CONFIG_NET_NETMON && CONFIG_NET_STATS */
