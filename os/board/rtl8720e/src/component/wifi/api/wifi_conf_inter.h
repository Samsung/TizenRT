/******************************************************************************
 * Copyright (c) 2013-2016 Realtek Semiconductor Corp.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
  ******************************************************************************
  * @file    wifi_conf_inter.h
  * @author
  * @version
  * @brief   This file provides internal interface for Wi-Fi  and other modules.
  ******************************************************************************
  */
#ifndef __WIFI_CONF_INTER_H
#define __WIFI_CONF_INTER_H


#include "basic_types.h"
#include "wifi_constants.h"
#include "wifi_structures.h"
#include "wlan_intf.h"
#include "wifi_ind.h"
#include <platform_stdlib.h>
#include "bt_intf.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief  Set PTA type when coex.
 * @param[in]  type: the PTA type(PTA_BT/PTA_WIFI/PTA_AUTO).
 * @return  Null.
 */
void wifi_btcoex_set_pta(pta_type_t type);

/**
  * @brief Write BT RFK  data to RFC
  * @param  calibration data
  * @retval  BT RFK result(1:success  0:failed)
  */
int wifi_btcoex_bt_rfk(struct bt_rfk_param *rfk_param);

/**
 * @brief  Set global variable wifi_wpa_mode.
 * @param[in]  wpa_mode:
 	(WPA_AUTO_MODE,WPA_ONLY_MODE, WPA2_ONLY_MODE,
	WPA3_ONLY_MODE,WPA_WPA2_MIXED_MODE, WPA2_WPA3_MIXED_MODE).
 * @return  0:success  -1:fail.
 */
int wifi_set_wpa_mode(rtw_wpa_mode wpa_mode);

/**
 * @brief  Dynamically modify the working mode of pmf.
 * @param[in]  pmf_mode:
 	0: none
 	1: optional
 	2: required
 * @return  0:success  -1:fail.
 */
int wifi_set_pmf_mode(u8 pmf_mode);

#ifdef __cplusplus
}
#endif


#endif // __WIFI_INTER_API_H

