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
#include <net/if.h>
#include <tinyara/lwnl/lwnl.h>
#include <tinyara/wifi/wifi_utils.h>
#include <tinyara/net/if/wifi.h>
#include "wifi_manager_log.h"

#ifdef CONFIG_NET_NETMGR
#define WU_INTF_NAME "wlan0"
#else
#define WU_INTF_NAME "wl1"
#endif

typedef enum {
	WIFI_UTILS_LWNL_SUCCESS,
	WIFI_UTILS_LWNL_OPEN_FAILED,
	WIFI_UTILS_LWNL_READ_FAILED,
	WIFI_UTILS_LWNL_WRITE_FAILED,
} wu_lwnl_status_e;


static inline wu_lwnl_status_e _send_msg(lwnl_msg *msg)
{
	int fd = socket(AF_LWNL, SOCK_RAW, LWNL_ROUTE);
	if (fd < 0) {
		return -WIFI_UTILS_LWNL_OPEN_FAILED;
	}

	int res = write(fd, msg, sizeof(*msg));
	close(fd);

	if (res < 0) {
		return -WIFI_UTILS_LWNL_WRITE_FAILED;
	}

	return WIFI_UTILS_LWNL_SUCCESS;
}

wifi_utils_result_e wifi_utils_init(void)
{
	WM_ENTER;

	/* Start to send ioctl */
	lwnl_msg msg = {WU_INTF_NAME, LWNL_INIT, 0, NULL, 0};
	wu_lwnl_status_e res = _send_msg(&msg);
	if (res < 0) {
		return WIFI_UTILS_FAIL;
	}

	return WIFI_UTILS_SUCCESS;
}

wifi_utils_result_e wifi_utils_deinit(void)
{
	WM_ENTER;
	wifi_utils_result_e wuret = WIFI_UTILS_SUCCESS;
	lwnl_msg msg = {WU_INTF_NAME, LWNL_DEINIT, 0, NULL, 0};
	wu_lwnl_status_e res = _send_msg(&msg);
	if (res < 0) {
		wuret = WIFI_UTILS_FAIL;
	}

	(void)lwnl_leave_monitor(NULL);

	return wuret;
}

wifi_utils_result_e wifi_utils_scan_ap(void *arg)
{
	WM_ENTER;
	wifi_utils_ap_config_s *config = NULL;
	uint32_t config_len = 0;
	if (arg) {
		config = (wifi_utils_ap_config_s *)arg;
		config_len = sizeof(wifi_utils_ap_config_s);
	}

	lwnl_msg msg = {WU_INTF_NAME, LWNL_SCAN_AP, sizeof(wifi_utils_ap_config_s), (void *)config, 0};

	wu_lwnl_status_e res = _send_msg(&msg);
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
		WM_LOG_ERROR("WiFi callback register failure (no callback)\n");
	} else {
		wuret = WIFI_UTILS_SUCCESS;
	}
	return wuret;
}


wifi_utils_result_e wifi_utils_connect_ap(wifi_utils_ap_config_s *ap_connect_config, void *arg)
{
	WM_ENTER;

	lwnl_msg msg = {WU_INTF_NAME, LWNL_CONNECT_AP,
						 sizeof(wifi_utils_ap_config_s), (void *)ap_connect_config, 0};
	wu_lwnl_status_e res = _send_msg(&msg);
	if (res < 0) {
		res = WIFI_UTILS_FAIL;
	}
	return WIFI_UTILS_SUCCESS;
}


wifi_utils_result_e wifi_utils_disconnect_ap(void *arg)
{
	WM_ENTER;

	lwnl_msg msg = {WU_INTF_NAME, LWNL_DISCONNECT_AP, 0, NULL, 0};
	wu_lwnl_status_e res = _send_msg(&msg);
	if (res < 0) {
		res = WIFI_UTILS_FAIL;
	}
	return WIFI_UTILS_SUCCESS;
}


wifi_utils_result_e wifi_utils_get_info(wifi_utils_info_s *wifi_info)
{
	WM_ENTER;

	lwnl_msg msg = {WU_INTF_NAME, LWNL_GET_INFO,
						 sizeof(wifi_utils_info_s), (void *)wifi_info, 0};
	wu_lwnl_status_e res = _send_msg(&msg);
	if (res < 0) {
		res = WIFI_UTILS_FAIL;
	}
	return WIFI_UTILS_SUCCESS;
}


wifi_utils_result_e wifi_utils_start_softap(wifi_utils_softap_config_s *softap_config)
{
	WM_ENTER;

	lwnl_msg msg = {WU_INTF_NAME, LWNL_START_SOFTAP,
						 sizeof(wifi_utils_softap_config_s), (void *)softap_config, 0};
	wu_lwnl_status_e res = _send_msg(&msg);
	if (res < 0) {
		res = WIFI_UTILS_FAIL;
	}
	return WIFI_UTILS_SUCCESS;
}


wifi_utils_result_e wifi_utils_start_sta(void)
{
	WM_ENTER;

	lwnl_msg msg = {WU_INTF_NAME, LWNL_START_STA, 0, NULL, 0};

	wu_lwnl_status_e res = _send_msg(&msg);
	if (res < 0) {
		res = WIFI_UTILS_FAIL;
	}

	return WIFI_UTILS_SUCCESS;
}


wifi_utils_result_e wifi_utils_stop_softap(void)
{
	WM_ENTER;

	lwnl_msg msg = {WU_INTF_NAME, LWNL_STOP_SOFTAP, 0, NULL, 0};
	wu_lwnl_status_e res = _send_msg(&msg);
	if (res < 0) {
		res = WIFI_UTILS_FAIL;
	}
	return WIFI_UTILS_SUCCESS;
}


wifi_utils_result_e wifi_utils_set_autoconnect(uint8_t check)
{
	WM_ENTER;

	uint8_t *chk = &check;
	lwnl_msg msg = {WU_INTF_NAME, LWNL_SET_AUTOCONNECT,
						 sizeof(uint8_t), (void *)chk, 0};
	wu_lwnl_status_e res = _send_msg(&msg);
	if (res < 0) {
		res = WIFI_UTILS_FAIL;
	}

	return WIFI_UTILS_SUCCESS;
}
