/*
 * Driver interface definition
 * Copyright (c) 2003-2015, Jouni Malinen <j@w1.fi>
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 *
 * This file defines a driver interface used by both %wpa_supplicant and
 * hostapd. The first part of the file defines data structures used in various
 * driver operations. This is followed by the struct wpa_driver_ops that each
 * driver wrapper will beed to define with callback functions for requesting
 * driver operations. After this, there are definitions for driver event
 * reporting with wpa_supplicant_event() and some convenience helper functions
 * that can be used to report events.
 */
#ifndef WPA_DRIVER_COPY_H
#define WPA_DRIVER_COPY_H

/**
 * enum reg_change_initiator - Regulatory change initiator
 */
enum reg_change_initiator {
	REGDOM_SET_BY_CORE,
	REGDOM_SET_BY_USER,
	REGDOM_SET_BY_DRIVER,
	REGDOM_SET_BY_COUNTRY_IE,
	REGDOM_BEACON_HINT,
};

/**
 * enum reg_type - Regulatory change types
 */
enum reg_type {
	REGDOM_TYPE_UNKNOWN,
	REGDOM_TYPE_COUNTRY,
	REGDOM_TYPE_WORLD,
	REGDOM_TYPE_CUSTOM_WORLD,
	REGDOM_TYPE_INTERSECTION,
};

/**
 * enum hostapd_hw_mode - Hardware mode
 */
enum hostapd_hw_mode {
	HOSTAPD_MODE_IEEE80211B,
	HOSTAPD_MODE_IEEE80211G,
	HOSTAPD_MODE_IEEE80211A,
	HOSTAPD_MODE_IEEE80211AD,
	HOSTAPD_MODE_IEEE80211ANY,
	NUM_HOSTAPD_MODES
};

/* enum chan_width - Channel width definitions */
enum chan_width {
	CHAN_WIDTH_20_NOHT,
	CHAN_WIDTH_20,
	CHAN_WIDTH_40,
	CHAN_WIDTH_80,
	CHAN_WIDTH_80P80,
	CHAN_WIDTH_160,
	CHAN_WIDTH_UNKNOWN
};

#define WPA_AUTH_ALG_OPEN BIT(0)
#define WPA_AUTH_ALG_SHARED BIT(1)
#define WPA_AUTH_ALG_LEAP BIT(2)
#define WPA_AUTH_ALG_FT BIT(3)
#define WPA_AUTH_ALG_SAE BIT(4)

typedef long os_time_t;

/**
 * struct dl_list - Doubly-linked list
 */
struct dl_list {
	struct dl_list *next;
	struct dl_list *prev;
};

struct wpa_freq_range_list {
	struct wpa_freq_range {
		unsigned int min;
		unsigned int max;
	} *range;
	unsigned int num;
};

struct os_reltime {
	os_time_t sec;
	os_time_t usec;
};

/**
 * struct wpa_scan_res - Scan result for an BSS/IBSS
 * @flags: information flags about the BSS/IBSS (WPA_SCAN_*)
 * @bssid: BSSID
 * @freq: frequency of the channel in MHz (e.g., 2412 = channel 1)
 * @beacon_int: beacon interval in TUs (host byte order)
 * @caps: capability information field in host byte order
 * @qual: signal quality
 * @noise: noise level
 * @level: signal level
 * @tsf: Timestamp
 * @age: Age of the information in milliseconds (i.e., how many milliseconds
 * ago the last Beacon or Probe Response frame was received)
 * @est_throughput: Estimated throughput in kbps (this is calculated during
 * scan result processing if left zero by the driver wrapper)
 * @snr: Signal-to-noise ratio in dB (calculated during scan result processing)
 * @ie_len: length of the following IE field in octets
 * @beacon_ie_len: length of the following Beacon IE field in octets
 *
 * This structure is used as a generic format for scan results from the
 * driver. Each driver interface implementation is responsible for converting
 * the driver or OS specific scan results into this format.
 *
 * If the driver does not support reporting all IEs, the IE data structure is
 * constructed of the IEs that are available. This field will also need to
 * include SSID in IE format. All drivers are encouraged to be extended to
 * report all IEs to make it easier to support future additions.
 *
 * This structure data is followed by ie_len octets of IEs from Probe Response
 * frame (or if the driver does not indicate source of IEs, these may also be
 * from Beacon frame). After the first set of IEs, another set of IEs may follow
 * (with beacon_ie_len octets of data) if the driver provides both IE sets.
 */
struct wpa_scan_res {
	unsigned int flags;
	u8 bssid[ETH_ALEN];
	int freq;
	u16 beacon_int;
	u16 caps;
	int qual;
	int noise;
	int level;
	u64 tsf;
	unsigned int age;
	unsigned int est_throughput;
	int snr;
	size_t ie_len;
	size_t beacon_ie_len;
	/* Followed by ie_len + beacon_ie_len octets of IE data */
};

/**
 * struct wpa_scan_results - Scan results
 * @res: Array of pointers to allocated variable length scan result entries
 * @num: Number of entries in the scan result array
 * @fetch_time: Time when the results were fetched from the driver
 */
struct wpa_scan_results {
	struct wpa_scan_res **res;
	size_t num;
	struct os_reltime fetch_time;
};

/**
 * struct wpa_signal_info - Information about channel signal quality
 */
struct wpa_signal_info {
	u32 frequency;
	int above_threshold;
	int current_signal;
	int avg_signal;
	int avg_beacon_signal;
	int current_noise;
	int current_txrate;
	enum chan_width chanwidth;
	int center_frq1;
	int center_frq2;
};

/**
 * struct wmm_params - WMM parameterss configured for this association
 * @info_bitmap: Bitmap of valid wmm_params info; indicates what fields
 *	of the struct contain valid information.
 * @uapsd_queues: Bitmap of ACs configured for uapsd (valid only if
 *	%WMM_PARAMS_UAPSD_QUEUES_INFO is set)
 */
struct wmm_params {
	u8 info_bitmap;
	u8 uapsd_queues;
};

struct wowlan_triggers {
	u8 any;
	u8 disconnect;
	u8 magic_pkt;
	u8 gtk_rekey_failure;
	u8 eap_identity_req;
	u8 four_way_handshake;
	u8 rfkill_release;
};

/**
 * enum wps_mode - WPS mode
 */
enum wps_mode {
	/**
	 * WPS_MODE_NONE - No WPS provisioning being used
	 */
	WPS_MODE_NONE,

	/**
	 * WPS_MODE_OPEN - WPS provisioning with AP that is in open mode
	 */
	WPS_MODE_OPEN,

	/**
	 * WPS_MODE_PRIVACY - WPS provisioning with AP that is using protection
	 */
	WPS_MODE_PRIVACY
};

/**
 * struct wpa_driver_capa - Driver capability information
 */
struct wpa_driver_capa {
#define WPA_DRIVER_CAPA_KEY_MGMT_WPA		0x00000001
#define WPA_DRIVER_CAPA_KEY_MGMT_WPA2		0x00000002
#define WPA_DRIVER_CAPA_KEY_MGMT_WPA_PSK	0x00000004
#define WPA_DRIVER_CAPA_KEY_MGMT_WPA2_PSK	0x00000008
#define WPA_DRIVER_CAPA_KEY_MGMT_WPA_NONE	0x00000010
#define WPA_DRIVER_CAPA_KEY_MGMT_FT		0x00000020
#define WPA_DRIVER_CAPA_KEY_MGMT_FT_PSK		0x00000040
#define WPA_DRIVER_CAPA_KEY_MGMT_WAPI_PSK	0x00000080
#define WPA_DRIVER_CAPA_KEY_MGMT_SUITE_B	0x00000100
#define WPA_DRIVER_CAPA_KEY_MGMT_SUITE_B_192	0x00000200
	/** Bitfield of supported key management suites */
	unsigned int key_mgmt;

#define WPA_DRIVER_CAPA_ENC_WEP40	0x00000001
#define WPA_DRIVER_CAPA_ENC_WEP104	0x00000002
#define WPA_DRIVER_CAPA_ENC_TKIP	0x00000004
#define WPA_DRIVER_CAPA_ENC_CCMP	0x00000008
#define WPA_DRIVER_CAPA_ENC_WEP128	0x00000010
#define WPA_DRIVER_CAPA_ENC_GCMP	0x00000020
#define WPA_DRIVER_CAPA_ENC_GCMP_256	0x00000040
#define WPA_DRIVER_CAPA_ENC_CCMP_256	0x00000080
#define WPA_DRIVER_CAPA_ENC_BIP		0x00000100
#define WPA_DRIVER_CAPA_ENC_BIP_GMAC_128	0x00000200
#define WPA_DRIVER_CAPA_ENC_BIP_GMAC_256	0x00000400
#define WPA_DRIVER_CAPA_ENC_BIP_CMAC_256	0x00000800
#define WPA_DRIVER_CAPA_ENC_GTK_NOT_USED	0x00001000
	/** Bitfield of supported cipher suites */
	unsigned int enc;

#define WPA_DRIVER_AUTH_OPEN		0x00000001
#define WPA_DRIVER_AUTH_SHARED		0x00000002
#define WPA_DRIVER_AUTH_LEAP		0x00000004
	/** Bitfield of supported IEEE 802.11 authentication algorithms */
	unsigned int auth;

