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
/****************************************************************************
 * apps/tzshlib/tzsh_netcmds.c
 *
 *   Copyright (C) 2007-2012, 2014-2015, 2017 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#ifdef CONFIG_NET

#include <tinyara/compiler.h>

#include <sys/stat.h>			/* Needed for open */
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sched.h>
#include <fcntl.h>				/* Needed for open */
#include <dirent.h>
#include <libgen.h>				/* Needed for basename */
#include <errno.h>
#include <debug.h>

#if defined(CONFIG_LIBC_NETDB) && defined(CONFIG_NETDB_DNSCLIENT)
#ifndef CONFIG_TZSH_DISABLE_NSLOOKUP
#include <netdb.h>
#endif
#endif

#include <net/ethernet.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ether.h>

#include <tinyara/irq.h>
#include <tinyara/clock.h>
#include <tinyara/net/net.h>
#include <tinyara/net/netdev.h>
#include <tinyara/net/netstats.h>
#include <tinyara/net/ip.h>
#include <tinyara/net/icmp.h>
#include <tinyara/net/icmpv6.h>

#ifdef CONFIG_NET_6LOWPAN
#include <tinyara/net/sixlowpan.h>
#ifdef CONFIG_WIRELESS_PKTRADIO
#include <tinyara/wireless/pktradio.h>
#endif
#endif

#ifdef CONFIG_NETUTILS_NETLIB
#include "netutils/netlib.h"
#endif

#if defined(CONFIG_NET_UDP) && CONFIG_NFILE_DESCRIPTORS > 0
#include "netutils/netlib.h"
#if !defined(CONFIG_TZSH_DISABLE_GET) || !defined(CONFIG_TZSH_DISABLE_PUT)
#include "netutils/tftp.h"
#endif
#endif

#if defined(CONFIG_NET_TCP) && CONFIG_NFILE_DESCRIPTORS > 0
#ifndef CONFIG_TZSH_DISABLE_WGET
#include "netutils/webclient.h"
#endif
#endif

#if defined(CONFIG_TZSH_DHCPC) || defined(CONFIG_TZSH_DNS)
#include "netutils/dhcpc.h"
#endif

#include "tzsh.h"
#include "tzsh_console.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#undef HAVE_HWADDR
#undef HAVE_EADDR
#undef HAVE_RADIOADDR

#if defined(CONFIG_NET_ETHERNET)
#define HAVE_HWADDR      1
#elif defined(CONFIG_NET_6LOWPAN)
#if defined(CONFIG_WIRELESS_IEEE802154)
#define HAVE_HWADDR    1
#define HAVE_EADDR     1
#elif defined(CONFIG_WIRELESS_PKTRADIO)
#define HAVE_HWADDR    1
#define HAVE_RADIOADDR 1
#endif
#endif

/* Get the larger value */

#ifndef MAX
#define MAX(a,b) (a > b ? a : b)
#endif

/****************************************************************************
 * Private Types
 ****************************************************************************/

/* Describes the MAC address of the selected device */

#ifdef HAVE_HWADDR
#if defined(CONFIG_NET_ETHERNET)
typedef uint8_t mac_addr_t[IFHWADDRLEN];
#elif defined(HAVE_EADDR)
typedef uint8_t mac_addr_t[8];
#elif defined(HAVE_RADIOADDR)
typedef struct pktradio_addr_s mac_addr_t;
#endif
#endif

#if defined(CONFIG_NET_UDP) && CONFIG_NFILE_DESCRIPTORS > 0
struct tftpc_args_s {
	bool binary;				/* true:binary ("octet") false:text ("netascii") */
	bool allocated;				/* true: destpath is allocated */
	FAR char *destpath;			/* Path at destination */
	FAR const char *srcpath;	/* Path at src */
	in_addr_t ipaddr;			/* Host IP address */
};
#endif

typedef int (*tzsh_netdev_callback_t)(FAR struct tzsh_vtbl_s *vtbl, FAR char *devname);

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: net_statistics
 ****************************************************************************/

#if defined(CONFIG_NET_STATISTICS) && defined(CONFIG_FS_PROCFS) && \
	!defined(CONFIG_FS_PROCFS_EXCLUDE_NET) && \
	!defined(CONFIG_TZSH_DISABLE_IFCONFIG)
