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

#define MLME_IS_PD(adapter) 0
#define MLME_IS_GC(adapter) 0
#define MLME_IS_GO(adapter) 0

#define 	WIFI_NULL_STATE		0x00000000
#define	WIFI_ASOC_STATE		0x00000001		// Under Linked state...
#define 	WIFI_REASOC_STATE	       0x00000002
#define	WIFI_SLEEP_STATE	       0x00000004
#define	WIFI_STATION_STATE	0x00000008
#define	WIFI_AP_STATE				0x00000010
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

enum {
	RTW_MLME_PREPARE_JOIN = 0,
	RTW_MLME_JOIN_FAIL = 1,
	RTW_MLME_JOIN_SUCCESS = 2,
};

struct mlme_priv {

	_lock	lock;
	u8 bScanInProcess;
	u8	to_join; //flag
	u8 	rx_stbc;

	u8		*nic_hdl;

	_queue	scanned_queue;
	struct wlan_network	*free_bss_buf;
	u8			free_bss_buf_used;

	struct scan_priv scanpriv;

	NDIS_802_11_SSID	assoc_ssid;
	u8	assoc_bssid[6];

#ifdef CONFIG_WOWLAN
	u8	ip_addr[4];
#endif

	_timer assoc_timer;

	u8 assoc_by_bssid;

	_timer scan_to_timer; // driver itself handles scan_timeout status.
	u32 scan_start_time; // used to evaluate the time spent in scanning

	RT_LINK_DETECT_T	LinkDetectInfo;
	_timer	dynamic_chk_timer; //dynamic/periodic check timer

	u8 	key_mask; //use for ips to set wep key after ips_leave
	u8	acm_mask; // for wmm acm mask
	u8	ChannelPlan;
	RT_SCAN_TYPE 	scan_mode; // active: 1, passive: 0

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

static inline u8 *rtw_get_capability_from_ie(u8 *ie)
{
	return (ie + 8 + 2);
}

static inline u16 rtw_get_capability(WLAN_BSSID_EX *bss)
{
	u16	val;

	rtw_memcpy((u8 *)&val, rtw_get_capability_from_ie(bss->IEs), 2);

	return val;
}

static inline u8 *rtw_get_beacon_interval_from_ie(u8 *ie)
{
	return (ie + 8);
}

extern void rtw_joinbss_event_prehandle(_adapter *adapter, u8 *pbuf);

extern void rtw_atimdone_event_callback(_adapter *adapter, u8 *pbuf);
extern void rtw_cpwm_event_callback(_adapter *adapter, u8 *pbuf);

extern void rtw_free_network_queue(_adapter *adapter);
extern void rtw_init_mlme_priv(_adapter *adapter);// (struct mlme_priv *pmlmepriv);

extern void rtw_free_mlme_priv(_adapter *adapter);


extern sint rtw_select_and_join_from_scanned_queue(struct mlme_priv *pmlmepriv);
extern sint rtw_set_key(_adapter *adapter, struct security_priv *psecuritypriv, sint keyid, u8 set_tx);
extern sint rtw_linked_check(_adapter *padapter);

#ifdef CONFIG_CONCURRENT_MODE
sint rtw_buddy_adapter_up(_adapter *padapter);
sint check_buddy_fwstate(_adapter *padapter, sint state);
#endif //CONFIG_CONCURRENT_MODE
sint check_both_fwstate(_adapter *padapter, sint state);

extern u16 rtw_get_capability(WLAN_BSSID_EX *bss);
extern void rtw_scan_update_scanned_network(_adapter *adapter, WLAN_BSSID_EX *target);
extern void rtw_disconnect_hdl_under_linked(_adapter *adapter, struct sta_info *psta, u8 free_assoc);
extern struct wlan_network *rtw_find_network(_queue *scanned_queue, u8 *addr);

extern void rtw_free_assoc_resources(_adapter *adapter, int lock_scanned_queue);
extern void rtw_indicate_disconnect(_adapter *adapter);
extern void rtw_indicate_connect(_adapter *adapter);
void rtw_scan_abort(_adapter *adapter);

extern int rtw_restruct_sec_ie(_adapter *adapter, u8 *in_ie, u8 *out_ie, uint in_len);
extern int rtw_restruct_wmm_ie(_adapter *adapter, u8 *in_ie, u8 *out_ie, uint in_len, uint initial_out_len);
//extern void rtw_init_registrypriv_dev_network(_adapter *adapter);

//extern void rtw_update_registrypriv_dev_network(_adapter *adapter);

extern void rtw_scan_timeout_handler(void *FunctionContext);

extern u8 rtw_dynamic_check_timer_handlder(_adapter *adapter);

void rtw_free_mlme_priv_ie_data(_adapter *padapter);

extern struct wlan_network *_rtw_dequeue_network(_queue *queue);

u8 *rtw_get_capability_from_ie(u8 *ie);
u8 *rtw_get_beacon_interval_from_ie(u8 *ie);

unsigned int rtw_restructure_ht_ie(_adapter *padapter, u8 *in_ie, u8 *out_ie, uint in_len, uint *pout_len);
void rtw_update_ht_cap(_adapter *padapter, u8 *pie, uint ie_len);
void rtw_issue_addbareq_cmd(_adapter *padapter, struct xmit_frame *pxmitframe);

int rtw_is_same_bss(_adapter *adapter, struct wlan_network *pnetwork);
int rtw_scan_is_same_network(WLAN_BSSID_EX *src, WLAN_BSSID_EX *dst);

void rtw_get_bcn_info(struct wlan_network *pnetwork);

#ifdef CONFIG_INTEL_PROXIM
void rtw_proxim_enable(_adapter *padapter);
void rtw_proxim_disable(_adapter *padapter);
void rtw_proxim_send_packet(_adapter *padapter, u8 *pbuf, u16 len, u8 hw_rate);
#endif //CONFIG_INTEL_PROXIM

extern void rtw_os_indicate_disconnect(_adapter *adapter);
extern void rtw_reset_securitypriv(_adapter *adapter);
#endif //__RTL871X_MLME_H_

