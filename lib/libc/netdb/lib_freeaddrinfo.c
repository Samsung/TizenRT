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

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: freeaddrinfo
 *
 * Description:
 *   Frees one or more addrinfo structures returned by getaddrinfo(), along with
 *   any additional storage associated with those structures. If the ai_next field
 *   of the structure is not null, the entire list of structures is freed.
 *
 * Input Parameters:
 *   ai - struct addrinfo to free
 *
 ****************************************************************************/

/****************************************************************************
 * Name: freeaddrinfo
 ****************************************************************************/
#ifdef CONFIG_NET_LWIP_NETDB
void freeaddrinfo(FAR struct addrinfo *ai)
{
	if (!ai) {
		printf("ai is null\n");
		return;
	}

	int ret = -1;
	struct req_lwip_data req;

	int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd < 0) {
		printf("socket() failed with errno: %d\t%s\n", errno, __FUNCTION__);
		return;
	}

	memset(&req, 0, sizeof(req));
	req.type = FREEADDRINFO;
	req.msg.netdb.ai_res = ai;

	ret = ioctl(sockfd, SIOCLWIP, (unsigned long)&req);
	if (ret == ERROR) {
		printf("ioctl() failed with errno: %d\t%s\n", errno, __FUNCTION__);
	}

	close(sockfd);
}
#endif
