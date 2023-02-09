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
#ifndef __STA_INFO_H_
#define __STA_INFO_H_

#define NUM_ACL 16

//if mode ==0, then the sta is allowed once the addr is hit.
//if mode ==1, then the sta is rejected once the addr is non-hit.
struct rtw_wlan_acl_node {
	_list		        list;
	u8       addr[ETH_ALEN];
	u8       valid;
};

//mode=0, disable
//mode=1, accept unless in deny list
//mode=2, deny unless in accept list
struct wlan_acl_pool {
	int mode;
	int num;
	struct rtw_wlan_acl_node aclnode[NUM_ACL];
	_queue	acl_node_q;
};

typedef struct _RSSI_STA {
	s32	UndecoratedSmoothedPWDB;
	s32	UndecoratedSmoothedCCK;
	s32	UndecoratedSmoothedOFDM;
	u64	PacketMap;
	u8	ValidBit;
	u32	OFDM_pkt;
} RSSI_STA, *PRSSI_STA;

struct	stainfo_stats	{

	u64 rx_mgnt_pkts;
	u64 rx_ctrl_pkts;
	u64 rx_data_pkts;
	u64	rx_bytes;
	u64	tx_bytes;

	u64	last_rx_mgnt_pkts;
	u64	last_rx_ctrl_pkts;
	u64	last_rx_data_pkts;
	u64	last_rx_bytes;
	u64 last_tx_bytes;

	u32 rx_tp_kbits;
	u32 tx_tp_kbits;
};

#ifdef TX_SHORTCUT
struct tx_sc_entry {
	u8 shortcut_hdr[128];//tx_desc(32B) + wlanhdr + snap[6];
	u32 sc_hdr_len;
	u8 ethhdr[14];
	struct pkt_attrib	attrib;
	u32 fr_len;
};
#endif

struct sta_info {

	_lock	lock;
	_list		list; //sta_list
	//_list asoc_list; //20061114
	//_list sleep_list;//sleep_q
	//_list wakeup_list;//wakeup_q
	_adapter *padapter;

	struct sta_xmit_priv sta_xmitpriv;
	struct sta_recv_priv sta_recvpriv;

#if (PHYDM_VERSION == 3)
	struct rtw_phl_stainfo_t *phl_sta;
#else
	struct cmn_sta_info cmn;
#endif

	WPA_STA_INFO		*wpa_sta_info;

#ifdef CONFIG_TWT
	u8 twt_id;	/*bit0 represent id0, .......bit7 represent id7*/
#endif

	_queue sleep_q;
	unsigned int sleepq_len;

	uint state;
	//uint aid;
	//uint mac_id;
	uint qos_option;

	uint	ieee8021x_blocked;	//0: allowed, 1:blocked
	uint	dot118021XPrivacy; //aes, tkip...
	union Keytype	dot11tkiptxmickey;
	union Keytype	dot11tkiprxmickey;
	union Keytype	dot118021x_UncstKey;
	union pn48		dot11txpn;			// PN48 used for Unicast xmit.
	union pn48		dot11rxpn;			// PN48 used for Unicast recv.
#ifdef CONFIG_IEEE80211W
	union pn48		dot11wtxpn;			/* PN48 used for Unicast mgmt xmit. */
	_timer dot11w_expire_timer;
#endif /* CONFIG_IEEE80211W */


	u8	bssrateset[16];
	u32	bssratelen;
	s32	signal_quality;

	u8	cts2self;
	u8	rtsen;

	u8	raid;	//ax useless
	u8 	init_rate;
	struct stainfo_stats sta_stats;

	//for A-MPDU TX, ADDBA timeout check
	_timer addba_retry_timer;
	//for empty defrag queue
	_timer defrag_queue_timer;
	//for A-MPDU Rx reordering buffer control
	struct recv_reorder_ctrl *recvreorder_ctrl[MAXTID];
	//for A-MPDU Tx
	//unsigned char		ampdu_txen_bitmap;
	u16	BA_starting_seqctrl[MAXTID];


	struct ht_priv	htpriv;

#ifdef CONFIG_80211AC_VHT
	struct vht_priv vhtpriv;
#endif

#ifdef CONFIG_80211AX_HE
	struct he_priv hepriv;
#endif

	//Notes:
	//STA_Mode:
	//curr_network(mlme_priv/security_priv/qos/ht) + sta_info: (STA & AP) CAP/INFO
	//scan_q: AP CAP/INFO

	//AP_Mode:
	//curr_network(mlme_priv/security_priv/qos/ht) : AP CAP/INFO
	//sta_info: (AP & STA) CAP/INFO

	u8 bpairwise_key_installed;

	_list asoc_list;
	_list auth_list;

