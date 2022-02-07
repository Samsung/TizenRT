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
 * netutils/dhcpc/dhcpc.c
 *
 *   Copyright (C) 2007, 2009, 2011-2012 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Based heavily on portions of uIP:
 *
 *   Author: Adam Dunkels <adam@dunkels.com>
 *   Copyright (c) 2005, Swedish Institute of Computer Science
 *   All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>

#include <netdb.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <debug.h>
#include <net/if.h>
#include <arpa/inet.h>

#include <protocols/dhcpc.h>
#include <netutils/netlib.h>

#if defined(CONFIG_NETDB_DNSCLIENT) && defined(CONFIG_NETDB_DNSSERVER_BY_DHCP)
#include <tinyara/net/dns.h>
#endif
/****************************************************************************
 * Definitions
 ****************************************************************************/

#define STATE_INITIAL           0
#define STATE_HAVE_OFFER        1
#define STATE_HAVE_LEASE        2

#define BOOTP_BROADCAST         0x8000

#define DHCP_REQUEST            1
#define DHCP_REPLY              2
#define DHCP_HTYPE_ETHERNET     1
#define DHCP_HLEN_ETHERNET      6
#define DHCP_MSG_LEN            236

#define DHCPC_SERVER_PORT       67
#define DHCPC_CLIENT_PORT       68

#define DHCPDISCOVER            1
#define DHCPOFFER               2
#define DHCPREQUEST             3
#define DHCPDECLINE             4
#define DHCPACK                 5
#define DHCPNAK                 6
#define DHCPRELEASE             7

#define DHCP_OPTION_SUBNET_MASK 1
#define DHCP_OPTION_ROUTER      3
#define DHCP_OPTION_DNS_SERVER  6
#define DHCP_OPTION_REQ_IPADDR  50
#define DHCP_OPTION_LEASE_TIME  51
#define DHCP_OPTION_MSG_TYPE    53
#define DHCP_OPTION_SERVER_ID   54
#define DHCP_OPTION_REQ_LIST    55
#define DHCP_OPTION_END         255

#define BUFFER_SIZE             256
#define MAX_DNS_SERVERS         2       /*max no of dns servers*/

#define DHCPC_SET_IP4ADDR(intf, ip, netmask, gateway)	        \
	do {							\
		int res = -1;					\
		res = netlib_set_ipv4addr(intf, &ip);		\
		if (res == -1) {				\
			nvdbg("[DHCPC] set ipv4 addr error\n");	\
		}						\
		res = netlib_set_ipv4netmask(intf, &netmask);	\
		if (res == -1) {				\
			nvdbg("[DHCPC] set netmask addr error\n");	\
		}						\
		res = netlib_set_dripv4addr(intf, &gateway);	\
		if (res == -1) {				\
			nvdbg("[DHCPC] set route addr error\n");	\
		}						\
	} while (0)

/****************************************************************************
 * Private Types
 ****************************************************************************/

struct dhcp_msg {
	uint8_t op;
	uint8_t htype;
	uint8_t hlen;
	uint8_t hops;
	uint8_t xid[4];
	uint16_t secs;
	uint16_t flags;
	uint8_t ciaddr[4];
	uint8_t yiaddr[4];
	uint8_t siaddr[4];
	uint8_t giaddr[4];
	uint8_t chaddr[16];
#ifndef CONFIG_NET_DHCP_LIGHT
	uint8_t sname[64];
	uint8_t file[128];
#endif
	uint8_t options[312];
};

struct dhcpc_state {
	struct in_addr serverid;
	struct in_addr ipaddr;
	struct in_addr netmask;
	struct in_addr dnsaddr[MAX_DNS_SERVERS];
	struct in_addr default_router;
	uint32_t lease_time;		/* Lease expires in this number of seconds */
};

struct dhcpc_state_s {
	uint8_t ds_macaddr[16];
	char nic[10];
	int ds_maclen;
	int sockfd;
	struct in_addr ipaddr;
	struct in_addr serverid;
	struct dhcp_msg packet;
};

static int g_total_dns_servers;

/****************************************************************************
 * Private Data
 ****************************************************************************/

