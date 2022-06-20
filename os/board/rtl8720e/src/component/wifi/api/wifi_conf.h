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
  * @file    wifi_conf.h
  * @author
  * @version
  * @brief   This file provides user interface for Wi-Fi station and AP mode configuration
  *             base on the functionalities provided by Realtek Wi-Fi driver.
  ******************************************************************************
  */
#ifndef __WIFI_CONF_H
#define __WIFI_CONF_H

/** @addtogroup nic NIC
 *  @ingroup    wlan
 *  @brief      NIC functions
 *  @{
 */
#include "basic_types.h"
#include "wifi_constants.h"
#include "wifi_structures.h"
#include "wlan_intf.h"
#include "wifi_ind.h"
#include <platform_stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************
 *                    Macros
 ******************************************************/
#define RTW_ENABLE_API_INFO

#ifdef RTW_ENABLE_API_INFO
#if defined(CONFIG_MBED_ENABLED)
extern __u32 GlobalDebugEnable;
#define RTW_API_INFO(...)     do {\
		if (GlobalDebugEnable) \
			printf(__VA_ARGS__);\
	}while(0)
#else
#define RTW_API_INFO printf
#endif
#else
#define RTW_API_INFO(args)
#endif

#define CMP_MAC( a, b)		(((a[0])==(b[0]))&& \
				((a[1])==(b[1]))&& \
				((a[2])==(b[2]))&& \
				((a[3])==(b[3]))&& \
				((a[4])==(b[4]))&& \
				((a[5])==(b[5])))

/** Create u32_t value from bytes */
#define WIFI_MAKEU32(a,b,c,d) (((u32_t)((a) & 0xff) << 24) | \
                               ((u32_t)((b) & 0xff) << 16) | \
                               ((u32_t)((c) & 0xff) << 8)  | \
                                (u32_t)((d) & 0xff))

/******************************************************
 *                    Constants
 ******************************************************/
#define SCAN_LONGEST_WAIT_TIME	(4500)

#define MAC_FMT			"%02x:%02x:%02x:%02x:%02x:%02x"

/* enable for partial channel scan*/
#define PSCAN_ENABLE		0x01
/* set to select scan time to FAST_SURVEY_TO, otherwise SURVEY_TO*/
#define PSCAN_FAST_SURVEY	0x02
/* set to select scan time to FAST_SURVEY_TO and resend probe request*/
#define PSCAN_SIMPLE_CONFIG	0x04

#ifndef WLAN0_NAME
#define WLAN0_NAME	"wlan0"
#endif
#ifndef WLAN1_NAME
#define WLAN1_NAME	"wlan1"
#endif

#ifndef NET_IF_NUM
#ifdef CONFIG_CONCURRENT_MODE
#define NET_IF_NUM		2
#else
#define NET_IF_NUM		1
#endif
#endif

typedef enum _WL_BAND_TYPE {
	WL_BAND_2_4G = 0,
	WL_BAND_5G,
	WL_BAND_2_4G_5G_BOTH,
	WL_BANDMAX
} WL_BAND_TYPE, *PWL_BAND_TYPE;

/******************************************************
 *                 Type Definitions
 ******************************************************/

/******************************************************
 *                    Structures
 ******************************************************/

/******************************************************
 *                    Structures
 ******************************************************/
typedef struct {
	void				*join_sema;
	unsigned int		join_timeout;
	unsigned char		block;
} internal_join_block_param_t;
/******************************************************
 *               Function Declarations
 ******************************************************/
/**
 * @brief  Join a Wi-Fi network.
 * 	Scan for, associate and authenticate with a Wi-Fi network.
 * @param[in]  connect_param: the pointer of a struct which store the connection
 * 	info, including ssid, bssid, password, etc, for details, please refer to struct
 * 	rtw_network_info_t in wifi_structures.h
 * @param[in]  block: if block is set to 1, it means synchronized wifi connect, and this
* 	API will return until connect is finished; if block is set to 0, it means asynchronized
* 	wifi connect, and this API will return immediately.
 * @return  RTW_SUCCESS: when the system is joined for synchronized wifi connect, when connect
* 	cmd is set successfully for asynchronized wifi connect.
 * @return  RTW_ERROR: if an error occurred.
 * @note  Please make sure the Wi-Fi is enabled before invoking this function.
 * 	(@ref wifi_on())
 * @note  if bssid in connect_param is set, then bssid will be used for connect, otherwise ssid
 * 	is used for connect.
 */
