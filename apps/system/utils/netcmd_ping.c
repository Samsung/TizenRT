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
 ****************************************************************************/

#include <tinyara/config.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <net/lwip/icmp.h>
#include <net/lwip/icmp6.h>
#include <net/lwip/ip.h>
#include <net/lwip/arch.h>
#include "netcmd.h"
#include "netcmd_ping.h"

#undef htons
#define htons HTONS
#define PING_RESULT(ping_ok)
#define PING_ID        0xAFAF
#define PING_DELAY     1000
#define ICMP_HDR_SIZE sizeof(struct ip_hdr) + sizeof(struct icmp_echo_hdr)
#ifdef CONFIG_NET_IPv6
#define ICMP6_HDR_SIZE sizeof(struct ip6_hdr) + sizeof(struct icmp6_echo_hdr)
#define PING_SIZE(proto) ((proto == IPPROTO_ICMPV6) ? ICMP6_HDR_SIZE : ICMP_HDR_SIZE )
#else
#define PING_SIZE(proto) (ICMP_HDR_SIZE + 32)
#endif

#ifndef CONFIG_NET_PING_CMD_ICOUNT
#define CONFIG_NET_PING_CMD_ICOUNT 5
#endif

static int g_ping_recv_counter;
static uint16_t g_ping_seq_num;

static void ping_usage(void)
{
	printf("ping -c <count> -s <data size> <destination address> \n");
	return;
}

static int ping_options(int argc, char **argv, int *count, int *size, char **staddr)
{
	FAR const char *fmt = fmtarginvalid;
	bool badarg = false;
	int option;
	int tmp;

	/* Get the ping options */

	optind = -1;
	while ((option = getopt(argc, argv, ":c:s:")) != ERROR) {
		switch (option) {
		case 'c':
			tmp = atoi(optarg);
			if (tmp < 1 || tmp > 10000) {
				printf(fmtargrange, argv[0]);
				badarg = true;
			} else {
				*count = tmp;
			}
			break;

		case 's':
			tmp = atoi(optarg);
			*size = tmp;
			break;

		case ':':
			printf(fmtargrequired, argv[0]);
			badarg = true;
			break;

		case '?':
		default:
			printf(fmtarginvalid, argv[0]);
			badarg = true;
			break;
		}
	}

	/* If a bad argument was encountered, then return without processing the
	 * command
	 */

	if (badarg) {
		return ERROR;
	}

	/* There should be exactly on parameter left on the command-line */

	if (optind == argc - 1) {
		*staddr = argv[optind];
	} else if (optind >= argc) {
		fmt = fmtargrequired;
		goto errout;
	} else {
		fmt = fmttoomanyargs;
		goto errout;
	}

	return OK;

errout:
	printf(fmt, argv[0]);
	ping_usage();
	return ERROR;
}

static u16_t standard_chksum(void *dataptr, u16_t len)
{
	u32_t acc;
	u16_t src;
	u8_t *octetptr;

	acc = 0;
	/* dataptr may be at odd or even addresses */
	octetptr = (u8_t *) dataptr;
	while (len > 1) {
		/* declare first octet as most significant
		   thus assume network order, ignoring host order */
		src = (*octetptr) << 8;
		octetptr++;
		/* declare second octet as least significant */
		src |= (*octetptr);
		octetptr++;
		acc += src;
		len -= 2;
	}
	if (len > 0) {
		/* accumulate remaining octet */
		src = (*octetptr) << 8;
		acc += src;
	}
	/* add deferred carry bits */
	acc = (acc >> 16) + (acc & 0x0000ffffUL);
	if ((acc & 0xffff0000UL) != 0) {
		acc = (acc >> 16) + (acc & 0x0000ffffUL);
	}
	/* This maybe a little confusing: reorder sum using htons()
	   instead of ntohs() since it has a little less call overhead.
	   The caller must invert bits for Internet sum ! */
	return htons((u16_t) acc);
}

