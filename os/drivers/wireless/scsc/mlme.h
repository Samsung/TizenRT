/*****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

#ifndef __SLSI_MLME_H__
#define __SLSI_MLME_H__

#include "dev.h"
#include "mib.h"
#include "wpa_driver_copy.h"

enum slsi_ac_index_wmm_pe {
	AC_BE,
	AC_BK,
	AC_VI,
	AC_VO
};

#define SLSI_FREQ_FW_TO_HOST(f) ((f) / 2)
#define SLSI_FREQ_HOST_TO_FW(f) ((f) * 2)

#define SLSI_SINFO_MIB_ACCESS_TIMEOUT (1000)	/* 1 sec timeout */

#define SLSI_WLAN_EID_VENDOR_SPECIFIC 0xdd
#define SLSI_WLAN_EID_INTERWORKING 107
#define SLSI_WLAN_EID_EXT_CAPABILITY 127

#define SLSI_AP_EXT_CAPAB_IE_LEN 10	/* EID (1) + Len (1) + Ext Capab (8) */

#define SLSI_SCAN_DONE_IND_WAIT_TIMEOUT 40000	/* 40 seconds */

#ifdef CONFIG_SCSC_ENABLE_P2P
/* P2P (Wi-Fi Direct) */
#define SLSI_P2P_WILDCARD_SSID "DIRECT-"
#define SLSI_P2P_WILDCARD_SSID_LENGTH 7
#define SLSI_P2P_SOCIAL_CHAN_COUNT      3

/* A join scan with P2P GO SSID can come and hence the SSID length comparision should include >= */
#define SLSI_IS_P2P_SSID(ssid, ssid_len) ((ssid_len >= SLSI_P2P_WILDCARD_SSID_LENGTH) && \
					  (memcmp(ssid, SLSI_P2P_WILDCARD_SSID, SLSI_P2P_WILDCARD_SSID_LENGTH) == 0))
#endif

/* Action frame categories for registering with firmware */
#define SLSI_ACTION_FRAME_PUBLIC        (1 << 4)
#define SLSI_ACTION_FRAME_VENDOR_SPEC_PROTECTED (1 << 30)
#define SLSI_ACTION_FRAME_VENDOR_SPEC   (1 << 31)
#define SLSI_ACTION_FRAME_WMM     (1 << 17)

/* Firmware transmit rates */
#define SLSI_TX_RATE_NON_HT_1MBPS 0x4001
#define SLSI_TX_RATE_NON_HT_6MBPS 0x4004

#define WLAN_EID_WAPI 68

/**
 * If availability_duration is set to SLSI_FW_CHANNEL_DURATION_UNSPECIFIED
 * then the firmware autonomously decides how long to remain listening on
 * the configured channel.
 */
#define SLSI_FW_CHANNEL_DURATION_UNSPECIFIED             (0x0000)

extern struct slsi_80211_supported_band slsi_band_2ghz;
#ifdef CONFIG_SCSC_ADV_FEATURE
extern struct slsi_80211_supported_band slsi_band_5ghz;
extern struct ieee80211_sta_vht_cap slsi_vht_cap;
#endif

/* Packet Filtering */
#define SLSI_MAX_PATTERN_DESC    4	/* We are not using more than 4 pattern descriptors in a pkt filter */
#define SLSI_PKT_DESC_FIXED_LEN 2	/* offset (1) + mask length (1) */
#define SLSI_PKT_FILTER_ELEM_FIXED_LEN  6	/* oui(3) + oui type(1) + filter id (1) + pkt filter mode(1) */
#define SLSI_PKT_FILTER_ELEM_HDR_LEN  (2 + SLSI_PKT_FILTER_ELEM_FIXED_LEN)	/* element id + len + SLSI_PKT_FILTER_ELEM_FIXED_LEN */
#define SLSI_MAX_PATTERN_LENGTH 6