int wifi_connect(rtw_network_info_t *connect_param, unsigned char block);

/**
 * @brief  Disassociates from current Wi-Fi network.
 * @param  None
 * @return  RTW_SUCCESS: On successful disassociation from the AP.
 * @return  RTW_ERROR: If an error occurred.
 */
int wifi_disconnect(void);

/**
 * @brief  Check if Wi-Fi has connected to AP before dhcp.
 * @param  None
 * @return  RTW_SUCCESS: If conneced.
 * @return  RTW_ERROR: If not connect.
 */
int wifi_is_connected_to_ap(void);

/**
 * @brief  Check if the specified wlan_idx is running.
 * @param[in]  wlan_idx: can be set as WLAN0_IDX or WLAN1_IDX.
 * @return  If the function succeeds, the return value is 1.
 * 	Otherwise, return 0.
 * @note  For STA mode, only use WLAN0_IDX
 * 	For AP mode, only use WLAN0_IDX
 * 	For CONCURRENT mode, use WLAN0_IDX for sta and WLAN1_IDX for ap
 */
int wifi_is_running(unsigned char wlan_idx);

/**
 * @brief  Retrieves the current Media Access Control (MAC) address
 *	(or Ethernet hardware address) of the 802.11 device.
 * @param[in]  mac: Pointer to the struct rtw_mac_t which contain obtained mac address.
 */
int wifi_get_mac_address(rtw_mac_t *mac);

/**
 * @brief  Get the associated clients with SoftAP.
 * @param[out]  client_list_buffer: The location where the client
 * 	list will be stored.
 * @param[in]  buffer_length: The buffer length is reserved for future use.
 * 	Currently, buffer length is set to a fixed value: 25.
 * @return  RTW_SUCCESS: The result is successfully got.
 * @return  RTW_ERROR: The result is not successfully got.
 */
int wifi_get_associated_client_list(
	void			*client_list_buffer,
	unsigned short	buffer_length);

/**
 * @brief  Get current Wi-Fi setting from driver.
 * @param[in]  wlan_idx: WLAN0_IDX or WLAN1_IDX.
 * @param[out]  psetting: Points to the rtw_wifi_setting_t structure which information is gotten.
 * @return  RTW_SUCCESS: The result is successfully got.
 * @return  RTW_ERROR: The result is not successfully got.
 * @note  the mode in rtw_wifi_setting_t corresponding to the wifi mode of current wlan_idx, if in
 * 	station mode, the info in rtw_wifi_setting_t except mode will correspond to the AP it connected,
 * 	if in AP mode, the info in rtw_wifi_setting_t will correspond to the softAP itself.
 */
int wifi_get_setting(unsigned char wlan_idx, rtw_wifi_setting_t *psetting);

/**
 * @brief  Show the network information stored in a rtw_wifi_setting_t structure.
 * @param[in]  ifname: the wlan interface name, can be WLAN0_NAME or WLAN1_NAME.
 * @param[in]  pSetting: Points to the rtw_wifi_setting_t structure which information is gotten by @ref wifi_get_setting().
 * @return  RTW_SUCCESS or RTW_ERROR.
 */
int wifi_show_setting(const char *ifname,rtw_wifi_setting_t *pSetting);

/**
 * @brief  Show the TX and RX statistic information which counted by software(wifi driver,not phy layer).
 * @param[in]  idx: the wlan interface index, can be WLAN0_IDX or WLAN1_IDX.
 * @return  NULL.
 */
int wifi_get_sw_statistic(unsigned char idx, rtw_sw_statistics_t *sw_statistics);

/**
 * @brief  fetch statistic info about wifi.
 * @param[out]  phy_statistic: The location where the statistic
 * 	info will be stored, for detail info, please refer to structrtw_phy_statistics .
 * @return  RTW_SUCCESS: If the statistic info is successfully get.
 * @return  RTW_ERROR: If the statistic info is not successfully get.
 * @note  the statistic info will only be valid after connected to AP successfully.
 */
int wifi_fetch_phy_statistic(rtw_phy_statistics_t *phy_statistic);

