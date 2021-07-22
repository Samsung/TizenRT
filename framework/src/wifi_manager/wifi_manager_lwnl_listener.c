/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
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
#include <debug.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <errno.h>
#include <net/if.h>
#include <tinyara/lwnl/lwnl.h>
#include <tinyara/net/if/wifi.h>
#include <tinyara/wifi/wifi_utils.h>
#include "wifi_manager_lwnl_listener.h"
#include "wifi_manager_dhcp.h"
#include "wifi_manager_event.h"
#include "wifi_manager_msghandler.h"
#include <tinyara/net/netlog.h>
#include "wifi_manager_message.h"

#define TAG "[WM]"

static int _lwnl_convert_scan(wifi_utils_scan_list_s **scan_list, void *input, int len)
{
	NET_LOGV(TAG, "len(%d)\n", len);
	int remain = len;
	wifi_utils_scan_list_s *prev = NULL;

	while (remain > 0) {
		wifi_utils_scan_list_s *item = (wifi_utils_scan_list_s *)malloc(sizeof(wifi_utils_scan_list_s));
		if (!item) {
			// To Do
			return -1;
		}
		// definition of wifi_utils_scan_list_s and lwnl80211_scan_list shoud be same
		memcpy(&item->ap_info, input, sizeof(wifi_utils_ap_scan_info_s));
		item->next = NULL;

		remain -= sizeof(wifi_utils_ap_scan_info_s);
		input = input + sizeof(wifi_utils_ap_scan_info_s);
		if (!prev) {
			prev = item;
			*scan_list = item;
			continue;
		}
		prev->next = item;
		prev = item;
	}

	return 0;
}

static wifi_utils_scan_list_s *_lwnl_handle_scan(int fd, int len)
{
	char *buf = (char *)malloc(len);
	if (!buf) {
		return NULL;
	}

	int res = read(fd, buf, len);
	if (res != len) {
		NET_LOGE(TAG, "read error\n");
		free(buf);
		return NULL;
	}

	wifi_utils_scan_list_s *scan_list = NULL;
	res = _lwnl_convert_scan(&scan_list, buf, len);
	free(buf);
	if (res < 0) {
		return NULL;
	}
	return scan_list;
}

/*  only wifi msg handler thread can use this variable */
static wifimgr_msg_s g_msg = {EVT_NONE, WIFI_MANAGER_FAIL, NULL, NULL};

static inline void LWNL_SET_MSG(wifimgr_msg_s *msg, wifimgr_evt_e event,
								wifi_manager_result_e result, void *param, sem_t *signal)
{
	msg->event = event;
	msg->result = result;
	msg->param = param;
	msg->signal = signal;
}

static int _lwnl_call_event(int fd, lwnl_cb_status status, int len)
{
	if (status.evt != LWNL_EVT_SCAN_DONE && len != 0) {
		assert(0);
	}
	switch (status.evt) {
	case LWNL_EVT_STA_CONNECTED:
		LWNL_SET_MSG(&g_msg, WIFIMGR_EVT_STA_CONNECTED, WIFI_MANAGER_FAIL, NULL, NULL);
		break;
	case LWNL_EVT_STA_CONNECT_FAILED:
		LWNL_SET_MSG(&g_msg, WIFIMGR_EVT_STA_CONNECT_FAILED, WIFI_MANAGER_FAIL, NULL, NULL);
		break;
	case LWNL_EVT_STA_DISCONNECTED:
		LWNL_SET_MSG(&g_msg, WIFIMGR_EVT_STA_DISCONNECTED, WIFI_MANAGER_FAIL, NULL, NULL);
		break;
	case LWNL_EVT_SOFTAP_STA_JOINED:
#ifdef CONFIG_WIFIMGR_DISABLE_DHCPS
		LWNL_SET_MSG(&g_msg, WIFIMGR_EVT_JOINED, WIFI_MANAGER_FAIL, NULL, NULL);
#endif
		break;
	case LWNL_EVT_SOFTAP_STA_LEFT:
		LWNL_SET_MSG(&g_msg, WIFIMGR_EVT_LEFT, WIFI_MANAGER_FAIL, NULL, NULL);
		break;
	case LWNL_EVT_SCAN_FAILED:
		LWNL_SET_MSG(&g_msg, WIFIMGR_EVT_SCAN_DONE, WIFI_MANAGER_FAIL, NULL, NULL);
		break;
	case LWNL_EVT_SCAN_DONE:
	{
		wifi_utils_scan_list_s *scan_list = _lwnl_handle_scan(fd, len);
		if (scan_list) {
			LWNL_SET_MSG(&g_msg, WIFIMGR_EVT_SCAN_DONE, WIFI_MANAGER_FAIL, scan_list, NULL);
		} else {
			LWNL_SET_MSG(&g_msg, WIFIMGR_EVT_SCAN_DONE, WIFI_MANAGER_FAIL, NULL, NULL);
		}
		break;
	}
	default:
		NET_LOGE(TAG, "Bad status received (%d)\n", status);
		return -1;
	}
	return 0;
}

/**
 * Public
 */
int lwnl_fetch_event(int fd, void *buf, int buflen)
{
	#define LWNL_CB_HEADER_LEN (sizeof(lwnl_cb_status) + sizeof(uint32_t))
	lwnl_cb_status status;
	uint32_t len;
	char type_buf[LWNL_CB_HEADER_LEN] = {0,};
	handler_msg *hmsg = (handler_msg *)buf;

	/*  lwnl guarantees that type_buf will read LWNL_CB_HEADER_LEN if it succeeds
	* So it doesn't need to consider partial read
	*/
	int nbytes = read(fd, (char *)type_buf, LWNL_CB_HEADER_LEN);
	if (nbytes < 0) {
		NET_LOGE(TAG, "Failed to receive (nbytes=%d)\n", nbytes);
		return -1;
	}

	memcpy(&status, type_buf, sizeof(lwnl_cb_status));
	memcpy(&len, type_buf + sizeof(lwnl_cb_status), sizeof(uint32_t));

	NET_LOGV(TAG, "scan state(%d) length(%d)\n", status.evt, len);
	if (status.type == LWNL_DEV_WIFI) {
		(void)_lwnl_call_event(fd, status, len);
		hmsg->msg = &g_msg;
	} else {
		char *t_buf = (char *)malloc(len);
		nbytes = read(fd, t_buf, len);
		NET_LOGI(TAG, "read unused data in the socket[%d]\n", nbytes);
		free(t_buf);
		hmsg->msg = NULL;
	}

	hmsg->signal = NULL;

	return 0;
}
