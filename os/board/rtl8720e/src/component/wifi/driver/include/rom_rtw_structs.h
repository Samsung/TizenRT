#ifndef __ROM_RTW_STRUCTS_H_
#define __ROM_RTW_STRUCTS_H_

/* Fasten in ROM CODE, revise cautiously. */
typedef struct _RT_CHANNEL_PLAN_MAP {
	unsigned char	ChannelPlan;
	unsigned char	Index2G;
	unsigned char	Index5G;
	unsigned char	PwrLmt2G;
	unsigned char	PwrLmt5G;
} RT_CHANNEL_PLAN_MAP;

typedef struct _RT_CHANNEL_PLAN_2G {
	unsigned char	Channel[MAX_CHANNEL_NUM_2G];
	unsigned char	Len;
} RT_CHANNEL_PLAN_2G;

typedef struct _RT_CHANNEL_PLAN_5G {
	unsigned char	Channel[MAX_CHANNEL_NUM_5G];
	unsigned char	Len;
} RT_CHANNEL_PLAN_5G;

typedef struct _RT_CHANNEL_INFO {
	uint8_t		ChannelNum;		// The channel number.
	uint8_t		ScanType;		// Scan type such as passive or active scan. _RT_SCAN_TYPE
	uint32_t		rx_count;
	uint8_t		hidden_bss_cnt; /* per scan count */
	uint8_t		pscan_config;
} RT_CHANNEL_INFO;

typedef struct _TxPowerInfo24G {
	u8 IndexCCK_Base[RTW_MAX_RF_PATH][MAX_CHNL_GROUP_24G];
	u8 IndexBW40_Base[RTW_MAX_RF_PATH][MAX_CHNL_GROUP_24G];
	//If only one tx, only BW20 and OFDM are used.
	s8 OFDM_Diff[RTW_MAX_RF_PATH][MAX_TX_COUNT];
	s8 BW20_Diff[RTW_MAX_RF_PATH][MAX_TX_COUNT];
#if !defined(NOT_SUPPORT_RF_MULTIPATH)
	s8 CCK_Diff[RTW_MAX_RF_PATH][MAX_TX_COUNT];
	s8 BW40_Diff[RTW_MAX_RF_PATH][MAX_TX_COUNT];
#endif
} TxPowerInfo24G, *PTxPowerInfo24G;

#ifdef SUPPORT_5G_CHANNEL
typedef struct _TxPowerInfo5G {
	u8 IndexBW40_Base[RTW_MAX_RF_PATH][MAX_CHNL_GROUP_5G];
	/* If only one tx, only BW20, OFDM, BW80 and BW160 are used. */
	s8 OFDM_Diff[RTW_MAX_RF_PATH][MAX_TX_COUNT];
	s8 BW20_Diff[RTW_MAX_RF_PATH][MAX_TX_COUNT];
	s8 BW40_Diff[RTW_MAX_RF_PATH][MAX_TX_COUNT];
	s8 BW80_Diff[RTW_MAX_RF_PATH][MAX_TX_COUNT];
	s8 BW160_Diff[RTW_MAX_RF_PATH][MAX_TX_COUNT];
} TxPowerInfo5G, *PTxPowerInfo5G;
#endif

