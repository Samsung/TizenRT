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

#ifdef CONFIG_NETUTILS_DHCPD_HOST
#include <stdio.h>

#define HTONS(a) htons(a)
#define HTONL(a) htonl(a)

#define CONFIG_CPP_HAVE_WARNING 1
#define FAR

#define ndbg(...) printf(__VA_ARGS__)
#define nvdbg(...) printf(__VA_ARGS__)

#define ERROR (-1)
#define OK    (0)
#else
#include <tinyara/config.h>		/* TinyAra configuration */
#include <debug.h>				/* For ndbg, vdbg */
#include <tinyara/compiler.h>	/* For CONFIG_CPP_HAVE_WARNING */
#include <arch/irq.h>			/* For irqstore() and friends -- REVISIT */
#include <tinyara/net/net.h>	/* For net_lock() and friends */
#ifndef CONFIG_NET_LWIP
#include <tinyara/net/arp.h>	/* For low-level ARP interfaces -- REVISIT */
#endif
#include <protocols/dhcpd.h>	/* Advertised DHCPD APIs */
#undef nvdbg
#undef ndbg
#define ndbg(...) printf(__VA_ARGS__)
#define nvdbg(...) printf(__VA_ARGS__)
#endif							/* CONFIG_NETUTILS_DHCPD_HOST */

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/select.h>

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>

#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <semaphore.h>
#include <pthread.h>
#include <netutils/netlib.h>

/****************************************************************************
 * Global Data
 ****************************************************************************/
sem_t g_dhcpd_sem;


/****************************************************************************
 * Private Data
 ****************************************************************************/
#define DHCPD_SELECT             1

#define DHCP_SERVER_PORT         67
#define DHCP_CLIENT_PORT         68

/* Option codes understood in this file                                     */
/*                              Code    Data   Description                  */
/*                                      Length                              */
#define DHCP_OPTION_PAD           0	/*  1     Pad                          */
#define DHCP_OPTION_SUBNET_MASK   1	/*  1     Subnet Mask                  */
#define DHCP_OPTION_ROUTER        3	/*  4     Router                       */
#define DHCP_OPTION_DNS_SERVER    6	/*  4N    DNS                          */
#define DHCP_OPTION_REQ_IPADDR   50	/*  4     Requested IP Address         */
#define DHCP_OPTION_LEASE_TIME   51	/*  4     IP address lease time        */
#define DHCP_OPTION_OVERLOAD     52	/*  1     Option overload              */
#define DHCP_OPTION_MSG_TYPE     53	/*  1     DHCP message type            */
#define DHCP_OPTION_SERVER_ID    54	/*  4     Server identifier            */
#define DHCP_OPTION_END         255	/*  0     End                          */

/* Values for the dhcp msg 'op' field */

#define DHCP_REQUEST              1
#define DHCP_REPLY                2

/* DHCP message types understood in this file */

#define DHCPDISCOVER              1	/* Received from client only */
#define DHCPOFFER                 2	/* Sent from server only */
#define DHCPREQUEST               3	/* Received from client only */
#define DHCPDECLINE               4	/* Received from client only */
#define DHCPACK                   5	/* Sent from server only */
#define DHCPNAK                   6	/* Sent from server only */
#define DHCPRELEASE               7	/* Received from client only */
#define DHCPINFORM                8	/* Not used */

/* The form of an option is:
 *   code   - 1 byte
 *   length - 1 byte
 *   data   - variable number of bytes
 */

#define DHCPD_OPTION_CODE         0
#define DHCPD_OPTION_LENGTH       1
#define DHCPD_OPTION_DATA         2

/* Size of options in DHCP message */

#define DHCPD_OPTIONS_SIZE        312

/* Values for htype and hlen field */

#define DHCP_HTYPE_ETHERNET       1
#define DHCP_HLEN_ETHERNET        6

/* Values for flags field */

#define BOOTP_BROADCAST           0x8000

/* Legal values for this option are:
 *
 *   1     the 'file' field is used to hold options
 *   2     the 'sname' field is used to hold options
 *   3     both fields are used to hold options
 */

#define DHCPD_OPTION_FIELD        0
#define DHCPD_FILE_FIELD          1
#define DHCPD_SNAME_FIELD         2

#ifndef CONFIG_NETUTILS_DHCPD_LEASETIME
#define CONFIG_NETUTILS_DHCPD_LEASETIME (60*60*24*10)	/* 10 days */
#undef CONFIG_NETUTILS_DHCPD_MINLEASETIME
#undef CONFIG_NETUTILS_DHCPD_MAXLEASETIME
#endif

#ifndef CONFIG_NETUTILS_DHCPD_MINLEASETIME
#define CONFIG_NETUTILS_DHCPD_MINLEASETIME (60*60*24*1)	/* 1 days */
#endif

#ifndef CONFIG_NETUTILS_DHCPD_MAXLEASETIME
#define CONFIG_NETUTILS_DHCPD_MAXLEASETIME (60*60*24*30)	/* 30 days */
#endif

#ifndef CONFIG_NETUTILS_DHCPD_MAXLEASES
#define CONFIG_NETUTILS_DHCPD_MAXLEASES 16
#endif

#ifndef CONFIG_NETUTILS_DHCPD_STARTIP
#define CONFIG_NETUTILS_DHCPD_STARTIP (192L<<24|168L<<16|0L<<8|3L)
#endif

#undef CONFIG_NETUTILS_DHCP_OPTION_ENDIP
#define CONFIG_NETUTILS_DHCP_OPTION_ENDIP (CONFIG_NETUTILS_DHCPD_STARTIP + CONFIG_NETUTILS_DHCPD_MAXLEASES - 1)

#ifndef CONFIG_NETUTILS_DHCPD_OFFERTIME
#define CONFIG_NETUTILS_DHCPD_OFFERTIME (60*60)	/* 1 hour */
#endif

#ifndef CONFIG_NETUTILS_DHCPD_DECLINETIME
#define CONFIG_NETUTILS_DHCPD_DECLINETIME (60*60)	/* 1 hour */
#endif

#undef HAVE_ROUTERIP
#if defined(CONFIG_NETUTILS_DHCPD_ROUTERIP) && CONFIG_NETUTILS_DHCPD_ROUTERIP
#define HAVE_ROUTERIP 1
#endif

#undef HAVE_NETMASK
#if defined(CONFIG_NETUTILS_DHCPD_NETMASK) && CONFIG_NETUTILS_DHCPD_NETMASK
#define HAVE_NETMASK 1
#endif

#undef HAVE_DNSIP
#if defined(CONFIG_NETUTILS_DHCPD_DNSIP) && CONFIG_NETUTILS_DHCPD_DNSIP
#define HAVE_DNSIP 1
#endif

#undef HAVE_LEASE_TIME
#if defined(CONFIG_NETUTILS_DHCPD_HOST) || !defined(CONFIG_DISABLE_POSIX_TIMERS)
#define HAVE_LEASE_TIME 1
#endif

/****************************************************************************
 * Private Types
 ****************************************************************************/

/* This structure describes one element in the lease table.  There is one slot
 * in the lease table for each assign-able IP address (hence, the IP address
 * itself does not have to be in the table.
 */

