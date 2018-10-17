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

#ifndef __SLSI_DEVICE_H__
#define __SLSI_DEVICE_H__

#include <sys/types.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <pthread.h>
#include <tinyara/net/net.h>
#include <tinyara/net/netdev.h>
#include <tinyara/kmalloc.h>
#include <scsc/scsc_mx.h>
#include <tinyara/wdog.h>
#include <net/lwip/netif.h>

#include "max_buf.h"
#include "fapi.h"
#include "utils_scsc.h"
#include "hip.h"
#include "log_clients.h"
#include "unifiio.h"
#include "scsc_wifi_fcq.h"
#include "scsc_wifi_cm_if.h"
#include "hip4.h"
#include "wlan_80211_utils.h"
#include "tinyara_wrapper.h"
#ifdef CONFIG_SLSI_WLAN_STATS
#include "stats.h"
#endif

/* Max number of peers */
#define SLSI_AP_PEER_CONNECTIONS_MAX 1

/* Max number of indexes */
#define SLSI_PEER_INDEX_MIN 1
#define SLSI_PEER_INDEX_MAX SLSI_AP_PEER_CONNECTIONS_MAX

#define SLSI_TX_PROCESS_ID_MIN       (0xC001)
#define SLSI_TX_PROCESS_ID_MAX       (0xCF00)
#define SLSI_TX_PROCESS_ID_UDI_MIN   (0xCF01)
#define SLSI_TX_PROCESS_ID_UDI_MAX   (0xCFFE)

#define SLSI_TX_MBUF_SIZE            (2 * 1024)

#define MAX_BA_BUFFER_SIZE 64
#define NUM_BA_SESSIONS_PER_PEER 8
#define MAX_CHANNEL_LIST 20
#define SLSI_MAX_RX_BA_SESSIONS (8)

#define WLAN_EID_VHT_CAPABILITY 191
#define WLAN_EID_VHT_OPERATION 192

#define NUM_COUNTRY             (300)

#ifdef CONFIG_SLSI_WLAN_STATS
#define SLSI_INCR_DATA_PATH_STATS(x)                ((x)++)
#define SLSI_INCR_DATA_PATH_STATS_SET_VAL(x, val)   ((x) = val)
#else
#define SLSI_INCR_DATA_PATH_STATS(x)                do {} while (0)
#define SLSI_INCR_DATA_PATH_STATS_SET_VAL(x, val)   do {} while (0)
#endif

/* For each mlme-req a mlme-cfm is expected to be received from the
 * firmware. The host is not allowed to send another mlme-req until
 * the mlme-cfm is received.
 *
 * However there are also instances where we need to wait for an mlme-ind
 * following a mlme-req/cfm exchange. One example of this is the disconnect
 * sequence:
 * mlme-disconnect-req - host requests disconnection
 * mlme-disconnect-cfm - firmware accepts disconnection request but hasn't
 *                       disconnected yet.
 * mlme-disconnect-ind - firmware reports final result of disconnection
 *
 * Assuming that waiting for the mlme-ind following on from the mlme-req/cfm
 * is ok.
 */
struct slsi_sig_send {
	/* a std spinlock */
	pthread_mutex_t send_signal_lock;
	pthread_mutex_t mutex;

	struct completion completion;

	u16 process_id;
	u16 req_id;
	u16 cfm_id;
	u16 ind_id;
	struct max_buff *cfm;
	struct max_buff *ind;
	struct max_buff *mib_error;
};

struct slsi_cm_ctx {
	struct slsi_dev *sdev;
};

/* Only one wlan service instance is assumed for now. */

extern struct slsi_cm_ctx cm_ctx;

static inline void slsi_sig_send_init(struct slsi_sig_send *sig_send)
{
	SLSI_MUTEX_INIT(sig_send->send_signal_lock);

	sig_send->req_id = 0;
	sig_send->cfm_id = 0;
	sig_send->process_id = SLSI_TX_PROCESS_ID_MIN;
	SLSI_MUTEX_INIT(sig_send->mutex);
	init_completion(&sig_send->completion);
}