static inline void net_statistics(FAR struct tzsh_vtbl_s *vtbl)
{
	(void)tzsh_catfile(vtbl, "ifconfig", CONFIG_TZSH_PROC_MOUNTPOINT "/net/stat");
}
#else
#define net_statistics(vtbl)
#endif

/****************************************************************************
 * Name: ifconfig_callback
 ****************************************************************************/

#if !defined(CONFIG_TZSH_DISABLE_IFUPDOWN) || !defined(CONFIG_TZSH_DISABLE_IFCONFIG)
static int ifconfig_callback(FAR struct tzsh_vtbl_s *vtbl, FAR char *devname)
{
	char buffer[IFNAMSIZ + 12];

	DEBUGASSERT(vtbl != NULL && devname != NULL);

	/* Construct the full path to the /proc/net entry for this device */

	snprintf(buffer, IFNAMSIZ + 12, CONFIG_TZSH_PROC_MOUNTPOINT "/net/%s", devname);
	(void)tzsh_catfile(vtbl, "ifconfig", buffer);

	return OK;
}
#endif							/* !CONFIG_TZSH_DISABLE_IFUPDOWN || !CONFIG_TZSH_DISABLE_IFCONFIG */

/****************************************************************************
 * Name: tftpc_parseargs
 ****************************************************************************/

#if defined(CONFIG_NET_UDP) && CONFIG_NFILE_DESCRIPTORS > 0
int tftpc_parseargs(FAR struct tzsh_vtbl_s *vtbl, int argc, char **argv, struct tftpc_args_s *args)
{
	FAR const char *fmt = g_fmtarginvalid;
	bool badarg = false;
	int option;

	/* Get the get/put options */

	memset(args, 0, sizeof(struct tftpc_args_s));
	while ((option = getopt(argc, argv, ":bnf:h:")) != ERROR) {
		switch (option) {
		case 'b':
			args->binary = true;
			break;

		case 'n':
			args->binary = false;
			break;

		case 'f':
			args->destpath = optarg;
			break;

		case 'h':
			if (!netlib_ipv4addrconv(optarg, (FAR unsigned char *)&args->ipaddr)) {
				tzsh_output(vtbl, g_fmtarginvalid, argv[0]);
				badarg = true;
			}
			break;

		case ':':
			tzsh_output(vtbl, g_fmtargrequired, argv[0]);
			badarg = true;
			break;

		case '?':
		default:
			tzsh_output(vtbl, g_fmtarginvalid, argv[0]);
			badarg = true;
			break;
		}
	}

	/* If a bad argument was encountered, then return without processing the command */

	if (badarg) {
		return ERROR;
	}

	/* There should be exactly one parameter left on the command-line */

	if (optind == argc - 1) {
		args->srcpath = argv[optind];
	}

	/* optind == argc means that there is nothing left on the command-line */

	else if (optind >= argc) {
		fmt = g_fmtargrequired;
		goto errout;
	}

	/* optind < argc - 1 means that there are too many arguments on the
	 * command-line
	 */

	else {
		fmt = g_fmttoomanyargs;
		goto errout;
	}

	/* The HOST IP address is also required */

	if (args->ipaddr == 0) {
		fmt = g_fmtargrequired;
		goto errout;
	}

	/* If the destpath was not provided, then we have do a little work. */

	if (args->destpath == NULL) {
		FAR char *tmp1;
		FAR char *tmp2;

		/* Copy the srcpath... baseanme might modify it */

		fmt = g_fmtcmdoutofmemory;
		tmp1 = strdup(args->srcpath);
		if (tmp1 == NULL) {
			goto errout;
		}

		/* Get the basename of the srcpath */

		tmp2 = basename(tmp1);
		if (tmp2 == NULL) {
			free(tmp1);
			goto errout;
		}

		/* Use that basename as the destpath */

		args->destpath = strdup(tmp2);
		free(tmp1);
		if (args->destpath == NULL) {
			goto errout;
		}

		args->allocated = true;
	}

	return OK;

errout:
	tzsh_output(vtbl, fmt, argv[0]);
	return ERROR;
}
#endif

