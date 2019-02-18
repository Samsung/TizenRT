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

#include <sys/socket.h>
#include <sys/ioctl.h>

#include <netdb.h>

#include <errno.h>

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: getaddrinfo
 *
 * Description:
 * Translates the name of a service location (for example, a host name) and/or
 * a service name and returns a set of socket addresses and associated
 * information to be used in creating a socket with which to address the
 * specified service.
 *
 * Due to a limitation in dns_gethostbyname, only the first address of a
 * host is returned.
 * Also, service names are not supported (only port numbers)!
 *
 * Input Parameters:
 *   hostname  - descriptive name or address string of the host
 *                 (may be NULL -> local address)
 *   servname - port number as string of NULL
 *   hint - structure containing input values that set socktype and protocol
 *   res - pointer to a pointer where to store the result (set to NULL on failure)
 *
 * Returned Value:
 *   0 on success, non-zero on failure
 *
 ****************************************************************************/

/****************************************************************************
 * Name: getaddrinfo
 ****************************************************************************/
#ifdef CONFIG_NET_LWIP_NETDB
int getaddrinfo(FAR const char *hostname, FAR const char *servname, FAR const struct addrinfo *hint, FAR struct addrinfo **res)
{
	int ret = -1;
	struct req_lwip_data req;

	int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd < 0) {
		printf("socket() failed with errno: %d\n", errno);
		return ret;
	}

	memset(&req, 0, sizeof(req));
	req.type = GETADDRINFO;
	req.host_name = hostname;
	req.serv_name = servname;
	req.ai_hint = hint;
	req.ai_res = NULL;

	ret = ioctl(sockfd, SIOCLWIP, (unsigned long)&req);
	if (ret == ERROR) {
		printf("ioctl() failed with errno: %d\n", errno);
		return ret;
	}

	ret = req.req_res;
	*res = (struct addrinfo *)req.ai_res;
	close(sockfd);
	return ret;
}
#endif