struct lease_s {
	uint8_t mac[DHCP_HLEN_ETHERNET];	/* MAC address (network order) -- could be larger! */
	bool allocated;				/* true: IP address is allocated */
#ifdef HAVE_LEASE_TIME
	time_t expiry;				/* Lease expiration time (seconds past Epoch) */
#endif
};

struct dhcpmsg_s {
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

struct dhcpd_state_s {
	/* Server configuration */

	in_addr_t ds_serverip;		/* The server IP address */

	/* Message buffers */

	struct dhcpmsg_s ds_inpacket;	/* Holds the incoming DHCP client message */
	struct dhcpmsg_s ds_outpacket;	/* Holds the outgoing DHCP server message */

	/* Parsed options from the incoming DHCP client message */

	uint8_t ds_optmsgtype;		/* Incoming DHCP message type */
	in_addr_t ds_optreqip;		/* Requested IP address (host order) */
	in_addr_t ds_optserverip;	/* Serverip IP address (host order) */
	time_t ds_optleasetime;		/* Requested lease time (host order) */

	/* End option pointer for outgoing DHCP server message */

	uint8_t *ds_optend;

	/* Leases */

	struct lease_s ds_leases[CONFIG_NETUTILS_DHCPD_MAXLEASES];
};

/****************************************************************************
 * Private Data
 ****************************************************************************/

static const uint8_t g_magiccookie[4] = { 99, 130, 83, 99 };
#ifndef CONFIG_NETUTILS_DHCPD_IGNOREBROADCAST
static const uint8_t g_anyipaddr[4] = { 0, 0, 0, 0 };
#endif

static struct dhcpd_state_s g_state;

static int g_dhcpd_running = 0;
static int g_dhcpd_quit = 0;
static int g_dhcpd_sockfd = -1;

static pthread_t g_tid = 0;

static char DHCPD_IFNAME[IFNAMSIZ] = { 0, };

#if DHCPD_SELECT
static struct timeval g_select_timeout = { 1, 0 };
#endif

static dhcp_sta_joined g_dhcp_sta_joined = NULL;
static pthread_mutex_t g_dhcpd_lock = PTHREAD_MUTEX_INITIALIZER; // protect a write operation in g_dhcpd_running

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: dhcpd_arpupdate
 ****************************************************************************/

#ifndef CONFIG_NETUTILS_DHCPD_HOST
#ifndef CONFIG_NET_LWIP
static inline void dhcpd_arpupdate(uint16_t *pipaddr, uint8_t *phwaddr)
{
	net_lock_t flags;

	/* Disable interrupts and update the ARP table -- very non-portable hack.
	 * REVISIT -- switch to the SIOCSARP ioctl call if/when it is implemented.
	 */

	flags = net_lock();
	arp_update(pipaddr, phwaddr);
	net_unlock(flags);
}
#else
#define dhcpd_arpupdate(pipaddr, phwaddr)
#endif							/* CONFIG_NET_LWIP */
#else
#define dhcpd_arpupdate(pipaddr, phwaddr)
#endif

/****************************************************************************
 * Name: dhcpd_time
 ****************************************************************************/

#ifdef CONFIG_NETUTILS_DHCPD_HOST
#define dhcpd_time() time(0)
#elif defined(HAVE_LEASE_TIME)
static time_t dhcpd_time(void)
{
	struct timespec ts;
	time_t ret = 0;

	if (clock_gettime(CLOCK_REALTIME, &ts) == OK) {
		ret = ts.tv_sec;
	}

	return ret;
}
#else
#define dhcpd_time() (0)
#endif

/****************************************************************************
 * Name: dhcpd_leaseexpired
 ****************************************************************************/

#ifdef HAVE_LEASE_TIME
static inline bool dhcpd_leaseexpired(struct lease_s *lease)
{
	ndbg("dhcpd_leaseexpired is called!!\n");

	return false;

	if (lease->expiry < dhcpd_time()) {
		return false;
	} else {
//      memset(lease, 0, sizeof(struct lease_s));
		return true;
	}
}
#else
#define dhcpd_leaseexpired(lease) (false)
#endif

/****************************************************************************
 * Name: dhcpd_setlease
 ****************************************************************************/

struct lease_s *dhcpd_setlease(const uint8_t *mac, in_addr_t ipaddr, time_t expiry)
{
	/* Calculate the offset from the first IP address managed by DHCPD.
	 * ipaddr must be in host order!
	 */

	int ndx = ipaddr - CONFIG_NETUTILS_DHCPD_STARTIP;
	struct lease_s *ret = NULL;

	nvdbg("ipaddr: %08x ipaddr: %08x ndx: %d MAX: %d\n", ipaddr, CONFIG_NETUTILS_DHCPD_STARTIP, ndx, CONFIG_NETUTILS_DHCPD_MAXLEASES);

	/* Verify that the address offset is within the supported range */

	if (ndx >= 0 && ndx < CONFIG_NETUTILS_DHCPD_MAXLEASES) {
		ret = &g_state.ds_leases[ndx];
		memcpy(ret->mac, mac, DHCP_HLEN_ETHERNET);
		ret->allocated = true;
#ifdef HAVE_LEASE_TIME
		ret->expiry = dhcpd_time() + expiry;
#endif
	}

	return ret;
}

/****************************************************************************
 * Name: dhcp_leaseipaddr
 ****************************************************************************/

static inline in_addr_t dhcp_leaseipaddr(struct lease_s *lease)
{
	/* Return IP address in host order */

	return (in_addr_t)(lease - g_state.ds_leases) + CONFIG_NETUTILS_DHCPD_STARTIP;
}

/****************************************************************************
 * Name: dhcpd_findbymac
 ****************************************************************************/

static struct lease_s *dhcpd_findbymac(const uint8_t *mac)
{
	int i;

	for (i = 0; i < CONFIG_NETUTILS_DHCPD_MAXLEASES; i++) {
		if (memcmp(g_state.ds_leases[i].mac, mac, DHCP_HLEN_ETHERNET) == 0) {
			return &(g_state.ds_leases[i]);
		}
	}

	return NULL;
}

/****************************************************************************
 * Name: dhcpd_findbyipaddr
 ****************************************************************************/

static struct lease_s *dhcpd_findbyipaddr(in_addr_t ipaddr)
{
	if (ipaddr >= CONFIG_NETUTILS_DHCPD_STARTIP && ipaddr <= CONFIG_NETUTILS_DHCP_OPTION_ENDIP) {

		struct lease_s *lease = &g_state.ds_leases[ipaddr - CONFIG_NETUTILS_DHCPD_STARTIP];
		ndbg("dhcpd_findbyipaddr lease index  = %d\n", ipaddr - CONFIG_NETUTILS_DHCPD_STARTIP);

		if (lease->allocated > 0) {
			ndbg("returen lease %d %d\n", ipaddr - CONFIG_NETUTILS_DHCPD_STARTIP, g_state.ds_leases[ipaddr - CONFIG_NETUTILS_DHCPD_STARTIP].allocated);
			return lease;
		}
	}

	ndbg("returen null\n");

