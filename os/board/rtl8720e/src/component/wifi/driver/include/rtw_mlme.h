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
#ifndef __RTW_MLME_H_
#define __RTW_MLME_H_

#define	MAX_BSS_CNT	1	//alloc less wlan_network due to memory limitation - Alex Fang

//#define   MAX_JOIN_TIMEOUT	2000
//#define   MAX_JOIN_TIMEOUT	2500
#define   MAX_JOIN_TIMEOUT	6500

#ifdef CONFIG_MULTICAST
#define MULTICAST_LIST_SIZE		4
#endif //CONFIG_MULTICAST

//	Commented by Albert 20101105
//	Increase the scanning timeout because of increasing the SURVEY_TO value.

#if defined(CONFIG_BT_COEXIST_SOC) || defined(CONFIG_BT_COEXIST)
#define 	SCANNING_TIMEOUT 	12000
#else
#define 	SCANNING_TIMEOUT 	8000
#endif //CONFIG_BT_COEXIST_SOC

#define	SCAN_INTERVAL	(30) // unit:2sec, 30*2=60sec

#ifdef PALTFORM_OS_WINCE
#define	SCANQUEUE_LIFETIME 12000000 // unit:us
#else
#define	SCANQUEUE_LIFETIME 20 // unit:sec
#endif //PALTFORM_OS_WINCE

#define MLME_IS_PD(adapter) 0
#define MLME_IS_GC(adapter) 0
#define MLME_IS_GO(adapter) 0

#define 	WIFI_NULL_STATE		0x00000000
#define	WIFI_ASOC_STATE		0x00000001		// Under Linked state...
#define 	WIFI_REASOC_STATE	       0x00000002
#define	WIFI_SLEEP_STATE	       0x00000004
#define	WIFI_STATION_STATE	0x00000008
#define	WIFI_AP_STATE				0x00000010
#define	WIFI_ADHOC_STATE			0x00000020
#define   WIFI_ADHOC_MASTER_STATE 0x00000040
#define   WIFI_UNDER_LINKING		0x00000080
#define WIFI_MESH_STATE					0x00000200
//#define WIFI_UNDER_CMD			0x00000200
// ========== P2P Section Start ===============
#define	WIFI_P2P_LISTEN_STATE		0x00010000
#define	WIFI_P2P_GROUP_FORMATION_STATE		0x00020000
// ========== P2P Section End ===============
#define WIFI_UNDER_WPS			0x00000100
#define WIFI_SITE_MONITOR		0x00000800		//to indicate the station is under site surveying

#ifdef WDS
#define	WIFI_WDS				0x00001000
#define	WIFI_WDS_RX_BEACON	0x00002000		// already rx WDS AP beacon
#endif
#ifdef AUTO_CONFIG
#define	WIFI_AUTOCONF			0x00004000
#define	WIFI_AUTOCONF_IND	0x00008000
#endif

//#ifdef UNDER_MPTEST
#define	WIFI_MP_STATE						0x00010000
#define	WIFI_MP_CTX_BACKGROUND			0x00020000	// in continous tx background
#define	WIFI_MP_CTX_ST					0x00040000	// in continous tx with single-tone
#define	WIFI_MP_CTX_BACKGROUND_PENDING	0x00080000	// pending in continous tx background due to out of skb
#define	WIFI_MP_CTX_CCK_HW				0x00100000	// in continous tx
#define	WIFI_MP_CTX_CCK_CS				0x00200000	// in continous tx with carrier suppression
#define   WIFI_MP_LPBK_STATE				0x00400000
//#endif
#define WIFI_OP_CH_SWITCHING			0x00800000

enum {
	MLME_ACTION_UNKNOWN,
	MLME_ACTION_NONE,
	MLME_SCAN_ENABLE, /* WIFI_SITE_MONITOR */
	MLME_SCAN_ENTER, /* WIFI_SITE_MONITOR && !SCAN_DISABLE && !SCAN_BACK_OP */
	MLME_SCAN_DONE, /*  WIFI_SITE_MONITOR && (SCAN_DISABLE || SCAN_BACK_OP) */
	MLME_SCAN_DISABLE, /* WIFI_SITE_MONITOR is going to be cleared */
	MLME_STA_CONNECTING,
	MLME_STA_CONNECTED,
	MLME_STA_DISCONNECTED,
	MLME_TDLS_LINKED,
	MLME_TDLS_NOLINK,
	MLME_AP_STARTED,
	MLME_AP_STOPPED,
	MLME_ADHOC_STARTED,
	MLME_ADHOC_STOPPED,
	MLME_MESH_STARTED,
	MLME_MESH_STOPPED,
	MLME_OPCH_SWITCH,
};


