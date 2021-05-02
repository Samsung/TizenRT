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

#include <tinyara/config.h>
#include <apps/shell/tash.h>

#include <sys/stat.h>			/* Needed for open */
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sched.h>
#include <fcntl.h>				/* Needed for open */
#include <libgen.h>				/* Needed for basename */
#include <errno.h>
#include <debug.h>

#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/ethernet.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <netinet/ether.h>

#include <tinyara/clock.h>
#include <tinyara/net/net.h>
#include <tinyara/net/ip.h>
#ifdef CONFIG_NETUTILS_NETLIB
#include <netutils/netlib.h>
#endif

#include <protocols/tftp.h>

#include <netdb.h>
#include <protocols/dhcpc.h>

#ifndef DNS_DEFAULT_PORT
#define DNS_DEFAULT_PORT   53
#endif

#include "netcmd.h"
#ifdef CONFIG_NET_PING_CMD
#include "netcmd_ping.h"
#endif
#ifdef CONFIG_NETUTILS_DHCPD
#include "netcmd_dhcpd.h"
#endif
#ifdef CONFIG_NETUTILS_TFTPC
#include "netcmd_tftpc.h"
#endif
#ifdef CONFIG_NET_NETMON
#include "netcmd_netmon.h"
#endif
#undef HAVE_PING
#undef HAVE_PING6

/* Size of the ECHO data */

#define DEFAULT_PING_DATALEN 56

struct ifconfig_cmd_info_s {
	uint8_t mac[IFHWADDRLEN];
	FAR char *intf;
	FAR char *hostip;
	FAR char *gwip;
	FAR char *mask;
	FAR char *dns;
#ifdef CONFIG_NET_ETHERNET
	FAR char *hw;
#endif
};

#ifdef CONFIG_SYSTEM_NETDB
extern int netdb_main(int argc, char *argv[]);
#endif

#define ARGC_CHECK(a, b)                        \
	do {                                        \
		if (a - 1 < b + 1) {                    \
			printf(fmtargrequired, argv[0]);    \
			ret = ERROR;                        \
			goto endout;                        \
		}                                       \
	} while (0)