/**
 * @brief  Set the listening channel for promiscuous mode.
 * 	Promiscuous mode will receive all the packets in
 * 	this channel.
 * @param[in]  channel: The desired channel.
 * @return  RTW_SUCCESS: If the channel is successfully set.
 * @return  RTW_ERROR: If the channel is not successfully set.
 * @note  DO NOT call this function for STA mode wifi driver,
 * 	since driver will determine the channel from its
 * 	received beacon.
 */
int wifi_set_channel(int channel);

/**
 * @brief  Get the current channel on STA interface(WLAN0_NAME).
 * @param[out]  channel: A pointer to the variable where the
 * 	channel value will be written.
 * @return  RTW_SUCCESS: If the channel is successfully read.
 * @return  RTW_ERROR: If the channel is not successfully read.
 */
int wifi_get_channel(int *channel);

/**
 * @brief  Enable Wi-Fi.
 * - Bring the Wireless interface "Up".
 * - Initialize the driver thread which arbitrates access
 * 	to the SDIO/SPI bus.
 * @param[in]  mode: Decide to enable WiFi in which mode.
 * 	The optional modes are RTW_MODE_STA, RTW_MODE_AP,
 * 	RTW_MODE_STA_AP, RTW_MODE_PROMISC and RTW_MODE_P2P.
 * @return  RTW_SUCCESS: if the WiFi chip initialized successfully.
 * @return  RTW_ERROR: if the WiFi chip initialization failed.
 */
int wifi_on(rtw_mode_t mode);

/**
 * @brief  Disable Wi-Fi.
 * @param  None
 * @return  RTW_SUCCESS: deinit success,
 * 	wifi mode is changed to RTW_MODE_NONE.
 * @return  RTW_ERROR: otherwise.
 */
int wifi_off(void);

/**
 * @brief  Switch Wifi Mode
 * - Switch Wifi Mode to @param[in]
 * @param[in]  mode: Decide to switch WiFi to which mode.
 * 	The optional modes are RTW_MODE_STA, RTW_MODE_AP,
 * 	RTW_MODE_STA_AP, RTW_MODE_PROMISC.
 * @return  RTW_SUCCESS: WiFi switch mode success.
 * @return  RTW_ERROR: WiFi switch mdoe fail.
 */
int wifi_set_mode(rtw_mode_t mode);

/**
 * @brief  Set IPS/LPS mode.
 * @param[in]  ips_mode: The desired IPS mode. It becomes effective
 * 	when wlan enter IPS.
 * 	IPS is the abbreviation of Inactive Power Save mode.
 * 	Wi-Fi automatically turns RF off if it is not associated to AP.
 * 	- ips_mode could be IPS_MODE_NONE,IPS_MODE_NORMAL or IPS_MODE_RESUME.
 * 	IPS_MODE_NONE means disable ips, IPS_MODE_NORMAL means enable ips,
 * 	IPS_MODE_RESUME means resume to the last ips powermode which
 * 	recorded in wifi driver.
 * @param[in]  lps_mode: The desired LPS mode. It becomes effective
 * 	when wlan enter LPS.
 * 	LPS is the abbreviation of Leisure Power Save mode.
 * 	Wi-Fi automatically turns RF off during the association to AP
 * 	if traffic is not busy, while it also automatically turns RF on
 * 	to listen to the beacon of the associated AP.
 * 	- lps_mode could be LPS_MODE_NONE,LPS_MODE_NORMAL or LPS_MODE_RESUME.
 * 	LPS_MODE_NONE means disable lps, LPS_MODE_NORMAL means enable lps,
 * 	LPS_MODE_RESUME means resume to the last lps powermode which recorded
 * 	in wifi driver.
 * @return  RTW_SUCCESS if setting the corresponding mode successful.
 * @return  RTW_ERROR otherwise.
 */
int wifi_set_powersave_mode(u8 ips_mode, u8 lps_mode);

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
 * @brief  enable or disable pmk cache.
 * @param[in] value:1 for enable, 0 for disable
 * @return  RTW_SUCCESS if setting is successful.
 * @return  RTW_ERROR otherwise.
 */
int wifi_set_pmk_cache_enable(unsigned char value);

