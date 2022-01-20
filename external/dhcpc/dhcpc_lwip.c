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
 * netutils/dhcpc/dhcpc_lwip.c
 *
 *   Copyright (C) 2007, 2009, 2011-2012 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Based heavily on portions of uIP:
 *
 *   Author: Adam Dunkels <adam@dunkels.com>
 *   Copyright (c) 2005, Swedish Institute of Computer Science
 *   All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <debug.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <netdb.h>
#include <protocols/dhcpc.h>
#include <netutils/netlib.h>
#include <tinyara/netmgr/netctl.h>

/****************************************************************************
 * Definitions
 ****************************************************************************/
/****************************************************************************
 * Private Types
 ****************************************************************************/
/****************************************************************************
 * Private Data
 ****************************************************************************/
/****************************************************************************
 * Private Functions
 ****************************************************************************/
/****************************************************************************
 * Public Functions
 ****************************************************************************/
/****************************************************************************
 * Name: dhcp_client_start
 ****************************************************************************/
int dhcp_client_start(const char *intf)
{
	int ret = -1;
	struct req_lwip_data req;

	int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd < 0) {
		printf("socket() failed with errno: %d\n", errno);
		return ret;
	}

	memset(&req, 0, sizeof(req));
	req.type = DHCPCSTART;
	req.msg.dhcp.intf = intf;

	ret = ioctl(sockfd, SIOCLWIP, (unsigned long)&req);
	if (ret == ERROR) {
		printf("ioctl() failed with errno: %d\n", errno);
		close(sockfd);
		return ret;
	}

	ret = req.req_res;
	close(sockfd);
	return ret;
}

/****************************************************************************
 * Name: dhcp_client_stop
 ****************************************************************************/
void dhcp_client_stop(const char *intf)
{
	struct req_lwip_data req;

	int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd < 0) {
		printf("socket() failed with errno: %d\n", errno);
		return;
	}

	memset(&req, 0, sizeof(req));
	req.type = DHCPCSTOP;
	req.msg.dhcp.intf = intf;

	(void)ioctl(sockfd, SIOCLWIP, (unsigned long)&req);

	close(sockfd);
	return;
}

int dhcp_client_sethostname(const char *intf, const char *hostname)
{
	int ret = -1;
	struct req_lwip_data req;

	int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd < 0) {
		printf("socket() failed with errno: %d\t%s\n", errno, __FUNCTION__);
		return ret;
	}

	memset(&req, 0, sizeof(req));
	req.type = DHCPCSETHOSTNAME;
	req.msg.dhcp.intf = intf;
	req.msg.dhcp.hostname = hostname;

	ret = ioctl(sockfd, SIOCLWIP, (unsigned long)&req);
	close(sockfd);
	if (ret == ERROR) {
		printf("ioctl() failed with errno: %d\t%s\n", errno, __FUNCTION__);
		return ret;
	}
	ret = req.req_res;

	return ret;
}