	/** Driver generated WPA/RSN IE */
#define WPA_DRIVER_FLAGS_DRIVER_IE	0x00000001
	/** Driver needs static WEP key setup after association command */
#define WPA_DRIVER_FLAGS_SET_KEYS_AFTER_ASSOC 0x00000002
	/** Driver takes care of all DFS operations */
#define WPA_DRIVER_FLAGS_DFS_OFFLOAD			0x00000004
	/** Driver takes care of RSN 4-way handshake internally; PMK is configured with
	 * struct wpa_driver_ops::set_key using alg = WPA_ALG_PMK */
#define WPA_DRIVER_FLAGS_4WAY_HANDSHAKE 0x00000008
	/** Driver is for a wired Ethernet interface */
#define WPA_DRIVER_FLAGS_WIRED		0x00000010
	/** Driver provides separate commands for authentication and association (SME in
	 * wpa_supplicant). */
#define WPA_DRIVER_FLAGS_SME		0x00000020
	/** Driver supports AP mode */
#define WPA_DRIVER_FLAGS_AP		0x00000040
	/** Driver needs static WEP key setup after association has been completed */
#define WPA_DRIVER_FLAGS_SET_KEYS_AFTER_ASSOC_DONE	0x00000080
	/** Driver supports dynamic HT 20/40 MHz channel changes during BSS lifetime */
#define WPA_DRIVER_FLAGS_HT_2040_COEX			0x00000100
	/** Driver supports concurrent P2P operations */
#define WPA_DRIVER_FLAGS_P2P_CONCURRENT	0x00000200
	/**
	 * Driver uses the initial interface as a dedicated management interface, i.e.,
	 * it cannot be used for P2P group operations or non-P2P purposes.
	 */
#define WPA_DRIVER_FLAGS_P2P_DEDICATED_INTERFACE	0x00000400
	/** This interface is P2P capable (P2P GO or P2P Client) */
#define WPA_DRIVER_FLAGS_P2P_CAPABLE	0x00000800
	/** Driver supports station and key removal when stopping an AP */
#define WPA_DRIVER_FLAGS_AP_TEARDOWN_SUPPORT		0x00001000
	/**
	 * Driver uses the initial interface for P2P management interface and non-P2P
	 * purposes (e.g., connect to infra AP), but this interface cannot be used for
	 * P2P group operations.
	 */
#define WPA_DRIVER_FLAGS_P2P_MGMT_AND_NON_P2P		0x00002000
	/**
	 * Driver is known to use sane error codes, i.e., when it indicates that
	 * something (e.g., association) fails, there was indeed a failure and the
	 * operation does not end up getting completed successfully later.
	 */
#define WPA_DRIVER_FLAGS_SANE_ERROR_CODES		0x00004000
	/** Driver supports off-channel TX */
#define WPA_DRIVER_FLAGS_OFFCHANNEL_TX			0x00008000
	/** Driver indicates TX status events for EAPOL Data frames */
#define WPA_DRIVER_FLAGS_EAPOL_TX_STATUS		0x00010000
	/** Driver indicates TX status events for Deauth/Disassoc frames */
#define WPA_DRIVER_FLAGS_DEAUTH_TX_STATUS		0x00020000
	/** Driver supports roaming (BSS selection) in firmware */
#define WPA_DRIVER_FLAGS_BSS_SELECTION			0x00040000
	/** Driver supports operating as a TDLS peer */
#define WPA_DRIVER_FLAGS_TDLS_SUPPORT			0x00080000
	/** Driver requires external TDLS setup/teardown/discovery */
#define WPA_DRIVER_FLAGS_TDLS_EXTERNAL_SETUP		0x00100000
	/** Driver indicates support for Probe Response offloading in AP mode */
#define WPA_DRIVER_FLAGS_PROBE_RESP_OFFLOAD		0x00200000
	/** Driver supports U-APSD in AP mode */
#define WPA_DRIVER_FLAGS_AP_UAPSD			0x00400000
	/** Driver supports inactivity timer in AP mode */
#define WPA_DRIVER_FLAGS_INACTIVITY_TIMER		0x00800000
	/** Driver expects user space implementation of MLME in AP mode */
#define WPA_DRIVER_FLAGS_AP_MLME			0x01000000
	/** Driver supports SAE with user space SME */
#define WPA_DRIVER_FLAGS_SAE				0x02000000
	/** Driver makes use of OBSS scan mechanism in wpa_supplicant */
#define WPA_DRIVER_FLAGS_OBSS_SCAN			0x04000000
	/** Driver supports IBSS (Ad-hoc) mode */
#define WPA_DRIVER_FLAGS_IBSS				0x08000000
	/** Driver supports radar detection */
#define WPA_DRIVER_FLAGS_RADAR				0x10000000
	/** Driver supports a dedicated interface for P2P Device */
#define WPA_DRIVER_FLAGS_DEDICATED_P2P_DEVICE		0x20000000
	/** Driver supports QoS Mapping */
#define WPA_DRIVER_FLAGS_QOS_MAPPING			0x40000000
	/** Driver supports CSA in AP mode */
#define WPA_DRIVER_FLAGS_AP_CSA				0x80000000
	/** Driver supports mesh */
#define WPA_DRIVER_FLAGS_MESH			0x0000000100000000ULL
	/** Driver support ACS offload */
#define WPA_DRIVER_FLAGS_ACS_OFFLOAD		0x0000000200000000ULL
	/** Driver supports key management offload */
#define WPA_DRIVER_FLAGS_KEY_MGMT_OFFLOAD	0x0000000400000000ULL
	/** Driver supports TDLS channel switching */
#define WPA_DRIVER_FLAGS_TDLS_CHANNEL_SWITCH	0x0000000800000000ULL
	/** Driver supports IBSS with HT datarates */
#define WPA_DRIVER_FLAGS_HT_IBSS		0x0000001000000000ULL
	/** Driver supports IBSS with VHT datarates */
#define WPA_DRIVER_FLAGS_VHT_IBSS		0x0000002000000000ULL
	/** Driver supports automatic band selection */
#define WPA_DRIVER_FLAGS_SUPPORT_HW_MODE_ANY	0x0000004000000000ULL
	u64 flags;

#define WPA_DRIVER_SMPS_MODE_STATIC			0x00000001
#define WPA_DRIVER_SMPS_MODE_DYNAMIC			0x00000002
	unsigned int smps_modes;

	unsigned int wmm_ac_supported:1;

	unsigned int mac_addr_rand_scan_supported:1;
	unsigned int mac_addr_rand_sched_scan_supported:1;

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

	/**
	 * probe_resp_offloads - Bitmap of supported protocols by the driver
	 * for Probe Response offloading.
	 */
	/** Driver Probe Response offloading support for WPS ver. 1 */
#define WPA_DRIVER_PROBE_RESP_OFFLOAD_WPS		0x00000001
	/** Driver Probe Response offloading support for WPS ver. 2 */
#define WPA_DRIVER_PROBE_RESP_OFFLOAD_WPS2		0x00000002
	/** Driver Probe Response offloading support for P2P */
#define WPA_DRIVER_PROBE_RESP_OFFLOAD_P2P		0x00000004
	/** Driver Probe Response offloading support for IEEE 802.11u (Interworking) */
#define WPA_DRIVER_PROBE_RESP_OFFLOAD_INTERWORKING	0x00000008
	unsigned int probe_resp_offloads;

	unsigned int max_acl_mac_addrs;

	/**
	 * Number of supported concurrent channels
	 */
	unsigned int num_multichan_concurrent;

	/**
	 * extended_capa - extended capabilities in driver/device
	 *
	 * Must be allocated and freed by driver and the pointers must be
	 * valid for the lifetime of the driver, i.e., freed in deinit()
	 */
	const u8 *extended_capa, *extended_capa_mask;
	unsigned int extended_capa_len;

	struct wowlan_triggers wowlan_triggers;

	/** Driver adds the DS Params Set IE in Probe Request frames */
#define WPA_DRIVER_FLAGS_DS_PARAM_SET_IE_IN_PROBES	0x00000001
	/** Driver adds the WFA TPC IE in Probe Request frames */
#define WPA_DRIVER_FLAGS_WFA_TPC_IE_IN_PROBES		0x00000002
	/** Driver handles quiet period requests */
#define WPA_DRIVER_FLAGS_QUIET				0x00000004
	/**
	 * Driver is capable of inserting the current TX power value into the body of
	 * transmitted frames.
	 * Background: Some Action frames include a TPC Report IE. This IE contains a
	 * TX power field, which has to be updated by lower layers. One such Action
	 * frame is Link Measurement Report (part of RRM). Another is TPC Report (part
	 * of spectrum management). Note that this insertion takes place at a fixed
	 * offset, namely the 6th byte in the Action frame body.
	 */
#define WPA_DRIVER_FLAGS_TX_POWER_INSERTION		0x00000008
	u32 rrm_flags;
};

#define WPAS_MAX_SCAN_SSIDS 16
#define SSID_MAX_LEN 32

/**
 * struct wpa_driver_scan_params - Scan parameters
 * Data for struct wpa_driver_ops::scan2().
 */
struct wpa_driver_scan_params {
	/**
	 * ssids - SSIDs to scan for
	 */
	struct wpa_driver_scan_ssid {
		/**
		 * ssid - specific SSID to scan for (ProbeReq)
		 * %NULL or zero-length SSID is used to indicate active scan
		 * with wildcard SSID.
		 */
		const u8 *ssid;
		/**
		 * ssid_len: Length of the SSID in octets
		 */
		u8 ssid_len;
	} ssids[WPAS_MAX_SCAN_SSIDS];

	/**
	 * num_ssids - Number of entries in ssids array
	 * Zero indicates a request for a passive scan.
	 */
	size_t num_ssids;

	/**
	 * extra_ies - Extra IE(s) to add into Probe Request or %NULL
	 */
	const u8 *extra_ies;

	/**
	 * extra_ies_len - Length of extra_ies in octets
	 */
	size_t extra_ies_len;

	/**
	 * freqs - Array of frequencies to scan or %NULL for all frequencies
	 *
	 * The frequency is set in MHz. The array is zero-terminated.
	 */
	int *freqs;

	/**
	 * filter_ssids - Filter for reporting SSIDs
	 *
	 * This optional parameter can be used to request the driver wrapper to
	 * filter scan results to include only the specified SSIDs. %NULL
	 * indicates that no filtering is to be done. This can be used to
	 * reduce memory needs for scan results in environments that have large
	 * number of APs with different SSIDs.
	 *
	 * The driver wrapper is allowed to take this allocated buffer into its
	 * own use by setting the pointer to %NULL. In that case, the driver
	 * wrapper is responsible for freeing the buffer with os_free() once it
	 * is not needed anymore.
	 */
	struct wpa_driver_scan_filter {
		u8 ssid[SSID_MAX_LEN];
		size_t ssid_len;
	} *filter_ssids;