	return NULL;
}

/****************************************************************************
 * Name: dhcpd_allocipaddr
 ****************************************************************************/

static in_addr_t dhcpd_allocipaddr(void)
{
	struct lease_s *lease = NULL;
	in_addr_t ipaddr;

	ipaddr = CONFIG_NETUTILS_DHCPD_STARTIP;
	for (; ipaddr <= CONFIG_NETUTILS_DHCP_OPTION_ENDIP; ipaddr++) {
		/* Skip over address ending in 0 or 255 */

		if ((ipaddr & 0xff) == 0 || (ipaddr & 0xff) == 0xff) {
			continue;
		}

		/* Is there already a lease on this address?  If so, has it expired? */

		lease = dhcpd_findbyipaddr(ipaddr);
		if ((lease == NULL || dhcpd_leaseexpired(lease))) {
			ndbg("lease pass!!\n");
#ifdef CONFIG_CPP_HAVE_WARNING
			/**
			#warning "FIXME: Should check if anything responds to an ARP request or ping"
			#warning "       to verify that there is no other user of this IP address"
			**/
#endif

			ndbg("leases talbe = %d %d \n", ipaddr - CONFIG_NETUTILS_DHCPD_STARTIP, g_state.ds_leases[ipaddr - CONFIG_NETUTILS_DHCPD_STARTIP].allocated);

			memset(g_state.ds_leases[ipaddr - CONFIG_NETUTILS_DHCPD_STARTIP].mac, 0, DHCP_HLEN_ETHERNET);
			g_state.ds_leases[ipaddr - CONFIG_NETUTILS_DHCPD_STARTIP].allocated = true;
#ifdef HAVE_LEASE_TIME
			g_state.ds_leases[ipaddr - CONFIG_NETUTILS_DHCPD_STARTIP].expiry = dhcpd_time()
					+ CONFIG_NETUTILS_DHCPD_OFFERTIME;
#endif
			/* Return the address in host order */

			ndbg("return ipaddr\n");
			return ipaddr;
		}
	}
	ndbg("return ipaddr = 0\n");
	return 0;
}

/****************************************************************************
 * Name: dhcpd_parseoptions
 ****************************************************************************/

static inline bool dhcpd_parseoptions(void)
{
	uint32_t tmp;
	uint8_t *ptr;
#ifndef CONFIG_NET_DHCP_LIGHT
	uint8_t overloaded;
	uint8_t currfield;
#endif
	int optlen = 0;
	int remaining;

	/* Verify that the option field starts with a valid magic number */

	ptr = g_state.ds_inpacket.options;
	if (memcmp(ptr, g_magiccookie, 4) != 0) {
		/* Bad magic number... skip g_state.ds_outpacket */

		ndbg("Bad magic: %d,%d,%d,%d\n", ptr[0], ptr[1], ptr[2], ptr[3]);
		return false;
	}

	/* Set up to parse the options */

	ptr += 4;
	remaining = DHCPD_OPTIONS_SIZE - 4;
#ifndef CONFIG_NET_DHCP_LIGHT
	overloaded = DHCPD_OPTION_FIELD;
	currfield = DHCPD_OPTION_FIELD;
#endif

	/* Set all options to the default value */

	g_state.ds_optmsgtype = 0;	/* Incoming DHCP message type */
	g_state.ds_optreqip = 0;	/* Requested IP address (host order) */
	g_state.ds_optserverip = 0;	/* Serverip IP address (host order) */
	g_state.ds_optleasetime = 0;	/* Requested lease time (host order) */
	g_state.ds_optend = NULL;

	do {
		/* The form of an option is:
		 *   code   - 1 byte
		 *   length - 1 byte
		 *   data   - variable number of bytes
		 */

		switch (ptr[DHCPD_OPTION_CODE]) {
			/* Skip over any padding bytes */

		case DHCP_OPTION_PAD:
			optlen = 1;
			break;

			/* the Overload option is used to indicate that the DHCP 'sname' or 'file'
			 * fields are being overloaded by using them to carry DHCP options. A DHCP
			 * server inserts this option if the returned parameters will exceed the
			 * usual space allotted for options.
			 *
			 * If this option is present, the client interprets the specified additional
			 * fields after it concludes interpretation of the standard option fields.
			 *
			 * Legal values for this option are:
			 *
			 *   1     the 'file' field is used to hold options
			 *   2     the 'sname' field is used to hold options
			 *   3     both fields are used to hold options
			 */

#ifndef CONFIG_NET_DHCP_LIGHT
		case DHCP_OPTION_OVERLOAD:
			optlen = ptr[DHCPD_OPTION_LENGTH] + 2;
			if (optlen >= 1 && optlen < remaining) {
				overloaded = ptr[DHCPD_OPTION_DATA];
			}
			break;
#endif

		case DHCP_OPTION_END:
#ifndef CONFIG_NET_DHCP_LIGHT
			if (currfield == DHCPD_OPTION_FIELD && (overloaded & DHCPD_FILE_FIELD) != 0) {
				ptr = g_state.ds_inpacket.file;
				remaining = sizeof(g_state.ds_inpacket.file);
				currfield = DHCPD_FILE_FIELD;
			} else if (currfield == DHCPD_FILE_FIELD && (overloaded & DHCPD_SNAME_FIELD) != 0) {
				ptr = g_state.ds_inpacket.sname;
				remaining = sizeof(g_state.ds_inpacket.sname);
				currfield = DHCPD_SNAME_FIELD;
			} else {
				return true;
			}
			break;
#else
			return true;
#endif

		case DHCP_OPTION_REQ_IPADDR:	/* Requested IP Address */
			optlen = ptr[DHCPD_OPTION_LENGTH] + 2;
			if (optlen >= 4 && optlen < remaining) {
				memcpy(&tmp, &ptr[DHCPD_OPTION_DATA], 4);
				g_state.ds_optreqip = (in_addr_t)ntohl(tmp);
			}
			break;

		case DHCP_OPTION_LEASE_TIME:	/* IP address lease time */
			optlen = ptr[DHCPD_OPTION_LENGTH] + 2;
			if (optlen >= 4 && optlen < remaining) {
				memcpy(&tmp, &ptr[DHCPD_OPTION_DATA], 4);
				g_state.ds_optleasetime = (time_t)ntohl(tmp);
			}
			break;

		case DHCP_OPTION_MSG_TYPE:	/* DHCP message type */
			optlen = ptr[DHCPD_OPTION_LENGTH] + 2;
			if (optlen >= 1 && optlen < remaining) {
				g_state.ds_optmsgtype = ptr[DHCPD_OPTION_DATA];
			}
			break;

		case DHCP_OPTION_SERVER_ID:	/* Server identifier */
			optlen = ptr[DHCPD_OPTION_LENGTH] + 2;
			if (optlen >= 4 && optlen < remaining) {
				memcpy(&tmp, &ptr[DHCPD_OPTION_DATA], 4);
				g_state.ds_optserverip = (in_addr_t)ntohl(tmp);
			}
			break;

		default:
			/* Skip over unsupported options */

			optlen = ptr[DHCPD_OPTION_LENGTH] + 2;
			break;
		}

		/* Advance to the next option */

		ptr += optlen;
		remaining -= optlen;
	} while (remaining > 0);

	return false;
}

/****************************************************************************
 * Name: dhcpd_verifyreqip
 ****************************************************************************/

static inline bool dhcpd_verifyreqip(void)
{
	struct lease_s *lease;

	/* Verify that the requested IP address is within the supported lease range */

	if (g_state.ds_optreqip >= CONFIG_NETUTILS_DHCPD_STARTIP && g_state.ds_optreqip <= CONFIG_NETUTILS_DHCP_OPTION_ENDIP) {
		/* And verify that the lease has not already been taken or offered
		 * (unless the lease/offer is expired, then the address is free game).
		 */

		lease = dhcpd_findbyipaddr(g_state.ds_optreqip);
		ndbg(" lease = %d\n", lease);
		if (!lease || dhcpd_leaseexpired(lease)) {
			ndbg("can use IP %08lx\n", (long)g_state.ds_optreqip);
			return true;
		}
	}
	ndbg("can't use IP %08lx\n", (long)g_state.ds_optreqip);
	return false;
}

/****************************************************************************
 * Name: dhcpd_verifyreqleasetime
 ****************************************************************************/

static inline bool dhcpd_verifyreqleasetime(uint32_t *leasetime)
{
	uint32_t tmp = g_state.ds_optleasetime;

	/* Did the client request a specific lease time? */

	if (tmp != 0) {
		/* Yes..  Verify that the requested lease time is within a valid range */

		if (tmp > CONFIG_NETUTILS_DHCPD_MAXLEASETIME) {
			tmp = CONFIG_NETUTILS_DHCPD_MAXLEASETIME;
		} else if (tmp < CONFIG_NETUTILS_DHCPD_MINLEASETIME) {
			tmp = CONFIG_NETUTILS_DHCPD_MINLEASETIME;
		}

		/* Return the clipped lease time */

		*leasetime = tmp;
		return true;
	}

	return false;
}

/****************************************************************************
 * Name: dhcpd_addoption
 ****************************************************************************/

static int dhcpd_addoption(uint8_t *option)
{
	int offset;
	int len = 4;

	if (g_state.ds_optend) {
		offset = g_state.ds_outpacket.options - g_state.ds_optend;
		len = option[DHCPD_OPTION_LENGTH] + 2;

		/* Check if the option will fit into the options array */

		if (offset + len + 1 < DHCPD_OPTIONS_SIZE) {
			/* Copy the option into the option array */

			memcpy(g_state.ds_optend, option, len);
			g_state.ds_optend += len;
			*g_state.ds_optend = DHCP_OPTION_END;
		}
	}

	return len;
}

/****************************************************************************
 * Name: dhcpd_addoption8
 ****************************************************************************/

static int dhcpd_addoption8(uint8_t code, uint8_t value)
{
	uint8_t option[3];

	/* Construct the option sequence */

	option[DHCPD_OPTION_CODE] = code;
	option[DHCPD_OPTION_LENGTH] = 1;
	option[DHCPD_OPTION_DATA] = value;

	/* Add the option sequence to the response */

	return dhcpd_addoption(option);
}

/****************************************************************************
 * Name: dhcpd_addoption32
 ****************************************************************************/

static int dhcpd_addoption32(uint8_t code, uint32_t value)
{
	uint8_t option[6];

	/* Construct the option sequence */

	option[DHCPD_OPTION_CODE] = code;
	option[DHCPD_OPTION_LENGTH] = 4;
	memcpy(&option[DHCPD_OPTION_DATA], &value, 4);

	/* Add the option sequence to the response */

	return dhcpd_addoption(option);
}

/****************************************************************************
 * Name: dhcp_addoption32p
 ****************************************************************************/

#if HAVE_DNSIP
static int dhcp_addoption32p(uint8_t code, FAR uint8_t *value)
{
	uint8_t option[6];

	/* Construct the option sequence */

	option[DHCPD_OPTION_CODE] = code;
	option[DHCPD_OPTION_LENGTH] = 4;
	memcpy(&option[DHCPD_OPTION_DATA], value, 4);

	/* Add the option sequence to the response */

	return dhcpd_addoption(option);
}
#endif

/****************************************************************************
 * Name: dhcpd_soclet
 ****************************************************************************/

static inline int dhcpd_socket(void)
{
	int sockfd;
	int optval;
	int ret;

	/* Create a socket to listen for requests from DHCP clients */

	sockfd = socket(PF_INET, SOCK_DGRAM, 0);
	if (sockfd < 0) {
		ndbg("socket failed: %d\n", errno);
		return ERROR;
	}

	/* Configure the socket */
	optval = 1;
	ret = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (void *)&optval, sizeof(int));
	if (ret < 0) {
		ndbg("setsockopt SO_REUSEADDR failed: %d\n", errno);
		close(sockfd);
		return ERROR;
	}

