/****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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

#ifndef _THINGS_NETWORK_H_
#define _THINGS_NETWORK_H_

#include "cacommon.h"
#include "things_api.h"

#define MAX_ESSID            (36)
#define MAX_SECUIRTYKEY      (64)
#define MAX_TYPE_AUTH        (16)
#define MAX_TYPE_ENC         (16)
#define MAX_CHANNEL           (8)
#define MAX_LEVEL_SIGNAL     (12)
#define MAX_BSSID            (20)

typedef struct access_point_info_s {
	char e_ssid[MAX_ESSID];		// mandatory
	char security_key[MAX_SECUIRTYKEY];	// mandatory
	char auth_type[MAX_TYPE_AUTH];	// mandatory (None | WEP | WPA-PSK | WPA2-PSK)
	char enc_type[MAX_TYPE_ENC];	// mandatory (WEP-64 | WEP-128 | TKIP | AES | TKIP_AES)
	char channel[MAX_CHANNEL];	// optional
	char signal_level[MAX_LEVEL_SIGNAL];	// optional
	char bss_id[MAX_BSSID];		// optional
} access_point_info_s;

bool things_is_connected_ap(void);

int things_is_net_initialize(void);

void things_wifi_state_changed_cb_init(void);

int things_wifi_state_changed_cb(int state, char *ap_name, char *ip_addr);

void things_adapter_state_cb(CATransportAdapter_t adapter, bool enabled);

void things_tcp_session_state_cb(const CAEndpoint_t *info, bool connected);

int things_set_ap_connection(access_point_info_s *APinfo);

int things_get_ap_list(access_point_info_s ***p_info, int *p_count);

typedef int (*things_set_ap_connection_func_type)(access_point_info_s *p_info, char *p_cmd_id);

typedef int (*things_get_ap_search_list_func_type)(access_point_info_s ***p_info, int *p_count);

/*
 * If Easy-Setup is Reseted, then need to recover about Current Connected-AP info.
 * This Function serve to recover Connected-AP info with Customized-method.
 */
void things_apply_current_ap(void);

#endif							/* _THINGS_NETWORK_H_ */
