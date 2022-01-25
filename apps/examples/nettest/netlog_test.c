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

static const char wifimanager_err_log[] = "wifi_manager error\n";
static const char wifimanager_info_log[] = "wifi_manager info\n";
static const char wifimanager_verb_log[] = "wifi_manager verb\n";

static const char netlib_err_log[] = "netlib error\n";
static const char netlib_info_log[] = "netlib info\n";
static const char netlib_verb_log[] = "netlib verb\n";

START_TEST_F(info_log)
{
	ST_EXPECT_EQ(0, netlog_reset());
	ST_EXPECT_EQ(0, netlog_set_level(NL_MOD_WIFI_MANAGER, NL_LEVEL_INFO));
	ST_EXPECT_EQ(31, NET_LOGE(NL_MOD_WIFI_MANAGER, wifimanager_err_log));
	ST_EXPECT_EQ(24, NET_LOGI(NL_MOD_WIFI_MANAGER, wifimanager_info_log));
	// this should be 0 because log level is error;
	ST_EXPECT_EQ(0, NET_LOGV(NL_MOD_WIFI_MANAGER, wifimanager_verb_log));
}
END_TEST_F

START_TEST_F(verb_log)
{
	ST_EXPECT_EQ(0, netlog_reset());
	ST_EXPECT_EQ(0, netlog_set_level(NL_MOD_WIFI_MANAGER, NL_LEVEL_VERB));
	ST_EXPECT_EQ(31, NET_LOGE(NL_MOD_WIFI_MANAGER, wifimanager_err_log));
	// this should be 0 because log level is error;
	ST_EXPECT_EQ(24, NET_LOGI(NL_MOD_WIFI_MANAGER, wifimanager_info_log));
	// this should be 0 because log level is error;
	ST_EXPECT_EQ(24, NET_LOGV(NL_MOD_WIFI_MANAGER, wifimanager_verb_log));
}
END_TEST_F

START_TEST_F(error_log)
{
	ST_EXPECT_EQ(0, netlog_reset());
	ST_EXPECT_EQ(0, netlog_set_level(NL_MOD_WIFI_MANAGER, NL_LEVEL_ERROR));
	ST_EXPECT_EQ(31, NET_LOGE(NL_MOD_WIFI_MANAGER, wifimanager_err_log));
	ST_EXPECT_EQ(0, NET_LOGI(NL_MOD_WIFI_MANAGER, wifimanager_info_log));
	ST_EXPECT_EQ(0, NET_LOGV(NL_MOD_WIFI_MANAGER, wifimanager_verb_log));
}
END_TEST_F

START_TEST_F(mode_log)
{
	ST_EXPECT_EQ(0, netlog_reset());
	ST_EXPECT_EQ(0, netlog_set_level(NL_MOD_NETLIB, NL_LEVEL_ERROR));
	ST_EXPECT_EQ(0, NET_LOGE(NL_MOD_WIFI_MANAGER, wifimanager_err_log));
	// this should be 0 because log level is error;
	ST_EXPECT_EQ(0, NET_LOGI(NL_MOD_WIFI_MANAGER, wifimanager_info_log));
	// this should be 0 because log level is error;
	ST_EXPECT_EQ(0, NET_LOGV(NL_MOD_WIFI_MANAGER, wifimanager_verb_log));
	ST_EXPECT_EQ(25, NET_LOGE(NL_MOD_NETLIB, "%s", netlib_err_log));
}
END_TEST_F

/*
 * description: test color log
 */
START_TEST_F(color_log)
{
	ST_EXPECT_EQ(0, netlog_reset());
	ST_EXPECT_EQ(0, netlog_set_color(NL_MOD_WIFI_MANAGER, NL_COLOR_GREEN));
	ST_EXPECT_EQ(0, netlog_set_level(NL_MOD_WIFI_MANAGER, NL_LEVEL_VERB));
	ST_EXPECT_EQ(28, NET_LOGV(NL_MOD_WIFI_MANAGER, wifimanager_verb_log));
	ST_EXPECT_EQ(0, netlog_set_color(NL_MOD_NETLIB, NL_COLOR_YELLOW));
	ST_EXPECT_EQ(0, netlog_set_level(NL_MOD_NETLIB, NL_LEVEL_VERB));
	ST_EXPECT_EQ(22, NET_LOGV(NL_MOD_NETLIB, netlib_verb_log));
}
END_TEST_F