	/**
	 * num_filter_ssids - Number of entries in filter_ssids array
	 */
	size_t num_filter_ssids;

	/**
	 * filter_rssi - Filter by RSSI
	 *
	 * The driver may filter scan results in firmware to reduce host
	 * wakeups and thereby save power. Specify the RSSI threshold in s32
	 * dBm.
	 */
	s32 filter_rssi;

	/**
	 * p2p_probe - Used to disable CCK (802.11b) rates for P2P probes
	 *
	 * When set, the driver is expected to remove rates 1, 2, 5.5, and 11
	 * Mbps from the support rates element(s) in the Probe Request frames
	 * and not to transmit the frames at any of those rates.
	 */
	unsigned int p2p_probe:1;

	/**
	 * only_new_results - Request driver to report only new results
	 *
	 * This is used to request the driver to report only BSSes that have
	 * been detected after this scan request has been started, i.e., to
	 * flush old cached BSS entries.
	 */
	unsigned int only_new_results:1;

	/**
	 * low_priority - Requests driver to use a lower scan priority
	 *
	 * This is used to request the driver to use a lower scan priority
	 * if it supports such a thing.
	 */
	unsigned int low_priority:1;

	/**
	 * mac_addr_rand - Requests driver to randomize MAC address
	 */
	unsigned int mac_addr_rand:1;

	/**
	 * mac_addr - MAC address used with randomization. The address cannot be
	 * a multicast one, i.e., bit 0 of byte 0 should not be set.
	 */
	const u8 *mac_addr;

	/**
	 * mac_addr_mask - MAC address mask used with randomization.
	 *
	 * Bits that are 0 in the mask should be randomized. Bits that are 1 in
	 * the mask should be taken as is from mac_addr. The mask should not
	 * allow the generation of a multicast address, i.e., bit 0 of byte 0
	 * must be set.
	 */
	const u8 *mac_addr_mask;

	/*
	 * NOTE: Whenever adding new parameters here, please make sure
	 * wpa_scan_clone_params() and wpa_scan_free_params() get updated with
	 * matching changes.
	 */
};

/**
 * enum wpa_event_type - Event type for wpa_supplicant_event() calls
 */
enum wpa_event_type {
	/**
	 * EVENT_ASSOC - Association completed
	 *
	 * This event needs to be delivered when the driver completes IEEE
	 * 802.11 association or reassociation successfully.
	 * wpa_driver_ops::get_bssid() is expected to provide the current BSSID
	 * after this event has been generated. In addition, optional
	 * EVENT_ASSOCINFO may be generated just before EVENT_ASSOC to provide
	 * more information about the association. If the driver interface gets
	 * both of these events at the same time, it can also include the
	 * assoc_info data in EVENT_ASSOC call.
	 */
	EVENT_ASSOC,

	/**
	 * EVENT_DISASSOC - Association lost
	 *
	 * This event should be called when association is lost either due to
	 * receiving deauthenticate or disassociate frame from the AP or when
	 * sending either of these frames to the current AP. If the driver
	 * supports separate deauthentication event, EVENT_DISASSOC should only
	 * be used for disassociation and EVENT_DEAUTH for deauthentication.
	 * In AP mode, union wpa_event_data::disassoc_info is required.
	 */
	EVENT_DISASSOC,

	/**
	 * EVENT_MICHAEL_MIC_FAILURE - Michael MIC (TKIP) detected
	 *
	 * This event must be delivered when a Michael MIC error is detected by
	 * the local driver. Additional data for event processing is
	 * provided with union wpa_event_data::michael_mic_failure. This
	 * information is used to request new encyption key and to initiate
	 * TKIP countermeasures if needed.
	 */
	EVENT_MICHAEL_MIC_FAILURE,

	/**
	 * EVENT_SCAN_RESULTS - Scan results available
	 *
	 * This event must be called whenever scan results are available to be
	 * fetched with struct wpa_driver_ops::get_scan_results(). This event
	 * is expected to be used some time after struct wpa_driver_ops::scan()
	 * is called. If the driver provides an unsolicited event when the scan
	 * has been completed, this event can be used to trigger
	 * EVENT_SCAN_RESULTS call. If such event is not available from the
	 * driver, the driver wrapper code is expected to use a registered
	 * timeout to generate EVENT_SCAN_RESULTS call after the time that the
	 * scan is expected to be completed. Optional information about
	 * completed scan can be provided with union wpa_event_data::scan_info.
	 */
	EVENT_SCAN_RESULTS,

	/**
	 * EVENT_ASSOCINFO - Report optional extra information for association
	 *
	 * This event can be used to report extra association information for
	 * EVENT_ASSOC processing. This extra information includes IEs from
	 * association frames and Beacon/Probe Response frames in union
	 * wpa_event_data::assoc_info. EVENT_ASSOCINFO must be send just before
	 * EVENT_ASSOC. Alternatively, the driver interface can include
	 * assoc_info data in the EVENT_ASSOC call if it has all the
	 * information available at the same point.
	 */
	EVENT_ASSOCINFO,

	/**
	 * EVENT_INTERFACE_STATUS - Report interface status changes
	 *
	 * This optional event can be used to report changes in interface
	 * status (interface added/removed) using union
	 * wpa_event_data::interface_status. This can be used to trigger
	 * wpa_supplicant to stop and re-start processing for the interface,
	 * e.g., when a cardbus card is ejected/inserted.
	 */
	EVENT_INTERFACE_STATUS,

	/**
	 * EVENT_PMKID_CANDIDATE - Report a candidate AP for pre-authentication
	 *
	 * This event can be used to inform wpa_supplicant about candidates for
	 * RSN (WPA2) pre-authentication. If wpa_supplicant is not responsible
	 * for scan request (ap_scan=2 mode), this event is required for
	 * pre-authentication. If wpa_supplicant is performing scan request
	 * (ap_scan=1), this event is optional since scan results can be used
	 * to add pre-authentication candidates. union
	 * wpa_event_data::pmkid_candidate is used to report the BSSID of the
	 * candidate and priority of the candidate, e.g., based on the signal
	 * strength, in order to try to pre-authenticate first with candidates
	 * that are most likely targets for re-association.
	 *
	 * EVENT_PMKID_CANDIDATE can be called whenever the driver has updates
	 * on the candidate list. In addition, it can be called for the current
	 * AP and APs that have existing PMKSA cache entries. wpa_supplicant
	 * will automatically skip pre-authentication in cases where a valid
	 * PMKSA exists. When more than one candidate exists, this event should
	 * be generated once for each candidate.
	 *
	 * Driver will be notified about successful pre-authentication with
	 * struct wpa_driver_ops::add_pmkid() calls.
	 */
	EVENT_PMKID_CANDIDATE,

	/**
	 * EVENT_STKSTART - Request STK handshake (MLME-STKSTART.request)
	 *
	 * This event can be used to inform wpa_supplicant about desire to set
	 * up secure direct link connection between two stations as defined in
	 * IEEE 802.11e with a new PeerKey mechanism that replaced the original
	 * STAKey negotiation. The caller will need to set peer address for the
	 * event.
	 */
	EVENT_STKSTART,

	/**
	 * EVENT_TDLS - Request TDLS operation
	 *
	 * This event can be used to request a TDLS operation to be performed.
	 */
	EVENT_TDLS,

	/**
	 * EVENT_FT_RESPONSE - Report FT (IEEE 802.11r) response IEs
	 *
	 * The driver is expected to report the received FT IEs from
	 * FT authentication sequence from the AP. The FT IEs are included in
	 * the extra information in union wpa_event_data::ft_ies.
	 */
	EVENT_FT_RESPONSE,

	/**
	 * EVENT_IBSS_RSN_START - Request RSN authentication in IBSS
	 *
	 * The driver can use this event to inform wpa_supplicant about a STA
	 * in an IBSS with which protected frames could be exchanged. This
	 * event starts RSN authentication with the other STA to authenticate
	 * the STA and set up encryption keys with it.
	 */
	EVENT_IBSS_RSN_START,

	/**
	 * EVENT_AUTH - Authentication result
	 *
	 * This event should be called when authentication attempt has been
	 * completed. This is only used if the driver supports separate
	 * authentication step (struct wpa_driver_ops::authenticate).
	 * Information about authentication result is included in
	 * union wpa_event_data::auth.
	 */
	EVENT_AUTH,

	/**
	 * EVENT_DEAUTH - Authentication lost
	 *
	 * This event should be called when authentication is lost either due
	 * to receiving deauthenticate frame from the AP or when sending that
	 * frame to the current AP.
	 * In AP mode, union wpa_event_data::deauth_info is required.
	 */
	EVENT_DEAUTH,

	/**
	 * EVENT_ASSOC_REJECT - Association rejected
	 *
	 * This event should be called when (re)association attempt has been
	 * rejected by the AP. Information about the association response is
	 * included in union wpa_event_data::assoc_reject.
	 */
	EVENT_ASSOC_REJECT,

	/**
	 * EVENT_AUTH_TIMED_OUT - Authentication timed out
	 */
	EVENT_AUTH_TIMED_OUT,

	/**
	 * EVENT_ASSOC_TIMED_OUT - Association timed out
	 */
	EVENT_ASSOC_TIMED_OUT,

	/**
	 * EVENT_WPS_BUTTON_PUSHED - Report hardware push button press for WPS
	 */
	EVENT_WPS_BUTTON_PUSHED,

	/**
	 * EVENT_TX_STATUS - Report TX status
	 */
	EVENT_TX_STATUS,

	/**
	 * EVENT_RX_FROM_UNKNOWN - Report RX from unknown STA
	 */
	EVENT_RX_FROM_UNKNOWN,

	/**
	 * EVENT_RX_MGMT - Report RX of a management frame
	 */
	EVENT_RX_MGMT,

	/**
	 * EVENT_REMAIN_ON_CHANNEL - Remain-on-channel duration started
	 *
	 * This event is used to indicate when the driver has started the
	 * requested remain-on-channel duration. Information about the
	 * operation is included in union wpa_event_data::remain_on_channel.
	 */
	EVENT_REMAIN_ON_CHANNEL,

