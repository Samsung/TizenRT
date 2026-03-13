/**
  ******************************************************************************
  * @file    wifi_api_ext.h
  * @author
  * @version
  * @date
  * @brief   This file provides user interface for Wi-Fi station and AP mode
  *          configuration base on the functionalities provided by Realtek Wi-Fi driver.
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2024, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

#ifndef __WIFI_API_EXT_H
#define __WIFI_API_EXT_H

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup WIFI_API Wi-Fi APIs
 *  @brief      WIFI_API module
 *  @{
 */
/** @addtogroup WIFI_Exported_Functions Wi-Fi Exported Functions
 * @{
 */
/** @addtogroup WIFI_Exported_Basic_Functions Basic Functions
  * @{
  */
/******************************************************
 *               Function Declarations
 ******************************************************/

/**
 * @brief  Abort onoging Wi-Fi scan.
 * @return  @ref RTK_SUCCESS or @ref RTK_FAIL.
 * @note
 *	   - This will return when scan is actually aborted.
 * 	     When scan is actually aborted, the user callback registered in wifi_scan_networks()
 * 	     will be executed. If there is no Wi-Fi scan in progress, this function will just return
 * 	     @ref RTK_SUCCESS and user callback won't be executed.
 */
s32 wifi_scan_abort(void);

/**
 * @brief  Enable or disable Legacy Power Save (LPS) mode.
 *         In LPS mode, Wi-Fi automatically turns the RF off when the traffic between the STA and the
 *         connected AP is low and periodically turns it on to listen to the beacons from the associated AP.
 * @param[in]  enable: 1 to enable LPS, 0 to disable LPS.
 * @return  @ref RTK_SUCCESS : The API executed successfully.
 * @note
 *      - This function does not work if a SoftAP interface is active.
 *      - STA does not immediately enter LPS; entry depends on current traffic status.
 *      - This function temporarily overrides the default LPS setting in ameba_wificfg.c
 *        (wifi_user_config.lps_enable).
 */
s32 wifi_set_lps_enable(u8 enable);

/**
 * @brief  Set the listen interval for Legacy Power Save (LPS) mode. The listen interval dictates
 *         how frequently the STA wakes up to listen to AP beacons when in LPS mode.
 * @param[in]  interval: The listen interval for LPS, in units of 102.4 ms.
 * @return  @ref RTK_SUCCESS : The API executed successfully.
 * @note
 *      - If interval is not a multiple of the beacon interval, it's rounded down to
 *        the largest multiple <= specified value.
 *      - If interval < beacon interval, it's set to one beacon interval.
 *      - For example, if the beacon interval is 2 * 102.4 ms:
 *        - wifi_set_lps_listen_interval(5) sets interval to 4 * 102.4 ms
 *        - wifi_set_lps_listen_interval(1) sets interval to 2 * 102.4 ms
 */
s32 wifi_set_lps_listen_interval(u8 interval);

/**
 * @brief   Enable or disable manual configuration of rxbcn window under LPS.
 * @param[in]  enable: It could be TRUE or FALSE.
 * 	FALSE(default) means rxbcn window automatically decided by IC; TRUE means rxbcn window set by this api parameters.
 * @param[in] bcn_ely_time(unit: 128us):set the rx bcn wakpup-ely-time before TBTT.
 * @param[in] rx_bcn_timeout(unit: 1 ms): If no bcn is received within rx_bcn_timeout, RF will be turned off. Max to 31ms.
 * @return  RTW_SUCCESS if setting successful.
 * @return  RTW_ERROR otherwise.
 */
s32 wifi_set_lps_bcn_window(u8 enable, u8 bcn_ely_time, u8 rx_bcn_timeout);

/**
 * @brief   Enable or disable rx broadcast in tickless wowlan mode.
 * @param[in]  enable: FALSE means disable rx broadcast in tickless wowlan mode, TRUE means enable(default) rx broadcast in tickless wowlan mode.
 * @return
 *    - @ref RTK_SUCCESS : The API executed successfully.
 */
s32 wifi_set_wowlan_rx_broadcast(u8 enable);

/**
 * @brief Set up custom TCP/UDP broadcast port filter white list for wifi wake application core under tickless state
 * @warning There are up to 6 port numbers.
 * @param[in] port_list: port list need to add in port filter white list
 * @param[in] list_count: number of port list
 * @code
 * u16 port_list[] = {1234, 2234, 3234, 4234};
 * wifi_set_broadcast_port_wakeup_white_list(port_list, 4);
 * @endcode
 * @return
 *    - @ref RTK_SUCCESS : The API executed successfully.
 *    - @ref RTK_FAIL : The API executed fail.
 *    - @ref RTK_ERR_WIFI_NOT_INIT : wifi not initial
 *    - @ref RTK_ERR_WIFI_POWEROFF : Wi-Fi is powered off in IPS(Inactive Power Save) mode.
 * @note
 *    - If the port of TCP/UDP broadcast packet not match port_list, drop the packet and not wakeup host
 *    - If this API is not set, all ports will not be filtered
 */
