/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
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

#include <tinyara/config.h>

#include <stdint.h>
#include <wifi_manager/wifi_manager.h>
#include "wifi_manager_cb.h"

static uint16_t g_wifimgr_stats[CB_MAX];

void wifimgr_inc_stats(int cb)
{
	g_wifimgr_stats[cb]++;
}

void wifimgr_get_stats(wifi_manager_stats_s *stats)
{
	//  is checked in caller function
	stats->connect = g_wifimgr_stats[CB_STA_CONNECTED];
	stats->connectfail = g_wifimgr_stats[CB_STA_CONNECT_FAILED];
	stats->disconnect = g_wifimgr_stats[CB_STA_DISCONNECTED];
	stats->reconnect = g_wifimgr_stats[CB_STA_RECONNECTED];
	stats->joined = g_wifimgr_stats[CB_STA_JOINED];
	stats->left = g_wifimgr_stats[CB_STA_LEFT];
	stats->scan = g_wifimgr_stats[CB_SCAN_DONE];

	/* SoftAP mode has no callback */
	stats->softap = g_wifimgr_stats[CB_SOFTAP_DONE];
}