struct slsi_ba_frame_desc {
	bool active;
	struct max_buff *signal;
	u16 sn;
};

struct slsi_ba_session_rx {
	bool active;
	bool used;
	void *vif;
	struct slsi_ba_frame_desc buffer[MAX_BA_BUFFER_SIZE];
	u16 buffer_size;
	u16 occupied_slots;
	u16 expected_sn;
	u16 start_sn;
	bool trigger_ba_after_ssn;
	u8 tid;

	/* Aging timer parameters */
	bool timer_on;
	pthread_mutex_t ba_lock;
	struct netif *dev;
	struct work_s work;
};

#ifdef CONFIG_SLSI_RX_PERFORMANCE_TEST
struct slsi_rx_perf_test {
	bool rx_perf_test_started;
	u32 num_bits_received;
	clock_t last_timestamp;
	pthread_mutex_t rx_perf_lock;
};

#define SLSI_RX_PERFORMANCE_REPORT_INTERVAL      (5)	/* In seconds */

/* If packet size is less than threshold then forward the packet to LWIP */
#define SLSI_RX_PERFORMANCE_PKT_LEN_THRESHOLD    (1400)
#endif

#define SLSI_SCAN_HW_ID       0
#define SLSI_SCAN_SCHED_ID    1
#define SLSI_SCAN_MAX         3

/* Per Interface Scan Data */
struct slsi_scan {
	bool scan_req;				/* When a scan is running this is set */
	struct max_buff_head scan_results;
};

struct slsi_peer {
	/* Flag MUST be set last when creating a record and immediately when removing.
	 * Otherwise another process could test the flag and start using the data.
	 */
	bool valid;
	u8 address[ETH_ALEN];
	u8 ssid[SSID_MAX_LEN];
	size_t ssid_len;

	/* Presently connected_state is used only for AP/GO mode */
	u8 connected_state;
	u16 aid;
	u16 capabilities;
	bool qos_enabled;
	u8 queueset;
	struct scsc_wifi_fcq_data_qset data_qs;
	struct scsc_wifi_fcq_ctrl_q ctrl_q;

	bool authorized;
	bool pairwise_key_set;

	/* Needed for STA/AP VIF */
	struct max_buff *assoc_ie;
	struct max_buff_head buffered_frames;
	/* Needed for STA VIF */
	struct max_buff *assoc_resp_ie;

	/* bitmask that keeps the status of acm bit for each AC
	 * bit 7  6  5  4  3  2  1  0
	 *     |  |  |  |  |  |  |  |
	 *     vo vo vi vi be bk bk be
	 */
	u8 wmm_acm;
	/* bitmask that keeps the status of tspec establishment for each priority
	 * bit 7  6  5  4  3  2  1  0
	 *     |  |  |  |  |  |  |  |
	 *     p7 p6 p5 p4 p3 p2 p1 p0
	 */
	u8 tspec_established;
	u8 uapsd;

	struct slsi_ba_session_rx *ba_session_rx[NUM_BA_SESSIONS_PER_PEER];
};

/* Used to update vif type on vif deactivation indicating vif is no longer available */
#define SLSI_VIFTYPE_UNSPECIFIED   0xFFFF

#ifdef CONFIG_SCSC_ENABLE_P2P
struct slsi_vif_mgmt_tx {
	u64 cookie;					/* Cookie assigned by Host for the tx mgmt frame */
	u16 host_tag;				/* Host tag for the tx mgmt frame */
	const u8 *buf;				/* Buffer - Mgmt frame requested for tx */
	size_t buf_len;				/* Buffer length */
	u8 exp_frame;				/* Next expected Public action frame subtype from peer */
};
#endif

struct slsi_wmm_ac {
	u8 aci_aifsn;
	u8 ecw;
	u16 txop_limit;
} STRUCT_PACKED;