	optval = 1;
	ret = setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, (void *)&optval, sizeof(int));
	if (ret < 0) {
		ndbg("setsockopt SO_BROADCAST failed: %d\n", errno);
		close(sockfd);
		return ERROR;
	}

	return sockfd;

}

/****************************************************************************
 * Name: dhcpd_initpacket
 ****************************************************************************/

static void dhcpd_initpacket(uint8_t mtype)
{
	uint32_t nulladdr = 0;

	/* Set up the generic parts of the DHCP server message */

	memset(&g_state.ds_outpacket, 0, sizeof(struct dhcpmsg_s));

	g_state.ds_outpacket.op = DHCP_REPLY;
	g_state.ds_outpacket.htype = g_state.ds_inpacket.htype;
	g_state.ds_outpacket.hlen = g_state.ds_inpacket.hlen;

	memcpy(&g_state.ds_outpacket.xid, &g_state.ds_inpacket.xid, 4);
	memcpy(g_state.ds_outpacket.chaddr, g_state.ds_inpacket.chaddr, 16);

	if (memcmp(g_state.ds_outpacket.giaddr, &nulladdr, 4) != 0) {
		g_state.ds_outpacket.flags = g_state.ds_inpacket.flags;
	} else {
		g_state.ds_outpacket.flags = 0;
	}
	memset(g_state.ds_outpacket.giaddr, 0, 4);

	/* Add the generic options */

	memcpy(g_state.ds_outpacket.options, g_magiccookie, 4);
	g_state.ds_optend = &g_state.ds_outpacket.options[4];
	*g_state.ds_optend = DHCP_OPTION_END;
	dhcpd_addoption8(DHCP_OPTION_MSG_TYPE, mtype);
	dhcpd_addoption32(DHCP_OPTION_SERVER_ID, g_state.ds_serverip);
}

/****************************************************************************
 * Name: dhcpd_sendpacket
 ****************************************************************************/