/**
 * @brief  Trigger Wi-Fi driver to start an infrastructure Wi-Fi network.
 * @param[in]  softAP_config:the pointer of a struct which store the softAP
 * 	configuration, please refer to struct rtw_softap_info_t in wifi_structures.h
 * @warning  If a STA interface is active when this function is called,
 * 	the softAP will start on the same channel as the STA.
 * 	It will NOT use the channel provided!
 * @return  RTW_SUCCESS: If successfully creates an AP.
 * @return  RTW_ERROR: If an error occurred.
 * @note  if hidden_ssid in softAP_config is set to 1, then this softAP will start
 * 	with hidden ssid.
 * @note  Please make sure the Wi-Fi is enabled before invoking this function.
 * 	(@ref wifi_on())
 */
int wifi_start_ap(rtw_softap_info_t *softAP_config);

/**
 * @brief  Initiate a scan to search for 802.11 networks.
  * Synchronized scan and asynchronized scan can be confgiured by the input param block.
  * For asynchronized scan, there are two different ways about how the scan result will be reported.
  * The first way is that when scan is done ,the total number of scanned APs will be reported through
  * scan_user_callback, and the detailed scanned AP infos can be get by calling wifi_get_scan_records,
  * so in this way, scan_user_callback need to be registered in scan_param.
  * The second way is that every time a AP is scanned, this AP info will be directly reported by
  * scan_report_each_mode_user_callback, and when scan is done, scan_report_each_mode_user_callback will
  * report a NULL pointer for notification. So in this way, scan_report_each_mode_user_callback need to
  * be registered in scan_param, and RTW_SCAN_REPORT_EACH need to be set in scan_param->options.Also in
  * this mode, scan_user_callback is no need to be registered.
 * @param[in]  scan_param: refer to struct rtw_scan_param_t in wifi_structures.h.
 * @param[in]  block: If set to 1, it's synchronized scan and this API will return
 * 	after scan is done. If set to 0, it's asynchronized scan and this API will return
 * 	immediately.
 * @return  RTW_SUCCESS or RTW_ERROR for asynchronized scan, return RTW_ERROR or
 * 	scanned AP number for Synchronized scan.
 * @note  If this API is called, the scanned APs are stored in WiFi driver dynamic
 * 	allocated memory, for synchronized scan or asynchronized scan which not use RTW_SCAN_REPORT_EACH,
 * 	these memory will be freed when wifi_get_scan_records is called.
 */
int wifi_scan_networks(rtw_scan_param_t *scan_param, unsigned char block);

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
 * @brief  Set the network mode according to the data rate its supported.
 * 	Driver works in BGN mode in default after driver initialization.
 * 	This function is used to change wireless network mode
 * 	for station mode before connecting to AP.
 * @param[in]  mode: Network mode to set. The value can be
 * 	RTW_NETWORK_B, RTW_NETWORK_BG and RTW_NETWORK_BGN.
 * @return  RTW_SUCCESS or RTW_ERROR.
 */
int wifi_set_network_mode(rtw_network_mode_t mode);

/**
 * @brief  Set the chip to start or stop the promiscuous mode.
 * @param[in]  enabled: enabled can be set 0, 1, 2, 3 and 4.
 * 	if enabled is zero, disable the promisc, else enable the promisc.
 * 	0) means disable the promisc.
 * 	1) means enable the promisc special for all ethernet frames.
 * 	2) means enable the promisc special for Broadcast/Multicast
 * 	   ethernet frames.
 * 	3) means enable the promisc special for all 802.11 frames.
 * 	4) means enable the promisc special for Broadcast/Multicast
 * 	   802.11 frames.
 * @param[in]  callback: the callback function which will
 * 	receive and process the network data.
 * @param[in]  len_used: specify if the the promisc data length is used.
 * 	If len_used set to 1, packet(frame data) length will be saved
 * 	and transferred to callback function.
 * @return  RTW_SUCCESS or RTW_ERROR
 * @note  This function can implement vendor specified simple configure.
 * @note  To fetch Ethernet frames, the len_used should be set to 1
 */
int wifi_set_promisc(
	rtw_rcr_level_t	enabled,
	void	(*callback)(unsigned char *, unsigned int, void *),
	unsigned char	len_used);

/**
 * @brief  Let Wi-Fi enter promiscuous mode.
 * @param[in]  None
 * @return  None
 */