/* struct slsi_wmm_parameter_element
 *
 * eid - Vendor Specific
 * len - Remaining Length of IE
 * oui - Microsoft OUI
 * oui_type - WMM
 * oui_subtype - Param IE
 * version - 1
 * qos_info - Qos
 * reserved -
 * ac - BE,BK,VI,VO
 */
struct slsi_wmm_parameter_element {
	u8 eid;
	u8 len;
	u8 oui[3];
	u8 oui_type;
	u8 oui_subtype;
	u8 version;
	u8 qos_info;
	u8 reserved;
	struct slsi_wmm_ac ac[4];
} STRUCT_PACKED;

#define SLSI_MIN_FILTER_ID  0x80	/* Start of filter range reserved for host */

/*for AP*/
#define SLSI_AP_ALL_IPv6_PKTS_FILTER_ID  0x80

/*filter ids for filters installed by driver*/
#ifdef CONFIG_SCSC_WLAN_BLOCK_IPV6

enum slsi_filter_id {
	SLSI_LOCAL_ARP_FILTER_ID = SLSI_MIN_FILTER_ID,	/*0x80 */
	SLSI_DNS_FILTER_ID,			/*0x81 */
	SLSI_ALL_BC_FILTER_ID,		/*0x82 */
	SLSI_ALL_IPv4_MC_FILTER_ID,	/*0x83 */
	SLSI_PROXY_ARP_FILTER_ID,	/*0x84 */
	SLSI_ALL_IPv6_PKTS_FILTER_ID,	/*0x85 */
#ifndef CONFIG_SCSC_WLAN_DISABLE_NAT_KA
	SLSI_NAT_IPSEC_FILTER_ID,	/*0x86 */
#endif
	SLSI_REGD_MC_FILTER_ID = 0x87,	/*0x87 */
};
#else

/*for STA*/
enum slsi_filter_id {
	SLSI_LOCAL_ARP_FILTER_ID = SLSI_MIN_FILTER_ID,	/*0x80 */
	SLSI_DNS_FILTER_ID,			/*0x81 */
	SLSI_ALL_BC_FILTER_ID,		/*0x82 */
	SLSI_ALL_IPv4_MC_FILTER_ID,	/*0x83 */
	SLSI_PROXY_ARP_FILTER_ID,	/*0x84 */
	SLSI_LOCAL_NS_FILTER_ID,	/*0x85 */
	SLSI_DNS6_FILTER_ID,		/*0x86 */
	SLSI_ALL_IPv6_MC_FILTER_ID,	/*0x87 */
	SLSI_PROXY_ARP_NA_FILTER_ID,	/*0x88 */
#ifndef CONFIG_SCSC_WLAN_DISABLE_NAT_KA
	SLSI_NAT_IPSEC_FILTER_ID,	/*0x89 */
#endif
	SLSI_REGD_MC_FILTER_ID = 0x8a,	/*0x8a */
};

#endif

#define SLSI_MAX_PKT_FILTERS       16

#ifndef CONFIG_SCSC_WLAN_DISABLE_NAT_KA
/*default config*/
#define SLSI_MC_ADDR_ENTRY_MAX  (SLSI_MIN_FILTER_ID + SLSI_MAX_PKT_FILTERS - SLSI_REGD_MC_FILTER_ID)
#else
#define SLSI_MC_ADDR_ENTRY_MAX  (SLSI_MIN_FILTER_ID + SLSI_MAX_PKT_FILTERS - SLSI_REGD_MC_FILTER_ID + 1)
#endif

/* Values for vif_status field
 *
 * Used to indicate the status of an activated VIF, to help resolve
 * conflicting activities with indications from the firmware eg.
 * cfg80211 triggers a disconnection before a STA completes its
 * connection to an AP.
 */
#define SLSI_VIF_STATUS_UNSPECIFIED   0
#define SLSI_VIF_STATUS_CONNECTING    1
#define SLSI_VIF_STATUS_CONNECTED     2
#define SLSI_VIF_STATUS_DISCONNECTING 3

