/****************************************************************************
 *
 * Copyright 2021 Samsung Electronics All Rights Reserved.
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

#pragma once

#if CONFIG_NET_LWIP
#include <sys/types.h>

typedef enum {
	GETADDRINFO,
	FREEADDRINFO,
	GETHOSTBYNAME,
	GETNAMEINFO,
	DNSSETSERVER,
	DHCPCSTART,
	DHCPCSTOP,
	DHCPCSETHOSTNAME,
	DHCPDSTART,
	DHCPDSTOP,
	DHCPDSTATUS,
	GETNETSTATS, /*  get network statistics */
	GETSOCKINFO, /*  get opened socket information */
	GETDEVSTATS, /*  get NIC statistics */
} req_type;

struct lwip_netdb_msg {
	const char *host_name;
	const char *serv_name;
	const struct addrinfo *ai_hint;
	struct addrinfo *ai_res;
	struct hostent *host_entry;
	const struct sockaddr *sa;
	size_t sa_len;
	size_t host_len;
	size_t serv_len;
	int flags;
};

struct lwip_dns_msg {
	int index;
	struct sockaddr *addr;
};

struct lwip_dhcp_msg {
	const char *intf;
	const char *hostname;
};

struct lwip_netmon_msg {
	/* contains monitoring information string
	 * info should be freed by caller */
	char *info;
	const char *ifname;
};

/* To send a request to lwip stack by ioctl() use */
struct req_lwip_data {
	req_type type;
	int req_res;
	union {
		struct lwip_netdb_msg netdb;
		struct lwip_dns_msg dns;
		struct lwip_dhcp_msg dhcp;
		struct lwip_netmon_msg netmon;
	} msg;
};

#endif