static void ping_recv(int family, int s, struct timespec *ping_time)
{
	char buf[64];
	char addr_str[64];
	socklen_t fromlen;
	int status = ERROR;
	int len;
	struct sockaddr *from = NULL;
	struct icmp_echo_hdr *iecho = NULL;
	struct ip_hdr *iphdr = NULL;

#ifdef CONFIG_NET_IPv6
	if (family == AF_INET6) {
		fromlen = sizeof(struct sockaddr_in6);
	} else
#endif
	if (family == AF_INET) {
		fromlen = sizeof(struct sockaddr_in);
	} else {
		printf("ping_recv: invalid family\n");
		return;
	}

	/* allocate memory due to difference of size between ipv4/v6 socket structure */
	from = malloc(fromlen);
	if (from == NULL) {
		printf("ping_recv: fail to allocate memory\n");
		return;
	}

	while (1) {
		len = recvfrom(s, buf, sizeof(buf), 0, from, &fromlen);
		if (len < 0) {
			printf("ping_recv: recvfrom error(%d)\n", errno);
			goto err_out;
		} else if (len == 0) {
			printf("ping_recv: timeout\n");
		}

#ifdef CONFIG_NET_IPv6
		if (family == AF_INET6) {
			if (len >= ICMP6_HDR_SIZE) {
				struct ip6_hdr *ip6hdr;
				unsigned char nexth;
				char *curp;
				int ok = 0;

				ip6hdr = (struct ip6_hdr *)buf;
				len = IP6H_PLEN(ip6hdr);
				curp = (char *)(buf + sizeof(struct ip6_hdr));

				nexth = ip6hdr->_nexth;
				while (nexth != IP6_NEXTH_NONE) {
					switch (nexth) {
						case IP6_NEXTH_FRAGMENT:
						{
							struct ip6_frag_hdr *frag_hdr;

							frag_hdr = (struct ip6_frag_hdr *)curp;

							nexth = frag_hdr->_nexth;
							curp += (sizeof(struct ip6_frag_hdr));
							len -= (sizeof(struct ip6_frag_hdr));
							break;
						}
						case IP6_NEXTH_ICMP6:
							ok = 1;
							nexth = IP6_NEXTH_NONE;
							break;
						default:
							nexth = IP6_NEXTH_NONE;
							break;
					}
				}

				printf("ok %d\n", ok);
				if (ok) {
					iecho = (struct icmp_echo_hdr *)(curp);

					if (iecho->type == ICMP6_TYPE_EREP) {
						inet_ntop(family, (void *)&((struct sockaddr_in6 *)from)->sin6_addr, addr_str, 64);
						status = OK;
					}
				}
			}
		} else
#endif /* CONFIG_NET_IPv6 */
		{
			if (len >= ICMP_HDR_SIZE) {
				inet_ntop(family, (void *)&((struct sockaddr_in *)from)->sin_addr, addr_str, 64);

				iphdr = (struct ip_hdr *)buf;
				len = htons(IPH_LEN(iphdr)) - IP_HLEN;
				iecho = (struct icmp_echo_hdr *)(buf + (IPH_HL(iphdr) * 4));
				if (iecho->type == ICMP_ER) {
					status = OK;
				}
			}
		}

		if (status == OK) {
			uint32_t elapsed;
			struct timespec now;

			clock_gettime(CLOCK_REALTIME, &now);
			g_ping_recv_counter++;
			elapsed = (now.tv_sec - ping_time->tv_sec) * 1000 + (now.tv_nsec - ping_time->tv_nsec) / 1000000;
			printf(" %d bytes from %s: icmp_seq=%d ttl=255 time=%" U32_F "ms\n", len, addr_str, g_ping_seq_num, elapsed);

			if ((iecho->id == PING_ID) && (iecho->seqno == htons(g_ping_seq_num))) {
				/* do some ping result processing */
				PING_RESULT((ICMPH_TYPE(iecho) == ICMP_ER));
				return;
			} else {
				printf("drop\n");
			}
		}
	}

	free(from);
	return;
err_out:
	if (from) {
		free(from);
		from = NULL;
	}
}

static void ping_prepare_echo(int family, struct icmp_echo_hdr *iecho, u16_t len)
{
	int icmp_hdrlen;
	size_t i;

#ifdef CONFIG_NET_IPv6
	if (family == AF_INET6) {
		icmp_hdrlen = sizeof(struct icmp6_echo_hdr);
	} else
#endif
	{
		icmp_hdrlen = sizeof(struct icmp_echo_hdr);
	}
	ICMPH_CODE_SET(iecho, 0);
	iecho->id = PING_ID;
	++g_ping_seq_num;
	iecho->seqno = htons(g_ping_seq_num);

	/* fill the additional data buffer with some data */
	for (i = icmp_hdrlen; i < len; i++) {
		((char *)iecho)[i] = (char)i;
	}

#ifdef CONFIG_NET_IPv6
	if (family == AF_INET6) {
		ICMPH_TYPE_SET(iecho, ICMP6_TYPE_EREQ);
		iecho->chksum = 0;
	} else
#endif
	{
		ICMPH_TYPE_SET(iecho, ICMP_ECHO);
		iecho->chksum = 0;
		iecho->chksum = ~standard_chksum(iecho, len);
	}
}

