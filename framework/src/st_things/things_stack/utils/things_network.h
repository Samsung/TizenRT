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

//--------------------------------------------------------------------------------
/**
 * @brief It initialize the WiFi manager callbacks.
 * @return : success return 1, othewise 0
 */
int things_network_initialize(void);

//--------------------------------------------------------------------------------
/**
 * @brief API for getting home ap configuration information.This function calling
 * the wifi manager api internally to get the configuration details.
 * @return home ap configuration information.
 */
wifi_manager_ap_config_s *things_network_get_homeap_config(void);

//--------------------------------------------------------------------------------
/**
 * @brief API for turn on soft ap.It create the soft ap.
 * @return success return 1, othewise 0
 */
bool things_network_turn_on_soft_ap(void);

//--------------------------------------------------------------------------------
/**
 * @brief function for connect client to home ap network.
 */
bool things_network_connect_home_ap(void);

//--------------------------------------------------------------------------------
/**
 * @brief function prototype to check that things is connected to target AP
 * @return true if things is connected to target AP, otherwise return false
 */
bool things_is_connected_ap(void);

//--------------------------------------------------------------------------------
/**
 * @brief callback function for changed the WiFi state(0 to 1 or 1 to 0)
 *	 State :1 -> Connected to WiFi AP.
 *	 State :0 -> Disconnected from WiFi AP.
 * @param state : WiFi state
 * @ap_name : Access point SSID
 * @ip_addr : IP address
 * @return 1 = successfully changed the WiFi state.
 */
int things_wifi_state_changed_cb(int state, char *ap_name, char *ip_addr);

//--------------------------------------------------------------------------------
/**
 * @brief callback function for adapter state changed.It is called from Iotivity
 *  Framework when Adaptor state changed.
 * @adapter : Adapter type
 * @enabled : true : connected
 *	      false : disconnected
 */
void things_adapter_state_cb(CATransportAdapter_t adapter, bool enabled);

//--------------------------------------------------------------------------------
/**
 * @brief callback function for TCP state changed.It is called from Iotivity framework
 *  when TCP session state changed.
 * @param info : Information about remote-peer.
 * @connected : true -> connected
 *		false -> disconnected.
 */
void things_tcp_session_state_cb(const CAEndpoint_t *info, bool connected);

//--------------------------------------------------------------------------------
/**
 * @brief API for saving the WiFi AP information.
 * @param ap_info : Access point information.
 * @return : 1 = success, otherwise return -1 value.
 */
int things_set_ap_connection(access_point_info_s *ap_info);

//--------------------------------------------------------------------------------
/**
 * @brief API for calling the WiFi manager scan api.
 * @return : 1 = success, otherwise negative value.
 */
int things_wifi_scan_ap(void);

//--------------------------------------------------------------------------------
/**
 * @brief API for get the ap list.
 * @param p_info : Access point information.
 * @param p_count : total number of ap list.
 * @return : 1 = success, otherwise return 0 value.
 */
int things_get_ap_list(access_point_info_s** p_info, int* p_count);

#endif							/* _THINGS_NETWORK_H_ */
