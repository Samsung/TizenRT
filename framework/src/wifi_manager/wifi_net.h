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

#ifndef WIFI_NET_H
#define WIFI_NET_H
#include "wifi_common.h"

/**
 * @file wifi_net.h
 * @brief network utility (dns, ntp) API
 */

/**
 * @brief host name to IP4 address
 *
 * @param[in]   hostname     :  host name string (ex - www.google.co.kr)
 * @param[out]  ip4_address  :  host ip4 address (32bit)
 *
 * @return WIFI_UTILS_SUCCESS       :  success
 * @return WIFI_UTILS_FAIL          :  fail
 * @return WIFI_UTILS_INVALID_ARGS  :  input parameter invalid
 */
wifi_utils_result_e wifi_utils_hostname_to_ip4(char *hostname, unsigned int *ip4_address);

/**
 * @brief convert ip4 address (32bit) to ip4 address string (123.123.123.123)
 *
 * @param[in]   ip4_addr  :  ip4 address 32bit
 * @param[out]  ip4_str   :  ip4 address string
 *
 * @return WIFI_UTILS_SUCCESS       :  success
 * @return WIFI_UTILS_FAIL          :  fail
 * @return WIFI_UTILS_INVALID_ARGS  :  input parameter invalid
 */
wifi_utils_result_e wifi_utils_ip4_addr_to_ip4_str(unsigned int ip4_addr, char ip4_str[18]);

/**
 * @brief convert mac address (48bit) to mac address string (FF:FF:FF:FF:FF:FF)
 *
 * @param[in]  mac_addr  :  mac address 48bit
 * @param[out] mac_str   :  mac address string
 *
 * @return WIFI_UTILS_SUCCESS       :  success
 * @return WIFI_UTILS_FAIL          :  fail
 * @return WIFI_UTILS_INVALID_ARGS  :  input parameter invalid
 */
wifi_utils_result_e wifi_utils_mac_addr_to_mac_str(unsigned char mac_addr[6], char mac_str[20]);

#endif //WIFI_NET_H
