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

/**
 * @file
 * Statistics module
 *
 */

#include <net/lwip/opt.h>

#if LWIP_STATS					/* don't build if not configured for use in lwipopts.h */

#include <net/lwip/def.h>
#include <net/lwip/stats.h>
#include <net/lwip/mem.h>

#include <string.h>
#include <stdio.h>

struct stats_ lwip_stats;

void stats_init(void)
{
#ifdef LWIP_DEBUG
#if MEMP_STATS
	const char *memp_names[] = {
#define LWIP_MEMPOOL(name, num, size, desc) desc,
#include <net/lwip/memp_std.h>
	};
	int i;
	for (i = 0; i < MEMP_MAX; i++) {
		lwip_stats.memp[i].name = memp_names[i];
	}
#endif							/* MEMP_STATS */
#if MEM_STATS
	lwip_stats.mem.name = "MEM";
#endif							/* MEM_STATS */
#endif							/* LWIP_DEBUG */
}

#if LWIP_STATS_DISPLAY
void stats_display_proto(struct stats_proto *proto, const char *name)
{
	LWIP_STATS_DIAG(("\n%s\n\t", name));
	LWIP_STATS_DIAG(("xmit: %" STAT_COUNTER_F "\n\t", proto->xmit));
	LWIP_STATS_DIAG(("recv: %" STAT_COUNTER_F "\n\t", proto->recv));
	LWIP_STATS_DIAG(("fw: %" STAT_COUNTER_F "\n\t", proto->fw));
	LWIP_STATS_DIAG(("drop: %" STAT_COUNTER_F "\n\t", proto->drop));
	LWIP_STATS_DIAG(("chkerr: %" STAT_COUNTER_F "\n\t", proto->chkerr));
	LWIP_STATS_DIAG(("lenerr: %" STAT_COUNTER_F "\n\t", proto->lenerr));
	LWIP_STATS_DIAG(("memerr: %" STAT_COUNTER_F "\n\t", proto->memerr));
	LWIP_STATS_DIAG(("rterr: %" STAT_COUNTER_F "\n\t", proto->rterr));
	LWIP_STATS_DIAG(("proterr: %" STAT_COUNTER_F "\n\t", proto->proterr));
	LWIP_STATS_DIAG(("opterr: %" STAT_COUNTER_F "\n\t", proto->opterr));
	LWIP_STATS_DIAG(("err: %" STAT_COUNTER_F "\n\t", proto->err));
	LWIP_STATS_DIAG(("cachehit: %" STAT_COUNTER_F "\n", proto->cachehit));
}

#if IGMP_STATS
void stats_display_igmp(struct stats_igmp *igmp)
{
	LWIP_STATS_DIAG(("\nIGMP\n\t"));
	LWIP_STATS_DIAG(("xmit: %" STAT_COUNTER_F "\n\t", igmp->xmit));
	LWIP_STATS_DIAG(("recv: %" STAT_COUNTER_F "\n\t", igmp->recv));
	LWIP_STATS_DIAG(("drop: %" STAT_COUNTER_F "\n\t", igmp->drop));
	LWIP_STATS_DIAG(("chkerr: %" STAT_COUNTER_F "\n\t", igmp->chkerr));
	LWIP_STATS_DIAG(("lenerr: %" STAT_COUNTER_F "\n\t", igmp->lenerr));
	LWIP_STATS_DIAG(("memerr: %" STAT_COUNTER_F "\n\t", igmp->memerr));
	LWIP_STATS_DIAG(("proterr: %" STAT_COUNTER_F "\n\t", igmp->proterr));
	LWIP_STATS_DIAG(("rx_v1: %" STAT_COUNTER_F "\n\t", igmp->rx_v1));
	LWIP_STATS_DIAG(("rx_group: %" STAT_COUNTER_F "\n", igmp->rx_group));
	LWIP_STATS_DIAG(("rx_general: %" STAT_COUNTER_F "\n", igmp->rx_general));
	LWIP_STATS_DIAG(("rx_report: %" STAT_COUNTER_F "\n\t", igmp->rx_report));
	LWIP_STATS_DIAG(("tx_join: %" STAT_COUNTER_F "\n\t", igmp->tx_join));
	LWIP_STATS_DIAG(("tx_leave: %" STAT_COUNTER_F "\n\t", igmp->tx_leave));
	LWIP_STATS_DIAG(("tx_report: %" STAT_COUNTER_F "\n\t", igmp->tx_report));
}
#endif							/* IGMP_STATS */