static const uint8_t g_dhcpc_xid[4] = { 0xad, 0xde, 0x12, 0x23 };
static const uint8_t magic_cookie[4] = { 99, 130, 83, 99 };

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: dhcpc_add<option>
 ****************************************************************************/
static uint8_t *dhcpc_addmsgtype(uint8_t *optptr, uint8_t type)
{
	*optptr++ = DHCP_OPTION_MSG_TYPE;
	*optptr++ = 1;
	*optptr++ = type;
	return optptr;
}

static uint8_t *dhcpc_addserverid(struct in_addr *serverid, uint8_t *optptr)
{
	*optptr++ = DHCP_OPTION_SERVER_ID;
	*optptr++ = 4;
	memcpy(optptr, &serverid->s_addr, 4);
	return optptr + 4;
}

static uint8_t *dhcpc_addreqipaddr(struct in_addr *ipaddr, uint8_t *optptr)
{
	*optptr++ = DHCP_OPTION_REQ_IPADDR;
	*optptr++ = 4;
	memcpy(optptr, &ipaddr->s_addr, 4);
	return optptr + 4;
}

static uint8_t *dhcpc_addreqoptions(uint8_t *optptr)
{
	*optptr++ = DHCP_OPTION_REQ_LIST;
	*optptr++ = 3;
	*optptr++ = DHCP_OPTION_SUBNET_MASK;
	*optptr++ = DHCP_OPTION_ROUTER;
	*optptr++ = DHCP_OPTION_DNS_SERVER;

	return optptr;
}

static uint8_t *dhcpc_addend(uint8_t *optptr)
{
	*optptr++ = DHCP_OPTION_END;
	return optptr;
}

/****************************************************************************
 * Name: dhcpc_sendmsg
 ****************************************************************************/

static int dhcpc_sendmsg(struct dhcpc_state_s *pdhcpc, struct dhcpc_state *presult, int msgtype)
{
	struct sockaddr_in addr;
	uint8_t *pend;
	in_addr_t serverid = INADDR_BROADCAST;
	int len;

	/* Create the common message header settings */

	memset(&pdhcpc->packet, 0, sizeof(struct dhcp_msg));
	pdhcpc->packet.op = DHCP_REQUEST;
	pdhcpc->packet.htype = DHCP_HTYPE_ETHERNET;
	pdhcpc->packet.hlen = pdhcpc->ds_maclen;
	memcpy(pdhcpc->packet.xid, g_dhcpc_xid, 4);
	memcpy(pdhcpc->packet.chaddr, pdhcpc->ds_macaddr, pdhcpc->ds_maclen);
	memset(&pdhcpc->packet.chaddr[pdhcpc->ds_maclen], 0, 16 - pdhcpc->ds_maclen);
	memcpy(pdhcpc->packet.options, magic_cookie, sizeof(magic_cookie));

	/* Add the common header options */

	pend = &pdhcpc->packet.options[4];
	pend = dhcpc_addmsgtype(pend, msgtype);

	/* Handle the message specific settings */

	switch (msgtype) {
		/* Broadcast DISCOVER message to all servers */

	case DHCPDISCOVER:
		/* we don't need the broadcast flag since we can receive unicast traffic
		   before being fully configured! */
		//pdhcpc->packet.flags = HTONS(BOOTP_BROADCAST);	/*  Broadcast bit. */
		pend = dhcpc_addreqoptions(pend);
		break;

		/* Send REQUEST message to the server that sent the *first* OFFER */

	case DHCPREQUEST:
		/* we don't need the broadcast flag since we can receive unicast traffic
		   before being fully configured! */
		//pdhcpc->packet.flags = HTONS(BOOTP_BROADCAST);	/*  Broadcast bit. */
		memcpy(pdhcpc->packet.ciaddr, &pdhcpc->ipaddr.s_addr, 4);
		pend = dhcpc_addserverid(&pdhcpc->serverid, pend);
		pend = dhcpc_addreqipaddr(&pdhcpc->ipaddr, pend);
		break;

		/* Send DECLINE message to the server that sent the *last* OFFER */

	case DHCPDECLINE:
		//ndbg("dhcpc_sendmsg is called : DHCPDECLINE\n");
		memcpy(pdhcpc->packet.ciaddr, &presult->ipaddr.s_addr, 4);
		pend = dhcpc_addserverid(&presult->serverid, pend);
		serverid = presult->serverid.s_addr;
		break;

	default:
		return ERROR;
	}

	pend = dhcpc_addend(pend);
	len = pend - (uint8_t *)&pdhcpc->packet;

	/* Send the request */

	addr.sin_family = AF_INET;
	addr.sin_port = HTONS(DHCPC_SERVER_PORT);
	addr.sin_addr.s_addr = serverid;

	int ret = sendto(pdhcpc->sockfd, &pdhcpc->packet, len, 0, (struct sockaddr *)&addr, sizeof(struct sockaddr_in));
	return ret;
}

