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

#pragma once

/*  State Definition */
enum _wifimgr_state {
#undef WIFIMGR_STATE_TABLE
#define WIFIMGR_STATE_TABLE(state, handler, str) state,
#include "wifi_manager_state_table.h"
	WIFIMGR_NONE, // it is used for prev state only
	WIFIMGR_STATE_MAX,
};
typedef enum _wifimgr_state wifimgr_state_e;

enum _wifimgr_disconn_substate {
	WIFIMGR_DISCONN_NONE = -1,
	WIFIMGR_DISCONN_DEINIT, // turn off wi-fi manager while station connected to AP
	WIFIMGR_DISCONN_SOFTAP, // set softap mode while station connected to AP
	WIFIMGR_DISCONN_INTERNAL_ERROR, // internal error ex) dhcp fails
	WIFIMGR_DISCONN_MAX,
};
typedef enum _wifimgr_disconn_substate _wifimgr_disconn_substate_e;
