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

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: netlib_setdnsserver
 *
 * Description:
 *   Set DNS server to a network stack
 *   If index is -1 then it'll allocate dns server to available slot.
 *   Otherwise index is not -1 then it'll allocate or overwrite dns server to
 *   index slot.
 *
 * Returned Value:
 *   0 on success, non-zero on failure
 *   < 0 on failure
 *   -1 : fail to communicate to network stack, See errno value get more info
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
 *   -2 : Invalid index number
 *   -3 : There are no available slot to assign a DNS server
 *
 ****************************************************************************/

int netlib_setdnsserver(struct sockaddr *addr, int index)
{
	int ret = -1;
	struct req_lwip_data req;

	int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd < 0) {
		printf("socket() failed with errno: %d\n", errno);
		return ret;
	}

	memset(&req, 0, sizeof(req));
	req.type = DNSSETSERVER;
	req.addr = addr;
	req.index = index;

	ret = ioctl(sockfd, SIOCLWIP, (unsigned long)&req);
	close(sockfd);
	if (ret == ERROR) {
		printf("ioctl() failed with errno: %d\n", errno);
		return ret;
	}
	return req.req_res;
}