/****************************************************************************
 * Name: dhcpc_parseoptions
 ****************************************************************************/
uint8_t itr;
static uint8_t dhcpc_parseoptions(struct dhcpc_state *presult, uint8_t *optptr, int len)
{
	uint8_t *end = optptr + len;
	uint8_t type = 0;
	uint8_t *start_addr;

	while (optptr < end) {
		switch (*optptr) {
		case DHCP_OPTION_SUBNET_MASK:
			/* Get subnet mask in network order */
			memcpy(&presult->netmask.s_addr, optptr + 2, 4);
			break;

		case DHCP_OPTION_ROUTER:
			/* Get the default router address in network order */

			memcpy(&presult->default_router.s_addr, optptr + 2, 4);
			break;

		case DHCP_OPTION_DNS_SERVER:
			/* Get the DNS server address in network order */

			g_total_dns_servers = (*(optptr + 1)) / 4;	/* Get the total number of DNS server addresses */
			g_total_dns_servers = (g_total_dns_servers > 2) ? 2 : g_total_dns_servers;
			start_addr = optptr + 2;
			for (itr = 0; itr < g_total_dns_servers; ++itr) {
				memcpy(&presult->dnsaddr[itr].s_addr, start_addr, 4);
				start_addr += 4;
			}
			break;

		case DHCP_OPTION_MSG_TYPE:
			/* Get message type */
			type = *(optptr + 2);
			break;

		case DHCP_OPTION_SERVER_ID:
			/* Get server address in network order */

			memcpy(&presult->serverid.s_addr, optptr + 2, 4);
			break;

		case DHCP_OPTION_LEASE_TIME:{
				/* Get lease time (in seconds) in host order */

				uint16_t tmp[2];
				memcpy(tmp, optptr + 2, 4);
				presult->lease_time = ((uint32_t)ntohs(tmp[0])) << 16 | (uint32_t)ntohs(tmp[1]);
			}
			break;

		case DHCP_OPTION_END:{
				return type;
			}
		default:{
				break;
			}
		}

		optptr += optptr[1] + 2;
	}
	return type;
}

/****************************************************************************
 * Name: dhcpc_parsemsg
 ****************************************************************************/
static uint8_t dhcpc_parsemsg(struct dhcpc_state_s *pdhcpc, int buflen, struct dhcpc_state *presult)
{
	if (pdhcpc->packet.op == DHCP_REPLY && memcmp(pdhcpc->packet.xid, g_dhcpc_xid, sizeof(g_dhcpc_xid)) == 0 && memcmp(pdhcpc->packet.chaddr, pdhcpc->ds_macaddr, pdhcpc->ds_maclen) == 0) {
		int remain = buflen - (sizeof(struct dhcp_msg) - 312);
		memcpy(&presult->ipaddr.s_addr, pdhcpc->packet.yiaddr, 4);
		return dhcpc_parseoptions(presult, &pdhcpc->packet.options[4], remain);
	}
	return 0;
}

/****************************************************************************
 * Name: dhcpc_open
 ****************************************************************************/
