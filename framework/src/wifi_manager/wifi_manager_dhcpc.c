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
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <tinyara/net/if/wifi.h>
#include <tinyara/net/netlog.h>
#include <wifi_manager/wifi_manager.h>
#include "wifi_manager_utils.h"
#include "wifi_manager_error.h"
#include "wifi_manager_dhcp.h"

#define TAG "[WM]"

/**
 * Internal DHCP client APIs
 */
#ifndef CONFIG_WIFIMGR_DISABLE_DHCPC
wifi_manager_result_e dhcpc_get_ipaddr(void)
{
	int ret;
	struct in_addr ip;
	wifi_manager_result_e wret = WIFI_MANAGER_FAIL;

	ret = dhcp_client_start(WIFIMGR_STA_IFNAME);
	if (ret != 0) {
		WIFIADD_ERR_RECORD(ERR_WIFIMGR_CONNECT_DHCPC_FAIL);
		NET_LOGE(TAG, "[DHCPC] get IP address fail\n");
		return wret;
	}

	ret = netlib_get_ipv4addr(WIFIMGR_STA_IFNAME, &ip);
	if (ret != 0) {
		NET_LOGE(TAG, "[DHCPC] get IP address fail\n");
		WIFIADD_ERR_RECORD(ERR_WIFIMGR_CONNECT_DHCPC_FAIL);
		return wret;
	}
	NET_LOGV(TAG, "[DHCPC] get IP address %s\n", inet_ntoa(ip));

	return WIFI_MANAGER_SUCCESS;
}
#endif //CONFIG_WIFIMGR_DISABLE_DHCPC

void dhcpc_close_ipaddr(void)
{
#ifndef CONFIG_WIFIMGR_DISABLE_DHCPC
	dhcp_client_stop(WIFIMGR_STA_IFNAME);
#endif
	NET_LOGV(TAG, "release IP address\n");
	return;
}

wifi_manager_result_e dhcpc_get_ap_type(wifi_manager_ap_type_e *aptype)
{
	int wifi_type = (int) *aptype;
	if (dhcp_client_get_aptype(&wifi_type) != OK) {
		NET_LOGE(TAG, "[DHCPC] get ap type fail\n");
		WIFIADD_ERR_RECORD(ERR_WIFIMGR_CONNECT_DHCPC_FAIL);
		return WIFI_MANAGER_FAIL;
	}

	*aptype = (wifi_manager_ap_type_e)wifi_type;
	return WIFI_MANAGER_SUCCESS;
}
