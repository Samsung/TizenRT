/**
  ******************************************************************************
  * @file    wifi_intf_drv_to_app_ext.h
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

#ifndef __WIFI_CONF_EXT_H
#define __WIFI_CONF_EXT_H

/** @defgroup WIFI_API
 *  @brief      WIFI_API module
 *  @{
 */


#include "rtw_wifi_constants.h"
#include "rom_rtw_defs.h"
#include "platform_stdlib.h"
#include "basic_types.h"
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
 * 	scanned AP info will be stored one by one in form of struct struct rtw_scan_result.
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
 * @param[in]  block: set 1 for wait scan actually aborted
 * @return  RTW_SUCCESS or RTW_ERROR.
 * @note  If block set to 0, this will be an asynchronized function and will return immediately,
 * 	return value only indicates whether the scan abort cmd is successfully notified to driver or not.
 *	If block set to 1, this will be a synchronized function and will return when scan is actually aborted.
 * 	When scan is actually aborted, the user callback registered in wifi_scan_networks
 * 	will be executed.If there is no wifi scan in progress, this function will just return
 * 	RTW_SUCCESS and user callback won't be executed.
 */
int wifi_scan_abort(u8 block);

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
 * @brief  Set the listen interval of LPS. LPS is the abbreviation of Leisure Power Save mode.
 * 	Wi-Fi automatically turns RF off during the association to AP
 * 	if traffic is not busy, while it also automatically turns RF on
 * 	to listen to the beacon of the associated AP. The listen interval is used to indicate
 *	how often the STA wakes to listen to beacons of AP.
 * @param[in]  interval: the listen interval of LPS (unit: 102.4 ms).
 *	If the specified interval is not a multiple of beacon interval, the actual listen interval
 *	will be rounded to the largest multiple that <= the specified value. It will not be
 *	rounded to 0 when the specified value < beacon interval. In this case, use 1 instead.
 *	e.g. Given that beacon interval = 2, calling wifi_set_lps_listen_interval(5) will make actual
 *	listen interval = 4, calling wifi_set_lps_listen_interval(1) will make actual listen interval = 1
 * @return  RTW_SUCCESS if setting LPS listen interval successful.
 * @return  RTW_ERROR otherwise.
 */
int wifi_set_lps_listen_interval(u8 interval);


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
 * @brief  Get the associated clients with SoftAP.
 * @param[out]  client_list_buffer: The location where the client
 * 	list will be stored.
 * @return  RTW_SUCCESS: The result is successfully got.
 * @return  RTW_ERROR: The result is not successfully got.
 */
int wifi_get_associated_client_list(struct _rtw_client_list_t	*client_list_buffer);

/**
 * @brief  delete a STA for softap
 * @param[in]  hwaddr: the pointer to the MAC address of the STA
 * 	which will be deleted
 * @return  RTW_SUCCESS or RTW_ERROR
 * @note  this function should be used when operating as AP
 */
int wifi_del_station(unsigned char *hwaddr);

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
 * @brief  set country code
 * @param[in]  cntcode: Pointer to the country code which want to be set
 * @return  RTW_SUCCESS or RTW_ERROR
 */
u8 wifi_set_countrycode(char *cntcode);

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
 * @note:
 *    callback return value will decide whether driver need continue process this packet.
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
 * @brief  get ccmp unicast key and group key
 * @param[in]  wlan_idx: STA_WLAN_IDX or SOFTAP_WLAN_IDX
 * @param[in]  mac_addr: client mac addr for softap mode, set to NULL for sta mdoe
 * @param[out]  uncst_key: CCMP unicast key
 * @param[out]  group_key: CCMP group key
 * @return  RTW_SUCCESS if get key successfully.
 * @return  RTW_ERROR otherwise.
 */
int wifi_get_ccmp_key(u8 wlan_idx, u8 *mac_addr, unsigned char *uncst_key, unsigned char *group_key);

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
 * @brief  Get antdiv infomation.
 * @param[in]  antdiv_mode: Point to the antdiv_mode value obtained from driver,
 * @param[in]  curr_ant: Point to the antenna value obtained from driver,
 * 	0 means main antenna, 1 means aux antenna.
 * @return  0 if get successfully, otherwise return -1.
 */

int wifi_get_antdiv_info(unsigned char *antdiv_mode, unsigned char *curr_ant);

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
 * @brief  Setup custom ie list. (Information Element)
 * @warning  This API can't be executed twice before deleting
 * 	the previous custom ie list.
 * @param[in]  cus_ie: a buffer stores custom IE list, format of custom ie is struct custom_ie.
 *  u8 ie1[] = {221, 2, 2, 2};
 *  u8 ie2[] = {221, 2, 1, 1};
 *  struct custom_ie buf[2] = {{ie1, BEACON}, {ie2, PROBE_RSP}};
 *  wifi_add_custom_ie(buf, 2);
 * @param[in]  ie_num: The number of custom IEs in cus_ie.
 * @return  0 if success, otherwise return -1.
 */
int wifi_add_custom_ie(struct custom_ie *cus_ie, int ie_num);