static int dhcpd_sendpacket(int bbroadcast)
{
	struct sockaddr_in addr;
	in_addr_t ipaddr;
	int len;
	int ret = ERROR;

#ifdef CONFIG_NETUTILS_DHCPD_IGNOREBROADCAST
	/* This is a hack.  I've had problems with Windows machines responding
	 * to unicast.  I think this is associated with a Windows registry key in
	 * HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\DHCPServer\Parameters:
	 * The IgnoreBroadcastFlag value controls this behavior:  A value of 1 will
	 * cause the server to ignore the client broadcast flag and always respond
	 * with multicast; the value 0 to allows clients to request unicast.
	 */

	ipaddr = INADDR_BROADCAST;
#else
	/* Determine which address to respond to (or if we need to broadcast the response)
	 *
	 * (1) If he caller know that it needs to multicast the response, it will set bbroadcast.
	 * (2) Otherwise, if the client already has and address (ciaddr), then use that for uni-cast
	 * (3) Broadcast if the client says it can't handle uni-cast (BOOTP_BROADCAST set)
	 * (4) Otherwise, the client claims it can handle the uni-casst response and we
	 *     will uni-cast to the offered address (yiaddr).
	 *
	 * NOTE: We really should also check the giaddr field.  If no zero, the server should
	 * send any return messages to the 'DHCP server' port on the BOOTP relay agent whose
	 * address appears in 'giaddr'.
	 */

	if (bbroadcast) {
		ipaddr = INADDR_BROADCAST;
	} else if (memcmp(g_state.ds_outpacket.ciaddr, g_anyipaddr, 4) != 0) {
		dhcpd_arpupdate((uint16_t *)g_state.ds_outpacket.ciaddr, g_state.ds_outpacket.chaddr);
		memcpy(&ipaddr, g_state.ds_outpacket.ciaddr, 4);
	} else if (g_state.ds_outpacket.flags & HTONS(BOOTP_BROADCAST)) {
		ipaddr = INADDR_BROADCAST;
	} else {
		dhcpd_arpupdate((uint16_t *)g_state.ds_outpacket.yiaddr, g_state.ds_outpacket.chaddr);
		memcpy(&ipaddr, g_state.ds_outpacket.yiaddr, 4);
	}
#endif

	/* Create a socket to respond with a packet to the client.  We
	 * cannot re-use the listener socket because it is not bound correctly
	 */
	if (g_dhcpd_sockfd == -1) {
		ndbg("socket is not valid\n");
	}

	/* Then send the reponse to the DHCP client port at that address */

	memset(&addr, 0, sizeof(struct sockaddr_in));
	addr.sin_family = AF_INET;
	addr.sin_port = HTONS(DHCP_CLIENT_PORT);
	addr.sin_addr.s_addr = ipaddr;

	/* Send the minimum sized packet that includes the END option */

	len = (g_state.ds_optend - (uint8_t *)&g_state.ds_outpacket) + 1;
	nvdbg("sendto %08lx:%04x len=%d\n", (long)ntohl(addr.sin_addr.s_addr), ntohs(addr.sin_port), len);
	ret = sendto(g_dhcpd_sockfd, &g_state.ds_outpacket, len, 0, (struct sockaddr *)&addr, sizeof(struct sockaddr_in));

	return ret;
}

/****************************************************************************
 * Name: dhcpd_send_offerpacket
 ****************************************************************************/

static int dhcpd_send_offerpacket(void)
{
	struct sockaddr_in addr;
	in_addr_t ipaddr;
	int len;
	int ret = ERROR;

	memcpy(&ipaddr, g_state.ds_outpacket.yiaddr, 4);

#if HAVE_NETMASK
	/* A dhcp offer message should be sent as broadcast message
	 *  Some platform might discard 255.255.255.255 broadcast message, so replace it to subnet broadcast message
	 */
	ipaddr |= (~htonl(CONFIG_NETUTILS_DHCPD_NETMASK));
#else
	ipaddr = htonl(IPADDR_BROADCAST);
#endif

	if (g_dhcpd_sockfd == -1) {
		ndbg("dhcpd socket is not valid\n");
	}
	/* Then send the reponse to the DHCP client port at that address */

	memset(&addr, 0, sizeof(struct sockaddr_in));
	addr.sin_family = AF_INET;
	addr.sin_port = HTONS(DHCP_CLIENT_PORT);
	addr.sin_addr.s_addr = ipaddr;

	/* Send the minimum sized packet that includes the END option */

	len = (g_state.ds_optend - (uint8_t *)&g_state.ds_outpacket) + 1;
	nvdbg("dhcp offer sendto %08lx:%04x len=%d\n", (long)ntohl(addr.sin_addr.s_addr), ntohs(addr.sin_port), len);
	ret = sendto(g_dhcpd_sockfd, &g_state.ds_outpacket, len, 0, (struct sockaddr *)&addr, sizeof(struct sockaddr_in));

	return ret;
}

/****************************************************************************
 * Name: dhcpd_sendoffer
 ****************************************************************************/

static inline int dhcpd_sendoffer(in_addr_t ipaddr, uint32_t leasetime)
{
	in_addr_t netaddr;
#if HAVE_DNSIP
	uint32_t dnsaddr;
	dnsaddr = htonl(CONFIG_NETUTILS_DHCPD_DNSIP);
#endif
	/* IP address is in host order */

	nvdbg("Sending offer: %08lx\n", (long)ipaddr);

	/* Initialize the outgoing packet */

	dhcpd_initpacket(DHCPOFFER);

	/* Add the address offered to the client (converting to network order) */

	netaddr = htonl(ipaddr);
	memcpy(g_state.ds_outpacket.yiaddr, &netaddr, 4);

	/* Add the leasetime to the response options */

	dhcpd_addoption32(DHCP_OPTION_LEASE_TIME, htonl(leasetime));
#if HAVE_NETMASK
	dhcpd_addoption32(DHCP_OPTION_SUBNET_MASK, htonl(CONFIG_NETUTILS_DHCPD_NETMASK));
#endif
#if HAVE_ROUTERIP
	dhcpd_addoption32(DHCP_OPTION_ROUTER, htonl(CONFIG_NETUTILS_DHCPD_ROUTERIP));
#endif
#if HAVE_DNSIP
	dhcp_addoption32p(DHCP_OPTION_DNS_SERVER, (FAR uint8_t *)&dnsaddr);
#endif

	/* Send the offer response */
	return dhcpd_send_offerpacket();
}

/****************************************************************************
 * Name: dhcpd_sendnak
 ****************************************************************************/

static int dhcpd_sendnak(void)
{
	/* Initialize and send the NAK response */

	dhcpd_initpacket(DHCPNAK);
	memcpy(g_state.ds_outpacket.ciaddr, g_state.ds_inpacket.ciaddr, 4);
	return dhcpd_sendpacket(true);
}

/****************************************************************************
 * Name: dhcpd_sendack
 ****************************************************************************/

int dhcpd_sendack(in_addr_t ipaddr)
{
	uint32_t leasetime = CONFIG_NETUTILS_DHCPD_LEASETIME;
	in_addr_t netaddr;
#if HAVE_DNSIP
	uint32_t dnsaddr;
	dnsaddr = htonl(CONFIG_NETUTILS_DHCPD_DNSIP);
#endif

	/* Initialize the ACK response */

	dhcpd_initpacket(DHCPACK);
	memcpy(g_state.ds_outpacket.ciaddr, g_state.ds_inpacket.ciaddr, 4);

	/* Add the IP address assigned to the client */

	netaddr = htonl(ipaddr);
	memcpy(g_state.ds_outpacket.yiaddr, &netaddr, 4);

	/* Did the client request a specific lease time? */

	(void)dhcpd_verifyreqleasetime(&leasetime);

	/* Add the lease time to the response */

	dhcpd_addoption32(DHCP_OPTION_LEASE_TIME, htonl(leasetime));
#if HAVE_NETMASK
	dhcpd_addoption32(DHCP_OPTION_SUBNET_MASK, htonl(CONFIG_NETUTILS_DHCPD_NETMASK));
#endif
#if HAVE_ROUTERIP
	dhcpd_addoption32(DHCP_OPTION_ROUTER, htonl(CONFIG_NETUTILS_DHCPD_ROUTERIP));
#endif
#if HAVE_DNSIP
	dhcp_addoption32p(DHCP_OPTION_DNS_SERVER, (FAR uint8_t *)&dnsaddr);
#endif

	if (dhcpd_sendpacket(true) < 0) {
		return ERROR;
	}

	dhcpd_setlease(g_state.ds_inpacket.chaddr, ipaddr, leasetime);
	/* TODO: new callback way up to application to inform a new STA has called
	 * dhcp client
	 */
	if (g_dhcp_sta_joined) {
		g_dhcp_sta_joined();
	}
	return OK;
}