	/**
	 * EVENT_CANCEL_REMAIN_ON_CHANNEL - Remain-on-channel timed out
	 *
	 * This event is used to indicate when the driver has completed
	 * remain-on-channel duration, i.e., may noot be available on the
	 * requested channel anymore. Information about the
	 * operation is included in union wpa_event_data::remain_on_channel.
	 */
	EVENT_CANCEL_REMAIN_ON_CHANNEL,

	/**
	 * EVENT_RX_PROBE_REQ - Indicate received Probe Request frame
	 *
	 * This event is used to indicate when a Probe Request frame has been
	 * received. Information about the received frame is included in
	 * union wpa_event_data::rx_probe_req. The driver is required to report
	 * these events only after successfully completed probe_req_report()
	 * commands to request the events (i.e., report parameter is non-zero)
	 * in station mode. In AP mode, Probe Request frames should always be
	 * reported.
	 */
	EVENT_RX_PROBE_REQ,

	/**
	 * EVENT_NEW_STA - New wired device noticed
	 *
	 * This event is used to indicate that a new device has been detected
	 * in a network that does not use association-like functionality (i.e.,
	 * mainly wired Ethernet). This can be used to start EAPOL
	 * authenticator when receiving a frame from a device. The address of
	 * the device is included in union wpa_event_data::new_sta.
	 */
	EVENT_NEW_STA,

	/**
	 * EVENT_EAPOL_RX - Report received EAPOL frame
	 *
	 * When in AP mode with hostapd, this event is required to be used to
	 * deliver the receive EAPOL frames from the driver.
	 */
	EVENT_EAPOL_RX,

	/**
	 * EVENT_SIGNAL_CHANGE - Indicate change in signal strength
	 *
	 * This event is used to indicate changes in the signal strength
	 * observed in frames received from the current AP if signal strength
	 * monitoring has been enabled with signal_monitor().
	 */
	EVENT_SIGNAL_CHANGE,

	/**
	 * EVENT_INTERFACE_ENABLED - Notify that interface was enabled
	 *
	 * This event is used to indicate that the interface was enabled after
	 * having been previously disabled, e.g., due to rfkill.
	 */
	EVENT_INTERFACE_ENABLED,

	/**
	 * EVENT_INTERFACE_DISABLED - Notify that interface was disabled
	 *
	 * This event is used to indicate that the interface was disabled,
	 * e.g., due to rfkill.
	 */
	EVENT_INTERFACE_DISABLED,

	/**
	 * EVENT_CHANNEL_LIST_CHANGED - Channel list changed
	 *
	 * This event is used to indicate that the channel list has changed,
	 * e.g., because of a regulatory domain change triggered by scan
	 * results including an AP advertising a country code.
	 */
	EVENT_CHANNEL_LIST_CHANGED,

	/**
	 * EVENT_INTERFACE_UNAVAILABLE - Notify that interface is unavailable
	 *
	 * This event is used to indicate that the driver cannot maintain this
	 * interface in its operation mode anymore. The most likely use for
	 * this is to indicate that AP mode operation is not available due to
	 * operating channel would need to be changed to a DFS channel when
	 * the driver does not support radar detection and another virtual
	 * interfaces caused the operating channel to change. Other similar
	 * resource conflicts could also trigger this for station mode
	 * interfaces. This event can be propagated when channel switching
	 * fails.
	 */
	EVENT_INTERFACE_UNAVAILABLE,

	/**
	 * EVENT_BEST_CHANNEL
	 *
	 * Driver generates this event whenever it detects a better channel
	 * (e.g., based on RSSI or channel use). This information can be used
	 * to improve channel selection for a new AP/P2P group.
	 */
	EVENT_BEST_CHANNEL,

	/**
	 * EVENT_UNPROT_DEAUTH - Unprotected Deauthentication frame received
	 *
	 * This event should be called when a Deauthentication frame is dropped
	 * due to it not being protected (MFP/IEEE 802.11w).
	 * union wpa_event_data::unprot_deauth is required to provide more
	 * details of the frame.
	 */
	EVENT_UNPROT_DEAUTH,

	/**
	 * EVENT_UNPROT_DISASSOC - Unprotected Disassociation frame received
	 *
	 * This event should be called when a Disassociation frame is dropped
	 * due to it not being protected (MFP/IEEE 802.11w).
	 * union wpa_event_data::unprot_disassoc is required to provide more
	 * details of the frame.
	 */
	EVENT_UNPROT_DISASSOC,

	/**
	 * EVENT_STATION_LOW_ACK
	 *
	 * Driver generates this event whenever it detected that a particular
	 * station was lost. Detection can be through massive transmission
	 * failures for example.
	 */
	EVENT_STATION_LOW_ACK,

	/**
	 * EVENT_IBSS_PEER_LOST - IBSS peer not reachable anymore
	 */
	EVENT_IBSS_PEER_LOST,

	/**
	 * EVENT_DRIVER_GTK_REKEY - Device/driver did GTK rekey
	 *
	 * This event carries the new replay counter to notify wpa_supplicant
	 * of the current EAPOL-Key Replay Counter in case the driver/firmware
	 * completed Group Key Handshake while the host (including
	 * wpa_supplicant was sleeping).
	 */
	EVENT_DRIVER_GTK_REKEY,

	/**
	 * EVENT_SCHED_SCAN_STOPPED - Scheduled scan was stopped
	 */
	EVENT_SCHED_SCAN_STOPPED,

	/**
	 * EVENT_DRIVER_CLIENT_POLL_OK - Station responded to poll
	 *
	 * This event indicates that the station responded to the poll
	 * initiated with @poll_client.
	 */
	EVENT_DRIVER_CLIENT_POLL_OK,

	/**
	 * EVENT_EAPOL_TX_STATUS - notify of EAPOL TX status
	 */
	EVENT_EAPOL_TX_STATUS,

	/**
	 * EVENT_CH_SWITCH - AP or GO decided to switch channels
	 *
	 * Described in wpa_event_data.ch_switch
	 * */
	EVENT_CH_SWITCH,

	/**
	 * EVENT_WNM - Request WNM operation
	 *
	 * This event can be used to request a WNM operation to be performed.
	 */
	EVENT_WNM,

	/**
	 * EVENT_CONNECT_FAILED_REASON - Connection failure reason in AP mode
	 *
	 * This event indicates that the driver reported a connection failure
	 * with the specified client (for example, max client reached, etc.) in
	 * AP mode.
	 */
	EVENT_CONNECT_FAILED_REASON,

	/**
	 * EVENT_DFS_RADAR_DETECTED - Notify of radar detection
	 *
	 * A radar has been detected on the supplied frequency, hostapd should
	 * react accordingly (e.g., change channel).
	 */
	EVENT_DFS_RADAR_DETECTED,

	/**
	 * EVENT_DFS_CAC_FINISHED - Notify that channel availability check has been completed
	 *
	 * After a successful CAC, the channel can be marked clear and used.
	 */
	EVENT_DFS_CAC_FINISHED,

	/**
	 * EVENT_DFS_CAC_ABORTED - Notify that channel availability check has been aborted
	 *
	 * The CAC was not successful, and the channel remains in the previous
	 * state. This may happen due to a radar beeing detected or other
	 * external influences.
	 */
	EVENT_DFS_CAC_ABORTED,

	/**
	 * EVENT_DFS_NOP_FINISHED - Notify that non-occupancy period is over
	 *
	 * The channel which was previously unavailable is now available again.
	 */
	EVENT_DFS_NOP_FINISHED,

	/**
	 * EVENT_SURVEY - Received survey data
	 *
	 * This event gets triggered when a driver query is issued for survey
	 * data and the requested data becomes available. The returned data is
	 * stored in struct survey_results. The results provide at most one
	 * survey entry for each frequency and at minimum will provide one
	 * survey entry for one frequency. The survey data can be os_malloc()'d
	 * and then os_free()'d, so the event callback must only copy data.
	 */
	EVENT_SURVEY,

	/**
	 * EVENT_SCAN_STARTED - Scan started
	 *
	 * This indicates that driver has started a scan operation either based
	 * on a request from wpa_supplicant/hostapd or from another application.
	 * EVENT_SCAN_RESULTS is used to indicate when the scan has been
	 * completed (either successfully or by getting cancelled).
	 */
	EVENT_SCAN_STARTED,

	/**
	 * EVENT_AVOID_FREQUENCIES - Received avoid frequency range
	 *
	 * This event indicates a set of frequency ranges that should be avoided
	 * to reduce issues due to interference or internal co-existence
	 * information in the driver.
	 */
	EVENT_AVOID_FREQUENCIES,

	/**
	 * EVENT_NEW_PEER_CANDIDATE - new (unknown) mesh peer notification
	 */
	EVENT_NEW_PEER_CANDIDATE,

	/**
	 * EVENT_ACS_CHANNEL_SELECTED - Received selected channels by ACS
	 *
	 * Indicates a pair of primary and secondary channels chosen by ACS
	 * in device.
	 */
	EVENT_ACS_CHANNEL_SELECTED,

	/**
	 * EVENT_DFS_CAC_STARTED - Notify that channel availability check has
	 * been started.
	 *
	 * This event indicates that channel availability check has been started
	 * on a DFS frequency by a driver that supports DFS Offload.
	 */
	EVENT_DFS_CAC_STARTED,

	/**
	 * EVENT_HANGED - Driver hang event.
	 *
	 * Indicates the driver or firmware is in bad state and start the auto recovery process.
	 */
	EVENT_HANGED,
};

/**
 * union wpa_event_data - Additional data for wpa_supplicant_event() calls
 */
union wpa_event_data {
	/**
	 * struct assoc_info - Data for EVENT_ASSOC and EVENT_ASSOCINFO events
	 *
	 * This structure is optional for EVENT_ASSOC calls and required for
	 * EVENT_ASSOCINFO calls. By using EVENT_ASSOC with this data, the
	 * driver interface does not need to generate separate EVENT_ASSOCINFO
	 * calls.
	 */
	struct assoc_info {
		/**
		 * reassoc - Flag to indicate association or reassociation
		 */
		int reassoc;

