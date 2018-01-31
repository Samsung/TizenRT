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

/// @file tc_net_ether.c
/// @brief Test Case Example for ether_ntoa() API
#include <tinyara/config.h>
#include <stdio.h>
#include <errno.h>

#include <sys/stat.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netutils/netlib.h>
#include <netinet/ether.h>

#include <sys/socket.h>
#include <sys/ioctl.h>

#include "tc_internal.h"

/**
* @testcase		tc_net_ether_ntoa_p
* @brief
* @scenario
* @apicovered		ether_ntoa()
* @precondition
* @postcondition
*/
static void tc_net_ether_ntoa_p(void)
{
	struct sockaddr *sa;
	struct ifreq *ifr;
	struct ifreq tmp;
	struct ifconf ifcfg;
	int numreqs = 3;
	int fd = -1;
	int ret;
	FAR char *buffer = NULL;

	ifcfg.ifc_buf = NULL;
	ifcfg.ifc_len = sizeof(struct ifreq) * numreqs;
	ifcfg.ifc_buf = malloc(ifcfg.ifc_len);
	ifr = ifcfg.ifc_req;

	fd = socket(AF_INET, SOCK_DGRAM, 0);
	TC_ASSERT_NEQ("ether_ntoa", fd, -1)

	ret = ioctl(fd, SIOCGIFCONF, (unsigned long)&ifcfg);
	TC_ASSERT_GEQ_CLEANUP("ether_ntoa", ret, 0, close(fd))

	strncpy(tmp.ifr_name, ifr->ifr_name, 6);
	ret = ioctl(fd, SIOCGIFHWADDR, (unsigned long)&tmp);
	TC_ASSERT_GEQ_CLEANUP("ether_ntoa", ret, 0, close(fd))

	sa = &tmp.ifr_hwaddr;
	buffer = (FAR char *)ether_ntoa((struct ether_addr *)sa->sa_data);
	TC_ASSERT_NEQ_CLEANUP("ether_ntoa", buffer, 0, close(fd))

	TC_SUCCESS_RESULT()

	close(fd);
	free(ifcfg.ifc_buf);
}

/****************************************************************************
 * Name: ether_ntoa()
 ****************************************************************************/

int net_ether_main(void)
{
	tc_net_ether_ntoa_p();
	return 0;
}
