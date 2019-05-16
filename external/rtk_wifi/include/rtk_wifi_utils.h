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
  * @file    rtk_wifi_utils.h
  * @author
  * @version
  * @brief   This file provides user interface for Wi-Fi station and AP mode configuration 
  *             base on the functionalities provided by Realtek Wi-Fi driver.
  ******************************************************************************
  */
#ifndef __RTK_WIFI_UTILS_H
#define __RTK_WIFI_UTILS_H

#include "wifi_constants.h"
#include "wifi_structures.h"
#include <sys/types.h>


#define RTW_LWIP_LAYER 1
#define RTW_AUTO_RECONNECT 1

typedef rtw_result_t (*rtw_scan_result_handler_t)( rtw_scan_handler_result_t* malloced_scan_result );
extern rtw_result_t app_scan_result_handler( rtw_scan_handler_result_t* malloced_scan_result );

/**
 * @brief  Enable Wi-Fi.
 * - Bring the Wireless interface "Up"
 * - Initialize the driver thread which arbitrates access
 *   to the SDIO/SPI bus
 *
 * @param[in]  mode: Decide to enable WiFi in which mode. The optional modes are enumerated in @ref rtw_mode_t.
 * @return  RTW_SUCCESS: if the WiFi chip was initialized successfully.
 * @return  RTW_ERROR: if the WiFi chip was not initialized successfully.
 */
extern int wifi_on(rtw_mode_t mode);

/**
  * @brief  Set reconnection mode with 3 retry limit and 5 seconds timeout as default.
  * @param[in]  mode: Set 1/0 to enalbe/disable the reconnection mode.
  * @return  0 if success, otherwise return -1.
  * @note  Defining CONFIG_AUTO_RECONNECT in "autoconf.h" needs to be done before compiling,
  *			or this API won't be effective.
  * @note  The difference between @ref wifi_config_autoreconnect() and @ref wifi_set_autoreconnect() is that 
  *			user can specify the retry times and timeout value in @ref wifi_config_autoreconnect().
  *			But in @ref wifi_set_autoreconnect() these values are set with 3 retry limit and 5 seconds timeout as default.
  */
extern int wifi_set_autoreconnect(uint8_t mode);

/**
 * @brief  Get current Wi-Fi setting from driver.
 * @param[in]  ifname: the wlan interface name, can be WLAN0_NAME or WLAN1_NAME.
 * @param[out]  pSetting: Points to the rtw_wifi_setting_t structure to store the WIFI setting gotten from driver.
 * @return  RTW_SUCCESS or RTW_ERROR.
 */
extern int wifi_get_setting(const char *ifname,rtw_wifi_setting_t *pSetting);

/**
 * @brief  Show the network information stored in a rtw_wifi_setting_t structure.
 * @param[in]  ifname: the wlan interface name, can be WLAN0_NAME or WLAN1_NAME.
 * @param[in]  pSetting: Points to the rtw_wifi_setting_t structure which information is gotten by @ref wifi_get_setting().
 * @return  RTW_SUCCESS or RTW_ERROR.
 */
extern int wifi_show_setting(const char *ifname,rtw_wifi_setting_t *pSetting);

/**
 * @brief  Disable Wi-Fi.
 *  
 * @param  None
 * @return  RTW_SUCCESS: if deinitialization is successful.
 * @return  RTW_ERROR: otherwise.
 */
extern int wifi_off(void);

/**
 * @brief  Initiate a scan to search for 802.11 networks, a higher level API based on wifi_scan
 *			to simplify the scan operation.
 * @param[in]  results_handler: The callback function which will receive and process the result data.
 * @param[in]  user_data: User specified data that will be passed directly to the callback function.
 * @return  RTW_SUCCESS or RTW_ERROR
 * @note  Callback must not use blocking functions, since it is called from the context of the RTW thread. 
 *			The callback, user_data variables will be referenced after the function returns. 
 *			Those variables must remain valid until the scan is completed.
 *			The usage of this api can reference ATWS in atcmd_wifi.c.
 */
extern int wifi_scan_networks(rtw_scan_result_handler_t results_handler, void* user_data);