struct slsi_vif_sta {
	/* Only valid when the VIF is activated */
	u8 vif_status;
	bool is_wps;
	u16 eap_hosttag;
	u16 m4_host_tag;
	u8 mac_addr[ETH_ALEN];
	/* This id is used to find out which response  (connect resp/reassoc resp)
	 * is to be sent once M4 is transmitted successfully
	 */
	u16 resp_id;
	bool gratuitous_arp_needed;
	bool group_key_set;
};

#ifdef CONFIG_SCSC_ENABLE_P2P
struct slsi_vif_unsync {
	//struct delayed_work roc_expiry_work;   /* Work on ROC duration expiry */
	//struct delayed_work del_vif_work;      /* Work on unsync vif retention timeout */
	u64 roc_cookie;				/* Cookie id for ROC */
	u8 *probe_rsp_ies;			/* Probe response IEs to be configured in firmware */
	size_t probe_rsp_ies_len;	/* Probe response IE length */
	bool ies_changed;			/* To indicate if Probe Response IEs have changed from that previously stored */
	bool listen_offload;		/* To indicate if Listen Offload is started */
};
#endif

struct slsi_vif_ap {
	struct slsi_wmm_parameter_element wmm_ie;
	u8 *cache_wmm_ie;
	u8 *cache_wpa_ie;
	u8 *add_info_ies;
	size_t wmm_ie_len;
	size_t wpa_ie_len;
	size_t add_info_ies_len;
#ifdef CONFIG_SCSC_ENABLE_P2P
	bool p2p_gc_keys_set;		/* Used in GO mode to identify that a CLI has connected after WPA2 handshake */
#endif
	bool privacy;				/* Used for port enabling based on the open/secured AP configuration */
	int beacon_interval;		/* Beacon interval in AP/GO mode */
#ifdef CONFIG_SCSC_ADV_FEATURE
	bool non_ht_bss_present;	/* Non HT BSS observed in HT20 OBSS scan */
#endif
	struct scsc_wifi_fcq_data_qset group_data_qs;
	u32 cipher;
};

struct netdev_vif {
	struct slsi_dev *sdev;
	bool is_registered;			/* Has the net dev been registered */
	bool is_available;			/* Has the net dev been opened AND is usable */
	bool is_fw_test;			/* Is the device in use as a test device via UDI */

	/* Structure can be accessed in supplicant and driver work context.
	 * The vif mutex is for synchronisation in such cases.
	 */
	pthread_mutex_t vif_mutex;

	struct slsi_sig_send sig_wait;
	struct slsi_mbuf_work rx_data;
	struct slsi_mbuf_work rx_mlme;
	u16 ifnum;
	enum slsi_80211_iftype iftype;
	enum slsi_80211_channel_type channel_type;
	int center_freq;

	struct hostapd_freq_params *chandef;

	/* NOTE: The Address is a __be32
	 * It needs converting to pass to the FW
	 * But not for the Arp or trace %pI4
	 */
	__be32 ipaddress;

#ifndef CONFIG_SCSC_WLAN_BLOCK_IPV6
	struct in6_addr ipv6address;
	pthread_mutex_t ipv6addr_lock;
#endif
	pthread_mutex_t scan_mutex;
	struct slsi_scan scan[SLSI_SCAN_MAX];

#ifdef CONFIG_SCSC_WLAN_POWER_SAVE
	/* Used while updating the power management state */
	pthread_mutex_t powermgt_lock;
	u16 power_mode;
#endif
	bool activated;				/* VIF is created in firmware and ready to use */
	u16 vif_type;
	pthread_mutex_t peer_lock;
	int peer_sta_records;
	struct slsi_peer *peer_sta_record[SLSI_AP_PEER_CONNECTIONS_MAX];

	/* Block Ack MPDU Re-order */
	struct max_buff_head ba_complete;
	int ba_flush;