/****************************************************************************
 * Name: dhcpd_discover
 ****************************************************************************/

static inline int dhcpd_discover(void)
{
	struct lease_s *lease;
	in_addr_t ipaddr;
	uint32_t leasetime = CONFIG_NETUTILS_DHCPD_LEASETIME;

	/* Check if the client is aleady in the lease table */

	lease = dhcpd_findbymac(g_state.ds_inpacket.chaddr);
	if (lease) {
		/* Yes... get the remaining time on the lease */

#ifdef HAVE_LEASE_TIME
		if (!dhcpd_leaseexpired(lease)) {
			leasetime = lease->expiry - dhcpd_time();
			if (leasetime < CONFIG_NETUTILS_DHCPD_MINLEASETIME) {
				leasetime = CONFIG_NETUTILS_DHCPD_MINLEASETIME;
			}
		}
#endif
		/* Get the IP address associated with the lease (host order) */

		ipaddr = dhcp_leaseipaddr(lease);
		ndbg("Already have lease for IP %08lx\n", (long)ipaddr);
	}

	/* Check if the client has requested a specific IP address */

	else if (dhcpd_verifyreqip()) {
		/* Use the requested IP address (host order) */

		ipaddr = g_state.ds_optreqip;

		ndbg("User requested IP %08lx\n", (long)ipaddr);
	} else {
		/* No... allocate a new IP address (host order) */

		ipaddr = dhcpd_allocipaddr();
		ndbg("Allocated IP %08lx\n", (long)ipaddr);
	}

	/* Did we get any IP address? */

	if (!ipaddr) {
		/* Nope... return failure */

		ndbg("Failed to get IP address\n");
		return ERROR;
	}

	/* Reserve the leased IP for a shorter time for the offer */

	if (!dhcpd_setlease(g_state.ds_inpacket.chaddr, ipaddr, CONFIG_NETUTILS_DHCPD_OFFERTIME)) {
		ndbg("Failed to set lease\n");
		return ERROR;
	}

	/* Check if the client has requested a specific lease time */

	(void)dhcpd_verifyreqleasetime(&leasetime);

	/* Send the offer response */

	return dhcpd_sendoffer(ipaddr, leasetime);
}

/****************************************************************************
 * Name: dhcpd_request
 ****************************************************************************/

static inline int dhcpd_request(void)
{
	struct lease_s *lease;
	in_addr_t ipaddr = 0;
	uint8_t response = 0;

	/* Check if this client already holds a lease.  This can happen when the client (1)
	 * the IP is reserved for the client from a previous offer, or (2) the client is
	 * re-initializing or rebooting while the lease is still valid.
	 */

	lease = dhcpd_findbymac(g_state.ds_inpacket.chaddr);
	if (lease) {
		/* Yes.. the client already holds a lease.  Verify that the request is consistent
		 * with the existing lease (host order).
		 */

		ipaddr = dhcp_leaseipaddr(lease);
		ndbg("Lease ipaddr: %08x Server IP: %08x Requested IP: %08x\n", ipaddr, g_state.ds_optserverip, g_state.ds_optreqip);

		if (g_state.ds_optserverip) {
			/* ACK if the serverip is correct and the requested IP address is the one
			 * already offered to the client.
			 */

			if (g_state.ds_optserverip == ntohl(g_state.ds_serverip) && (g_state.ds_optreqip != 0 || g_state.ds_optreqip == ipaddr)) {
				response = DHCPACK;
			} else {
				response = DHCPNAK;
			}
		}

		/* We have the lease and no server IP was requested. Was a specific IP address
		 * requested? (host order)
		 */

		else if (g_state.ds_optreqip) {
			/* Yes..ACK if the requested IP address is the one already leased.
			 * Both addresses are in host order.
			 */

			if (ipaddr == g_state.ds_optreqip) {
				response = DHCPACK;
			} else {
				response = DHCPNAK;
			}
		}

		/* The client has specified neither a server IP nor requested IP address */

		else {
			/* ACK if the IP used by the client is the one already assigned to it.
			 * NOTE ipaddr is in host order; ciaddr is network order!
			 */

			uint32_t tmp = htonl(ipaddr);
			if (memcmp(&tmp, g_state.ds_inpacket.ciaddr, 4) == 0) {
				response = DHCPACK;
			} else {
				response = DHCPNAK;
			}
		}
	}

	/* The client does not hold a lease (referenced by its MAC address) and is
	 * requesting a specific IP address that was, apparently, never offered to
	 * to the client.  Perform some sanity checks before sending the NAK.
	 */

	else if (g_state.ds_optreqip && !g_state.ds_optserverip) {
		ndbg("Server IP: %08x Requested IP: %08x\n", g_state.ds_optserverip, g_state.ds_optreqip);

		/* Is this IP address already assigned? */

		lease = dhcpd_findbyipaddr(g_state.ds_optreqip);
		if (lease) {
			/* Yes.. Send NAK unless the lease has expired */

			if (!dhcpd_leaseexpired(lease)) {
				response = DHCPNAK;
			}
		}

		/* No.. is the requested IP address in range? NAK if not */

		else if (g_state.ds_optreqip < CONFIG_NETUTILS_DHCPD_STARTIP || g_state.ds_optreqip > CONFIG_NETUTILS_DHCP_OPTION_ENDIP) {
			response = DHCPNAK;
		}
	}

	/* Otherwise, the client does not hold a lease and is not requesting any
	 * specific IP address.
	 */

	/* Finally, either (1) send the ACK, (2) send a NAK, or (3) remain silent
	 * based on the checks above.
	 */

	if (response == DHCPACK) {
		ndbg("ACK IP %08lx\n", (long)ipaddr);

		memset(g_state.ds_leases[ipaddr - CONFIG_NETUTILS_DHCPD_STARTIP].mac, 0, DHCP_HLEN_ETHERNET);
		g_state.ds_leases[ipaddr - CONFIG_NETUTILS_DHCPD_STARTIP].allocated = true;
#ifdef HAVE_LEASE_TIME
		g_state.ds_leases[ipaddr - CONFIG_NETUTILS_DHCPD_STARTIP].expiry = dhcpd_time() + CONFIG_NETUTILS_DHCPD_OFFERTIME;
#endif
		if (dhcpd_sendack(ipaddr) == ERROR) {
			ndbg("Failed to send ACK\n");
			return ERROR;
		}
	} else if (response == DHCPNAK) {
		ndbg("NAK IP %08lx\n", (long)ipaddr);
		if (dhcpd_sendnak() == ERROR) {
			ndbg("Failed to send NAK\n");
			return ERROR;
		}
	} else {
		ndbg("Remaining silent IP %08lx\n", (long)ipaddr);
	}

	return OK;
}

/****************************************************************************
 * Name: dhcpd_decline
 ****************************************************************************/

