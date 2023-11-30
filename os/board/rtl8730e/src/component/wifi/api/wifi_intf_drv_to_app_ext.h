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
  * @file    wifi_conf_ext.h
  * @author
  * @version
  * @brief   This file provides user interface for Wi-Fi station and AP mode configuration
  *             base on the functionalities provided by Realtek Wi-Fi driver.
  ******************************************************************************
  */
#ifndef __WIFI_CONF_EXT_H
#define __WIFI_CONF_EXT_H

/** @defgroup WIFI
 *  @brief      WIFI module
 *  @{
 */


#include "rtw_wifi_constants.h"
#include "rom_rtw_defs.h"
#include <platform_stdlib.h>
#include "wifi_intf_drv_to_bt.h"
#include "dlist.h"
#include <rtw_skbuff.h>

#ifdef __cplusplus
extern "C" {
#endif



/** @defgroup WIFI_Exported_Functions WIFI Exported Functions
  * @{
  */


/** @defgroup Basic_Functions
  * @{
  */

/******************************************************
 *               Function Declarations
 ******************************************************/

/**
 * @brief  Check if Wi-Fi has connected to AP before dhcp.
 * @param  None
 * @return  RTW_SUCCESS: If conneced.
 * @return  RTW_ERROR: If not connect.
 */
int wifi_is_connected_to_ap(void);

/**
 * @brief  Get scan results
 * @param[inout]  AP_num: input the pointer to the number of scanned ap info which
 * 	want to get, output the number of scanned ap info whicn can actually get.
 * @param[in]  scan_buf: pointer to the buf where scan result will be stored, the
 * 	scanned AP info will be stored one by one in form of struct rtw_scan_result_t.
 * @return  RTW_SUCCESS or RTW_ERROR.
 * @note  For synchronized scan or asynchronized scan which do not config RTW_SCAN_REPORT_EACH,
 * 	if once called wifi_scan_networks but not use this API to get scanned AP info,
 * 	driver memory for these scanned AP will not be freed until next time
 * 	wifi_scan_networks is called.
 * @note  For asynchronized scan which config RTW_SCAN_REPORT_EACH, every time a
 * 	AP is scanned, the AP info will be directly reported through scan_report_each_mode_user_callback
 * 	and freed after user callback executed, thus there is no need to use this function to get scan result.
 */
int wifi_get_scan_records(unsigned int *AP_num, char *scan_buf);

/**
 * @brief  Abort onoging wifi scan
 * @return  RTW_SUCCESS or RTW_ERROR.
 * @note  This is an asynchronized function and will return immediately, return value
 * 	only indicates whether the scan abort cmd is successfully notified to driver or not.
 * 	When scan is actually aborted, the user callback registered in wifi_scan_networks
 * 	will be executed.If there is no wifi scan in progress, this function will just return
 * 	RTW_SUCCESS and user callback won't be executed.
 */
int wifi_scan_abort(void);

/**
 * @brief  Enable or disable LPS. LPS is the abbreviation of Leisure Power Save mode.
 * 	Wi-Fi automatically turns RF off during the association to AP
 * 	if traffic is not busy, while it also automatically turns RF on
 * 	to listen to the beacon of the associated AP.
 * @param[in]  enable: It could be TRUE or FALSE.
 * 	FALSE means disable LPS, TRUE means enable LPS.
 * @return  RTW_SUCCESS if setting LPS successful.
 * @return  RTW_ERROR otherwise.
 */
int wifi_set_lps_enable(u8 enable);

/**
 * @brief  Set reconnection mode with configuration.
 * @param[in]  mode: Set 1/0 to enalbe/disable the reconnection mode.
 * @return  0 if success, otherwise return -1.
 * @note  Defining CONFIG_AUTO_RECONNECT in "rtw_autoconf.h" needs to be
 * 	done before compiling, or this API won't be effective.
 */
int wifi_config_autoreconnect(__u8 mode);

/**
 * @brief  Get the result of setting reconnection mode.
 * @param[out]  mode: Point to the result of setting reconnection mode.
 * 	mode = 0 means autoreconnect is off
 *	mode = 1 means autoreconnect is on
 * @return  0 if get the status of autoreconnect success, otherwise return -1.
 * @note  Defining CONFIG_AUTO_RECONNECT in "rtw_autoconf.h" needs to be
 * 	done before compiling, or this API won't be effective.
 */
int wifi_get_autoreconnect(__u8 *mode);

/**
 * @brief  set the timeout value for checking no beacon
 * @param  the value of timeout, in seconds.
 * @return None
 */
void wifi_set_no_beacon_timeout(unsigned char timeout_sec);

/**
 * @brief  Get reason code of latest disassociation or deauthentication.
 * @param[out]  reason_code: A pointer to the variable where the
 * 	reason code (rtw_connect_error_flag_t) will be written.
 * @return  RTW_SUCCESS or RTW ERROR.
 */
int wifi_get_disconn_reason_code(unsigned short *reason_code);


/**
 * @brief  Get the associated clients with SoftAP.
 * @param[out]  client_list_buffer: The location where the client
 * 	list will be stored.
 * @return  RTW_SUCCESS: The result is successfully got.
 * @return  RTW_ERROR: The result is not successfully got.
 */
int wifi_get_associated_client_list(rtw_client_list_t	*client_list_buffer);

/**
 * @brief  delete a STA
 * @param[in]  wlan_idx: the wlan interface index, should be SOFTAP_WLAN_INDEX.
 * @param[in]  hwaddr: the pointer to the MAC address of the STA
 * 	which will be deleted
 * @return  RTW_SUCCESS or RTW_ERROR
 * @note  this function should be used when operating as AP
 */
int wifi_del_station(unsigned char wlan_idx, unsigned char *hwaddr);

/**
 * @brief  set channel
 * @param[in]  wlan_idx: the wlan interface index, should be SOFTAP_WLAN_INDEX or STA_WLAN_INDEX.
 * @param[in]  channel: the channel which will be switch to
 * @return  RTW_SUCCESS or RTW_ERROR
 */
int wifi_set_channel(unsigned char wlan_idx, u8 channel);

/**
 * @brief  get current channel
 * @param[in]  wlan_idx: the wlan interface index, should be SOFTAP_WLAN_INDEX or STA_WLAN_INDEX.
 * @param[inout]  channel: the current channel
 * @return  RTW_SUCCESS or RTW_ERROR
 */
int wifi_get_channel(unsigned char wlan_idx, u8 *channel);

/**
 * @brief  Set the current Media Access Control (MAC) address
 *	(or Ethernet hardware address) of the 802.11 device.
 * @param[in]  idx: Set STA or SoftAP mac address.
 * @param[in]  mac: Pointer to the mac address.
 * @param[in]  efuse: Set mac address to efuse or set to RAM.
 * @return  RTW_SUCCESS or RTW_ERROR
 * @note  Set to Efuse(efuse = 1): effective after reboot.
 * Set to RAM(efuse = 0): reboot will lose effectiveness. (RECOMMENDED)
 */
int wifi_set_mac_address(int idx, unsigned char *mac, u8 efuse);

/**
 * @brief  Retrieves the current Media Access Control (MAC) address
 *	(or Ethernet hardware address) of the 802.11 device.
 * @param[in]  idx: Get STA or SoftAP mac address. Invalid parameter while setting efuse = 1.
 * @param[in]  mac: Pointer to the struct rtw_mac_t which contain obtained mac address.
 * @param[in]  efuse: Get mac address from efuse or get from RAM.
 * @return  RTW_SUCCESS or RTW_ERROR
 * @note  Get mac address inside EFUSE(efuse = 1).
 * Get runtime mac address(efuse = 0). (RECOMMENDED)
 */
int wifi_get_mac_address(int idx, rtw_mac_t *mac, u8 efuse);

/**
 * @brief  Get current Wi-Fi setting from driver.
 * @param[in]  wlan_idx: STA_WLAN_IDX or SOFTAP_WLAN_IDX.
 * @param[out]  psetting: Points to the rtw_wifi_setting_t structure which information is gotten.
 * @return  RTW_SUCCESS: The result is successfully got.
 * @return  RTW_ERROR: The result is not successfully got.
 * @note  the mode in rtw_wifi_setting_t corresponding to the wifi mode of current wlan_idx, if in
 * 	station mode, the info in rtw_wifi_setting_t except mode will correspond to the AP it connected,
 * 	if in AP mode, the info in rtw_wifi_setting_t will correspond to the softAP itself.
 */
int wifi_get_setting(unsigned char wlan_idx, rtw_wifi_setting_t *psetting);

/**
 * @brief  This function is used to get wifi mode
 * 	for station mode when connecting to AP.
 * @param[in]  no
 * @return  0-unconnected, 4-wifi4, 5-wifi5, 6-wifi6.
 */
int wifi_get_network_mode(void);

/**
 * @brief  Set the network mode according to the data rate its supported.
 * 	Driver works in BGNAX mode in default after driver initialization.
 * 	This function is used to change wireless network mode
 * 	for station mode before connecting to AP.
 * @param[in]  mode: Network mode to set. The value can be
 * 	WLAN_MD_11B, WLAN_MD_11BG, WLAN_MD_11BGN and WLAN_MD_24G_MIX.
 * @return  RTW_SUCCESS or RTW_ERROR.
 */
int wifi_set_network_mode(enum wlan_mode mode);

/**
  * @}
  */


/** @defgroup Extended_Functions
  * @{
  */

/**
 * @brief  enable promisc mode and set promisc mode
 * @param[in] enable: enable or disable promisc mode
 * @param[in] para: promisc mode and callback if enable promisc mode
 * @return  None
 */
void wifi_promisc_enable(u32 enable, promisc_para_t *para);

/**
 * @brief  check whether current wifi driver is mp or not.
 * @param  None
 * @return  1: drv_mode is RTW_DRV_MODE_MP
 * 	0: drv_mode is not RTW_DRV_MODE_MP
 */
u8 wifi_driver_is_mp(void);

/**
 * @brief  Set Management Frame Protection Support.
 * @param[in] value:
 * 	0) NO_MGMT_FRAME_PROTECTION: not support
 * 	1) MGMT_FRAME_PROTECTION_OPTIONAL: capable
 * 	2) MGMT_FRAME_PROTECTION_REQUIRED: required
 * @return  RTW_SUCCESS if setting Management Frame Protection Support successful.
 * @return  RTW_ERROR otherwise.
 */
int wifi_set_mfp_support(unsigned char value);

/**
 * @brief  Set group id of SAE.
 * @param[in] value:group id which want to be set
 * @return  RTW_SUCCESS if setting is successful.
 * @return  RTW_ERROR otherwise.
 */
int wifi_set_group_id(unsigned char value);

/**
 * @brief  for wpa supplicant indicate sae status.
 * @param[in] wlan_idx:STA_WLAN_IDX or SOFTAP_WLAN_IDX.
 * @param[in] status:sae_status which will be indicated.
 * @param[in] mac_addr:pointer of sae peer's mac_addr .
 * @return  RTW_SUCCESS if setting is successful.
 * @return  RTW_ERROR otherwise.
 */
int wifi_sae_status_indicate(u8 wlan_idx, u16 status, u8 *mac_addr);

/**
 * @brief  enable or disable pmk cache.
 * @param[in] value:1 for enable, 0 for disable
 * @return  RTW_SUCCESS if setting is successful.
 * @return  RTW_ERROR otherwise.
 */
int wifi_set_pmk_cache_enable(unsigned char value);

/**
 * @brief  Set psk related info, including ssid, passphrase, psk.
 * @param[in]  psk_data: pointer to the structure that can set
 * 	psk related info in driver.{@ref struct psk_info}
 * @return  NULL.
 * @note  can be used to set psk related info to driver when fast connect
 */

int wifi_get_ccmp_key(unsigned char *uncst_key, unsigned char *group_key);

/**
 * @brief  Show the TX and RX statistic information which counted by software(wifi driver,not phy layer).
 * @param[in]  idx: the wlan interface index, can be STA_WLAN_INDEX or SOFTAP_WLAN_IDX.
 * @return  NULL.
 */
int wifi_get_sw_statistic(unsigned char idx, rtw_sw_statistics_t *sw_statistics);

/**
 * @brief  fetch statistic info about wifi.
 * @param[out]  phy_statistic: The location where the statistic
 * 	info will be stored, for detail info, please refer to structrtw_phy_statistics .
 * @return  RTW_SUCCESS: If the statistic info is successfully get.
 * @return  RTW_ERROR: If the statistic info is not successfully get.
 * @note  the rssi and snr info will only be valid after connected to AP successfully.
 */
int wifi_fetch_phy_statistic(rtw_phy_statistics_t *phy_statistic);

/**
 * @brief  get current remaining number of packets in HW TX buffer.
 * @param[in]  None
 * @return  RTW_ERROR or the remaining packets number.
 */
int wifi_get_txbuf_pkt_num(void);

/**
 * @brief  config mode of HW indicating packets(mgnt and data)
 * 	and SW reporting packets to wifi_indication().
 * @param[in]  enable
 * 	0) WIFI_INDICATE_DISABLE: disable mode(default), HW only
 * 	   indicate bssid-matched pkts and SW don't report.
 * 	1) WIFI_INDICATE_NORMAL: HW only indicate bssid-matched
 * 	   pkts and SW report.
 * 	2) WIFI_INDICATE_WILD: HW indicate all pkts and SW report.
 * @return  None
 */
void wifi_set_indicate_mgnt(int enable);

/**
 * @brief  Get antenna infomation.
 * @param[in]  antenna: Point to the antenna value obtained from driver,
 * 	0 means main antenna, 1 means aux antenna.
 * @return  0 if get successfully, otherwise return -1.
 */

#if defined CONFIG_ANTENNA_DIVERSITY ||defined __DOXYGEN__
int wifi_get_antenna_info(unsigned char *antenna);
#endif // #ifdef CONFIG_ANTENNA_DIVERSITY

//-------------------------------------------------------------//
/*
 * @brief get band type
 * @return  the support band type.
 * 	0) WL_BAND_2_4G: only support 2.4G
 * 	1) WL_BAND_5G: only support 5G
 * 	2) WL_BAND_2_4G_5G_BOTH: support both 2.4G and 5G
 */
WL_BAND_TYPE wifi_get_band_type(void);

/**
 * @brief  get an auto channel
 * @param[in]  wlan_idx: the wlan interface index, can be STA_WLAN_INDEX or SOFTAP_WLAN_INDEX.
 * @param[in]  channel_set: the pointer to the channel set
 * 	auto channel will select from this channel set
 * @param[in]  channel_num: the number of channel in channel set
 * @return  the selected auto channel
 */
int wifi_get_auto_chl(
	unsigned char wlan_idx,
	unsigned char *channel_set,
	unsigned char channel_num);

/**
 * @brief	Get wifi TSF register[63:32]&[31:0].
 * @param[in]	port: wifi port 0/1.
 * @return TSF[63:0] or 0
 */
u64 wifi_get_tsf(unsigned char port_id);

/**
 * @brief  switch to a new channel in AP mode and using CSA to inform sta
 * @param[in]  csa_param: pointer to the csa config structure. The structure contains the following parameters:
 *   		new_chl: the new channel will be switched to.
 * 		chl_switch_cnt: the channel switch cnt, after chl_switch_cnt*102ms, ap will switch to new channel.
 * 		callback: this callback will be called after channel switch is done, and will return the new channel number and channel switch result.
 * 		action_type: 0: unicast csa action, 1: broadcast csa action, other values: disable transmit csa action.
 * 		bc_action_cnt: indicate the number of broadcast csa actions to send for each beacon interval. only valid when action_type = 1.
 * @return  RTW_SUCCESS or RTW_ERROR, only indicate whether channel switch cmd is
 * 	successfully set to wifi driver.
 * @note  this function should be used when operating as AP.
 */
int wifi_ap_switch_chl_and_inform(rtw_csa_parm_t *csa_param);

/**
 * @brief  initialize mac address filter list
 * @return  RTW_SUCCESS or RTW_ERROR.
 */
int wifi_init_mac_filter(void);

/**
 * @brief  add mac address to mac filter list then this address
 * 	will be rejected during authentication
 * @param[in]  hwaddr: the mac address which will be added to
 * 	mac filter list.
 * @return  RTW_SUCCESS or RTW_ERROR.
 */
int wifi_add_mac_filter(unsigned char *hwaddr);

/**
 * @brief  delete mac address from mac filter list
 * @param[in]  hwaddr: the mac address which will be deleted
 * 	from mac filter list.
 * @return  RTW_SUCCESS or RTW_ERROR.
 */
int wifi_del_mac_filter(unsigned char *hwaddr);

/**
 * @brief  Set the wps phase
 * 	wps: Wi-Fi Protected Setup
 * @param[in]  is_trigger_wps: to trigger wps function or not
 * 	is_trigger_wps value should only be 0 or 1
 * @return  RTW_SUCCESS or RTW_ERROR
 */
int wifi_set_wps_phase(unsigned char is_trigger_wps);

/**
 * @brief  set WPS IE in Probe Request/Probe Response/Beacon/
 * 	Association Request/Association Response
 * @param[in]  wlan_idx: the wlan interface index, can be STA_WLAN_INDEX or SOFTAP_WLAN_INDEX
 * @param[in]  buf: the pointer to buf which store the WPS IE
 * @param[in]  buf_len: the length of buf
 * @param[in]  flags:
 * 	P2PWPS_PROBE_REQ_IE: set WPS ie in Probe Request
 * 	P2PWPS_PROBE_RSP_IE: set WPS ie in Probe Response
 * 	P2PWPS_BEACON_IE: set WPS ie in Beacon
 * 	P2PWPS_ASSOC_REQ_IE: set WPS ie in Association Request
 * 	P2PWPS_ASSOC_RSP_IE: set WPS ie in Association Response
 * @return  RTW_ERROR or RTW SUCCESS
 */
int wifi_set_gen_ie(unsigned char wlan_idx, char *buf, __u16 buf_len, __u16 flags);

/**
 * @brief  Set the eap phase to wifi driver
 * @param[in]  is_trigger_eap: to trigger 802.1X EAP function or not
 * @return    RTW_SUCCESS or RTW_ERROR
 */
int wifi_set_eap_phase(unsigned char is_trigger_eap);

/**
 * @brief  get the current eap phase from wifi driver
 * @param[in]  None
 * @return  1: eap_phase is equal to 1, indicate 802.1X EAP is triggered
 * @return  0: eap_phase is equal to 0, indicate 802.1X EAP is not triggered
 */
unsigned char wifi_get_eap_phase(void);

/**
 * @brief  Set the current eap authentication method to wifi driver
 * @param[in]  eap_method: the current 802.1x EAP authentication method
 * @return    RTW_SUCCESS or RTW_ERROR
 */
int wifi_set_eap_method(unsigned char eap_method);

/**
 * @brief  send 802.1X EAP frame
 * @param[in]  wlan_idx: the wlan interface index,
 * 	wlan_idx should be STA_WLAN_INDEX or SOFTAP_WLAN_INDEX
 * @param[in]  buf: the pointer to buf which store the EAP frame
 * @param[in]  buf_len: the length of buf
 * @param[in]  flags: reserved, set to 0
 * @return  RTW_ERROR or RTW SUCCESS
 */
int wifi_if_send_eapol(unsigned char wlan_idx, char *buf, __u16 buf_len, __u16 flags);

/**
 * @brief  Set ble scan duty when coex.
 * @param[in]  duty: the desired scan duty for ble
 * @return  1: if the scan duty is successfully set
 * 	-1: if the scan duty is not successfully set.
 */
int wifi_btcoex_set_ble_scan_duty(u8 duty);

/**
 * @brief  Setup custom ie list. (Information Element)
 * @warning  This API can't be executed twice before deleting
 * 	the previous custom ie list.
 * @param[in]  cus_ie: Pointer to WIFI CUSTOM IE list.
 * @param[in]  ie_num: The number of WIFI CUSTOM IE list.
 * @return  0 if success, otherwise return -1.
 */
int wifi_add_custom_ie(void *cus_ie, int ie_num);

/**
 * @brief  Update the item in WIFI CUSTOM IE list.
 * 	(Information Element)
 * @param[in]  cus_ie: Pointer to WIFI CUSTOM IE address.
 * @param[in]  ie_index: Index of WIFI CUSTOM IE list.
 * @return  0 if success, otherwise return -1.
 */
int wifi_update_custom_ie(void *cus_ie, int ie_index);

/**
 * @brief  Delete WIFI CUSTOM IE list. (Information Element)
 * @param[in]  wlan_idx,
 * 	wlan_idx should be STA_WLAN_INDEX or SOFTAP_WLAN_INDEX
 * @return  0 if success, otherwise return -1.
 */
int wifi_del_custom_ie(unsigned char wlan_idx);

/**
 * @brief  send raw frame
 * @param[in]  raw_data_desc: the pointer of raw_data_desc_t,
 * 	which describe related information, include the pointer of raw frame and so on.
 * @return  RTW_ERROR or RTW SUCCESS
 */
int wifi_send_raw_frame(struct raw_frame_desc_t *raw_frame_desc);

/**
 * @brief  send raw frame
 * @param[in]  raw_data_desc: the pointer of raw_data_desc_t,
 * 	which describe related information, include the pointer of raw frame and so on.
 * @return  RTW_ERROR or RTW SUCCESS
 */
int wifi_send_mgnt(raw_data_desc_t *raw_data_desc);

/**
 * @brief  Control initial tx rate by different ToS value in IP header.
 * @param[in]  enable: set 1 to add control for specified tx_rate for
 * 	corresponding ToS_value, set 0 to disable initial rate control for this ToS_value.
 * @param[in]  ToS_precedence: range from 0 to 7, corresponding to IP precedence in TOS field of IP header(BIT7~5).
 * @param[in]  tx_rate: initial tx rate for packet which has the same ToS value as setted.Please
 * 	refer to enum MGN_RATE in wifi_constants.h for rate definition.
 * @return  RTW_SUCCESS or RTW_ERROR.
 * @note this function only take IP Precedence(BIT 7~5 in ToS field) into consideration.
 */
int wifi_set_tx_rate_by_ToS(unsigned char enable, unsigned char ToS_precedence, unsigned char tx_rate);

/**
 * @brief  Set EDCA parameter.
 * @param[in]  AC_param: format is shown as:
 * |        TXOP Limit        |ECWmin/ECWmax|  ACI/AIFSN  |
 * 	BIT31~16 corresponding to TXOP Limit, BIT15~8 corresponding
 * 	to ECWmin/ECWmax, BIT7~0 corresponding to ACI/AIFSN.
 * @return  RTW_SUCCESS or RTW_ERROR
 */
int wifi_set_EDCA_param(unsigned int AC_param);

/**
 * @brief  enable or disable CCA for TX
 * @param[in]  enable: 1 for enable, 0 for disable
 * @return  RTW_SUCCESS or RTW_ERROR.
 * @note when disable, both CCA and EDCCA will be disabled.
 */
int wifi_set_TX_CCA(unsigned char enable);

/**
 * @brief  set duration and send a CTS2SELF frame
 * @param[in]  wlan_idx: the wlan interface index, can be STA_WLAN_IDX or SOFTAP_WLAN_IDX
 * @param[in]  duration: the duration value for the CTS2SELF frame
 * @return  RTW_SUCCESS or RTW_ERROR.
 */
int wifi_set_cts2self_duration_and_send(unsigned char wlan_idx, unsigned short duration);

#if defined CONFIG_WOWLAN_TCP_KEEP_ALIVE || defined __DOXYGEN__
/**
 * @brief  construct a tcp packet that offload to wlan. wlan would keep sending this packet to tcp server.
 *
 * @param[in]  socket_fd : tcp socket
 * @param[in]  content : tcp payload
 * @param[in]  len : tcp payload size
 * @param[in]  interval_ms : send this packeter every interval_ms milliseconds
 * @param[in]  resend_ms : if packet fails to send at interval_ms, then resend it after resemd_ms
 * @param[in]  wake_sys : if packet fails to send after resend several times, should wlan wake system
 * @return  RTW_SUCCESS
 */
int wifi_set_tcp_keep_alive_offload(int socket_fd, uint8_t *content, size_t len, uint32_t interval_ms, uint32_t resend_ms, uint8_t wake_sys);
#endif

#if defined CONFIG_WOWLAN_DHCP_RENEW || defined __DOXYGEN__
/**
 * @brief  construct a udp renew packet that offload to wlan. wlan would keep sending this packet to dhcp server.
 *
 * @return  RTW_SUCCESS
 */
int wifi_set_dhcp_offload(void);
#endif



#if defined CONFIG_ARP_KEEP_ALIVE || defined __DOXYGEN__
/**
 * @brief   use ARP response as keep alive packet instead of null frame
 *
 * @param[in]   enable : enable or disable ARP response keep alive
 * @return  RTW_SUCCESS
 */
int wifi_wowlan_set_arp_rsp_keep_alive(int enable);
#endif

#if defined CONFIG_WOWLAN_DTIMTO || defined __DOXYGEN__
/**
 * @brief   tcp keep alive fine tune
 *
 * @param[in]   dtimto_enable : enable or disable
 * @param[in]   retry_inc : change dtim to this value
 * @param[in]   ack_timeout : timeout for wait tcp ack
 * @param[in]   dtim : TBTT interval
 * @return  RTW_SUCCESS
 */
int wifi_wowlan_set_dtimto(uint8_t dtimto_enable, uint8_t retry_inc, uint8_t ack_timeout, uint8_t dtim);
#endif


#if defined CONFIG_SMART_DTIM  || defined __DOXYGEN
/**
 * @brief   smart dtim in wowlan keep alive
 *
 * @param[in]   check_period : check period of TBTT
 * @param[in]   threshold : threshold of beacon lose
 * @param[in]   change_dtim : change dtim
 * @param[in]   dtim : TBTT interval
 * @return  RTW_SUCCESS
 */
int wifi_wowlan_set_smartdtim(uint8_t check_period, uint8_t threshold, uint8_t change_dtim, uint8_t dtim);
#endif


#if defined CONFIG_WOWLAN_PARAM || defined __DOXYGEN__
/**
 * @brief   wowlan parameter setting
 *
 * @param[in]   fwdis_period : fw decision disconnect check period
 * @param[in]   fwdis_trypktnum : fw decision disconnect null packet retry
 * @param[in]   pno_enable : pno method enable
 * @param[in]   pno_timeout : timeout for wait ap enable
 * @param[in]   l2_keepalive_period : send period of l2 keep alive
 * @return  RTW_SUCCESS
 */
int wifi_wowlan_set_wowlan_param(u8  fwdis_period,
								 u8  fwdis_trypktnum,
								 u8  pno_enable,
								 u8  pno_timeout,
								 u8  l2_keepalive_period);
#endif


#if defined CONFIG_ARP_REQUEST_KEEP_ALIVE  || defined __DOXYGEN__
/**
 * @brief   use ARP request as keep alive packet instead of null frame
 *
 * @param[in]   powerbit : power bit setting
 * @param[in]   dtim1to : change to dtim1
 * @return  RTW_SUCCESS
 */
int wifi_wowlan_set_arpreq_keepalive(u8  powerbit,
									 u8  dtim1to);
#endif

#if defined CONFIG_WOWLAN_IO_WDT || defined __DOXYGEN__
/**
 * @brief   set gpio pull ctrl in wowlan
 *
 * @param[in]   gpio : gpiof number
 * @param[in]   interval : pull ctrl interval
 * @return  RTW_SUCCESS
 */
int wifi_wowlan_set_wdt(u8  gpio,
						u8  interval);
#endif

// WoWlan related
//-------------------------------------------------------------//
#if defined CONFIG_WOWLAN_SD1 || defined __DOXYGEN__
/**
 * @brief  control wowlan mode enable and disable
 * @param[in]  enable:
 * 	0 means WOWLAN_DISABLE
 * 	1 means WOWLAN_ENABLE
 * @return  RTW_SUCCESS or RTW_ERROR
 */
int wifi_wowlan_ctrl(int enable);

/**
 * @brief  set wowlan customer pattern
 * @param[in]  pattern: refer to struct wowlan_pattern_t
 * @return  RTW_SUCCESS or RTW_ERROR
 */
int wifi_wowlan_set_pattern(wowlan_pattern_t pattern);
#endif

/**
 * @brief  set the csi parameters and enable or disable csi func(sta or softap)
 * @param[in]  act_param: A pointer to the param
 * @note:
 *    rtw_csi_action_parm_t act_param = {0};
 *    act_param.mode = 2;
 *    ...
 *    wifi_csi_config(&act_param);
 * @return  RTW_SUCCESS or RTW_ERROR
 */
int wifi_csi_config(rtw_csi_action_parm_t *act_param);

/**
 * @brief  get csi raw data and csi hdr info
 * @param[in]  buf_len: buffer size for getting csi info
 * @param[in]  csi_buf: the pointer to csi data buffer
 * @param[in]  len: the size of csi raw data
 * @note:
 *    unsigned char *csi_buf = NULL;
 *    unsigned int len;
 *    csi_buf = rtw_zmalloc(buf_len);
 *    wifi_csi_report(buf_len,csi_buf,&len);
 * @return  RTW_SUCCESS or RTW_ERROR
 */
int wifi_csi_report(u32 buf_len, u8 *csi_buf, u32 *len);

/**
 * @brief  Set PTA type when coex.
 * @param[in]  type: the PTA type(PTA_BT/PTA_WIFI/PTA_AUTO).
 * @param[in]  role, PTA_HOST_BT / PTA_HOST_WIFI
 * @param[in]  process, CALIBRATION_START / CALIBRATION_STOP / COMMON_ACTION
 * @return  Null.
 */
void wifi_btcoex_set_pta(pta_type_t type, u8 role, u8 process);

/**
 * @brief  Set BTS0 or BTS1.
 * @param[in]  bt_ant: the BT ant(BTS0/BTS1).
 * @return  Null.
 */
void wifi_btcoex_set_bt_ant(u8 bt_ant);

/**
  * @brief Write BT RFK  data to RFC
  * @param  calibration data
  * @retval  BT RFK result(1:success  0:failed)
  */
int wifi_btcoex_bt_rfk(struct bt_rfk_param *rfk_param);

/**
  * @brief Zigbee Call WL RFK
  * @param  void
  * @retval  ZBC RFK result(0:success  -1:failed)
  */
int wifi_zigbee_coex_zb_rfk(void);

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

/**
 * @brief  wpa notify wifi driver that 4 way handshake is failed.
 * wifi driver will do disconnect and autoreconnect.
 * can delete this API if autoreconnect can move to up layer.
 * @param[in]  void:
 * @return  null.
 */
void wifi_wpa_sta_4way_fail_notify(void);

/**
 * @brief  for wpa to set key to driver
 * @param[in]  rtw_crypt_info
 * @return  null.
 */
void wifi_wpa_add_key(struct rtw_crypt_info *crypt);

/**
 * @brief  for wpa to set/del/flush pmksa
 * @param[in]  pmksa_ops
 * @return  null.
 */
void wifi_wpa_pmksa_ops(struct rtw_pmksa_ops_t *pmksa_ops);


/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */




#ifdef __cplusplus
}
#endif

#endif // __WIFI_API_H

