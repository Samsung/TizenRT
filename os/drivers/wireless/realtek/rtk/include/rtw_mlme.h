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
#ifndef __RTW_MLME_H_
#define __RTW_MLME_H_

#ifdef CONFIG_INTEL_WIDI
#include <rtw_intel_widi.h>
#endif

#define MAX_BSS_CNT 1 //alloc less wlan_network due to memory limitation - Alex Fang
#define MAX_JOIN_TIMEOUT 6500

#ifdef CONFIG_MULTICAST
#define MULTICAST_LIST_SIZE 4
#endif

//	Commented by Albert 20101105
//	Increase the scanning timeout because of increasing the SURVEY_TO value.

#define SCANNING_TIMEOUT 8000

#define SCAN_INTERVAL (30) // unit:2sec, 30*2=60sec

#ifdef PALTFORM_OS_WINCE
#define SCANQUEUE_LIFETIME 12000000 // unit:us
#else
#define SCANQUEUE_LIFETIME 20 // unit:sec
#endif

#define WIFI_NULL_STATE 0x00000000
#define WIFI_ASOC_STATE 0x00000001 // Under Linked state...
#define WIFI_REASOC_STATE 0x00000002
#define WIFI_SLEEP_STATE 0x00000004
#define WIFI_STATION_STATE 0x00000008
#define WIFI_AP_STATE 0x00000010
#define WIFI_ADHOC_STATE 0x00000020
#define WIFI_ADHOC_MASTER_STATE 0x00000040
#define WIFI_UNDER_LINKING 0x00000080
#define WIFI_MESH_STATE 0x00000200

//#define WIFI_UNDER_CMD			0x00000200
// ========== P2P Section Start ===============
#define WIFI_P2P_LISTEN_STATE 0x00010000
#define WIFI_P2P_GROUP_FORMATION_STATE 0x00020000
// ========== P2P Section End ===============
#define WIFI_UNDER_WPS 0x00000100
#define WIFI_SITE_MONITOR 0x00000800 //to indicate the station is under site surveying

#ifdef WDS
#define WIFI_WDS 0x00001000
#define WIFI_WDS_RX_BEACON 0x00002000 // already rx WDS AP beacon
#endif
#ifdef AUTO_CONFIG
#define WIFI_AUTOCONF 0x00004000
#define WIFI_AUTOCONF_IND 0x00008000
#endif

//#ifdef UNDER_MPTEST
#define WIFI_MP_STATE 0x00010000
#define WIFI_MP_CTX_BACKGROUND 0x00020000		  // in continous tx background
#define WIFI_MP_CTX_ST 0x00040000				  // in continous tx with single-tone
#define WIFI_MP_CTX_BACKGROUND_PENDING 0x00080000 // pending in continous tx background due to out of skb
#define WIFI_MP_CTX_CCK_HW 0x00100000			  // in continous tx
#define WIFI_MP_CTX_CCK_CS 0x00200000			  // in continous tx with carrier suppression
#define WIFI_MP_LPBK_STATE 0x00400000
//#endif

//#define _FW_UNDER_CMD		WIFI_UNDER_CMD
#define _FW_UNDER_LINKING WIFI_UNDER_LINKING
#define _FW_LINKED WIFI_ASOC_STATE
#define _FW_UNDER_SURVEY WIFI_SITE_MONITOR

enum dot11AuthAlgrthmNum {
	dot11AuthAlgrthm_Open = 0,
	dot11AuthAlgrthm_Shared,
	dot11AuthAlgrthm_8021X,
	dot11AuthAlgrthm_Auto,
	dot11AuthAlgrthm_WAPI,
	dot11AuthAlgrthm_SAE,
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
	SCAN_PASSIVE,
	SCAN_ACTIVE,
	SCAN_MIX,
} RT_SCAN_TYPE,
	*PRT_SCAN_TYPE;

/*

there are several "locks" in mlme_priv,
since mlme_priv is a shared resource between many threads,
like ISR/Call-Back functions, the OID handlers, and even timer functions.


Each _queue has its own locks, already.
Other items are protected by mlme_priv.lock.

To avoid possible dead lock, any thread trying to modifiying mlme_priv
SHALL not lock up more than one locks at a time!

*/

