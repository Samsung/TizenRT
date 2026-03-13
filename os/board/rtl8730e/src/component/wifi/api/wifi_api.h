/**
  ******************************************************************************
  * @file    wifi_api.h
  * @author
  * @version
  * @date
  * @brief   This file provides user interface for Wi-Fi station and AP mode configuration
  *             base on the functionalities provided by Realtek Wi-Fi driver.
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2024, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

#ifndef __WIFI_API_H
#define __WIFI_API_H

#include "wifi_api_types.h"
#include "wifi_api_event.h"
#include "wifi_api_ext.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup WIFI_API Wi-Fi APIs
 *  @brief      WIFI_API module
 *  @{
 */
/**********************************************************************************************
 *                                     Function Declarations
 *********************************************************************************************/
/** @addtogroup WIFI_Exported_Functions Wi-Fi Exported Functions
 * @{
 */
/** @addtogroup WIFI_Exported_Basic_Functions Basic Functions
  * @{
  */
/**
 * @brief  Enable Wi-Fi, i.e., bring the Wireless interface 0 "Up".
 * @param[in]  mode: Should always set to @ref RTW_MODE_STA.
 * @return @ref RTK_SUCCESS : The API executed successfully.
 * @note
 *    - Called by default in wifi_init.c.
 *    - For WHC host mode: Call whc_host_init() first to initialize the host-device interface.
 *    - For WHC device mode: Automatically called by host’s wifi_on(); no user action required.
 *    - Performs general initialization and sets interface 0 to STA mode.
 *    - To start AP mode, call wifi_start_ap() after this API.
 */
s32 wifi_on(u8 mode);

/**
 * @brief  Check if the specified wlan interface is running.
 * @param[in]  wlan_idx: Can be set as @ref STA_WLAN_INDEX (for STA mode) or @ref SOFTAP_WLAN_INDEX (for AP mode).
 * @return
 *        - 1: The interface is running.
 *        - 0: The interface is not running.
 */
s32 wifi_is_running(u8 wlan_idx);

/**
 * @brief  Join a Wi-Fi network by scanning, authenticating, and associating.
 * @param[in]  connect_param: Pointer to a struct containing connection info (SSID, BSSID, password, etc.).
 *                          See struct rtw_network_info in wifi_api_types.h for details.
 * @param[in]  block:
 *                  - 1: Synchronous Wi-Fi connect (function returns after connection attempt completes).
 *                  - 0: Asynchronous Wi-Fi connect (function returns immediately after initiating connection).
 * @return
 *    - @ref RTK_SUCCESS : Join successfully for synchronous Wi-Fi connect,
 *  						or connect cmd is set successfully for asynchronous Wi-Fi connect.
 *    - @ref RTK_FAIL : Driver internal error.
 *    - -@ref RTK_ERR_BUSY : Wi-Fi connect or scan is ongoing.
 *    - -@ref RTK_ERR_NOMEM : Memory malloc fail during Wi-Fi connect.
 *    - -@ref RTK_ERR_TIMEOUT : Connection attempt exceeded ~70 seconds.
 *    - -@ref RTK_ERR_BADARG : Invalid input parameters (e.g., both `connect_param->bssid` and `connect_param->ssid` not set).
 *    - -@ref RTK_ERR_WIFI_CONN_INVALID_KEY : Incorrect password format.
 *    - -@ref RTK_ERR_WIFI_CONN_SCAN_FAIL : Scan fail.
 *    - -@ref RTK_ERR_WIFI_CONN_AUTH_FAIL : Auth fail.
 *    - -@ref RTK_ERR_WIFI_CONN_AUTH_PASSWORD_WRONG : Password error causing auth failure, not entirely accurate.
 *    - -@ref RTK_ERR_WIFI_CONN_ASSOC_FAIL : Assoc fail.
 *    - -@ref RTK_ERR_WIFI_CONN_4WAY_HANDSHAKE_FAIL : 4 way handshake fail.
 *    - -@ref RTK_ERR_WIFI_CONN_4WAY_PASSWORD_WRONG : Password error causing 4 way handshake failure, not entirely accurate.
 * @note
 *      - Ensure Wi-Fi is enabled (wifi_on()) before calling this function.
 *      - If `connect_param->bssid` is set, it takes precedence over `connect_param->ssid` for connection.
 *      - Asynchronous Wi-Fi connect results are obtained through the event handler registered
 *        for @ref RTW_EVENT_JOIN_STATUS (see example_wifi_event.c).
 */
