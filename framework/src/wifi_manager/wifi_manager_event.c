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
#include <tinyara/net/if/wifi.h>
#include <tinyara/net/netlog.h>
#include "wifi_manager_dhcp.h"
#include "wifi_manager_utils.h"
#include "wifi_manager_event.h"
#include "wifi_manager_msghandler.h"

#define TAG "[WM]"

/*  wifimgr_evt_str should be mapped to enum _wifimgr_evt  */
static char *wifimgr_evt_str[] = {
#undef WIFIMGR_REQUEST_TABLE
#define WIFIMGR_REQUEST_TABLE(str, type, desc) str,
#include "wifi_manager_cmd_table.h"
#undef WIFIMGR_EVT_TABLE
#define WIFIMGR_EVT_TABLE(str, type, desc) str,
#include "wifi_manager_event_table.h"
	"EVT_NONE",
};

char *wifimgr_get_evt_str(wifimgr_evt_e evt)
{
	return wifimgr_evt_str[evt];
}