//#define _FW_UNDER_CMD		WIFI_UNDER_CMD
#define _FW_UNDER_LINKING	WIFI_UNDER_LINKING
#define _FW_LINKED			WIFI_ASOC_STATE
#define _FW_UNDER_SURVEY	WIFI_SITE_MONITOR

enum dot11AuthAlgrthmNum {
	dot11AuthAlgrthm_Open = 0,
	dot11AuthAlgrthm_Shared,
	dot11AuthAlgrthm_8021X,
	dot11AuthAlgrthm_SAE,
	dot11AuthAlgrthm_Auto,
	dot11AuthAlgrthm_WAPI,
	dot11AuthAlgrthm_FT_PSK,
	dot11AuthAlgrthm_MaxNum
};

/**
 * enum mfp_options - Management frame protection (IEEE 802.11w) options
 */
enum mfp_options {
	NO_MGMT_FRAME_PROTECTION = 0,
	MGMT_FRAME_PROTECTION_OPTIONAL = 1,
	MGMT_FRAME_PROTECTION_REQUIRED = 2,
};
#define MGMT_FRAME_PROTECTION_DEFAULT 3

#define WPA_CAPABILITY_MFPR BIT(6)
#define WPA_CAPABILITY_MFPC BIT(7)

#define adapter_mac_addr(adapter) (adapter->eeprompriv.mac_addr)

// Scan type including active and passive scan.
typedef enum _RT_SCAN_TYPE {
	SCAN_PASSIVE = 1,
	SCAN_ACTIVE,
	SCAN_MIX,
} RT_SCAN_TYPE, *PRT_SCAN_TYPE;

struct scan_priv {
	u8										scan_type_user_config;
	rtw_channel_scan_time_t 				chan_scan_time;
	u8										hidden_ssid_disable;
	u16 									max_ap_record_num;
	_queue 									scan_report_list;
	u16 									scan_cnt;
	u8										report_each_mode_enable; /* Every time a AP is scanned, it will be reported to user immediately */
	void 									*scan_user_data;
	scan_user_callback_t					scan_user_callback;
	scan_report_each_mode_user_callback_t	scan_report_each_mode_user_callback;
};

typedef struct rtw_scan_report_node {
	_list list;
	rtw_scan_result_t scanned_AP_info;
} rtw_scan_report_node_t;

/*

there are several "locks" in mlme_priv,
since mlme_priv is a shared resource between many threads,
like ISR/Call-Back functions, the OID handlers, and even timer functions.


Each _queue has its own locks, already.
Other items are protected by mlme_priv.lock.

To avoid possible dead lock, any thread trying to modifiying mlme_priv
SHALL not lock up more than one locks at a time!

*/


#define traffic_threshold	10
#define	traffic_scan_period	500

struct sitesurvey_ctrl {
	u64	last_tx_pkts;
	uint	last_rx_pkts;
	sint	traffic_busy;
	_timer	sitesurvey_ctrl_timer;
};

typedef struct _RT_LINK_DETECT_T {
	u32			NumTxOkInPeriod;
	u32			NumRxOkInPeriod;
	u32			NumRxUnicastOkInPeriod;
	u32			NumRxBcnInPeriod;
	BOOLEAN			bBusyTraffic;
	BOOLEAN			bTxBusyTraffic;
	BOOLEAN			bRxBusyTraffic;
	BOOLEAN			bHigherBusyTraffic;	// For interrupt migration purpose.
	BOOLEAN			bHigherBusyRxTraffic;	// We may disable Tx interrupt according as Rx traffic.
	BOOLEAN			bHigherBusyTxTraffic;	// We may disable Tx interrupt according as Tx traffic.
} RT_LINK_DETECT_T, *PRT_LINK_DETECT_T;

