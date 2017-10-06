/****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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

// @file tc_net_igmp.c
// @brief Test Case Example for igmp() API
#include <tinyara/config.h>
#include <errno.h>
#include <sys/stat.h>
#include <net/if.h>
#include <net/lwip/netif.h>
#include <netutils/netlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <pthread.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <sys/ioctl.h>
#include <netutils/ipmsfilter.h>
#include <sys/sockio.h>
#include <netutils/ipmsfilter.h>

#include "tc_internal.h"

#define PORTNUM        5001
#define MAXRCVLEN      20
#define BACKLOG        1
#define HOST_NAME_MAX  20
#define DSIZE          30
#define LEN            20

#if CONFIG_NSOCKET_DESCRIPTORS > 0
extern struct netif *g_netdevices;
#endif
static int count_wait;
struct sockaddr_in groupSock;
struct sockaddr_in localSock;
char interface_name[LEN];
char host_ip[LEN];

/**
* @fn                    :wait
* @brief                 :function to wait on semaphore.
* @scenario              :use wait function to decrement count_wait value.
* @API's covered         :none
* @Preconditions         :none
* @Postconditions        :none
* @return                :void
*/
static void wait(void)
{
	while (count_wait <= ZERO) {
		printf("");
	}
	count_wait--;
}

/**
* @fn                    :nw_signal
* @brief                 :function to signal semaphore.
* @scenario              :use to increase the count_wait value.
* @API's covered         :none
* @Preconditions         :none
* @Postconditions        :none
* @return                :void
*/
static void nw_signal(void)
{
	count_wait++;
}

/**
* @fn                   :getif_addrs
* @brief                :function creates a linked list of structures describing the network interfaces of the local system
* @scenario             :get the interface
* @API's covered        :netlib_get_ipv4addr,netlib_get_dripv4addr,netlib_getifstatus
* @Preconditions        :none
* @Postconditions       :none
* @return               :int
*/
int getif_addrs(struct ifaddrs **ifap)
{
	uint8_t flags;
	static struct ifaddrs ifa;
	static struct sockaddr_in addr;
	static struct sockaddr_in netmask;
	struct netif *curr = g_netdevices;

	memset(&ifa, 0, sizeof(ifa));
	memset(&addr, 0, sizeof(addr));
	memset(&netmask, 0, sizeof(netmask));

	netlib_get_ipv4addr(curr->d_ifname, &addr.sin_addr);
	netlib_get_dripv4addr(curr->d_ifname, &netmask.sin_addr);
	netlib_getifstatus(curr->d_ifname, &flags);

	ifa.ifa_next = NULL;
	ifa.ifa_name = curr->d_ifname;
	ifa.ifa_flags = flags | IFF_RUNNING;
	addr.sin_family = netmask.sin_family = AF_INET;
	ifa.ifa_addr = (struct sockaddr *)&addr;
	ifa.ifa_netmask = (struct sockaddr *)&netmask;
	*ifap = &ifa;
	return 0;
}

/**
* @fn                   :gethost_addr
* @brief                :function get the host interface and IP addr
* @scenario             :get the interface and IP
* @API's covered        :socket
* @Preconditions        :none
* @Postconditions       :none
* @return               :void
*/
void gethost_addr(void)
{
	int family;
	int n;
	int ret;
	char host[LEN];
	struct ifaddrs *ifaddr;
	struct ifaddrs *ifa;

	ret = getif_addrs(&ifaddr);
	TC_ASSERT_NEQ("getifaddrs", ret, NEG_VAL);

	for (ifa = ifaddr, n = 0; ifa != NULL; ifa = ifa->ifa_next, n++) {
		if ((struct ifaddrs *)ifa->ifa_addr == NULL)
			continue;
		family = ifa->ifa_addr->sa_family;
		if (family == AF_INET) {
			ret = getnameinfo(ifa->ifa_addr, (family == AF_INET) ? sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6), host, LEN, NULL, 0, NI_NUMERICHOST);
			TC_ASSERT_NEQ("getnameinfo", ret, ZERO);
		} else if (family == AF_PACKET && ifa->ifa_data != NULL) {
			if (strcmp(ifa->ifa_name, "lo") != ZERO) {
				struct ifreq ifr;

				int fd = socket(AF_INET, SOCK_DGRAM, 0);
				TC_ASSERT_NEQ("socket", fd, NEG_VAL);

				ifr.ifr_addr.sa_family = AF_INET;
				strcpy(interface_name, ifa->ifa_name);
				strncpy(ifr.ifr_name, ifa->ifa_name, IFNAMSIZ-1);
				ret = ioctl(fd, SIOCGIFADDR, &ifr);
				TC_ASSERT_NEQ("ioctl", fd, NEG_VAL);

				close(fd);
				if (ifr.ifr_addr.sa_data) {
					strcpy(host_ip, (FAR char *)ether_ntoa((struct ether_addr *)ifr.ifr_addr.sa_data));
				} else {
					continue;
				}
			}
		}
	}
}