		/**
		 * req_ies - (Re)Association Request IEs
		 *
		 * If the driver generates WPA/RSN IE, this event data must be
		 * returned for WPA handshake to have needed information. If
		 * wpa_supplicant-generated WPA/RSN IE is used, this
		 * information event is optional.
		 *
		 * This should start with the first IE (fixed fields before IEs
		 * are not included).
		 */
		const u8 *req_ies;

		/**
		 * req_ies_len - Length of req_ies in bytes
		 */
		size_t req_ies_len;

		/**
		 * resp_ies - (Re)Association Response IEs
		 *
		 * Optional association data from the driver. This data is not
		 * required WPA, but may be useful for some protocols and as
		 * such, should be reported if this is available to the driver
		 * interface.
		 *
		 * This should start with the first IE (fixed fields before IEs
		 * are not included).
		 */
		const u8 *resp_ies;

		/**
		 * resp_ies_len - Length of resp_ies in bytes
		 */
		size_t resp_ies_len;

		/**
		 * beacon_ies - Beacon or Probe Response IEs
		 *
		 * Optional Beacon/ProbeResp data: IEs included in Beacon or
		 * Probe Response frames from the current AP (i.e., the one
		 * that the client just associated with). This information is
		 * used to update WPA/RSN IE for the AP. If this field is not
		 * set, the results from previous scan will be used. If no
		 * data for the new AP is found, scan results will be requested
		 * again (without scan request). At this point, the driver is
		 * expected to provide WPA/RSN IE for the AP (if WPA/WPA2 is
		 * used).
		 *
		 * This should start with the first IE (fixed fields before IEs
		 * are not included).
		 */
		const u8 *beacon_ies;

		/**
		 * beacon_ies_len - Length of beacon_ies */
		size_t beacon_ies_len;

		/**
		 * freq - Frequency of the operational channel in MHz
		 */
		unsigned int freq;

		/**
		 * wmm_params - WMM parameters used in this association.
		 */
		struct wmm_params wmm_params;

		/**
		 * addr - Station address (for AP mode)
		 */
		const u8 *addr;

		/**
		 * The following is the key management offload information
		 * @authorized
		 * @key_replay_ctr
		 * @key_replay_ctr_len
		 * @ptk_kck
		 * @ptk_kek_len
		 * @ptk_kek
		 * @ptk_kek_len
		 */

		/**
		 * authorized - Status of key management offload,
		 * 1 = successful
		 */
		int authorized;

		/**
		 * key_replay_ctr - Key replay counter value last used
		 * in a valid EAPOL-Key frame
		 */
		const u8 *key_replay_ctr;

		/**
		 * key_replay_ctr_len - The length of key_replay_ctr
		 */
		size_t key_replay_ctr_len;

		/**
		 * ptk_kck - The derived PTK KCK
		 */
		const u8 *ptk_kck;

		/**
		 * ptk_kek_len - The length of ptk_kck
		 */
		size_t ptk_kck_len;

		/**
		 * ptk_kek - The derived PTK KEK
		 */
		const u8 *ptk_kek;

		/**
		 * ptk_kek_len - The length of ptk_kek
		 */
		size_t ptk_kek_len;
	} assoc_info;

	/**
	 * struct disassoc_info - Data for EVENT_DISASSOC events
	 */
	struct disassoc_info {
		/**
		 * addr - Station address (for AP mode)
		 */
		const u8 *addr;

		/**
		 * reason_code - Reason Code (host byte order) used in
		 *	Deauthentication frame
		 */
		u16 reason_code;

		/**
		 * ie - Optional IE(s) in Disassociation frame
		 */
		const u8 *ie;

		/**
		 * ie_len - Length of ie buffer in octets
		 */
		size_t ie_len;

		/**
		 * locally_generated - Whether the frame was locally generated
		 */
		int locally_generated;
	} disassoc_info;

	/**
	 * struct deauth_info - Data for EVENT_DEAUTH events
	 */
	struct deauth_info {
		/**
		 * addr - Station address (for AP mode)
		 */
		const u8 *addr;

		/**
		 * reason_code - Reason Code (host byte order) used in
		 *	Deauthentication frame
		 */
		u16 reason_code;

		/**
		 * ie - Optional IE(s) in Deauthentication frame
		 */
		const u8 *ie;

		/**
		 * ie_len - Length of ie buffer in octets
		 */
		size_t ie_len;

		/**
		 * locally_generated - Whether the frame was locally generated
		 */
		int locally_generated;
	} deauth_info;

	/**
	 * struct michael_mic_failure - Data for EVENT_MICHAEL_MIC_FAILURE
	 */
	struct michael_mic_failure {
		int unicast;
		const u8 *src;
	} michael_mic_failure;

	/**
	 * struct interface_status - Data for EVENT_INTERFACE_STATUS
	 */
	struct interface_status {
		char ifname[100];
		enum {
			EVENT_INTERFACE_ADDED, EVENT_INTERFACE_REMOVED
		} ievent;
	} interface_status;

	/**
	 * struct pmkid_candidate - Data for EVENT_PMKID_CANDIDATE
	 */
	struct pmkid_candidate {
		/** BSSID of the PMKID candidate */
		u8 bssid[ETH_ALEN];
		/** Smaller the index, higher the priority */
		int index;
		/** Whether RSN IE includes pre-authenticate flag */
		int preauth;
	} pmkid_candidate;

	/**
	 * struct stkstart - Data for EVENT_STKSTART
	 */
	struct stkstart {
		u8 peer[ETH_ALEN];
	} stkstart;

	/**
	 * struct tdls - Data for EVENT_TDLS
	 */
	struct tdls {
		u8 peer[ETH_ALEN];
		enum {
			TDLS_REQUEST_SETUP,
			TDLS_REQUEST_TEARDOWN,
			TDLS_REQUEST_DISCOVER,
		} oper;
		u16 reason_code;		/* for teardown */
	} tdls;

	/**
	 * struct wnm - Data for EVENT_WNM
	 */
	struct wnm {
		u8 addr[ETH_ALEN];
		enum {
			WNM_OPER_SLEEP,
		} oper;
		enum {
			WNM_SLEEP_ENTER,
			WNM_SLEEP_EXIT
		} sleep_action;
		int sleep_intval;
		u16 reason_code;
		u8 *buf;
		u16 buf_len;
	} wnm;

	/**
	 * struct ft_ies - FT information elements (EVENT_FT_RESPONSE)
	 *
	 * During FT (IEEE 802.11r) authentication sequence, the driver is
	 * expected to use this event to report received FT IEs (MDIE, FTIE,
	 * RSN IE, TIE, possible resource request) to the supplicant. The FT
	 * IEs for the next message will be delivered through the
	 * struct wpa_driver_ops::update_ft_ies() callback.
	 */
	struct ft_ies {
		const u8 *ies;
		size_t ies_len;
		int ft_action;
		u8 target_ap[ETH_ALEN];
		/** Optional IE(s), e.g., WMM TSPEC(s), for RIC-Request */
		const u8 *ric_ies;
		/** Length of ric_ies buffer in octets */
		size_t ric_ies_len;
	} ft_ies;

	/**
	 * struct ibss_rsn_start - Data for EVENT_IBSS_RSN_START
	 */
	struct ibss_rsn_start {
		u8 peer[ETH_ALEN];
	} ibss_rsn_start;

	/**
	 * struct auth_info - Data for EVENT_AUTH events
	 */
	struct auth_info {
		u8 peer[ETH_ALEN];
		u8 bssid[ETH_ALEN];
		u16 auth_type;
		u16 auth_transaction;
		u16 status_code;
		const u8 *ies;
		size_t ies_len;
	} auth;

	/**
	 * struct assoc_reject - Data for EVENT_ASSOC_REJECT events
	 */
	struct assoc_reject {
		/**
		 * bssid - BSSID of the AP that rejected association
		 */
		const u8 *bssid;

		/**
		 * resp_ies - (Re)Association Response IEs
		 *
		 * Optional association data from the driver. This data is not
		 * required WPA, but may be useful for some protocols and as
		 * such, should be reported if this is available to the driver
		 * interface.
		 *
		 * This should start with the first IE (fixed fields before IEs
		 * are not included).
		 */
		const u8 *resp_ies;

		/**
		 * resp_ies_len - Length of resp_ies in bytes
		 */
		size_t resp_ies_len;

		/**
		 * status_code - Status Code from (Re)association Response
		 */
		u16 status_code;
	} assoc_reject;

	struct timeout_event {
		u8 addr[ETH_ALEN];
	} timeout_event;

	/**
	 * struct tx_status - Data for EVENT_TX_STATUS events
	 */
	struct tx_status {
		u16 type;
		u16 stype;
		const u8 *dst;
		const u8 *data;
		size_t data_len;
		int ack;
	} tx_status;

	/**
	 * struct rx_from_unknown - Data for EVENT_RX_FROM_UNKNOWN events
	 */
	struct rx_from_unknown {
		const u8 *bssid;
		const u8 *addr;
		int wds;
	} rx_from_unknown;

	/**
	 * struct rx_mgmt - Data for EVENT_RX_MGMT events
	 */
	struct rx_mgmt {
		const u8 *frame;
		size_t frame_len;
		u32 datarate;

		/**
		 * drv_priv - Pointer to store driver private BSS information
		 *
		 * If not set to NULL, this is used for comparison with
		 * hostapd_data->drv_priv to determine which BSS should process
		 * the frame.
		 */
		void *drv_priv;

		/**
		 * freq - Frequency (in MHz) on which the frame was received
		 */
		int freq;

		/**
		 * ssi_signal - Signal strength in dBm (or 0 if not available)
		 */
		int ssi_signal;
	} rx_mgmt;

	/**
	 * struct remain_on_channel - Data for EVENT_REMAIN_ON_CHANNEL events
	 *
	 * This is also used with EVENT_CANCEL_REMAIN_ON_CHANNEL events.
	 */
	struct remain_on_channel {
		/**
		 * freq - Channel frequency in MHz
		 */
		unsigned int freq;

		/**
		 * duration - Duration to remain on the channel in milliseconds
		 */
		unsigned int duration;
	} remain_on_channel;

