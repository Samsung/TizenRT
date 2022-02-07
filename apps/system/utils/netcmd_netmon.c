/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
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
#include <stdio.h>
#include <errno.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <queue.h>
#include <netinet/in.h>
#include <net/if.h>
#include <netutils/netlib.h>
#ifdef CONFIG_WIFI_MANAGER
#include <wifi_manager/wifi_manager.h>
#endif
#include "netcmd_log.h"

/****************************************************************************
 * Preprocessor Definitions
 ****************************************************************************/
#define NTAG "[NETCMD]"
#define USAGE									\
	"\n usage: netmon [options]\n"				\
	"\n socket information:\n"					\
	"       netmon sock\n"						\
	"\n WiFi Manager stats:\n"					\
	"       netmon wifi\n"						\
	"\n Net device stats:\n"					\
	"       netmon [devname]\n\n"

#ifdef CONFIG_NET_IPv6
static inline void PRINT_IPV(struct netmon_sock *sock)
{
	if (sock->type & NETMON_TYPE_IPV6) {
		NETCMD_LOG(NTAG, "IPv6 (not yet supported)\t");
	} else {
		NETCMD_LOG(NTAG, "IPv4\t");
		NETCMD_LOG(NTAG, "%-15s:", inet_ntoa(sock->local.ip.sin_addr));
		if (sock->local.ip.sin_port == 0) {
			NETCMD_LOG(NTAG, "%-8s", "*");
		} else {
			NETCMD_LOG(NTAG, "%-8d", sock->local.ip.sin_port);
		}
		NETCMD_LOG(NTAG, "%-15s:", inet_ntoa(sock->remote.ip.sin_addr));
		if (sock->remote.ip.sin_port == 0) {
			NETCMD_LOG(NTAG, "%-8s", "*");
		} else {
			NETCMD_LOG(NTAG, "%-8d", sock->remote.ip.sin_port);
		}
	}
}
#else
static inline void PRINT_IPV(struct netmon_sock *sock)
{
	NETCMD_LOG(NTAG, "IPv4\t");
	NETCMD_LOG(NTAG, "%-15s:", inet_ntoa(sock->local.ip.sin_addr));
	if (sock->local.ip.sin_port == 0) {
		NETCMD_LOG(NTAG, "%-8s", "*");
	} else {
		NETCMD_LOG(NTAG, "%-8d", sock->local.ip.sin_port);
	}
	NETCMD_LOG(NTAG, "%-15s:", inet_ntoa(sock->remote.ip.sin_addr));
	if (sock->remote.ip.sin_port == 0) {
		NETCMD_LOG(NTAG, "%-8s", "*");
	} else {
		NETCMD_LOG(NTAG, "%-8d", sock->remote.ip.sin_port);
	}
}
#endif

/****************************************************************************
 * Global Data
 ****************************************************************************/


/****************************************************************************
 * Private Functions
 ****************************************************************************/
#ifdef CONFIG_NET_IPv6
/*
 * To Do: _netmon_ip6addr_ntoa_r is a temporary function to show ipv6
 * this has to be changed as a socket interface to transform in_addr to string
 */
#define xchar(i)             ((i) < 10 ? '0' + (i) : 'A' + (i) - 10)
static char *_netmon_ip6addr_ntoa_r(const ip6_addr_t *addr, char *buf, int buflen)
{
	u32_t current_block_index;
	u32_t current_block_value;
	u32_t next_block_value;
	s32_t i = 0;
	u8_t zero_flag = 0;
	u8_t empty_block_flag = 0;

	for (current_block_index = 0; current_block_index < 8; current_block_index++) {
		/* get the current 16-bit block */
		current_block_value = htonl(addr->addr[current_block_index >> 1]);
		if ((current_block_index & 0x1) == 0) {
			current_block_value = current_block_value >> 16;
		}
		current_block_value &= 0xffff;

		/* Check for empty block. */
		if (current_block_value == 0) {
			if (current_block_index == 7 && empty_block_flag == 1) {
				/* special case, we must render a ':' for the last block. */
				buf[i++] = ':';
				if (i >= buflen) {
					return NULL;
				}
				break;
			}
			if (empty_block_flag == 0) {
				/* generate empty block "::", but only if more than one contiguous zero block,
				 * according to current formatting suggestions RFC 5952. */
				next_block_value = htonl(addr->addr[(current_block_index + 1) >> 1]);
				if ((current_block_index & 0x1) == 0x01) {
					next_block_value = next_block_value >> 16;
				}
				next_block_value &= 0xffff;
				if (next_block_value == 0) {
					empty_block_flag = 1;
					buf[i++] = ':';
					if (i >= buflen) {
						return NULL;
					}
					continue;	/* move on to next block. */
				}
			} else if (empty_block_flag == 1) {
				/* move on to next block. */
				continue;
			}
		} else if (empty_block_flag == 1) {
			/* Set this flag value so we don't produce multiple empty blocks. */
			empty_block_flag = 2;
		}

		if (current_block_index > 0) {
			buf[i++] = ':';
			if (i >= buflen) {
				return NULL;
			}
		}

		if ((current_block_value & 0xf000) == 0) {
			zero_flag = 1;
		} else {
			buf[i++] = xchar(((current_block_value & 0xf000) >> 12));
			zero_flag = 0;
			if (i >= buflen) {
				return NULL;
			}
		}

		if (((current_block_value & 0xf00) == 0) && (zero_flag)) {
			/* do nothing */
		} else {
			buf[i++] = xchar(((current_block_value & 0xf00) >> 8));
			zero_flag = 0;
			if (i >= buflen) {
				return NULL;
			}
		}

		if (((current_block_value & 0xf0) == 0) && (zero_flag)) {
			/* do nothing */
		} else {
			buf[i++] = xchar(((current_block_value & 0xf0) >> 4));
			zero_flag = 0;
			if (i >= buflen) {
				return NULL;
			}
		}

		buf[i++] = xchar((current_block_value & 0xf));
		if (i >= buflen) {
			return NULL;
		}
	}

	buf[i] = 0;

	return buf;
}

