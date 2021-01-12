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
#include <tinyara/lwnl/lwnl.h>
#include <netinet/in.h>
#include <tinyara/net/if/wifi.h>
#include <tinyara/net/if/ethernet.h>
#include <tinyara/netmgr/netdev_mgr.h>
#include "netdev_mgr_internal.h"

int netdev_handle_wifi(struct netdev *dev, lwnl_req cmd, void *data, uint32_t data_len)
{
	lwnl_result_e res = LWNL_FAIL;
	switch (cmd) {
	case LWNL_INIT:
	{
		int ret = nm_ifup(dev);
		if (ret != 0) {
			ret = LWNL_FAIL;
		}
	}
	break;
	case LWNL_DEINIT:
	{
		int ret = nm_ifdown(dev);
		if (ret != 0) {
			ret = LWNL_FAIL;
		}
	}
	break;
	case LWNL_GET_INFO:
	{
		trwifi_info *info = (trwifi_info *)data;
		res = dev->t_ops.wl->get_info(dev, info);
	}
	break;
	case LWNL_SET_AUTOCONNECT:
	{

		uint8_t *check = (uint8_t *)data;
		res = dev->t_ops.wl->set_autoconnect(dev, *check);
	}
	break;
	case LWNL_START_STA:
	{
		res = dev->t_ops.wl->start_sta(dev);
	}
	break;
	case LWNL_CONNECT_AP:
	{
		trwifi_ap_config_s *config = (trwifi_ap_config_s *)data;
		res = dev->t_ops.wl->connect_ap(dev, config, NULL);
	}
	break;
	case LWNL_DISCONNECT_AP:
	{
		res = dev->t_ops.wl->disconnect_ap(dev, NULL);
	}
	break;
	case LWNL_START_SOFTAP:
	{
		trwifi_softap_config_s *config = (trwifi_softap_config_s *)data;
		res = dev->t_ops.wl->start_softap(dev, config);
	}
	break;
	case LWNL_STOP_SOFTAP:
	{
		res = dev->t_ops.wl->stop_softap(dev);
	}
	break;
	case LWNL_SCAN_AP:
	{
		res = dev->t_ops.wl->scan_ap(dev, NULL);
	}
	break;
	default:
		res = LWNL_FAIL;
		break;
	}
	return res;
}