/****************************************************************************
 * Name: wget_callback
 ****************************************************************************/

#if defined(CONFIG_NET_TCP) && CONFIG_NFILE_DESCRIPTORS > 0
#ifndef CONFIG_TZSH_DISABLE_WGET
static void wget_callback(FAR char **buffer, int offset, int datend, FAR int *buflen, FAR void *arg)
{
	(void)write((int)((intptr_t) arg), &((*buffer)[offset]), datend - offset);
}
#endif
#endif

/****************************************************************************
 * Name: tzsh_foreach_netdev
 ****************************************************************************/

#if !defined(CONFIG_TZSH_DISABLE_IFUPDOWN) || !defined(CONFIG_TZSH_DISABLE_IFCONFIG)
static int tzsh_foreach_netdev(tzsh_netdev_callback_t callback, FAR struct tzsh_vtbl_s *vtbl, FAR char *cmd)
{
	FAR struct dirent *entry;
	FAR DIR *dir;
	int ret = OK;

	/* Open the /proc/net directory */

	dir = opendir(CONFIG_TZSH_PROC_MOUNTPOINT "/net");
	if (dir == NULL) {
		tzsh_output(vtbl, g_fmtcmdfailed, cmd, "opendir", TZSH_ERRNO);
		return ERROR;
	}

	/* Look for device configuration "regular" files */

	while ((entry = readdir(dir)) != NULL) {
		/* Skip anything that is not a regular file and skip the file
		 * /proc/dev/stat which does not correspond to a network driver.
		 */

		if (entry->d_type == DTYPE_FILE && strcmp(entry->d_name, "stat") != 0) {
			/* Performt he callback.  It returns any non-zero value, then
			 * terminate the serach.
			 */

			ret = callback(vtbl, entry->d_name);
			if (ret != OK) {
				break;
			}
		}
	}

	/* Close the directory */

	(void)closedir(dir);
	return ret;
}
#endif

/****************************************************************************
 * Name: tzsh_addrconv
 ****************************************************************************/

#ifdef HAVE_HWADDR
static inline bool tzsh_addrconv(FAR const char *hwstr, FAR mac_addr_t *macaddr)
{
	/* REVISIT: How will we handle Ethernet and SLIP networks together? */

#if defined(CONFIG_NET_ETHERNET)
	return !netlib_ethaddrconv(hwstr, *macaddr);
#elif defined(HAVE_EADDR)
	return !netlib_eaddrconv(hwstr, *macaddr);
#elif defined(HAVE_RADIOADDR)
	return !netlib_nodeaddrconv(hwstr, macaddr);
#else
	return -ENOSUPP;
#endif
}
#endif

/****************************************************************************
 * Name: tzsh_sethwaddr
 ****************************************************************************/

#ifdef HAVE_HWADDR
static inline void tzsh_sethwaddr(FAR const char *ifname, FAR mac_addr_t *macaddr)
{
#if defined(CONFIG_NET_ETHERNET)
	netlib_setmacaddr(ifname, *macaddr);
#elif defined(HAVE_EADDR)
	netlib_seteaddr(ifname, *macaddr);
#elif defined(HAVE_RADIOADDR)
	netlib_setnodeaddr(ifname, macaddr);
#endif
}
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: cmd_get
 ****************************************************************************/

#if defined(CONFIG_NET_UDP) && CONFIG_NFILE_DESCRIPTORS > 0
#ifndef CONFIG_TZSH_DISABLE_GET
int cmd_get(FAR struct tzsh_vtbl_s *vtbl, int argc, char **argv)
{
	struct tftpc_args_s args;
	FAR char *fullpath;

	/* Parse the input parameter list */

	if (tftpc_parseargs(vtbl, argc, argv, &args) != OK) {
		return ERROR;
	}

	/* Get the full path to the local file */

	fullpath = tzsh_getfullpath(vtbl, args.destpath);

	/* Then perform the TFTP get operation */

	if (tftpget(args.srcpath, fullpath, args.ipaddr, args.binary) != OK) {
		tzsh_output(vtbl, g_fmtcmdfailed, argv[0], "tftpget", TZSH_ERRNO);
	}

	/* Release any allocated memory */

	if (args.allocated) {
		free(args.destpath);
	}

	free(fullpath);
	return OK;
}
#endif
#endif

