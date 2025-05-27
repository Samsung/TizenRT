/****************************************************************************
 *
 * Copyright 2025 Samsung Electronics All Rights Reserved.
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

#ifndef __CSI_NETWORK_MONITOR_H__
#define __CSI_NETWORK_MONITOR_H__
#include "csimanager/csifw_api.h"

typedef enum CONNECTION_STATE {
	WIFI_CONNECTED,
	WIFI_DISCONNECTED
} CONNECTION_STATE;

typedef void (*network_status_listener)(CONNECTION_STATE state);

CSIFW_RES network_monitor_init(network_status_listener network_status_callback);
CSIFW_RES checkWifiConnection(void);
CSIFW_RES network_monitor_deinit(void);
#endif /* __CSI_NETWORK_MONITOR_H__ */
