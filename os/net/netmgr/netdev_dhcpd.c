/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
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
#include <tinyara/config.h>		/* TinyAra configuration */

#include <sys/types.h>

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <debug.h>				/* For ndbg, vdbg */

#include <net/if.h>
#include <netinet/in.h>
#include <semaphore.h>
#include <pthread.h>
#include <netutils/netlib.h>
#include <protocols/dhcpd.h>	/* Advertised DHCPD APIs */

#include "lwip/netif.h"
#include "lwip/netifapi.h"

/****************************************************************************
 * Global Data
 ****************************************************************************/
/****************************************************************************
 * Private Data
 ****************************************************************************/
#define DHCP_SERVER_PORT         67
#define DHCP_CLIENT_PORT         68

/****************************************************************************
 * Private Types
 ****************************************************************************/
/****************************************************************************
 * Private Data
 ****************************************************************************/
/****************************************************************************
 * Private Functions
 ****************************************************************************/
/****************************************************************************
 * Global Functions
 ****************************************************************************/
/****************************************************************************
 * Name: dhcps_server_status
 ****************************************************************************/
int netdev_dhcp_server_status(char *intf)
{
	struct netif *cur_netif;
	cur_netif = netif_find(intf);
	if (cur_netif == NULL) {
		ndbg("no network interface for dhcpd start\n");
		return ERROR;
	}

	if (cur_netif->dhcps_pcb == NULL) {
		ndbg("DHCP server closed\n");
		return ERROR;
	} else {
		ndbg("DHCP server opened\n");
		return OK;
	}
}

/****************************************************************************
 * Name: dhcps_server_start
 ****************************************************************************/
int netdev_dhcp_server_start(char *intf, dhcp_sta_joined dhcp_join_cb)
{
	struct netif *cur_netif;
	cur_netif = netif_find(intf);
	if (cur_netif == NULL) {
		ndbg("no network interface for dhcpd start\n");
		return ERROR;
	}
	if (dhcp_join_cb) {
		if (dhcps_register_cb(dhcp_join_cb) != OK) {
			ndbg("link callback fail\n");
			return ERROR;
		}
	}

	if (netifapi_dhcps_start(cur_netif) == OK) {
		ndbg("started successfully (LWIP)\n");
		return OK;
	}

	return ERROR;
}

/****************************************************************************
 * Name: dhcps_server_stop
 ****************************************************************************/
int netdev_dhcp_server_stop(char *intf)
{
	struct netif *cur_netif;
	cur_netif = netif_find(intf);
	if (cur_netif == NULL) {
		ndbg("no network interface for dhcpd start\n");
		return ERROR;
	}
	if (cur_netif->dhcps_pcb == NULL) {
		ndbg("stop dhcpd fail: no pcb\n");
		return ERROR;
	}

	netifapi_dhcps_stop(cur_netif);
	ndbg("stopped successfully (LWIP)\n");

	return OK;
}
