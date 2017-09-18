/****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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

/// @file tc_net_core.c
/// @brief Test Case Example for api() API
#include <net/lwip/pbuf.h>
#include <net/lwip/stats.h>

#include "tc_internal.h"

/**
* @statitcase			: tc_net_stats_display
* @brief				:
* @scenario				:
* @apicovered			: stats_display
* @precondition			:
* @postcondition		:
*/
static void tc_net_stats_display(void)
{
	stats_display();
	TC_SUCCESS_RESULT();
}

/**
* @statitcase			: tc_net_stats_display_proto_n
* @brief				:
* @scenario				:
* @apicovered			: stats_display_proto
* @precondition			:
* @postcondition		:
*/
static void tc_net_stats_display_proto_n(void)
{
	struct stats_proto *proto;

	proto = (struct stats_proto *)malloc(sizeof(struct stats_proto));

	proto->xmit = proto->recv = proto->fw = proto->drop = proto->chkerr = 0;
	proto->lenerr = proto->memerr = proto->rterr = proto->proterr = proto->opterr = proto->err = proto->cachehit = 0;

	stats_display_proto(proto, NULL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase				: tc_net_stats_sys_n
* @brief				:
* @scenario				:
* @apicovered			: stats_display_sys
* @precondition			:
* @postcondition		:
*/
static void tc_net_stats_sys_n(void)
{
	stats_display_sys(NULL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase				: tc_net_stats_sys_p
* @brief				:
* @scenario				:
* @apicovered			: stats_display_sys
* @precondition			:
* @postcondition		:
*/
static void tc_net_stats_sys_p(void)
{
	struct stats_sys *sys = NULL;

	sys = (struct stats_sys *)malloc(sizeof(struct stats_sys));
	sys->sem.used = 1;
	sys->sem.max = 1;
	sys->sem.err = 1;
	sys->mutex.used = 1;
	sys->mutex.max = 1;
	sys->mutex.err = 1;
	sys->mbox.used = 1;
	sys->mbox.max = 1;
	sys->mbox.err = 1;

	stats_display_sys(sys);
	TC_SUCCESS_RESULT();
	free(sys);
	sys = NULL;
}

/**
* @testcase				: tc_net_stats_display_igmp_n
* @brief				:
* @scenario				:
* @apicovered			: stats_display_igmp
* @precondition			:
* @postcondition		:
*/
static void tc_net_stats_display_igmp_n(void)
{
	stats_display_igmp(NULL);
	TC_SUCCESS_RESULT();

}

/**
* @testcase				: tc_net_stats_display_igmp_p
* @brief				:
* @scenario				:
* @apicovered			: stats_display_igmp
* @precondition			:
* @postcondition		:
*/
static void tc_net_stats_display_igmp_p(void)
{
	struct stats_igmp *igmp = NULL;

	igmp = (struct stats_igmp *)malloc(sizeof(struct stats_proto));

	igmp->xmit = 10;
	igmp->recv = 10;
	igmp->drop = 10;
	igmp->chkerr = 0;
	igmp->lenerr = 0;
	igmp->memerr= 0;
	igmp->proterr = 0;
	igmp->rx_v1 = 1;
	igmp->rx_group = 0;
	igmp->rx_general = 0;
	igmp->rx_report = 0;
	igmp->tx_join = 1;
	igmp->tx_leave = 1;
	igmp->tx_report = 1;
	stats_display_igmp(igmp);
	TC_SUCCESS_RESULT();
	free(igmp);
	igmp = NULL;
}

/**
* @statitcase			: tc_net_stats_display_proto_p
* @brief				:
* @scenario				:
* @apicovered			: stats_display_proto
* @precondition			:
* @postcondition		:
*/
static void tc_net_stats_display_proto_p(void)
{
	struct stats_proto *proto;

	proto = (struct stats_proto *)malloc(sizeof(struct stats_proto));

	proto->xmit = 10;
	proto->recv = 10;
	proto->fw = 10;
	proto->drop = 5;
	proto->chkerr = 0;
	proto->lenerr = 0;
	proto->memerr = 0;
	proto->rterr = 0;
	proto->proterr = 0;
	proto->opterr = 0;
	proto->err = 0;
	proto->cachehit = 3;

	stats_display_proto(proto, "data");
	TC_SUCCESS_RESULT();
}

/**
* @statitcase			: net_core_main
* @brief				:
* @scenario				:
* @apicovered			:
* @precondition			:
* @postcondition		:
*/
int net_stats_main(void)
{
	tc_net_stats_display();
	tc_net_stats_display_proto_n();
	tc_net_stats_display_proto_p();
	tc_net_stats_display_igmp_n();
	tc_net_stats_display_igmp_p();
	tc_net_stats_sys_p();
	tc_net_stats_sys_n();
	return 0;
}
