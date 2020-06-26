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
#if defined(CONFIG_NET) && CONFIG_NSOCKET_DESCRIPTORS > 0

#include <string.h>
#include <errno.h>

#include "lwip/netif.h"
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
 * Function: netdev_getifname
 *
 * Description:
 *   Return active NIC name
 *
 * Parameters:
 *   ifname - a user buffer to receive NIC name.
 *
 * Returned Value:
 *  0 on success, non-zero on failure
 *
 ****************************************************************************/
extern struct netif *g_netdevices;

int netdev_getifname(char *ifname)
{
	struct netif *dev = NULL;
	for (dev = g_netdevices; dev; dev = dev->next) {
		if (dev->flags & NETIF_FLAG_LINK_UP) {
			sprintf(ifname, "%c%c%d", dev->name[0], dev->name[1], dev->num);
			return 0;
		}
	}

	return -1;
}
#endif // (CONFIG_NET) && CONFIG_NSOCKET_DESCRIPTORS > 0