/****************************************************************************
 * Name: cmd_ifup
 ****************************************************************************/

#ifndef CONFIG_TZSH_DISABLE_IFUPDOWN
int cmd_ifup(FAR struct tzsh_vtbl_s *vtbl, int argc, char **argv)
{
	FAR char *ifname = NULL;
	int ret;

	if (argc != 2) {
		tzsh_output(vtbl, "Please select ifname:\n");
		return tzsh_foreach_netdev(ifconfig_callback, vtbl, "ifup");
	}

	ifname = argv[1];
	ret = netlib_ifup(ifname);
	tzsh_output(vtbl, "ifup %s...%s\n", ifname, (ret == OK) ? "OK" : "Failed");
	return ret;
}
#endif

/****************************************************************************
 * Name: cmd_ifdown
 ****************************************************************************/

#ifndef CONFIG_TZSH_DISABLE_IFUPDOWN
int cmd_ifdown(FAR struct tzsh_vtbl_s *vtbl, int argc, char **argv)
{
	FAR char *ifname = NULL;
	int ret;

	if (argc != 2) {
		tzsh_output(vtbl, "Please select ifname:\n");
		return tzsh_foreach_netdev(ifconfig_callback, vtbl, "ifdown");
	}

	ifname = argv[1];
	ret = netlib_ifdown(ifname);
	tzsh_output(vtbl, "ifdown %s...%s\n", ifname, (ret == OK) ? "OK" : "Failed");
	return ret;
}
#endif

/****************************************************************************
 * Name: cmd_ifconfig
 ****************************************************************************/

#ifndef CONFIG_TZSH_DISABLE_IFCONFIG
int cmd_ifconfig(FAR struct tzsh_vtbl_s *vtbl, int argc, char **argv)
{
#ifdef CONFIG_NET_IPv4
	struct in_addr addr;
	in_addr_t gip = INADDR_ANY;
#endif
#ifdef CONFIG_NET_IPv6
	struct in6_addr addr6;
#endif
	int i;
	FAR char *ifname = NULL;
	FAR char *hostip = NULL;
	FAR char *gwip = NULL;
	FAR char *mask = NULL;
	FAR char *tmp = NULL;
#ifdef HAVE_HWADDR
	FAR char *hw = NULL;
#endif
#if defined(CONFIG_TZSH_DHCPC) || defined(CONFIG_TZSH_DNS)
	FAR char *dns = NULL;
#endif
#if defined(CONFIG_NET_IPv4) && defined(CONFIG_NET_IPv6)
	bool inet6 = false;
#endif
	bool missingarg = true;
	bool badarg = false;
#ifdef HAVE_HWADDR
	mac_addr_t macaddr;
#endif
#if defined(CONFIG_TZSH_DHCPC)
	FAR void *handle;
#endif
	int ret;

	/* With one or no arguments, ifconfig simply shows the status of the
	 * network device:
	 *
	 *   ifconfig
	 *   ifconfig [interface]
	 */

	if (argc <= 2) {
		ret = tzsh_foreach_netdev(ifconfig_callback, vtbl, "ifconfig");
		if (ret < 0) {
			return ERROR;
		}

		net_statistics(vtbl);
		return OK;
	}

	/* If both the network interface name and an IP address are supplied as
	 * arguments, then ifconfig will set the address of the Ethernet device:
	 *
	 *    ifconfig ifname [ip_address] [named options]
	 */

	if (argc > 2) {
		for (i = 1; i < argc; i++) {
			if (i == 1) {
				ifname = argv[i];
				missingarg = false;
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
				} else if (!strcmp(tmp, "inet")) {
#if defined(CONFIG_NET_IPv4) && defined(CONFIG_NET_IPv6)
					inet6 = false;
#elif !defined(CONFIG_NET_IPv4)
					badarg = true;
#endif
				} else if (!strcmp(tmp, "inet6")) {
#if defined(CONFIG_NET_IPv4) && defined(CONFIG_NET_IPv6)
					inet6 = true;
#elif !defined(CONFIG_NET_IPv6)
					badarg = true;
#endif
				}
#ifdef HAVE_HWADDR
				/* REVISIT: How will we handle Ethernet and SLIP networks together? */

				else if (!strcmp(tmp, "hw")) {
					if (argc - 1 >= i + 1) {
						hw = argv[i + 1];
						i++;

						badarg = tzsh_addrconv(hw, &macaddr);
					} else {
						badarg = true;
					}
				}
#endif

#if defined(CONFIG_TZSH_DHCPC) || defined(CONFIG_TZSH_DNS)
				else if (!strcmp(tmp, "dns")) {
					if (argc - 1 >= i + 1) {
						dns = argv[i + 1];
						i++;
					} else {
						badarg = true;
					}
				}
#endif
				else if (i == 2) {
					hostip = tmp;
				} else {
					badarg = true;
				}
			}
		}
	}

	if (missingarg) {
		tzsh_output(vtbl, g_fmtargrequired, argv[0]);
		return ERROR;
	}

	if (badarg) {
		tzsh_output(vtbl, g_fmtarginvalid, argv[0]);
		return ERROR;
	}
