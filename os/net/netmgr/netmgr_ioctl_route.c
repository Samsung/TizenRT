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

/****************************************************************************
 * Name: netdev_rtioctl
 *
 * Description:
 *   Perform routing table specific operations.
 *
 * Parameters:
 *   psock    Socket structure
 *   dev      Ethernet driver device structure
 *   cmd      The ioctl command
 *   rtentry  The argument of the ioctl cmd
 *
 * Return:
 *   >=0 on success (positive non-zero values are cmd-specific)
 *   Negated errno returned on failure.
 *
 ****************************************************************************/
static int netdev_rtioctl(FAR struct socket *sock, int cmd, FAR struct rtentry *rtentry)
{
	(void)sock;
	int ret = -EAFNOSUPPORT;

	/* Execute the command */

	switch (cmd) {
	case SIOCADDRT: {			/* Add an entry to the routing table */
		/* The target address and the netmask are required values */
		ret = netmgr_addroute(rtentry);
	}
	break;

	case SIOCDELRT: {			/* Delete an entry from the routing table */
		/* The target address and the netmask are required values */
		ret = netmgr_delroute(rtentry);
	}
	break;

	default:
		ret = -ENOTTY;
		break;
	}

	return ret;
}
