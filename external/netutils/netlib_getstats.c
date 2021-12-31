/****************************************************************************
 *
 * Copyright 2021 Samsung Electronics All Rights Reserved.
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

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netdb.h>
#include <errno.h>
#include <tinyara/netmgr/netctl.h>
#include <tinyara/net/netlog.h>

#define TAG "[NETLIB]"
/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: netlib_getstats
 *
 * Description:
 *   Outputcurrent TX/RX statistics informations of network stack.
 *
 * Returned Value:
 *   0 on success, non-zero on failure
 *   -1 on failure with errno set properly:
 *
 *   EBADF
 *     'fd' is not a valid descriptor.
 *   EFAULT
 *     'arg' references an inaccessible memory area.
 *   EINVAL
 *     'cmd' or 'arg' is not valid.
 *   ENOTTY
 *     'fd' is not associated with a character special device.
 *   ENOTTY
 *      The specified request does not apply to the kind of object that the
 *      descriptor 'fd' references.
 *
 ****************************************************************************/

int netlib_getstats(void *arg)
{
	int ret = -1;
	struct req_lwip_data req;

	int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd < 0) {
		NET_LOGE(TAG, "socket() failed with errno: %d\n", errno);
		return ret;
	}

	memset(&req, 0, sizeof(req));
	req.type = GETNETSTATS;

	ret = ioctl(sockfd, SIOCLWIP, (unsigned long)&req);
	close(sockfd);
	if (ret == ERROR) {
		NET_LOGE(TAG, "ioctl() failed with errno: %d\n", errno);
		return ret;
	}
	// req_req_res is always OK.
	return OK;
}
