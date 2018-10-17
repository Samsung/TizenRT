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

/****************************************************************************
 * Preprocessor Definitions
 ****************************************************************************/
#define USAGE							\
	"\n usage: netmon [options]\n"		\
	"\n socket information:\n"			\
	"       netmon sock\n"				\
	"\n WiFi Manager stats:\n"			\
	"       netmon wifi\n"				\
	"\n Net device stats:\n"			\
	"       netmon [devname]\n\n"

#ifdef CONFIG_NET_IPv6
#define PRINT_IPV(sock)										\
	do {													\
		if (sock->type & NETMON_TYPE_IPV6) {				\
			printf("IPv6\t");								\
			printf("%-15s:", inet6_ntoa(sock->local_ip));	\
			if (sock->local_port == 0) {					\
				printf("%-8s", "*");						\
			} else {										\
				printf("%-8d", sock->local_port);			\
			}												\
			printf("%-15s:", inet6_ntoa(sock->remote_ip));	\
			if (sock->remote_port == 0) {					\
				printf("%-8s", "*");						\
			} else {										\
				printf("%-8d", sock->remote_port);			\
			}												\
		} else {											\
			printf("IPv4\t");								\
			printf("%-15s:", inet_ntoa(sock->local_ip));	\
			if (sock->local_port == 0) {					\
				printf("%-8s", "*");						\
			} else {										\
				printf("%-8d", sock->local_port);			\
			}												\
			printf("%-15s:", inet_ntoa(sock->remote_ip));	\
			if (sock->remote_port == 0) {					\
				printf("%-8s", "*");						\
			} else {										\
				printf("%-8d", sock->remote_port);			\
			}												\
		}													\
	} while (0)
#else
#define PRINT_IPV(sock)									\
	do {												\
		printf("IPv4\t");								\
		printf("%-15s:", inet_ntoa(sock->local_ip));	\
		if (sock->local_port == 0) {					\
			printf("%-8s", "*");						\
		} else {										\
			printf("%-8d", sock->local_port);			\
		}												\
		printf("%-15s:", inet_ntoa(sock->remote_ip));	\
		if (sock->remote_port == 0) {					\
			printf("%-8s", "*");						\
		} else {										\
			printf("%-8d", sock->remote_port);			\
		}												\
	} while (0)
#endif

/****************************************************************************
* Global Data
****************************************************************************/


/****************************************************************************
* Private Functions
****************************************************************************/
/**
 * Print the command list
 */
static void print_help(void)
{
	printf("%s", USAGE);
	return;
}

/**
 * Print a externally used socket information.
 */
static void print_socket(sq_queue_t *q_sock)
{
	int i = 1;
	struct netmon_sock *sock_info = (struct netmon_sock *) sq_remfirst(q_sock);
	printf("\nSockfd\tProto\tType\tLocal\t\t\tRemote\t\t\tstate\tPID\n");
	while (sock_info) {
		printf("%-8d", i);
		/* Protocol */
		if (sock_info->type & NETMON_TCP) {
			printf("TCP\t");
			PRINT_IPV(sock_info);
		} else if (sock_info->type & NETMON_UDP) {
			printf("UDP\t");
			PRINT_IPV(sock_info);
		} else {
			printf("RAW\t");
			PRINT_IPV(sock_info);
		}
		switch (sock_info->state) {
		case NETMON_WRITE:
			printf("WRITE\t");
			break;
		case NETMON_LISTEN:
			printf("LISTEN\t");
			break;
		case NETMON_CONNECT:
			printf("CONNECT\t");
			break;
		case NETMON_CLOSE:
			printf("CLOSE\t");
			break;
		default:
			printf("NONE\t");
			break;
		}
		printf("%d:%s\n", sock_info->pid, sock_info->pid_name);
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
	printf("\n==============================================\n");
	printf("IFNAME    RXbyte    RXPKT    TXbyte    TXPKT\n");
	printf("----------------------------------------------\n");
	printf("%-10s%-10d%-9d", stats->devname, stats->devinoctets, stats->devinpkts);
	printf("%-10d%-9d\n", stats->devoutoctets, stats->devoutpkts);
	printf("==============================================\n");
	return;
}
#endif							/* CONFIG_NET_STATS */
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
			printf("Failed to fetch socket info.\n");
		}
	} else if (!(strncmp(argv[1], "wifi", strlen("wifi") + 1))) {
#ifdef CONFIG_WIFI_MANAGER
		wifi_manager_stats_s stats;
		wifi_manager_info_s info;

		wifi_manager_result_e res = wifi_manager_get_stats(&stats);
		if (res != WIFI_MANAGER_SUCCESS) {
			printf("Get Wi-Fi Manager stats failed\n");
			return ERROR;
		}
		printf("\n=======================================================================\n");
		printf("CONN    CONNFAIL    DISCONN    RECONN    SCAN    SOFTAP    JOIN    LEFT\n");
		printf("%-8d%-12d%-11d%-10d", stats.connect, stats.connectfail, stats.disconnect, stats.reconnect);
		printf("%-8d%-10d%-8d%-8d\n", stats.scan, stats.softap, stats.joined, stats.left);
		printf("=======================================================================\n");

		printf("Connection INFO.\n");
		res = wifi_manager_get_info(&info);
		if (res != WIFI_MANAGER_SUCCESS) {
			printf("Get Wi-Fi Manager Connection info failed\n");
			return ERROR;
		}
		if (info.mode == SOFTAP_MODE) {
			if (info.status == CLIENT_CONNECTED) {
				printf("MODE: softap (client connected)\n");
			} else if (info.status == CLIENT_DISCONNECTED) {
				printf("MODE: softap (no client)\n");
			}
			printf("IP: %s\n", info.ip4_address);
			printf("SSID: %s\n", info.ssid);
			printf("MAC %02X:%02X:%02X:%02X:%02X:%02X\n", info.mac_address[0], info.mac_address[1], info.mac_address[2], info.mac_address[3], info.mac_address[4], info.mac_address[5]);
		} else if (info.mode == STA_MODE) {
			if (info.status == AP_CONNECTED) {
				printf("MODE: station (connected)\n");
				printf("IP: %s\n", info.ip4_address);
				printf("SSID: %s\n", info.ssid);
				printf("rssi: %d\n", info.rssi);
			} else if (info.status == AP_DISCONNECTED) {
				printf("MODE: station (disconnected)\n");
			}
			printf("MAC %02X:%02X:%02X:%02X:%02X:%02X\n", info.mac_address[0], info.mac_address[1], info.mac_address[2], info.mac_address[3], info.mac_address[4], info.mac_address[5]);
		} else {
			printf("STATE: NONE\n");
		}
		printf("=======================================================================\n");
#else
		printf("Wi-Fi Manager is not enabled\n");
#endif
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
			printf("No device interface %s\n", intf);
			return ERROR;
		}
#else
		printf("No such an option\n");
#endif
	}

	return OK;
}