	/**
	 * struct scan_info - Optional data for EVENT_SCAN_RESULTS events
	 * @aborted: Whether the scan was aborted
	 * @freqs: Scanned frequencies in MHz (%NULL = all channels scanned)
	 * @num_freqs: Number of entries in freqs array
	 * @ssids: Scanned SSIDs (%NULL or zero-length SSID indicates wildcard
	 *	SSID)
	 * @num_ssids: Number of entries in ssids array
	 */
	struct scan_info {
		int aborted;
		const int *freqs;
		size_t num_freqs;
		struct wpa_driver_scan_ssid ssids[WPAS_MAX_SCAN_SSIDS];
		size_t num_ssids;
	} scan_info;

	/**
	 * struct rx_probe_req - Data for EVENT_RX_PROBE_REQ events
	 */
	struct rx_probe_req {
		/**
		 * sa - Source address of the received Probe Request frame
		 */
		const u8 *sa;

		/**
		 * da - Destination address of the received Probe Request frame
		 *	or %NULL if not available
		 */
		const u8 *da;

		/**
		 * bssid - BSSID of the received Probe Request frame or %NULL
		 *	if not available
		 */
		const u8 *bssid;

		/**
		 * ie - IEs from the Probe Request body
		 */
		const u8 *ie;

		/**
		 * ie_len - Length of ie buffer in octets
		 */
		size_t ie_len;

		/**
		 * signal - signal strength in dBm (or 0 if not available)
		 */
		int ssi_signal;
	} rx_probe_req;

	/**
	 * struct new_sta - Data for EVENT_NEW_STA events
	 */
	struct new_sta {
		const u8 *addr;
	} new_sta;

	/**
	 * struct eapol_rx - Data for EVENT_EAPOL_RX events
	 */
	struct eapol_rx {
		const u8 *src;
		const u8 *data;
		size_t data_len;
	} eapol_rx;

	/**
	 * signal_change - Data for EVENT_SIGNAL_CHANGE events
	 */
	struct wpa_signal_info signal_change;

	/**
	 * struct best_channel - Data for EVENT_BEST_CHANNEL events
	 * @freq_24: Best 2.4 GHz band channel frequency in MHz
	 * @freq_5: Best 5 GHz band channel frequency in MHz
	 * @freq_overall: Best channel frequency in MHz
	 *
	 * 0 can be used to indicate no preference in either band.
	 */
	struct best_channel {
		int freq_24;
		int freq_5;
		int freq_overall;
	} best_chan;

	struct unprot_deauth {
		const u8 *sa;
		const u8 *da;
		u16 reason_code;
	} unprot_deauth;

	struct unprot_disassoc {
		const u8 *sa;
		const u8 *da;
		u16 reason_code;
	} unprot_disassoc;

	/**
	 * struct low_ack - Data for EVENT_STATION_LOW_ACK events
	 * @addr: station address
	 */
	struct low_ack {
		u8 addr[ETH_ALEN];
	} low_ack;

	/**
	 * struct ibss_peer_lost - Data for EVENT_IBSS_PEER_LOST
	 */
	struct ibss_peer_lost {
		u8 peer[ETH_ALEN];
	} ibss_peer_lost;

	/**
	 * struct driver_gtk_rekey - Data for EVENT_DRIVER_GTK_REKEY
	 */
	struct driver_gtk_rekey {
		const u8 *bssid;
		const u8 *replay_ctr;
	} driver_gtk_rekey;

	/**
	 * struct client_poll - Data for EVENT_DRIVER_CLIENT_POLL_OK events
	 * @addr: station address
	 */
	struct client_poll {
		u8 addr[ETH_ALEN];
	} client_poll;

	/**
	 * struct eapol_tx_status
	 * @dst: Original destination
	 * @data: Data starting with IEEE 802.1X header (!)
	 * @data_len: Length of data
	 * @ack: Indicates ack or lost frame
	 *
	 * This corresponds to hapd_send_eapol if the frame sent
	 * there isn't just reported as EVENT_TX_STATUS.
	 */
	struct eapol_tx_status {
		const u8 *dst;
		const u8 *data;
		int data_len;
		int ack;
	} eapol_tx_status;

	/**
	 * struct ch_switch
	 * @freq: Frequency of new channel in MHz
	 * @ht_enabled: Whether this is an HT channel
	 * @ch_offset: Secondary channel offset
	 * @ch_width: Channel width
	 * @cf1: Center frequency 1
	 * @cf2: Center frequency 2
	 */
	struct ch_switch {
		int freq;
		int ht_enabled;
		int ch_offset;
		enum chan_width ch_width;
		int cf1;
		int cf2;
	} ch_switch;

	/**
	 * struct connect_failed - Data for EVENT_CONNECT_FAILED_REASON
	 * @addr: Remote client address
	 * @code: Reason code for connection failure
	 */
	struct connect_failed_reason {
		u8 addr[ETH_ALEN];
		enum {
			MAX_CLIENT_REACHED,
			BLOCKED_CLIENT
		} code;
	} connect_failed_reason;

	/**
	 * struct dfs_event - Data for radar detected events
	 * @freq: Frequency of the channel in MHz
	 */
	struct dfs_event {
		int freq;
		int ht_enabled;
		int chan_offset;
		enum chan_width chan_width;
		int cf1;
		int cf2;
	} dfs_event;

	/**
	 * survey_results - Survey result data for EVENT_SURVEY
	 * @freq_filter: Requested frequency survey filter, 0 if request
	 *	was for all survey data
	 * @survey_list: Linked list of survey data (struct freq_survey)
	 */
	struct survey_results {
		unsigned int freq_filter;
		struct dl_list survey_list;	/* struct freq_survey */
	} survey_results;

	/**
	 * channel_list_changed - Data for EVENT_CHANNEL_LIST_CHANGED
	 * @initiator: Initiator of the regulatory change
	 * @type: Regulatory change type
	 * @alpha2: Country code (or "" if not available)
	 */
	struct channel_list_changed {
		enum reg_change_initiator initiator;
		enum reg_type type;
		char alpha2[3];
	} channel_list_changed;

	/**
	 * freq_range - List of frequency ranges
	 *
	 * This is used as the data with EVENT_AVOID_FREQUENCIES.
	 */
	struct wpa_freq_range_list freq_range;

	/**
	 * struct mesh_peer
	 *
	 * @peer: Peer address
	 * @ies: Beacon IEs
	 * @ie_len: Length of @ies
	 *
	 * Notification of new candidate mesh peer.
	 */
	struct mesh_peer {
		const u8 *peer;
		const u8 *ies;
		size_t ie_len;
	} mesh_peer;

	/**
	 * struct acs_selected_channels - Data for EVENT_ACS_CHANNEL_SELECTED
	 * @pri_channel: Selected primary channel
	 * @sec_channel: Selected secondary channel
	 * @vht_seg0_center_ch: VHT mode Segment0 center channel
	 * @vht_seg1_center_ch: VHT mode Segment1 center channel
	 * @ch_width: Selected Channel width by driver. Driver may choose to
	 *	change hostapd configured ACS channel width due driver internal
	 *	channel restrictions.
	 * hw_mode: Selected band (used with hw_mode=any)
	 */
	struct acs_selected_channels {
		u8 pri_channel;
		u8 sec_channel;
		u8 vht_seg0_center_ch;
		u8 vht_seg1_center_ch;
		u16 ch_width;
		enum hostapd_hw_mode hw_mode;
	} acs_selected_channels;

	/**
	 * struct hanged_event - Data for EVENT_HANGED events
	 * @panic_code: Panic code
	 */
	struct hanged_event {
		u16 panic_code;
	} hanged_event;
};

/* struct hostapd_freq_params - Channel parameters
 */
struct hostapd_freq_params {
	/**
	 * mode - Mode/band (HOSTAPD_MODE_IEEE80211A, ..)
	 */
	enum hostapd_hw_mode mode;

	/**
	 * freq - Primary channel center frequency in MHz
	 */
	int freq;

	/**
	 * channel - Channel number
	 */
	int channel;

	/**
	 * ht_enabled - Whether HT is enabled
	 */
	int ht_enabled;

	/**
	 * sec_channel_offset - Secondary channel offset for HT40
	 *
	 * 0 = HT40 disabled,
	 * -1 = HT40 enabled, secondary channel below primary,
	 * 1 = HT40 enabled, secondary channel above primary
	 */
	int sec_channel_offset;

	/**
	 * vht_enabled - Whether VHT is enabled
	 */
	int vht_enabled;

	/**
	 * center_freq1 - Segment 0 center frequency in MHz
	 *
	 * Valid for both HT and VHT.
	 */
	int center_freq1;

	/**
	 * center_freq2 - Segment 1 center frequency in MHz
	 *
	 * Non-zero only for bandwidth 80 and an 80+80 channel
	 */
	int center_freq2;

	/**
	 * bandwidth - Channel bandwidth in MHz (20, 40, 80, 160)
	 */
	int bandwidth;
};

enum hide_ssid {
	NO_SSID_HIDING,
	HIDDEN_SSID_ZERO_LEN,
	HIDDEN_SSID_ZERO_CONTENTS
};

/**
 * struct wpa_driver_associate_params - Association parameters
 * Data for struct wpa_driver_ops::associate().
 */
struct wpa_driver_associate_params {
	/**
	 * bssid - BSSID of the selected AP
	 * This can be %NULL, if ap_scan=2 mode is used and the driver is
	 * responsible for selecting with which BSS to associate. */
	const u8 *bssid;

	/**
	 * bssid_hint - BSSID of a proposed AP
	 *
	 * This indicates which BSS has been found a suitable candidate for
	 * initial association for drivers that use driver/firmwate-based BSS
	 * selection. Unlike the @bssid parameter, @bssid_hint does not limit
	 * the driver from selecting other BSSes in the ESS.
	 */
	const u8 *bssid_hint;

	/**
	 * ssid - The selected SSID
	 */
	const u8 *ssid;

	/**
	 * ssid_len - Length of the SSID (1..32)
	 */
	size_t ssid_len;

	/**
	 * freq - channel parameters
	 */
	struct hostapd_freq_params freq;