struct wifidirect_info {
	enum P2P_ROLE			role;
	enum P2P_STATE			p2p_state;
	u8						baction_tx_pending;
	u8						pending_peer[ETH_ALEN];
	struct xmit_frame			*pending_action;
	_timer					pre_tx_scan_timer;
};

#ifdef CONFIG_LAYER2_ROAMING
#define RTW_ROAM_SCAN_RESULT_EXP_MS (5*1000)
#define RTW_ROAM_RSSI_DIFF_TH 10
#define RTW_ROAM_SCAN_INTERVAL (5)    /* 5*(2 second)*/
#define RTW_ROAM_RSSI_THRESHOLD 50

enum {
	RTW_ROAM_ON_EXPIRED = BIT(0),
	RTW_ROAM_ON_RESUME = BIT(1),
	RTW_ROAM_ACTIVE = BIT(2),
	RTW_ROAM_ON_BTM = BIT(3),
};

enum {
	RTW_NO_ROAM = 0,
	RTW_ROAM_BTM_RUNNING = 1,
	RTW_ROAM_LAYER2_RUNNING = 2,
};
#endif

struct mlme_priv {

	_lock	lock;
	sint	fw_state;	//shall we protect this variable? maybe not necessarily...
	u8 bScanInProcess;
	u8	to_join; //flag
	u8 	rx_stbc;

#ifdef CONFIG_LAYER2_ROAMING
	u8 roam_flags;
	u8 roam_status;
	u8 to_roam; /* roaming trying times */
	struct wlan_network *roam_network; /* the target of active roam */
	u8 roam_rssi_diff_th; /* rssi difference threshold for active scan candidate selection */
	u32 roam_scan_int;		/* scan interval for active roam (Unit:2 second)*/
	u32 roam_scanr_exp_ms; /* scan result expire time in ms  for roam */
	u8 roam_tgt_addr[ETH_ALEN]; /* request to roam to speicific target without other consideration */
	u8 roam_rssi_threshold;
	u32 last_roaming;
	bool need_to_roam;
	u8 previous_ap[ETH_ALEN];
#endif

	u8		*nic_hdl;

	//u8		not_indic_disco;
	_list		*pscanned;
	_queue	free_bss_pool;
	_queue	scanned_queue;
	u8		*free_bss_buf;
	u16		num_of_scanned;

	struct scan_priv scanpriv;

	NDIS_802_11_SSID	assoc_ssid;
	u8	assoc_bssid[6];

#ifdef CONFIG_WOWLAN
	u8	ip_addr[4];
#endif

	struct wlan_network	cur_network;

	//uint wireless_mode; no used, remove it

	_timer assoc_timer;

	u8 assoc_by_bssid;

	_timer scan_to_timer; // driver itself handles scan_timeout status.
	u32 scan_start_time; // used to evaluate the time spent in scanning

	struct qos_priv qospriv;

	/* Number of non-HT AP/stations */
	u16 num_sta_no_ht; //int num_sta_no_ht;

	/* Number of HT AP/stations 20 MHz */
	//int num_sta_ht_20mhz;


	u16 num_FortyMHzIntolerant; //int num_FortyMHzIntolerant;

	struct ht_priv	htpriv;


#ifdef CONFIG_80211AC_VHT
	struct vht_priv vhtpriv;
#endif

#ifdef CONFIG_80211AX_HE
	struct he_priv hepriv;
#endif

	RT_LINK_DETECT_T	LinkDetectInfo;
	_timer	dynamic_chk_timer; //dynamic/periodic check timer

	u8 	key_mask; //use for ips to set wep key after ips_leave
	u8	acm_mask; // for wmm acm mask
	u8	ChannelPlan;
	RT_SCAN_TYPE 	scan_mode; // active: 1, passive: 0

#ifdef CONFIG_WPS
	u8 *wps_probe_req_ie;
	u32 wps_probe_req_ie_len;
	u8 *wps_assoc_req_ie;
	u32 wps_assoc_req_ie_len;
#endif
#ifdef CONFIG_IEEE80211R
	u8	ft_flags;
	struct ft_roam_info *ft_info;
#endif
#if defined(CONFIG_RTW_WNM) || defined(CONFIG_IEEE80211K)
	struct roam_nb_info *nb_info;
#endif
	/* Number of associated Non-ERP stations (i.e., stations using 802.11b
	 * in 802.11g BSS) */
	u16 num_sta_non_erp;