s32 wifi_connect(struct rtw_network_info *connect_param, u8 block);

/**
 * @brief  Disassociates from current Wi-Fi network.
 * @return
 *    - @ref RTK_SUCCESS : The API executed successfully.
 *    - @ref RTK_FAIL : Driver internal error.
 * @note  The return value @ref RTK_SUCCESS indicates that the disconnection request was successfully initiated,
 *        not that the disconnection has completed. The actual disconnection time may vary depending on the
 *        current Wi-Fi connection status. If this function is called while a Wi-Fi connection is in progress,
 *        the disconnection will occur after the ongoing connection attempt is completed.
 */
s32 wifi_disconnect(void);

/**
 * @brief  Get join status during Wi-Fi connectection.
 * @param[out] join_status the join status during Wi-Fi connectection.
 *    - @ref RTW_JOINSTATUS_UNKNOWN : Unknown.
 *    - @ref RTW_JOINSTATUS_STARTING : Connection initiation.
 *    - @ref RTW_JOINSTATUS_SCANNING : Network scanning.
 *    - @ref RTW_JOINSTATUS_AUTHENTICATING : Authentication in progress.
 *    - @ref RTW_JOINSTATUS_AUTHENTICATED : Authentication completed.
 *    - @ref RTW_JOINSTATUS_ASSOCIATING : Association in progress.
 *    - @ref RTW_JOINSTATUS_ASSOCIATED : Association completed.
 *    - @ref RTW_JOINSTATUS_4WAY_HANDSHAKING : 4-way handshake in progress.
 *    - @ref RTW_JOINSTATUS_4WAY_HANDSHAKE_DONE : 4-way handshake completed.
 *    - @ref RTW_JOINSTATUS_SUCCESS : Connection successful.
 *    - @ref RTW_JOINSTATUS_FAIL : Connection failed.
 *    - @ref RTW_JOINSTATUS_DISCONNECT : Disconnected after successful connection.
 * @return @ref RTK_SUCCESS : The API executed successfully.
 */
s32 wifi_get_join_status(u8 *join_status);

/**
 * @brief  Initiate a scan to search for 802.11 networks.
 * @param[in]  scan_param: Scan parameters. See struct rtw_scan_param in wifi_api_types.h for details.
 * @param[in]  block:
 *             - 1: Synchronous scan (function returns after scan completes).
 *             - 0: Asynchronous scan (function returns immediately).
 * @return  Number of scanned APs if successful (≥ 0), or an error code:
 *          - @ref RTK_FAIL : Driver internal error.
 *          - -@ref RTK_ERR_BUSY : Wi-Fi connect or scan is ongoing.
 *          - -@ref RTK_ERR_BADARG : Invalid input parameters, such as set `block` to 1 but simultaneously register
 *           `scan_param->scan_user_callback`.
 * @note
 * 1. Scan Types:
 *    - Synchronous scan: Calling thread waits until scan done;
 *    - Asynchronous scan: Function returns immediately after parameter check.
 * 2. Scan Result Reporting:
 *    1. Bulk Report:
 *       - Total number of scanned APs reported via `scan_param->scan_user_callback` (async) or return value (sync).
 *       - Detailed APs info gotten by calling wifi_get_scan_records().
 *       - For async scan, need to register `scan_param->scan_user_callback`.
 *    2. Individual Report:
 *       - Each scanned AP reported via `scan_param->scan_report_each_mode_user_callback`.
 *       - Scan completion indicated by NULL pointer in `scan_report_each_mode_user_callback`.
 *       - Need to register `scan_param->scan_user_callback` and set `scan_param->options` to @ref RTW_SCAN_REPORT_EACH.
 * 3. Memory Management:
 *    - Scanned APs info stored in driver's dynamic allocated memory.
 *    - For bulk report, memory freed when wifi_get_scan_records() is called.
 */