#ifdef HAVE_HWADDR
	/* Set Hardware Ethernet MAC address */
	/* REVISIT: How will we handle Ethernet and SLIP networks together? */

	if (hw != NULL) {
		ninfo("HW MAC: %s\n", hw);
		tzsh_sethwaddr(ifname, &macaddr);
	}
#endif

	/* Set IP address */

#ifdef CONFIG_NET_IPv6
#ifdef CONFIG_NET_IPv4
	if (inet6)
#endif
	{
		if (hostip != NULL) {
			/* REVISIT: Should DHCPC check be used here too? */

			ninfo("Host IP: %s\n", hostip);
			inet_pton(AF_INET6, hostip, &addr6);
		}

		netlib_set_ipv6addr(ifname, &addr6);
	}
#endif							/* CONFIG_NET_IPv6 */

#ifdef CONFIG_NET_IPv4
#ifdef CONFIG_NET_IPv6
	else
#endif
	{
		if (hostip != NULL) {
#if defined(CONFIG_TZSH_DHCPC)
			if (strcmp(hostip, "dhcp") == 0) {
				/* Set DHCP addr */

				ninfo("DHCPC Mode\n");
				addr.s_addr = 0;
				gip = 0;
			} else
#endif
			{
				/* Set host IP address */

				ninfo("Host IP: %s\n", hostip);
				addr.s_addr = inet_addr(hostip);
				gip = addr.s_addr;
			}
		}

		netlib_set_ipv4addr(ifname, &addr);
	}
#endif							/* CONFIG_NET_IPv4 */

	/* Set gateway */

#ifdef CONFIG_NET_IPv6
#ifdef CONFIG_NET_IPv4
	if (inet6)
#endif
	{
		/* Only set the gateway address if it was explicitly provided. */

		if (gwip != NULL) {
			ninfo("Gateway: %s\n", gwip);
			inet_pton(AF_INET6, gwip, &addr6);

			netlib_set_dripv6addr(ifname, &addr6);
		}
	}
#endif							/* CONFIG_NET_IPv6 */

#ifdef CONFIG_NET_IPv4
#ifdef CONFIG_NET_IPv6
	else
#endif
	{
		if (gwip != NULL) {
			ninfo("Gateway: %s\n", gwip);
			gip = addr.s_addr = inet_addr(gwip);
		} else {
			if (gip != 0) {
				ninfo("Gateway: default\n");
				gip = NTOHL(gip);
				gip &= ~0x000000ff;
				gip |= 0x00000001;
				gip = HTONL(gip);
			}

			addr.s_addr = gip;
		}

		netlib_set_dripv4addr(ifname, &addr);
	}
#endif							/* CONFIG_NET_IPv4 */

	/* Set network mask */

#ifdef CONFIG_NET_IPv6
#ifdef CONFIG_NET_IPv4
	if (inet6)
#endif
	{
		if (mask != NULL) {
			ninfo("Netmask: %s\n", mask);
			inet_pton(AF_INET6, mask, &addr6);
		} else {
			ninfo("Netmask: Default\n");
			inet_pton(AF_INET6, "ffff:ffff:ffff:ffff::", &addr6);
		}

		netlib_set_ipv6netmask(ifname, &addr6);
	}