static void *dhcpc_open(const char *intf)
{
	struct dhcpc_state_s *pdhcpc;
	struct sockaddr_in addr;
	struct timeval tv;
	int ret;
	uint8_t macaddr[IFHWADDRLEN];
	int maclen = IFHWADDRLEN;

	if (netlib_getmacaddr(intf, macaddr) != OK) {
		/* Do not open dhcpc socket on wrong interface name */
		ndbg("ERROR : failed to netlib_getmacaddr\n");
		return NULL;
	}

	ndbg("MAC: %02x:%02x:%02x:%02x:%02x:%02x\n", ((uint8_t *)macaddr)[0], ((uint8_t *)macaddr)[1], ((uint8_t *)macaddr)[2], ((uint8_t *)macaddr)[3], ((uint8_t *)macaddr)[4], ((uint8_t *)macaddr)[5]);

	/* Allocate an internal DHCP structure */

	pdhcpc = (struct dhcpc_state_s *)malloc(sizeof(struct dhcpc_state_s));
	if (!pdhcpc)
		return NULL;

	/* Initialize the allocated structure */

	memset(pdhcpc, 0, sizeof(struct dhcpc_state_s));
	memcpy(pdhcpc->ds_macaddr, macaddr, maclen);
	pdhcpc->ds_maclen = maclen;

	/* Create a UDP socket */

	pdhcpc->sockfd = socket(PF_INET, SOCK_DGRAM, 0);
	if (pdhcpc->sockfd < 0) {
		nvdbg("get the socket handle fail\n");
		free(pdhcpc);
		return NULL;
	}

	/* Bind the socket */

	addr.sin_family = AF_INET;
	addr.sin_port = HTONS(DHCPC_CLIENT_PORT);
	addr.sin_addr.s_addr = INADDR_ANY;
	ret = bind(pdhcpc->sockfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in));
	if (ret < 0) {
		nvdbg("bind status %d\n", ret);
		close(pdhcpc->sockfd);
		free(pdhcpc);
		return NULL;
	}

	/* Configure for read timeouts */

	tv.tv_sec = 10;
	tv.tv_usec = 0;
	ret = setsockopt(pdhcpc->sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(struct timeval));
	if (ret < 0) {
		nvdbg("setsockopt status %d\n", ret);
		close(pdhcpc->sockfd);
		free(pdhcpc);
		return NULL;
	}

	memcpy(pdhcpc->nic, intf, IFHWADDRLEN);

	return (void *)pdhcpc;
}

/****************************************************************************
 * Name: dhcpc_close
 ****************************************************************************/
static void dhcpc_close(void *handle)
{
	struct dhcpc_state_s *pdhcpc = (struct dhcpc_state_s *)handle;

	if (pdhcpc) {
		if (pdhcpc->sockfd) {
			close(pdhcpc->sockfd);
		}

		free(pdhcpc);
	}
}

static struct dhcpc_state *g_pResult;
int g_dhcpc_state;

/****************************************************************************
 * Name: dhcpc_request
 ****************************************************************************/