static inline int dhcpd_decline(void)
{
	struct lease_s *lease;

	/* Find the lease associated with this hardware address */

	lease = dhcpd_findbymac(g_state.ds_inpacket.chaddr);
	if (lease) {
		/* Disassociate the IP from the MAC, but prevent re-used of this
		 * address for a period of time.
		 */

		memset(lease->mac, 0, DHCP_HLEN_ETHERNET);
#ifdef HAVE_LEASE_TIME
		lease->expiry = dhcpd_time() + CONFIG_NETUTILS_DHCPD_DECLINETIME;
#endif
	}

	return OK;
}

static inline int dhcpd_release(void)
{
	struct lease_s *lease;

	/* Find the lease associated with this hardware address */

	lease = dhcpd_findbymac(g_state.ds_inpacket.chaddr);
	if (lease) {
		/* Release the IP address now */

		memset(lease, 0, sizeof(struct lease_s));
	}

	return OK;
}

/****************************************************************************
 * Name: dhcpd_openlistener
 ****************************************************************************/

static inline int dhcpd_openlistener(void)
{
	struct sockaddr_in addr;
	struct ifreq req;
	int sockfd;
	int ret;

	/* Create a socket to listen for requests from DHCP clients */

	sockfd = dhcpd_socket();
	if (sockfd < 0) {
		ndbg("socket failed: %d\n", errno);
		return ERROR;
	}

	/* Get the IP address of the selected device */

	strncpy(req.ifr_name, DHCPD_IFNAME, IFNAMSIZ);
	ret = ioctl(sockfd, SIOCGIFADDR, (unsigned long)&req);
	if (ret < 0) {
		ndbg("setsockopt SIOCGIFADDR failed: %d\n", errno);
		close(sockfd);
		return ERROR;
	}

	g_state.ds_serverip = ((struct sockaddr_in *)&req.ifr_addr)->sin_addr.s_addr;
	nvdbg("serverip: %08lx\n", ntohl(g_state.ds_serverip));

	/* Bind the socket to a local port. We have to bind to INADDRY_ANY to
	 * receive broadcast messages.
	 */

	addr.sin_family = AF_INET;
	addr.sin_port = htons(DHCP_SERVER_PORT);
	addr.sin_addr.s_addr = INADDR_ANY;

	ret = bind(sockfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in));
	if (ret < 0) {
		ndbg("bind failed, port=%d addr=%08lx: %d\n", addr.sin_port, (long)addr.sin_addr.s_addr, errno);
		close(sockfd);
		return ERROR;
	}

	return sockfd;
}

/****************************************************************************
 * Name: dhcpd_netif_init
 ****************************************************************************/

static int dhcpd_netif_init(char *intf)
{
	struct in_addr server_ipaddr;
	struct in_addr netmask_ipaddr;

	server_ipaddr.s_addr = htonl(CONFIG_NETUTILS_DHCPD_ROUTERIP);
	netmask_ipaddr.s_addr = htonl(CONFIG_NETUTILS_DHCPD_NETMASK);

	if (netlib_set_ipv4addr(intf, &server_ipaddr) == ERROR) {
		ndbg("failed to set server IP address\n");
		return -1;
	}

	if (netlib_set_ipv4netmask(intf, &netmask_ipaddr) == ERROR) {
		ndbg("failed to set netmask\n");
		return -1;
	}

	if (netlib_set_dripv4addr(intf, &server_ipaddr) == ERROR) {
		ndbg("failed to set default gateway\n");
		return -1;
	}

	ndbg("\n");

	ndbg("Server IP address set : %u.%u.%u.%u\n", (unsigned char)((htonl(server_ipaddr.s_addr) >> 24) & 0xff), (unsigned char)((htonl(server_ipaddr.s_addr) >> 16) & 0xff), (unsigned char)((htonl(server_ipaddr.s_addr) >> 8) & 0xff), (unsigned char)((htonl(server_ipaddr.s_addr) >> 0) & 0xff));

	ndbg("Server netmask address set : %u.%u.%u.%u\n", (unsigned char)((htonl(netmask_ipaddr.s_addr) >> 24) & 0xff), (unsigned char)((htonl(netmask_ipaddr.s_addr) >> 16) & 0xff), (unsigned char)((htonl(netmask_ipaddr.s_addr) >> 8) & 0xff), (unsigned char)((htonl(netmask_ipaddr.s_addr) >> 0) & 0xff));

	ndbg("Server default gateway address set : %u.%u.%u.%u\n", (unsigned char)((htonl(server_ipaddr.s_addr) >> 24) & 0xff), (unsigned char)((htonl(server_ipaddr.s_addr) >> 16) & 0xff), (unsigned char)((htonl(server_ipaddr.s_addr) >> 8) & 0xff), (unsigned char)((htonl(server_ipaddr.s_addr) >> 0) & 0xff));

	ndbg("\n");
	return 0;
}

/****************************************************************************
 * Name: dhcpd_netif_deinit
 ****************************************************************************/

static int dhcpd_netif_deinit(char *intf)
{
	struct in_addr server_ipaddr;
	struct in_addr netmask_ipaddr;

	server_ipaddr.s_addr = IPADDR_ANY;
	netmask_ipaddr.s_addr = IPADDR_NONE;

	if (netlib_set_ipv4addr(intf, &server_ipaddr) == ERROR) {
		ndbg("failed to set server IP address\n");
		return -1;
	}

	if (netlib_set_ipv4netmask(intf, &netmask_ipaddr) == ERROR) {
		ndbg("failed to set netmask\n");
		return -1;
	}

	if (netlib_set_dripv4addr(intf, &server_ipaddr) == ERROR) {
		ndbg("failed to set default gateway\n");
		return -1;
	}

	return 0;
}

/****************************************************************************
 * Name: dhcpd_status
 ****************************************************************************/

static int dhcpd_status(void)
{
	return g_dhcpd_running;
}

/****************************************************************************
 * Name: dhcpd_stop
 ****************************************************************************/

static void dhcpd_stop(void)
{
	int ret = -1;
	pthread_mutex_lock(&g_dhcpd_lock);
	if (g_dhcpd_running == 0) {
		pthread_mutex_unlock(&g_dhcpd_lock);
		return;
	}

	g_dhcpd_quit = 1;
	while (ret != OK) {
		ret = sem_wait(&g_dhcpd_sem);
		if (ret != OK) {
			ndbg("ERR: sem_wait for dhcpd failed\n");
			if (errno == EINTR) {
				ndbg("ERR: EINTR for sem_wait in dhcpd\n");
				continue;
			}
			pthread_mutex_unlock(&g_dhcpd_lock);			
			return;
		}
	}
	g_dhcpd_running = 0;
	ret = sem_destroy(&g_dhcpd_sem);
	if (ret != OK) {
		ndbg("ERR: sem_destroy for dhcpd failed\n");
		pthread_mutex_unlock(&g_dhcpd_lock);
		return;
	}
#if DHCPD_SELECT
	ndbg("WARN : dhcpd will be stopped after %d seconds\n", g_select_timeout.tv_sec);
#endif
	pthread_mutex_unlock(&g_dhcpd_lock);
}

/****************************************************************************
 * Name: dhcpd_run
 ****************************************************************************/

