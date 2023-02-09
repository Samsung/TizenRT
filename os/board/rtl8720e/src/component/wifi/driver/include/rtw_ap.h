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

#define AUTH_TIMEOUT				3
#define ASSOC_TIMEOUT				3
#define EXPIRE_TIMEOUT				60	// 60*2 = 120 sec = 2 min, expire after no any traffic.

struct ap_priv {
	/* Number of non-HT AP/stations */
	u8 num_sta_no_ht; //int num_sta_no_ht;
	/* Number of associated Non-ERP stations (i.e., stations using 802.11b
	 * in 802.11g BSS) */
	u8 num_sta_non_erp;
	/* Number of associated stations that do not support Short Slot Time */
	u8 num_sta_no_short_slot_time;
	/* Number of associated stations that do not support Short Preamble */
	u8 num_sta_no_short_preamble;
	/* Number of HT associated stations that do not support greenfield */
	u8 num_sta_ht_no_gf;
	/* Number of HT associated stations 20 MHz */
	u8 num_sta_ht_20mhz;
	u8 num_sta_no_wmm;

	/* Overlapping BSS information */
	u8 olbc_ht;

	u8 ht_op_mode;

	_lock	bcn_update_lock;
	u8		update_bcn;

	/*ap csa related*/
	u8 ap_csa_start;
	u8 csa_new_chl;
	u8 *csa_ie;
	ap_channel_switch_callback_t ap_channel_switch_callback;
	_timer ap_csa_update_timer;

	_list asoc_list;
	_list auth_list;
	_lock asoc_list_lock;
	_lock auth_list_lock;
	_lock expire_lock;

	/* pointers to STA info; based on allocated AID or NULL if AID free
	 * AID is in the range 1-2007, so sta_aid[0] corresponders to AID 1
	 * and so on
	 */
	struct sta_info *sta_aid[NUM_STA];

	u16 sta_dz_bitmap;//only support 15 stations, staion aid bitmap for sleeping sta.
	u16 tim_bitmap;//only support 15 stations, aid=0~15 mapping bit0~bit15

	u16 max_num_sta;

};

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
void rtw_set_macaddr_acl(_adapter *padapter, int mode);
int rtw_acl_add_sta(_adapter *padapter, u8 *addr);
int rtw_acl_remove_sta(_adapter *padapter, u8 *addr);
int rtw_generate_bcn_ie(_adapter *adapter, u8 *ssid, u16 ssid_len, u8 *ie, u32 max_ie_sz);
//void start_bss_network(_adapter *padapter, struct createbss_parm *parm);
void start_bss_network(_adapter *padapter, u8 *parm);

void associated_clients_update(_adapter *padapter, u8 updated);
void bss_cap_update_on_sta_join(_adapter *padapter, struct sta_info *psta);
u8 bss_cap_update_on_sta_leave(_adapter *padapter, struct sta_info *psta);
void sta_info_update(_adapter *padapter, struct sta_info *psta);
void ap_sta_info_defer_update(_adapter *padapter, struct sta_info *psta);
u8 ap_free_sta(_adapter *padapter, struct sta_info *psta, u16 reason);
int rtw_sta_flush(_adapter *padapter);
void ap_csa_update_timer_hdl(void *FunctionContext);

#endif