	struct work_s scan_timeout_work;	/* Work on scan timeout */
#ifdef CONFIG_SCSC_ENABLE_P2P
	u64 mgmt_tx_cookie;			/* Cookie id for mgmt tx */
	struct slsi_vif_mgmt_tx mgmt_tx_data;
	struct slsi_vif_unsync unsync;
#endif
	struct slsi_vif_sta sta;
	struct slsi_vif_ap ap;
};

struct slsi_802_11d_reg_domain {
	u8 alpha2[3];
	u8 *countrylist;
	//struct ieee80211_regdomain *regdomain;
	int country_len;
};

struct slsi_dev_config {
	/* Current Channel Config */
	struct max_buff *channel_config;

	/* Supported Freq Band (Dynamic)
	 * Set via the freq_band procfs
	 */
#define SLSI_FREQ_BAND_AUTO 0
#define SLSI_FREQ_BAND_5GHZ 1
#define SLSI_FREQ_BAND_2GHZ 2
	int supported_band;

	struct slsi_80211_supported_band *band_5G;
	struct slsi_80211_supported_band *band_2G;

	/* current user suspend mode
	 * Set via the suspend_mode procfs
	 * 0 : not suspended
	 * 1 : suspended
	 */
	int user_suspend_mode;

	/* Rx filtering rule
	 * Set via the rx_filter_num procfs
	 * 0: Unicast, 1: Broadcast, 2:Multicast IPv4, 3: Multicast IPv6
	 */
	int rx_filter_num;

	/* Rx filter rule enabled
	 * Set  via the rx_filter_start & rx_filter_stop procfs
	 */
	bool rx_filter_rule_started;

	/* Read from FW mib at startup */
	unsigned int fast_power_save_timeout;

	/* AP Auto channel Selection */
#define SLSI_NO_OF_SCAN_CHANLS_FOR_AUTO_CHAN_MAX 14
	int ap_auto_chan;

	/*QoS capability for a non-AP Station */
	int qos_info;
	struct slsi_802_11d_reg_domain domain_info;

	int ap_disconnect_ind_timeout;
};

#define SLSI_DEVICE_STATE_ATTACHING 0
#define SLSI_DEVICE_STATE_STOPPED   1
#define SLSI_DEVICE_STATE_STARTING  2
#define SLSI_DEVICE_STATE_STARTED   3
#define SLSI_DEVICE_STATE_STOPPING  4

#define SLSI_NET_INDEX_WLAN 1
#define SLSI_NET_INDEX_P2P  2
#define SLSI_NET_INDEX_P2PX 3

#define SLSI_CHANNEL_BANDWIDTH_20MHZ 20
#define SLSI_CHANNEL_BANDWIDTH_40MHZ 40

struct slsi_chip_info_mib {
	u16 chip_version;
	u32 fw_build_id;
	u32 fw_patch_id;
};

#ifdef CONFIG_SCSC_ENABLE_P2P
/* States used during P2P operations */
enum slsi_p2p_states {
	P2P_IDLE_NO_VIF,			/* Initial state - Unsync vif is not present */
	P2P_IDLE_VIF_ACTIVE,		/* Unsync vif is present but no P2P procedure in progress */
	P2P_SCANNING,				/* P2P SOCIAL channel (1,6,11) scan in progress. Not used for P2P full scan */
	P2P_LISTENING,				/* P2P Listen (ROC) in progress */
	P2P_ACTION_FRAME_TX_RX,		/* P2P Action frame Tx in progress or waiting for a peer action frame Rx (i.e. in response to the Tx frame) */
	P2P_GROUP_FORMED_CLI,		/* P2P Group Formed - CLI role */
	P2P_GROUP_FORMED_GO,		/* P2P Group Formed - GO role */
	/* NOTE: In P2P_LISTENING state if frame transmission is requested to driver then a peer response is ideally NOT expected.
	 * This is an assumption based on the fact that FIND would be stopped prior to group formation/connection.
	 * If driver were to receive a peer frame in P2P_LISTENING state then it would most probably be a REQUEST frame and the supplicant would respond to it.
	 * Hence the driver should get only RESPONSE frames for transmission in P2P_LISTENING state.
	 */
};

