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
#include <tinyara/config.h>
#include <wifi_manager/wifi_manager.h>
#include <tinyara/net/if/wifi.h>
#include "wifi_manager_utils.h"

wifi_manager_result_e wifimgr_convert_trwifi(trwifi_result_e tres)
{
	wifi_manager_result_e res = WIFI_MANAGER_FAIL;
	switch (tres) {
	case TRWIFI_SUCCESS:
		res = WIFI_MANAGER_SUCCESS;
		break;
	case TRWIFI_INVALID_ARGS:
		res = WIFI_MANAGER_INVALID_ARGS;
		break;
	case TRWIFI_TIMEOUT:
		res = WIFI_MANAGER_TIMEOUT;
		break;
	case TRWIFI_BUSY:
		res = WIFI_MANAGER_BUSY;
		break;
	case TRWIFI_ALREADY_CONNECTED:
		res = WIFI_MANAGER_ALREADY_CONNECTED;
		break;
	case TRWIFI_NOT_SUPPORTED:
		res = WIFI_MANAGER_NOT_AVAILABLE;
		break;
	case TRWIFI_FILE_ERROR:
	case TRWIFI_FAIL:
	default:
		res = WIFI_MANAGER_FAIL;
		break;
	}
	return res;
}
