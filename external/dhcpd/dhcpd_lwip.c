/****************************************************************************
 * netutils/dhcpd/dhcpd.c
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
 * netutils/dhcpd/dhcpd.c
 *
 *   Copyright (C) 2007-2009, 2011-2014 Gregory Nutt. All rights reserved.
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

#include <stdio.h>

#include <tinyara/config.h>		/* TinyAra configuration */
#include <debug.h>				/* For ndbg, vdbg */
#include <tinyara/compiler.h>	/* For CONFIG_CPP_HAVE_WARNING */
#include <protocols/dhcpd.h>	/* Advertised DHCPD APIs */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>

#include <net/if.h>
#include <netinet/in.h>
#include <semaphore.h>
#include <pthread.h>
#include <netutils/netlib.h>

#include <net/lwip/netif.h>
#include <net/lwip/netifapi.h>

#undef nvdbg
#undef ndbg
#define ndbg(...) printf(__VA_ARGS__)
#define nvdbg(...) printf(__VA_ARGS__)

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
int dhcp_server_status(char *intf)
{
	struct netif *cur_netif;
	cur_netif = netif_find(intf);
	if (cur_netif == NULL) {
		ndbg("[DHCPS] no network interface for dhcpd start\n");
		return 0;
	}

	if (cur_netif->dhcps_pcb == NULL) {
		ndbg("[DHCPS] DHCP server closed\n");
		return 0;
	} else {
		ndbg("[DHCPS] DHCP server opened\n");
		return 1;
	}
}

/****************************************************************************
 * Name: dhcps_server_start
 ****************************************************************************/
int dhcp_server_start(char *intf, dhcp_sta_joined dhcp_join_cb)
{
	struct netif *cur_netif;
	cur_netif = netif_find(intf);
	if (cur_netif == NULL) {
		ndbg("[DHCPS] no network interface for dhcpd start\n");
		return -1;
	}
	if (dhcp_join_cb) {
		if (dhcps_register_cb(dhcp_join_cb) != ERR_OK) {
			ndbg("[DHCPS] link callback fail\n");
			return -1;
		}
	}

	if (netifapi_dhcps_start(cur_netif) == ERR_OK) {
		ndbg("[DHCPS] started successfully (LWIP)\n");
		return OK;
	}

	return -1;
}

/****************************************************************************
 * Name: dhcps_server_stop
 ****************************************************************************/
int dhcp_server_stop(char *intf)
{
	struct netif *cur_netif;
	cur_netif = netif_find(intf);
	if (cur_netif == NULL) {
		ndbg("[DHCPS] no network interface for dhcpd start\n");
		return -1;
	}
	if (cur_netif->dhcps_pcb == NULL) {
		ndbg("[DHCPS] stop dhcpd fail: no pcb\n");
		return -1;
	}

	netifapi_dhcps_stop(cur_netif);
	ndbg("[DHCPS] stopped successfully (LWIP)\n");

	return OK;
}