#endif							/* CONFIG_NET_IPv6 */

#ifdef CONFIG_NET_IPv4
#ifdef CONFIG_NET_IPv6
	else
#endif
	{
		if (mask != NULL) {
			ninfo("Netmask: %s\n", mask);
			addr.s_addr = inet_addr(mask);
		} else {
			ninfo("Netmask: Default\n");
			addr.s_addr = inet_addr("255.255.255.0");
		}

		netlib_set_ipv4netmask(ifname, &addr);
	}
#endif							/* CONFIG_NET_IPv4 */

	UNUSED(ifname);				/* Not used in all configurations */

#if defined(CONFIG_TZSH_DHCPC) || defined(CONFIG_TZSH_DNS)
#ifdef CONFIG_NET_IPv6
#ifdef CONFIG_NET_IPv4
	if (inet6)
#endif
	{
#warning Missing Logic
	}
#endif							/* CONFIG_NET_IPv6 */

#ifdef CONFIG_NET_IPv4
#ifdef CONFIG_NET_IPv6
	else
#endif
	{
		if (dns != NULL) {
			ninfo("DNS: %s\n", dns);
			addr.s_addr = inet_addr(dns);
		} else {
			ninfo("DNS: Default\n");
			addr.s_addr = gip;
		}

		netlib_set_ipv4dnsaddr(&addr);
	}
#endif							/* CONFIG_NET_IPv4 */
#endif							/* CONFIG_TZSH_DHCPC || CONFIG_TZSH_DNS */

#if defined(CONFIG_TZSH_DHCPC)
	/* Get the MAC address of the NIC */

	if (!gip) {
		netlib_getmacaddr("eth0", macaddr);

		/* Set up the DHCPC modules */

		handle = dhcpc_open("eth0", &macaddr, IFHWADDRLEN);

		/* Get an IP address.  Note that there is no logic for renewing the IP
		 * address in this example.  The address should be renewed in
		 * ds.lease_time/2 seconds.
		 */

		if (handle != NULL) {
			struct dhcpc_state ds;

			(void)dhcpc_request(handle, &ds);
			netlib_set_ipv4addr("eth0", &ds.ipaddr);

			if (ds.netmask.s_addr != 0) {
				netlib_set_ipv4netmask("eth0", &ds.netmask);
			}

			if (ds.default_router.s_addr != 0) {
				netlib_set_dripv4addr("eth0", &ds.default_router);
			}

			if (ds.dnsaddr.s_addr != 0) {
				netlib_set_ipv4dnsaddr(&ds.dnsaddr);
			}

			dhcpc_close(handle);
		}
	}
#endif

#if !defined(CONFIG_NET_IPv4) && !defined(CONFIG_NET_IPv6)
	UNUSED(hostip);
	UNUSED(mask);
	UNUSED(gwip);
#endif
#ifdef CONFIG_NET_IPv4
	UNUSED(gip);
#endif

	return OK;
}
#endif

/****************************************************************************
 * Name: cmd_nslookup
 ****************************************************************************/

#if defined(CONFIG_LIBC_NETDB) && defined(CONFIG_NETDB_DNSCLIENT) && \
	!defined(CONFIG_TZSH_DISABLE_NSLOOKUP)
