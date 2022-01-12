/****************************************************************************
 *
 * Copyright 2022 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License\n");
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

#include <tinyara/config.h>
#include <sys/types.h>
#include <stdio.h>
#include <errno.h>
#include <netdb.h>
#include <semaphore.h>
#include <wifi_manager/wifi_manager.h>
#include <stress_tool/st_perf.h>
#include "lwip/lwipopts.h"
#include "wm_test.h"
#include "wm_test_utils.h"
#include "wm_test_network.h"
#include "wm_test_log.h"

#define TAG "[WT]"

static sem_t g_wm_sem;

static char *g_host_list[] = {
	"www.google.com",
	"www.facebook.com",
	"www.naver.com",
	"www.daum.net",
};

TESTCASE_SETUP(simple_tc)
{
	/*  setup DNS server 8.8.8.8 */
	struct sockaddr_in dns_addr;
	dns_addr.sin_family = AF_INET;
	inet_pton(AF_INET, "8.8.8.8", (void *)&dns_addr.sin_addr);
	ST_ASSERT_EQ(0, netlib_setdnsserver((struct sockaddr *)&dns_addr, -1));
} END_TESTCASE

TESTCASE_TEARDOWN(simple_tc)
{
} END_TESTCASE

static void _print_result(char *hostname, struct addrinfo *local_res)
{
	struct addrinfo *ainfo = NULL;
	WT_LOGP(TAG, "Host: %s\n", hostname);
	for (ainfo = local_res; ainfo != NULL; ainfo = ainfo->ai_next) {
		WT_LOGP(TAG, "family %s\n", ((ainfo->ai_family == AF_INET) ? "AF_INET" : "Not IPv4"));
		if (ainfo->ai_family != AF_INET) {
			WT_LOGP(TAG, "not interesting type %d\n", ainfo->ai_family);
			continue;
		}
		struct sockaddr_in *dst = (struct sockaddr_in *)ainfo->ai_addr;
		struct in_addr info_addr = dst->sin_addr;
		uint8_t addr[4] = {0,};
		addr[0] = info_addr.s_addr & 0xFF;
		addr[1] = (info_addr.s_addr >> 8) & 0xFF;
		addr[2] = (info_addr.s_addr >> 16) & 0xFF;
		addr[3] = (info_addr.s_addr >> 24) & 0xFF;
		WT_LOGP(TAG, "%d.%d.%d.%d\n", addr[0], addr[1], addr[2], addr[3]);
	}
	WT_LOGP(TAG, "\n");
}

START_TEST_F(dns_queue)
{
	struct addrinfo hints, *local_res;
	int arr_size = sizeof(g_host_list) / sizeof(char *);
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	for (int i = 0; i < arr_size; i++) {
		int res = getaddrinfo(g_host_list[i], NULL, &hints, &local_res);
		ST_EXPECT_EQ(0, res);
		if (res == 0) {
			_print_result(g_host_list[i], local_res);
			freeaddrinfo(local_res);
		}
	}
#ifdef CONFIG_NET_DNS_MAX_TTL
	// wait until dns entries are refreshed.
	sleep(CONFIG_NET_DNS_MAX_TTL);
#endif
} END_TEST_F

void wm_run_dns_test(void *arg)
{
	struct wt_options *opt = (struct wt_options *)arg;
	WT_LOG(TAG, "init sem");
	if (0 != sem_init(&g_wm_sem, 0, 0)) {
		WT_LOGE(TAG, "sem init fail %d", errno);
		return;
	}

	WT_LOG(TAG, "run dns test\n");
	WT_LOG(TAG, "repeat %d\n", opt->repeat);

	ST_SET_PACK(network_dns);
	ST_TC_SET_GLOBAL(network_dns, simple_tc);
	ST_SET_SMOKE1(network_dns, opt->repeat, 0, "dns test", dns_queue);
	ST_RUN_TEST(network_dns);
	ST_RESULT_TEST(network_dns);

	WT_LOG(TAG, "deinit sem");
	sem_destroy(&g_wm_sem);
}