s32 wifi_set_broadcast_port_wakeup_white_list(u16 *port_list, u8 list_count);

/**
 * @brief  Set the auto-reconnect mode for Wi-Fi connection.
 * This function allows temporarily enabling or disabling auto-reconnect mode,
 * overriding the default setting in ameba_wificfg.c.
 * @param[in]  enable:
 *                 - 1: Enable auto-reconnect mode.
 *                 - 0: Disable auto-reconnect mode.
 * @return
 *    - @ref RTK_SUCCESS : The API executed successfully.
 *    - @ref RTK_FAIL : CONFIG_AUTO_RECONNECT in not defined or creating reconnect timer failed.
 */
s32 wifi_set_autoreconnect(u8 enable);

/**
 * @brief  Get current auto-reconnect setting which can be used to backup the
 * setting before temporarily changing it.
 * @param[out]  enable: Pointer to store auto-reconnect mode.
 *                 - 1: Enable auto-reconnect mode.
 *                 - 0: Disable auto-reconnect mode.
 * @return
 *    - @ref RTK_SUCCESS : The API executed successfully.
 *    - @ref RTK_FAIL : CONFIG_AUTO_RECONNECT not defined.
 */
s32 wifi_get_autoreconnect(u8 *enable);

/**
 * @brief  Get the list of clients connected to the SoftAP.
 * @param[out]  client_list_buffer: Pointer to store the number of connected clients and their MAC addresses.
 * @return
 *    - @ref RTK_SUCCESS : The API executed successfully.
 *    - @ref RTK_FAIL : SoftAP is not enabled.
 *    - -@ref RTK_ERR_BADARG : NULL pointer passed for `client_list_buffer`.
 */
s32 wifi_ap_get_connected_clients(struct rtw_client_list	*client_list_buffer);

/**
 * @brief  Disconnect a client from the SoftAP.
 * @param[in]  hwaddr: Pointer to the MAC address of the client to be disconnected.
 * @return
 *    - @ref RTK_SUCCESS : The API executed successfully.
 *    - @ref RTK_FAIL : SoftAP is not enabled or no clients are currently connected to SoftAP.
 *    - -@ref RTK_ERR_BADARG :
 *      - `hwaddr` is brodcast MAC address (ff:ff:ff:ff:ff:ff).
 *      - The client with the specified MAC address `hwaddr` not connected to the SoftAP.
 */
s32 wifi_ap_del_client(u8 *hwaddr);

/**
 * @brief  Change the operating channel in AP mode and notify connected clients using CSA (Channel Switch Announcement).
 * @param[in]  csa_param: Pointer to the CSA configuration structure rtw_csa_parm.
 * @return
 *    - @ref RTK_SUCCESS : The API executed successfully.
 *    - @ref RTK_FAIL : SoftAP is not enabled.
 *    - -@ref RTK_ERR_BADARG : The specified channel `csa_param->new_chl` is invalid.
 */
s32 wifi_ap_switch_chl_and_inform(struct rtw_csa_parm *csa_param);

/**
  * @brief  Toggle whether SoftAP can be discovered.
  * @param[in]  enable: 1-invisible, 0-visible.
  * @return  None
  * @note
  *    - SoftAP becomes invisible by pausing tx beacon and not responsing to probe request.
  *    - Should be deauth all associted STAs[ref wifi_ap_del_client()] when set ap invisible.
  */
void wifi_ap_set_invisible(u8 enable);

/**
 * @brief  Set the operating channel for a Wi-Fi interface.
 * @param[in]  wlan_idx: @ref SOFTAP_WLAN_INDEX or @ref STA_WLAN_INDEX.
 * @param[in]  channel: RF channel to switch to.
 * @return
 *    - @ref RTK_SUCCESS : The API executed successfully.
 *    - @ref RTK_FAIL : Driver internal error.
 *    - -@ref RTK_ERR_BADARG : Invalid `channel` or `wlan_idx`(when set to @ref SOFTAP_WLAN_INDEX but SoftAP is not enabled).
 *    - -@ref RTK_ERR_BUSY : Wi-Fi scan is ongoing.
 * @note
 *    - There are two channel concepts, generally consistent:
 *      1. The actual operating channel of the interface.
 *      2. The channel declared in the beacon of the connected AP on interface(0) or SoftAP on interface(1).
 *    - Recommended to call this function before the STA is connected or the SoftAP is started, beacuse:
 *      - Changing STA channel during/after connection may cause connection failure or disconnection due to
 *        channel inconsistencies with the AP.
 *      - Changing SoftAP channel after start may disrupt client connections due to beacon channel mismatch.
 *        **For SoftAP channel switching, use wifi_ap_switch_chl_and_inform() instead.**
 *    - When MCC (Multi-Channel Concurrent) feature is not enabled, both SoftAP (if enabled) and STA use the same operating channel:
 *      - Changing SoftAP channel while the STA is connected will have no effect.
 *      - Changing STA channel will also change SoftAP channel.
 */
