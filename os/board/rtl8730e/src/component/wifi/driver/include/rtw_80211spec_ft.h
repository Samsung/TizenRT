/******************************************************************************
 *
 * Copyright(c) 2007 - 2011 Realtek Corporation. All rights reserved.
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
#ifndef	__RTW_80211SPEC_FT_H_
#define __RTW_80211SPEC_FT_H_

enum rtw_ieee80211_ft_actioncode {
	RTW_WLAN_ACTION_FT_RESV,
	RTW_WLAN_ACTION_FT_REQ,
	RTW_WLAN_ACTION_FT_RSP,
	RTW_WLAN_ACTION_FT_CONF,
	RTW_WLAN_ACTION_FT_ACK,
	RTW_WLAN_ACTION_FT_MAX,
};

enum _rtw_ft_sta_status {
	RTW_FT_UNASSOCIATED_STA = 0,
	RTW_FT_AUTHENTICATING_STA,
	RTW_FT_AUTHENTICATED_STA,
	RTW_FT_ASSOCIATING_STA,
	RTW_FT_ASSOCIATED_STA,
	RTW_FT_REQUESTING_STA,
	RTW_FT_REQUESTED_STA,
};

enum rtw_ft_capability {
	RTW_FT_EN = BIT(0),
	RTW_FT_OTD_EN = BIT(1),
	RTW_FT_PEER_EN = BIT(2),
	RTW_FT_PEER_OTD_EN = BIT(3),
};

#define FTIE_SUBELEM_R1KH_ID 1
#define FTIE_SUBELEM_GTK 2
#define FTIE_SUBELEM_R0KH_ID 3

#define RTW_FT_ACTION_REQ_LMT	4
#define RTW_FT_MAX_IE_SZ	256

#define PMK_LEN_MAX 64
#define MOBILITY_DOMAIN_ID_LEN 2
#define FT_R0KH_ID_MAX_LEN 48
#define FT_R1KH_ID_LEN 6
#define WPA_PMK_NAME_LEN 16
#define KEY_NONCE_LEN		32

struct wpa_ie_data {
	int proto;
	int pairwise_cipher;
	int has_pairwise;
	int group_cipher;
	int has_group;
	int key_mgmt;
	int capabilities;
	u32 num_pmkid;
	u8 *pmkid;
	int mgmt_group_cipher;
};

struct ft_ftie {
	u8 mic_control[2];
	u8 mic[16];
	u8 anonce[KEY_NONCE_LEN];
	u8 snonce[KEY_NONCE_LEN];
	/* followed by optional parameters */
};

struct ft_mdie {
	u8 mobility_domain[MOBILITY_DOMAIN_ID_LEN];
	u8 ft_capab;
};


struct rsn_ie_hdr {
	u8 elem_id; /* WLAN_EID_RSN */
	u8 len;
	u8 version[2]; /* little endian */
};

#endif

