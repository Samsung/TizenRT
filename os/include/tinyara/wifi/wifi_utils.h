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

#ifndef __WIFI_UTILS_H__
#define __WIFI_UTILS_H__

#include <tinyara/config.h>

#include <mqueue.h>
#include <pthread.h>

#include <tinyara/wifi/wifi_common.h>

#ifdef CONFIG_LWNL80211
struct _wifi_utils_s {
	char mqname[16];
	pid_t cb_receiver;
};
#endif

/**
 * @brief wifi interface init
 *
 * @return WIFI_UTILS_SUCCESS       :  success
 * @return WIFI_UTILS_FAIL          :  fail
 */
wifi_utils_result_e wifi_utils_init(void);

/**
 * @brief wifi interface deinit
 *
 * @return WIFI_UTILS_SUCCESS       :  success
 * @return WIFI_UTILS_FAIL          :  fail
 */
wifi_utils_result_e wifi_utils_deinit(void);

/**
 * @brief scans access point list
 *
 * @param[in]   ap_list        :  pre declared wifi_utils_ap_scan_info_s array pointer
 * @param[in]   list_size      :  ap_list size (wifi_utils_ap_info array)
 * @param[out]  found_ap_count :  found ap count
 *
 * @return WIFI_UTILS_SUCCESS       :  success
 * @return WIFI_UTILS_FAIL          :  fail
 * @return WIFI_UTILS_INVALID_ARGS  :  input parameter invalid
 */
wifi_utils_result_e wifi_utils_scan_ap(void *arg);

/**
 * @brief wifi connect access point
 *
 * @param[in]   ap_connect_config  :  target ap connect config
 *
 * @return WIFI_UTILS_SUCCESS       :  success
 * @return WIFI_UTILS_FAIL          :  fail
 * @return WIFI_UTILS_INVALID_ARGS  :  input parameter invalid
 */
wifi_utils_result_e wifi_utils_connect_ap(wifi_utils_ap_config_s *ap_connect_config, void *arg);

/**
 * @brief wifi disconnect access point
 *
 * @return WIFI_UTILS_SUCCESS       :  success
 * @return WIFI_UTILS_FAIL          :  fail
 */
wifi_utils_result_e wifi_utils_disconnect_ap(void *arg);

/**
 * @brief get wifi information (IP address, MAC address)
 *
 * @param[out]  wifi_info      :  @ref wifi_utils_info_s
 *
 * @return WIFI_UTILS_SUCCESS       :  success
 * @return WIFI_UTILS_FAIL          :  fail
 * @return WIFI_UTILS_INVALID_ARGS  :  input parameter invalid
 */
wifi_utils_result_e wifi_utils_get_info(wifi_utils_info_s *wifi_info);

/**
 * @brief register wifi connection event callback (connect/disconnect event)
 *
 * @param[in]   cbk                 :  when an event received, function start
 *
 * @return WIFI_UTILS_SUCCESS       :  success
 * @return WIFI_UTILS_FAIL          :  fail
 * @return WIFI_UTILS_INVALID_ARGS  :  input parameter invalid
 */
wifi_utils_result_e wifi_utils_register_callback(wifi_utils_cb_s *cbk);

/**
 * @brief wifi start soft ap mode
 *
 * @param[in]   soft_ap_config :  @ref  wifi_utils_softap_config_s
 *
 * @return WIFI_UTILS_SUCCESS       :  success
 * @return WIFI_UTILS_FAIL          :  fail
 * @return WIFI_UTILS_INVALID_ARGS  :  input parameter invalid
 */
wifi_utils_result_e wifi_utils_start_softap(wifi_utils_softap_config_s *softap_config);

/**
 * @brief start wifi sta mode
 *
 * @return WIFI_UTILS_SUCCESS       :  success
 * @return WIFI_UTILS_FAIL          :  fail
 */
wifi_utils_result_e wifi_utils_start_sta(void);

/**
 * @brief stop wifi softap mode
 *
 * @return WIFI_UTILS_SUCCESS       :  success
 * @return WIFI_UTILS_FAIL          :  fail
 */
wifi_utils_result_e wifi_utils_stop_softap(void);

/**
 * @brief enable/disable external autoconnect
 *
 * @param[in]   check :  enable/disable external autoconnect
 *
 * @return WIFI_UTILS_SUCCESS       :  success
 * @return WIFI_UTILS_FAIL          :  fail
 */
wifi_utils_result_e wifi_utils_set_autoconnect(uint8_t check);

/**
 * @brief lwnl80211 driver callback handler
 *
 */
int wifi_utils_callback_handler(int argc, char *argv[]);

#endif /* __WIFI_UTILS_H__ */