s32 wifi_set_channel(u8 wlan_idx, u8 channel);

/**
 * @brief  Set the country code for Wi-Fi regulation.
 * @param[in]  cntcode:  Pointer to the country code to be set.
 * @return
 *    - @ref RTK_SUCCESS : The API executed successfully.
 *    - -@ref RTK_ERR_BADARG : Invalid country code. Refer to ameba_wifi_country_code_table_usrcfg.c for valid codes.
 */
s32 wifi_set_countrycode(u8 *cntcode);

/**
 * @brief  Retrieve current country code information.
 * @param[in]  table: Pointer to store the country code table containing country code,
 *                    channel plan code, and TX power limit index.
 * @return  @ref RTK_SUCCESS : The API executed successfully.
 */
s32 wifi_get_countrycode(struct rtw_country_code_table *table);

/**
 * @brief  Get the list of available WiFi channels for the current regulatory domain.
 * @param[in]  ch_list: Pointer to store the available channel list.
 * @return
 *    - @ref RTK_SUCCESS : The API executed successfully.
 *    - -@ref RTK_ERR_BADARG : NULL pointer passed for `ch_list`.
 */
s32 wifi_get_channel_list(struct rtw_channel_list *ch_list);

/**
 * @brief  Retrieves the current Media Access Control (MAC) address
 *	(or Ethernet hardware address) of the 802.11 device.
 * @param[in]  idx: Index to specify STA or SoftAP MAC address (ignored if `efuse = 1`).
 * @param[in]  mac: Pointer to store the obtained MAC address.
 * @param[in]  efuse: 1 to read from EFUSE, 0 to read from RAM (RECOMMENDED).
 * @return
 *    - @ref RTK_SUCCESS : The API executed successfully.
 *    - @ref RTK_FAIL : Driver internal error.
 *    - -@ref RTK_ERR_BADARG : NULL pointer passed for `mac`.
 * @note  Reading the runtime MAC address from RAM (`efuse = 0`) is recommended for most use cases.
 */
s32 wifi_get_mac_address(s32 idx, struct rtw_mac *mac, u8 efuse);

/**
 * @brief  Get the current Wi-Fi wireless mode for station mode when connected to an AP.
 * @param[out]  wmode: Pointer to store the wireless mode (e.g., @ref RTW_80211_B, @ref RTW_80211_A, etc.).
 * @return
 *    - @ref RTK_SUCCESS : The API executed successfully.
 *    - @ref RTK_FAIL : STA not connected to AP.
 *    - -@ref RTK_ERR_BADARG : NULL pointer passed for `wmode`.
 */
s32 wifi_get_wireless_mode(u8 *wmode);

/**
 * @brief  Set the wireless mode.
 * @param[in]  wmode: Wireless mode to set. Valid values include:
 *               	- @ref RTW_80211_B
 *               	- @ref RTW_80211_A
 *               	- @ref RTW_80211_G
 *               	- @ref RTW_80211_N
 *               	- @ref RTW_80211_AC
 *               	- @ref RTW_80211_AX
 *               	- @ref RTW_80211_BG
 *               	- @ref RTW_80211_GN
 *               	- @ref RTW_80211_AN
 *               	- @ref RTW_80211_BN
 *               	- @ref RTW_80211_BGN
 *               	- @ref RTW_80211_BGAX
 *               	- @ref RTW_80211_GAX
 *               	- @ref RTW_80211_A_AC
 *               	- @ref RTW_80211_A_AX
 *               	- @ref RTW_80211_AGN
 *               	- @ref RTW_80211_ABGN
 *               	- @ref RTW_80211_ANAC
 *               	- @ref RTW_80211_24G_MIX
 *               	- @ref RTW_80211_5G_MIX
 *               	- @ref RTW_80211_MAX
 * @return
 *    - @ref RTK_SUCCESS : The API executed successfully.
 *    - -@ref RTK_ERR_BADARG : Invalid `wmode` provided.
 * @note
 *    - Default mode after driver initialization:
 *      - Dual-band device: @ref RTW_80211_MAX (i.e. @ref RTW_80211_24G_MIX (BGNAX) and @ref RTW_80211_5G_MIX (ANACAX)).
 *      - Single-band device (i.e. Amebalite): @ref RTW_80211_24G_MIX for 2.4GHz only.
 *    - Use this function to change wireless mode for station mode before connecting to AP.
 *    - Avoid using 2.4GHz modes without 11b or 5GHz modes without 11a to prevent compatibility issues.
 */