	/**
	 * freq_hint - Frequency of the channel the proposed AP is using
	 *
	 * This provides a channel on which a suitable BSS has been found as a
	 * hint for the driver. Unlike the @freq parameter, @freq_hint does not
	 * limit the driver from selecting other channels for
	 * driver/firmware-based BSS selection.
	 */
	int freq_hint;

	/**
	 * bg_scan_period - Background scan period in seconds, 0 to disable
	 * background scan, or -1 to indicate no change to default driver
	 * configuration
	 */
	int bg_scan_period;

	/**
	 * beacon_int - Beacon interval for IBSS or 0 to use driver default
	 */
	int beacon_int;

	/**
	 * wpa_ie - WPA information element for (Re)Association Request
	 * WPA information element to be included in (Re)Association
	 * Request (including information element id and length). Use
	 * of this WPA IE is optional. If the driver generates the WPA
	 * IE, it can use pairwise_suite, group_suite, and
	 * key_mgmt_suite to select proper algorithms. In this case,
	 * the driver has to notify wpa_supplicant about the used WPA
	 * IE by generating an event that the interface code will
	 * convert into EVENT_ASSOCINFO data (see below).
	 *
	 * When using WPA2/IEEE 802.11i, wpa_ie is used for RSN IE
	 * instead. The driver can determine which version is used by
	 * looking at the first byte of the IE (0xdd for WPA, 0x30 for
	 * WPA2/RSN).
	 *
	 * When using WPS, wpa_ie is used for WPS IE instead of WPA/RSN IE.
	 */
	const u8 *wpa_ie;

	/**
	 * wpa_ie_len - length of the wpa_ie
	 */
	size_t wpa_ie_len;

	/**
	 * wpa_proto - Bitfield of WPA_PROTO_* values to indicate WPA/WPA2
	 */
	unsigned int wpa_proto;

	/**
	 * pairwise_suite - Selected pairwise cipher suite (WPA_CIPHER_*)
	 *
	 * This is usually ignored if @wpa_ie is used.
	 */
	unsigned int pairwise_suite;

	/**
	 * group_suite - Selected group cipher suite (WPA_CIPHER_*)
	 *
	 * This is usually ignored if @wpa_ie is used.
	 */
	unsigned int group_suite;

	/**
	 * key_mgmt_suite - Selected key management suite (WPA_KEY_MGMT_*)
	 *
	 * This is usually ignored if @wpa_ie is used.
	 */
	unsigned int key_mgmt_suite;

	/**
	 * auth_alg - Allowed authentication algorithms
	 * Bit field of WPA_AUTH_ALG_*
	 */
	int auth_alg;

	/**
	 * mode - Operation mode (infra/ibss) IEEE80211_MODE_*
	 */
	int mode;

	/**
	 * wep_key - WEP keys for static WEP configuration
	 */
	const u8 *wep_key[4];

	/**
	 * wep_key_len - WEP key length for static WEP configuration
	 */
	size_t wep_key_len[4];

	/**
	 * wep_tx_keyidx - WEP TX key index for static WEP configuration
	 */
	int wep_tx_keyidx;

	/**
	 * passphrase - RSN passphrase for PSK
	 *
	 * This value is made available only for WPA/WPA2-Personal (PSK) and
	 * only for drivers that set WPA_DRIVER_FLAGS_4WAY_HANDSHAKE. This is
	 * the 8..63 character ASCII passphrase, if available. Please note that
	 * this can be %NULL if passphrase was not used to generate the PSK. In
	 * that case, the psk field must be used to fetch the PSK.
	 */
	const char *passphrase;

	/**
	 * psk - RSN PSK (alternative for passphrase for PSK)
	 *
	 * This value is made available only for WPA/WPA2-Personal (PSK) and
	 * only for drivers that set WPA_DRIVER_FLAGS_4WAY_HANDSHAKE. This is
	 * the 32-octet (256-bit) PSK, if available. The driver wrapper should
	 * be prepared to handle %NULL value as an error.
	 */
	const u8 *psk;

	/**
	 * drop_unencrypted - Enable/disable unencrypted frame filtering
	 *
	 * Configure the driver to drop all non-EAPOL frames (both receive and
	 * transmit paths). Unencrypted EAPOL frames (ethertype 0x888e) must
	 * still be allowed for key negotiation.
	 */
	int drop_unencrypted;

	/**
	 * prev_bssid - Previously used BSSID in this ESS
	 *
	 * When not %NULL, this is a request to use reassociation instead of
	 * association.
	 */
	const u8 *prev_bssid;

	/**
	 * wps - WPS mode
	 *
	 * If the driver needs to do special configuration for WPS association,
	 * this variable provides more information on what type of association
	 * is being requested. Most drivers should not need ot use this.
	 */
	enum wps_mode wps;

	/**
	 * p2p - Whether this connection is a P2P group
	 */
	int p2p;

	/**
	 * uapsd - UAPSD parameters for the network
	 * -1 = do not change defaults
	 * AP mode: 1 = enabled, 0 = disabled
	 * STA mode: bits 0..3 UAPSD enabled for VO,VI,BK,BE
	 */
	int uapsd;

	/**
	 * fixed_bssid - Whether to force this BSSID in IBSS mode
	 * 1 = Fix this BSSID and prevent merges.
	 * 0 = Do not fix BSSID.
	 */
	int fixed_bssid;

	/**
	 * fixed_freq - Fix control channel in IBSS mode
	 * 0 = don't fix control channel (default)
	 * 1 = fix control channel; this prevents IBSS merging with another
	 *	channel
	 */
	int fixed_freq;

	/**
	 * disable_ht - Disable HT (IEEE 802.11n) for this connection
	 */
	int disable_ht;

	/**
	 * htcaps - HT Capabilities over-rides
	 *
	 * Only bits set in the mask will be used, and not all values are used
	 * by the kernel anyway. Currently, MCS, MPDU and MSDU fields are used.
	 *
	 * Pointer to struct ieee80211_ht_capabilities.
	 */
	const u8 *htcaps;

	/**
	 * htcaps_mask - HT Capabilities over-rides mask
	 *
	 * Pointer to struct ieee80211_ht_capabilities.
	 */
	const u8 *htcaps_mask;

#ifdef CONFIG_VHT_OVERRIDES
	/**
	 * disable_vht - Disable VHT for this connection
	 */
	int disable_vht;

	/**
	 * VHT capability overrides.
	 */
	const struct ieee80211_vht_capabilities *vhtcaps;
	const struct ieee80211_vht_capabilities *vhtcaps_mask;
#endif							/* CONFIG_VHT_OVERRIDES */

	/**
	 * req_key_mgmt_offload - Request key management offload for connection
	 *
	 * Request key management offload for this connection if the device
	 * supports it.
	 */
	int req_key_mgmt_offload;

	/**
	 * Flag for indicating whether this association includes support for
	 * RRM (Radio Resource Measurements)
	 */
	int rrm_used;
};

struct wpa_driver_ap_params {
	/**
	 * head - Beacon head from IEEE 802.11 header to IEs before TIM IE
	 */
	u8 *head;

	/**
	 * head_len - Length of the head buffer in octets
	 */
	size_t head_len;

	/**
	 * tail - Beacon tail following TIM IE
	 */
	u8 *tail;

	/**
	 * tail_len - Length of the tail buffer in octets
	 */
	size_t tail_len;

	/**
	 * dtim_period - DTIM period
	 */
	int dtim_period;

	/**
	 * beacon_int - Beacon interval
	 */
	int beacon_int;

	/**
	 * basic_rates: -1 terminated array of basic rates in 100 kbps
	 *
	 * This parameter can be used to set a specific basic rate set for the
	 * BSS. If %NULL, default basic rate set is used.
	 */
	int *basic_rates;

	/**
	 * proberesp - Probe Response template
	 *
	 * This is used by drivers that reply to Probe Requests internally in
	 * AP mode and require the full Probe Response template.
	 */
	u8 *proberesp;

	/**
	 * proberesp_len - Length of the proberesp buffer in octets
	 */
	size_t proberesp_len;

	/**
	 * ssid - The SSID to use in Beacon/Probe Response frames
	 */
	const u8 *ssid;

	/**
	 * ssid_len - Length of the SSID (1..32)
	 */
	size_t ssid_len;

	/**
	 * hide_ssid - Whether to hide the SSID
	 */
	enum hide_ssid hide_ssid;

	/**
	 * pairwise_ciphers - WPA_CIPHER_* bitfield
	 */
	unsigned int pairwise_ciphers;

	/**
	 * group_cipher - WPA_CIPHER_*
	 */
	unsigned int group_cipher;

	/**
	 * key_mgmt_suites - WPA_KEY_MGMT_* bitfield
	 */
	unsigned int key_mgmt_suites;

	/**
	 * auth_algs - WPA_AUTH_ALG_* bitfield
	 */
	unsigned int auth_algs;

	/**
	 * wpa_version - WPA_PROTO_* bitfield
	 */
	unsigned int wpa_version;

	/**
	 * privacy - Whether privacy is used in the BSS
	 */
	int privacy;

	/**
	 * beacon_ies - WPS/P2P IE(s) for Beacon frames
	 *
	 * This is used to add IEs like WPS IE and P2P IE by drivers that do
	 * not use the full Beacon template.
	 */
	const struct wpabuf *beacon_ies;

	/**
	 * proberesp_ies - P2P/WPS IE(s) for Probe Response frames
	 *
	 * This is used to add IEs like WPS IE and P2P IE by drivers that
	 * reply to Probe Request frames internally.
	 */
	const struct wpabuf *proberesp_ies;

	/**
	 * assocresp_ies - WPS IE(s) for (Re)Association Response frames
	 *
	 * This is used to add IEs like WPS IE by drivers that reply to
	 * (Re)Association Request frames internally.
	 */
	const struct wpabuf *assocresp_ies;

	/**
	 * isolate - Whether to isolate frames between associated stations
	 *
	 * If this is non-zero, the AP is requested to disable forwarding of
	 * frames between associated stations.
	 */
	int isolate;

	/**
	 * cts_protect - Whether CTS protection is enabled
	 */
	int cts_protect;

	/**
	 * preamble - Whether short preamble is enabled
	 */
	int preamble;

