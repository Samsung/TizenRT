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

static inline wifi_utils_result_e _convert_trwifi_result(trwifi_result_e res)
{
	switch (res) {
	case TRWIFI_FAIL:
		return WIFI_UTILS_FAIL;
	case TRWIFI_SUCCESS:
		return WIFI_UTILS_SUCCESS;
	case TRWIFI_INVALID_ARGS:
		return WIFI_UTILS_INVALID_ARGS;
	case TRWIFI_TIMEOUT:
		return WIFI_UTILS_TIMEOUT;
	case TRWIFI_BUSY:
		return WIFI_UTILS_BUSY;
	case TRWIFI_FILE_ERROR:
		return WIFI_UTILS_FILE_ERROR;
	case TRWIFI_ALREADY_CONNECTED:
		return WIFI_UTILS_ALREADY_CONNECTED;
	case TRWIFI_NOT_SUPPORTED:
		return WIFI_UTILS_NOT_SUPPORTED;
	default:
		return WIFI_UTILS_FAIL;
	}
}

static inline int _send_msg(lwnl_msg *msg)
{
	int fd = socket(AF_LWNL, SOCK_RAW, LWNL_ROUTE);
	if (fd < 0) {
		WM_LOG_ERROR("send lwnl msg open error\n");
		return -1;
	}

	int res = write(fd, msg, sizeof(*msg));
	close(fd);
	if (res < 0) {
		WM_LOG_ERROR("send lwnl msg write error\n");
		return -2;
	}
	return 0;
}

wifi_utils_result_e wifi_utils_init(void)
{
	WM_ENTER;
	trwifi_result_e res = TRWIFI_SUCCESS;
	lwnl_msg msg = {WU_INTF_NAME, {LWNL_REQ_WIFI_INIT}, 0, NULL, (void *)&res};
	if (_send_msg(&msg) < 0) {
		return WIFI_UTILS_FAIL;
	}
	return _convert_trwifi_result(res);
}

wifi_utils_result_e wifi_utils_deinit(void)
{
	WM_ENTER;
	trwifi_result_e res = TRWIFI_SUCCESS;
	lwnl_msg msg = {WU_INTF_NAME, {LWNL_REQ_WIFI_DEINIT}, 0, NULL, (void *)&res};
	if (_send_msg(&msg) < 0) {
		return WIFI_UTILS_FAIL;
	}
	return _convert_trwifi_result(res);
}

wifi_utils_result_e wifi_utils_scan_ap(void *arg)
{
	WM_ENTER;
	trwifi_result_e res = TRWIFI_SUCCESS;
	wifi_utils_ap_config_s *config = NULL;
	if (arg) {
		config = (wifi_utils_ap_config_s *)arg;
	}
	lwnl_msg msg = {WU_INTF_NAME, {LWNL_REQ_WIFI_SCANAP},
					sizeof(wifi_utils_ap_config_s), (void *)config, (void *)&res};
	if (_send_msg(&msg) < 0) {
		return WIFI_UTILS_FAIL;
	}
	return _convert_trwifi_result(res);
}

/* wifi_manager_lwnl_listener monitor status of wi-fi driver
 * So wifi_manager_lwnl doesn't need to register callback */
wifi_utils_result_e wifi_utils_register_callback(wifi_utils_cb_s *cbk)
{
	return WIFI_UTILS_SUCCESS;
}

wifi_utils_result_e wifi_utils_connect_ap(wifi_utils_ap_config_s *ap_connect_config, void *arg)
{
	WM_ENTER;
	trwifi_result_e res = TRWIFI_SUCCESS;
	lwnl_msg msg = {WU_INTF_NAME, {LWNL_REQ_WIFI_CONNECTAP},
					sizeof(wifi_utils_ap_config_s), (void *)ap_connect_config, (void *)&res};
	if (_send_msg(&msg) < 0) {
		return WIFI_UTILS_FAIL;
	}
	return _convert_trwifi_result(res);
}

wifi_utils_result_e wifi_utils_disconnect_ap(void *arg)
{
	WM_ENTER;
	trwifi_result_e res = TRWIFI_SUCCESS;
	lwnl_msg msg = {WU_INTF_NAME, {LWNL_REQ_WIFI_DISCONNECTAP}, 0, NULL, (void *)&res};
	if (_send_msg(&msg) < 0) {
		return WIFI_UTILS_FAIL;
	}
	return _convert_trwifi_result(res);
}

wifi_utils_result_e wifi_utils_start_softap(wifi_utils_softap_config_s *softap_config)
{
	WM_ENTER;
	trwifi_result_e res = TRWIFI_SUCCESS;
	lwnl_msg msg = {WU_INTF_NAME, {LWNL_REQ_WIFI_STARTSOFTAP},
					sizeof(wifi_utils_softap_config_s),
					(void *)softap_config, (void *)&res};
	if (_send_msg(&msg) < 0) {
		return WIFI_UTILS_FAIL;
	}
	return _convert_trwifi_result(res);
}

wifi_utils_result_e wifi_utils_start_sta(void)
{
	WM_ENTER;
	trwifi_result_e res = TRWIFI_SUCCESS;
	lwnl_msg msg = {WU_INTF_NAME, {LWNL_REQ_WIFI_STARTSTA}, 0, NULL, (void *)&res};
	if (_send_msg(&msg) < 0) {
		return WIFI_UTILS_FAIL;
	}
	return _convert_trwifi_result(res);
}

wifi_utils_result_e wifi_utils_stop_softap(void)
{
	WM_ENTER;
	trwifi_result_e res = TRWIFI_SUCCESS;
	lwnl_msg msg = {WU_INTF_NAME, {LWNL_REQ_WIFI_STOPSOFTAP}, 0, NULL, (void *)&res};
	if (_send_msg(&msg) < 0) {
		return WIFI_UTILS_FAIL;
	}
	return _convert_trwifi_result(res);
}

wifi_utils_result_e wifi_utils_set_autoconnect(uint8_t check)
{
	WM_ENTER;
	trwifi_result_e res = TRWIFI_SUCCESS;
	uint8_t *chk = &check;
	lwnl_msg msg = {WU_INTF_NAME, {LWNL_REQ_WIFI_SETAUTOCONNECT},
					sizeof(uint8_t), (void *)chk, (void *)&res};
	if (_send_msg(&msg) < 0) {
		return WIFI_UTILS_FAIL;
	}
	return _convert_trwifi_result(res);
}

wifi_utils_result_e wifi_utils_ioctl(trwifi_msg_s *dmsg)
{
	WM_ENTER;
	trwifi_result_e res = TRWIFI_SUCCESS;
	lwnl_msg msg = {WU_INTF_NAME, {LWNL_REQ_WIFI_IOCTL},
					sizeof(trwifi_msg_s), (void *)dmsg, (void *)&res};
	if (_send_msg(&msg) < 0) {
		return WIFI_UTILS_FAIL;
	}
	return _convert_trwifi_result(res);
}

wifi_utils_result_e wifi_utils_get_info(wifi_utils_info_s *wifi_info)
{
	WM_ENTER;
	trwifi_result_e res = TRWIFI_SUCCESS;
	lwnl_msg msg = {WU_INTF_NAME, {LWNL_REQ_WIFI_GETINFO},
					sizeof(wifi_utils_info_s), (void *)wifi_info, (void *)&res};
	if (_send_msg(&msg) < 0) {
		return WIFI_UTILS_FAIL;
	}
	return _convert_trwifi_result(res);
}