#define traffic_threshold 10
#define traffic_scan_period 500

struct sitesurvey_ctrl {
	u64 last_tx_pkts;
	uint last_rx_pkts;
	sint traffic_busy;
	_timer sitesurvey_ctrl_timer;
};

typedef struct _RT_LINK_DETECT_T {
	u32 NumTxOkInPeriod;
	u32 NumRxOkInPeriod;
	u32 NumRxUnicastOkInPeriod;
	u32 NumRxBcnInPeriod;
	BOOLEAN bBusyTraffic;
	BOOLEAN bTxBusyTraffic;
	BOOLEAN bRxBusyTraffic;
	BOOLEAN bHigherBusyTraffic;   // For interrupt migration purpose.
	BOOLEAN bHigherBusyRxTraffic; // We may disable Tx interrupt according as Rx traffic.
	BOOLEAN bHigherBusyTxTraffic; // We may disable Tx interrupt according as Tx traffic.
} RT_LINK_DETECT_T, *PRT_LINK_DETECT_T;

struct wifidirect_info {
	enum P2P_ROLE role;
	enum P2P_STATE p2p_state;
	u8 baction_tx_pending;
	u8 pending_peer[ETH_ALEN];
	struct xmit_frame *pending_action;
	_timer pre_tx_scan_timer;

};

struct mlme_priv {

	_lock lock;
	sint fw_state; //shall we protect this variable? maybe not necessarily...
	u8 bScanInProcess;
	u8 to_join; //flag
#ifdef CONFIG_LAYER2_ROAMING
	u8 to_roaming; // roaming trying times
#endif

	u8 *nic_hdl; //can be removed

	//u8		not_indic_disco;
	_list *pscanned;
	_queue free_bss_pool;
	_queue scanned_queue;
	u8 *free_bss_buf;
	u16 num_of_scanned;

#if SUPPORT_SCAN_BUF // Cloud 2013/12/20
	u8 *scan_buf;
	u32 scan_buf_len;
	u16 scan_cnt;
	u16 scan_type;
#endif

	NDIS_802_11_SSID assoc_ssid;
	u8 assoc_bssid[6];

#ifdef CONFIG_WOWLAN
	u8 ip_addr[4];
#endif

	struct wlan_network cur_network;

	//uint wireless_mode; no used, remove it

	u32 scan_interval;

	_timer assoc_timer;

	u8 assoc_by_bssid;
	u8 assoc_by_rssi;

	_timer scan_to_timer; // driver itself handles scan_timeout status.
	u32 scan_start_time;  // used to evaluate the time spent in scanning

#ifdef CONFIG_SET_SCAN_DENY_TIMER
	_timer set_scan_deny_timer;
	ATOMIC_T set_scan_deny; //0: allowed, 1: deny
#endif

	struct qos_priv qospriv;

#ifdef CONFIG_80211N_HT

	/* Number of non-HT AP/stations */
	u16 num_sta_no_ht; //int num_sta_no_ht;

	/* Number of HT AP/stations 20 MHz */
	//int num_sta_ht_20mhz;

	u16 num_FortyMHzIntolerant; //int num_FortyMHzIntolerant;

	struct ht_priv htpriv;

#endif

#ifdef CONFIG_80211AC_VHT
	struct vht_priv vhtpriv;
#endif

	RT_LINK_DETECT_T LinkDetectInfo;
	_timer dynamic_chk_timer; //dynamic/periodic check timer

	u8 key_mask; //use for ips to set wep key after ips_leave
	u8 acm_mask; // for wmm acm mask
	u8 ChannelPlan;
	RT_SCAN_TYPE scan_mode; // active: 1, passive: 0

#ifdef CONFIG_WPS
	u8 *wps_probe_req_ie;
	u32 wps_probe_req_ie_len;
	u8 *wps_assoc_req_ie;
	u32 wps_assoc_req_ie_len;
#endif

#if defined(CONFIG_AP_MODE) && defined(CONFIG_NATIVEAP_MLME)
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

	/* Overlapping BSS information */
	u8 olbc_ht;

#ifdef CONFIG_80211N_HT
	u16 ht_op_mode;
#endif /* CONFIG_80211N_HT */

#ifdef CONFIG_WPS
	u8 *wps_beacon_ie;
	u8 *wps_probe_resp_ie;
	u8 *wps_assoc_resp_ie; // for CONFIG_IOCTL_CFG80211, this IE could include p2p ie