s32 wifi_set_wireless_mode(u32 wmode);

/** @} End of Basic_Functions group */


/** @addtogroup WIFI_Exported_Extended_Functions Extended Functions
  * @{
  */

/**
 * @brief  Enable or disable promisc mode.
 * @param[in] enable: 1 to enable, 0 to disable promisc mode.
 * @param[in] para: Pointer to a struct containing filter mode and callback if enable promisc mode.
 *                 - Filter mode: Receive all packets unconditionally or only packets from the connected AP.
 *                 - Callback: Provides details of the received packets. The return value of the callback determines
 *                   whether the driver should continue processing the packet.
 * @note
 *        - Do not support calling APIs in callback.
 *        - Enabling promisc mode temporarily disables LPS(Legacy Power Save) and IPS(Inactive Power Save).
 *        - Original power save settings are restored when promisc mode is disabled.
 *        - Promiscuous mode can coexist with STA mode and SoftAP mode. After packets are processed by the application layer, the Wi-Fi driver layer can proceed with further processing.
 *        - Promiscuous mode can report Data Frames and Management Frames, but Control Frames are currently not reported.
 * @return  None.
 */
void wifi_promisc_enable(u32 enable, struct rtw_promisc_para *para);

/**
 * @brief  Check if the Wi-Fi driver is in Mass Production (MP) mode.
 * @param[out]  is_mp: 1 indicate driver is MP mode, 0 indicate driver is not MP mode.
 * @note  MP mode is used for Wi-Fi & Bluetooth performance verification and parameter calibration during mass production.
 * @return
 *    - @ref RTK_SUCCESS : The API executed successfully.
 */
s32 wifi_driver_is_mp(u8 *is_mp);

/**
 * @brief  Get CCMP unicast and group keys.
 * @param[in]  wlan_idx: @ref STA_WLAN_INDEX or @ref SOFTAP_WLAN_INDEX.
 * @param[in]  mac_addr: Pointer to client MAC address for SoftAP mode, NULL for STA mdoe.
 * @param[out]  uncst_key: Buffer to store CCMP unicast key.
 * @param[out]  group_key: Buffer to store CCMP group key.
 * @return
 *    - @ref RTK_SUCCESS : The API executed successfully.
 *    - @ref RTK_FAIL : Pairwise encryption cipher is not CCMP between STA and connected AP or SoftAP.
 *    - -@ref RTK_ERR_BADARG :
 *      - NULL pointer passed for `uncst_key` or `group_key`.
 *      - `wlan_idx` set to @ref SOFTAP_WLAN_INDEX but NULL pointer passed for `mac_addr`.
 *      - `wlan_idx` set to @ref STA_WLAN_INDEX but STA not connected to AP.
 */
s32 wifi_get_ccmp_key(u8 wlan_idx, u8 *mac_addr, u8 *uncst_key, u8 *group_key);

/**
 * @brief  Fetch the traffic statistics for the specified Wi-Fi interface.
 * @param[in]  wlan_idx: @ref STA_WLAN_INDEX or @ref SOFTAP_WLAN_INDEX.
 * @param[in]  traffic_stats: Pointer to union rtw_traffic_stats to store the statistics.
 * @return
 *    - @ref RTK_SUCCESS : The API executed successfully.
 *    - -@ref RTK_ERR_BADARG :
 *      - `wlan_idx` set to @ref SOFTAP_WLAN_INDEX but SoftAP is not enabled.
 *      - NULL pointer passed for `traffic_stats`.
 */
s32 wifi_get_traffic_stats(u8 wlan_idx, union rtw_traffic_stats *traffic_stats);

