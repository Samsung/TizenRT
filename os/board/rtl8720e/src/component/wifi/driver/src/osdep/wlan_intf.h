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
int rltk_wlan_init(int idx_wlan, rtw_mode_t mode);				//return 0: success. -1:fail
void rltk_wlan_deinit(void);
void rltk_wlan_deinit_fastly(void);
int rltk_wlan_start(int idx_wlan);
int rltk_wlan_statistic(unsigned char idx, rtw_sw_statistics_t *sw_statistics);
unsigned char rltk_wlan_running(unsigned char idx);		// interface is up. 0: interface is down
int rltk_wlan_handshake_done(void);
int rltk_wlan_wireless_mode(unsigned char mode);
int rltk_wlan_set_wps_phase(unsigned char is_trigger_wps);
int rltk_wlan_set_eap_phase(unsigned char is_trigger_eap);
unsigned char rltk_wlan_get_eap_phase(void);
int rltk_wlan_set_eap_method(unsigned char eap_method);
int rltk_wlan_is_connected_to_ap(void);
void rltk_wlan_set_scan_chan_interval(unsigned short interval_ms);
int rltk_wlan_change_channel_plan(unsigned char channel_plan);
int rltk_del_station(unsigned char wlan_idx, unsigned char *hwaddr);
int rltk_get_auto_chl(unsigned char wlan_idx, unsigned char *channel_set, unsigned char channel_num);
int rltk_wlan_get_disconn_reason_code(unsigned short *reason);
int rltk_change_mac_address_from_ram(int idx, unsigned char *mac);
int rltk_wlan_set_wifi_mac_address(__u8 *wifi_mac);
int rltk_wlan_get_wifi_mac_address(__u8 *mac);
int rltk_wlan_set_bt_mac_address(__u8 *bt_mac);
int rltk_wlan_get_bt_mac_address(__u8 *mac);
int rltk_wlan_get_ap_dtim(__u8 *dtim_period);

void rltk_psk_info_set(struct psk_info *psk_data);
void rltk_psk_info_get(struct psk_info *psk_data);
__u8 rltk_wlan_is_mp(void);

#ifdef CONFIG_WLAN_SWITCH_MODE
int rltk_wlan_reinit_drv_sw(const char *ifname, rtw_mode_t mode);
int rltk_set_mode_prehandle(rtw_mode_t curr_mode, rtw_mode_t next_mode, const char *ifname);
int rltk_set_mode_posthandle(rtw_mode_t curr_mode, rtw_mode_t next_mode, const char *ifname);
#endif

#ifdef CONFIG_IEEE80211W
void rltk_wlan_tx_sa_query(unsigned char key_type);
void rltk_wlan_tx_deauth(unsigned char b_broadcast, unsigned char key_type);
void rltk_wlan_tx_auth(void);
#endif

void rltk_wlan_set_indicate_mgnt(int enable);
int rltk_wlan_init_mac_filter(void);
int rltk_wlan_deinit_mac_filter(void);
int rltk_wlan_add_mac_filter(unsigned char *hwaddr);
int rltk_wlan_del_mac_filter(unsigned char *hwaddr);

int rltk_set_hidden_ssid(const char *ifname, unsigned char value);
unsigned char rltk_get_band_type(void);
unsigned int rltk_wlan_get_tsf(unsigned char port_id);
int rltk_wlan_get_ccmp_key(unsigned char *uncst_key, unsigned char *group_key);
int rltk_wlan_set_tx_rate_by_ToS(unsigned char enable, unsigned char ToS_precedence, unsigned char tx_rate);
int rltk_wlan_set_EDCA_param(unsigned int AC_param);
int rltk_wlan_set_TX_CCA(unsigned char enable);
int rltk_wlan_ap_switch_chl_and_inform_sta(unsigned char new_chl, unsigned char chl_switch_cnt, ap_channel_switch_callback_t callback);
int rltk_wlan_set_cts2self_dur_and_send(unsigned char wlan_idx, unsigned short duration);
int rltk_wlan_get_sta_max_data_rate(unsigned char *inidata_rate);
void rltk_wlan_set_no_beacon_timeout(unsigned char timeout_sec);

int rltk_wlan_csi_config(rtw_csi_action_parm_t *act_param);
int rltk_wlan_csi_report(__u32 buf_len, __u8 *csi_buf, __u32 *len, rtw_csi_header_t *csi_hdr);