/**
 * @brief  Join a Wi-Fi network.
 * 		Scan for, associate and authenticate with a Wi-Fi network.
 *		On successful return, the system is ready to send data packets.
 *
 * @param[in]  ssid: A null terminated string containing the SSID name of the network to join.
 * @param[in]  security_type: Authentication type:
 *                         - RTW_SECURITY_OPEN           - Open Security
 *                         - RTW_SECURITY_WEP_PSK        - WEP Security with open authentication
 *                         - RTW_SECURITY_WEP_SHARED     - WEP Security with shared authentication
 *                         - RTW_SECURITY_WPA_TKIP_PSK   - WPA Security
 *                         - RTW_SECURITY_WPA2_AES_PSK   - WPA2 Security using AES cipher
 *                         - RTW_SECURITY_WPA2_TKIP_PSK  - WPA2 Security using TKIP cipher
 *                         - RTW_SECURITY_WPA2_MIXED_PSK - WPA2 Security using AES and/or TKIP ciphers
 * @param[in]  password: A byte array containing either the cleartext security key for WPA/WPA2
 *  						 secured networks, or a pointer to an array of rtw_wep_key_t
 *  						 structures for WEP secured networks.
 * @param[in]  ssid_len: The length of the SSID in bytes.
 * @param[in]  password_len: The length of the security_key in bytes.
 * @param[in]  key_id: The index of the wep key (0, 1, 2, or 3). If not using it, leave it with value -1.
 * @param[in]  semaphore: A user provided semaphore that is flagged when the join is complete. If not using it, leave it with NULL value.
 * @return  RTW_SUCCESS: when the system is joined and ready to send data packets.
 * @return  RTW_ERROR: if an error occurred.
 * @note  Please make sure the Wi-Fi is enabled before invoking this function. (@ref wifi_on())
 */
extern int wifi_connect(
	char 				*ssid,
	rtw_security_t	security_type,
	char 				*password,
	int 				ssid_len,
	int 				password_len,
	int 				key_id,
	void 				*semaphore);

extern int wext_get_ssid(const char *ifname, uint8_t *ssid);

/**
  * @brief  Disassociates from current Wi-Fi network.
  * @param  None
  * @return  RTW_SUCCESS: On successful disassociation from the AP.
  * @return  RTW_ERROR: If an error occurred.
  */
extern int wifi_disconnect(void);

/**
 * @brief  Retrieves the current Media Access Control (MAC) address
 *			(or Ethernet hardware address) of the 802.11 device.
 * @param[in]  mac: Point to the result of the mac address will be get.
 * @return    RTW_SUCCESS or RTW_ERROR
 */
extern int wifi_get_mac_address(char * mac);

/**
 * @brief  Get the SoftAP information.
 * @param[out]  ap_info: The location where the AP info will be stored.
 * @param[out]  security: The security type.
 * @return  RTW_SUCCESS: The result is successfully got.
 * @return  RTW_ERROR: The result is not successfully got.
 */
extern int wifi_get_ap_info(rtw_bss_info_t * ap_info, rtw_security_t* security);

/**
 * @brief  Trigger Wi-Fi driver to start an infrastructure Wi-Fi network.
 * @warning If a STA interface is active when this function is called, the softAP will
 *          start on the same channel as the STA. It will NOT use the channel provided!
 * @param[in]  ssid: A null terminated string containing the SSID name of the network.
 * @param[in]  security_type: 
 *                         - RTW_SECURITY_OPEN           - Open Security
 *                         - RTW_SECURITY_WPA_TKIP_PSK   - WPA Security
 *                         - RTW_SECURITY_WPA2_AES_PSK   - WPA2 Security using AES cipher
 *                         - RTW_SECURITY_WPA2_MIXED_PSK - WPA2 Security using AES and/or TKIP ciphers
 *                         - WEP security is NOT IMPLEMENTED. It is NOT SECURE!
 * @param[in]  password: A byte array containing the cleartext security key for the network.
 * @param[in]  ssid_len: The length of the SSID in bytes.
 * @param[in]  password_len: The length of the security_key in bytes.
 * @param[in]  channel: 802.11 channel number.
 * @return  RTW_SUCCESS: If successfully creates an AP.
 * @return  RTW_ERROR: If an error occurred.
 * @note  Please make sure the Wi-Fi is enabled before invoking this function. (@ref wifi_on())
 */
extern int wifi_start_ap(
	char 				*ssid,
	rtw_security_t		security_type,
	char 				*password,
	int 				ssid_len,
	int 				password_len,
	int					channel);

#endif

