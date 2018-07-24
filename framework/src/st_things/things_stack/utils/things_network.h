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

#include <wifi_manager/wifi_manager.h>

#include "cacommon.h"
#include "things_api.h"
#include "things_types.h"

int things_network_initialize(void);

wifi_manager_ap_config_s *things_network_get_homeap_config(void);

bool things_network_turn_on_soft_ap(void);

bool things_network_connect_home_ap(void);

bool things_is_connected_ap(void);

int things_wifi_state_changed_cb(int state, char *ap_name, char *ip_addr);

void things_adapter_state_cb(CATransportAdapter_t adapter, bool enabled);

void things_tcp_session_state_cb(const CAEndpoint_t *info, bool connected);

int things_set_ap_connection(access_point_info_s *ap_info);

int things_wifi_scan_ap(void);

int things_get_ap_list(access_point_info_s** p_info, int* p_count);

#endif							/* _THINGS_NETWORK_H_ */
