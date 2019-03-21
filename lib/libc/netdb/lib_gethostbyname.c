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

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: gethostbyname
 *
 * Description:
 *   Returns an entry containing addresses of address family AF_INET
 *   for the host with name name.
 *   Due to dns_gethostbyname limitations, only one address is returned.
 *
 * Input Parameters:
 *   name - the hostname to resolve
 * 
 * Returned Value:
 *   an entry containing addresses of address family AF_INET
 *   for the host with name
 *
 ****************************************************************************/

/****************************************************************************
 * Name: gethostbyname
 ****************************************************************************/
#ifdef CONFIG_NET_LWIP_NETDB
struct hostent *gethostbyname(const char *name)
{
	struct req_lwip_data req;

	int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd < 0) {
		printf("socket() failed with errno: %d\n", errno);
		return NULL;
	}

	memset(&req, 0, sizeof(req));
	req.type = GETHOSTBYNAME;
	req.host_name = name;

	int ret = ioctl(sockfd, SIOCLWIP, (unsigned long)&req);
	if (ret == ERROR) {
		printf("ioctl() failed with errno: %d\n", errno);
		close(sockfd);
		return NULL;
	}

	close(sockfd);
	return req.host_entry;
}
#endif