	/* Number of associated stations that do not support Short Slot Time */
	u16 num_sta_no_short_slot_time;

	/* Number of associated stations that do not support Short Preamble */
	u16 num_sta_no_short_preamble;

	//int olbc; /* Overlapping Legacy BSS Condition */

	/* Number of HT associated stations that do not support greenfield */
	u16 num_sta_ht_no_gf;

	/* Number of associated non-HT stations */
	//int num_sta_no_ht;

	/* Number of HT associated stations 20 MHz */
	u16 num_sta_ht_20mhz;

	u8 num_sta_no_wmm;

	/* Overlapping BSS information */
	u8 olbc_ht;

	u16 ht_op_mode;

#ifdef CONFIG_WPS
	u8 *wps_beacon_ie;
	u8 *wps_probe_resp_ie;
	u8 *wps_assoc_resp_ie; // for CONFIG_IOCTL_CFG80211, this IE could include p2p ie

	u32 wps_beacon_ie_len;
	u32 wps_probe_resp_ie_len;
	u32 wps_assoc_resp_ie_len;


#endif //CONFIG_WPS

	_lock	bcn_update_lock;
	u8		update_bcn;

	u8 ori_ch;
	u8 ori_bw;
#ifdef CONFIG_80211AC_VHT
	u8 ori_vht_en;
#endif
	u32 lastscantime;

#ifdef CONFIG_MULTICAST
	u32 multicast_list[MULTICAST_LIST_SIZE];
#endif
	//For fast reconnection to keep frame info temporarily
	union recv_frame *p_copy_recv_frame;
	unsigned short disassoc_reason;
};

#define RTW_AUTO_SCAN_REASON_UNSPECIFIED		0
#define RTW_AUTO_SCAN_REASON_2040_BSS			BIT(0)
#define RTW_AUTO_SCAN_REASON_ACS				BIT(1)
#define RTW_AUTO_SCAN_REASON_ROAM				BIT(2)
#define RTW_AUTO_SCAN_REASON_MESH_OFFCH_CAND	BIT(3)

extern void rtw_joinbss_event_prehandle(_adapter *adapter, u8 *pbuf);
extern void rtw_survey_event_callback(_adapter *adapter, u8 *pbuf);
extern void rtw_surveydone_event_callback(_adapter *adapter, u8 *pbuf);
extern void rtw_joinbss_event_callback(_adapter *adapter, u8 *pbuf);
extern void rtw_stassoc_event_callback(_adapter *adapter, u8 *pbuf);
extern void rtw_stadel_event_callback(_adapter *adapter, u8 *pbuf);
#if defined(CONFIG_IEEE80211W)
extern void rtw_sta_timeout_event_callback(_adapter *adapter, u8 *pbuf);
#endif /* CONFIG_IEEE80211W */
extern void rtw_atimdone_event_callback(_adapter *adapter, u8 *pbuf);
extern void rtw_cpwm_event_callback(_adapter *adapter, u8 *pbuf);

extern void rtw_free_network_queue(_adapter *adapter, u8 isfreeall);
extern int rtw_init_mlme_priv(_adapter *adapter);// (struct mlme_priv *pmlmepriv);

extern void rtw_free_mlme_priv(struct mlme_priv *pmlmepriv);


extern sint rtw_select_and_join_from_scanned_queue(struct mlme_priv *pmlmepriv);
extern sint rtw_set_key(_adapter *adapter, struct security_priv *psecuritypriv, sint keyid, u8 set_tx);
extern sint rtw_set_auth(_adapter *adapter, struct security_priv *psecuritypriv);
extern sint rtw_linked_check(_adapter *padapter);

__inline static u8 *get_bssid(struct mlme_priv *pmlmepriv)
{
	//if sta_mode:pmlmepriv->cur_network.network.MacAddress=> bssid
	// if adhoc_mode:pmlmepriv->cur_network.network.MacAddress=> ibss mac address
	return pmlmepriv->cur_network.network.MacAddress;
}

__inline static sint check_fwstate(struct mlme_priv *pmlmepriv, sint state)
{
	if (pmlmepriv->fw_state & state) {
		return _TRUE;
	}

	return _FALSE;
}