struct halphy_data_t {
#if (PHYDM_VERSION < 3)
	s8	TxPwrByRateOffset[TX_PWR_BY_RATE_NUM_BAND][RTW_TX_PWR_BY_RATE_NUM_RF][RTW_TX_PWR_BY_RATE_NUM_RF][TX_PWR_BY_RATE_NUM_RATE];
	u8	Index24G_CCK_Base[RTW_MAX_RF_PATH][CHANNEL_MAX_NUMBER];
	u8	Index24G_BW40_Base[RTW_MAX_RF_PATH][CHANNEL_MAX_NUMBER];
	//If only one tx, only BW20 and OFDM are used.
	s8	OFDM_24G_Diff[RTW_MAX_RF_PATH][MAX_TX_COUNT];
	s8	BW20_24G_Diff[RTW_MAX_RF_PATH][MAX_TX_COUNT];
#if !defined(NOT_SUPPORT_RF_MULTIPATH)
	s8	CCK_24G_Diff[RTW_MAX_RF_PATH][MAX_TX_COUNT];
	s8	BW40_24G_Diff[RTW_MAX_RF_PATH][MAX_TX_COUNT];
#endif
	/* 5G TX power info for target TX power*/
#if defined(SUPPORT_5G_CHANNEL)
	u8	Index5G_BW40_Base[RTW_MAX_RF_PATH][CENTER_CH_5G_ALL_NUM];
#if !defined(NOT_SUPPORT_80M)
	u8	Index5G_BW80_Base[RTW_MAX_RF_PATH][CENTER_CH_5G_80M_NUM];
#endif
	s8	OFDM_5G_Diff[RTW_MAX_RF_PATH][MAX_TX_COUNT];
	s8	BW20_5G_Diff[RTW_MAX_RF_PATH][MAX_TX_COUNT];
	s8	BW40_5G_Diff[RTW_MAX_RF_PATH][MAX_TX_COUNT];
#if !defined(NOT_SUPPORT_80M)
	s8	BW80_5G_Diff[RTW_MAX_RF_PATH][MAX_TX_COUNT];
#endif
#endif
	/* 2 Power Limit Table */
	/* Power Limit Table for 2.4G */
	s8	TxPwrLimit_2_4G[MAX_REGULATION_NUM][MAX_2_4G_BANDWITH_NUM][MAX_RATE_SECTION_NUM][CHANNEL_MAX_NUMBER_2G][RTW_MAX_RF_PATH];
#if defined(SUPPORT_5G_CHANNEL)
	/* Power Limit Table for 5G */
	s8	TxPwrLimit_5G[MAX_REGULATION_NUM][MAX_5G_BANDWITH_NUM][MAX_RATE_SECTION_NUM][CHANNEL_MAX_NUMBER_5G][RTW_MAX_RF_PATH];
#endif
	/* Store the original power by rate value of the base of each rate section of rf path A & B */
	u8	TxPwrByRateBase2_4G[RTW_TX_PWR_BY_RATE_NUM_RF][RTW_TX_PWR_BY_RATE_NUM_RF][MAX_BASE_NUM_IN_PHY_REG_PG_2_4G];
#if defined(SUPPORT_5G_CHANNEL)
	u8	TxPwrByRateBase5G[RTW_TX_PWR_BY_RATE_NUM_RF][RTW_TX_PWR_BY_RATE_NUM_RF][MAX_BASE_NUM_IN_PHY_REG_PG_5G];
#endif
#endif // (PHYDM_VERSION < 3)
};

struct wps_priv {
	uint8_t *wps_probe_req_ie;
	uint16_t wps_probe_req_ie_len;
	uint8_t *wps_assoc_req_ie;
	uint16_t wps_assoc_req_ie_len;

	uint8_t *wps_beacon_ie;
	uint8_t *wps_probe_resp_ie;
	uint8_t *wps_assoc_resp_ie; // for CONFIG_IOCTL_CFG80211, this IE could include p2p ie

	uint16_t wps_beacon_ie_len;
	uint16_t wps_probe_resp_ie_len;
	uint16_t wps_assoc_resp_ie_len;
};

struct init_chset_parms {
	u8	band_type_support;
	u8	*pRegulation2_4G;
	u8	*pRegulation5G;
	u8	ChannelPlan;
	u8	country_code;
};

struct update_mode_parms {
	u8	*SupportedRates;
	u8	HT_info_enable;
	u8	HT_caps_enable;
	u8	cur_channel;
	u8	he_option;
	u8	*pHT_enable;
	u8	*pcur_wireless_mode;
	u8	wireless_mode_support;
	u8	*puser_tx_rate;
	u8 ht_enable;
	u8	*cur_ch_offset;
	u8 ht_option;
	u8 qos_option;
};

struct security_ie_set {
	u8 *supplicant_ie;
	u8 *rsnxe_ie;
	u8 *pmkid;
	u8 h2e;
	u32 ndisauthmode;
};

struct singal_stat_param {
	u8 avg_signal_snr;
	u8 avg_signal_strength;
	u8 avg_signal_qual;
	int RxSNRdB;
	u8 signal_strength;
	u8 signal_qual;
	u8 tmp_snr;
	u32 tmp_s;
	u32 tmp_q;
};