static void nic_display_state(void)
{
	struct ifreq *ifr;
	struct sockaddr_in *sin;
	struct sockaddr *sa;
	struct ifconf ifcfg;
	int ret = -1;
	int fd;
	int numreqs = 3;
	int num_nic = 0;

	// to get ipv6 address call netlib_getifaddrs()
	struct ifaddrs *ifa = NULL, *ifp = NULL;
	ret = netlib_getifaddrs(&ifa);
	if (ret < 0) {
		printf("get ifaddrs fail\n");
		return;
	}

	fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (fd < 0) {
		ndbg("fail %s:%d\n", __FUNCTION__, __LINE__);
		return;
	}
	memset(&ifcfg, 0, sizeof(ifcfg));
	ifcfg.ifc_buf = NULL;
	ifcfg.ifc_len = sizeof(struct ifreq) * numreqs;
	ifcfg.ifc_buf = malloc(ifcfg.ifc_len);
	if (ioctl(fd, SIOCGIFCONF, (unsigned long)&ifcfg) < 0) {
		perror("SIOCGIFCONF ");
		goto DONE;
	}
	num_nic = ifcfg.ifc_len / sizeof(struct ifreq);
	ifr = ifcfg.ifc_req;
	int i = 0;

	for (; i < num_nic; ifr++, i++) {
		printf("%s\t", ifr->ifr_name);
		sin = (struct sockaddr_in *)&ifr->ifr_addr;
		if ((sin->sin_addr.s_addr) == INADDR_LOOPBACK) {
			printf("Loop Back\t");
		} else {
			struct ifreq tmp;
			strncpy(tmp.ifr_name, ifr->ifr_name, IF_NAMESIZE);
			ret = ioctl(fd, SIOCGIFHWADDR, (unsigned long)&tmp);
			if (ret < 0) {
				ndbg("fail %s:%d\n", __FUNCTION__, __LINE__);
			} else {
				sa = &tmp.ifr_hwaddr;
				printf("Link encap: %s\t", ether_ntoa((struct ether_addr *)sa->sa_data));
			}

			ret = ioctl(fd, SIOCGIFFLAGS, (unsigned long)ifr);
			if (ret < 0) {
				ndbg("fail %s:%d\n", __FUNCTION__, __LINE__);
			} else {
				printf("RUNNING: %s\n", (ifr->ifr_flags & IFF_UP) ? "UP" : "DOWN");
			}
		}
		printf("\tinet addr: %s\t", inet_ntoa(sin->sin_addr));

		ret = ioctl(fd, SIOCGIFNETMASK, (unsigned long)ifr);
		if (ret < 0) {
			ndbg("fail %s:%d\n", __FUNCTION__, __LINE__);
		} else {
			sin = (struct sockaddr_in *)&ifr->ifr_addr;
			printf("Mask: %s\t", inet_ntoa(sin->sin_addr));
		}

		ret = ioctl(fd, SIOCGIFMTU, (unsigned long)ifr);
		if (ret < 0) {
			ndbg("fail %s:%d\n", __FUNCTION__, __LINE__);
		} else {
			printf("MTU: %d\n", ifr->ifr_mtu);
		}

		for (ifp = ifa; ifp; ifp = ifp->ifa_next) {
			if (ifp->ifa_addr && ifp->ifa_addr->sa_family == AF_INET6
				&& !strncmp(ifr->ifr_name, ifp->ifa_name, IFNAMSIZ)) {
				struct sockaddr_in6 *sin6 = (struct sockaddr_in6 *)ifp->ifa_addr;
				char ipaddr[INET6_ADDRSTRLEN + 1] = {0,};
				memset(ipaddr, 0, INET6_ADDRSTRLEN + 1);
				inet_ntop(AF_INET6, (void *)&(sin6->sin6_addr), ipaddr, INET6_ADDRSTRLEN);
				printf("\tinet6: %s\n", ipaddr);
			}
		}
		printf("\n");
	}
DONE:
	netlib_freeifaddrs(ifa);
	free(ifcfg.ifc_buf);
	close(fd);
}

int cmd_ifup(int argc, char **argv)
{
	FAR char *intf = NULL;
	int ret;

	if (argc != 2) {
		printf("Please select nic_name:\n");
		nic_display_state();
		return OK;
	}

	intf = argv[1];
	ret = netlib_ifup(intf);
	printf("ifup %s...%s\n", intf, (ret == OK) ? "OK" : "Failed");
	return ret;
}

int cmd_ifdown(int argc, char **argv)
{
	FAR char *intf = NULL;
	int ret;

	if (argc != 2) {
		printf("Please select nic_name:\n");
		nic_display_state();
		return OK;
	}

	intf = argv[1];
	ret = netlib_ifdown(intf);
	printf("ifdown %s...%s\n", intf, (ret == OK) ? "OK" : "Failed");
	return ret;
}

static int _cmd_ifconfig_parse(int argc, char **argv, struct ifconfig_cmd_info_s *info)
{
	int ret = OK;
	int i = 0;
	FAR char *tmp;

	if (argc <= 2) {
		printf(fmtargrequired, argv[0]);
		ret = ERROR;
		goto endout;
	}

	info->intf = argv[1];

	for (i = 2; i < argc; i++) {
		tmp = argv[i];
		if (!strcmp(tmp, "dr") || !strcmp(tmp, "gw") || !strcmp(tmp, "gateway")) {
			ARGC_CHECK(argc, i);
			info->gwip = argv[i + 1];
			i++;
		} else if (!strcmp(tmp, "netmask")) {
			ARGC_CHECK(argc, i);
			info->mask = argv[i + 1];
			i++;
#ifdef CONFIG_NET_ETHERNET
		/* REVISIT: How will we handle Ethernet and SLIP networks together? */
		} else if (!strcmp(tmp, "hw")) {
			ARGC_CHECK(argc, i);
			info->hw = argv[i + 1];
			i++;
			if (!netlib_hwmacconv(info->hw, info->mac)) {
				ret = ERROR;
				goto endout;
			}
#endif
		} else if (!strcmp(tmp, "dns")) {
			ARGC_CHECK(argc, i);
			info->dns = argv[i + 1];
			i++;
		} else {
			if (i == 2) {
				info->hostip = argv[i];
			}
		}
	}

endout:
	return ret;
}

