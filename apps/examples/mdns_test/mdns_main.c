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
 * examples/mdns_test/mdns_main.c
 *
 *   Copyright (C) 2008, 2011-2012 Gregory Nutt. All rights reserved.
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
/**
 * @file mdns_main.c
 * @brief the program for testing mdns
 */

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <stdio.h>
#include <stdlib.h>

#include <protocols/mdnsd.h>

#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>

/****************************************************************************
 * Definitions
 ****************************************************************************/
#if defined(CONFIG_WIFI_MANAGER)
#define MDNS_NETIF_NAME	CONFIG_WIFIMGR_STA_IFNAME
#else
#error "cannot set MDNS_NETIF_NAME"
#endif

/****************************************************************************
 * Enumeration
 ****************************************************************************/

/****************************************************************************
 * Private Data
 ****************************************************************************/

/****************************************************************************
 * function prototype
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * static function
 ****************************************************************************/
static void show_usage(FAR const char *progname)
{
#if defined(CONFIG_NETUTILS_MDNS_RESPONDER_SUPPORT)
	printf("\nUsage: %s <command> [2nd param] [3rd param] [4th param] [5th param]\n", progname);
	printf("\nWhere:\n");
	printf(" <command>   start | stop | hostname | resolve | discover | register\n");
	printf("              - mdns start <hostname>   : start mdns daemon.\n");
	printf("              - mdns stop               : terminate mdns daemon\n");
	printf("              - mdns hostname           : get current host name as mdns style\n");
	printf("              - mdns register <service_name> <service_type> <port_num> [description]\n");
	printf("                                        : register service.\n");
	printf("                                          [desciprtion] is optional.\n");
#else
	printf("\nUsage: %s <command> [2nd param] [3rd param]\n", progname);
	printf("\nWhere:\n");
	printf(" <command>   resolve | discover\n");
#endif
	printf("              - mdns resolve <hostname> : resolve hostname to ipaddr.\n");
	printf("              - mdns discover <service_type> [discovery_time_in_ms]\n");
	printf("                                        : discover service.\n");
	printf("                                          [discovery_time_in_ms] is optional (default=3000ms)\n");
	printf("\n");
}

#if defined(CONFIG_NETUTILS_MDNS_RESPONDER_SUPPORT)
static int test_mdns_start(char *hostname, char *netif_name)
{
	if (mdnsd_start(hostname, netif_name) != 0) {
		fprintf(stderr, "ERROR: |%s| fail to execute mdnsd_start() \n", __FUNCTION__);
		return -1;
	}
	printf("mdnsd_start() OK. \n");

	return 0;
}

static int test_mdns_stop(void)
{
	if (mdnsd_stop() != 0) {
		fprintf(stderr, "ERROR: |%s| fail to execute mdnsd_stop() \n", __FUNCTION__);
		return -1;
	}
	printf("mdnsd_stop() OK. \n");

	return 0;
}

static int test_mdns_get_hostname(void)
{
	char hostname_result[32];

	if (mdnsd_get_hostname(hostname_result) != 0) {
		fprintf(stderr, "ERROR: |%s| fail to execute mdnsd_get_hostname() \n", __FUNCTION__);
		return -1;
	}
	printf("mdns hostname : %s \n", hostname_result);

	return 0;
}

static int test_mdns_register_service(char *service_name, char *service_type, int port, char *txt)
{
	const char *text[2] = { txt, NULL };

	if (mdnsd_register_service(service_name, service_type, port, NULL, text) == 0) {
		printf("registered service : name:%s , type:%s , port:%d , txt:%s \n", service_name, service_type, port, text[0] ? text[0] : "N/A");
	} else {
		printf("cannot register service : name:%s , type:%s , port:%d , txt:%s \n", service_name, service_type, port, text[0] ? text[0] : "N/A");
	}

	return 0;
}
#endif							/* CONFIG_NETUTILS_MDNS_RESPONDER_SUPPORT */

static int test_mdns_resolve(char *hostname)
{
	int ipaddr = 0;

	if (mdnsd_resolve_hostname(hostname, &ipaddr) == 0) {
		printf("%s : %d.%d.%d.%d \n", hostname, (ipaddr >> 0) & 0xFF, (ipaddr >> 8) & 0xFF, (ipaddr >> 16) & 0xFF, (ipaddr >> 24) & 0xFF);
	} else {
		printf("%s is not found \n", hostname);
	}

	return 0;
}

static int test_mdns_discover_service(char *service_type, int discovery_time_ms)
{
	struct mdns_service_info *sd_result = NULL;
	int num_of_result = 0;

	if (mdnsd_discover_service(service_type, discovery_time_ms, &sd_result, &num_of_result) == 0) {
		int i;
		printf(" service discovery : %s \n", service_type);
		printf("-------------------------------------------------------------------------------- \n");
		printf(" %-4s %-25s %-25s %8s \n", "no", "hostname", "service name", "ip:port");
		printf("-------------------------------------------------------------------------------- \n");
		for (i = 0; i < num_of_result; i++) {
			printf(" %-4d %-25s %-25s %d.%d.%d.%d:%d \n", i + 1, sd_result[i].hostname ? sd_result[i].hostname : "(null)", sd_result[i].instance_name ? sd_result[i].instance_name : "(null)", (sd_result[i].ipaddr >> 0) & 0xFF, (sd_result[i].ipaddr >> 8) & 0xFF, (sd_result[i].ipaddr >> 16) & 0xFF, (sd_result[i].ipaddr >> 24) & 0xFF, sd_result[i].port);
		}
		printf("-------------------------------------------------------------------------------- \n");
	} else {
		printf("%s is not found \n", service_type);
	}

	return 0;
}

/****************************************************************************
 * mdns_main
 ****************************************************************************/

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int mdns_main(int argc, char *argv[])
#endif
{
	if ((argc == 3) && !strcmp(argv[1], "resolve")) {
		if (test_mdns_resolve(argv[2]) != 0) {
			goto errout;
		}
	} else if ((argc >= 3 && argc <= 4) && !strcmp(argv[1], "discover")) {
		if (test_mdns_discover_service(argv[2], argc == 4 ? atoi(argv[3]) : 3000) != 0) {
			goto errout;
		}
	}
#if defined(CONFIG_NETUTILS_MDNS_RESPONDER_SUPPORT)
	else if ((argc == 3) && !strcmp(argv[1], "start")) {
		if (test_mdns_start(argv[2], MDNS_NETIF_NAME) != 0) {
			goto errout;
		}
	} else if ((argc == 2) && !strcmp(argv[1], "stop")) {
		if (test_mdns_stop() != 0) {
			goto errout;
		}
	} else if ((argc == 2) && !strcmp(argv[1], "hostname")) {
		if (test_mdns_get_hostname() != 0) {
			goto errout;
		}
	} else if ((argc >= 5 && argc <= 6) && !strcmp(argv[1], "register")) {
		if (test_mdns_register_service(argv[2], argv[3], atoi(argv[4]), argc == 6 ? argv[5] : NULL) != 0) {
			goto errout;
		}
	}
#endif							/* CONFIG_NETUTILS_MDNS_RESPONDER_SUPPORT */
	else {
		show_usage(argv[0]);
		goto errout;
	}

	return 0;

errout:
	return -1;
}