int cmd_nslookup(FAR struct tzsh_vtbl_s *vtbl, int argc, char **argv)
{
	FAR struct hostent *host;
	FAR const char *addrtype;
	char buffer[48];

	/* We should be guaranteed this by the command line parser */

	DEBUGASSERT(argc == 2);

	/* Get the matching address + any aliases */

	host = gethostbyname(argv[1]);
	if (!host) {
		/* REVISIT: gethostbyname() does not set errno, but h_errno */

		tzsh_output(vtbl, g_fmtcmdfailed, argv[0], "gethostbyname", TZSH_ERRNO);
		return ERROR;
	}

	/* Convert the address to a string */
	/* Handle IPv4 addresses */

	if (host->h_addrtype == AF_INET) {
		if (inet_ntop(AF_INET, host->h_addr, buffer, 48) == NULL) {
			tzsh_output(vtbl, g_fmtcmdfailed, argv[0], "inet_ntop", TZSH_ERRNO);
			return ERROR;
		}

		addrtype = "IPv4";
	}

	/* Handle IPv6 addresses */

	else {						/* if (host->h_addrtype == AF_INET6) */

		DEBUGASSERT(host->h_addrtype == AF_INET6);

		if (inet_ntop(AF_INET6, host->h_addr, buffer, 48) == NULL) {
			tzsh_output(vtbl, g_fmtcmdfailed, argv[0], "inet_ntop", TZSH_ERRNO);
			return ERROR;
		}

		addrtype = "IPv6";
	}

	/* Print the host name / address mapping */

	tzsh_output(vtbl, "Host: %s  %s Addr: %s\n", host->h_name, addrtype, buffer);

	/* Print any host name aliases */

	if (host->h_aliases != NULL && *host->h_aliases != NULL) {
		FAR char **alias;

		tzsh_output(vtbl, "Aliases:");
		for (alias = host->h_aliases; *alias != NULL; alias++) {
			tzsh_output(vtbl, " %s", *alias);
		}

		tzsh_output(vtbl, "\n");
	}

	return OK;
}
#endif

/****************************************************************************
 * Name: cmd_arp
 ****************************************************************************/

#if defined(CONFIG_NET_ARP) && !defined(CONFIG_TZSH_DISABLE_ARP)
int cmd_arp(FAR struct tzsh_vtbl_s *vtbl, int argc, char **argv)
{
	struct sockaddr_in inaddr;
	struct ether_addr mac;
	int ret;

	/* Forms:
	 *
	 * aap -a <ipaddr>
	 * arp -d <ipdaddr>
	 * arp -s <ipaddr> <hwaddr>
	 */

	if (strcmp(argv[1], "-a") == 0) {
		if (argc != 3) {
			goto errout_toomany;
		}

		/* Show the corresponding hardware address */

		inaddr.sin_family = AF_INET;
		inaddr.sin_port = 0;
		inaddr.sin_addr.s_addr = inet_addr(argv[2]);

		ret = netlib_get_arpmapping(&inaddr, mac.ether_addr_octet);
		if (ret < 0) {
			goto errout_cmdfaild;
		}

		tzsh_output(vtbl, "HWaddr: %s\n", ether_ntoa(&mac));
	} else if (strcmp(argv[1], "-d") == 0) {
		if (argc != 3) {
			goto errout_toomany;
		}

		/* Delete the corresponding address mapping from the arp table */

		inaddr.sin_family = AF_INET;
		inaddr.sin_port = 0;
		inaddr.sin_addr.s_addr = inet_addr(argv[2]);

		ret = netlib_del_arpmapping(&inaddr);
		if (ret < 0) {
			goto errout_cmdfaild;
		}
	} else if (strcmp(argv[1], "-s") == 0) {
		if (argc != 4) {
			goto errout_missing;
		}

		/* Convert the MAC address string to a binary */

		if (!netlib_ethaddrconv(argv[3], mac.ether_addr_octet)) {
			goto errout_invalid;
		}

		/* Add the address mapping to the arp table */

		inaddr.sin_family = AF_INET;
		inaddr.sin_port = 0;
		inaddr.sin_addr.s_addr = inet_addr(argv[2]);

		ret = netlib_set_arpmapping(&inaddr, mac.ether_addr_octet);
		if (ret < 0) {
			goto errout_cmdfaild;
		}
	} else {
		goto errout_invalid;
	}

	return OK;

	/* Error exits */

errout_cmdfaild:
	if (ret == -ENOENT) {
		tzsh_output(vtbl, g_fmtnosuch, argv[0], "ARP entry", argv[2]);
	} else {
		tzsh_output(vtbl, g_fmtcmdfailed, argv[0], "ioctl", TZSH_ERRNO);
	}

	return ERROR;

errout_missing:
	tzsh_output(vtbl, g_fmttoomanyargs, argv[0]);
	return ERROR;

errout_toomany:
	tzsh_output(vtbl, g_fmtargrequired, argv[0]);
	return ERROR;

errout_invalid:
	tzsh_output(vtbl, g_fmtarginvalid, argv[0]);
	return ERROR;
}
#endif

