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
/**
 * @file dnsclient_main.c
 * @brief the program for testing dnsclient
 */

/****************************************************************************
 * examples/dnsclient_test/dnsclient_main.c
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

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>

#include <tinyara/net/dns.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#ifdef CONFIG_NET_LWIP
#include "lwip/inet.h"
#else
#include <arpa/inet.h>
#endif
/****************************************************************************
 * Definitions
 ****************************************************************************/
#ifndef DNS_DEFAULT_PORT
#define DNS_DEFAULT_PORT   53
#endif

#define DNS_NAME_MAXSIZE 512

/****************************************************************************
 * Enumeration
 ****************************************************************************/

/****************************************************************************
 * Structure
 ****************************************************************************/

/****************************************************************************
 * Private Data
 ****************************************************************************/
char hostname[DNS_NAME_MAXSIZE];

/****************************************************************************
 * function prototype
 ****************************************************************************/
static void show_usage(FAR const char *progname);

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * static function
 ****************************************************************************/
static void show_usage(FAR const char *progname)
{
	printf("\nUsage: %s [hostname] [IP address]\n", progname);
	printf("\nWhere:\n");
	printf("	[hostname] hostname for resolving IP address, (e.g., www.google.com) \n");
	printf("	[dns-server IP address] DNS server's IP address (optional)\n");
	printf("                            Note. Only available when CONFIG_NETDB_DNSSERVER_IPv4 is enabled\n");
	printf("\n");
}

/****************************************************************************
 * dnsclient_main
 ****************************************************************************/
#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int dnsclient_main(int argc, FAR char *argv[])
#endif
{
	struct hostent *shost = NULL;
	struct sockaddr_in dns_addr;

	if (argc < 2) {
		show_usage(argv[0]);
		return 0;
	}

	if (argc == 3 && argv[2] != NULL) {
		printf("dnsclient : dns_add_nameserver : %s\n", argv[2]);
		dns_addr.sin_family = AF_INET;
		inet_pton(AF_INET, argv[2], (void *)&dns_addr.sin_addr);
		netlib_setdnsserver((struct sockaddr *)&dns_addr, -1);
	}

	memset(hostname, 0x00, DNS_NAME_MAXSIZE);

	if (strlen(argv[1]) > DNS_NAME_MAXSIZE) {
		printf("dnsclient : length of hostname has to lower than %d\n", DNS_NAME_MAXSIZE);
		return -1;
	}

	strncpy(hostname, argv[1], DNS_NAME_MAXSIZE);
	printf("\nHostname : %s [len %d]\n", hostname, strlen(hostname));

	if ((shost = gethostbyname(hostname)) == NULL || shost->h_addr_list == NULL) {
		printf("dnsclient : failed to resolve host's IP address, shost %p\n", shost);
		return -1;
	} else {
		printf("DNS results\n");
		printf("IP Address : %s\n", inet_ntoa(*((struct in_addr *)shost->h_addr_list[0])));
	}

	return 0;
}
