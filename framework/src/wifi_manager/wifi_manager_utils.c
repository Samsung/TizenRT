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

#include <wifi_manager/wifi_manager.h>

#define WIFIMGR_FREE_CONNMSG(msg)				\
	do {										\
		free(msg->config);						\
		msg->config = NULL;						\
		free(msg->conn_config);					\
		msg->conn_config = NULL;				\
		close(msg->fd);							\
		free(msg);								\
	} while (0)


void _convert_state_to_info(connect_status_e *conn, wifi_manager_mode_e *mode, wifimgr_state_e state)
{
	switch (state) {
	case WIFIMGR_UNINITIALIZED:
		*mode = WIFI_NONE;
		*conn = STATUS_UNKNOWN;
		break;
	case WIFIMGR_STA_DISCONNECTED:
	case WIFIMGR_STA_DISCONNECTING:
	case WIFIMGR_STA_CONNECTING:
	case WIFIMGR_STA_CONNECT_CANCEL:
		*mode = STA_MODE;
		*conn = AP_DISCONNECTED;
		break;
	case WIFIMGR_STA_CONNECTED:
		*mode = STA_MODE;
		*conn = AP_CONNECTED;
		break;
	case WIFIMGR_SOFTAP:
		*mode = SOFTAP_MODE;
		if (dhcps_get_num() > 0) {
			*conn = CLIENT_CONNECTED;
		} else {
			*conn = CLIENT_DISCONNECTED;
		}
		break;
	case WIFIMGR_STA_RECONNECT:
	case WIFIMGR_STA_RECONNECTING:
		*mode = STA_MODE;
		*conn = AP_RECONNECTING;
		break;
	default:
		// CRITICAL ERROR
		WM_LOG_ERROR("[WM] CRITICAL ERROR: BAD STATE (%d)\n", state);
		break;
	}
}