/**
 * @brief  Fetch Wi-Fi related physical layer information.
 * @param[in]  wlan_idx: The Wi-Fi interface:
 *                      - @ref STA_WLAN_INDEX or @ref SOFTAP_WLAN_INDEX : Get RSSI for the interface.
 *                      - @ref NONE_WLAN_INDEX : Get information that is common across all interfaces,
 *                        such as channel loading measurement.
 * @param[in]  mac_addr: Client MAC address for SoftAP mode, NULL for STA and NONE mode.
 * @param[out]  phy_stats: Pointer to union rtw_phy_stats to store the information.
 * @return
 *    - @ref RTK_SUCCESS : The API executed successfully.
 *    - -@ref RTK_ERR_BADARG :
 *      - NULL pointer passed for `traffic_stats`.
 *      - Set `wlan_idx` to @ref SOFTAP_WLAN_INDEX but SoftAP is not enabled.
 *      - Set `wlan_idx` to @ref SOFTAP_WLAN_INDEX but the STA with `mac_addr` not connected to SoftAP.
 * @note  For STA mode, all fields in `phy_stats->sta` will be 0 if the STA is not successfully connected to an AP.
 */
s32 wifi_get_phy_stats(u8 wlan_idx, u8 *mac_addr, union rtw_phy_stats *phy_stats);

/**
 * @brief  Get the number of packets remaining in the hardware TX buffer.
 * @param[out]  pkt_num: Pointer to store the number of packets.
 * @return
 *    - @ref RTK_SUCCESS : The API executed successfully.
 *    - @ref RTK_FAIL : Unsupported chip.
 *    - -@ref RTK_ERR_WIFI_POWEROFF : Wi-Fi is powered off in IPS(Inactive Power Save) mode, unable to access Wi-Fi registers.
 * @note Supported only by RTL8730E chip. Returns @ref RTK_FAIL for other chips.
 */
s32 wifi_get_txbuf_pkt_num(s32 *pkt_num);

/**
 * @brief  Get antenna diversity infomation.
 * @param[out]  antdiv_mode: Pointer to store the antenna diversity mode.
 *                           Values: @ref RTW_ANTDIV_AUTO, @ref RTW_ANTDIV_FIX_MAIN, etc.
 * @param[out]  curr_ant: Pointer to store the current antenna in use.
 *                        0: Main antenna, 1: Auxiliary antenna.
 * @return
 *    - @ref RTK_SUCCESS : The API executed successfully.
 *    - @ref RTK_FAIL : CONFIG_WIFI_ANTDIV is not defined.
 *    - -@ref RTK_ERR_BADARG : NULL pointer passed for `antdiv_mode` or `curr_ant`.
 */
s32 wifi_get_antdiv_info(u8 *antdiv_mode, u8 *curr_ant);

/**
 * @brief Get the supported frequency band type.
 * @param[out]  band_type: Pointer to store the supported band type. Values:
 *                       - @ref RTW_SUPPORT_BAND_2_4G : 2.4GHz only
 *                       - @ref RTW_SUPPORT_BAND_5G : 5GHz only
 *                       - @ref RTW_SUPPORT_BAND_2_4G_5G_BOTH : Both 2.4GHz and 5GHz
 * @return  @ref RTK_SUCCESS : The API executed successfully.
 */
s32 wifi_get_band_type(u8 *band_type);

/**
 * @brief	Get Wi-Fi TSF register value (64-bit).
 * @param[in]	wlan_idx: @ref STA_WLAN_INDEX or @ref SOFTAP_WLAN_INDEX.
 * @param[out] tsf: Pointer to store the 64-bit TSF value.
 * @return
 *    - @ref RTK_SUCCESS : The API executed successfully.
 *    - -@ref RTK_ERR_WIFI_POWEROFF : Wi-Fi is powered off in IPS(Inactive Power Save) mode, unable to access Wi-Fi registers.
 */
s32 wifi_get_tsf(u8 wlan_idx, u64 *tsf);

/**
 * @brief	Get latest latched WIFI TSF us(64-bit), WIFI TSF ns(10-bit), I2S counter(64-bit).
 * @param[in] req: Pointer to the struc for the request parameters.
 * @param[out] rpt: Pointer to the struc to store the latched values.
 * @return
 *    - @ref RTK_SUCCESS : The API executed successfully.
 *    - @ref RTK_FAIL : Unsupported chip.
 *    - -@ref RTK_ERR_WIFI_POWEROFF : Wi-Fi is powered off in IPS(Inactive Power Save) mode, unable to access Wi-Fi registers.
 *    - -@ref RTK_ERR_BADARG : NULL pointer passed.
 */
s32 wifi_get_latched_tsf_i2s(struct rtw_speaker_read_latch_req *req, struct rtw_speaker_read_latch_rpt *rpt);

