/******************************************************************************
 *
 * Copyright(c) 2007 - 2012 Realtek Corporation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
 *
 *
 ******************************************************************************/
#ifndef __RTW_AP_H_
#define __RTW_AP_H_

//external function
extern void rtw_indicate_sta_assoc_event(_adapter *padapter, struct sta_info *psta);
extern void rtw_indicate_sta_disassoc_event(_adapter *padapter, struct sta_info *psta);


void init_mlme_ap_info(_adapter *padapter);
void free_mlme_ap_info(_adapter *padapter);
//void update_BCNTIM(_adapter *padapter);
void rtw_add_bcn_ie(_adapter *padapter, WLAN_BSSID_EX *pnetwork, u8 index, u8 *data, u8 len);
void rtw_remove_bcn_ie(_adapter *padapter, WLAN_BSSID_EX *pnetwork, u8 index);
void update_beacon(_adapter *padapter, u8 ie_id, u8 *oui, u8 tx);
void add_RATid(_adapter *padapter, struct sta_info *psta, u8 rssi_level);
void expire_timeout_chk(_adapter *padapter);
void update_sta_info_apmode(_adapter *padapter, struct sta_info *psta);
int rtw_check_beacon_data(_adapter *padapter, u8 *pbuf,  int len);
void rtw_set_macaddr_acl(_adapter *padapter, int mode);
int rtw_acl_add_sta(_adapter *padapter, u8 *addr);
int rtw_acl_remove_sta(_adapter *padapter, u8 *addr);
int rtw_generate_bcn_ie(_adapter *adapter, u8 *ssid, u16 ssid_len, u8 *ie, u32 max_ie_sz);
void rtw_start_bss_hdl_after_chbw_decided(_adapter *adapter);
//void start_bss_network(_adapter *padapter, struct createbss_parm *parm);
void start_bss_network(_adapter *padapter, u8 *parm);

void associated_clients_update(_adapter *padapter, u8 updated);
void bss_cap_update_on_sta_join(_adapter *padapter, struct sta_info *psta);
u8 bss_cap_update_on_sta_leave(_adapter *padapter, struct sta_info *psta);
void sta_info_update(_adapter *padapter, struct sta_info *psta);
void ap_sta_info_defer_update(_adapter *padapter, struct sta_info *psta);
u8 ap_free_sta(_adapter *padapter, struct sta_info *psta, u16 reason);
int rtw_sta_flush(_adapter *padapter);
void start_ap_mode(_adapter *padapter);
void stop_ap_mode(_adapter *padapter);

void rtw_ap_update_bss_chbw(_adapter *adapter, WLAN_BSSID_EX *bss, u8 ch, u8 bw, u8 offset);
u8 rtw_ap_chbw_decision(_adapter *adapter, u8 ifbmp, u8 excl_ifbmp
						, s16 req_ch, s8 req_bw, s8 req_offset, u8 *ch, u8 *bw, u8 *offset, u8 *chbw_allow);

#endif