void wifi_enter_promisc_mode(void);

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
 * @param[in]  wlan_idx: the wlan interface index, can be WLAN0_IDX or WLAN1_IDX
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
 * @param[in]  ifname: the wlan interface name,
 * 	ifname should be WLAN0_NAME or WLAN1_NAME
 * @param[in]  buf: the pointer to buf which store the EAP frame
 * @param[in]  buf_len: the length of buf
 * @param[in]  flags: reserved, set to 0
 * @return  RTW_ERROR or RTW SUCCESS
 */
int wifi_send_eapol(const char *ifname, char *buf, __u16 buf_len, __u16 flags);

/**
 * @brief  Set reconnection mode with configuration.
 * @param[in]  mode: Set 1/0 to enalbe/disable the reconnection mode.
 * @param[in]  retry_times: The number of retry limit.
 * @param[in]  timeout: The timeout value (in seconds).
 * @return  0 if success, otherwise return -1.
 * @note  Defining CONFIG_AUTO_RECONNECT in "autoconf.h" needs to be
 * 	done before compiling, or this API won't be effective.
 */
int wifi_config_autoreconnect(__u8 mode, __u8 retry_times, __u16 timeout);

/**
 * @brief  Get the result of setting reconnection mode.
 * @param[out]  mode: Point to the result of setting reconnection mode.
 * 	mode = 0 means autoreconnect is off
 *	mode = 1 means autoreconnect is on
 * @return  0 if get the status of autoreconnect success, otherwise return -1.
 * @note  Defining CONFIG_AUTO_RECONNECT in "autoconf.h" needs to be
 * 	done before compiling, or this API won't be effective.
 */
int wifi_get_autoreconnect(__u8 *mode);

/**
 * @brief  Set psk related info, including ssid, passphrase, psk.
 * @param[in]  psk_data: pointer to the structure that can set
 * 	psk related info in driver.{@ref struct psk_info}
 * @return  NULL.
 * @note  can be used to set psk related info to driver when fast connect
 */
void wifi_psk_info_set(struct psk_info *psk_data);

/**
 * @brief  Get psk related info, including ssid, passphrase, psk.
 * @param[out]  psk_data: pointer to the structure that will restore
 * 	psk related info. {ref struct psk_info}
 * @return  NULL.
 * @note  can be used to get current psk related info after connected
 * 	to AP successfully，and may write these info
 * 	to flash for fast connect next time
 */
void wifi_psk_info_get(struct psk_info *psk_data);

/**
 * @brief  Get enctryption ccmp key used by wifi(sta mode only)
 * @param[out]  UncstKey: the location where the CCMP TK(tempary key)
 *  will be stored
 * @param[out]  group_key: the location where the CCMP GTK(group key)
 *  will be stored
 * @return  RTW_SUCCESS: The result is successfully got.
 * @return  RTW_ERROR: The result is not successfully got
 */
int wifi_get_ccmp_key(unsigned char *uncst_key, unsigned char *group_key);

/**
 * @brief  Get reason code of latest disassociation or deauthentication.
 * @param[out]  reason_code: A pointer to the variable where the
 * 	reason code will be written.
 * @return  RTW_SUCCESS or RTW ERROR.
 */
int wifi_get_disconn_reason_code(unsigned short *reason_code);

/**
 * @brief  get join status during wifi connectection
 * @param  None
 * @return join status, refer to macros in wifi_conf.c
 */
rtw_join_status_t wifi_get_join_status(void);

/**
 * @brief  set the timeout value for checking no beacon
 * @param  the value of timeout, in seconds.
 * @return None
 */
void wifi_set_no_beacon_timeout(unsigned char timeout_sec);

/**
 * @brief  Set ble scan duty when coex.
 * @param[in]  duty: the desired scan duty for ble
 * @return  1: if the scan duty is successfully set
 * 	-1: if the scan duty is not successfully set.
 */
int wifi_btcoex_set_ble_scan_duty(u8 duty);

/**
 * @brief  check whether current wifi driver is mp or not.
 * @param  None
 * @return  1: CONFIG_MP_INCLUDED is open
 * 	0: CONFIG_MP_INCLUDED is not open
 */
u8 wifi_driver_is_mp(void);

#ifndef BIT
#define BIT(x)	((__u32)1 << (x))
#endif