#ifdef CONFIG_NET_ETHERNET
static int _cmd_ifconfig_setmacaddr(struct ifconfig_cmd_info_s *info)
{
	int ret = OK;
	/* Set Hardware Ethernet MAC address */
	/* REVISIT: How will we handle Ethernet and SLIP networks together? */
	if (info->hw) {
		ndbg("HW MAC: %s\n", info->hw);
		ret = netlib_setmacaddr(info->intf, info->mac);
		if (ret < 0) {
			ndbg("Set mac address fail\n");
			return ret;
		}
	}

	return ret;
}
#endif

static int _ipv4_set_gateway_netmask(struct ifconfig_cmd_info_s *info, struct in_addr *addr)
{
	int ret = OK;
	in_addr_t gip = 0;

	if (info->gwip) {
		ndbg("Gateway: %s\n", info->gwip);
		addr->s_addr = inet_addr(info->gwip);
	} else {
		ndbg("Gateway: default\n");
		gip = NTOHL(addr->s_addr);
		gip &= ~0x000000ff;
		gip |= 0x00000001;
		gip = HTONL(gip);

		addr->s_addr = gip;
	}
	ret = netlib_set_dripv4addr(info->intf, addr);
	if (ret < 0) {
		ndbg("Set IPv4 route fail %s:%d\n", __FUNCTION__, __LINE__);
		return ret;
	}

	if (info->mask) {
		ndbg("Netmask: %s\n", info->mask);
		addr->s_addr = inet_addr(info->mask);
	} else {
		ndbg("Netmask: default\n");
		addr->s_addr = inet_addr("255.255.255.0");
	}
	ret = netlib_set_ipv4netmask(info->intf, addr);
	if (ret < 0) {
		ndbg("Set IPv4 netmask fail %s:%d\n", __FUNCTION__, __LINE__);
		return ret;
	}

	return ret;
}

static int _cmd_ifconfig_setipaddr(struct ifconfig_cmd_info_s *info)
{
	int ret;
	char *hostip = info->hostip;
	char *intf = info->intf;
	struct in_addr addr;

	if (hostip != NULL) {
		if (!strcmp(hostip, "dhcp")) {
			/* Set DHCP addr */
			addr.s_addr = 0;
			ndbg("DHCPC Mode\n");
			ret = netlib_getmacaddr(intf, info->mac);
			if (ret < 0) {
				ndbg("get mac fail %s:%d\n", __FUNCTION__, __LINE__);
				return ret;
			}

			ret = dhcp_client_start(intf);
			if (ret != OK) {
				ndbg("get IP address fail\n");
				return ret;
			}

			ret = netlib_get_ipv4addr(intf, &addr);
			if (ret != OK) {
				ndbg("get IP address fail\n");
				return ret;
			}
			ndbg("get IP address %s\n", inet_ntoa(addr));
		} else {
			/* Set manual host IP address */
			ndbg("Host IP: %s\n", hostip);
			if (strstr(hostip, ".") != NULL) {
				addr.s_addr = inet_addr(hostip);
				netlib_set_ipv4addr(intf, &addr);
#ifdef CONFIG_NET_IPv6
			} else if (strstr(hostip, ":") != NULL) {
				struct in6_addr addr6;
				inet_pton(AF_INET6, hostip, &addr6);
				netlib_set_ipv6addr(intf, &addr6);
#endif /* CONFIG_NET_IPv6 */
			} else {
				ndbg("hostip is not valid\n");
				return ERROR;
			}
		}

		/* Gateway and Netmask setting */
		ret = _ipv4_set_gateway_netmask(info, &addr);
		if (ret < 0) {
			ndbg("failed to set Gateway and Netmask\n");
			return ret;
		}
	}
	return OK;
}

