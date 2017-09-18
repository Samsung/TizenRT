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
struct stats_ *proto;

/**
* @statitcase            : tc_net_stats_display
* @brief                 : display the current stats of lwip protocol
* @scenario              : Its display stats
* @apicovered            : stats_display
* @precondition          : none
* @postcondition         : none
*/
static void tc_net_stats_display(void)
{
	stats_display();
	TC_SUCCESS_RESULT();
}

/**
* @statitcase            : tc_net_stats_display_proto_n
* @brief                 : display all member of proto
* @scenario              : no return value
* @apicovered            : stats_display_proto
* @precondition          : none
* @postcondition         : none
*/
static void tc_net_stats_display_proto_n(void)
{
	int ret = NEG_VAL;

	if (proto->tcp.xmit) {
		stats_display_proto(&proto->tcp, "TCP");
		ret = 0;
	}
	TC_ASSERT_NEQ("stats_display_proto", ret, 0)
	free(proto);
	TC_SUCCESS_RESULT()
}

/**
* @testcase              : tc_net_stats_sys_n
* @brief                 : display  the sys stats
* @scenario              : show all stats of sys member
* @apicovered            : stats_display_sys
* @precondition          : none
* @postcondition         : none
*/
static void tc_net_stats_sys_n(void)
{
	stats_display_sys(NULL);
	TC_SUCCESS_RESULT();
}

/**
* @testcase              : tc_net_stats_sys_p
* @brief                 : display the sys stats
* @scenario              : show all stats 
* @apicovered            : stats_display_sys
* @precondition          : none
* @postcondition         : none
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
	free(sys);
	TC_SUCCESS_RESULT();
	sys = NULL;
}

/**
* @testcase             : tc_net_stats_display_igmp_n
* @brief                : display all states of igmp members
* @scenario             : Diplay stats
* @apicovered           : stats_display_igmp
* @precondition         : none
* @postcondition        : none
*/
static void tc_net_stats_display_igmp_n(void)
{
	stats_display_igmp(NULL);
	TC_SUCCESS_RESULT();

}

/**
* @testcase             : tc_net_stats_display_igmp_p
* @brief                : display all stats of igmp memebes
* @scenario             : Display stats
* @apicovered           : stats_display_igmp
* @precondition         : none
* @postcondition        : none
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
	igmp->memerr = 0;
	igmp->proterr = 0;
	igmp->rx_v1 = 1;
	igmp->rx_group = 0;
	igmp->rx_general = 0;
	igmp->rx_report = 0;
	igmp->tx_join = 1;
	igmp->tx_leave = 1;
	igmp->tx_report = 1;
	stats_display_igmp(igmp);
	free(igmp);
	TC_SUCCESS_RESULT();
	igmp = NULL;
}

/**
* @statitcase           : tc_net_stats_display_proto_p
* @brief                : Display the counter of proto members
* @scenario             : Display stats of proto members
* @apicovered           : stats_display_proto
* @precondition         : none
* @postcondition        : none
*/
static void tc_net_stats_display_proto_p(void)
{
	proto->tcp.xmit = 10;
	proto->tcp.recv = 10;
	proto->tcp.fw = 10;
	proto->tcp.drop = 5;
	proto->tcp.chkerr = 0;
	proto->tcp.lenerr = 0;
	proto->tcp.memerr = 0;
	proto->tcp.rterr = 0;
	proto->tcp.proterr = 0;
	proto->tcp.opterr = 0;
	proto->tcp.err = 0;
	proto->tcp.cachehit = 3;

	stats_display_proto((struct stats_proto *)proto, "data");
	free(proto);
	TC_SUCCESS_RESULT();
}

/**
* @statitcase           : net_core_main
* @brief                : none
* @scenario             : none
* @apicovered           : none
* @precondition         : none
* @postcondition        : none
*/
int net_stats_main(void)
{
	//tc_net_stats_display();
//  tc_net_stats_display_proto_n();
//  tc_net_stats_display_proto_p();
//  tc_net_stats_display_igmp_n();
//  tc_net_stats_display_igmp_p();
//  tc_net_stats_sys_p();
//  tc_net_stats_sys_n();
	return 0;
}
