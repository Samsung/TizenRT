#ifndef _WPA_LITE_INTF_H_
#define _WPA_LITE_INTF_H_

#include "rom_wpa_lite_intf.h"

struct wpa_param_t {
	unsigned char 		peer_mac[6];
	unsigned char 		self_mac[6];
	u32	 				unicast_cipher;
	u8					is_enterprise;
	int					RSNEnabled;		// bit0-WPA, bit1-WPA2
	u32 				dot118021XGrpPrivacy;	// This specify the privacy algthm. used for Grp key
	u32 				AuthKeyMgmt;	// This specify the auth key algthm. used for 11w
	unsigned char		AuthInfoBuf[INFO_ELEMENT_SIZE];
	u32 				dot11_wpa_mode;
	u8 					rsnxe_ie[RSNXE_MAX_LEN];
	u8 					pmksa_id[PMKID_LEN];
	u8 					pmk[PMK_LEN];
	u8					use_pmksa;
	u8					ap_gbl_info;
	u8					eapol_key_rsnd_limit;
	u32					rom_rsvd;
};

/**
  * @brief  The structure is used to describe the psk info
  */
struct psk_info {
	unsigned char index;                  ///<  index
	unsigned char psk_essid[RTW_ESSID_MAX_SIZE + 1]; ///< refer to NDIS_802_11_LENGTH_SSID + 1
	unsigned char psk_passphrase[RTW_MAX_PSK_LEN + 1]; ///< refer to IW_PASSPHRASE_MAX_SIZE + 1
	unsigned char wpa_global_PSK[20 * 2]; ///< refer to A_SHA_DIGEST_LEN * 2
	enum rtw_security security_type;
};

/**
  * @brief  The structure is used to describe the stored deauth info
  */
struct deauth_info {
	u8 bssid[6];
	u32  frame_len;
	u8  frame_buf[50];
};

/**
  * @brief  The enum is used to indicate the wnm actioncode
  */
enum rtw_ieee80211_wnm_actioncode {
	RTW_WLAN_ACTION_WNM_BTM_QUERY = 6,
	RTW_WLAN_ACTION_WNM_BTM_REQ = 7,
	RTW_WLAN_ACTION_WNM_BTM_RSP = 8,
	RTW_WLAN_ACTION_WNM_NOTIF_REQ = 26,
	RTW_WLAN_ACTION_WNM_NOTIF_RSP = 27,
};

void rtw_wpa_init(u8 port);
void rtw_psk_wpa_deinit(u8 port);
void rtw_sae_set_user_group_id(unsigned char value);
void rtw_psk_get_psk_info(struct psk_info *psk_data);
void rtw_psk_set_psk_info(struct psk_info *psk_data);
void rtw_psk_set_pmk_from_eap(u8 *pmk, u8 *hwaddr);

void rtw_psk_disconnect_hdl(u8 *mac_addr, u8 iface_type);
void rtw_set_to_roam(u8 trigger_roam);
u8 rtw_roam_nb_ch_num_get(void);
u16 rtw_roam_nb_ch_get(u8 ch);

/*event handle functions*/
void rtw_sae_sta_rx_auth(u8 *evt_info);/*RTW_EVENT_RX_MGNT*/
void rtw_sae_ap_rx_auth(u8 *evt_info);/*RTW_EVENT_RX_MGNT_AP*/
void rtw_sae_sta_start(u8 *evt_info);/*RTW_EVENT_EXTERNAL_AUTH_REQ*/
void rtw_psk_sta_start_4way(u8 *evt_info);/*RTW_EVENT_WPA_STA_4WAY_START*/
void rtw_psk_ap_start_4way(u8 *evt_info);/*RTW_EVENT_WPA_AP_4WAY_START*/
void rtw_psk_sta_recv_eapol(u8 *evt_info);/*RTW_EVENT_WPA_STA_4WAY_RECV*/
void rtw_psk_ap_recv_eapol(u8 *evt_info);/*RTW_EVENT_WPA_AP_4WAY_RECV*/
void rtw_psk_set_psk_info_evt_hdl(u8 *evt_info);/*RTW_EVENT_WPA_SET_PSK_INFO*/
void rtw_owe_start_calc(u8 *evt_info);/*RTW_EVENT_OWE_START_CALC*/
void rtw_owe_peer_key_recv(u8 *evt_info);/*RTW_EVENT_OWE_PEER_KEY_RECV*/
#if defined(CONFIG_IEEE80211V) || defined(CONFIG_IEEE80211K) || defined(CONFIG_IEEE80211R)
void rtw_roam_kvr_cap_update(u8 *evt_info);/*RTW_EVENT_KVR_CAP_UPDATE*/
#if defined(CONFIG_IEEE80211V) || defined(CONFIG_IEEE80211K)
void rtw_roam_nb_rpt_elem_parsing(u8 *evt_info);/*RTW_EVENT_NB_RESP_RECV*/
#endif
#ifdef CONFIG_IEEE80211V
void rtw_wnm_btm_req_process(u8 *evt_info);/*RTW_EVENT_BTM_REQ_RECV*/
void rtw_wnm_dbg_cmd(u8 *evt_info);/*RTW_EVENT_BTM_DEBUG_CMD*/
#endif
#ifdef CONFIG_IEEE80211R
void rtw_ft_auth_start(u8 *evt_info);/*RTW_EVENT_FT_AUTH_START*/
void rtw_ft_rx_mgnt(u8 *evt_info);/*RTW_EVENT_FT_RX_MGNT*/
#endif
#endif
#ifndef CONFIG_PLATFORM_TIZENRT_OS
// TizenRT currently doesn't support saving deauth info as it requires a flash region
void rtw_psk_deauth_info_flash_event_hdl(u8 *evt_info);/*RTW_EVENT_DEAUTH_INFO_FLASH*/
void rtw_psk_deauth_info_flash(u8 *data, s32 len, s32 flags);
#endif //#ifndef CONFIG_PLATFORM_TIZENRT_OS
#endif