/**
 * @brief  Set up custom Information Element (IE) list.
 * @warning  This API cannot be called twice without deleting the previous custom IE list.
 * @param[in]  ie_list: Buffer containing custom IE list. Each element should be of type struct rtw_custom_ie.
 * @param[in]  ie_num: Number of custom IEs in `ie_list`.
 * @code
 *  u8 ie1[] = {221, 2, 2, 2};
 *  u8 ie2[] = {221, 2, 1, 1};
 *  struct rtw_custom_ie ie_list[2] = {
 *	  {ie1, RTW_CUS_IE_BEACON|RTW_CUS_IE_PROBERSP},
 *    {ie2, RTW_CUS_IE_PROBERSP}
 *  };
 *  wifi_add_custom_ie(ie_list, 2);
 * @endcode
 * @return
 *    - @ref RTK_SUCCESS : The API executed successfully.
 *    - @ref RTK_FAIL : Previous custom IE list not deleted.
 *    - -@ref RTK_ERR_BADARG : NULL pointer passed for `ie_list` or incompatible frame types specified
 *      (e.g. set @ref RTW_CUS_IE_BEACON | @ref RTW_CUS_IE_PROBEREQ to `ie_list->type`).
 * @note The Wi-Fi interface is determined by the sepecified frame type in `ie_list->type`:
 *       - For SoftAP: Use @ref RTW_CUS_IE_BEACON or @ref RTW_CUS_IE_PROBERSP.
 *       - For STA: Use @ref RTW_CUS_IE_PROBEREQ or @ref RTW_CUS_IE_ASSOCREQ.
 */
s32 wifi_add_custom_ie(struct rtw_custom_ie *ie_list, s32 ie_num);

/**
 * @brief  Update an item in Wi-Fi custom Information Element (IE) list.
 * @param[in]  cus_ie: Pointer to the struct containing details of the IE to be updated.
 * @param[in]  ie_index: Index of the IE to be updated in the custom IE list.
 * @code
 *  u8 ie[] = {221, 2, 1, 3} ;
 *  struct rtw_custom_ie ie_update = {ie, RTW_CUS_IE_PROBERSP};
 *  wifi_update_custom_ie(&ie_update, 2);
 * @endcode
 * @return
 *    - @ref RTK_SUCCESS : The API executed successfully.
 *    - -@ref RTK_ERR_BADARG :
 *      - NULL pointer passed for `cus_ie`.
 *      - Incompatible frame types specified (e.g. set @ref RTW_CUS_IE_BEACON | @ref RTW_CUS_IE_PROBEREQ to `cus_ie->type`).
 *      - The IE at `ie_index` does not exist in the list.
 * @note  Ensure the IE is added by wifi_add_custom_ie() before calling this function.
 */
s32 wifi_update_custom_ie(struct rtw_custom_ie *cus_ie, s32 ie_index);

/**
 * @brief  Delete Wi-Fi custom Information Element (IE) list.
 * @param[in]  wlan_idx: @ref STA_WLAN_INDEX or @ref SOFTAP_WLAN_INDEX.
 * @return
 *    - @ref RTK_SUCCESS : The API executed successfully.
 *    - @ref RTK_FAIL : No IE list has been added before.
 */
s32 wifi_del_custom_ie(u8 wlan_idx);

/**
 * @brief  Send raw frame without upper layer protocol processing or encapsulation.
 * @param[in]  raw_frame_desc: Pointer of struct rtw_raw_frame_desc, describing raw frame
 *             details such as raw data pointer and transmission rate.
 * @return
 *    - @ref RTK_SUCCESS : The API executed successfully.
 *    - -@ref RTK_ERR_WIFI_TX_BUF_FULL : HW & SW tx buffer full, please wait for a while.
 *    - -@ref RTK_ERR_BUFFER_OVERFLOW : The packet length exceeds the SW per buf size.
 *    - -@ref RTK_ERR_BADARG : NULL pointer passed for `raw_frame_desc`.
 * @note  For unassociated peer devices in RX mode, only unencrypted frames are currently supported.
 */
s32 wifi_send_raw_frame(struct rtw_raw_frame_desc *raw_frame_desc);

/**
 * @brief  Control initial tx rate based on ToS value in IP header.
 * @param[in]  enable: Set to 1 to enable, 0 to disable initial rate control for this ToS value.
 * @param[in]  tos_precedence: IP precedence in TOS field (bits 7~5), range 0~7.
 * @param[in]  tx_rate: Initial tx rate for packets which with matching ToS. Values: @ref RTW_RATE_1M, @ref RTW_RATE_2M, etc.
 * @return  @ref RTK_SUCCESS : The API executed successfully.
 * @note
 *      - Only considers IP Precedence (bits 7~5 in ToS field).
 *      - Applies to both SoftAP (if enabled) and STA modes.
 */
s32 wifi_set_tx_rate_by_tos(u8 enable, u8 tos_precedence, u8 tx_rate);