static int ping_send(int s, struct sockaddr *to, int size)
{
	int ret;
	size_t icmplen;
	socklen_t addrlen;
	struct icmp_echo_hdr *iecho = NULL;

#ifdef CONFIG_NET_IPv6
	if (to->sa_family == AF_INET6) {
		addrlen = sizeof(struct sockaddr_in6);
		icmplen = sizeof(struct icmp6_echo_hdr) + size;
	} else
#endif
	if (to->sa_family == AF_INET) {
		addrlen = sizeof(struct sockaddr_in);
		icmplen = sizeof(struct icmp_echo_hdr) + size;
	} else {
		printf("ping_send: invalid family\n");
		return ERROR;
	}

	iecho = (struct icmp_echo_hdr *)malloc(icmplen);
	if (!iecho) {
		printf("ping_send: fail to allocate memory\n");
		return ERROR;
	}

	ping_prepare_echo((int)to->sa_family, iecho, (u16_t)icmplen);

	ret = sendto(s, iecho, icmplen, 0, to, addrlen);
	if (ret <= 0) {
		free(iecho);
		return ret;
	}
	free(iecho);

	return 0;
}

int ping_process(int count, const char *taddr, int size)
{
	int s = -1;
	int ping_send_counter = 0;
	struct timeval tv;
	struct timespec ping_time;
	struct addrinfo hints;
	struct addrinfo *result = NULL;
	struct addrinfo *rp = NULL;
	struct sockaddr *to = NULL;

	g_ping_seq_num = 0;
	g_ping_recv_counter = 0;

	/* write information for getaddrinfo() */
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_RAW;
	if (strstr(taddr, ".") != NULL) {
		hints.ai_protocol = IPPROTO_ICMP;
	}
#ifdef CONFIG_NET_IPv6
	if (strstr(taddr, ":") != NULL) {
		hints.ai_protocol = hints.ai_protocol ? 0 : IPPROTO_ICMPV6;
	}
#endif /* CONFIG_NET_IPv6 */
	if (hints.ai_protocol == 0) {
		printf("ping_process: invalid target ip address\n");
		return ERROR;
	}

	printf("PING %s (%s) %d bytes of data. count(%d)\n", taddr, taddr, size, count);

	/* get address information */
	if (getaddrinfo(taddr, NULL, &hints, &result) != 0) {
		printf("ping_process: fail to get addrinfo\n");
		return ERROR;
	}

	/* try to find valid socket with address information */
	for (rp = result; rp != NULL; rp = rp->ai_next) {
		if ((s = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol)) < 0) {
			/* continue if open is failed */
			continue;
		} else {
			/* success */
			break;
		}
	}
	if (rp == NULL) {
		/* opening socket is totally failed */
		printf("ping_process: fail to create raw socket\n");
		goto err_out;
	}

	/* copy the socket pointer we found */
	to = rp->ai_addr;

	tv.tv_sec = 1;
	tv.tv_usec = 0;

	if (setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (struct timeval *)&tv, sizeof(struct timeval)) != ERR_OK) {
		printf("ping_process: setsockopt error\n");
		goto err_out;
	}

	while (1) {
		if (ping_send(s, to, size) == ERR_OK) {
			// printf("ping : send %d\n", ping_send_counter);
      clock_gettime(CLOCK_REALTIME, &ping_time);
			ping_recv((int)to->sa_family, s, &ping_time);
		} else {
			printf("ping_process: sendto error(%d)\n", errno);
			break;
		}

		ping_send_counter++;
		usleep(1000 * PING_DELAY);
		if (ping_send_counter == count) {
			break;
		}
	}

	lwip_freeaddrinfo(result);
	close(s);

	printf("--- %s ping statistics ---\n", taddr);
	printf("%d packets transmitted, %d received, %f%% packet loss\n", ping_send_counter,
		g_ping_recv_counter,
		(100.0f * (float)(ping_send_counter - g_ping_recv_counter) / (float)ping_send_counter));

	return OK;

err_out:
	lwip_freeaddrinfo(result);
	if (s >= 0) {
		close(s);
		s = -1;
	}

	return ERROR;
}

int cmd_ping(int argc, char **argv)
{
	char *staddr;
	int size = 0;
	int count = CONFIG_NET_PING_CMD_ICOUNT;

	/* Get the ping options */

	int ret = ping_options(argc, argv, &count, &size, &staddr);
	if (ret < 0) {
		return ERROR;
	}
	ret = ping_process(count, staddr, size);
	if (ret < 0) {
		return ERROR;
	}

	return OK;
}