	u32 wps_beacon_ie_len;
	u32 wps_probe_resp_ie_len;
	u32 wps_assoc_resp_ie_len;

#ifdef CONFIG_P2P_NEW
	u8 *p2p_beacon_ie;
	u8 *p2p_probe_req_ie;
	u8 *p2p_probe_resp_ie;
	//	u8 *p2p_go_probe_resp_ie; //for GO
	u8 *p2p_assoc_req_ie;
	u8 *p2p_assoc_rsp_ie;

	u32 p2p_beacon_ie_len;
	u32 p2p_probe_req_ie_len;
	u32 p2p_probe_resp_ie_len;
	//	u32 p2p_go_probe_resp_ie_len; //for GO
	u32 p2p_assoc_req_ie_len;
	u32 p2p_assoc_rsp_ie_len;
#endif //CONFIG_P2P
#endif //CONFIG_WPS

	_lock bcn_update_lock;
	u8 update_bcn;
#if USE_DEDICATED_BCN_TX
	//Dedicated xmit frame and buffer for beacon update - Alex Fang
	struct xmit_frame bcn_xmit_frame;
	struct xmit_buf bcn_xmit_buf;
	//u8 bcn_buf[256];
	u8 bcn_buf[320]; //p2p go beacon size is about 272+32 bytes
#endif
#endif //#if defined (CONFIG_AP_MODE) && defined (CONFIG_NATIVEAP_MLME)

#ifdef RTK_DMP_PLATFORM
	// DMP kobject_hotplug function  signal need in passive level
	_workitem Linkup_workitem;
	_workitem Linkdown_workitem;
#endif

#ifdef CONFIG_INTEL_WIDI
	int widi_state;
	int listen_state;
	_timer listen_timer;
	ATOMIC_T rx_probe_rsp; // 1:receive probe respone from RDS source.
	u8 *l2sdTaBuffer;
	u8 channel_idx;
	u8 group_cnt; //In WiDi 3.5, they specified another scan algo. for WFD/RDS co-existed
	u8 sa_ext[L2SDTA_SERVICE_VE_LEN];
#endif // CONFIG_INTEL_WIDI

	u32 lastscantime;

#ifdef CONFIG_CONCURRENT_MODE
	u8 scanning_via_buddy_intf;
#endif

#ifdef CONFIG_FTP_PROTECT
	u8 ftp_lock_flag;
#endif //CONFIG_FTP_PROTECT

#ifdef CONFIG_MULTICAST
	u32 multicast_list[MULTICAST_LIST_SIZE];
#endif
	//For fast reconnection to keep frame info temporarily
	union recv_frame *p_copy_recv_frame;
	unsigned short disassoc_reason;
};

#ifdef CONFIG_AP_MODE

struct hostapd_priv {
	_adapter *padapter;

#ifdef CONFIG_HOSTAPD_MLME
	struct net_device *pmgnt_netdev;
	struct usb_anchor anchored;
#endif
};

extern int hostapd_mode_init(_adapter *padapter);
extern void hostapd_mode_unload(_adapter *padapter);
#endif

extern void rtw_joinbss_event_prehandle(_adapter *adapter, u8 *pbuf);
extern void rtw_survey_event_callback(_adapter *adapter, u8 *pbuf);
extern void rtw_surveydone_event_callback(_adapter *adapter, u8 *pbuf);
extern void rtw_joinbss_event_callback(_adapter *adapter, u8 *pbuf);
extern void rtw_stassoc_event_callback(_adapter *adapter, u8 *pbuf);
extern void rtw_stadel_event_callback(_adapter *adapter, u8 *pbuf);
#if defined(CONFIG_IEEE80211W) && defined(CONFIG_AP_MODE)
extern void rtw_sta_timeout_event_callback(_adapter *adapter, u8 *pbuf);
#endif /* CONFIG_IEEE80211W */
extern void rtw_atimdone_event_callback(_adapter *adapter, u8 *pbuf);
extern void rtw_cpwm_event_callback(_adapter *adapter, u8 *pbuf);

