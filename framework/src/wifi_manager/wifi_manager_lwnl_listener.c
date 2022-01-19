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
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <errno.h>
#include <net/if.h>
#include <tinyara/lwnl/lwnl.h>
#include <tinyara/net/if/wifi.h>
#include "wifi_manager_dhcp.h"
#include "wifi_manager_event.h"
#include "wifi_manager_msghandler.h"
#include <tinyara/net/netlog.h>
#include "wifi_manager_message.h"

#define TAG "[WM]"

typedef struct {
	wifimgr_evt_e wevt;
	lwnl_cb_wifi levt;
} lwnl_convert_table;

lwnl_convert_table g_convert_table[8] = {
#define WIFIMGR_LWNL_MAP_TABLE(lwnl, type) {type, lwnl},
#include "wifi_manager_lwnl_table.h"
};

/*  only wifi msg handler thread can use this variable */
static wifimgr_msg_s g_msg = {WIFIMGR_EVT_NONE, WIFI_MANAGER_FAIL, NULL, NULL};
static trwifi_cbk_msg_s g_cbk;
static const trwifi_cbk_msg_s g_nullmsg = TRWIFI_CBK_MSG_INITIALIZER;
static inline void LWNL_SET_MSG(wifimgr_msg_s *msg, wifimgr_evt_e event,
								wifi_manager_result_e result, void *param, sem_t *signal)
{
	msg->event = event;
	msg->result = result;
	msg->param = param;
	msg->signal = signal;
}

static wifimgr_evt_e _lwnl_convert_levent(lwnl_cb_wifi levt)
{
	int size = sizeof(g_convert_table) / sizeof(lwnl_convert_table);
	for (int i = 0; i < size; i++) {
		if (levt == g_convert_table[i].levt) {
			return g_convert_table[i].wevt;
		}
	}
	return WIFIMGR_EVT_NONE;
}
static int _lwnl_convert_scan(trwifi_scan_list_s **scan_list, void *input, int len)
{
	NET_LOGV(TAG, "len(%d)\n", len);
	int remain = len;
	trwifi_scan_list_s *prev = NULL;

	while (remain > 0) {
		trwifi_scan_list_s *item = (trwifi_scan_list_s *)malloc(sizeof(trwifi_scan_list_s));
		if (!item) {
			assert(0); // out of memory
			return -1;
		}
		// definition of trwifi_scan_list_s and lwnl80211_scan_list shoud be same
		memcpy(&item->ap_info, input, sizeof(trwifi_ap_scan_info_s));
		item->next = NULL;

		remain -= sizeof(trwifi_ap_scan_info_s);
		input = input + sizeof(trwifi_ap_scan_info_s);
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

static trwifi_scan_list_s *_lwnl_handle_scan(int fd, int len)
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

	trwifi_scan_list_s *scan_list = NULL;
	res = _lwnl_convert_scan(&scan_list, buf, len);
	free(buf);
	if (res < 0) {
		return NULL;
	}
	return scan_list;
}

static int _lwnl_generate_msg(int fd, lwnl_cb_status status, int len)
{
	switch (status.evt) {
	case LWNL_EVT_SCAN_DONE: {
		trwifi_scan_list_s *scan_list = _lwnl_handle_scan(fd, len);
		if (scan_list) {
			LWNL_SET_MSG(&g_msg, WIFIMGR_EVT_SCAN_DONE, WIFI_MANAGER_FAIL, scan_list, NULL);
		} else {
			LWNL_SET_MSG(&g_msg, WIFIMGR_EVT_SCAN_DONE, WIFI_MANAGER_FAIL, NULL, NULL);
		}
		break;
	}
	case LWNL_EVT_SCAN_FAILED:
		LWNL_SET_MSG(&g_msg, WIFIMGR_EVT_SCAN_DONE, WIFI_MANAGER_FAIL, NULL, NULL);
		break;
	case LWNL_EVT_STA_CONNECTED:
	case LWNL_EVT_STA_CONNECT_FAILED:
	case LWNL_EVT_STA_DISCONNECTED:
	case LWNL_EVT_SOFTAP_STA_JOINED:
	case LWNL_EVT_SOFTAP_STA_LEFT: {
		if (len == sizeof(trwifi_cbk_msg_s)) {
			int res = read(fd, (void *)&g_cbk, len);
			if (res == -1) {
				NET_LOGE(TAG, "read lwnl msg error %d %d\n", status.evt, errno);
				assert(0);
			}
		} else if (len == 0){
			g_cbk = g_nullmsg;
		} else {
			assert(0);
		}
		wifimgr_evt_e wevt = _lwnl_convert_levent(status.evt);
		LWNL_SET_MSG(&g_msg, wevt, WIFI_MANAGER_FAIL, &g_cbk, NULL);
		break;
	}
	default:
		NET_LOGE(TAG, "Bad status received (%d)\n", status.evt);
		return -1;
	}
	return 0;
}

/**
 * Public
 */
int lwnl_fetch_event(int fd, void *buf, int buflen)
{
	lwnl_cb_status status;
	uint32_t len;
	char type_buf[LWNL_CB_HEADER_LEN] = {0, };
	handler_msg *hmsg = (handler_msg *)buf;

	/* lwnl guarantees that type_buf will read LWNL_CB_HEADER_LEN if it succeeds
	 * So it doesn't need to consider partial read
	 */
	int nbytes = read(fd, (char *)type_buf, LWNL_CB_HEADER_LEN);
	if (nbytes < 0) {
		NET_LOGE(TAG, "Failed to receive (nbytes=%d)\n", nbytes);
		return -1;
	}

	memcpy(&status, type_buf, sizeof(lwnl_cb_status));
	memcpy(&len, type_buf + sizeof(lwnl_cb_status), sizeof(uint32_t));

	NET_LOGV(TAG, "receive state(%d) length(%d)\n", status.evt, len);
	(void)_lwnl_generate_msg(fd, status, len);

	hmsg->msg = &g_msg;
	hmsg->signal = NULL;

	return 0;
}
