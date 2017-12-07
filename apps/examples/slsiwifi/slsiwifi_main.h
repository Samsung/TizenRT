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

#ifndef __SLSIWIFI_MAIN_H
#define __SLSIWIFI_MAIN_H

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include "slsi_wifi_api.h"
#include "nettest_functions.h"

#ifdef CONFIG_SLSI_WIFI_P2P_API
#define CONFIG_SLSI_WIFI_P2P_APP
#endif

void sw_linkUpHandler(slsi_reason_t *reason);
void sw_linkDownHandler(slsi_reason_t *reason);

#ifdef CONFIG_SLSI_WIFI_P2P_APP
void sw_connectionHandler(slsi_peer_info_t *, slsi_config_method_t, slsi_p2p_connect_response_t *, uint8_t *, char **);
void sw_p2pLinkUpHandler(slsi_reason_t *reason);
void sw_p2pLinkDownHandler(slsi_reason_t *reason);
#endif

#endif