/**
 * @brief  Set EDCA parameters for STA/SOFTAP. Suggest to call API when wifi connected for STA mode, e,g., in RTW_EVENT_JOIN_STATUS event handler.
 * @param[in]  pedca_param: EDCA parameters {ACI, AIFSN, CWmax, CWmin, TXOP, SlotTime} (as WMM_Specification_1.1 table 13):
 * @code
 * struct rtw_edca_param edca_param_0 = {0, 7, 0xa, 0x4, 0x0, 0};
 * // BE Queue, AIFSN=7, CWmax=10, CWmin=4, TXOP=0, SlotTime default
 * (short slot time 9us, long slot time 20us)
 * struct rtw_edca_param edca_param_1 = {1, 3, 0xa, 0x4, 0x0, 0};
 * // BK Queue, AIFSN=3, CWmax=10, CWmin=4, TXOP=0, SlotTime default
 * (short slot time 9us, long slot time 20us)
 * struct rtw_edca_param edca_param_2 = {2, 2, 0x4, 0x3, 0x5e, 0};
 * // VI Queue, AIFSN=2, CWmax=4, CWmin=3, TXOP=3.008ms, SlotTime default
 * (short slot time 9us, long slot time 20us)
 * struct rtw_edca_param edca_param_3 = {3, 2, 0x3, 0x2, 0x2f, 0};
 * // VO Queue, AIFSN=2, CWmax=3, CWmin=2, TXOP=1.504ms, SlotTime default
 * (short slot time 9us, long slot time 20us)
 * wifi_set_edca_param(&edca_param_0);
 * wifi_set_edca_param(&edca_param_1);
 * wifi_set_edca_param(&edca_param_2);
 * wifi_set_edca_param(&edca_param_3);
 * @endcode
 * @return
 *    - @ref RTK_SUCCESS : The API executed successfully.
 *    - -@ref RTK_ERR_WIFI_POWEROFF : Wi-Fi is powered off in IPS(Inactive Power Save) mode,
 *                      unable to access Wi-Fi registers.
 */
s32 wifi_set_edca_param(struct rtw_edca_param *pedca_param);

/**
 * @brief  Enable or disable CCA/EDCCA for TX.
 * @param[in]  enable: 1 to enable, 0 to disable.
 * @return
 *    - @ref RTK_SUCCESS : The API executed successfully.
 *    - -@ref RTK_ERR_WIFI_POWEROFF : Wi-Fi is powered off in IPS(Inactive Power Save) mode,
 *                      unable to access Wi-Fi registers.
 * @note
 *      - Both CCA and EDCCA will be simultaneously enabled or disabled.
 *      - Applies to both SoftAP (if enabled) and STA modes.
 */
s32 wifi_set_tx_cca_enable(u8 enable);

/**
 * @brief  Set duration by sending a CTS2SELF frame.
 * @param[in]  wlan_idx: @ref STA_WLAN_INDEX or @ref SOFTAP_WLAN_INDEX.
 * @param[in]  duration: Duration value for the CTS2SELF frame.
 * @return
 *    - @ref RTK_SUCCESS : The API executed successfully.
 *    - @ref RTK_FAIL : Driver internal error.
 */
s32 wifi_set_cts2self_duration_and_send(u8 wlan_idx, u16 duration);

/**
 * @brief   Configure and control the CSI (Channel State Information) functionality for STA or SoftAP.
 * @param[in]  act_param: Pointer to the configuration parameters.
 *                      - Set `act_param->act` to @ref RTW_CSI_ACT_CFG to configure CSI parameters.
 *                      - Set `act_param->act` to @ref RTW_CSI_ACT_EN to enable/disable CSI.
 *                        If disabled, reconfigure parameters before re-enabling.
 * @code
 *    // config csi parameters and enable wifi csi
 *    struct rtw_csi_action_parm act_param = {0};
 *    act_param.mode = RTW_CSI_MODE_RX_RESP;
 *    // ... (set other parameters)
 *    act_param.act = RTW_CSI_ACT_CFG;  //csi cfg
 *    wifi_csi_config(&act_param);
 *    act_param.act = RTW_CSI_ACT_EN;  //csi en
 *    act_param.enable = 1;
 *    wifi_csi_config(&act_param);
 * @endcode
 * @return
 *    - @ref RTK_SUCCESS : The API executed successfully.
 *    - @ref RTK_FAIL : CONFIG_CSI is not defined or driver internal error.
 *    - -@ref RTK_ERR_BADARG :
 *      - Invalid `act_param->mac_addr`, such as not set `mac_addr` when not use active CSI broadcast mode
 *        or the specified STA with `mac_addr` not connected the SoftAP when SoftAP enabled.
 *      - Invalid `act_param->data_rate`, such as use non-OFDM/HT rate or set `act_param->data_bw = 1`(i.e. 40M) while use OFDM rate.
 *      - SoftAP is not enabled, nor is the STA connected to an AP.
 * @note
 *     - If the SoftAP is enabled, Wi-Fi CSI function will only be initiated by the SoftAP.
 *     - 40MHz bandwidth (`act_param->data_bw = 1`) is supported only on RTL8721Dx chip.
 */
