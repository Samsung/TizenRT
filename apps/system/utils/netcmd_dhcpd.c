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

/*
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 *
 * Author: Adam Dunkels <adam@sics.se>
 *
 */

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#ifdef CONFIG_NETUTILS_DHCPD

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include <net/if.h>

#include <protocols/dhcpd.h>
#include <netutils/netlib.h>

/****************************************************************************
 * Preprocessor Definitions
 ****************************************************************************/
#define DHCPD_STACK_SIZE		20480
#define DHCPD_SCHED_PRI			100
#define DHCPD_SCHED_POLICY		SCHED_RR

#define OK       0
#define ERR     -1

/****************************************************************************
* Private Data
****************************************************************************/
/****************************************************************************
* Global Data
****************************************************************************/
/****************************************************************************
* Private Functions
****************************************************************************/
/****************************************************************************
* Name: show_usage
****************************************************************************/

static void show_usage(void)
{
	printf("\n");
	printf("Usage: dhcpd <command> [interface name]\n");
	printf("\nWhere:\n");
	printf("	<command>		command string (start | stop | status ) \n");
	printf("					 - start  : start dhcpd daemon \n");
	printf("					 - stop   : terminate dhcpd daemon \n");
	printf("					 - status : show dhcpd's status \n");
	printf("	[interface name] name of network interface used for running dhcpd  \n");
	printf("\n");

}


/****************************************************************************
 * Public Functions
 ****************************************************************************/
/****************************************************************************
 * Name: cmd_dhcpd
 ****************************************************************************/

int cmd_dhcpd(int argc, char *argv[])
{
	int result = OK;
	uint8_t flags;

	if (argc < 2) {
		show_usage();
		goto done;
	}

	if (!strcmp(argv[1], "start")) {
		if (argc < 3) {
			printf("ERROR : %s insert [interface name]\n", __FUNCTION__);
			show_usage();
			goto done;
		}

		if (dhcp_server_status(argv[2])) {
			printf("ERROR : %s, dhcpd is already running\n", __FUNCTION__);
			result = ERR;
			goto done;
		}

		if (netlib_getifstatus(argv[2], &flags) == ERROR) {
			printf("ERROR : %s, failed to get interface status\n", __FUNCTION__);
			result = ERR;
			goto done;
		} else {
			if (flags & IFF_UP) {
				printf("%s : dhcp server start on %s\n", __FUNCTION__, argv[2]);
			} else {
				printf("%s : interface %s is down, unable to run dhcpd\n", __FUNCTION__, argv[2]);
				result = ERR;
				goto done;
			}
		}

		if (dhcp_server_start(argv[2], NULL) != 0) {
			printf("%s : failed to start dhcp server\n", __FUNCTION__);
			goto done;
		}
	} else if (!strcmp(argv[1], "stop")) {
		printf("%s : dhcp server stop\n", __FUNCTION__);
		dhcp_server_stop(argv[2]);
	} else if (!strcmp(argv[1], "status")) {
		printf("\ndhcpd status : ");
		if (dhcp_server_status(argv[2])) {
			printf("running\n");
		} else {
			printf("stopped\n");
		}
	} else {
		printf("ERROR : %s invalid <command> : %s\n", __FUNCTION__, argv[1]);
		show_usage();
		goto done;
	}

done:
	return result;
}
#endif