	unsigned int expire_to;
#ifdef CONFIG_AP_POLLING_CLIENT_ALIVE
	unsigned int tx_null0;
	unsigned int tx_null0_fail;
	unsigned int tx_null0_retry;
#endif
	unsigned int auth_seq;
	unsigned int authalg;
	unsigned char chg_txt[128];

	u16 capability;
	u32 flags;

	int dot8021xalg;//0:disable, 1:psk, 2:802.1x
	int wpa_psk;//0:disable, bit(0): WPA, bit(1):WPA2
	int wpa_group_cipher;
	int wpa2_group_cipher;
	int wpa_pairwise_cipher;
	int wpa2_pairwise_cipher;

	u8 wpa_ie[32];

	u8 nonerp_set;
	u8 no_short_slot_time_set;
	u8 no_short_preamble_set;
	u8 no_ht_gf_set;
	u8 no_ht_set;
	u8 ht_20mhz_set;

	unsigned int tx_ra_bitmap;
	u8 qos_info;

	u8 max_sp_len;
	u8 uapsd_bk;//BIT(0): Delivery enabled, BIT(1): Trigger enabled
	u8 uapsd_be;
	u8 uapsd_vi;
	u8 uapsd_vo;

	u8 has_legacy_ac;
	unsigned int sleepq_ac_len;

#ifdef CONFIG_LPS_PG
	u8		lps_pg_rssi_lv;
#endif

	//for DM
	RSSI_STA	 rssi_stat;

	//
	// ================ODM Relative Info=======================
	// Please be care, dont declare too much structure here. It will cost memory * STA support num.
	//
	//
	// 2011/10/20 MH Add for ODM STA info.
	//
	// Driver Write
	u8		bValid;				// record the sta status link or not?
	//u8		WirelessMode;		//
	u8		IOTPeer;			// Enum value.	HT_IOT_PEER_E
	u8		rssi_level;			//for Refresh RA mask
	// ODM Write
	//1 PHY_STATUS_INFO
	u8		RSSI_Path[4];		//
	u8		RSSI_Ave;
	u8		RXEVM[4];
	u8		RXSNR[4];

#ifdef TX_SHORTCUT
	struct tx_sc_entry	tx_sc_ent[TX_SC_ENTRY_NUM];
	int tx_sc_replace_idx;
#endif
#ifdef CONFIG_CSI
	u32 rx_csi_cnt;
#endif
};

#define sta_rx_pkts(sta) \
	(sta->sta_stats.rx_mgnt_pkts \
	+ sta->sta_stats.rx_ctrl_pkts \
	+ sta->sta_stats.rx_data_pkts)

#define sta_last_rx_pkts(sta) \
	(sta->sta_stats.last_rx_mgnt_pkts \
	+ sta->sta_stats.last_rx_ctrl_pkts \
	+ sta->sta_stats.last_rx_data_pkts)

#define sta_update_last_rx_pkts(sta) \
	do { \
		sta->sta_stats.last_rx_mgnt_pkts = sta->sta_stats.rx_mgnt_pkts; \
		sta->sta_stats.last_rx_ctrl_pkts = sta->sta_stats.rx_ctrl_pkts; \
		sta->sta_stats.last_rx_data_pkts = sta->sta_stats.rx_data_pkts; \
	} while(0)

#define STA_RX_PKTS_ARG(sta) \
	sta->sta_stats.rx_mgnt_pkts \
	, sta->sta_stats.rx_ctrl_pkts \
	, sta->sta_stats.rx_data_pkts

#define STA_LAST_RX_PKTS_ARG(sta) \
	sta->sta_stats.last_rx_mgnt_pkts \
	, sta->sta_stats.last_rx_ctrl_pkts \
	, sta->sta_stats.last_rx_data_pkts

#define STA_PKTS_FMT "(m:%llu, c:%llu, d:%llu)"

struct	sta_priv {
	_lock sta_list_lock;
	_list   sta_list;
	int asoc_sta_count;

	_adapter *padapter;

};

extern struct sta_info *rtw_alloc_stainfo(struct	sta_priv *pstapriv, u8 *hwaddr);
extern u32 rtw_free_stainfo_enqueue_cmd(_adapter *padapter, struct sta_info *psta);
extern u32 rtw_free_stainfo(_adapter *padapter, struct sta_info *psta);
extern void rtw_free_all_stainfo(_adapter *padapter);
extern struct sta_info *rtw_get_stainfo(struct sta_priv *pstapriv, u8 *hwaddr);
extern u32 rtw_init_bcmc_stainfo(_adapter *padapter);
extern struct sta_info *rtw_get_bcmc_stainfo(_adapter *padapter);
extern u8 rtw_access_ctrl(_adapter *padapter, u8 *mac_addr);
extern struct recv_reorder_ctrl *rtw_alloc_reorder_ctrl(_adapter *padapter);
extern void rtw_free_reorder_ctrl(_adapter *padapter, struct sta_info *psta, u8 tid);
#endif //_STA_INFO_H_