static int dhcpc_request(void *handle, struct dhcpc_state *presult)
{
	int idx;
	if (!handle) {
		ndbg("ERROR : handle must not be null\n");
		return -100;
	}

	if (!presult) {
		ndbg("ERROR : presult must not be null\n");
		return -100;
	}

	struct dhcpc_state_s *pdhcpc = (struct dhcpc_state_s *)handle;
	g_pResult = presult;
	struct in_addr oldaddr;
	struct in_addr newaddr;
	uint8_t msgtype;
	int retries = 0, result = 0;
	char *intf;
#define CNT_MAX_DISCOVER 5
#define CNT_MAX_REQUEST 5
	/* Save the currently assigned IP address (should be INADDR_ANY) */
	oldaddr.s_addr = 0;

	intf = pdhcpc->nic;
	int ret = netlib_get_ipv4addr(intf, &oldaddr);
	if (ret == -1) {
		printf("Set IPv4 fail\n");
	}

	/* Loop until we receive the lease (or an error occurs) */
	/* Set the IP address to INADDR_ANY. */
	newaddr.s_addr = INADDR_ANY;
	if (netlib_set_ipv4addr(intf, &newaddr) == ERROR) {
		ndbg("netlib_set_ipv4addr failed\n");
	}

	/* Loop sending DISCOVER until we receive an OFFER from a DHCP
	 * server.  We will lock on to the first OFFER and decline any
	 * subsequent offers (which will happen if there are more than one
	 * DHCP servers on the network.
	 */
	g_dhcpc_state = STATE_INITIAL;
	do {
		/* Get the DHCPOFFER response */
		if (++retries > CNT_MAX_DISCOVER) {	/* reach max try */
			return -2;
		}

		ndbg("Send Discover Packet\n");
		result = dhcpc_sendmsg(pdhcpc, g_pResult, DHCPDISCOVER);
		usleep(100000);
		if (result < 0) {
			if (get_errno() == EAGAIN) {
				continue;
			}
			ndbg("sendDiscover Error\n");
			return -1;
		}

		ndbg("Waiting Offer...(%d)\n", pdhcpc->sockfd);
		result = recv(pdhcpc->sockfd, &pdhcpc->packet, sizeof(struct dhcp_msg), 0);
		ndbg("Received...(%d)\n", result);

		if (result <= 0) {
			ndbg("receive fail\n");
			continue;
		}
		msgtype = dhcpc_parsemsg(pdhcpc, result, presult);
		if (msgtype == DHCPOFFER) {
			/* Save the servid from the presult so that it is not clobbered
			 * by a new OFFER.
			 */
			ndbg("Received OFFER from %08x\n", ntohl(presult->serverid.s_addr));
			pdhcpc->ipaddr.s_addr = presult->ipaddr.s_addr;
			pdhcpc->serverid.s_addr = presult->serverid.s_addr;

			/* Temporarily use the address offered by the server and break
			 * out of the loop.
			 */

			ret = netlib_set_ipv4addr(intf, &presult->ipaddr);
			if (ret == -1) {
				ndbg("Set IPv4 fail\n");
			}
			g_dhcpc_state = STATE_HAVE_OFFER;
			break;
		} else {
			ndbg("Received AnotherData %d %x\n", result, msgtype);
		}
	} while (g_dhcpc_state == STATE_INITIAL);

	/* Loop sending the REQUEST up to three times (if there is no response) */
	retries = 0;

	/* Send the REQUEST message to obtain the lease that was offered to us. */
	ndbg("Send REQUEST\n");

	do {
		if (retries++ > CNT_MAX_REQUEST)
			break;

		ndbg("Send Request Packet\n");

		result = dhcpc_sendmsg(pdhcpc, g_pResult, DHCPREQUEST);
		if (result < 0) {
			ndbg("thread_sendRequest Error\n");
			return -3;
		}
		usleep(100000);
		/* Get the ACK/NAK response to the REQUEST (or timeout) */
		ndbg("Waiting Ack...\n");
		result = recv(pdhcpc->sockfd, &pdhcpc->packet, sizeof(struct dhcp_msg), 0);
		ndbg("Received...(%d)\n", result);

		if (result <= 0) {
			ndbg("recv request error(%d)(%d)\n", result, errno);
			continue;
		}

		/* Parse the response */
		ndbg("Data Received\n");

		msgtype = dhcpc_parsemsg(pdhcpc, result, presult);

		/* The ACK response means that the server has accepted our request
		 * and we have the lease.
		 */

		if (msgtype == DHCPACK) {
			ndbg("Received ACK\n");
			g_dhcpc_state = STATE_HAVE_LEASE;

		}

		/* NAK means the server has refused our request.  Break out of
		 * this loop with state == STATE_HAVE_OFFER and send DISCOVER again
		 */

		else if (msgtype == DHCPNAK) {
			ndbg("Received NAK\n");
			break;
		}

		/* If we get any OFFERs from other servers, then decline them now
		 * and continue waiting for the ACK from the server that we
		 * requested from.
		 */

		else if (msgtype == DHCPOFFER) {
			/* If we get OFFERs from same dhcp server, do not send DECLINE */
			if (pdhcpc->serverid.s_addr == presult->serverid.s_addr) {
				ndbg("Received duplicated OFFER from %08x\n", ntohl(presult->serverid.s_addr));
			} else {
				ndbg("Received another OFFER from %08x, send DECLINE\n", ntohl(presult->serverid.s_addr));
				result = dhcpc_sendmsg(pdhcpc, presult, DHCPDECLINE);
				if (result <= 0) {
					ndbg("recv request error(%d)(%d)\n", result, errno);
				}
			}
		}

		/* Otherwise, it is something that we do not recognize */

		else {
			ndbg("Ignoring msgtype=%d %d\n", msgtype, result);
		}
		usleep(100000L);

		/* An error has occurred.  If this was a timeout error (meaning
		 * that nothing was received on this socket for a long period of time).
		 * Then break out and send the DISCOVER command again (at most
		 * 3 times).
		 */
	} while (g_dhcpc_state == STATE_HAVE_OFFER);

	ndbg("Got IP address %d.%d.%d.%d\n", (presult->ipaddr.s_addr) & 0xff,
		 (presult->ipaddr.s_addr >> 8) & 0xff,
		 (presult->ipaddr.s_addr >> 16) & 0xff,
		 (presult->ipaddr.s_addr >> 24) & 0xff);

	ndbg("Got netmask %d.%d.%d.%d\n", (presult->netmask.s_addr) & 0xff,
		 (presult->netmask.s_addr >> 8) & 0xff,
		 (presult->netmask.s_addr >> 16) & 0xff,
		 (presult->netmask.s_addr >> 24) & 0xff);

	for (itr = 0; itr < g_total_dns_servers; ++itr) {
		ndbg("Got DNS server %d.%d.%d.%d\n", (presult->dnsaddr[itr].s_addr) & 0xff,
			 (presult->dnsaddr[itr].s_addr >> 8) & 0xff,
			 (presult->dnsaddr[itr].s_addr >> 16) & 0xff,
			 (presult->dnsaddr[itr].s_addr >> 24) & 0xff);
	}

	ndbg("Got default router %d.%d.%d.%d\n", (presult->default_router.s_addr) & 0xff,
		 (presult->default_router.s_addr >> 8) & 0xff,
		 (presult->default_router.s_addr >> 16) & 0xff,
		 (presult->default_router.s_addr >> 24) & 0xff);
	ndbg("Lease expires in %d seconds\n", presult->lease_time);

	struct sockaddr_in dns_addr;
	dns_addr.sin_family = AF_INET;
	for (idx = 0; idx < g_total_dns_servers; ++idx) {
		char dns_buffer[32] = {0,};
		dns_addr.sin_addr = presult->dnsaddr[idx];
		int res = netlib_setdnsserver((struct sockaddr *)&dns_addr, -1);
		if (res < 0) {
			ndbg("Set DNS server fail idx(%d)\n", idx);
			return -1;
		} else {
			inet_ntop(AF_INET, &presult->dnsaddr[idx], dns_buffer, 32);
			ndbg("Set DNS server %s\n", dns_buffer);
		}
	}

	return 0;
}