__inline static sint get_fwstate(struct mlme_priv *pmlmepriv)
{
	return pmlmepriv->fw_state;
}

/*
 * No Limit on the calling context,
 * therefore set it to be the critical section...
 *
 * ### NOTE:#### (!!!!)
 * MUST TAKE CARE THAT BEFORE CALLING THIS FUNC, YOU SHOULD HAVE LOCKED pmlmepriv->lock
 */
__inline static void set_fwstate(struct mlme_priv *pmlmepriv, sint state)
{
	pmlmepriv->fw_state |= state;
	//FOR HW integration
	if (_FW_UNDER_SURVEY == state) {
		pmlmepriv->bScanInProcess = _TRUE;
	}
}

__inline static void _clr_fwstate_(struct mlme_priv *pmlmepriv, sint state)
{
	pmlmepriv->fw_state &= ~state;
	//FOR HW integration
	if (_FW_UNDER_SURVEY == state) {
		pmlmepriv->bScanInProcess = _FALSE;
	}
}

/*
 * No Limit on the calling context,
 * therefore set it to be the critical section...
 */
__inline static void clr_fwstate(struct mlme_priv *pmlmepriv, sint state)
{
	_irqL irqL;

	rtw_enter_critical_bh(&pmlmepriv->lock, &irqL);
	if (check_fwstate(pmlmepriv, state) == _TRUE) {
		pmlmepriv->fw_state ^= state;
	}
	rtw_exit_critical_bh(&pmlmepriv->lock, &irqL);
}

__inline static void clr_fwstate_ex(struct mlme_priv *pmlmepriv, sint state)
{
	_irqL irqL;

	rtw_enter_critical_bh(&pmlmepriv->lock, &irqL);
	_clr_fwstate_(pmlmepriv, state);
	rtw_exit_critical_bh(&pmlmepriv->lock, &irqL);
}

__inline static void up_scanned_network(struct mlme_priv *pmlmepriv)
{
	_irqL irqL;

	rtw_enter_critical_bh(&pmlmepriv->lock, &irqL);
	pmlmepriv->num_of_scanned++;
	rtw_exit_critical_bh(&pmlmepriv->lock, &irqL);
}

#ifdef CONFIG_CONCURRENT_MODE
sint rtw_buddy_adapter_up(_adapter *padapter);
sint check_buddy_fwstate(_adapter *padapter, sint state);
#endif //CONFIG_CONCURRENT_MODE

__inline static void down_scanned_network(struct mlme_priv *pmlmepriv)
{
	_irqL irqL;

	rtw_enter_critical_bh(&pmlmepriv->lock, &irqL);
	pmlmepriv->num_of_scanned--;
	rtw_exit_critical_bh(&pmlmepriv->lock, &irqL);
}

__inline static void set_scanned_network_val(struct mlme_priv *pmlmepriv, sint val)
{
	_irqL irqL;

	rtw_enter_critical_bh(&pmlmepriv->lock, &irqL);
	pmlmepriv->num_of_scanned = val;
	rtw_exit_critical_bh(&pmlmepriv->lock, &irqL);
}

extern u16 rtw_get_capability(WLAN_BSSID_EX *bss);
extern void rtw_update_scanned_network(_adapter *adapter, WLAN_BSSID_EX *target);
extern void rtw_disconnect_hdl_under_linked(_adapter *adapter, struct sta_info *psta, u8 free_assoc);
extern void rtw_generate_random_ibss(u8 *pibss);
extern struct wlan_network *rtw_find_network(_queue *scanned_queue, u8 *addr);
extern struct wlan_network *rtw_get_oldest_wlan_network(_queue *scanned_queue);

extern void rtw_free_assoc_resources(_adapter *adapter, int lock_scanned_queue);
extern void rtw_indicate_disconnect(_adapter *adapter);
extern void rtw_indicate_connect(_adapter *adapter);
void rtw_scan_abort(_adapter *adapter);

extern int rtw_restruct_sec_ie(_adapter *adapter, u8 *in_ie, u8 *out_ie, uint in_len);
extern int rtw_restruct_wmm_ie(_adapter *adapter, u8 *in_ie, u8 *out_ie, uint in_len, uint initial_out_len);
//extern void rtw_init_registrypriv_dev_network(_adapter *adapter);

