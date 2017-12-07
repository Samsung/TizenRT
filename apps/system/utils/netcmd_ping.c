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
#include <tinyara/config.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <net/lwip/ipv4/icmp.h>
#include <net/lwip/ipv4/ip.h>
#include "netcmd.h"
#include "netcmd_ping.h"

#undef htons
#define htons HTONS
#define PING_ID        0xAFAF
#define PING_DELAY     1000
#define PING_MAX_TRY_COUNTER	10
#define PING_DATA_SIZE			32
#define PING_RESULT(ping_ok)

static int g_ping_recv_counter;
static uint16_t g_ping_seq_num;
static uint16_t g_ping_data_size;

static void dump_usage(void)
{
	printf("Usage :\n");
	printf("\tping <IP address>\n");
	printf("\tping <IP address> <count>\n");
	printf("\tping <IP address> <count> <size>\n");
	printf("Where :\n");
	printf("\tIP address : destination's IP address\n");
	printf("\tcount : number of ping packets to send (default 10)\n");
	printf("\tsize : ping packet data size (default 32 Bytes)\n");
	printf("\n");
}

static int
nu_ping_options(int argc, char **argv, int *count, uint32_t *dsec, char **staddr)
{
	FAR const char *fmt = fmtarginvalid;
	bool badarg = false;
	int option;
	int tmp;

	/* Get the ping options */

	optind = -1;
	while ((option = getopt(argc, argv, ":c:i:")) != ERROR) {
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

		case 'i':
			tmp = atoi(optarg);
			if (tmp < 1 || tmp >= 4294) {
				printf(fmtargrange, argv[0]);
				badarg = true;
			} else {
				*dsec = 10 * tmp;
			}
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
		fmt = fmttoomanyargs;
		goto errout;
	} else {
		fmt = fmtargrequired;
		goto errout;
	}

	return OK;

errout:
	printf(fmt, argv[0]);
	return ERROR;
}

static u16_t
nu_standard_chksum(void *dataptr, u16_t len)
{
	u32_t acc;
	u16_t src;
	u8_t *octetptr;

	acc = 0;
	/* dataptr may be at odd or even addresses */
	octetptr = (u8_t *)dataptr;
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
	return htons((u16_t)acc);
}


static void
nu_ping_recv(int s, struct timespec *ping_time)
{
	char *buf;
	int fromlen;
	int len;
	struct sockaddr_in from;
	struct ip_hdr *iphdr;
	struct icmp_echo_hdr *iecho;
	int ping_size = sizeof(struct ip_hdr) + sizeof(struct icmp_echo_hdr) + g_ping_data_size;

	fromlen = sizeof(struct sockaddr_in);

	while (1) {
		buf = (char *)malloc(sizeof(char)*ping_size);
		if (!buf) {
			printf("failed to allocate memory\n");
			return;
		}

		len = recvfrom(s, buf, ping_size, 0, (struct sockaddr *)&from, (socklen_t *)&fromlen);
		if (len <= 0) break;

		if (len >= (int)(sizeof(struct ip_hdr) + sizeof(struct icmp_echo_hdr))) {

			iphdr = (struct ip_hdr *)buf;
			iecho = (struct icmp_echo_hdr *)(buf + (IPH_HL(iphdr) * 4));

			if (iecho->type == ICMP_ER) {
				struct timespec now;
				clock_gettime(CLOCK_REALTIME, &now);
				g_ping_recv_counter++;
				uint32_t elapsed = (now.tv_sec - ping_time->tv_sec) * 1000 + (now.tv_nsec - ping_time->tv_nsec) / 1000000;
				printf(" %d bytes from %s: icmp_seq=%d ttl=255 time=%" U32_F " ms\n",
					   len, inet_ntoa(from.sin_addr), g_ping_seq_num, elapsed);

				if ((iecho->id == PING_ID) && (iecho->seqno == htons(g_ping_seq_num))) {
					/* do some ping result processing */
					PING_RESULT((ICMPH_TYPE(iecho) == ICMP_ER));
					free(buf);
					return;
				} else printf("drop\n");
			}
		}
		free(buf);
	}
	if (len == 0) {
		printf("ping: recv - timeout\n");
	}
}


