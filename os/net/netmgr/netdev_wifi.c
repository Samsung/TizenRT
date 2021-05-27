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
#include <stdint.h>
#include <errno.h>
#include <net/if.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <debug.h>
#include <tinyara/lwnl/lwnl.h>
#include <tinyara/net/if/wifi.h>
#include <tinyara/net/if/ethernet.h>
#include <tinyara/netmgr/netdev_mgr.h>
#include "netdev_mgr_internal.h"

#define TRDRV_CALL(res, dev, method, param)		\
	do {										\
		if (dev->t_ops.wl->method) {			\
			res = (dev->t_ops.wl->method)param;	\
		}										\
	} while (0)

int netdev_handle_wifi(struct netdev *dev, lwnl_req cmd, void *data, uint32_t data_len)
{
	trwifi_result_e res = TRWIFI_FAIL;
	lldbg("T%d cmd(%d) (%p) (%d)\n", getpid(), cmd, data, data_len);
	switch (cmd.type) {
	case LWNL_REQ_WIFI_INIT:
	{
		if (0 == nm_ifup(dev)) {
			res = TRWIFI_SUCCESS;
		}
	}
	break;
	case LWNL_REQ_WIFI_DEINIT:
	{
		if (0 == nm_ifdown(dev)) {
			res = TRWIFI_SUCCESS;
		}
	}
	break;
	case LWNL_REQ_WIFI_GETINFO:
	{
		TRDRV_CALL(res, dev, get_info, (dev, (trwifi_info *)data));
	}
	break;
	case LWNL_REQ_WIFI_SETAUTOCONNECT:
	{
		TRDRV_CALL(res, dev, set_autoconnect, (dev, *((uint8_t *)data)));
	}
	break;
	case LWNL_REQ_WIFI_STARTSTA:
	{
		TRDRV_CALL(res, dev, start_sta, (dev));
	}
	break;
	case LWNL_REQ_WIFI_CONNECTAP:
	{
		TRDRV_CALL(res, dev, connect_ap, (dev, (trwifi_ap_config_s*)data, NULL));
	}
	break;
	case LWNL_REQ_WIFI_DISCONNECTAP:
	{
		TRDRV_CALL(res, dev, disconnect_ap, (dev, NULL));
	}
	break;
	case LWNL_REQ_WIFI_STARTSOFTAP:
	{
		TRDRV_CALL(res, dev, start_softap, (dev, (trwifi_softap_config_s *)data));
	}
	break;
	case LWNL_REQ_WIFI_STOPSOFTAP:
	{
		TRDRV_CALL(res, dev, stop_softap, (dev));
	}
	break;
	case LWNL_REQ_WIFI_SCANAP:
	{
		TRDRV_CALL(res, dev, scan_ap, (dev, NULL));
	}
	break;
	case LWNL_IOCTL:
	{
		TRDRV_CALL(res, dev, drv_ioctl, (dev, (trwifi_msg_s *)data));
	}
	break;
	default:
		break;
	}
	return res;
}

int trwifi_serialize_scaninfo(uint8_t **buffer, trwifi_scan_list_s *scan_list)
{
	trwifi_scan_list_s *item = scan_list;
	int32_t cnt = 0;
	int32_t total = 0;

	while (item) {
		item = item->next;
		cnt++;
	}
	total = cnt * sizeof(trwifi_ap_scan_info_s);
	uint32_t item_size = sizeof(trwifi_ap_scan_info_s);
	nvdbg("total size(%d) (%d) \n", sizeof(trwifi_ap_scan_info_s), total);

	*buffer = (uint8_t *)kmm_malloc(total);
	if (!(*buffer)) {
		ndbg("malloc fail %d\n", total);
		return -1;
	}

	item = scan_list;
	cnt = 0;
	while (item) {
		memcpy(*buffer + (item_size * cnt), &item->ap_info, item_size);
		item = item->next;
		cnt++;
	}
	return total;
}