#ifndef _CUSTOM_IE_TYPE_
#define _CUSTOM_IE_TYPE_
/**
 * @brief  The enumeration is transmission type for wifi custom ie.
 */
enum CUSTOM_IE_TYPE {
	PROBE_REQ = BIT(0),
	PROBE_RSP = BIT(1),
	BEACON	  = BIT(2),
	ASSOC_REQ = BIT(3),
};
typedef __u32 rtw_custom_ie_type_t;
#endif /* _CUSTOM_IE_TYPE_ */

/* ie format
 * +-----------+--------+-----------------------+
 * |element ID | length | content in length byte|
 * +-----------+--------+-----------------------+
 *
 * type: refer to CUSTOM_IE_TYPE
 */
#ifndef _CUS_IE_
#define _CUS_IE_
/**
 * @brief  The structure is used to set WIFI custom ie list,
 * 	and type match CUSTOM_IE_TYPE.
 * 	The ie will be transmitted according to the type.
 */
typedef struct _cus_ie {
	__u8 *ie;
	__u8 type;
} rtw_custom_ie_t, *p_rtw_custom_ie_t;
#endif /* _CUS_IE_ */

/**
 * @brief  Setup custom ie list. (Information Element)
 * @warning  This API can't be executed twice before deleting
 * 	the previous custom ie list.
 * @param[in]  cus_ie: Pointer to WIFI CUSTOM IE list.
 * @param[in]  ie_num: The number of WIFI CUSTOM IE list.
 * @return  0 if success, otherwise return -1.
 * @note  Defininig CONFIG_CUSTOM_IE in "autoconf.h" needs to be done
 * 	before compiling, or this API won't be effective.
 */
int wifi_add_custom_ie(void *cus_ie, int ie_num);

/**
 * @brief  Update the item in WIFI CUSTOM IE list.
 * 	(Information Element)
 * @param[in]  cus_ie: Pointer to WIFI CUSTOM IE address.
 * @param[in]  ie_index: Index of WIFI CUSTOM IE list.
 * @return  0 if success, otherwise return -1.
 * @note  Defininig CONFIG_CUSTOM_IE in "autoconf.h" needs to be
 * 	done before compiling, or this API won't be effective.
 */
int wifi_update_custom_ie(void *cus_ie, int ie_index);

/**
 * @brief  Delete WIFI CUSTOM IE list. (Information Element)
 * @param  None
 * @return  0 if success, otherwise return -1.
 * @note  Defininig CONFIG_CUSTOM_IE in "autoconf.h" needs to be
 * 	done before compiling, or this API won't be effective.
 */
int wifi_del_custom_ie(void);

#ifdef CONFIG_PROMISC

/**
 * @brief  Initialize packet filter related data.
 * @param  None
 * @return  None
 */
void wifi_init_packet_filter(void);

/**
 * @brief  Add packet filter.
 * @param[in]  filter_id: The filter id.
 * 	filter id can be num between 0 to 4.
 * @param[in]  patt: Point to the filter pattern.
 * @param[in]  rule: Point to the filter rule.
 * @return  0 if success, otherwise return -1.
 * @note  For now, the maximum number of filters is 5.
 */
int wifi_add_packet_filter(
	unsigned char					filter_id,
	rtw_packet_filter_pattern_t		*patt,
	rtw_packet_filter_rule_t		rule);

/**
 * @brief  Enable the packet filter.
 * @param[in]  filter_id: The filter id.
 * 	filter id can be num between 0 to 4.
 * @return  0 if success, otherwise return -1.
 * @note  The filter can be used only if it has been enabled.
 */
int wifi_enable_packet_filter(unsigned char filter_id);

/**
 * @brief  Disable the packet filter.
 * @param[in]  filter_id: The filter id.
 * 	filter id can be num between 0 to 4.
 * @return  0 if success, otherwise return -1.
 */
int wifi_disable_packet_filter(unsigned char filter_id);

/**
 * @brief  Remove the packet filter.
 * @param[in]  filter_id: The filter id.
 * 	filter id can be num between 0 to 4.
 * @return  0 if success, otherwise return -1.
 */
int wifi_remove_packet_filter(unsigned char filter_id);

