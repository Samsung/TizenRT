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
 ******************************************************************************/
#ifndef __WLAN_INTF_H__
#define __WLAN_INTF_H__

#ifdef	__cplusplus
extern "C" {
#endif
#include <autoconf.h>

#include <wireless.h>
#include "wifi_constants.h"
#include "wifi_structures.h"

#ifndef WLAN_UNDEF
#define WLAN_UNDEF	-1
#endif

/***********************************************************/
/*
struct sk_buff {
	// These two members must be first.
	struct sk_buff		*next;		// Next buffer in list
	struct sk_buff		*prev;		// Previous buffer in list

	struct sk_buff_head	*list;			// List we are on
	unsigned char		*head;		// Head of buffer
	unsigned char		*data;		// Data head pointer
	unsigned char		*tail;		// Tail pointer
	unsigned char		*end;		//End pointer
	struct net_device 	*dev;		//Device we arrived on/are leaving by
	unsigned int 		len;			// Length of actual data
};
*/
/************************************************************/

//----- ------------------------------------------------------------------
// Wlan Interface opened for upper layer
//----- ------------------------------------------------------------------
int wifi_if1_init(void);
int wifi_if2_init(void);
void wifi_if1_deinit(void);
void wifi_if2_deinit(void);
void rltk_wlan_set_scan_chan_interval(unsigned short interval_ms);

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

int rltk_wlan_get_ccmp_key(unsigned char *uncst_key, unsigned char *group_key);
int rltk_wlan_wireless_mode(unsigned char mode);
extern int wifi_hal_iwpriv_command(unsigned char wlan_idx, char *cmd, int show_msg);
#define rtw_iwpriv_command(wlan_idx, cmd,show_msg)	wifi_hal_iwpriv_command(wlan_idx, cmd,show_msg)

//add temporarily
extern int rtw_wx_set_essid(__u8 *ssid, __u16 ssid_len);
extern int rtw_wx_set_wap(unsigned char wlan_idx, __u8 *bssid);
extern int rtw_wx_set_auth(unsigned char wlan_idx, __u32 value);
extern int rtw_wx_set_enc_ext(unsigned char wlan_idx, __u16 alg, __u8 *addr, int key_idx, int set_tx, __u8 *seq, __u16 seq_len, __u8 *key, __u16 key_len);
extern int rtw_wx_set_passphrase(unsigned char wlan_idx, __u8 *passphrase, __u16 passphrase_len);
extern int rtw_wx_connect_local(rtw_network_info_t *connect_param);
extern int rtw_wx_set_mode(unsigned char wlan_idx, int mode);
extern int rtw_wx_set_ap_essid(rtw_softap_info_t *softAP_config, unsigned char value);

/**
 * @brief  Set the listening channel.
 * @param[in]  wlan_idx: 0 means set channel for promisc STA,
 *  1 means set channel to start Soft AP.
 * @param[in]  ch: The desired channel.
 * @return  RTW_SUCCESS: If the channel is successfully set.
 * @return  RTW_ERROR: If the channel is not successfully set.
 * @note  DO NOT call this function for STA mode wifi driver,
 * 	since driver will determine the channel from its
 * 	received beacon.
 */
extern int wifi_set_freq(unsigned char wlan_idx, __u8 ch);

/**
 * @brief  Get the current channel.
 * @param[in]  wlan_idx: 0 means get current STA channel,
 *  1 means get current Soft AP channel.
 * @param[out]  channel: A pointer to the variable where the
 * 	channel value will be written.
 * @return  RTW_SUCCESS: If the channel is successfully read.
 * @return  RTW_ERROR: If the channel is not successfully read.
 */
extern int wifi_get_freq(unsigned char wlan_idx, __u8 *ch);

extern int rtw_wx_set_scan(rtw_scan_param_t *scan_param, unsigned char block);
#ifdef CONFIG_WOWLAN
extern int rtw_wowlan_ctrl(unsigned char wlan_idx, rtw_wowlan_option_t type, void *param);
#endif

//promisc related
extern int promisc_filter_retransmit_pkt(unsigned char enable, unsigned char filter_interval_ms);
extern int promisc_set(rtw_rcr_level_t enabled, void (*callback)(unsigned char *, unsigned int, void *), unsigned char len_used);
extern unsigned char is_promisc_enabled(void);
extern int promisc_get_fixed_channel(void *fixed_bssid, unsigned char *ssid, int *ssid_length);
extern void promisc_filter_by_ap_and_phone_mac(unsigned char enable, void *ap_mac, void *phone_mac);
extern int promisc_set_mgntframe(unsigned char enable);
extern int promisc_get_chnl_by_bssid(unsigned char *bssid);
extern void promisc_update_candi_ap_rssi_avg(signed char rssi, unsigned char cnt);
extern void promisc_issue_probersp(unsigned char *da);
extern void promisc_stop_tx_beacn(void);
extern void promisc_resume_tx_beacn(void);

#ifdef	__cplusplus
}
#endif

#endif //#ifndef __WLAN_INTF_H__
