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
#include <stdlib.h>
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
#define USAGE                      \
	"\n usage: netmon [options]\n" \
	"\n socket information:\n"     \
	"       netmon sock\n"         \
	"\n WiFi Manager stats:\n"     \
	"       netmon wifi\n"         \
	"\n Net device stats:\n"       \
	"       netmon [devname]\n\n"

/**
 * Print the command list
 */
static inline void _print_help(void)
{
	NETCMD_LOG(NTAG, "%s", USAGE);
	return;
}

static inline void _print_sock(char *buf)
{
	NETCMD_LOG(NTAG, "\n==============================================\n");
	NETCMD_LOG(NTAG, "TCP\tfd\tpname:pid\tconn state\tIP type\tTCP state\tlocal IP\tlocal port\tremote IP\tremote port\n");
	NETCMD_LOG(NTAG, "UDP\tfd\tpname:pid\tconn state\tIP type\tUDP flag\tlocal IP\tlocal port\tremote IP\tremote port\n");
	NETCMD_LOG(NTAG, "RAW\tfd\tpname:pid\tconn state\tIP type\tprotocol\tlocal IP\tremote IP\n");
	NETCMD_LOG(NTAG, "----------------------------------------------\n");
	NETCMD_LOG(NTAG, "%s\n", buf);
	NETCMD_LOG(NTAG, "==============================================\n");
}

/**
 * Print a externally used netdev information.
 */
static void _print_devstats(char *buf)
{
	NETCMD_LOG(NTAG, "\n==============================================\n");
	NETCMD_LOG(NTAG, "%s", buf);
	NETCMD_LOG(NTAG, "==============================================\n");
	return;
}

static inline int _print_wifi_info(void)
{
	NETCMD_LOGE(NTAG, "Wi-Fi Manager is not enabled\n");
	return ERROR;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/
int cmd_netmon(int argc, char **argv)
{
	int ret;

	if (argc == 1 || argc > 2) {
		_print_help();
	} else if (!(strncmp(argv[1], "sock", strlen("sock") + 1))) {
		/* Get socket information: SIOCGETSOCK */
		char *buf = NULL;
		ret = netlib_netmon_sock(buf);
		if (!ret) {
			_print_sock(buf);
			free(buf);
		} else {
			NETCMD_LOGE(NTAG, "Failed to fetch socket info.\n");
		}
	} else if (!(strncmp(argv[1], "wifi", strlen("wifi") + 1))) {
		return _print_wifi_info();
	} else if (!(strncmp(argv[1], "netstats", strlen("netstats") + 1))) {
		return -1;
	} else {
		char *buf = NULL;
		ret = netlib_netmon_devstats(argv[1], (void **)&buf);
		if (!ret) {
			_print_devstats(buf);
			free(buf);
		} else {
			NETCMD_LOGE(NTAG, "No device interface %s\n", argv[1]);
			return ERROR;
		}
	}
	return OK;
}

int cmd_netstats(int argc, char **argv)
{
	(void)netlib_getstats();
	return 0;
}