#ifdef PLATFORM_WINDOWS
extern thread_return event_thread(void *context);

extern void rtw_join_timeout_handler(
	IN PVOID SystemSpecific1,
	IN PVOID FunctionContext,
	IN PVOID SystemSpecific2,
	IN PVOID SystemSpecific3);

extern void _rtw_scan_timeout_handler(
	IN PVOID SystemSpecific1,
	IN PVOID FunctionContext,
	IN PVOID SystemSpecific2,
	IN PVOID SystemSpecific3);

#endif

extern void rtw_free_network_queue(_adapter *adapter, u8 isfreeall);
extern int rtw_init_mlme_priv(_adapter *adapter); // (struct mlme_priv *pmlmepriv);

extern void rtw_free_mlme_priv(struct mlme_priv *pmlmepriv);

extern sint rtw_select_and_join_from_scanned_queue(struct mlme_priv *pmlmepriv);
extern sint rtw_set_key(_adapter *adapter, struct security_priv *psecuritypriv, sint keyid, u8 set_tx);
extern sint rtw_set_auth(_adapter *adapter, struct security_priv *psecuritypriv);
extern sint rtw_linked_check(_adapter *padapter);

__inline static u8 *get_bssid(struct mlme_priv *pmlmepriv)
{ //if sta_mode:pmlmepriv->cur_network.network.MacAddress=> bssid
	// if adhoc_mode:pmlmepriv->cur_network.network.MacAddress=> ibss mac address
	return pmlmepriv->cur_network.network.MacAddress;
}

__inline static sint check_fwstate(struct mlme_priv *pmlmepriv, sint state)
{
	if (pmlmepriv->fw_state & state)
		return _TRUE;

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
	if (check_fwstate(pmlmepriv, state) == _TRUE)
		pmlmepriv->fw_state ^= state;
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
void rtw_indicate_scan_done(_adapter *padapter, bool aborted);
void rtw_scan_abort(_adapter *adapter);

extern int rtw_restruct_sec_ie(_adapter *adapter, u8 *in_ie, u8 *out_ie, uint in_len);
extern int rtw_restruct_wmm_ie(_adapter *adapter, u8 *in_ie, u8 *out_ie, uint in_len, uint initial_out_len);

extern void rtw_get_encrypt_decrypt_from_registrypriv(_adapter *adapter);

extern void _rtw_join_timeout_handler(_adapter *adapter);
extern void rtw_scan_timeout_handler(_adapter *adapter);

extern void rtw_dynamic_check_timer_handlder(_adapter *adapter);
#ifdef CONFIG_SET_SCAN_DENY_TIMER
extern void rtw_set_scan_deny_timer_hdl(_adapter *adapter);
void rtw_set_scan_deny(struct mlme_priv *mlmepriv, u32 ms);
#endif

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

#ifdef CONFIG_80211N_HT
unsigned int rtw_restructure_ht_ie(_adapter *padapter, u8 *in_ie, u8 *out_ie, uint in_len, uint *pout_len);
void rtw_update_ht_cap(_adapter *padapter, u8 *pie, uint ie_len);
void rtw_issue_addbareq_cmd(_adapter *padapter, struct xmit_frame *pxmitframe);
#endif

int rtw_is_same_ibss(_adapter *adapter, struct wlan_network *pnetwork);
int is_same_network(WLAN_BSSID_EX *src, WLAN_BSSID_EX *dst);

#ifdef CONFIG_LAYER2_ROAMING
void rtw_roaming(_adapter *padapter, struct wlan_network *tgt_network);
void _rtw_roaming(_adapter *padapter, struct wlan_network *tgt_network);
#endif

#ifdef CONFIG_INTEL_PROXIM
void rtw_proxim_enable(_adapter *padapter);
void rtw_proxim_disable(_adapter *padapter);
void rtw_proxim_send_packet(_adapter *padapter, u8 *pbuf, u16 len, u8 hw_rate);
#endif //CONFIG_INTEL_PROXIM

extern void rtw_os_indicate_disconnect(_adapter *adapter);
extern void rtw_os_indicate_scan_done(_adapter *padapter, bool aborted);
extern void rtw_reset_securitypriv(_adapter *adapter);
#endif //__RTL871X_MLME_H_