/****************************************************************************
 * Name: cmd_put
 ****************************************************************************/

#if defined(CONFIG_NET_UDP) && CONFIG_NFILE_DESCRIPTORS > 0
#ifndef CONFIG_TZSH_DISABLE_PUT
int cmd_put(FAR struct tzsh_vtbl_s *vtbl, int argc, char **argv)
{
	struct tftpc_args_s args;
	FAR char *fullpath;

	/* Parse the input parameter list */

	if (tftpc_parseargs(vtbl, argc, argv, &args) != OK) {
		return ERROR;
	}

	/* Get the full path to the local file */

	fullpath = tzsh_getfullpath(vtbl, args.srcpath);

	/* Then perform the TFTP put operation */

	if (tftpput(fullpath, args.destpath, args.ipaddr, args.binary) != OK) {
		tzsh_output(vtbl, g_fmtcmdfailed, argv[0], "tftpput", TZSH_ERRNO);
	}

	/* Release any allocated memory */

	if (args.allocated) {
		free(args.destpath);
	}

	free(fullpath);
	return OK;
}
#endif
#endif

/****************************************************************************
 * Name: cmd_wget
 ****************************************************************************/

#if defined(CONFIG_NET_TCP) && CONFIG_NFILE_DESCRIPTORS > 0
#ifndef CONFIG_TZSH_DISABLE_WGET
int cmd_wget(FAR struct tzsh_vtbl_s *vtbl, int argc, char **argv)
{
	FAR char *localfile = NULL;
	FAR char *allocfile = NULL;
	FAR char *buffer = NULL;
	FAR char *fullpath = NULL;
	FAR char *url;
	FAR const char *fmt;
	bool badarg = false;
	int option;
	int fd = -1;
	int ret;

	/* Get the wget options */

	while ((option = getopt(argc, argv, ":o:")) != ERROR) {
		switch (option) {
		case 'o':
			localfile = optarg;
			break;

		case ':':
			tzsh_output(vtbl, g_fmtargrequired, argv[0]);
			badarg = true;
			break;

		case '?':
		default:
			tzsh_output(vtbl, g_fmtarginvalid, argv[0]);
			badarg = true;
			break;
		}
	}

	/* If a bad argument was encountered, then return without processing the
	 * command
	 */

	if (badarg) {
		return ERROR;
	}

	/* There should be exactly on parameter left on the command-line */

	if (optind == argc - 1) {
		url = argv[optind];
	} else if (optind >= argc) {
		fmt = g_fmttoomanyargs;
		goto errout;
	} else {
		fmt = g_fmtargrequired;
		goto errout;
	}

	/* Get the local file name */

	if (localfile == NULL) {
		allocfile = strdup(url);
		localfile = basename(allocfile);
	}

	/* Get the full path to the local file */

	fullpath = tzsh_getfullpath(vtbl, localfile);

	/* Open the local file for writing */

	fd = open(fullpath, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0) {
		tzsh_output(vtbl, g_fmtcmdfailed, argv[0], "open", TZSH_ERRNO);
		ret = ERROR;
		goto exit;
	}

	/* Allocate an I/O buffer */

	buffer = malloc(512);
	if (buffer == NULL) {
		fmt = g_fmtcmdoutofmemory;
		goto errout;
	}

	/* And perform the wget */

	ret = wget(url, buffer, 512, wget_callback, (FAR void *)((intptr_t) fd));
	if (ret < 0) {
		tzsh_output(vtbl, g_fmtcmdfailed, argv[0], "wget", TZSH_ERRNO);
		goto exit;
	}

	/* Free allocated resources */

exit:
	if (fd >= 0) {
		close(fd);
	}

	if (allocfile != NULL) {
		free(allocfile);
	}

	if (fullpath != NULL) {
		free(fullpath);
	}

	if (buffer != NULL) {
		free(buffer);
	}

	return ret;

errout:
	tzsh_output(vtbl, fmt, argv[0]);
	ret = ERROR;
	goto exit;
}
#endif
#endif

#endif							/* CONFIG_NET */