s32 wifi_csi_config(struct rtw_csi_action_parm *act_param);

/**
 * @brief  Configure Wi-Fi speaker settings for audio module.
 * @param[in]  set_type: Wi-Fi speaker setting type:
 *                - @ref RTW_SPEAKER_SET_INIT
 *                - @ref RTW_SPEAKER_SET_LATCH_I2S_COUNT
 *                - @ref RTW_SPEAKER_SET_TSF_TIMER
 * @param[in]  settings: A pointer to specific parameters.
 * @return  None.
 */
void wifi_speaker_setting(u8 set_type, union rtw_speaker_set *settings);

/**
 * @brief  Set Wi-Fi tx power.
 * @param[in]  txpwr_ctrl_info: Pointer to rtw_tx_power_ctl_info:
 *                            - \b b_tx_pwr_force_enbale: 1 to enable, 0 to disable.
 *                            - \b tx_pwr_force: Tx power to set (unit: 0.25 dBm).
 * @note
 *    - Recommended power ranges:
 *      - For Amebadplus, actual power range varies by channel and specific hardware (slight hardware differences
 *        between boards can affect the tx power), the recommended range is approximately -2 to 23 dBm.
 *      - For Amebasmart & Amebalite, the recommended range is -24 to 24 dBm.
 *    - Values outside these ranges may be inaccurate and will be adjusted to the nearest boundary value.
 *    - Not advised to exceed the tx power values defined in the rate table in `ameba_wifi_power_table_usrcfg.c`.
 *    - This setting overrides power by rate and power by limit for all rates.
 * @return  @ref RTK_SUCCESS : The API executed successfully.
 */
s32 wifi_set_tx_power(struct rtw_tx_power_ctl_info *txpwr_ctrl_info);

/**
 * @brief  Get current Wi-Fi tx power for a specific rate.
 * @param[in]  rate: The PHY rate (e.g., RTW_RATE_1M, RTW_RATE_2M, etc.):
 *                 - CCK rates: 1M, 2M, 5.5M, 11M
 *                 - OFDM rates: 6M, 9M, 12M, 18M, 24M, 36M, 48M, 54M
 *                 - HT rates: MCS0 to MCS7
 *                 - VHT rates: MCS0 to MCS8
 *                 - HE rates: MCS0 to MCS9
 * @param[out]  txpwr: Current tx power (unit: 0.25 dBm).
 * @note Retrieved the power set by wifi_set_tx_power() or the minimum of
 *       power by rate and regulatory power limit.
 * @return @ref RTK_SUCCESS : The API executed successfully.
 */
s32 wifi_get_tx_power(u8 rate, s8 *txpwr);

/**
  * @brief  Configure retry limits for different stages of Wi-Fi connection process.
  * @param[in]  conn_step_retries: Pointer to rtw_conn_step_retries.
  * @return  None.
 * @note
 *     - Configures retry limits for authentication, association, and key exchange.
 *     - The retry limits for each stage are constrained to a maximum of 10 attempts.
  */
void wifi_set_conn_step_try_limit(struct rtw_conn_step_retries *conn_step_retries);

/**
  * @brief  Automatically select a better channel based on environmental conditions.
  * @param[in]  acs_config: configure acs parameters
  * @param[out] ideal_ch
  * @return
  *    - @ref RTK_SUCCESS : The API executed successfully.
  *    - @ref RTK_FAIL :
  *      - Scan fail.
  *      - Get acs report fail
  */
s32 wifi_acs_find_ideal_channel(struct rtw_acs_config *acs_config, u8 *ideal_ch);

/**
 * @brief  Configure Wi-Fi tx advanced settings for special scenario.
 * @param[in]  tx_setting: A pointer to specific parameters.
 * @return
 *    - @ref RTK_SUCCESS : The API executed successfully.
 *    - -@ref RTK_ERR_WIFI_POWEROFF : Wi-Fi is powered off in IPS(Inactive Power Save) mode,
 *                      unable to access Wi-Fi registers.
 */
s32 wifi_set_tx_advanced_config(struct rtw_tx_advanced_cfg *tx_setting);

/** @} End of Extended_Functions group */
/** @} End of WIFI_Exported_Functions group*/
/** @} End of WIFI_API group*/

#ifdef __cplusplus
}
#endif

#endif // __WIFI_API_EXT_H

