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

#include "sys/sockio.h"

static FAR struct netdev *_netdev_imsfdev(FAR struct ip_msfilter *imsf)
{
	if (!imsf) {
		return NULL;
	}
	/* Find the network device associated with the device name
	 * in the request data.
	 */
	return netmgr_getnetdev(imsf->imsf_name);
}

/****************************************************************************
 * Name: netdev_imsfioctl
 *
 * Description:
 *   Perform network device specific operations.
 *
 * Parameters:
 *   psock    Socket structure
 *   dev      Ethernet driver device structure
 *   cmd      The ioctl command
 *   imsf     The argument of the ioctl cmd
 *
 * Return:
 *   >=0 on success (positive non-zero values are cmd-specific)
 *   Negated errno returned on failure.
 *
 ****************************************************************************/
int netdev_imsfioctl(FAR struct socket *sock, int cmd, FAR struct ip_msfilter *imsf)
{
	(void)sock;
	FAR struct netdev *dev;
	int ret = -EINVAL;

	nvdbg("cmd: %d\n", cmd);

	/* Execute the command */
	switch (cmd) {
	case SIOCSIPMSFILTER: {		/* Set source filter content */
		dev = _netdev_imsfdev(imsf);
		if (!dev) {
			break;
		}

		if (imsf->imsf_fmode == MCAST_INCLUDE) {
			ret = dev->ops->joingroup(dev, &imsf->imsf_multiaddr);
		} else {
			DEBUGASSERT(imsf->imsf_fmode == MCAST_EXCLUDE);
			ret = dev->ops->leavegroup(dev, &imsf->imsf_multiaddr);
		}
	}
	break;
	case SIOCGIPMSFILTER:		/* Retrieve source filter addresses */
	default:
		ret = -ENOTTY;
		break;
	}

	return ret;
}