#if MEM_STATS || MEMP_STATS
void stats_display_mem(struct stats_mem *mem, const char *name)
{
	LWIP_STATS_DIAG(("\nMEM %s\n\t", name));
	LWIP_STATS_DIAG(("avail: %" U32_F "\n\t", (u32_t) mem->avail));
	LWIP_STATS_DIAG(("used: %" U32_F "\n\t", (u32_t) mem->used));
	LWIP_STATS_DIAG(("max: %" U32_F "\n\t", (u32_t) mem->max));
	LWIP_STATS_DIAG(("err: %" U32_F "\n", (u32_t) mem->err));
}

#if MEMP_STATS
void stats_display_memp(struct stats_mem *mem, int index)
{
	char *memp_names[] = {
#define LWIP_MEMPOOL(name, num, size, desc) desc,
#include <net/lwip/memp_std.h>
	};
	if (index < MEMP_MAX) {
		stats_display_mem(mem, memp_names[index]);
	}
}
#endif							/* MEMP_STATS */
#endif							/* MEM_STATS || MEMP_STATS */

#if SYS_STATS
void stats_display_sys(struct stats_sys *sys)
{
	LWIP_STATS_DIAG(("\nSYS\n\t"));
	LWIP_STATS_DIAG(("sem.used:  %" U32_F "\n\t", (u32_t) sys->sem.used));
	LWIP_STATS_DIAG(("sem.max:   %" U32_F "\n\t", (u32_t) sys->sem.max));
	LWIP_STATS_DIAG(("sem.err:   %" U32_F "\n\t", (u32_t) sys->sem.err));
	LWIP_STATS_DIAG(("mutex.used: %" U32_F "\n\t", (u32_t) sys->mutex.used));
	LWIP_STATS_DIAG(("mutex.max:  %" U32_F "\n\t", (u32_t) sys->mutex.max));
	LWIP_STATS_DIAG(("mutex.err:  %" U32_F "\n\t", (u32_t) sys->mutex.err));
	LWIP_STATS_DIAG(("mbox.used:  %" U32_F "\n\t", (u32_t) sys->mbox.used));
	LWIP_STATS_DIAG(("mbox.max:   %" U32_F "\n\t", (u32_t) sys->mbox.max));
	LWIP_STATS_DIAG(("mbox.err:   %" U32_F "\n\t", (u32_t) sys->mbox.err));
}
#endif							/* SYS_STATS */

int stats_display(int argc, char *argv[])
{
	s16_t i;
	int all = 0;
	int cmd_opt_matched = 0;
	if (argc < 2) {
		LWIP_STATS_DIAG(("USAGE: lwip_stats [opt]"));
		LWIP_STATS_DIAG(("opt: link / etharp / ipfrag / ip / igmp / icmp / udp / tcp / mem / sys / all"));
		return 0;
	}
	if (!strcmp(argv[1], "all")) {
		all = 1;
	}

	if (!strcmp(argv[1], "link") || all) {
		LINK_STATS_DISPLAY();
		cmd_opt_matched = 1;
	}
	if (!strcmp(argv[1], "etharp") || all) {
		ETHARP_STATS_DISPLAY();
		cmd_opt_matched = 1;
	}
	if (!strcmp(argv[1], "ipfrag") || all) {
		IPFRAG_STATS_DISPLAY();
		cmd_opt_matched = 1;
	}
	if (!strcmp(argv[1], "ip") || all) {
		IP_STATS_DISPLAY();
		cmd_opt_matched = 1;
	}
	if (!strcmp(argv[1], "igmp") || all) {
		IGMP_STATS_DISPLAY();
		cmd_opt_matched = 1;
	}
	if (!strcmp(argv[1], "icmp") || all) {
		ICMP_STATS_DISPLAY();
		cmd_opt_matched = 1;
	}
	if (!strcmp(argv[1], "udp") || all) {
		UDP_STATS_DISPLAY();
		cmd_opt_matched = 1;
	}
	if (!strcmp(argv[1], "tcp") || all) {
		TCP_STATS_DISPLAY();
		cmd_opt_matched = 1;
	}
	if (!strcmp(argv[1], "mem") || all) {
		MEM_STATS_DISPLAY();
		cmd_opt_matched = 1;
	}
	if (!strcmp(argv[1], "memp") || all) {
		for (i = 0; i < MEMP_MAX; i++) {
			MEMP_STATS_DISPLAY(i);
		}
		cmd_opt_matched = 1;
	}
	if (!strcmp(argv[1], "sys") || all) {
		SYS_STATS_DISPLAY();
		cmd_opt_matched = 1;
	}
	if (!cmd_opt_matched) {
		LWIP_STATS_DIAG(("ERROR:lwip_stats - Invalid Option"));
		LWIP_STATS_DIAG(("USAGE: lwip_stats [opt]"));
		LWIP_STATS_DIAG(("opt: link / etharp / ipfrag / ip / igmp / icmp / udp / tcp / mem / sys / all"));
		return 0;
	}
	return 0;
}
#endif							/* LWIP_STATS_DISPLAY */

#endif							/* LWIP_STATS */