s32 wifi_scan_networks(struct rtw_scan_param *scan_param, u8 block);

/**
 * @brief  Get scan results
 * @param[inout]  ap_num: Pointer to the number of AP records:
 *                       - Input: Desired number of AP records;
 *                       - Output: Actual number of AP records retrieved.

 * @param[in]  ap_list: A buffer stores scanned AP list, each entry as struct rtw_scan_result.
 * @return
 *        - @ref RTK_SUCCESS : The API executed successfully.
 *        - @ref RTK_FAIL : Driver internal error.
 * @note
 *     - For scan which not config @ref RTW_SCAN_REPORT_EACH : If wifi_scan_networks() is called but this
 *       API is not used to get scanned results, driver memory for scanned APs will be retained until
 *       the next wifi_scan_networks() call.
 *     - For scan which config @ref RTW_SCAN_REPORT_EACH : Every time a AP is scanned, the AP info will be directly
 *       reported via `scan_report_each_mode_user_callback` and freed after callback execution, thus this function
 *       is not needed to get scan results.
 */
s32 wifi_get_scan_records(u32 *ap_num, struct rtw_scan_result *ap_list);

/**
 * @brief  Start an infrastructure Wi-Fi network (SoftAP).
 * @param[in]  softap_config: Pointer to a struct containing SoftAP configuration parameters.
 *                            See struct rtw_softap_info in wifi_api_types.h for details.
 * @return
 *    - @ref RTK_SUCCESS : The API executed successfully.
 *    - @ref RTK_FAIL : Driver internal error.
 *    - -@ref RTK_ERR_BADARG : SSID length exceed @ref RTW_ESSID_MAX_SIZE.
 *    - -@ref RTK_ERR_WIFI_CONN_INVALID_KEY : Invalid password format.
 *       - For non-open security: Password length must be between @ref RTW_MIN_PSK_LEN and
 *         @ref RTW_MAX_PSK_LEN;
 *       - For WEP security: Password length must be 5/10/13/26.
 * @note
 *     - If a STA interface already connected or connecting to an AP, the SoftAP will start
 *       on the same channel as the STA, not using the channel specified in the configuration.
 *     - Setting `softap_config->hidden_ssid` to 1 will start a hidden SSID SoftAP.
 *     - Ensure Wi-Fi is enabled (wifi_on()) before calling this function.
 */
s32 wifi_start_ap(struct rtw_softap_info *softap_config);

/**
 * @brief  Disable SoftAP (i.e. Wi-Fi interface-1).
 * @return @ref RTK_SUCCESS : The API executed successfully.
 */
s32 wifi_stop_ap(void);

/**
 * @brief  Get current Wi-Fi settings from driver.
 * @param[in]  wlan_idx: @ref STA_WLAN_INDEX or @ref SOFTAP_WLAN_INDEX.
 * @param[out]  psetting: Pointer to rtw_wifi_setting which stores the gotten information.
 * @return
 *    - @ref RTK_SUCCESS : The API executed successfully.
 *    - @ref RTK_FAIL : Driver internal error.
 *    - -@ref RTK_ERR_BADARG : Invalid input parameters, such as set `wlan_idx` to @ref SOFTAP_WLAN_INDEX but SoftAP is not enabled;
 * @note  The mode in struct rtw_wifi_setting corresponding to the Wi-Fi mode of the specified `wlan_idx`:
 *      - In station mode: Information corresponds to the connected AP.
 *      - In AP mode: Information corresponds to the SoftAP itself.
 */
s32 wifi_get_setting(u8 wlan_idx, struct rtw_wifi_setting *psetting);

/** @} End of Basic_Functions group */
/** @} End of WIFI_Exported_Functions group */
/** @} End of WIFI_API group */
#ifdef __cplusplus
}
#endif

#endif // __WIFI_API_H