/*
 * description: print timer
 */
START_TEST_F(timer_log)
{
	ST_EXPECT_EQ(0, netlog_reset());
	ST_EXPECT_EQ(0, netlog_set_color(NL_MOD_WIFI_MANAGER, NL_COLOR_GREEN));
	ST_EXPECT_EQ(0, netlog_set_level(NL_MOD_WIFI_MANAGER, NL_LEVEL_ERROR));
	ST_EXPECT_EQ(0, netlog_set_timer(NL_OPT_ENABLE));
	ST_EXPECT_EQ(45, NET_LOGE(NL_MOD_WIFI_MANAGER, wifimanager_err_log));
}
END_TEST_F

/*
 * description: add options
 */
START_TEST_F(add_option)
{
	ST_EXPECT_EQ(0, netlog_reset());
	ST_EXPECT_EQ(0, netlog_set_level(NL_MOD_WIFI_MANAGER, NL_LEVEL_VERB));
	ST_EXPECT_EQ(0, netlog_set_function(NL_OPT_ENABLE));
	ST_EXPECT_EQ(39, NET_LOGV(NL_MOD_WIFI_MANAGER, wifimanager_verb_log));
	ST_EXPECT_EQ(0, netlog_set_file(NL_OPT_ENABLE));
	ST_EXPECT_EQ(105, NET_LOGV(NL_MOD_WIFI_MANAGER, wifimanager_verb_log));
}
END_TEST_F

/*
 * description: remove options
 */
START_TEST_F(remove_option)
{
	ST_EXPECT_EQ(0, netlog_reset());
	ST_EXPECT_EQ(0, netlog_set_level(NL_MOD_WIFI_MANAGER, NL_LEVEL_VERB));
	ST_EXPECT_EQ(0, netlog_set_function(NL_OPT_ENABLE));
	ST_EXPECT_EQ(42, NET_LOGV(NL_MOD_WIFI_MANAGER, wifimanager_verb_log));
	ST_EXPECT_EQ(0, netlog_set_file(NL_OPT_ENABLE));
	ST_EXPECT_EQ(108, NET_LOGV(NL_MOD_WIFI_MANAGER, wifimanager_verb_log));
	ST_EXPECT_EQ(0, netlog_set_function(NL_OPT_DISABLE));
	ST_EXPECT_EQ(90, NET_LOGV(NL_MOD_WIFI_MANAGER, wifimanager_verb_log));
	ST_EXPECT_EQ(0, netlog_set_file(NL_OPT_DISABLE));
	ST_EXPECT_EQ(24, NET_LOGV(NL_MOD_WIFI_MANAGER, wifimanager_verb_log));
}
END_TEST_F

void netlog_run_test(void)
{
	ST_SET_PACK(netlog);
	ST_SET_SMOKE1(netlog, NT_TEST_TRIAL, ST_NO_TIMELIMIT, "turn on info", info_log);
	ST_SET_SMOKE1(netlog, NT_TEST_TRIAL, ST_NO_TIMELIMIT, "turn on debug", verb_log);
	ST_SET_SMOKE1(netlog, NT_TEST_TRIAL, ST_NO_TIMELIMIT, "turn on error", error_log);
	ST_SET_SMOKE1(netlog, NT_TEST_TRIAL, ST_NO_TIMELIMIT, "log per mode", mode_log);
	ST_SET_SMOKE1(netlog, NT_TEST_TRIAL, ST_NO_TIMELIMIT, "color log", color_log);
	ST_SET_SMOKE1(netlog, NT_TEST_TRIAL, ST_NO_TIMELIMIT, "timer log", timer_log);
	ST_SET_SMOKE1(netlog, NT_TEST_TRIAL, ST_NO_TIMELIMIT, "set additional info", add_option);
	ST_SET_SMOKE1(netlog, NT_TEST_TRIAL, ST_NO_TIMELIMIT, "remove additional info", remove_option);
	ST_RUN_TEST(netlog);
}