/*
 * To Do: _netmon_ip6addr_ntoa is temporary functions to show ipv6
 * this has to be changed to a socket interface to transform in_addr to string
 */
static char *_netmon_ip6addr_ntoa(const ip6_addr_t *addr)
{
	static char str[40];
	return _netmon_ip6addr_ntoa_r(addr, str, 40);
}
#endif

/**
 * Print the command list
 */
static inline void print_help(void)
{
	NETCMD_LOG(NTAG, "%s", USAGE);
	return;
}

/**
 * Print a externally used socket information.
 */
static void print_socket(sq_queue_t *q_sock)
{
	int i = 1;
	struct netmon_sock *sock_info = (struct netmon_sock *) sq_remfirst(q_sock);
	NETCMD_LOG(NTAG, "\nSockfd\tProto\tType\tLocal\t\t\tRemote\t\t\tstate\tPID\n");
	while (sock_info) {
		NETCMD_LOG(NTAG, "%-8d", i);
		/* Protocol */
		if (sock_info->type & NETMON_TCP) {
			NETCMD_LOG(NTAG, "TCP\t");
			PRINT_IPV(sock_info);
		} else if (sock_info->type & NETMON_UDP) {
			NETCMD_LOG(NTAG, "UDP\t");
			PRINT_IPV(sock_info);
		} else {
			NETCMD_LOG(NTAG, "RAW\t");
			PRINT_IPV(sock_info);
		}
		switch (sock_info->state) {
		case NETMON_WRITE:
			NETCMD_LOG(NTAG, "WRITE\t");
			break;
		case NETMON_LISTEN:
			NETCMD_LOG(NTAG, "LISTEN\t");
			break;
		case NETMON_CONNECT:
			NETCMD_LOG(NTAG, "CONNECT\t");
			break;
		case NETMON_CLOSE:
			NETCMD_LOG(NTAG, "CLOSE\t");
			break;
		default:
			NETCMD_LOG(NTAG, "NONE\t");
			break;
		}
		NETCMD_LOG(NTAG, "%d:%s\n", sock_info->pid, sock_info->pid_name);
		i++;
		free(sock_info);
		sock_info = (struct netmon_sock *) sq_remfirst(q_sock);
	}

	return;
}

#ifdef CONFIG_NET_STATS
/**
 * Print a externally used netdev information.
 */
static void print_devstats(struct netmon_netdev_stats *stats)
{
	NETCMD_LOG(NTAG, "\n==============================================\n");
	NETCMD_LOG(NTAG, "IFNAME    RXbyte    RXPKT    TXbyte    TXPKT\n");
	NETCMD_LOG(NTAG, "----------------------------------------------\n");
	NETCMD_LOG(NTAG, "%-10s%-10d%-9d", stats->devname, stats->devinoctets, stats->devinpkts);
	NETCMD_LOG(NTAG, "%-10d%-9d\n", stats->devoutoctets, stats->devoutpkts);
	NETCMD_LOG(NTAG, "==============================================\n");
	return;
}
#endif							/* CONFIG_NET_STATS */

