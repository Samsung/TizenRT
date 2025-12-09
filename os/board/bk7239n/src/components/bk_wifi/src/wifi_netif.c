// Copyright 2020-2025 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

//TODO
// 1. We should finally delete this file
// 2. The LWIP and WiFi stack should communication via EVENT

#include <components/netif.h>
#include <components/event.h>
#include "bk_rw.h"
#include "wifi_v2.h"
#include "bk_wifi.h"
#include "bk_wifi_types.h"
#include "common/bk_err.h"
#include "bk_wifi_prop_private.h"

#define WIFI_INVALID_VIFID 0xff

int wifi_netif_mac_to_vifid(uint8_t *mac)
{
	if(!mac || (!(mac[0] | mac[1] | mac[2] | mac[3] | mac[4] | mac[5])))
		return WIFI_INVALID_VIFID;

	return rwm_mgmt_vif_mac2idx(mac);
}

static void *wifi_netif_vifid_to_vif_impl(int vifid)
{
	return rwm_mgmt_vif_idx2ptr(vifid);
}

void *wifi_netif_vifid_to_vif(int vifid)
{
	return wifi_netif_vifid_to_vif_impl(vifid);
}

static void *wifi_netif_mac_to_vif_impl(uint8_t *mac)
{
	int vifid = wifi_netif_mac_to_vifid(mac);
	return wifi_netif_vifid_to_vif_impl(vifid);
}

void *wifi_netif_mac_to_vif(uint8_t *mac)
{
	return wifi_netif_mac_to_vif_impl(mac);
}

uint8_t *wifi_netif_vif_to_mac(void *vif)
{
	return mac_vif_mgmt_get_mac_addr(vif);
}

//TODO is vif->index same as vifid???
int wifi_netif_vif_to_vifid(void *vif)
{
	if (vif)
		return mac_vif_mgmt_get_index(vif);

	return WIFI_INVALID_VIFID;
}

netif_if_t wifi_netif_vif_to_netif_type(void *vif)
{
	if (!vif) {
		return NETIF_IF_INVALID;
	} else if(mac_vif_mgmt_get_type(vif) == VIF_AP) {
		return NETIF_IF_AP;
	} else if(mac_vif_mgmt_get_type(vif) == VIF_STA) {
		return NETIF_IF_STA;
	} else {
		return NETIF_IF_INVALID;
	}
}

netif_if_t wifi_netif_mac_to_netif_type(uint8_t *mac)
{
	void *vif_entry = wifi_netif_mac_to_vif(mac);

	return wifi_netif_vif_to_netif_type(vif_entry);
}

void wifi_netif_set_vif_private_data(void *vif, void *data)
{
	if (vif)
		mac_vif_mgmt_set_priv(vif, data);
}

void *wifi_netif_get_vif_private_data(void *vif)
{
	if (vif)
		return mac_vif_mgmt_get_priv(vif);

	return NULL;
}

extern sta_param_t *g_sta_param_ptr;
void wifi_netif_notify_sta_got_ip(enum ip_ver ver)
{
	wifi_linkstate_reason_t info;
	unsigned char vif_idx;

	/* set wifi status */
	info.state = WIFI_LINKSTATE_STA_GOT_IP;
	info.reason_code = WIFI_REASON_MAX;
	mhdr_set_station_status(info);

	/* post event GOT_IP4 */
	netif_event_got_ip4_t event_data = {0};
	event_data.netif_if = NETIF_IF_STA;

	vif_idx = wifi_netif_mac_to_vifid((uint8_t*)&g_sta_param_ptr->own_mac);
        if (vif_idx != WIFI_INVALID_VIFID)
		wlan_dhcp_done_ind(vif_idx);
	if (ver == IP4)
		bk_event_post(EVENT_MOD_NETIF, EVENT_NETIF_GOT_IP4,
									&event_data, sizeof(event_data), BEKEN_NEVER_TIMEOUT);
	else
		bk_event_post(EVENT_MOD_NETIF, EVENT_NETIF_GOT_IP6,
									&event_data, sizeof(event_data), BEKEN_NEVER_TIMEOUT);
}

void wifi_netif_notify_sta_dhcp_timeout(void)
{
	wifi_linkstate_reason_t info;
	netif_event_got_ip4_t event_data = {0};

	/* set link status */
	info.state = WIFI_LINKSTATE_STA_CONNECTED;
	info.reason_code = WIFI_REASON_DHCP_TIMEOUT;
	mhdr_set_station_status(info);

	/* Send Event DHCP_TIMEOUT */
	event_data.netif_if = NETIF_IF_STA;
	bk_event_post(EVENT_MOD_NETIF, EVENT_NETIF_DHCP_TIMEOUT,
				  &event_data, sizeof(event_data), BEKEN_NEVER_TIMEOUT);
}

void wifi_netif_notify_sta_disconnect(void)
{
	bk_wlan_dhcp_timeout();
}

bool wifi_netif_sta_is_connected(void)
{
	wifi_linkstate_reason_t info = mhdr_get_station_status();
	return (info.state == WIFI_LINKSTATE_STA_CONNECTED);
}

bool wifi_netif_sta_is_got_ip(void)
{
	wifi_linkstate_reason_t info = mhdr_get_station_status();
	return (info.state == WIFI_LINKSTATE_STA_GOT_IP);
}

#if !CONFIG_DISABLE_DEPRECIATED_WIFI_API
void wifi_netif_call_status_cb_when_sta_got_ip(void)
{
	FUNC_1PARAM_PTR fn;
	wifi_linkstate_reason_t info = mhdr_get_station_status();

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
	fn = (FUNC_1PARAM_PTR)bk_wlan_get_status_cb();
#pragma GCC diagnostic pop
	if(fn) {
		info.state = WIFI_LINKSTATE_STA_GOT_IP;
		info.reason_code = WIFI_REASON_MAX;
		(*fn)(&info);
	}
}

void wifi_netif_call_status_cb_when_sta_dhcp_timeout(void)
{
	FUNC_1PARAM_PTR fn;
	wifi_linkstate_reason_t info = mhdr_get_station_status();

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
	fn = (FUNC_1PARAM_PTR)bk_wlan_get_status_cb();
#pragma GCC diagnostic pop
	if(fn) {
		info.state = WIFI_LINKSTATE_STA_DISCONNECTED;
		info.reason_code = WIFI_REASON_DHCP_TIMEOUT;
		(*fn)(&info);
	}
}
#endif