//add temporarily
extern int rtw_wx_get_essid(unsigned char wlan_idx, __u8 *ssid);
extern int rtw_wx_set_essid(unsigned char wlan_idx, __u8 *ssid, __u16 ssid_len);
extern int rtw_wx_set_wap(unsigned char wlan_idx, __u8 *bssid);
extern int rtw_wx_get_wap(unsigned char wlan_idx, __u8 *bssid);
extern int rtw_wx_set_auth(const char *ifname, __u16 idx, __u32 value);
extern int rtw_wx_set_enc_ext(const char *ifname, __u16 alg, __u8 *addr, int key_idx, int set_tx, __u8 *seq, __u16 seq_len, __u8 *key, __u16 key_len);
extern int rtw_wx_get_enc_ext(unsigned char wlan_idx, __u16 *alg, __u8 *key_idx, __u8 *passphrase);
extern int rtw_wx_get_auth_type(unsigned char wlan_idx, unsigned int *auth_type);
extern int rtw_wx_set_passphrase(unsigned char wlan_idx, __u8 *passphrase, __u16 passphrase_len);
extern int rtw_wx_connect_local(rtw_network_info_t *connect_param);
extern int rtw_wx_get_passphrase(unsigned char wlan_idx, __u8 *passphrase);
#ifdef CONFIG_IEEE80211W
extern int rtw_wx_set_mfp_support(unsigned char wlan_idx, __u8 value);
#endif
#ifdef CONFIG_SAE_SUPPORT
extern int rtw_wx_set_group_id(unsigned char wlan_idx, __u8 value);
#endif
extern int rtw_wx_set_pmk_cache_enable(unsigned char wlan_idx, __u8 value);
extern int rtw_wx_set_mode(unsigned char wlan_idx, int mode);
extern int rtw_wx_get_mode(unsigned char wlan_idx, int *mode);
extern int rtw_wx_set_ap_essid(unsigned char wlan_idx, __u8 *ssid, __u16 ssid_len);
extern int rtw_wx_get_phy_statistic(rtw_phy_statistics_t *phy_statistic);
extern int rtw_wx_set_freq(unsigned char wlan_idx, __u8 ch);
extern int rtw_wx_get_freq(unsigned char wlan_idx, __u8 *ch);
extern int rtw_wx_set_scan(rtw_scan_param_t *scan_param, unsigned char block);
extern int rtw_wx_get_scan_results(unsigned int *AP_num, char *scan_buf);
extern int rtw_wx_scan_abort(void);
extern int rtw_wx_get_scan(unsigned char wlan_idx, char *buf, __u16 buf_len);
extern int rtw_wx_send_eapol(const char *ifname, char *buf, __u16 buf_len, __u16 flags);
extern int rtw_wx_send_raw_frame(raw_data_desc_t *raw_data_desc);
#ifdef CONFIG_WPS
extern int rtw_wx_set_gen_ie(unsigned char wlan_idx, char *buf, __u16 buf_len, __u16 flags);
#endif
extern int rtw_pm_set(rtw_pm_option_t type, __u8 *param);
#if CONFIG_AUTO_RECONNECT
extern int rtw_wx_set_autoreconnect(__u8 mode, __u8 retry_times, __u16 timeout);
extern int rtw_wx_get_autoreconnect(__u8 *mode);
#endif
extern int rtw_ex_get_channel_plan(__u8 idx, __u8 *channel_plan);
#ifdef CONFIG_WOWLAN
extern int rtw_wowlan_ctrl(const char *ifname, rtw_wowlan_option_t type, void *param);
#endif
extern int rtw_wx_set_custome_ie(const char *ifname, void *cus_ie, int ie_num);
extern int rtw_wx_update_custome_ie(const char *ifname, void *cus_ie, int ie_index);
extern int rtw_wx_del_custome_ie(const char *ifname);

int rltk_coex_set_ble_scan_duty(__u8 duty);

#ifdef CONFIG_RTK_MESH
extern int rtw_ex_set_mesh(const char *ifname, __u8 enable);
extern int rtw_ex_set_mesh_id(const char *ifname, void *mesh_id, int id_len);
extern int rtw_ex_get_mesh_id(const char *ifname, void *mesh_id);
extern int rtw_ex_set_mesh_rssi_threshold(const char *ifname, __s32 rssi);
extern int rtw_ex_get_mesh_rssi_threshold(const char *ifname, __s32 *rssi);
#endif
extern int rtw_ex_read_mac(unsigned char wlan_idx, rtw_mac_t *mac);
extern int rtw_ex_get_client_list(unsigned char wlan_idx, rtw_maclist_t *client_list_buffer);
#ifdef CONFIG_ANTENNA_DIVERSITY
extern int rtw_ex_get_antenna_info(unsigned char wlan_idx, unsigned char *antenna);
#endif
extern int rtw_iwpriv_command(const char *ifname, char *cmd, int show_msg);
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
