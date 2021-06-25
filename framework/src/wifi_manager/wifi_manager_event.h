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

#ifndef _WIFI_MANAGER_EVENT_H__
#define _WIFI_MANAGER_EVENT_H__

#define WIFIMGR_EVENT_INITIALIZER {NULL, {NULL, NULL, NULL, NULL, NULL} }

enum _wifimgr_evt {
#define WIFIMGR_EVT_TABLE(str, type, desc) type,
	#include "wifi_manager_event_table.h"
	EVT_NONE,
};
typedef enum _wifimgr_evt wifimgr_evt_e;

struct wifimgr_evt_handler {
	dhcp_sta_joined_cb dhcps_sta_joined;
	wifi_utils_cb_s wifi_evt;
};
typedef struct wifimgr_evt_handler wifimgr_evt_handler_s;

int wifimgr_get_evthandler(wifimgr_evt_handler_s *evt);
char *wifimgr_get_evt_str(wifimgr_evt_e evt);

#endif // _WIFI_MANAGER_EVENT_H__