/* Signal wait timeout in milliseconds */
#define SLSI_SIG_WAIT_CFM_TIMEOUT  (10 * 1000)

struct slsi_mlme_pattern_desc {
	u8 offset;
	u8 mask_length;
	u8 mask[SLSI_MAX_PATTERN_LENGTH];
	u8 pattern[SLSI_MAX_PATTERN_LENGTH];
};

struct slsi_mlme_pkt_filter_elem {
	u8 header[SLSI_PKT_FILTER_ELEM_HDR_LEN];
	u8 num_pattern_desc;
	struct slsi_mlme_pattern_desc pattern_desc[SLSI_MAX_PATTERN_DESC];
};

u16 slsi_get_chann_info(struct slsi_dev *sdev, struct hostapd_freq_params *chandef);
int slsi_check_channelization(struct slsi_dev *sdev, struct slsi_80211_chan_def *chandef);
#ifndef CONFIG_SCSC_WLAN_BLOCK_IPV6
int slsi_mlme_set_ipv6_address(struct slsi_dev *sdev, struct netif *dev);
#endif
int slsi_mlme_set(struct slsi_dev *sdev, struct netif *dev, u8 *req, int req_len);
int slsi_mlme_get(struct slsi_dev *sdev, struct netif *dev, u8 *req, int req_len, u8 *resp, int resp_buf_len, int *resp_len);

int slsi_mlme_add_vif(struct slsi_dev *sdev, struct netif *dev, u8 *interface_address, u8 *device_address);
void slsi_mlme_del_vif(struct slsi_dev *sdev, struct netif *dev);
int slsi_mlme_set_channel(struct slsi_dev *sdev, struct netif *dev, struct slsi_80211_channel *chan, u16 duration, u16 interval, u16 count);
const u8 *slsi_80211_find_ie(u8 eid, const u8 *ies, int len);
u8 *slsi_80211_find_ie_mod(u8 eid, u8 *ies, int len);
const u8 *slsi_80211_find_vendor_ie(unsigned int oui, u8 oui_type, const u8 *ies, int len);

/**
 * slsi_mlme_add_scan() Returns:
 *  0 : Scan installed
 * >0 : Scan NOT installed. Not an Error
 * <0 : Scan NOT installed. Error
 */
int slsi_mlme_add_scan(struct slsi_dev *sdev, struct netif *dev, u16 scan_type, u16 report_mode, u32 n_ssids, struct wpa_driver_scan_ssid *ssids, u32 n_channels, struct slsi_80211_channel *channels[], void *gscan_param, const u8 *ies, u16 ies_len, bool wait_for_ind);

int slsi_mlme_del_scan(struct slsi_dev *sdev, struct netif *dev, u16 scan_id);
int slsi_mlme_start(struct slsi_dev *sdev, struct netif *dev, u8 *bssid, struct wpa_driver_ap_params *settings, const u8 *wpa_ie_pos, const u8 *wmm_ie_pos, bool append_vht_ies);
int slsi_mlme_connect(struct slsi_dev *sdev, struct netif *dev, struct wpa_driver_associate_params *sme);
int slsi_mlme_set_key(struct slsi_dev *sdev, struct netif *dev, u16 key_id, u16 key_type, const u8 *address, struct slsi_key_params *key);
int slsi_mlme_get_key(struct slsi_dev *sdev, struct netif *dev, u16 key_id, u16 key_type, u8 *seq, int *seq_len);
int slsi_mlme_set_cckm_key(struct slsi_dev *sdev, struct netif *dev, u8 *gk_key);

/**
 * Sends MLME-DISCONNECT-REQ and waits for the MLME-DISCONNECT-CFM
 * MLME-DISCONNECT-CFM only indicates if the firmware has accepted the request (or not)
 * the actual end of the disconnection is indicated by the firmware sending
 * MLME-DISCONNECT-IND (following a successful MLME-DISCONNECT-CFM).
 * The host has to wait for the full exchange to complete with the firmware before
 * returning to cfg80211 if it made the disconnect request. Due to this, this function
 * waits for both the MLME-DISCONNECT-CFM and the MLME-DISCONNECT-IND (if the
 * MLME-DISCONNECT-CFM was successful)
 */