/**
 * @brief: Filter out the retransmission MIMO packet in promisc mode.
 * @param[in]  enable: set 1 to enable filter retransmission pkt function,
 * 	set 0 to disable this filter function.
 * @param[in]  filter_interval_ms: if 'enable' equals 0, it's useless;
 * 	if 'enable' equals 1, this value:
 * 	- indicate the time(ms) below which an adjacent pkt received
 * 	will be claimed a retransmission pkt;
 * 	- if it has the same length with the previous pkt, and driver
 * 	will drop all retransmission pkts;
 * @return 0 if success, otherwise return -1.
 * @e.g.  For example, if the packet transmission time interval is 10ms,
 * 	but driver receives two packets with the same length within 3ms
 * 	then the second packet will be dropped if configed as
 * 	wifi_retransmit_packet_filter(1,3).
 */
int wifi_retransmit_packet_filter(u8 enable, u8 filter_interval_ms);

/**
 * @brief  Only receive the packets sent by the specified
 * 	ap and phone in promisc mode.
 * @param[in]  enable: set 1 to enable filter,
 * 	set 0 to disable this filter function.
 * @param[in]  ap_mac: if 'enable' equals 0, it's useless;
 * 	if 'enable' equals 1, this value is the ap's mac address.
 * @param[in]  phone_mac: if 'enable' equals 0, it's useless;
 * 	if 'enable' equals 1, this value is the phone's mac address.
 * @return  None.
 * @note  Please invoke this function as
 * 	"wifi_filter_by_ap_and_phone_mac(0,NULL,NULL)" before exiting
 * 	promisc mode if you enabled it during the promisc mode.
 */
void wifi_filter_by_ap_and_phone_mac(
	u8		enable,
	void	*ap_mac,
	void	*phone_mac);

/**
 * @brief  config to report ctrl packet or not under promisc mode.
 * @param[in]  enable: set 1 to enable ctrl packet report,
 * 	set 0 to disable ctrl packet report.
 * @return  0 if success, otherwise return -1.
 * @note this function can only be used under promisc mode,
 * 	i.e. between wifi_set_promisc(enable,...,...) and
 * 	wifi_set_promisc(disable,...,...)
 */
int wifi_promisc_ctrl_packet_rpt(u8 enable);
#endif

/**
 * @brief  Get antenna infomation.
 * @param[in]  antenna: Point to the antenna value obtained from driver,
 * 	0 means main antenna, 1 means aux antenna.
 * @return  0 if get successfully, otherwise return -1.
 */
#ifdef CONFIG_ANTENNA_DIVERSITY
int wifi_get_antenna_info(unsigned char *antenna);
#endif // #ifdef CONFIG_ANTENNA_DIVERSITY

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
 * @brief  send raw frame
 * @param[in]  raw_data_desc: the pointer of raw_data_desc_t,
 * 	which describe related information, include the pointer of raw frame and so on.
 * @return  RTW_ERROR or RTW SUCCESS
 */
int wifi_send_raw_frame(raw_data_desc_t *raw_data_desc);

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
 * @param[in]  AC_param: format is shown as in below ,
 * +--------------------------+-------------+-------------+
 * |        TXOP Limit        |ECWmin/ECWmax|  ACI/AIFSN  |
 * +--------------------------+-------------+-------------+
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
 * @param[in]  wlan_idx: the wlan interface index, can be WLAN0_IDX or WLAN1_IDX
 * @param[in]  duration: the duration value for the CTS2SELF frame
 * @return  RTW_SUCCESS or RTW_ERROR.
 */
int wifi_set_cts2self_duration_and_send(unsigned char wlan_idx, unsigned short duration);

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

#ifdef CONFIG_WOWLAN_TCP_KEEP_ALIVE
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

#ifdef CONFIG_WOWLAN_DHCP_RENEW
/**
 * @brief  construct a udp renew packet that offload to wlan. wlan would keep sending this packet to dhcp server.
 *
 * @return  RTW_SUCCESS
 */
int wifi_set_dhcp_offload(void);
#endif

#ifdef CONFIG_ARP_KEEP_ALIVE
/**
 * @brief   use ARP response as keep alive packet instead of null frame
 *
 * @param[in]   enable : enable or disable ARP response keep alive
 * @return  RTW_SUCCESS
 */
int wifi_wowlan_set_arp_rsp_keep_alive(int enable);
#endif

