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
#if defined(CONFIG_NET) && (CONFIG_NSOCKET_DESCRIPTORS > 0)

#include <sys/socket.h>
#include <sys/ioctl.h>

#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <netinet/in.h>
#include <net/if.h>

#include <netutils/netlib.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
/****************************************************************************
 * Public Functions
 ****************************************************************************/
/****************************************************************************
 * Name: netlib_netmon_sock
 *
 * Description:
 *   Get the socket info

 * Parameters:
 *   arg   Type of information to get
 *
 * Return:
 *   0 on success; -1 on failure
 *
 ****************************************************************************/

int netlib_netmon_sock(void *arg)
{
	int ret = ERROR;
	/* Get sockets */
	int sockfd = socket(AF_INET, NETLIB_SOCK_IOCTL, 0);
	if (sockfd >= 0) {
		ret = ioctl(sockfd, SIOCGETSOCK, (unsigned long)arg);
	    close(sockfd);
	}
	return ret;
}

#ifdef CONFIG_NET_STATS
/****************************************************************************
 * Public Functions
 ****************************************************************************/
/****************************************************************************
 * Name: netlib_netmon_devstats
 *
 * Description:
 *   Get the stats of a specific interface

 * Parameters:
 *   arg   Type of information to get
 *
 * Return:
 *   0 on success; -1 on failure
 *
 ****************************************************************************/

int netlib_netmon_devstats(void *arg)
{
    int ret = ERROR;
    /* Get netdev stats */
    int sockfd = socket(AF_INET, NETLIB_SOCK_IOCTL, 0);
    if (sockfd >= 0) {
        ret = ioctl(sockfd, SIOCGDSTATS, (unsigned long)arg);
        close(sockfd);
    }
    return ret;
}
#endif							/* CONFIG_NET_STATS */
#endif							/* CONFIG_NET && CONFIG_NSOCKET_DESCRIPTORS */
