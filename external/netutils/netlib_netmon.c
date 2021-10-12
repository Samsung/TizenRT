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

#include <errno.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <net/if.h>
#include <tinyara/netmgr/netctl.h>
#include <netutils/netlib.h>
#include <tinyara/net/netlog.h>

#define TAG "[NETLIB]"

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
	struct req_lwip_data req;
	int sockfd = socket(AF_INET, NETLIB_SOCK_IOCTL, 0);
	if (sockfd == -1) {
		NET_LOGE(TAG, "socket() failed with errno: %d\n", errno);
		return -1;
	}

	memset(&req, 0, sizeof(req));
	req.type = GETSOCKINFO;

	ret = ioctl(sockfd, SIOCLWIP, (unsigned long)&req);
	close(sockfd);
	if (ret == ERROR) {
		NET_LOGE(TAG, "ioctl() failed with errno: %d\n", errno);
		return -1;
	}
	arg = (void *)req.msg.netmon.info;
	return req.req_res;
}

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

int netlib_netmon_devstats(const char *ifname, void **arg)
{
	if (ifname == NULL || arg == NULL) {
		return -1;
	}
	int ret = ERROR;
	struct req_lwip_data req;
	int sockfd = socket(AF_INET, NETLIB_SOCK_IOCTL, 0);
	if (sockfd == -1) {
		NET_LOGE(TAG, "socket() failed with errno: %d\n", errno);
		return -1;
	}

	memset(&req, 0, sizeof(req));
	req.type = GETDEVSTATS;
	req.msg.netmon.ifname = ifname;

	ret = ioctl(sockfd, SIOCLWIP, (unsigned long)&req);
	close(sockfd);
	if (ret == ERROR) {
		NET_LOGE(TAG, "ioctl() failed with errno: %d\n", errno);
		return -1;
	}
	ret = req.req_res;
	if (ret == 0) {
		*arg = (void *)req.msg.netmon.info;
	}
	return req.req_res;
}
#endif /* CONFIG_NET && CONFIG_NSOCKET_DESCRIPTORS */