static int dhcpd_run(void *arg)
{
	int nbytes;
#if DHCPD_SELECT
	int ret = OK;
	fd_set sockfd_set;
#endif
	ndbg("Started on %s\n", DHCPD_IFNAME);

	/* Initialize everything to zero */
	memset(&g_state, 0, sizeof(struct dhcpd_state_s));

	/* Initialize netif address (ip address, netmask, default gateway) */

	if (dhcpd_netif_init(DHCPD_IFNAME) < 0) {
		ndbg("Failed to initialize network interface %s\n", DHCPD_IFNAME);
		ret = ERROR;
		return ret;
	}

	/* Now loop indefinitely, reading packets from the DHCP server socket */

	g_dhcpd_sockfd = -1;
	g_dhcpd_quit = 0;

	/* Create a socket to listen for requests from DHCP clients */
	/* TODO : Need to add cancellation point */
	g_dhcpd_sockfd = dhcpd_openlistener();
	if (g_dhcpd_sockfd < 0) {
		ndbg("Failed to create socket\n");
		ret = ERROR;
		goto exit_with_error;
	}

	while (!g_dhcpd_quit) {
#if DHCPD_SELECT
		nbytes = -1;
		FD_ZERO(&sockfd_set);
		FD_SET(g_dhcpd_sockfd, &sockfd_set);

		ret = select(g_dhcpd_sockfd + 1, &sockfd_set, NULL, NULL, &g_select_timeout);
		if ((ret > 0) && FD_ISSET(g_dhcpd_sockfd, &sockfd_set)) {
			/* Read the next g_state.ds_outpacket */
			nbytes = recv(g_dhcpd_sockfd, &g_state.ds_inpacket, sizeof(struct dhcpmsg_s), 0);
		} else if (ret == 0) {
			if (!g_dhcpd_quit) {
				continue;
			} else {
				ndbg("select timeout exit\n");
				break;
			}
		} else {
			/* Debugging purpose : Error case */
			ndbg("ERROR, select ret %d [errno %d]\n", ret, errno);
			break;
		}
#else
		nbytes = recv(g_dhcpd_sockfd, &g_state.ds_inpacket, sizeof(struct dhcpmsg_s), 0);
#endif
		if (nbytes < 0) {
			/* On errors (other EINTR), close the socket and try again */

			ndbg("recv failed: %d\n", errno);
			if (errno != EINTR) {
				close(g_dhcpd_sockfd);
				g_dhcpd_sockfd = -1;
			}
			continue;
		}

		/* Parse the incoming message options */

		if (!dhcpd_parseoptions()) {
			/* Failed to parse the message options */

			ndbg("No msg type\n");

			continue;
		}
#ifdef CONFIG_NETUTILS_DHCPD_HOST
		/* Get the poor little uC a change to get its recvfrom in place */

		usleep(500 * 1000);
#endif

		/* Now process the incoming DHCP message by its message type */

		switch (g_state.ds_optmsgtype) {
		case DHCPDISCOVER:
			ndbg("DHCPDISCOVER\n");
			int res = dhcpd_discover();
			if (res == ERROR) {
				ndbg("dhcpd discover fail\n");
			}
			break;

		case DHCPREQUEST:
			ndbg("DHCPREQUEST\n");
			dhcpd_request();
			break;

		case DHCPDECLINE:
			ndbg("DHCPDECLINE\n");
			dhcpd_decline();
			break;

		case DHCPRELEASE:
			ndbg("DHCPRELEASE\n");
			dhcpd_release();
			break;

		case DHCPINFORM:		/* Not supported */
		default:
			ndbg("Unsupported message type: %d\n", g_state.ds_optmsgtype);
			break;
		}
	}

	if (g_dhcpd_sockfd != -1) {
		close(g_dhcpd_sockfd);
	}
	sem_post(&g_dhcpd_sem);

exit_with_error:
	/* We don't need to de-initialize g_dhcp_sta_joined
       because it will be initialized at dhcpd_start*/
	/* de-initialize netif address (ip address, netmask, default gateway) */

	if (dhcpd_netif_deinit(DHCPD_IFNAME) < 0) {
		ndbg("Failed to deinit netif %s\n", DHCPD_IFNAME);
	}

	return ret;
}

/****************************************************************************
 * Name: dhcpd_start
 ****************************************************************************/

#define DHCPD_STACK_SIZE		20480
#define DHCPD_SCHED_PRI			100
#define DHCPD_SCHED_POLICY		SCHED_RR

static int dhcpd_start(char *intf, dhcp_sta_joined dhcp_join_cb)
{
	pthread_mutex_lock(&g_dhcpd_lock);
	if (g_dhcpd_running == 1) {
		pthread_mutex_unlock(&g_dhcpd_lock);
		return -1;
	}

	pthread_attr_t attr;
	int status;
	int ret;
	struct sched_param sparam;

	g_dhcp_sta_joined = 0;

	ret = sem_init(&g_dhcpd_sem, 0, 0);
	if (ret != OK) {
		ndbg("failed to initialize semaphore\n");
		goto err_exit;
	}
	/* Set network interface name for DHCPD */
	if (intf) {
		strncpy(DHCPD_IFNAME, intf, strlen(intf));
	} else {
		ndbg("failed to get interface\n");
		goto err_exit;
	}

	status = pthread_attr_init(&attr);
	if (status != 0) {
		ndbg("failed to start dhcpd\n");
		goto err_exit;
	}

	sparam.sched_priority = DHCPD_SCHED_PRI;
	status = pthread_attr_setschedparam(&attr, &sparam);
	if (status != 0) {
		ndbg("failed to pthread_attr_setschedparam, ret %d, errno %d\n", status, errno);
		goto err_exit;
	}

	status = pthread_attr_setschedpolicy(&attr, DHCPD_SCHED_POLICY);
	if (status != 0) {
		ndbg("failed to pthread_attr_setchedpolicy, ret %d, errno %d\n", status, errno);
		goto err_exit;
	}

	status = pthread_attr_setstacksize(&attr, DHCPD_STACK_SIZE);
	if (status != 0) {
		ndbg("failed to pthread_attr_setstacksize, ret %d, errno %d\n", status, errno);
		goto err_exit;
	}

	if (dhcp_join_cb) {
		g_dhcp_sta_joined = dhcp_join_cb;
	}

	status = pthread_create(&g_tid, &attr, (pthread_startroutine_t)dhcpd_run, NULL);
	if (status != 0) {
		ndbg("failed to start dhcpd\n");
		goto err_exit;
	}

	pthread_setname_np(g_tid, "dhcpd");
	pthread_detach(g_tid);

	g_dhcpd_running = 1;
	pthread_mutex_unlock(&g_dhcpd_lock);

	return 0;
err_exit:
	g_dhcpd_running = 0;
	pthread_mutex_unlock(&g_dhcpd_lock);

	return -1;
}

/****************************************************************************
 * Name: dhcps_server_status
 ****************************************************************************/
int dhcp_server_status(char *intf)
{
	return dhcpd_status();
}

/****************************************************************************
 * Name: dhcps_server_start
 ****************************************************************************/

int dhcp_server_start(char *intf, dhcp_sta_joined dhcp_join_cb)
{
	if (dhcpd_start(intf, dhcp_join_cb) >= 0) {
		ndbg("[DHCPS] started successfully (External app)\n");
		return OK;
	}
	return -1;
}

/****************************************************************************
 * Name: dhcps_server_stop
 ****************************************************************************/

int dhcp_server_stop(char *intf)
{
	dhcpd_stop();
	ndbg("[DHCPS] stopped successfully (External app)\n");
	return OK;
}
