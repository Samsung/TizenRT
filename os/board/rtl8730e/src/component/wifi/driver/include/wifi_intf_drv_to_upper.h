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
#ifndef __RTW_INTF_USER_H__
#define __RTW_INTF_USER_H__

#ifdef	__cplusplus
extern "C" {
#endif

#include "wifi_conf.h"

#define		_1M_RATE_		0
#define		_2M_RATE_		1
#define		_5M_RATE_		2
#define		_11M_RATE_		3
#define		_6M_RATE_		4
#define		_9M_RATE_		5
#define		_12M_RATE_		6
#define		_18M_RATE_		7
#define		_24M_RATE_		8
#define		_36M_RATE_		9
#define		_48M_RATE_		10
#define		_54M_RATE_		11

enum WIFI_INDICATE_MODE {
	WIFI_INDICATE_DISABLE = 0,
	WIFI_INDICATE_NORMAL = 1,
	WIFI_INDICATE_WILD = 2,
	WIFI_INDICATE_MAX,
};

//----- ------------------------------------------------------------------
// Wlan Interface opened for upper layer
//----- ------------------------------------------------------------------
int wifi_if1_init(void);
int wifi_if2_init(void);
void wifi_if1_deinit(void);
void wifi_if2_deinit(void);
int wifi_if1_open(void);
int wifi_if2_open(void);
void wifi_stop_bss_network(void);
int wifi_get_chplan(u8 *chplan);
int wifi_set_platform_rom_func(void *(*calloc_func)(size_t, size_t),
							   void (*free_func)(void *),
							   int (*rand_func)(void *, unsigned char *, size_t));
void wifi_heap_statistics(u8 start);

extern int rtw_joinbss_set_bssid(unsigned char wlan_idx, __u8 *bssid);
extern int rtw_bss_set_auth(unsigned char wlan_idx, __u32 value);
extern int rtw_bss_set_enc(unsigned char wlan_idx, __u16 alg, __u8 *addr, int key_idx, int set_tx,  __u8 *key, __u16 key_len);
extern int rtw_bss_set_passphrase(unsigned char wlan_idx, __u8 *passphrase, __u16 passphrase_len);
extern int rtw_joinbss_start_api(rtw_network_info_t *connect_param);
extern int rtw_ap_start_api(rtw_softap_info_t *softAP_config, unsigned char value);
extern int rtw_scan_start_api(rtw_scan_param_t *scan_param, unsigned char block);
extern void rtw_joinbss_by_roaming(rtw_network_info_t *connect_param);

#ifdef CONFIG_WOWLAN_SD1
extern int rtw_wowlan_ctrl(unsigned char wlan_idx, rtw_wowlan_option_t type, void *param);
#endif

extern int wifi_hal_iwpriv_command(unsigned char wlan_idx, char *cmd, int show_msg);
#define rtw_iwpriv_command(wlan_idx, cmd,show_msg)	wifi_hal_iwpriv_command(wlan_idx, cmd,show_msg)

//promisc related
extern int promisc_filter_retransmit_pkt(unsigned char enable, unsigned char filter_interval_ms);
extern int _promisc_set(rtw_rcr_level_t enabled, void (*callback)(unsigned char *, unsigned int, void *), unsigned char len_used);
extern unsigned char _is_promisc_enabled(void);
extern int _promisc_get_fixed_channel(void *fixed_bssid, unsigned char *ssid, int *ssid_length);
extern void _promisc_filter_by_ap_and_phone_mac(unsigned char enable, void *ap_mac, void *phone_mac);
extern int _promisc_set_mgntframe(unsigned char enable);
extern int _promisc_get_chnl_by_bssid(unsigned char *bssid);
extern void _promisc_update_candi_ap_rssi_avg(signed char rssi, unsigned char cnt);
extern void _promisc_issue_probersp(unsigned char *da);
extern void _promisc_stop_tx_beacn(void);
extern void _promisc_resume_tx_beacn(void);

#ifdef CONFIG_NAN
int nan_intfs_init(void);
int nan_intfs_deinit(void);
int rtw_start_nan_api(u8 master_pref, u8 band_support);
void rtw_stop_nan_api(void);
int rtw_add_nan_func(void *func_param, void *nan_func_pointer);
int rtw_del_nan_func(u64 cookie);
#endif

#ifdef	__cplusplus
}
#endif

#endif //#ifndef __RTW_INTF_USER_H__