static inline int _print_wifi_info(void)
{
#ifdef CONFIG_WIFI_MANAGER
	wifi_manager_stats_s stats;
	wifi_manager_info_s info;

	wifi_manager_result_e res = wifi_manager_get_stats(&stats);
	if (res != WIFI_MANAGER_SUCCESS) {
		NETCMD_LOG(NTAG, "Get Wi-Fi Manager stats failed\n");
		return ERROR;
	}
	NETCMD_LOG(NTAG, "\n=======================================================================\n");
	NETCMD_LOG(NTAG, "CONN    CONNFAIL    DISCONN    RECONN    SCAN    SOFTAP    JOIN    LEFT\n");
	NETCMD_LOG(NTAG, "%-8d%-12d%-11d%-10d", stats.connect, stats.connectfail, stats.disconnect, stats.reconnect);
	NETCMD_LOG(NTAG, "%-8d%-10d%-8d%-8d\n", stats.scan, stats.softap, stats.joined, stats.left);
	NETCMD_LOG(NTAG, "=======================================================================\n");

	NETCMD_LOG(NTAG, "Connection INFO.\n");
	res = wifi_manager_get_info(&info);
	if (res != WIFI_MANAGER_SUCCESS) {
		NETCMD_LOG(NTAG, "Get Wi-Fi Manager Connection info failed\n");
		return ERROR;
	}
	if (info.mode == SOFTAP_MODE) {
		if (info.status == CLIENT_CONNECTED) {
			NETCMD_LOG(NTAG, "MODE: softap (client connected)\n");
		} else if (info.status == CLIENT_DISCONNECTED) {
			NETCMD_LOG(NTAG, "MODE: softap (no client)\n");
		}
		NETCMD_LOG(NTAG, "IP: %s\n", info.ip4_address);
		NETCMD_LOG(NTAG, "SSID: %s\n", info.ssid);
		NETCMD_LOG(NTAG, "MAC %02X:%02X:%02X:%02X:%02X:%02X\n",
			   info.mac_address[0], info.mac_address[1],
			   info.mac_address[2], info.mac_address[3],
			   info.mac_address[4], info.mac_address[5]);
	} else if (info.mode == STA_MODE) {
		if (info.status == AP_CONNECTED) {
			NETCMD_LOG(NTAG, "MODE: station (connected)\n");
			NETCMD_LOG(NTAG, "IP: %s\n", info.ip4_address);
			NETCMD_LOG(NTAG, "SSID: %s\n", info.ssid);
			NETCMD_LOG(NTAG, "rssi: %d\n", info.rssi);
		} else if (info.status == AP_DISCONNECTED) {
			NETCMD_LOG(NTAG, "MODE: station (disconnected)\n");
		} else if (info.status == AP_RECONNECTING) {
			NETCMD_LOG(NTAG, "MODE: station (reconnecting)\n");
			NETCMD_LOG(NTAG, "IP: %s\n", info.ip4_address);
			NETCMD_LOG(NTAG, "SSID: %s\n", info.ssid);
			NETCMD_LOG(NTAG, "rssi: %d\n", info.rssi);
		}
		NETCMD_LOG(NTAG, "MAC %02X:%02X:%02X:%02X:%02X:%02X\n", info.mac_address[0],
			   info.mac_address[1],
			   info.mac_address[2],
			   info.mac_address[3],
			   info.mac_address[4],
			   info.mac_address[5]);
	} else {
		NETCMD_LOG(NTAG, "STATE: NONE\n");
	}
	NETCMD_LOG(NTAG, "=======================================================================\n");
	return OK;
#else
	NETCMD_LOGE(NTAG, "Wi-Fi Manager is not enabled\n");
	return ERROR;
#endif
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/
int cmd_netmon(int argc, char **argv)
{
	sq_queue_t q_data;
	sq_init(&q_data);
	int ret;

	if (argc == 1 || argc > 2) {
		print_help();
	} else if (!(strncmp(argv[1], "sock", strlen("sock") + 1))) {
		/* Get socket information: SIOCGETSOCK */
		ret = netlib_netmon_sock(&q_data);
		if (!ret) {
			/* Free sq_queue entry of netmon_sock in print_socket() */
			print_socket(&q_data);
		} else {
			NETCMD_LOGE(NTAG, "Failed to fetch socket info.\n");
		}
	} else if (!(strncmp(argv[1], "wifi", strlen("wifi") + 1))) {
		return _print_wifi_info();
	} else {
#ifdef CONFIG_NET_STATS
		struct netmon_netdev_stats stats = {{0,}, 0, 0, 0, 0};
		char *intf = NULL;
		/* Get network interface stats if exists: SIOCGDEVSTATS */
		intf = argv[1];
		strncpy(stats.devname, intf, IFNAMSIZ);
		stats.devname[IFNAMSIZ] = '\0';
		ret = netlib_netmon_devstats(&stats);
		if (!ret) {
			print_devstats(&stats);
		} else {
			NETCMD_LOGE(NTAG, "No device interface %s\n", intf);
			return ERROR;
		}
#else
		NETCMD_LOGE(NTAG, "No such an option\n");
#endif
	}

	return OK;
}


int cmd_netstats(int argc, char **argv)
{
	(void)netlib_getstats();
	return 0;
}
