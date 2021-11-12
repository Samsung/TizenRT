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

#include <stdio.h>
#include <sys/time.h>
#include <queue.h>
#include <stress_tool/st_perf.h>
#ifdef __LINUX__
#include "netlog.h"
#else
#include <tinyara/net/netlog.h>
#endif

#define NT_TEST_TRIAL 1

static const char wifimanager_err_log[] = "wifi_manager debug";
static const char wifimanager_info_log[] = "wifi_manager info";
static const char wifimanager_verb_log[] = "wifi_manager verb";

static const char netlib_err_log[] = "netlib debug";
static const char netlib_info_log[] = "netlib info";
static const char netlib_verb_log[] = "netlib verb";

START_TEST_F(error_log)
{
	int res = netlog_reset();
	ST_EXPECT_EQ(0, res);
	res = netlog_set_level(NL_LEVEL_ERROR, NL_MOD_WIFI_MANAGER);
	ST_EXPECT_EQ(0, res);
	int len = NET_LOGE(NL_MOD_WIFI_MANAGER, wifimanager_err_log);
	ST_EXPECT_EQ(-1, len);
	len = NET_LOGI(NL_MOD_WIFI_MANAGER, wifimanager_info_log);
	ST_EXPECT_EQ(0, len); // this should be 0 because log level is error;
	len = NET_LOGV(NL_MOD_WIFI_MANAGER, wifimanager_verb_log);
	ST_EXPECT_EQ(0, len); // this should be 0 because log level is error;
}
END_TEST_F

START_TEST_F(info_log)
{
	int res = netlog_reset();
	ST_EXPECT_EQ(0, res);
	res = netlog_set_level(NL_LEVEL_ERROR, NL_MOD_WIFI_MANAGER);
	ST_EXPECT_EQ(0, res);
	int len = NET_LOGE(NL_MOD_WIFI_MANAGER, wifimanager_err_log);
	ST_EXPECT_EQ(-1, len);
	len = NET_LOGI(NL_MOD_WIFI_MANAGER, wifimanager_info_log);
	ST_EXPECT_EQ(-1, len); // this should be 0 because log level is error;
	len = NET_LOGV(NL_MOD_WIFI_MANAGER, wifimanager_verb_log);
	ST_EXPECT_EQ(0, len); // this should be 0 because log level is error;
}
END_TEST_F

START_TEST_F(verb_log)
{
	int res = netlog_reset();
	ST_EXPECT_EQ(0, res);
	res = netlog_set_level(NL_LEVEL_ERROR, NL_MOD_WIFI_MANAGER);
	ST_EXPECT_EQ(0, res);
	int len = NET_LOGE(NL_MOD_WIFI_MANAGER, wifimanager_err_log);
	ST_EXPECT_EQ(-1, len);
	len = NET_LOGI(NL_MOD_WIFI_MANAGER, wifimanager_info_log);
	ST_EXPECT_EQ(-1, len); // this should be 0 because log level is error;
	len = NET_LOGV(NL_MOD_WIFI_MANAGER, wifimanager_verb_log);
	ST_EXPECT_EQ(-1, len); // this should be 0 because log level is error;
}
END_TEST_F

START_TEST_F(mode_log)
{
	int res = netlog_reset();
	ST_EXPECT_EQ(0, res);
	res = netlog_set_level(NL_LEVEL_ERROR, NL_MOD_NETLIB);
	ST_EXPECT_EQ(0, res);
	int len = NET_LOGE(NL_MOD_WIFI_MANAGER, wifimanager_err_log);
	ST_EXPECT_EQ(0, len);
	len = NET_LOGI(NL_MOD_WIFI_MANAGER, wifimanager_info_log);
	ST_EXPECT_EQ(0, len); // this should be 0 because log level is error;
	len = NET_LOGV(NL_MOD_WIFI_MANAGER, wifimanager_verb_log);
	ST_EXPECT_EQ(0, len); // this should be 0 because log level is error;
	len = NET_LOGE(NL_MOD_NETLIB, "%s", netlib_err_log);
	ST_EXPECT_EQ(-1, len);
}
END_TEST_F

void netlog_run_test(void)
{
	ST_SET_PACK(netlog);

	ST_SET_SMOKE1(netlog, NT_TEST_TRIAL, ST_NO_TIMELIMIT, "turn on info", info_log);
	ST_SET_SMOKE1(netlog, NT_TEST_TRIAL, ST_NO_TIMELIMIT, "turn on debug", verb_log);
	ST_SET_SMOKE1(netlog, NT_TEST_TRIAL, ST_NO_TIMELIMIT, "turn on error", error_log);
	ST_SET_SMOKE1(netlog, NT_TEST_TRIAL, ST_NO_TIMELIMIT, "log per mode", mode_log);

	ST_RUN_TEST(netlog);
	ST_RESULT_TEST(netlog);
}