/* P2P States in text format for debug purposes */
static inline char *slsi_p2p_state_text(u8 state)
{
	switch (state) {
	case P2P_IDLE_NO_VIF:
		return "P2P_IDLE_NO_VIF";
	case P2P_IDLE_VIF_ACTIVE:
		return "P2P_IDLE_VIF_ACTIVE";
	case P2P_SCANNING:
		return "P2P_SCANNING";
	case P2P_LISTENING:
		return "P2P_LISTENING";
	case P2P_ACTION_FRAME_TX_RX:
		return "P2P_ACTION_FRAME_TX_RX";
	case P2P_GROUP_FORMED_CLI:
		return "P2P_GROUP_FORMED_CLI";
	case P2P_GROUP_FORMED_GO:
		return "P2P_GROUP_FORMED_GO";
	default:
		return "UNKNOWN";
	}
}
#endif

struct hydra_service_info {
	char ver_str[128];
	uint16_t hw_ver;
	uint32_t fw_rom_ver;
	uint32_t fw_patch_ver;
	bool populated;
};

struct slsi_dev {
	struct slsi_t20_drv *drv;
	struct slsi_hip hip;		/* HIP bookkeeping block */
	struct slsi_hip4 hip4_inst;	/* The handler to parse to HIP */
	struct scsc_wifi_cm_if cm_if;	/* cm_if bookkeeping block */
	struct scsc_mx *maxwell_core;
	struct scsc_service_client mx_wlan_client;
	struct scsc_service *service;
	struct hydra_service_info chip_info;
	struct slsi_chip_info_mib chip_info_mib;
	pthread_mutex_t netdev_add_remove_mutex;
	int netdev_up_count;
	struct netif *netdev[CONFIG_SCSC_WLAN_MAX_INTERFACES + 1];	/* 0 is reserved */
	u8 netdev_addresses[CONFIG_SCSC_WLAN_MAX_INTERFACES + 1][ETH_ALEN];	/* 0 is reserved */
	int device_state;

	/* Locking used to control Starting and stopping the chip */
	pthread_mutex_t start_stop_mutex;

	/* UDI Logging */
	struct slsi_log_clients log_clients;
	void *uf_cdev;

	/* ProcFS */
	int procfs_instance;
	struct proc_dir_entry *procfs_dir;

	/* Configuration */
	u8 hw_addr[ETH_ALEN];
	char *mib_file_name;
	char *local_mib_file_name;
	char *maddr_file_name;
	struct max_buff *tx_mbuf;	/* On TinyARA allocating the mbuf during runtime fails */
#ifdef CONFIG_SLSI_WLAN_STATS
	struct data_path_stats dp_stats;
#endif
#ifdef CONFIG_SLSI_RX_PERFORMANCE_TEST
	struct slsi_rx_perf_test rx_perf;
#endif
	pthread_mutex_t rx_data_mutex;

	struct slsi_sig_send sig_wait;
	struct slsi_mbuf_work rx_dbg_sap;
	int tx_host_tag;
	pthread_mutex_t device_config_mutex;
	struct slsi_dev_config device_config;

#ifdef CONFIG_SCSC_ENABLE_P2P
	enum slsi_p2p_states p2p_state;	/* Store current P2P operation */
	bool delete_gc_probe_req_ies;	/* Delete probe request stored at  gc_probe_req_ies, if connected for WAP2 at mlme_del_vif */
	u8 *gc_probe_req_ies;
	size_t gc_probe_req_ie_len;
	u8 p2p_group_exp_frame;		/* Next expected Public action frame subtype from peer */
#endif
	int current_tspec_id;
	int tspec_error_code;
	bool scan_init_24g;

	struct slsi_ba_session_rx rx_ba_buffer_pool[SLSI_MAX_RX_BA_SESSIONS];
	pthread_mutex_t rx_ba_buffer_pool_lock;

	bool fail_reported;
	u64 flags;
	u16 software_iftypes;
	unsigned int probe_resp_offloads;