/**
* @fn                   :tc_net_igmp_sourcefilter
* @brief                :to set or modify the source filter content
* @scenario             :source filter
* @API's covered        :socket,inet_addr,ioctl,close
* @Preconditions        :none
* @Postconditions       :none
* @return               :void
*/
void tc_net_igmp_sourcefilter(void)
{
	int ret;
	struct ip_msfilter imsf;

	int sock = socket(AF_INET, SOCK_DGRAM, 0);
	TC_ASSERT_NEQ("socket", sock, NEG_VAL);

	imsf.imsf_multiaddr.s_addr = inet_addr("239.255.255.20");
	strncpy(imsf.imsf_name, host_ip, IMSFNAMSIZ);
	imsf.imsf_fmode = MCAST_INCLUDE;

	ret = ioctl(sock, SIOCSIPMSFILTER, (unsigned long)&imsf);
	TC_ASSERT_NEQ_CLEANUP("ioctl", ret, NEG_VAL, close(sock));
	close(sock);
	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_net_igmp_join
* @brief                :function to join IGMP group
* @scenario             :to join IGMP group
* @API's covered        :socket,inet_addr,setsockopt,close
* @Preconditions        :none
* @Postconditions       :none
* @return               :void
*/
void tc_net_igmp_join(void)
{
	int ret;
	struct ip_mreq imr;

	int sock = socket(AF_INET, SOCK_DGRAM, 0);
	TC_ASSERT_NEQ("socket", sock, NEG_VAL);

	imr.imr_multiaddr.s_addr = inet_addr("239.255.255.20");
	imr.imr_interface.s_addr = inet_addr(host_ip);
	ret = setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, &imr, sizeof(struct ip_mreq));
	TC_ASSERT_NEQ_CLEANUP("setsockopt", ret, NEG_VAL, close(sock));
	close(sock);
	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_net_igmp_leave
* @brief                :function to leave IGMP group
* @scenario             :to leave IGMP group
* @API's covered        :socket,inet_addr,setsockopt,close
* @Preconditions        :none
* @Postconditions       :none
* @return               :void
*/
void tc_net_igmp_leave(void)
{
	int ret;
	struct ip_mreq imr;

	int sock = socket(AF_INET, SOCK_DGRAM, 0);
	TC_ASSERT_NEQ("socket", sock, NEG_VAL);

	imr.imr_multiaddr.s_addr = inet_addr("239.255.255.20");
	imr.imr_interface.s_addr = inet_addr(host_ip);
	ret = setsockopt(sock, IPPROTO_IP, IP_DROP_MEMBERSHIP, &imr, sizeof(struct ip_mreq));
	TC_ASSERT_NEQ_CLEANUP("setsockopt", ret, NEG_VAL, close(sock));
	close(sock);
	TC_SUCCESS_RESULT();
}

/****************************************************************************
 * Name: net_igmp_main
 ****************************************************************************/

int net_igmp_main(void)
{
	gethost_addr();
	tc_net_igmp_join();
	tc_net_igmp_leave();
	tc_net_igmp_sourcefilter();
	return 0;
}