/****************************************************************************
 * Global Functions
 ****************************************************************************/
/****************************************************************************
 * Name: dhcp_client_start
 ****************************************************************************/
int dhcp_client_start(const char *intf)
{
	struct dhcpc_state state;
	int ret;
	void *dhcp_hnd = NULL;
	ndbg("[DHCPC] External DHCPC application started\n");
	dhcp_hnd = dhcpc_open(intf);
	if (dhcp_hnd) {
		ret = dhcpc_request(dhcp_hnd, &state);
		if (ret != OK) {
			ndbg("[DHCPC] get IP address fail\n");
			dhcpc_close(dhcp_hnd);
			return -1;
		}
		DHCPC_SET_IP4ADDR(intf, state.ipaddr, state.netmask, state.default_router);
		ndbg("[DHCPC] IP address : %s ----\n", inet_ntoa(state.ipaddr));
		dhcpc_close(dhcp_hnd);
	} else {
		ndbg("[DHCPC] Invalid dhcp handle\n");
		return -1;
	}

	return OK;
}

/****************************************************************************
 * Name: dhcp_client_stop
 ****************************************************************************/
void dhcp_client_stop(const char *intf)
{
	struct in_addr in = { .s_addr = INADDR_NONE };
	DHCPC_SET_IP4ADDR(intf, in, in, in);
	ndbg("[DHCPC] dhcpc_stop -release IP address (app)\n");
	return;
}