	/**
	 * short_slot_time - Whether short slot time is enabled
	 *
	 * 0 = short slot time disable, 1 = short slot time enabled, -1 = do
	 * not set (e.g., when 802.11g mode is not in use)
	 */
	int short_slot_time;

	/**
	 * ht_opmode - HT operation mode or -1 if HT not in use
	 */
	int ht_opmode;

	/**
	 * interworking - Whether Interworking is enabled
	 */
	int interworking;

	/**
	 * hessid - Homogeneous ESS identifier or %NULL if not set
	 */
	const u8 *hessid;

	/**
	 * access_network_type - Access Network Type (0..15)
	 *
	 * This is used for filtering Probe Request frames when Interworking is
	 * enabled.
	 */
	u8 access_network_type;

	/**
	 * ap_max_inactivity - Timeout in seconds to detect STA's inactivity
	 *
	 * This is used by driver which advertises this capability.
	 */
	int ap_max_inactivity;

	/**
	 * ctwindow - Client Traffic Window (in TUs)
	 */
	u8 p2p_go_ctwindow;

	/**
	 * smps_mode - SMPS mode
	 *
	 * SMPS mode to be used by the AP, specified as the relevant bits of
	 * ht_capab (i.e. HT_CAP_INFO_SMPS_*).
	 */
	unsigned int smps_mode;

	/**
	 * disable_dgaf - Whether group-addressed frames are disabled
	 */
	int disable_dgaf;

	/**
	 * osen - Whether OSEN security is enabled
	 */
	int osen;

	/**
	 * freq - Channel parameters for dynamic bandwidth changes
	 */
	struct hostapd_freq_params *freq;

	/**
	 * reenable - Whether this is to re-enable beaconing
	 */
	int reenable;
};

/**
 * struct hostapd_channel_data - Channel information
 */
struct hostapd_channel_data {
	/**
	 * chan - Channel number (IEEE 802.11)
	 */
	short chan;

	/**
	 * freq - Frequency in MHz
	 */
	int freq;

	/**
	 * flag - Channel flags (HOSTAPD_CHAN_*)
	 */
	int flag;

	/**
	 * max_tx_power - Regulatory transmit power limit in dBm
	 */
	u8 max_tx_power;

	/**
	 * survey_list - Linked list of surveys (struct freq_survey)
	 */
	struct dl_list survey_list;

	/**
	 * min_nf - Minimum observed noise floor, in dBm, based on all
	 * surveyed channel data
	 */
	s8 min_nf;

#ifdef CONFIG_ACS
	/**
	 * interference_factor - Computed interference factor on this
	 * channel (used internally in src/ap/acs.c; driver wrappers do not
	 * need to set this)
	 */
	long double interference_factor;
#endif							/* CONFIG_ACS */

	/**
	 * dfs_cac_ms - DFS CAC time in milliseconds
	 */
	unsigned int dfs_cac_ms;
};

#define HOSTAPD_MODE_FLAG_HT_INFO_KNOWN BIT(0)
#define HOSTAPD_MODE_FLAG_VHT_INFO_KNOWN BIT(1)

struct hostapd_hw_modes {
	/**
	 * mode - Hardware mode
	 */
	enum hostapd_hw_mode mode;

	/**
	 * num_channels - Number of entries in the channels array
	 */
	int num_channels;

	/**
	 * channels - Array of supported channels
	 */
	struct hostapd_channel_data *channels;

	/**
	 * num_rates - Number of entries in the rates array
	 */
	int num_rates;

	/**
	 * rates - Array of supported rates in 100 kbps units
	 */
	int *rates;

	/**
	 * ht_capab - HT (IEEE 802.11n) capabilities
	 */
	u16 ht_capab;

	/**
	 * mcs_set - MCS (IEEE 802.11n) rate parameters
	 */
	u8 mcs_set[16];

	/**
	 * a_mpdu_params - A-MPDU (IEEE 802.11n) parameters
	 */
	u8 a_mpdu_params;

	/**
	 * vht_capab - VHT (IEEE 802.11ac) capabilities
	 */
	u32 vht_capab;

	/**
	 * vht_mcs_set - VHT MCS (IEEE 802.11ac) rate parameters
	 */
	u8 vht_mcs_set[8];

	unsigned int flags;			/* HOSTAPD_MODE_FLAG_* */
};

typedef u16 __bitwise be16;
typedef u16 __bitwise le16;
typedef u32 __bitwise be32;
typedef u32 __bitwise le32;
typedef u64 __bitwise be64;
typedef u64 __bitwise le64;

struct ieee80211_mgmt {
	le16 frame_control;
	le16 duration;
	u8 da[6];
	u8 sa[6];
	u8 bssid[6];
	le16 seq_ctrl;
	union {
		struct {
			le16 auth_alg;
			le16 auth_transaction;
			le16 status_code;
			/* possibly followed by Challenge text */
			u8 variable[];
		} STRUCT_PACKED auth;
		struct {
			le16 reason_code;
			u8 variable[];
		} STRUCT_PACKED deauth;
		struct {
			le16 capab_info;
			le16 listen_interval;
			/* followed by SSID and Supported rates */
			u8 variable[];
		} STRUCT_PACKED assoc_req;
		struct {
			le16 capab_info;
			le16 status_code;
			le16 aid;
			/* followed by Supported rates */
			u8 variable[];
		} STRUCT_PACKED assoc_resp, reassoc_resp;
		struct {
			le16 capab_info;
			le16 listen_interval;
			u8 current_ap[6];
			/* followed by SSID and Supported rates */
			u8 variable[];
		} STRUCT_PACKED reassoc_req;
		struct {
			le16 reason_code;
			u8 variable[];
		} STRUCT_PACKED disassoc;
		struct {
			u8 timestamp[8];
			le16 beacon_int;
			le16 capab_info;
			/* followed by some of SSID, Supported rates,
			 * FH Params, DS Params, CF Params, IBSS Params, TIM */
			u8 variable[];
		} STRUCT_PACKED beacon;
		struct {
			/* only variable items: SSID, Supported rates */
			u8 variable[0];
		} STRUCT_PACKED probe_req;
		struct {
			u8 timestamp[8];
			le16 beacon_int;
			le16 capab_info;
			/* followed by some of SSID, Supported rates,
			 * FH Params, DS Params, CF Params, IBSS Params */
			u8 variable[];
		} STRUCT_PACKED probe_resp;
		struct {
			u8 category;
			union {
				struct {
					u8 action_code;
					u8 dialog_token;
					u8 status_code;
					u8 variable[];
				} STRUCT_PACKED wmm_action;
				struct {
					u8 action_code;
					u8 element_id;
					u8 length;
					u8 switch_mode;
					u8 new_chan;
					u8 switch_count;
				} STRUCT_PACKED chan_switch;
				struct {
					u8 action;
					u8 sta_addr[ETH_ALEN];
					u8 target_ap_addr[ETH_ALEN];
					u8 variable[];	/* FT Request */
				} STRUCT_PACKED ft_action_req;
				struct {
					u8 action;
					u8 sta_addr[ETH_ALEN];
					u8 target_ap_addr[ETH_ALEN];
					le16 status_code;
					u8 variable[];	/* FT Request */
				} STRUCT_PACKED ft_action_resp;
				struct {
					u8 action;
					u8 trans_id[2];
				} STRUCT_PACKED sa_query_req;
				struct {
					u8 action;	/* */
					u8 trans_id[2];
				} STRUCT_PACKED sa_query_resp;
				struct {
					u8 action;
					u8 dialogtoken;
					u8 variable[];
				} STRUCT_PACKED wnm_sleep_req;
				struct {
					u8 action;
					u8 dialogtoken;
					le16 keydata_len;
					u8 variable[];
				} STRUCT_PACKED wnm_sleep_resp;
				struct {
					u8 action;
					u8 variable[];
				} STRUCT_PACKED public_action;
				struct {
					u8 action;	/* 9 */
					u8 oui[3];
					/* Vendor-specific content */
					u8 variable[];
				} STRUCT_PACKED vs_public_action;
				struct {
					u8 action;	/* 7 */
					u8 dialog_token;
					u8 req_mode;
					le16 disassoc_timer;
					u8 validity_interval;
					/* BSS Termination Duration (optional),
					 * Session Information URL (optional),
					 * BSS Transition Candidate List
					 * Entries */
					u8 variable[];
				} STRUCT_PACKED bss_tm_req;
				struct {
					u8 action;	/* 8 */
					u8 dialog_token;
					u8 status_code;
					u8 bss_termination_delay;
					/* Target BSSID (optional),
					 * BSS Transition Candidate List
					 * Entries (optional) */
					u8 variable[];
				} STRUCT_PACKED bss_tm_resp;
				struct {
					u8 action;	/* 6 */
					u8 dialog_token;
					u8 query_reason;
					/* BSS Transition Candidate List
					 * Entries (optional) */
					u8 variable[];
				} STRUCT_PACKED bss_tm_query;
				struct {
					u8 action;	/* 15 */
					u8 variable[];
				} STRUCT_PACKED slf_prot_action;
			} u;
		} STRUCT_PACKED action;
	} u;
} STRUCT_PACKED;

/*
 * Internal data structure for wpabuf. Please do not touch this directly from
 * elsewhere. This is only defined in header file to allow inline functions
 * from this file to access data.
 */
struct wpabuf {
	size_t size;				/* total size of the allocated buffer */
	size_t used;				/* length of data in the buffer */
	u8 *buf;					/* pointer to the head of the buffer */
	unsigned int flags;
	/* optionally followed by the allocated buffer */
};

/**
 * wpa_supplicant_event_send - Report a driver event for wpa_supplicant
 * @ctx: Context pointer (wpa_s); this is the ctx variable registered
 *	with struct wpa_driver_ops::init()
 * @event: event type (defined above)
 * @data: possible extra data for the event
 *
 * Driver wrapper code should call this function whenever an event is received
 * from the driver.
 */
extern void wpa_supplicant_event_send(void *ctx, enum wpa_event_type event, union wpa_event_data *data);

#endif							/* WPA_DRIVER_COPY_H */