//extern void rtw_update_registrypriv_dev_network(_adapter *adapter);

extern void rtw_get_encrypt_decrypt_from_registrypriv(_adapter *adapter);

extern void _rtw_join_timeout_handler(_adapter *adapter);
extern void rtw_scan_timeout_handler(_adapter *adapter);

extern u8 rtw_dynamic_check_timer_handlder(_adapter *adapter);

extern int _rtw_init_mlme_priv(_adapter *padapter);

void rtw_free_mlme_priv_ie_data(struct mlme_priv *pmlmepriv);

extern void _rtw_free_mlme_priv(struct mlme_priv *pmlmepriv);

extern int _rtw_enqueue_network(_queue *queue, struct wlan_network *pnetwork);

extern struct wlan_network *_rtw_dequeue_network(_queue *queue);

extern struct wlan_network *_rtw_alloc_network(struct mlme_priv *pmlmepriv);


extern void _rtw_free_network(struct mlme_priv *pmlmepriv, struct wlan_network *pnetwork, u8 isfreeall);
extern void _rtw_free_network_nolock(struct mlme_priv *pmlmepriv, struct wlan_network *pnetwork);


extern struct wlan_network *_rtw_find_network(_queue *scanned_queue, u8 *addr);

extern void _rtw_free_network_queue(_adapter *padapter, u8 isfreeall);

extern sint rtw_if_up(_adapter *padapter);


u8 *rtw_get_capability_from_ie(u8 *ie);
u8 *rtw_get_timestampe_from_ie(u8 *ie);
u8 *rtw_get_beacon_interval_from_ie(u8 *ie);


void rtw_joinbss_reset(_adapter *padapter);

unsigned int rtw_restructure_ht_ie(_adapter *padapter, u8 *in_ie, u8 *out_ie, uint in_len, uint *pout_len);
void rtw_update_ht_cap(_adapter *padapter, u8 *pie, uint ie_len);
void rtw_issue_addbareq_cmd(_adapter *padapter, struct xmit_frame *pxmitframe);

int rtw_is_same_ibss(_adapter *adapter, struct wlan_network *pnetwork);
int is_same_network(WLAN_BSSID_EX *src, WLAN_BSSID_EX *dst);

#ifdef CONFIG_LAYER2_ROAMING
#define rtw_roam_flags(adapter) ((adapter)->mlmepriv.roam_flags)
#define rtw_chk_roam_flags(adapter, flags) ((adapter)->mlmepriv.roam_flags & flags)
#define rtw_clr_roam_flags(adapter, flags) \
	do { \
		((adapter)->mlmepriv.roam_flags &= ~flags); \
	} while (0)

#define rtw_set_roam_flags(adapter, flags) \
	do { \
		((adapter)->mlmepriv.roam_flags |= flags); \
	} while (0)

#define rtw_assign_roam_flags(adapter, flags) \
	do { \
		((adapter)->mlmepriv.roam_flags = flags); \
	} while (0)

void _rtw_roaming(_adapter *adapter, struct wlan_network *tgt_network);
void rtw_roaming(_adapter *adapter, struct wlan_network *tgt_network);
void rtw_set_to_roam(_adapter *adapter, u8 to_roam);
u8 rtw_dec_to_roam(_adapter *adapter);
u8 rtw_to_roam(_adapter *adapter);
void rtw_roaming_sitesurvey_again(_adapter *padapter);
void rtw_start_roam(_adapter *padapter);
void rtw_drv_scan_by_self(_adapter *padapter, u8 reason);
void rtw_roamimg_info_init(_adapter *padapter);
int rtw_select_roaming_candidate(struct mlme_priv *pmlmepriv);
#endif /* CONFIG_LAYER2_ROAMING */


#ifdef CONFIG_INTEL_PROXIM
void rtw_proxim_enable(_adapter *padapter);
void rtw_proxim_disable(_adapter *padapter);
void rtw_proxim_send_packet(_adapter *padapter, u8 *pbuf, u16 len, u8 hw_rate);
#endif //CONFIG_INTEL_PROXIM

extern void rtw_os_indicate_disconnect(_adapter *adapter);
extern void rtw_reset_securitypriv(_adapter *adapter);
#endif //__RTL871X_MLME_H_

