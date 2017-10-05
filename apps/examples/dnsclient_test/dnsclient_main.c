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

#include <tinyara/net/dns.h>
#include <netdb.h>
#include <sys/socket.h>

#ifdef CONFIG_NET_LWIP
#include <net/lwip/ipv4/inet.h>
#else
#include <arpa/inet.h>
#endif

/****************************************************************************
 * Definitions
 ****************************************************************************/
#ifndef DNS_DEFAULT_PORT
#define DNS_DEFAULT_PORT   53
#endif

#ifndef CONFIG_EXAMPLES_DNSCLIENT_TEST_SERVER_PORT
#define CONFIG_EXAMPLES_DNSCLIENT_TEST_SERVER_PORT	DNS_DEFAULT_PORT
#endif

#ifndef CONFIG_NETDB_DNSCLIENT_NAMESIZE
#error "CONFIG_NETDB_DNSCLIENT_NAMESIZE is not defined"
#endif
/****************************************************************************
 * Enumeration
 ****************************************************************************/

/****************************************************************************
 * Structure
 ****************************************************************************/

/****************************************************************************
 * Private Data
 ****************************************************************************/
char hostname[CONFIG_NETDB_DNSCLIENT_NAMESIZE];

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
#ifdef CONFIG_NETDB_DNSSERVER_IPv4
	struct sockaddr_in dns;
#endif

	if (argc < 2) {
		show_usage(argv[0]);
		return 0;
	}

	if (argc == 3 && argv[2] != NULL) {
#ifdef CONFIG_NETDB_DNSSERVER_IPv4
		printf("dnsclient : dns_add_nameserver : %s\n", argv[2]);
		dns.sin_family = AF_INET;
		dns.sin_port = htons(CONFIG_EXAMPLES_DNSCLIENT_TEST_SERVER_PORT);
		dns.sin_addr.s_addr = inet_addr(argv[2]);
		dns_add_nameserver((FAR struct sockaddr *)&dns, sizeof(struct sockaddr_in));
#endif
#ifdef CONFIG_NETDB_DNSSERVER_BY_DHCP
		printf("dnsclient : dns server address is set by DHCP\n");
#endif
	}

	memset(hostname, 0x00, CONFIG_NETDB_DNSCLIENT_NAMESIZE);

	if (strlen(argv[1]) > CONFIG_NETDB_DNSCLIENT_NAMESIZE - 1) {
		printf("dnsclient : length of hostname has to lower than or equal to %d\n", CONFIG_NETDB_DNSCLIENT_NAMESIZE - 1);
		return -1;
	}

	strncpy(hostname, argv[1], CONFIG_NETDB_DNSCLIENT_NAMESIZE);
	printf("\nHostname : %s [len %d]\n", hostname, strlen(hostname));

	if ((shost = gethostbyname(hostname)) == NULL || shost->h_addr_list == NULL) {
		printf("dnsclient : failed to resolve host's IP address, shost %p\n", shost);
		return -1;
	} else {
		printf("DNS results\n");
		printf("IP Address : %s\n", ip_ntoa((ip_addr_t *)shost->h_addr_list[0]));
	}

	return 0;
}
