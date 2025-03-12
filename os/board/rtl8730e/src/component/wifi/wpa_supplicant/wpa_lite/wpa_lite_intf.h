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
	u32					rom_rsvd;
};

/**
  * @brief  The structure is used to describe the psk info
  */
struct psk_info {
	unsigned char index;                  ///<  index
	unsigned char psk_essid[32 + 4]; ///< refer to NDIS_802_11_LENGTH_SSID + 4
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
void rtw_psk_deauth_info_flash(char *data, int len, int flags, void *userdata);
void rtw_psk_disconnect_hdl(char *buf, int buf_len, int flags, void *userdata);
__weak void rtw_set_to_roam(u8 to_roam);
__weak u8 rtw_roam_nb_ch_num_get(void);
__weak u16 rtw_roam_nb_ch_get(u8 ch);

#endif // _WPA_LITE_INTF_H_