/**
 * @brief  Update the item in WIFI CUSTOM IE list.
 * 	(Information Element)
 * @param[in]  cus_ie: Pointer to WIFI CUSTOM IE address.
 *  u8 ie[] = {221, 2, 1, 3} ;
 *  struct custom_ie buf_update = {ie, PROBE_RSP};
 *  wifi_update_custom_ie(&buf_update, 2);
 * @param[in]  ie_index: Index of WIFI CUSTOM IE list.
 * @return  0 if success, otherwise return -1.
 */
int wifi_update_custom_ie(struct custom_ie *cus_ie, int ie_index);

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
 * @brief  Control initial tx rate by different ToS value in IP header.
 * @param[in]  enable: set 1 to add control for specified tx_rate for
 * 	corresponding ToS_value, set 0 to disable initial rate control for this ToS_value.
 * @param[in]  ToS_precedence: range from 0 to 7, corresponding to IP precedence in TOS field of IP header(BIT7~5).
 * @param[in]  tx_rate: initial tx rate for packet which has the same ToS value as setted.Please
 * 	refer to enum enum mgn_rate_type in wifi_constants.h for rate definition.
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
 *    csi_buf = rtos_mem_zmalloc(buf_len);
 *    wifi_csi_report(buf_len,csi_buf,&len);
 * @return  RTW_SUCCESS or RTW_ERROR
 */
int wifi_csi_report(u32 buf_len, u8 *csi_buf, u32 *len);

/**
 * @brief  for wifi speaker setting
 * @param[in]  set_type: wifi speaker setting type
 * @param[in]  settings: A pointer to the params
 * @note:
 *      when set_type == SPEAKER_SET_INIT
 *          mode: 0 for slave, 1 for master
 *          thresh: unit 128us
 *          relay_en: relay control
 *      when set_type == SPEAKER_SET_LATCH_I2S_COUNT
 *          port: 0 for select port 0's TSFT to trigger audio latch count, 1 for port 1
 *          latch_period: 0 for trigger audio latch period is 4.096ms, 1 for 8.192ms
 *      when set_type == SPEAKER_SET_TSF_TIMER
 *          enable: 1 for enable twt timer, 0 for disable
 *          tsft: absolute value for twt timer, unit ms
 *          port: 0 for select port 0's TSFT to trigger twt timer interrupt, 1 for port 1
 * @return  null.
 */
void wifi_speaker_setting(enum SPEAKER_SET_TYPE set_type, union speaker_set *settings);

/**
 * @brief  for user to set tx power
 * 1. Currently will TX with the set power,  regardless of power by rate and power by limit.
 * 2. Afterwards, it can be extended to specify rate, or power by limit needs to be considered.
 * @param[in]  txpwr_ctrl_info: the pointer of rtw_tx_power_ctl_info_t
 *    b_tx_pwr_force_enbale: 1 for enable, 0 for disable.
 *    tx_pwr_force: unit 0.25dBm.
 * @note:
 *    For amebadplus, the power range varies for different channels or IC, the recommended power range is -2 ~ 23 dBm,
 *    if exceeds the power range, the power may be inaccurate, and will be changed to the boundary value.
 *    For amebasmart&amebalite, the recommended power range is -24 ~ 24 dBm.
 *    For both, we suggest setting the power not to exceed the power by rate table.
 * @return  RTW_SUCCESS or RTW_ERROR
 */
int wifi_set_tx_power(struct rtw_tx_power_ctl_info_t *txpwr_ctrl_info);

/**
 * @brief  for user to get tx power
 * @param[in]  rate: phy rate
 *    For the definition of phy rate, please refer to enum mgn_rate_type
 *    CCK rate 1M,2M,5.5M,11M
 *    OFDM rate 6M,9M,12M,18M,24M,36M,48M,54M
 *    HT rate MCS0~MCS7
 *    VHT rate MCS0~MCS8
 *    HE rate MCS0~MCS9
 * @param[out]  txpwr: the current tx power, unit 0.25dBm.
 * @return  RTW_SUCCESS or RTW_ERROR
 */
int wifi_get_tx_power(u8 rate, s8 *txpwr);

#ifdef CONFIG_PLATFORM_TIZENRT_OS
/**
 * @brief  Get current bandwidth in current connection
 * @param[in]  void
 * @return  int: The current bandwidth value in MHz
 */
int wifi_get_current_bw(void);

/**
 * @brief  Get key_mgmt value of current connection
 * @param[in]  void
 * @return  int: The key_mgmt value of current connection
 * @return  RTW_ERROR: Failed to get key_mgmt, not connected to AP.
 */
int wifi_get_key_mgmt(void);

/**
 * @brief  Get the tx retry count 
 * @param[in]  idx: wlan idx to obtain from
 * for station: STA_WLAN_INDEX 
 * for SoftAP: SOFTAP_WLAN_INDEX
 * @return  unsigned int: The total tx retry value. If more than UINT_MAX, value will wrap around.
 * @return  RTW_ERROR: Failed to get key_mgmt, not connected to AP.
 */
unsigned int wifi_get_tx_retry(int idx);
#endif //CONFIG_PLATFORM_TIZENRT_OS
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