struct fill_wlan_hdr_parms {
	u8	*pstart;
	u8	*paddr1;
	u8	*paddr2;
	u8	*paddr3;
	u16	frame_ctl;
	u16	*pseq;
	u16	frame_subtype;
	u16	*phdrlen;
	u32	*ppktlen;
};

struct bcn_info_param {
	/* these infor get from rtw_get_encrypt_info when
	 * 	 * translate scan to UI */
	u8	encryp_protocol;	//ENCRYP_PROTOCOL_E: OPEN/WEP/WPA/WPA2/WAPI
	int	group_cipher;		//WPA/WPA2 group cipher
	int	pairwise_cipher;	//WPA/WPA2/WEP pairwise cipher
	int	is_8021x;
	u8 is_1M_beacon;		// check if the beacon uses 1M data rate

	/* bwmode 20/40 and ch_offset UP/LOW */
	unsigned short 	ht_cap_info;
	unsigned char	ht_info_infos_0;
};

struct network_param {
	u32	*BeaconPeriod;
	u8 *Ssid;
	u32  *SsidLength;
	u32           *DSConfig;
	unsigned char *SupportedRates;
	u32 *Privacy;
	u8 *IEs;
	u32 IELength;
};

struct securitypriv_parms {
	u32 *dot11PrivacyAlgrthm;
	u32 *ndisauthtype;
	u32 *dot118021XGrpPrivacy;
	u32 *AuthKeyMgmt;
	u8 *ieee80211w;
	u32 *wep_enable;
	u32 *dot11AuthAlgrthm;

	unsigned int *wpa_psk;//0:disable, bit(0): WPA, bit(1):WPA2
	unsigned int *wpa_group_cipher;
	unsigned int *wpa2_group_cipher;
	unsigned int *wpa_pairwise_cipher;
	unsigned int *wpa2_pairwise_cipher;

	u8 *supplicant_ie;

	unsigned char rtw_cmd_tsk_spt_wap3;
};

struct stainfo_parms {
	uint *qos_option;
	u16 *capability;
	u32 *flags;

	int *dot8021xalg;//0:disable, 1:psk, 2:802.1x
	int *wpa_psk;//0:disable, bit(0): WPA, bit(1):WPA2
	int *wpa_group_cipher;
	int *wpa2_group_cipher;
	int *wpa_pairwise_cipher;
	int *wpa2_pairwise_cipher;

	u8 *wpa_ie;
	u8 wpa_ie_len;

	u8 *qos_info;

	u8 *max_sp_len;
	u8 *uapsd_bk;//BIT(0): Delivery enabled, BIT(1): Trigger enabled
	u8 *uapsd_be;
	u8 *uapsd_vi;
	u8 *uapsd_vo;
	u8 *has_legacy_ac;

	u8 *bssrateset;
	u32	*bssratelen;

	u8 *ht_cap;
	u8 ht_cap_len;
};

struct issue_assoc_req_param {
	struct network_param net_param;

	u8 *supported_rateset;
	u8 rateset_len;
	RT_CHANNEL_INFO *channel_set;

	u8 cur_channel;
};

struct issue_assoc_req_param_2 {
	struct network_param net_param;
	u8 wps_phase;
	u8 ht_option;
	u8 vht_option;
	u8 he_option;
};


struct on_assoc_req_param {
	struct securitypriv_parms secure_params;
	struct network_param net_param;
	struct stainfo_parms sta_param;
	struct update_mode_parms mode_param;
	struct wps_priv *wpspriv;
};

struct issue_assoc_rsp_param {
	u8 *IEs;
	u32 IELength;
	u8 *bssrateset;
	u32 bssratelen;
	u32 sta_flags;
	u8 ht_option;
	u8 qos_option;

	struct wps_priv *wpspriv;
};

struct dot11w_action_params {
	unsigned char *ra;
	unsigned char *bssid;
	unsigned char *sa;
	u16 *mgnt_seq;
	u32 *pktlen;
	unsigned char action;
	unsigned short tid;
	u16	*sa_query_seq;
};

struct signal_stat {
	u8	update_req;		//used to indicate
	u8	avg_val;		//avg of valid elements
	u8	latest_val;		//latest valid elements
	u32	total_num;		//num of valid elements
	u32	total_val;		//sum of valid elements
};

#endif //__ROM_RTW_STRUCTS_H_
