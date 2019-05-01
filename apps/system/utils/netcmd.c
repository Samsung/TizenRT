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
#include <netinet/in.h>
#include <netinet/ether.h>

#include <tinyara/clock.h>
#include <tinyara/net/net.h>
#include <net/lwip/netif.h>
//#include <net/lwip/dhcp.h>
//#include <net/lwip/stats.h>
#include <tinyara/net/ip.h>

#ifdef CONFIG_NETUTILS_NETLIB
#include <netutils/netlib.h>
#endif

#include <protocols/tftp.h>

#ifdef CONFIG_HAVE_GETHOSTBYNAME
#include <netdb.h>
#endif
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

#ifdef CONFIG_SYSTEM_NETDB
extern int netdb_main(int argc, char *argv[]);
#endif

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
#ifdef CONFIG_NET_IPv6_NUM_ADDRESSES
	int j;
	struct netif *netif;
#endif

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
#ifdef CONFIG_NET_IPv6_NUM_ADDRESSES
		netif = netif_find(ifr->ifr_name);
		for (j = 0; netif != NULL && j < CONFIG_NET_IPv6_NUM_ADDRESSES; j++) {
			if (netif->ip6_addr_state[j] != 0) {
				printf("\tinet6 addr: %s\n", ip6addr_ntoa(ip_2_ip6(&netif->ip6_addr[j])));
			}
		}
#else
		printf("\n");
#endif /* CONFIG_NET_IPv6_NUM_ADDRESSES */
		printf("\n");
	}
DONE:
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

int cmd_ifconfig(int argc, char **argv)
{
	struct in_addr addr;
	in_addr_t gip = 0;
	int i;
	FAR char *intf = NULL;
	FAR char *hostip = NULL;
	FAR char *gwip = NULL;
	FAR char *mask = NULL;
	FAR char *tmp = NULL;
#ifdef CONFIG_NET_ETHERNET
	FAR char *hw = NULL;
#endif
	FAR char *dns = NULL;
	bool badarg = false;
	uint8_t mac[IFHWADDRLEN];

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

	if (argc > 2) {
		for (i = 0; i < argc; i++) {
			if (i == 1) {
				intf = argv[i];
			} else if (i == 2) {
				hostip = argv[i];
			} else {
				tmp = argv[i];
				if (!strcmp(tmp, "dr") || !strcmp(tmp, "gw") || !strcmp(tmp, "gateway")) {
					if (argc - 1 >= i + 1) {
						gwip = argv[i + 1];
						i++;
					} else {
						badarg = true;
					}
				} else if (!strcmp(tmp, "netmask")) {
					if (argc - 1 >= i + 1) {
						mask = argv[i + 1];
						i++;
					} else {
						badarg = true;
					}
				}
#ifdef CONFIG_NET_ETHERNET
				/* REVISIT: How will we handle Ethernet and SLIP networks together? */

				else if (!strcmp(tmp, "hw")) {
					if (argc - 1 >= i + 1) {
						hw = argv[i + 1];
						i++;
						badarg = !netlib_hwmacconv(hw, mac);
					} else {
						badarg = true;
					}
				}
#endif
				else if (!strcmp(tmp, "dns")) {
					if (argc - 1 >= i + 1) {
						dns = argv[i + 1];
						i++;
					} else {
						badarg = true;
					}
				}
			}
		}
	}

	if (badarg) {
		printf(fmtargrequired, argv[0]);
		return ERROR;
	}
#ifdef CONFIG_NET_ETHERNET
	/* Set Hardware Ethernet MAC address */
	/* REVISIT: How will we handle Ethernet and SLIP networks together? */

	if (hw) {
		ndbg("HW MAC: %s\n", hw);
		int ret = netlib_setmacaddr(intf, mac);
		if (ret < 0) {
			ndbg("Set mac address fail\n");
		}
	}
#endif

	if (hostip != NULL) {
		if (!strcmp(hostip, "dhcp")) {
			/* Set DHCP addr */

			ndbg("DHCPC Mode\n");
			gip = addr.s_addr = 0;
			netlib_set_ipv4addr(intf, &addr);

		} else {
			/* Set host IP address */
			ndbg("Host IP: %s\n", hostip);

			if (strstr(hostip, ".") != NULL) {
				gip = addr.s_addr = inet_addr(hostip);
				netlib_set_ipv4addr(intf, &addr);
			}
#ifdef CONFIG_NET_IPv6
			else if (strstr(hostip, ":") != NULL) {
				struct in6_addr addr6;
				inet_pton(AF_INET6, hostip, &addr6);
				netlib_set_ipv6addr(intf, &addr6);
			}
#endif /* CONFIG_NET_IPv6 */
			else {
				ndbg("hostip is not valid\n");

				return ERROR;
			}
		}
	} else {
		printf("hostip is not provided\n");
		return ERROR;
	}

	/* Get the MAC address of the NIC */
	if (!gip) {
		int ret;
		ret = netlib_getmacaddr(intf, mac);
		if (ret < 0) {
			ndbg("get mac  fail %s:%d\n", __FUNCTION__, __LINE__);
			return ERROR;
		}
		struct in_addr ip_check;

		ret = dhcp_client_start(intf);
		if (ret != OK) {
			ndbg("get IP address fail\n");
			return ERROR;
		}

		ret = netlib_get_ipv4addr(intf, &ip_check);
		if (ret != OK) {
			ndbg("get IP address fail\n");
			return ERROR;
		}
		ndbg("get IP address %s\n", inet_ntoa(ip_check));

		return OK;
	}

	/* Set gateway */
	if (gwip) {
		ndbg("Gateway: %s\n", gwip);
		gip = addr.s_addr = inet_addr(gwip);
	} else {
		if (gip) {
			ndbg("Gateway: default\n");
			gip = NTOHL(gip);
			gip &= ~0x000000ff;
			gip |= 0x00000001;
			gip = HTONL(gip);
		}

		addr.s_addr = gip;
	}
	int ret = netlib_set_dripv4addr(intf, &addr);
	if (ret < 0) {
		ndbg("Set IPv4 route fail %s:%d\n", __FUNCTION__, __LINE__);
	}
	/* Set network mask */
	if (mask) {
		ndbg("Netmask: %s\n", mask);
		addr.s_addr = inet_addr(mask);
	} else {
		ndbg("Netmask: Default\n");
		addr.s_addr = inet_addr("255.255.255.0");
	}
	ret = netlib_set_ipv4netmask(intf, &addr);
	if (ret < 0) {
		ndbg("Set IPv4 netmask fail %s:%d\n", __FUNCTION__, __LINE__);
	}
	if (dns) {
		ndbg("DNS: %s\n", dns);
		addr.s_addr = inet_addr(dns);
	} else {
		ndbg("DNS: Default\n");
		addr.s_addr = gip;
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
#ifdef NET_LWIP_STATS_DISPLAY
	{"lwip_stats", stats_display, TASH_EXECMD_ASYNC},
#endif
#ifdef CONFIG_NET_NETMON
	{"netmon", cmd_netmon, TASH_EXECMD_ASYNC},
#endif
#ifdef CONFIG_NET_PING_CMD
	{"ping", cmd_ping, TASH_EXECMD_ASYNC},
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
