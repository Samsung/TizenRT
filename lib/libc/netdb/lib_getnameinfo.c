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
 * Name: getnameinfo
 *
 * Description:
 *   Translates the socket addresses and returns the string.
 *   This is the dummy function to support compatibility for iotivity.
 *
 * Input Parameters:
 *   sa - socket address to translate
 *   salen - length of the socket address
 *   host - host string pointer to be returned
 *   hostlen - length of the host name buffer
 *   serv - service string pointer to be returned
 *   servlen - length of the service name buffer
 *   flags - takes NI_NUMERICHOST and NI_NUMERICSERV
 * 
 * Returned Value:
 *   0 on success, non-zero on failure
 *
 ****************************************************************************/

/****************************************************************************
 * Name: getnameinfo
 ****************************************************************************/
#if defined(CONFIG_NET_LWIP_NETDB) && defined(LWIP_COMPAT_SOCKETS)
int getnameinfo(const struct sockaddr *sa, size_t salen, char *host, size_t hostlen, char *serv, size_t servlen, int flags)
{
	int ret = -1;
	struct req_lwip_data req;

	int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd < 0) {
		printf("socket() failed with errno: %d\n", errno);
		return ret;
	}

	memset(&req, 0, sizeof(req));
	req.type = GETNAMEINFO;
	req.sa = sa;
	req.sa_len = salen;
	req.host_name = host;
	req.host_len = hostlen;
	req.serv_name = serv;
	req.serv_len = servlen;
	req.flags = flags;

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
#endif
