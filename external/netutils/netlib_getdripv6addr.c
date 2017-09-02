/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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
 * netutils/netlib/netlib_setdripv6addr.c
 *
 *   Copyright (C) 2015 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#if defined(CONFIG_NET_IPv6) && CONFIG_NSOCKET_DESCRIPTORS > 0

#include <sys/socket.h>
#include <sys/ioctl.h>

#include <unistd.h>
#include <string.h>
#include <errno.h>

#include <netinet/in.h>
#include <net/if.h>

#include <netutils/netlib.h>

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: netlib_set_dripv6addr
 *
 * Description:
 *   Set the default router IP address
 *
 * Parameters:
 *   ifname   The name of the interface to use
 *   ipaddr   The address to set
 *
 * Return:
 *   0 on success; -1 on failure
 *
 ****************************************************************************/

int netlib_get_dripv6addr(FAR const char *ifname, FAR const struct in6_addr *addr)
{
	int ret = ERROR;

	if (ifname && addr) {
		int sockfd = socket(PF_INET6, NETLIB_SOCK_IOCTL, 0);
		if (sockfd >= 0) {
			FAR struct sockaddr_in6 *inaddr;
			struct lifreq req;

			/* Add the device name to the request */

			strncpy(req.lifr_name, ifname, IFNAMSIZ);

			/* Add the INET address to the request */

			inaddr = (FAR struct sockaddr_in6 *)&req.lifr_addr;
			inaddr->sin6_family = AF_INET6;
			inaddr->sin6_port = 0;
			memcpy(&inaddr->sin6_addr, addr, sizeof(struct in6_addr));

			ret = ioctl(sockfd, SIOCGLIFDSTADDR, (unsigned long)((uintptr_t)&req));
			close(sockfd);
		}
	}

	return ret;
}

#endif							/* CONFIG_NET_IPv6 && CONFIG_NSOCKET_DESCRIPTORS */