	/* Supported interface modes, OR together BIT(SLSI_80211_IFTYPE_...) */
	u16 interface_modes;

	u16 max_acl_mac_addrs;

	u32 ap_sme_capa;
	/** Maximum number of supported active probe SSIDs */
	int max_scan_ssids;

	/** Maximum number of supported active probe SSIDs for sched_scan */
	int max_sched_scan_ssids;

	/** Whether sched_scan (offloaded scanning) is supported */
	int sched_scan_supported;

	/** Maximum number of supported match sets for sched_scan */
	int max_match_sets;

	/**
	 * max_remain_on_chan - Maximum remain-on-channel duration in msec
	 */
	unsigned int max_remain_on_chan;

	/**
	 * max_stations - Maximum number of associated stations the driver
	 * supports in AP mode
	 */
	unsigned int max_stations;
	bool is_supplicant_deinit;
	int recovery_next_state;
	struct completion recovery_remove_completion;
	struct completion recovery_stop_completion;
	int recovery_status;
	bool mlme_blocked;			/* When true do not send mlme signals to FW */
};

#define LLC_SNAP_HDR_LEN 8

#ifdef CONFIG_SLSI_WLAN_STATS
extern int firmware_triggered_panic;
#endif

int slsi_rx_data(struct slsi_dev *sdev, struct netif *dev, struct max_buff *mbuf, bool fromBA);
void slsi_rx_dbg_sap_work(FAR void *arg);
void slsi_rx_netdev_data_work(FAR void *arg);
void slsi_rx_netdev_mlme_work(FAR void *arg);
int slsi_rx_enqueue_netdev_mlme(struct slsi_dev *sdev, struct max_buff *mbuf, u16 vif);
void slsi_rx_buffered_frames(struct slsi_dev *sdev, struct netif *dev, struct slsi_peer *peer);
int slsi_rx_blocking_signals(struct slsi_dev *sdev, struct max_buff *mbuf);
void slsi_tx_pause_queues(struct slsi_dev *sdev);
void slsi_tx_unpause_queues(struct slsi_dev *sdev);
int slsi_tx_control(struct slsi_dev *sdev, struct netif *dev, struct max_buff *mbuf, bool free_buf);
void slsi_alloc_tx_mbuf(struct slsi_dev *sdev);
void slsi_free_tx_mbuf(struct slsi_dev *sdev);
int slsi_tx_data(struct slsi_dev *sdev, struct netif *dev, struct max_buff *mbuf);
int slsi_tx_data_lower(struct slsi_dev *sdev, struct max_buff *mbuf);
bool slsi_is_wlan_service_active(void);
bool slsi_is_test_mode_enabled(void);
void slsi_regd_deinit(struct slsi_dev *sdev);
void slsi_init_netdev_mac_addr(struct slsi_dev *sdev);
int sap_mlme_notifier(struct slsi_dev *sdev, unsigned long event);
int sap_ma_notifier(struct slsi_dev *sdev, unsigned long event);

static inline u16 slsi_tx_host_tag(struct slsi_dev *sdev)
{
	sdev->tx_host_tag += 1;
	return (u16)(sdev->tx_host_tag);
}

static inline struct netif *slsi_get_netdev_locked(struct slsi_dev *sdev, u16 ifnum)
{
	WARN_ON(!SLSI_MUTEX_IS_LOCKED(sdev->netdev_add_remove_mutex));
	if (ifnum > CONFIG_SCSC_WLAN_MAX_INTERFACES) {
		return NULL;
	}
	return sdev->netdev[ifnum];
}

static inline struct netif *slsi_get_netdev(struct slsi_dev *sdev, u16 ifnum)
{
	struct netif *dev;

	SLSI_MUTEX_LOCK(sdev->netdev_add_remove_mutex);
	dev = slsi_get_netdev_locked(sdev, ifnum);
	SLSI_MUTEX_UNLOCK(sdev->netdev_add_remove_mutex);

	return dev;
}

#endif
