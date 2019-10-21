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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <debug.h>
#include <tinyara/lwnl/lwnl80211.h>
#include <tinyara/wifi/wifi_utils.h>

#define WU_INTF_NAME "wl1"

#define WU_TAG "[WU]"

#define WU_ERR										\
	do {											\
		ndbg(WU_TAG"[ERR] %s:%d code(%s)\n",		\
			 __FILE__, __LINE__, strerror(errno));	\
	} while (0)

#define WU_ERR_FD(fd)									\
	do {												\
		ndbg(WU_TAG"[ERR] %s:%d fd(%d) code(%s)\n",		\
			 __FILE__, __LINE__, fd, strerror(errno));	\
	} while (0)

#define WU_ENTER									\
	do {											\
		ndbg(WU_TAG"%s:%d\n", __FILE__, __LINE__);	\
	} while (0)

typedef enum {
	WIFI_UTILS_LWNL80211_SUCCESS,
	WIFI_UTILS_LWNL80211_OPEN_FAILED,
	WIFI_UTILS_LWNL80211_READ_FAILED,
	WIFI_UTILS_LWNL80211_WRITE_FAILED,
} wu_lwnl80211_status_e;


static inline wu_lwnl80211_status_e _send_msg(lwnl80211_msg *msg)
{
	int fd = socket(AF_LWNL, SOCK_RAW, LWNL_ROUTE);
	if (fd < 0) {
		return -WIFI_UTILS_LWNL80211_OPEN_FAILED;
	}

	int res = write(fd, msg, sizeof(*msg));
	if (res < 0) {
		return -WIFI_UTILS_LWNL80211_WRITE_FAILED;
	}

	close(fd);

	return WIFI_UTILS_LWNL80211_SUCCESS;
}

wifi_utils_result_e wifi_utils_init(void)
{
	WU_ENTER;

	/* Start to send ioctl */
	lwnl80211_msg msg = {WU_INTF_NAME, LWNL80211_INIT, 0, NULL, 0};
	wu_lwnl80211_status_e res = _send_msg(&msg);
	if (res < 0) {
		return WIFI_UTILS_FAIL;
	}

	return WIFI_UTILS_SUCCESS;
}

wifi_utils_result_e wifi_utils_deinit(void)
{
	WU_ENTER;
	wifi_utils_result_e wuret = WIFI_UTILS_SUCCESS;
	lwnl80211_msg msg = {WU_INTF_NAME, LWNL80211_DEINIT, 0, NULL, 0};
	wu_lwnl80211_status_e res = _send_msg(&msg);
	if (res < 0) {
		wuret = WIFI_UTILS_FAIL;
	}

	(void)lwnl_leave_monitor(NULL);

	return wuret;
}

wifi_utils_result_e wifi_utils_scan_ap(void *arg)
{
	WU_ENTER;
	wifi_utils_ap_config_s *config = NULL;
	uint32_t config_len = 0;
	if (arg) {
		config = (wifi_utils_ap_config_s *)arg;
		config_len = sizeof(wifi_utils_ap_config_s);
	}

	lwnl80211_msg msg = {WU_INTF_NAME, LWNL80211_SCAN_AP, sizeof(wifi_utils_ap_config_s), (void *)config, 0};

	wu_lwnl80211_status_e res = _send_msg(&msg);
	if (res < 0) {
		res = WIFI_UTILS_FAIL;
	}
	return WIFI_UTILS_SUCCESS;
}


wifi_utils_result_e wifi_utils_register_callback(wifi_utils_cb_s *cbk)
{
	wifi_utils_result_e wuret = WIFI_UTILS_INVALID_ARGS;

	int res = lwnl_join_monitor(cbk);
	if (res < 0) {
		ndbg("WiFi callback register failure (no callback)\n");
	} else {
		wuret = WIFI_UTILS_SUCCESS;
	}
	return wuret;
}


wifi_utils_result_e wifi_utils_connect_ap(wifi_utils_ap_config_s *ap_connect_config, void *arg)
{
	WU_ENTER;

	lwnl80211_msg msg = {WU_INTF_NAME, LWNL80211_CONNECT_AP,
						 sizeof(wifi_utils_ap_config_s), (void *)ap_connect_config, 0};
	wu_lwnl80211_status_e res = _send_msg(&msg);
	if (res < 0) {
		res = WIFI_UTILS_FAIL;
	}
	return WIFI_UTILS_SUCCESS;
}


wifi_utils_result_e wifi_utils_disconnect_ap(void *arg)
{
	WU_ENTER;

	lwnl80211_msg msg = {WU_INTF_NAME, LWNL80211_DISCONNECT_AP, 0, NULL, 0};
	wu_lwnl80211_status_e res = _send_msg(&msg);
	if (res < 0) {
		res = WIFI_UTILS_FAIL;
	}
	return WIFI_UTILS_SUCCESS;
}


wifi_utils_result_e wifi_utils_get_info(wifi_utils_info_s *wifi_info)
{
	WU_ENTER;
	lwnl80211_msg msg = {WU_INTF_NAME, LWNL80211_GET_INFO,
						 sizeof(wifi_utils_info_s), (void *)wifi_info, 0};
	wu_lwnl80211_status_e res = _send_msg(&msg);
	if (res < 0) {
		res = WIFI_UTILS_FAIL;
	}
	return WIFI_UTILS_SUCCESS;
}


wifi_utils_result_e wifi_utils_start_softap(wifi_utils_softap_config_s *softap_config)
{
	WU_ENTER;

	lwnl80211_msg msg = {WU_INTF_NAME, LWNL80211_START_SOFTAP,
						 sizeof(wifi_utils_softap_config_s), (void *)softap_config, 0};
	wu_lwnl80211_status_e res = _send_msg(&msg);
	if (res < 0) {
		res = WIFI_UTILS_FAIL;
	}
	return WIFI_UTILS_SUCCESS;
}


wifi_utils_result_e wifi_utils_start_sta(void)
{
	WU_ENTER;

	lwnl80211_msg msg = {WU_INTF_NAME, LWNL80211_START_STA, 0, NULL, 0};

	wu_lwnl80211_status_e res = _send_msg(&msg);
	if (res < 0) {
		res = WIFI_UTILS_FAIL;
	}

	return WIFI_UTILS_SUCCESS;
}


wifi_utils_result_e wifi_utils_stop_softap(void)
{
	WU_ENTER;

	lwnl80211_msg msg = {WU_INTF_NAME, LWNL80211_STOP_SOFTAP, 0, NULL, 0};
	wu_lwnl80211_status_e res = _send_msg(&msg);
	if (res < 0) {
		res = WIFI_UTILS_FAIL;
	}
	return WIFI_UTILS_SUCCESS;
}


wifi_utils_result_e wifi_utils_set_autoconnect(uint8_t check)
{
	WU_ENTER;

	uint8_t *chk = &check;
	lwnl80211_msg msg = {WU_INTF_NAME, LWNL80211_SET_AUTOCONNECT,
						 sizeof(uint8_t), (void *)chk, 0};
	wu_lwnl80211_status_e res = _send_msg(&msg);
	if (res < 0) {
		res = WIFI_UTILS_FAIL;
	}

	return WIFI_UTILS_SUCCESS;
}