#ifdef CONFIG_WOWLAN_DTIMTO
int wifi_wowlan_set_dtimto(uint8_t dtimto_enable, uint8_t retry_inc, uint8_t ack_timeout, uint8_t dtim);
#endif


// WoWlan related
//-------------------------------------------------------------//
#ifdef CONFIG_WOWLAN
typedef struct {
	unsigned int	filter_id;
	unsigned int	polarity;
	unsigned int	type;
	unsigned int	offset;
	unsigned char	*bitmask;
	unsigned char	*pattern;
} wowlan_pattern_param_t;

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
 * @param[in]  wlan_idx: the wlan interface index, can be WLAN0_IDX or WLAN1_IDX.
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
 * @brief  delete a STA
 * @param[in]  wlan_idx: the wlan interface index, can be WLAN0_IDX or WLAN1_IDX.
 * @param[in]  hwaddr: the pointer to the MAC address of the STA
 * 	which will be deleted
 * @return  RTW_SUCCESS or RTW_ERROR
 * @note  this function should be used when operating as AP
 */
int wifi_del_station(unsigned char wlan_idx, unsigned char *hwaddr);

/**
 * @brief  switch to a new channel in AP mode and using CSA to inform sta
 * @param[in]  new_chl: the new channel will be switched to.
 * @param[in]  chl_switch_cnt: the channel switch cnt,
* 	after chl_switch_cnt*102ms, ap will switch to new channel.
 * @param[in]  callback: this callback will be called after channel switch is done,
 * 	and will return the new channel number and channel switch result.
 * @return  RTW_SUCCESS or RTW_ERROR, only indicate whether channel switch cmd is
 * 	successfully set to wifi driver.
 * @note  this function should be used when operating as AP.
 */
int wifi_ap_switch_chl_and_inform(
	unsigned char new_chl,
	unsigned char chl_switch_cnt,
	ap_channel_switch_callback_t callback);

/**
 * @brief	Get wifi TSF register[31:0].
 * @param[in]	port: wifi port 0/1.
 * @return TSF[31:0] or 0
 */
unsigned int wifi_get_tsf_low(unsigned char port_id);

#ifdef CONFIG_RTK_MESH
/**
 * @brief  print current mac filter list
 * @return  RTW_SUCCESS or RTW_ERROR
 */
int wifi_list_mac_filter(void);

/**
 * @brief  print current mesh ID
 * @param[out]  mesh_id : point to the mesh ID that will get
 * @return  RTW_SUCCESS or RTW_ERROR
 */
int wifi_get_mesh_id(unsigned char *mesh_id);

/**
 * @brief  set mesh RSSI threshold
 * @param[in]  ifname: can be WLAN0_NAME or WLAN1_NAME
 * @param[in]  rssi: the mesh RSSI threshold value that will set
 * @return  RTW_SUCCESS or RTW_ERROR
 */
int wifi_set_mesh_rssi_threshold(const char *ifname, s32 rssi);

#endif

/**
 * @brief  set the csi parameters and enable or disable csi func(sta or softap)
 * @param[in]  act_param: A pointer to the param
 * @Example:
 *    rtw_csi_action_parm_t act_param = {0};
 *    act_param.mode = 2;
 *    ...
 *    wifi_csi_config(&act_param);
 * @return  RTW_SUCCESS or RTW_ERROR
 */
int wifi_csi_config(rtw_csi_action_parm_t *act_param);

/**
 * @brief  get csi raw data and other info(sta or softap)
 * @param[in]  buf_len: buffer size for getting csi info
 * @param[in]  csi_buf: the pointer to csi raw data buffer
 * @param[in]  len: the size of csi raw data
 * @param[in]  csi_header: the pointer to csi extra info buffer
 * @Example:
 *    unsigned char *csi_buf = NULL;
 *    rtw_csi_header_t csi_hdr = {0};
 *    csi_buf = rtw_zmalloc(buf_len);
 *    wifi_csi_report(buf_len,csi_buf,len,&csi_hdr);
 * @return  RTW_SUCCESS or RTW_ERROR
 */
int wifi_csi_report(u32 buf_len, u8 *csi_buf, u32 *len, rtw_csi_header_t *csi_header);

#ifdef __cplusplus
}
#endif

/*\@}*/

#endif // __WIFI_API_H