static int _cmd_ifconfig_setdns(struct ifconfig_cmd_info_s *info)
{
	int ret = OK;
	struct sockaddr_in dns_addr;

	if (info->dns) {
		ndbg("DNS: %s\n", info->dns);

		dns_addr.sin_addr.s_addr = inet_addr(info->dns);
		dns_addr.sin_family = AF_INET;

		ret = netlib_setdnsserver((struct sockaddr *)&dns_addr, -1);
		if (ret < 0) {
			ndbg("set dns server fail (%d)\n", ret);
		}
	}

	return ret;
}

int cmd_ifconfig(int argc, char **argv)
{
	int ret;
	struct ifconfig_cmd_info_s info;
	memset(&info, 0, sizeof(struct ifconfig_cmd_info_s));

	/* With one or no arguments, ifconfig simply shows the status of Ethernet
	 * device:
	 *
	 *   ifconfig
	 *   ifconfig [nic_name]
	 */

	if (argc <= 2) {
		nic_display_state();
		return OK;
	}

	/* If both the network interface name and an IP address are supplied as
	 * arguments, then ifconfig will set the address of the Ethernet device:
	 *
	 *    ifconfig nic_name ip_address
	 */
	ret = _cmd_ifconfig_parse(argc, argv, &info);
	if (ret != OK) {
		return ret;
	}

#ifdef CONFIG_NET_ETHERNET
	/* MAC address setting */
	ret = _cmd_ifconfig_setmacaddr(&info);
	if (ret != OK) {
		return ret;
	}
#endif

	/* IP addr setting */
	ret = _cmd_ifconfig_setipaddr(&info);
	if (ret != OK) {
		return ret;
	}

	/* DNS server setting */
	ret = _cmd_ifconfig_setdns(&info);
	if (ret != OK) {
		return ret;
	}

	return OK;
}

const static tash_cmdlist_t net_utilcmds[] = {
#ifdef CONFIG_NETUTILS_DHCPD
	{"dhcpd", cmd_dhcpd, TASH_EXECMD_SYNC},
#endif
	{"ifconfig", cmd_ifconfig, TASH_EXECMD_SYNC},
	{"ifdown", cmd_ifdown, TASH_EXECMD_SYNC},
	{"ifup", cmd_ifup, TASH_EXECMD_SYNC},
#ifdef CONFIG_NETUTILS_STATS_DISPLAY_TOOL
	{"net_stats", cmd_netstats, TASH_EXECMD_ASYNC},
#endif
#ifdef CONFIG_NET_NETMON
	{"netmon", cmd_netmon, TASH_EXECMD_ASYNC},
#endif
#ifdef CONFIG_NET_PING_CMD
	{"ping", cmd_ping, TASH_EXECMD_ASYNC},
#ifdef CONFIG_NET_IPv6
	{"ping6", cmd_ping6, TASH_EXECMD_ASYNC},
#endif
#endif
#ifdef CONFIG_NETUTILS_TFTPC
	{"tftpc", cmd_tftpc, TASH_EXECMD_SYNC},
#endif
	{NULL, NULL, 0}
};

const static tash_cmdlist_t net_appcmds[] = {
#ifdef CONFIG_SYSTEM_NETDB
	{"netdb", netdb_main, TASH_EXECMD_SYNC},
#endif
	{NULL, NULL, 0}
};

void net_register_utilcmds(void)
{
	tash_cmdlist_install(net_utilcmds);
}

void net_register_appcmds(void)
{
	tash_cmdlist_install(net_appcmds);
}