static void
nu_ping_prepare_echo(struct icmp_echo_hdr *iecho, u16_t len)
{
	size_t i;
	size_t data_len = len - sizeof(struct icmp_echo_hdr);

	ICMPH_TYPE_SET(iecho, ICMP_ECHO);
	ICMPH_CODE_SET(iecho, 0);
	iecho->chksum = 0;
	iecho->id     = PING_ID;
	++g_ping_seq_num;
	iecho->seqno  = htons(g_ping_seq_num);

	/* fill the additional data buffer with some data */
	for (i = 0; i < data_len; i++) {
		((char *)iecho)[sizeof(struct icmp_echo_hdr) + i] = (char)i;
	}

	iecho->chksum = ~nu_standard_chksum(iecho, len);
}


static int
nu_ping_send(int s, struct sockaddr_in *to)
{
	int err;
    int ret = 0;
	struct icmp_echo_hdr *iecho;
	size_t ping_size = sizeof(struct icmp_echo_hdr) + g_ping_data_size;

	LWIP_ASSERT("ping_size is too big", ping_size <= 0xffff);

	iecho = (struct icmp_echo_hdr *)malloc(ping_size);
	if (!iecho) {
		printf("fail to alloc mem\n");
		return -1;
	}

	nu_ping_prepare_echo(iecho, (u16_t)ping_size);

	err = sendto(s, iecho, ping_size, 0, (struct sockaddr *)to, sizeof(struct sockaddr_in));
	if (err < 0) {
		printf("sendto is failed\n");
		ret = -1;
	}
	free(iecho);

	return ret;
}

int
nu_ping_process(int count, const char *taddr)
{
	int s;
	struct timeval tv;
	struct sockaddr_in to;
	struct timespec  ping_time;
	int32_t ping_send_counter = 0;
	g_ping_seq_num = 0;
	g_ping_recv_counter = 0;

	printf("PING %s (%s) 60(88) bytes of data. count(%d)\n", taddr, taddr, count);
	if ((s = socket(AF_INET, SOCK_RAW, IP_PROTO_ICMP)) < 0) {
		printf("fail to create raw socket...\n");
		return -1;
	}

	tv.tv_sec = 1;
	tv.tv_usec = 0;

	setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (struct timeval *)&tv,
			   sizeof(struct timeval));

	to.sin_len = sizeof(to);
	to.sin_family = AF_INET;
	to.sin_addr.s_addr = inet_addr(taddr);

	while (1) {
		if (nu_ping_send(s, &to) == ERR_OK) {
			// printf("ping : send %d\n", ping_send_counter);
			clock_gettime(CLOCK_REALTIME, &ping_time);
			nu_ping_recv(s, &ping_time);
		} else {
			printf("ping: send error\n");
			break;
		}

		ping_send_counter++;
		usleep(1000 * PING_DELAY);
		if (ping_send_counter == count) break;
	}

	close(s);

	printf("--- %s ping statistics ---\n", taddr);
	printf("%d packets transmitted, %d received, %f\%% packet loss,\n", ping_send_counter,
		   g_ping_recv_counter,
		   (100.0f * (float)(g_ping_recv_counter - ping_send_counter))/(float)g_ping_recv_counter);

	return OK;
}

int cmd_ping(int argc, char **argv)
{
	int count;
	int ret;
	char *staddr;

	if (argc < 2) {
		dump_usage();
		return OK;
	}

	staddr = argv[1];

	g_ping_data_size = PING_DATA_SIZE;
	count = PING_MAX_TRY_COUNTER;

	if (argc == 3) {
		count = atoi(argv[2]);

		if (count < 1) {
			count = PING_MAX_TRY_COUNTER;
			printf("Invaild data : max counter set to default value %d\n", count);
		}
	} else if (argc == 4) {
		count = atoi(argv[2]);
		g_ping_data_size = atoi(argv[3]);

		if (g_ping_data_size < 1) {
			g_ping_data_size = PING_DATA_SIZE;
			printf("Invaild data : data size set to default value %d\n", g_ping_data_size);
		}

		if (count < 1) {
			count = PING_MAX_TRY_COUNTER;
			printf("Invaild data : max counter set to default value %d\n", count);
		}
	}

	ret = nu_ping_process(count, staddr);
	if (ret < 0) {
		return ERROR;
	}

	return OK;
}

