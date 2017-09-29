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
#include <sys/socket.h>
#include <sys/ioctl.h>

#include "tc_internal.h"

#define NUMREQS 3

/**
* @testcase            :tc_net_ether_ntoa_p
* @brief               :ethernet address manipulation routines.
* @scenario            :converts the Ethernet host address addr given in network byte
*                       order to a string into dotted decimal.
* @apicovered          :ether_ntoa, ioctl
* @precondition        :socket file descriptor.
* @postcondition       :none
* @return              :void
*/
static void tc_net_ether_ntoa_p(int sock_udp)
{
	int ret;
	struct sockaddr *sa = NULL;
	struct ifreq *ifr = NULL;
	struct ifreq tmp;
	struct ifconf ifcfg;
	FAR char *buffer = NULL;

	ifcfg.ifc_buf = NULL;
	ifcfg.ifc_len = sizeof(struct ifreq) * NUMREQS;

	ifcfg.ifc_buf = malloc(ifcfg.ifc_len);
	TC_ASSERT_NEQ("malloc", ifcfg.ifc_buf, NULL);
	ifr = ifcfg.ifc_req;

	ret = ioctl(sock_udp, SIOCGIFCONF, (unsigned long)&ifcfg);
	TC_ASSERT_NEQ_CLEANUP("ioctl", ret, NEG_VAL, TC_FREE_MEMORY(ifcfg.ifc_buf));

	strncpy(tmp.ifr_name, ifr->ifr_name, sizeof(tmp.ifr_name));
	ret = ioctl(sock_udp, SIOCGIFHWADDR, (unsigned long)&tmp);
	TC_ASSERT_NEQ_CLEANUP("ioctl", ret, NEG_VAL, TC_FREE_MEMORY(ifcfg.ifc_buf));

	sa = &tmp.ifr_hwaddr;
	buffer = (FAR char *)ether_ntoa((struct ether_addr *)sa->sa_data);
	TC_ASSERT_NEQ_CLEANUP("ether_ntoa", buffer, NULL, TC_FREE_MEMORY(ifcfg.ifc_buf););
	TC_FREE_MEMORY(ifcfg.ifc_buf);
	TC_SUCCESS_RESULT();
}

/****************************************************************************
 * Name: net_ether_main
 ****************************************************************************/

int net_ether_main(void)
{
	int sock_udp = socket(AF_INET, SOCK_DGRAM, 0);

	tc_net_ether_ntoa_p(sock_udp);
	close(sock_udp);
	return 0;
}