int slsi_mlme_disconnect(struct slsi_dev *sdev, struct netif *dev, u8 *bssid, u16 reason_code, bool wait_ind);

int slsi_mlme_req(struct slsi_dev *sdev, struct netif *dev, struct max_buff *mbuf);
struct max_buff *slsi_mlme_req_no_cfm(struct slsi_dev *sdev, struct netif *dev, struct max_buff *mbuf);

struct max_buff *slsi_mlme_req_ind(struct slsi_dev *sdev, struct netif *dev, struct max_buff *mbuf, u16 ind_id);
/* Reads multiple MIB data related to station info. returns 0 if success else errno*/
int slsi_mlme_get_sinfo_mib(struct slsi_dev *sdev, struct netif *dev, struct wpa_signal_info *si);
#ifdef CONFIG_SCSC_WLAN_POWER_SAVE
int slsi_mlme_powermgt(struct slsi_dev *sdev, struct netif *dev, u16 ps_mode);
int slsi_mlme_powermgt_unlocked(struct slsi_dev *sdev, struct netif *dev, u16 ps_mode);
#endif
int slsi_mlme_register_action_frame(struct slsi_dev *sdev, struct netif *dev, u32 af_bitmap_active, u32 af_bitmap_suspended);
int slsi_mlme_channel_switch(struct slsi_dev *sdev, struct netif *dev, u16 center_freq, u16 chan_info);
int slsi_mlme_add_info_elements(struct slsi_dev *sdev, struct netif *dev, u16 purpose, const u8 *ies, const u16 ies_len);
int slsi_mlme_send_frame_mgmt(struct slsi_dev *sdev, struct netif *dev, const u8 *frame, int frame_len, u16 data_desc, u16 msg_type, u16 host_tag, u16 freq, u32 dwell_time, u32 period);
int slsi_mlme_send_frame_data(struct slsi_dev *sdev, struct netif *dev, struct max_buff *mbuf, u16 msg_type);
#ifdef CONFIG_SCSC_ENABLE_P2P
int slsi_mlme_reset_dwell_time(struct slsi_dev *sdev, struct netif *dev);
#endif
int slsi_mlme_set_packet_filter(struct slsi_dev *sdev, struct netif *dev, int pkt_filter_len, u8 num_filters, struct slsi_mlme_pkt_filter_elem *pkt_filter_elems);
void slsi_mlme_connect_resp(struct slsi_dev *sdev, struct netif *dev);
void slsi_mlme_connected_resp(struct slsi_dev *sdev, struct netif *dev, u16 peer_index);
int slsi_mlme_set_pmk(struct slsi_dev *sdev, struct netif *dev, const u8 *pmk, u16 pmklen);
#ifdef CONFIG_SCSC_ADV_FEATURE
void slsi_ap_obss_scan_done_ind(struct netif *dev, struct netdev_vif *ndev_vif);
int slsi_mlme_set_acl(struct slsi_dev *sdev, struct netif *dev, const struct cfg80211_acl_data *params);
#endif
int slsi_mlme_blockack_control_req(struct slsi_dev *sdev, struct netif *dev, u16 blockack_control_bitmap, u16 direction, const u8 *peer_sta_address);

int slsi_mlme_set_ext_capab(struct slsi_dev *sdev, struct netif *dev, struct slsi_mib_value *mib_val);
int slsi_mlme_set_iw_ext_cap(struct slsi_dev *sdev, struct netif *dev, const u8 *ies, int ie_len);
int slsi_modify_ies(struct netif *dev, u8 eid, u8 *ies, int ies_len, u8 ie_index, u8 ie_value);
#endif /*__SLSI_MLME_H__*/
