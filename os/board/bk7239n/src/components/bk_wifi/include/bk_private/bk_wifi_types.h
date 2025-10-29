// Copyright 2020-2025 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/* TODO
 *
 * Make it to new WiFi API
 * */
#pragma once

#include <stdint.h>
#include <modules/wifi_types.h>
#include <common/bk_compiler.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief WLAN SSID and passphrase definition
 */
#define WLAN_SCAN_SSID_MAX      6 //2  /* max # of SSIDs */
#define WLAN_SSID_MAX_LEN       32
#define WLAN_PASSPHRASE_MIN_LEN 8
#define WLAN_PASSPHRASE_MAX_LEN 63
#define WLAN_PSK_HEX_LEN        32
#define WLAN_PSK_HEX_STR_LEN    64 /* two characters per octet of PSK */

/**
 * @brief WLAN WEP key length definition
 */
#define WLAN_WEP40_KEY_LEN      5  /* 5-byte  (40-bit)  */
#define WLAN_WEP104_KEY_LEN     13 /* 13-byte (104-bit) */
#define WLAN_WEP128_KEY_LEN     16 /* 16-byte (128-bit), unsupported */
#define WLAN_WEP_KEY_MAX_LEN    WLAN_WEP128_KEY_LEN

#define WLAN_MAC_LEN		6

#ifndef ETH_ALEN
#define ETH_ALEN		6
#endif

/**
 * Maximum number of scan results that can be stored.
 * Can be customized via Kconfig: CONFIG_CUSTOM_MAX_SCAN_RST_NUM
 * Default value: 120 (SCANU_MAX_RESULTS)
 */
#ifndef MAX_BSS_LIST
#ifdef CONFIG_CUSTOM_MAX_SCAN_RST_NUM
#define MAX_BSS_LIST            CONFIG_CUSTOM_MAX_SCAN_RST_NUM
#else
#define MAX_BSS_LIST            120  /* Default SCANU_MAX_RESULTS */
#endif
#endif

/**
 * @brief Wlan ssid definition
 */
typedef struct wlan_ssid {
	uint8_t ssid[WLAN_SSID_MAX_LEN];
	uint8_t ssid_len;
} wlan_ssid_t;

typedef struct {
	uint8_t bssid[ETH_ALEN];
	uint8_t ssid[WLAN_SSID_MAX_LEN];
	uint8_t ssid_len;
	uint16_t freq;
	uint16_t beacon_int;
	uint16_t caps;
	int level;
	// u64 tsf;
	uint16_t ie_len;
	uint8_t  ies[0];  /* FIXME: use dynamic len */
} wlan_sta_add_bss_t;

typedef struct {
	int8_t rssi;
	int len;
	uint8_t bcn[0];
} wlan_sta_new_rx_beacon_t;

typedef struct {
	uint8_t bssid[ETH_ALEN];
	int akmp;
	uint8_t pmk_len;
	uint8_t pmk[64];
	uint8_t pmkid[16];
} wlan_sta_add_pmksa_cache_entry_t;

/**
 * @brief Wlan station configuration field definition
 */
typedef enum wlan_sta_field {
	WLAN_STA_FIELD_SSID = 0,
	WLAN_STA_FIELD_BSSID,
	WLAN_STA_FIELD_PSK,
	WLAN_STA_FIELD_WEP_KEY0,
	WLAN_STA_FIELD_WEP_KEY1,
	WLAN_STA_FIELD_WEP_KEY2,
	WLAN_STA_FIELD_WEP_KEY3,
	WLAN_STA_FIELD_WEP_KEY_INDEX,
	WLAN_STA_FIELD_KEY_MGMT,
	WLAN_STA_FIELD_PAIRWISE_CIPHER,
	WLAN_STA_FIELD_GROUP_CIPHER,
	WLAN_STA_FIELD_PROTO,
	WLAN_STA_FIELD_AUTH_ALG,
	WLAN_STA_FIELD_WPA_PTK_REKEY,
	WLAN_STA_FIELD_SCAN_SSID,
	WLAN_STA_FIELD_FREQ,	/* only used in fast connect */
	WLAN_STA_FIELD_SAE_GROUPS,
	WLAN_STA_FIELD_MFP,
	WLAN_STA_FIELD_OCV,
	WLAN_STA_FIELD_EAP,
	WLAN_STA_FIELD_IDENTITY,
	WLAN_STA_FIELD_CA_CERT,
	WLAN_STA_FIELD_CLIENT_CERT,
	WLAN_STA_FIELD_PRIVATE_KEY,
	WLAN_STA_FIELD_PRIVATE_KEY_PASSWD,
	WLAN_STA_FIELD_PHASE1,

	/* for debug */
	WLAN_STA_FIELD_DEBUG_LEVEL,
	WLAN_STA_FIELD_DEBUG_SHOW_KEYS,

	WLAN_STA_FIELD_NUM,
} wlan_sta_field_t;

/**
 * @brief Wlan station configuration definition
 */
typedef struct wlan_sta_config {
	wlan_sta_field_t field;
#ifdef CONFIG_WPA_SUPPLICANT_MULTI_NETWORK
	int id;	/* network id */
#endif

	union {
		/**
		 * Network name
		 */
		wlan_ssid_t ssid;

		/**
		 * bssid - BSSID
		 *
		 * If set, this network block is used only when associating with the AP
		 * using the configured BSSID
		 */
		uint8_t bssid[WLAN_MAC_LEN];

		/**
		 * fast_connect_freq
		 */
		int channel;

		/**
		 * WPA preshared key in one of the optional formats:
		 *   - an ASCII string of passphrase, length is [8, 63]
		 *   - a hex string of PSK (two characters per octet of PSK), length is 64
		 */
		char psk[65];

		/**
		 * WEP key in one of the optional formats:
		 *   - an ASCII string with double quotation, length is {5, 13}
		 *   - a hex string (two characters per octet), length is {10, 26}
		 */
		char wep_key[WLAN_WEP_KEY_MAX_LEN * 2 + 1];

		/**
		 * Default key index for TX frames using WEP
		 */
		int wep_tx_keyidx;

		/**
		 * Bitfield of allowed key management protocols
		 *
		 * WPA_KEY_MGMT_*
		 */
		int key_mgmt;

		/**
		 * Bitfield of allowed pairwise ciphers
		 *
		 * WPA_CIPHER_*
		 */
		int pairwise_cipher;

		/**
		 * Bitfield of allowed group ciphers
		 *
		 * WPA_CIPHER_*
		 */
		int group_cipher;

		/**
		 * Bitfield of allowed protocols
		 *
		 * WPA_PROTO_*
		 */
		int proto;

		/**
		 * Bitfield of allowed authentication algorithms
		 *
		 * WPA_AUTH_ALG_*
		 */
		int auth_alg;

		/**
		 * Maximum lifetime for PTK in seconds
		 *
		 * This value can be used to enforce rekeying of PTK to
		 * mitigate some attacks against TKIP deficiencies.
		 */
		int wpa_ptk_rekey;

		/**
		 * Scan this SSID with Probe Requests
		 *
		 * scan_ssid can be used to scan for APs using hidden SSIDs.
		 */
		int scan_ssid;


		int sae_groups[16];

		int ieee80211w;

		/* ocv capability */
		int ocv;
		int debug_level;
		int debug_show_keys;

		/* EAP related */
		char eap[64];
		char identity[64];
		char ca_cert[64];
		char client_cert[64];
		char private_key[64];
		char private_key_passwd[64];
		char phase1[64];		/* phase1 config */
	} u;
} wlan_sta_config_t;

/**
 * @brief Wlan station connection state definition
 */
typedef enum wlan_sta_states {
	WLAN_STA_STATE_DISCONNECTED = 0,
	WLAN_STA_STATE_CONNECTED = 1,
} wlan_sta_states_t;

/**
 * @brief Wlan AP information definition
 */
typedef struct wlan_sta_ap {
	wlan_ssid_t	ssid;
	uint8_t		bssid[6];
	uint8_t		channel;
	uint16_t	beacon_int;
	int		freq;
	int		rssi;
	int		level;
	int		wpa_flags;
	int		wpa_cipher;
	int		wpa_key_mgmt;
	int		wpa2_cipher;
	int		wpa2_key_mgmt;
} wlan_sta_ap_t;

/**
 * @brief Wlan station scan parameters definition
 */
typedef struct wlan_sta_scan_param {
	uint8_t scan_only;    /* do scan only */
	uint8_t scan_passive; /* passive scan */
	uint8_t scan_cc;      /* country code scan */
	uint8_t scan_ssid;    /* Scan SSID of configured network with Probe Requests */
	uint8_t num_ssids;
	wlan_ssid_t ssids[WLAN_SCAN_SSID_MAX];
	uint32_t id;
} wlan_sta_scan_param_t;

/**
 * @brief Wlan station scan results definition
 */
typedef struct wlan_sta_scan_results {
	wlan_sta_ap_t *ap;
	int size;
	int num;
} wlan_sta_scan_results_t;

/**
 * @brief Wlan station bss infomation definition
 */
typedef struct wlan_sta_bss_info {
	uint8_t *bss;
	uint32_t size;
} wlan_sta_bss_info_t;

/**
 * @brief Parameter of generating WPA PSK based on passphrase and SSID
 */
typedef struct wlan_gen_psk_param {
	uint8_t ssid[WLAN_SSID_MAX_LEN];
	uint8_t ssid_len;
	char passphrase[WLAN_PASSPHRASE_MAX_LEN + 1];
	uint8_t psk[WLAN_PSK_HEX_LEN]; /* out */
} wlan_gen_psk_param_t;

/**
 * @brief Wlan WPS pin definition
 */
typedef struct wlan_sta_wps_pin {
	char pin[9];
} wlan_sta_wps_pin_t;

/**
 * @brief Wlan auto reconnect definition
 *
 * count     auto reconnect retry count, 0 for no restrict
 * timeout   auto reconnect timeout, 0 for no restrict
 * disable_reconnect_when_disconnect  disable reconnect when disconnect by AP if current
 *           state is connect with AP.
 */
typedef struct wlan_auto_reconnect {
	int max_count;
	int timeout;
	//bool disable_reconnect_when_disconnect;
	unsigned int disable_reconnect_when_disconnect;
} wlan_auto_reconnect_t;

/**
 * @brief Wlan AP configuration field definition
 */
typedef enum wlan_ap_field {
	WLAN_AP_FIELD_SSID = 0,
	WLAN_AP_FIELD_PSK,
	WLAN_AP_FIELD_KEY_MGMT,
	WLAN_AP_FIELD_WPA_CIPHER,
	WLAN_AP_FIELD_RSN_CIPHER,
	WLAN_AP_FIELD_PROTO,
	WLAN_AP_FIELD_AUTH_ALG,
	WLAN_AP_FIELD_GROUP_REKEY,
	WLAN_AP_FIELD_STRICT_REKEY,
	WLAN_AP_FIELD_GMK_REKEY,
	WLAN_AP_FIELD_PTK_REKEY,
	WLAN_AP_FIELD_HW_MODE,
	WLAN_AP_FIELD_IEEE80211N,
	WLAN_AP_FIELD_CHANNEL,
	WLAN_AP_FIELD_BEACON_INT,
	WLAN_AP_FIELD_DTIM,
	WLAN_AP_FIELD_MAX_NUM_STA,

	WLAN_AP_FIELD_NUM,
} wlan_ap_field_t;

/**
 * @brief Wlan AP hardware mode definition
 */
typedef enum wlan_ap_hw_mode {
	WLAN_AP_HW_MODE_IEEE80211B = 0,
	WLAN_AP_HW_MODE_IEEE80211G,
	WLAN_AP_HW_MODE_IEEE80211A,
	WLAN_AP_HW_MODE_IEEE80211AD,

	WLAN_AP_HW_MODE_NUM,
} wlan_ap_hw_mode_t;

/**
 * @brief Wlan AP configuration definition
 */
typedef struct wlan_ap_config {
	wlan_ap_field_t field;

	union {
		/**
		 * Network name
		 */
		wlan_ssid_t ssid;

		/**
		 * WPA preshared key in one of the optional formats:
		 *   - an ASCII string of passphrase, length is [8, 63]
		 *   - a hex string of PSK (two characters per octet of PSK), length is 64
		 */
		uint8_t psk[65];

		/**
		 * Bitfield of allowed key management protocols
		 *
		 * WPA_KEY_MGMT_*
		 */
		int key_mgmt;

		/**
		 * Bitfield of allowed WPA pairwise ciphers
		 *
		 * WPA_CIPHER_*
		 */
		int wpa_cipher;

		/**
		 * Bitfield of allowed RSN pairwise ciphers
		 *
		 * WPA_CIPHER_*
		 */
		int rsn_cipher;

		/**
		 * Bitfield of allowed protocols
		 *
		 * WPA_PROTO_*
		 */
		int proto;

		/**
		 * Bitfield of allowed authentication algorithms
		 *
		 * WPA_AUTH_ALG_*
		 */
		int auth_alg;

		/**
		 * Maximum lifetime for GTK in seconds
		 */
		int group_rekey;

		/**
		 * Rekey GTK when any STA that possesses the current GTK is
		 * leaving the BSS
		 */
		int strict_rekey;

		/**
		 * Maximum lifetime for GMK in seconds
		 */
		int gmk_rekey;

		/**
		 * Maximum lifetime for PTK in seconds
		 */
		int ptk_rekey;

		/**
		 * Hardware mode
		 */
		wlan_ap_hw_mode_t hw_mode;

		/**
		 * IEEE802.11n mode
		 */
		int ieee80211n;

		/**
		 * RF channel
		 */
		uint8_t channel;

		/**
		 * MIB defines range as 1..65535, but very small values
		 * cause problems with the current implementation.
		 * Since it is unlikely that this small numbers are
		 * useful in real life scenarios, do not allow beacon
		 * period to be set below 15 TU.
		 */
		uint16_t beacon_int;

		/**
		 * Delivery traffic indication message
		 */
		int dtim;

		/**
		 * Maximum number of STAs in station table
		 */
		int max_num_sta;
	} u;
} wlan_ap_config_t;

struct wlan_p2p_connect_param {
	uint8_t addr[6];
	int method;
	int intent;
};

/* TODO - we shoul finally remove the INTERNAL event, all WiFi events are
 * from supplicant and are public.
 * */
enum {
	WIFI_INTERNAL_EVENT_STA_CONNECTED = 0,
	WIFI_INTERNAL_EVENT_STA_DISCONNECTED,
	WIFI_INTERNAL_EVENT_MAX,
};

enum {
	WIFI_PS_CMD_OPEN= 0,
	WIFI_PS_CMD_CLOSE,
	WIFI_PS_CMD_DEBUG_PRINTF,
	WIFI_PS_CMD_TD_INFO,
	WIFI_PS_CMD_TD_MASK,
};

/**
 * @addr: softap deauth sta's mac addr, all 0xff for deauth all STA.
 * @reason: reason code: <=0 for automatic select
 */
typedef struct {
	uint8_t addr[6];
	int     reason;
} wlan_ap_sta_deauth_t;

typedef struct {
	uint8_t addr[6];
} wlan_ap_blacklist_t;

/* bk_net_param */

#define INFO_TLV_HEADER           (0x00564c54)   // ASCII TLV

#define NET_INFO_MAC_ADDR_OFFSET			0x0
#define NET_INFO_FAST_CONNECT_OFFSET		0x6

typedef enum {
	FAST_CONNECT_ITEM	= 0x00001111,
	AUTO_CONNECT_ITEM	= 0x11111111,
	WIFI_MODE_ITEM	= 0x22222222,
	DHCP_MODE_ITEM	= 0x33333333,
	WIFI_MAC_ITEM	= 0x44444444,
	SSID_KEY_ITEM	= 0x55555555,
	IP_CONFIG_ITEM	= 0x66666666,
	RF_CFG_TSSI_ITEM    = 0x77777777,
	RF_CFG_DIST_ITEM    = 0x88888888,
	RF_CFG_MODE_ITEM    = 0x99999999,
	CHARGE_CONFIG_ITEM  = 0xaaaaaaaa,
	RF_CFG_TSSI_B_ITEM  = 0xbbbbbbbb,
	USER_COMM1_ITEM	= 0x11112222,
	DID_STR_ITEM	= 0xcccccccc,
	USER_PSW_ITEM	= 0xdddddddd,
	CAMERA_PARA_ITEM	= 0xeeeeeeee,
	REBOOT_FLAG_ITEM    = 0xffffffff
} NET_INFO_ITEM;

typedef struct info_item_st {
	unsigned int type;
	unsigned int len;
} INFO_ITEM_ST, TLV_HEADER_ST, *INFO_ITEM_ST_PTR;

typedef struct item_common_st {
	INFO_ITEM_ST head;
	unsigned int value;
} ITEM_COMM_ST, *ITEM_COMM_ST_PTR;

typedef struct item_mac_addr_st {
	INFO_ITEM_ST head;
	char mac[6];
	char reserved[2];// 4bytes boundary
} ITEM_MAC_ADDR_ST, *ITEM_MAC_ADDR_ST_PTR;

typedef struct item_charge_st {
	INFO_ITEM_ST head;
#if (CONFIG_SOC_BK7251)
	char chrg[4];
#else
	char chrg[3];
	char reserved[1];
#endif
} ITEM_CHARGE_ST, *ITEM_CHARGE_ST_PTR;

typedef struct item_fastconnect_st {
	uint8_t ssid[33];
	uint8_t bssid[6];
	uint8_t security;
	uint8_t channel;
	uint8_t psk[65];
	uint8_t pwd[65];
	uint8_t ip_addr[4];
	uint8_t netmask[4];
	uint8_t gw[4];
	uint8_t dns1[4];
#if CONFIG_WLAN_FAST_CONNECT_WITHOUT_SCAN
	uint16_t freq;
	u16 beacon_int;
	uint16_t caps;
	int level;
	// u64 tsf;
	uint16_t ie_len;
	uint8_t ies[1024];	/* FIXME: use dynamic len */
#endif
	// #if CONFIG_WIFI_MFP_CONNECT_DEAUTH
	uint8_t pmf;
	uint8_t tk[16];
	// #endif
#if CONFIG_WLAN_FAST_CONNECT_WPA3
	uint8_t pmk_len;
	uint8_t pmk[64]; // for WPA2 Personal, pmk = psk
	uint8_t pmkid[16];
	int akmp;
#endif
	/* aes attention: sizeof(RL_BSSID_INFO_T) = 16 * n */
	uint8_t padding[0] __attribute__((aligned(16)));
} ITEM_FASTCONNECT_ST, *ITEM_FASTCONNECT_ST_PTR;

typedef struct item_ssidkey_st {
	INFO_ITEM_ST head;
	char wifi_ssid[32];
	char wifi_key[64];
} ITEM_SSIDKEY_ST, *ITEM_SSIDKEY_ST_PTR;

typedef struct item_ip_config_st {
	INFO_ITEM_ST head;
	char local_ip_addr[16];
	char net_mask[16];
	char gateway_ip_addr[16];
} ITEM_IP_CONFIG_ST, *ITEM_IP_CONFIG_ST_PTR;

/* bk_wifi_debug */
/* IP Header */
typedef struct Ipv4Hdr {
    uint8_t   ver_hl;              /* version/header length */
    uint8_t   tos;                 /* type of service */
    uint16_t  length;              /* total length */
    uint16_t  id;                  /* identification */
    uint16_t  offset;              /* fragment offset field */
    uint8_t   ttl;                 /* time to live */
    uint8_t   protocol;            /* protocol */
    uint16_t  sum;                 /* checksum */
    uint32_t  src;                 /* source address */
    uint32_t  dst;                 /* destination address */
} Ipv4Hdr;


/* TCP Header */
typedef struct TcpHdr {
    uint16_t  srcPort;            /* source port */
    uint16_t  dstPort;            /* destination port */
    uint32_t  sequence;           /* sequence number */
    uint32_t  ack;                /* acknoledgment number */
    uint8_t   offset;             /* data offset, reserved */
    uint8_t   flags;              /* option flags */
    uint16_t  window;             /* window */
    uint16_t  sum;                /* checksum */
    uint16_t  urgent;             /* urgent pointer */
} TcpHdr;

/* UDP Header */
typedef struct UdpHdr {
    uint16_t  srcPort;            /* source port */
    uint16_t  dstPort;            /* destination port */
    uint16_t  len;                /* len */
    uint16_t  sum;                /* checksum */
} UdpHdr;


#define RAW_TX_AC                   AC_VO
#define RAW_TX_AC_TIMEOUT_MS        50

/* bk_wifi_frame */
/// MAC address structure.
struct wifi_mac_addr
{
    uint8_t array[WIFI_MAC_LEN];
} __attribute__((packed));

/// Structure of a long control frame MAC header
struct wifi_mac_hdr
{
    /// Frame control
    uint16_t fctl;
    /// Duration/ID
    uint16_t durid;
    /// Address 1
    struct wifi_mac_addr addr1;
    /// Address 2
    struct wifi_mac_addr addr2;
    /// Address 3
    struct wifi_mac_addr addr3;
    /// Sequence control
    uint16_t seq;
} __attribute__((packed));

/// Structure of a Beacon or ProbeRsp frame
struct wifi_bcn_frame
{
    /// MAC Header
    struct wifi_mac_hdr h;
    /// Timestamp
    uint64_t tsf;
    /// Beacon interval
    uint16_t bcnint;
    /// Capability information
    uint16_t capa;
    /// Rest of the payload
    uint8_t variable[];
} __attribute__((packed));

/// MIC Header Length -> DA (6 bytes) + SA (6 bytes) + Priority (4 bytes)
#define MIC_HDR_LEN     (16)
/// IV Length
#define IV_LEN          (4)
/// EIV Length
#define EIV_LEN         (4)
/// MIC Length
#define MIC_LEN         (8)
/// ICV Length
#define ICV_LEN         (4)
// WPI IV length
#define WPI_IV_LEN      (18)
// WPI MIC length
#define WPI_MIC_LEN     (16)

/// Word 1 of CCMP IV
#define EIV_PRESENT     (0x2000)

/**
 * MAC HEADER LENGTH DEFINITIONS
 ****************************************************************************************
 */
/// Long MAC Header length (with QoS control field and HT control field)
#define MAC_LONG_QOS_HTC_MAC_HDR_LEN   36
/// Long MAC Header length (with QoS control field)
#define MAC_LONG_QOS_MAC_HDR_LEN       32
/// Long MAC Header length (without QoS control field)
#define MAC_LONG_MAC_HDR_LEN           30
/// Short MAC Header length (with QoS control field and HT control field)
#define MAC_SHORT_QOS_HTC_MAC_HDR_LEN  30
/// Short MAC Header length (with QoS control field)
#define MAC_SHORT_QOS_MAC_HDR_LEN      26
/// Short MAC Header length (without QoS control field)
#define MAC_SHORT_MAC_HDR_LEN          24

/// QoS Control Field Length
#define MAC_HDR_QOS_CTRL_LEN           2

/// Longest possible MAC Header
#define MAC_HDR_LEN_MAX                MAC_LONG_QOS_HTC_MAC_HDR_LEN

/// Long control frame header length
#define MAC_LONG_CTRLFRAME_LEN         16
/// Short control frame header length (ACK/CTS)
#define MAC_SHORT_CTRLFRAME_LEN        10

/**
 * MAC Header offset definitions
 */
/// 802.11 MAC header definition
#define MAC_HEAD_FCTRL_OFT              0
#define MAC_HEAD_DURATION_OFT           2
#define MAC_HEAD_DURATION_CFP      0x8000
#define MAC_HEAD_ADDR1_OFT              4
#define MAC_HEAD_ADDR2_OFT             10
#define MAC_HEAD_ADDR3_OFT             16
#define MAC_HEAD_CTRL_OFT              22
#define MAC_HEAD_ADDR4_OFT             24
#define MAC_HEAD_SHORT_QOS_OFT         24
#define MAC_HEAD_LONG_QOS_OFT          30
#define MAC_ORIGINAL_ETHTYPE_OFT       36

/**
 * Frame Control bit field definitions
 */
/* The type information in the header of a frame consists of the Type and Subtype fields
 * When using the term "frame type" in the code, we refer to the type field and not to
 * the combined type+subtype information.
 */
/// 802.11 frame control definition
#define MAC_FCTRL_LEN                   2

#define MAC_FCTRL_PROTOCOLVERSION_MASK  0x0003
#define MAC_FCTRL_TYPE_MASK             0x000C
#define MAC_FCTRL_SUBT_MASK             0x00F0
#define MAC_FCTRL_TODS                  0x0100
#define MAC_FCTRL_FROMDS                0x0200
#define MAC_FCTRL_MOREFRAG              0x0400
#define MAC_FCTRL_RETRY                 0x0800
#define MAC_FCTRL_PWRMGT                0x1000
#define MAC_FCTRL_MOREDATA              0x2000
#define MAC_FCTRL_PROTECTEDFRAME        0x4000
#define MAC_FCTRL_ORDER                 0x8000

#define MAC_FCTRL_TODS_FROMDS          (MAC_FCTRL_TODS | MAC_FCTRL_FROMDS)

/**
 * Frame Control Type definitions
 */
/// 802.11 type definition
#define MAC_FCTRL_MGT_T                 0x0000
#define MAC_FCTRL_CTRL_T                0x0004
#define MAC_FCTRL_DATA_T                0x0008

/**
 * Frame Control Subtype definitions
 */
/// 802.11 subtype definition
// Management SubType
#define MAC_FCTRL_ASSOCREQ_ST           0x0000
#define MAC_FCTRL_ASSOCRSP_ST           0x0010
#define MAC_FCTRL_REASSOCREQ_ST         0x0020
#define MAC_FCTRL_REASSOCRSP_ST         0x0030
#define MAC_FCTRL_PROBEREQ_ST           0x0040
#define MAC_FCTRL_PROBERSP_ST           0x0050
#define MAC_FCTRL_BEACON_ST             0x0080
#define MAC_FCTRL_ATIM_ST               0x0090
#define MAC_FCTRL_DISASSOC_ST           0x00A0
#define MAC_FCTRL_AUTHENT_ST            0x00B0
#define MAC_FCTRL_DEAUTHENT_ST          0x00C0
#define MAC_FCTRL_ACTION_ST             0x00D0
#define MAC_FCTRL_ACTION_NO_ACK_ST      0x00E0
// Control SubTypes
#define MAC_FCTRL_HE_TRIGGER_ST         0x0020
#define MAC_FCTRL_BFM_REPORT_POLL_ST    0x0040
#define MAC_FCTRL_VHT_NDPA_ST           0x0050
#define MAC_FCTRL_CTRL_WRAPPER_ST       0x0070
#define MAC_FCTRL_BAR_ST                0x0080
#define MAC_FCTRL_BA_ST                 0x0090
#define MAC_FCTRL_PSPOLL_ST             0x00A0
#define MAC_FCTRL_RTS_ST                0x00B0
#define MAC_FCTRL_CTS_ST                0x00C0
#define MAC_FCTRL_ACK_ST                0x00D0
#define MAC_FCTRL_CFEND_ST              0x00E0
#define MAC_FCTRL_CFEND_CFACK_ST        0x00F0

// Data SubTypes
/* Decoding the subtypes of data type frames can take advantage of the fact that
 * each subtype field bit position is used to indicate a specific modification of
 * the basic data frame (subtype 0). Frame control bit 4 is set to 1 in data
 * subtypes which include +CF-Ack, bit 5 is set to 1 in data subtypes which include
 * +CF-Poll, bit 6 is set to 1 in data subtypes that contain no Frame Body,
 * and bit 7 is set to 1 in the QoS data subtypes, which have QoS Control fields in
 * their MAC headers.
 *
 * Usage: check FrameT and FrameSubT individually. If the FrameT is MAC_FCTRL_DATA_T,
 * check the following bits of the FrameSubT
 */
#define MAC_CFACK_ST_BIT                CO_BIT(4)
#define MAC_CFPOLL_ST_BIT               CO_BIT(5)
#define MAC_NODATA_ST_BIT               CO_BIT(6)
#define MAC_QOS_ST_BIT                  CO_BIT(7)
#define MAC_FCTRL_DATACFACKPOLL_ST      (MAC_CFACK_ST_BIT | MAC_CFPOLL_ST_BIT)

/**
 * Frame Control various frame type/subtype definitions
 */
/// 802.11 type/subtype definition
#define MAC_FCTRL_TYPESUBTYPE_MASK      (MAC_FCTRL_TYPE_MASK | MAC_FCTRL_SUBT_MASK)
#define MAC_FCTRL_ASSOCREQ              (MAC_FCTRL_MGT_T     | MAC_FCTRL_ASSOCREQ_ST)
#define MAC_FCTRL_ASSOCRSP              (MAC_FCTRL_MGT_T     | MAC_FCTRL_ASSOCRSP_ST)
#define MAC_FCTRL_REASSOCREQ            (MAC_FCTRL_MGT_T     | MAC_FCTRL_REASSOCREQ_ST)
#define MAC_FCTRL_REASSOCRSP            (MAC_FCTRL_MGT_T     | MAC_FCTRL_REASSOCRSP_ST)
#define MAC_FCTRL_PROBEREQ              (MAC_FCTRL_MGT_T     | MAC_FCTRL_PROBEREQ_ST)
#define MAC_FCTRL_PROBERSP              (MAC_FCTRL_MGT_T     | MAC_FCTRL_PROBERSP_ST)
#define MAC_FCTRL_BEACON                (MAC_FCTRL_MGT_T     | MAC_FCTRL_BEACON_ST)
#define MAC_FCTRL_ATIM                  (MAC_FCTRL_MGT_T     | MAC_FCTRL_ATIM_ST)
#define MAC_FCTRL_DISASSOC              (MAC_FCTRL_MGT_T     | MAC_FCTRL_DISASSOC_ST)
#define MAC_FCTRL_AUTHENT               (MAC_FCTRL_MGT_T     | MAC_FCTRL_AUTHENT_ST)
#define MAC_FCTRL_DEAUTHENT             (MAC_FCTRL_MGT_T     | MAC_FCTRL_DEAUTHENT_ST)
#define MAC_FCTRL_ACTION                (MAC_FCTRL_MGT_T     | MAC_FCTRL_ACTION_ST)
#define MAC_FCTRL_ACTION_NO_ACK         (MAC_FCTRL_MGT_T     | MAC_FCTRL_ACTION_NO_ACK_ST)
#define MAC_FCTRL_BFM_REPORT_POLL       (MAC_FCTRL_CTRL_T    | MAC_FCTRL_BFM_REPORT_POLL_ST)
#define MAC_FCTRL_HE_TRIGGER            (MAC_FCTRL_CTRL_T    | MAC_FCTRL_HE_TRIGGER_ST)
#define MAC_FCTRL_VHT_NDPA              (MAC_FCTRL_CTRL_T    | MAC_FCTRL_VHT_NDPA_ST)
#define MAC_FCTRL_BA                    (MAC_FCTRL_CTRL_T    | MAC_FCTRL_BA_ST)
#define MAC_FCTRL_BAR                   (MAC_FCTRL_CTRL_T    | MAC_FCTRL_BAR_ST)
#define MAC_FCTRL_PSPOLL                (MAC_FCTRL_CTRL_T    | MAC_FCTRL_PSPOLL_ST)
#define MAC_FCTRL_RTS                   (MAC_FCTRL_CTRL_T    | MAC_FCTRL_RTS_ST)
#define MAC_FCTRL_CTS                   (MAC_FCTRL_CTRL_T    | MAC_FCTRL_CTS_ST)
#define MAC_FCTRL_ACK                   (MAC_FCTRL_CTRL_T    | MAC_FCTRL_ACK_ST)
#define MAC_FCTRL_CFEND                 (MAC_FCTRL_CTRL_T    | MAC_FCTRL_CFEND_ST)
#define MAC_FCTRL_CFEND_CFACK           (MAC_FCTRL_CFEND     | MAC_CFACK_ST_BIT)
#define MAC_FCTRL_DATA_CFACK            (MAC_FCTRL_DATA_T    | MAC_CFACK_ST_BIT)
#define MAC_FCTRL_DATA_CFPOLL           (MAC_FCTRL_DATA_T    | MAC_CFPOLL_ST_BIT)
#define MAC_FCTRL_DATA_CFACKPOLL        (MAC_FCTRL_DATA_T    | MAC_FCTRL_DATACFACKPOLL_ST)
#define MAC_FCTRL_NULL_FUNCTION         (MAC_FCTRL_DATA_T    | MAC_NODATA_ST_BIT)
#define MAC_FCTRL_NULL_CFACK            (MAC_FCTRL_NULL_FUNCTION  | MAC_CFACK_ST_BIT)
#define MAC_FCTRL_NULL_CFPOLL           (MAC_FCTRL_NULL_FUNCTION  | MAC_CFPOLL_ST_BIT)
#define MAC_FCTRL_NULL_CFACKPOLL        (MAC_FCTRL_NULL_FUNCTION  | MAC_FCTRL_DATACFACKPOLL_ST)
#define MAC_FCTRL_QOS_DATA              (MAC_FCTRL_DATA_T    | MAC_QOS_ST_BIT)
#define MAC_FCTRL_QOS_DATA_CFACK        (MAC_FCTRL_QOS_DATA  | MAC_CFACK_ST_BIT)
#define MAC_FCTRL_QOS_DATA_CFPOLL       (MAC_FCTRL_QOS_DATA  | MAC_CFPOLL_ST_BIT)
#define MAC_FCTRL_QOS_DATA_CFACKPOLL    (MAC_FCTRL_QOS_DATA  | MAC_FCTRL_DATACFACKPOLL_ST)
#define MAC_FCTRL_QOS_NULL              (MAC_FCTRL_QOS_DATA  | MAC_NODATA_ST_BIT)
#define MAC_FCTRL_QOS_NULL_CFACK        (MAC_FCTRL_QOS_DATA  | MAC_FCTRL_NULL_CFACK)
#define MAC_FCTRL_QOS_NULL_CFPOLL       (MAC_FCTRL_QOS_DATA  | MAC_FCTRL_NULL_CFPOLL)
#define MAC_FCTRL_QOS_NULL_CFACKPOLL    (MAC_FCTRL_QOS_DATA  | MAC_FCTRL_NULL_CFACKPOLL)

#define MAC_FCTRL_IS(fc, type) (((fc) & MAC_FCTRL_TYPESUBTYPE_MASK) == MAC_FCTRL_##type)
#define MAC_FCTRL_SUBTYPE(fc) (((fc) & MAC_FCTRL_SUBT_MASK) >> 4)

/*
 *  * Beacon Frame offset (Table 5 p46)
 *   */
#define MAC_BEACON_TIMESTAMP_OFT          MAC_SHORT_MAC_HDR_LEN   // Order 1
#define MAC_BEACON_INTERVAL_OFT          (MAC_SHORT_MAC_HDR_LEN + 8)   // Order 2
#define MAC_BEACON_CAPA_OFT              (MAC_SHORT_MAC_HDR_LEN + 10)   // Order 3
#define MAC_BEACON_VARIABLE_PART_OFT     (MAC_SHORT_MAC_HDR_LEN + 12)   // Order 4

#define MAC_ELTID_DS                      3

// DS PARAM SET
#define MAC_DS_ID_OFT                           0
#define MAC_DS_LEN_OFT                          1
#define MAC_DS_CHANNEL_OFT                      2
#define MAC_DS_PARAM_LEN                        3

#define MAC_ELTID_COUNTRY                 7
// Country
#define MAC_COUNTRY_LEN_OFT                     1
#define MAC_COUNTRY_STRING_OFT                  2
#define MAC_COUNTRY_STRING_LEN                  3

/* param_config */
#define CONFIG_ROLE_NULL        0
#define CONFIG_ROLE_AP          1
#define CONFIG_ROLE_STA         2
#define CONFIG_ROLE_COEXIST     3

#define DEFAULT_CHANNEL_AP      11

struct wifi_ssid {
    /// Actual length of the SSID.
    uint8_t length;
    /// Array containing the SSID name.
    uint8_t array[32];
};

#if CONFIG_MAC_BSSID
#define MAC_ADDR_LEN		6
/// MAC address structure.
struct mac_bssid
{
    /// Array of 16-bit words that make up the MAC address.
    uint16_t array[MAC_ADDR_LEN/2];
} __bk_packed;
#endif

struct wifi_bssid {
	uint8_t bssid[6];
};

typedef struct fast_connect_param {
	uint8_t bssid[6];
	uint8_t chann;
} fast_connect_param_t;

typedef struct general_param {
	uint8_t role;
	uint8_t dhcp_enable;
	uint32_t ip_addr;
	uint32_t ip_mask;
	uint32_t ip_gw;
} general_param_t;

typedef struct ap_param {
#if CONFIG_MAC_BSSID
	struct mac_bssid bssid;
#else
	struct wifi_bssid bssid;
#endif
	struct wifi_ssid ssid;

	uint8_t chann;
	uint8_t cipher_suite;
	uint8_t key[65];
	uint8_t key_len;
	bool hidden_ssid;
#if CONFIG_AP_VSIE
	uint8_t vsie[255];
	uint8_t vsie_len;
#endif
#if CONFIG_WIFI_AP_CUSTOM_RATES
	/* make last basic_rates be zero */
	int basic_rates[16];
	/* make last supported_rates be zero */
	int supported_rates[16];
	/* don't change mcs_set size */
	uint8_t mcs_set[16];
#endif
#if CONFIG_WIFI_AP_HW_MODE
	int hw_mode;
#endif
	u8 max_statype_num[4];
	u8 max_con;
} ap_param_t;

typedef struct sta_param {
	uint8_t own_mac[6];
	struct wifi_ssid ssid;
	uint8_t cipher_suite;
	uint8_t key[65];
	uint8_t key_len;
	uint8_t fast_connect_set;
	uint8_t ocv;
	fast_connect_param_t fast_connect;
#ifdef CONFIG_CONNECT_THROUGH_PSK_OR_SAE_PASSWORD
	bool psk_calculated;
	uint8_t psk[65];
	uint8_t psk_len;
#endif

	struct bk_vise *vsies[NUM_WIFI_VENDOR_ELEM_FRAMES];

#if CONFIG_COMPONENTS_WPA2_ENTERPRISE
	/* starts of WPA2-Enterprise/WPA3-Enterprise EAP-TLS configuration */
	char eap[16];					   /**< phase1 authType: TLS/TTLS/SIM */
	char identity[32];				   /**< user identity */
	char ca[32];					   /**< CA certificate filename */
	char client_cert[32];			   /**< client's Certification filename in PEM,DER format */
	char private_key[32];			   /**< client's private key filename in PEM,DER format */
	char private_key_passwd[32];	   /**< client's private key password */
	char phase1[32];	               /**< client's phase1 parameters */
#endif

	int auto_reconnect_count;          /**< auto reconnect max count, 0 for always reconnect */
	int auto_reconnect_timeout;        /**< auto reconnect timeout in secs, 0 for no timeout */
	bool disable_auto_reconnect_after_disconnect;  /**< disable auto reconnect if deauth/disassoc by AP when in connected state */
} sta_param_t;

/* video demo */
enum
{
    DAP_TIMER_POLL          = 0,
    DAP_WIFI_DISCONECTED,
    DAP_WIFI_CONECTED,
    DAP_APP_CONECTED,
    DAP_APP_DISCONECTED,
    DAP_EXIT,
    DAP_START_OTA,
};

typedef struct tvideo_ota_st
{
    const char *http_url;
    int    http_port;
    UINT32 http_timeout;
} TV_OTA_ST, *TV_OTA_PTR;

typedef struct tempory_message
{
    u32 dmsg;
    u32 data;
} DRONE_MSG_T;

/* wlan_defs_pub */
/**
 * @brief WiFi APIs Version 1 (Old WiFi API)
 * @addtogroup bk_api_wifi_v1 Old WiFi API group
 * @{
 */

/**
 * @brief WLAN encryption type
 * @defgroup bk_api_wifi_v1_enum enum
 * @ingroup bk_api_wifi_v1
 * @{
 */
enum {
	WLAN_ENC_OPEN,  /**< No encryption */
	WLAN_ENC_WEP,   /**< WiFi encryption algorithmn is WEP */
	WLAN_ENC_CCMP,  /**< WiFi encryption algorithmn is CCMP */
	WLAN_ENC_TKIP,  /**< WiFi encryption algorithmn is TKIP */
};

#define WiFi_Interface  wlanInterfaceTypedef /**< WiFi interface */

#define DHCP_DISABLE  (0)   /**< Disable DHCP service. */
#define DHCP_CLIENT   (1)   /**< Enable DHCP client which get IP address from DHCP server automatically */
#define DHCP_SERVER   (2)   /**< Enable DHCP server, needs assign a static address as local address. */

#if CONFIG_MINIMUM_SCAN_RESULTS
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
 * @parent_tsf: Time when the Beacon/Probe Response frame was received in terms
 * of TSF of the BSS specified by %tsf_bssid.
 * @tsf_bssid: The BSS that %parent_tsf TSF time refers to.
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
typedef  struct wpa_scan_res {
	unsigned int flags;
	u8 bssid[6];
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
	u64 parent_tsf;
	u8 tsf_bssid[6];
	size_t ie_len;
	size_t beacon_ie_len;
	int security;
	char ssid[32];
	char on_channel;
	unsigned char channel;
	/* Followed by ie_len + beacon_ie_len octets of IE data */
}SCAN_RST_ITEM_T, *SCAN_RST_ITEM_PTR;
#endif

typedef enum _wifi_dis_reason
{
	UNSUPPORT_ENCRYPT = 0, /**< Unsupport encrypt algorithmn */
	WRONG_PASSPHRASE,      /**< WiFi disconnect because configured wrong password */
	DEAUTHENTICATION,      /**< WiFi disconnect because of receiving deauth frame */
	DISASSOCIATION,        /**< WiFi disconnect because of receiving disassociation frame */
	AP_MISSING,            /**< WiFi disconnect because AP is missing, eg. AP is powered off etc */
	WLAN_DISCONNECT,       /**< WiFi is disconnected by application */
} WIFI_DISCONN_REASON;         /**< WiFi reason code */

typedef enum
{
	BK_SOFT_AP,  /**< Act as an access point, and other station can connect, 4 stations Max*/
	BK_STATION,   /**< Act as a station which can connect to an access point*/
	BK_P2P
} wlanInterfaceTypedef;

enum bk_wlan_sec_type_e
{
	BK_SECURITY_TYPE_NONE,        /**< Open system. */
	BK_SECURITY_TYPE_WEP,         /**< Wired Equivalent Privacy. WEP security. */
	BK_SECURITY_TYPE_WPA_TKIP,    /**< WPA /w TKIP */
	BK_SECURITY_TYPE_WPA_AES,     /**< WPA /w AES */
	BK_SECURITY_TYPE_WPA_MIXED,   /**< WPA /w AES or TKIP */
	BK_SECURITY_TYPE_WPA2_TKIP,   /**< WPA2 /w TKIP */
	BK_SECURITY_TYPE_WPA2_AES,    /**< WPA2 /w AES */
	BK_SECURITY_TYPE_WPA2_MIXED,  /**< WPA2 /w AES or TKIP */
	BK_SECURITY_TYPE_WPA3_SAE,    /**< WPA3 SAE */
	BK_SECURITY_TYPE_WPA3_WPA2_MIXED, /**< WPA3 SAE or WPA2 AES */
	BK_SECURITY_TYPE_EAP,         /**< EAP */
	BK_SECURITY_TYPE_OWE,         /**< OWE */
	BK_SECURITY_TYPE_AUTO,        /**< It is used when calling @ref bk_wlan_start_sta_adv, _BK_ read security type from scan result. */
};

enum
{
	WLAN_RX_BEACON,    /**< receive beacon packet */
	WLAN_RX_PROBE_REQ, /**< receive probe request packet */
	WLAN_RX_PROBE_RES, /**< receive probe response packet */
	WLAN_RX_ACTION,    /**< receive action packet */
	WLAN_RX_MANAGEMENT,/**< receive ALL management packet */
	WLAN_RX_DATA,      /**< receive ALL data packet */
	WLAN_RX_MCAST_DATA,/**< receive ALL multicast and broadcast packet */
	WLAN_RX_ALL,       /**< receive ALL 802.11 packet */
};

typedef uint8_t wlan_sec_type_t;  /**< WiFi security type */

#define MONITOR_FILTER_MUL_BRD_CAST     (1U << 0) /**< Multicast frame filter bit */
#define MONITOR_FILTER_DUPL_FRM         (1U << 1) /**< Duplicated frame filter bit */

/**
 * @brief WLAN encryption type
 * @defgroup bk_api_wifi_v1_typedef struct
 * @ingroup bk_api_wifi_v1 struct
 * @{
 */

typedef struct _wifi_link_info
{
	uint8_t bssid[6];           /**< BSSID of link */
	int16_t rssi;               /**< RSSI of link */
	uint32_t data_rate;         /**< Data rate TODO */
	WIFI_DISCONN_REASON reason; /**< Disconnect reason TODO */
} WIFI_LINK_INFO_T;

/**
 *  @brief  wlan local IP information structure definition.
 */
typedef struct
{
	uint8_t dhcp;       /**< DHCP mode: @ref DHCP_DISABLE, @ref DHCP_CLIENT, @ref DHCP_SERVER.*/
	char    ip[16];     /**< Local IP address on the target wlan interface: @ref wlanInterfaceTypedef.*/
	char    gate[16];   /**< Router IP address on the target wlan interface: @ref wlanInterfaceTypedef.*/
	char    mask[16];   /**< Netmask on the target wlan interface: @ref wlanInterfaceTypedef.*/
	char    dns[16];    /**< DNS server IP address.*/
	char    mac[16];    /**< MAC address, example: "C89346112233".*/
	char    broadcastip[16]; /**< Broadcast IP address */
} IPStatusTypedef;

/**
 *  @brief  Scan result using normal scan.
 */
typedef  struct  _ScanResult
{
	char ApNum;       /**< The number of access points found in scanning. */
	struct _ApList
	{
		char ssid[33];  /**< The SSID of an access point. */
		char ApPower;   /**< Signal strength, min:0, max:100. */
	} *ApList;
} ScanResult;

/**
 *      @brief  Scan result using advanced scan.
 */
typedef  struct  _ScanResult_adv
{
	char ApNum;       /**< The number of access points found in scanning.*/
	struct ApListStruct
	{
		char ssid[33];    /**< The SSID of an access point.*/
		char ApPower;     /**< Signal strength, min:0, max:100*/
		uint8_t bssid[6]; /**< The BSSID of an access point.*/
		char channel;     /**< The RF frequency, 1-13*/
		wlan_sec_type_t security;       /**< Security type, see wlan_sec_type_t */
	} *ApList;  /**< AP list found by scan */
} ScanResult_adv;
#if !CONFIG_MINIMUM_SCAN_RESULTS
/**
 * @brief  The info of the APs found by scan.
 *
 * The WiFi driver scans the APs by parsing the received beacon or probe response of
 * the AP and all following info are came from the beacon or probe response.
 */
typedef struct sta_scan_res
{
	UINT8 bssid[6];    /**< The BSSID of AP */
	char ssid[32];     /**< The SSID of AP */
	char on_channel;   /**< Indicate whether the channel in DS IE is valid 2.4G channel */
	unsigned char channel;    /**< The channel of AP */
	UINT16 beacon_int; /**< Beacon interval of AP */
	UINT16 caps;       /**< Capability IE of AP */
	int level;         /**< RSSI of the received frame */
	int security;      /**< Encryption algorithm of AP */
	UINT8 tsf[8];      /**< TSF value in beacon or probe response */
	UINT32 ie_len;     /**< IE length of beacon or probe response */
	/* Followed by ie_len of IE data */
} SCAN_RST_ITEM_T, *SCAN_RST_ITEM_PTR;
#endif
/**
 *  @brief  Input network paras, used in bk_wlan_start function.
 */
typedef struct _network_InitTypeDef_st
{
	char wifi_mode;               /**< WiFi mode, BK_SOFT_AP or BK_STATION */
	char wifi_ssid[33];           /**< For station, it's SSID of the AP to be connected, for AP, it's the SSID of our AP */
	char wifi_key[65];            /**< Security key of the wlan needs to be connected, ignored in an open system.*/
	char local_ip_addr[16];       /**< Static IP configuration, Local IP address. */
	char net_mask[16];            /**< Static IP configuration, Netmask. */
	char gateway_ip_addr[16];     /**< Static IP configuration, Router IP address. */
	char dns_server_ip_addr[16];  /**< Static IP configuration, DNS server IP address. */
	char dhcp_mode;               /**< DHCP mode, @ref DHCP_DISABLE, @ref DHCP_CLIENT and @ref DHCP_SERVER. */
	char wifi_bssid[6];           /**< For station, it's the BSSID of AP to be connected, for AP, it's BSSID of our AP */
	char reserved[26];            /**< Reserved */
	int  wifi_retry_interval;     /**< Retry interval if an error is occured when connecting an access point,
						time unit is millisecond. */
	bool hidden_ssid;			  /**< hidden ssid, only for softap */
	bool ocv;                     /**< operating channel validation */
#if CONFIG_WIFI_STA_VSIE || CONFIG_WIFI_AP_VSIE
	uint8_t vsie[255];			  /**< vendor specific IE for probe req/assoc req. */
	uint8_t vsie_len;			  /**< vendor specific IE len. */
#endif
#if CONFIG_WIFI_AP_CUSTOM_RATES
	/* mark last basic_rates be zero */
	int basic_rates[16];
	/* mark last supported_rates be zero */
	int supported_rates[16];
	/* mark last mcs_set be zero, don't change mcs_set length */
	uint8_t mcs_set[16];
#endif
#if CONFIG_WIFI_AP_HW_MODE
	/* bk_wlan_hw_mode */
	int hw_mode;
#endif
#if CONFIG_QUICK_TRACK
	int key_mgmt;
	int pairwise_cipher;
	int group_cipher;
	int proto;		// WPA, RSN
	int ieee80211w;
#endif
} network_InitTypeDef_st;

/**
 *  @brief  Advanced precise wlan parameters, used in @ref network_InitTypeDef_adv_st.
 */
typedef struct
{
	char    ssid[32+1];    /**< SSID of the wlan that needs to be connected. Example: "SSID String". */
	char    bssid[6];    /**< BSSID of the wlan needs to be connected. Example: {0xC8 0x93 0x46 0x11 0x22 0x33}. */
	uint8_t channel;     /**< Wlan's RF frequency, channel 0-13. 1-13 means a fixed channel
                            that can speed up a connection procedure, 0 is not a fixed input
                            means all channels are possible*/
	wlan_sec_type_t security; /**< Security type of AP */
} apinfo_adv_t;

/**
 *  @brief  Input network precise paras in bk_wlan_start_sta_adv function.
 */
typedef struct _network_InitTypeDef_adv_st
{
	apinfo_adv_t ap_info;         /**< @ref apinfo_adv_t. */
	char  key[64];                /**< Security key or PMK of the wlan. */
	int   key_len;                /**< The length of the key. */
	char  local_ip_addr[16];      /**< Static IP configuration, Local IP address. */
	char  net_mask[16];           /**< Static IP configuration, Netmask. */
	char  gateway_ip_addr[16];    /**< Static IP configuration, Router IP address. */
	char  dns_server_ip_addr[16]; /**< Static IP configuration, DNS server IP address. */
	char  dhcp_mode;              /**< DHCP mode, @ref DHCP_DISABLE, @ref DHCP_CLIENT and @ref DHCP_SERVER. */
	char  reserved[32];           /**< reserved */
	int   wifi_retry_interval;    /**< Retry interval if an error is occured when connecting an access point,
					time unit is millisecond. */
} network_InitTypeDef_adv_st;

typedef struct _network_InitTypeDef_ap_st
{
	char wifi_ssid[32];           /**< SSID of AP */
	char wifi_key[64];            /**< key of AP */
	uint8_t channel;              /**< Channel of AP */
	wlan_sec_type_t security;     /**< Security type of AP */
	uint8_t ssid_hidden;          /**< Whether the AP is hidden */
	uint8_t max_con;              /**< Max number of stations allowed to connect in, TODO? */
	char local_ip_addr[16];       /**< IP of AP */
	char net_mask[16];            /**< Network mask of AP */
	char gateway_ip_addr[16];     /**< Gateway IP of AP */
	char dns_server_ip_addr[16];  /**< DNS server IP of AP */
	char dhcp_mode;               /**< DHCP mode */
	char reserved[32];            /**< Reserved */
	int  wifi_retry_interval;     /**< For STA only */
} network_InitTypeDef_ap_st;

/**
 *  @brief  Current link status in station mode.
 */
typedef struct _linkStatus_t
{
	int conn_state;         /**< The link to wlan is established or not, 0: disconnected, 1: connected. */
	int wifi_strength;      /**< Signal strength of the current connected AP */
	uint8_t  ssid[32];      /**< SSID of the current connected wlan */
	uint8_t  bssid[6];      /**< BSSID of the current connected wlan */
	int      channel;       /**< Channel of the current connected wlan */
	wlan_sec_type_t security; /**< Security type of WiFi link */
} LinkStatusTypeDef;

typedef struct
{
        int8_t rssi; /**< RSSI of WiFi link, generally it's the RSSI of the received WiFi frame */
} wifi_link_info_t;

/*same with RL_BSSID_INFO_T{}*/
struct wlan_fast_connect_info
{
	uint8_t ssid[33]; /**< SSID of AP */
	uint8_t bssid[6]; /**< BSSID of AP */
	uint8_t security; /**< Security of AP */
	uint8_t channel;  /**< Channel of AP */
	uint8_t psk[65];  /**< PSK of AP */
	uint8_t pwd[65];  /**< password of AP */
	uint8_t ip_addr[4];
	uint8_t netmask[4];
	uint8_t gw[4];
	uint8_t dns1[4];

#if CONFIG_WLAN_FAST_CONNECT_WITHOUT_SCAN
	uint16_t freq;
	u16 beacon_int;
	uint16_t caps;
	int level;
	// u64 tsf;
	uint16_t ie_len;
	uint8_t ies[1024];	/* FIXME: use dynamic len */
#endif
	// #if CONFIG_WIFI_MFP_CONNECT_DEAUTH
	uint8_t pmf;
	uint8_t tk[16];
	// #endif
#if CONFIG_WLAN_FAST_CONNECT_WPA3
	uint8_t pmk_len;
	uint8_t pmk[64]; // for WPA2 Personal, pmk = psk
	uint8_t pmkid[16];
	int akmp;
#endif
	/* aes attention: sizeof(RL_BSSID_INFO_T) = 16 * n */
	uint8_t padding[0] __attribute__ ((aligned (16)));
};

typedef struct vif_addcfg_st {
	char *ssid;   /**< SSID of VIF */
	char *key;    /**< Key of VIF */
	char *name;   /**< Name of VIF, can be "sta" or "softap" */
	u8 wlan_role; /**< Role of VIF, can be BK_SOFT_AP or BK_STATION */
	u8 adv;       /**< Only for station role, indicate whether connect the AP with advance method */
} VIF_ADDCFG_ST, *VIF_ADDCFG_PTR;

/* sa_station */
//TODO Move this file to better place
#define SASTA_DEBUG

#ifdef SASTA_DEBUG
#define SASTA_PRT                 os_printf
#define SASTA_WPRT                warning_prf
#else
#define SASTA_PRT                 os_null_printf
#define SASTA_WPRT                os_null_printf
#endif

#define SCANU_IND_PAYLOAD_LEN                   512

typedef struct rxu_mgt_ind SCAN_IND_T, *SCAN_IND_PTR;

typedef struct probe_rsp {
    #ifdef CONFIG_MINIMUM_SCAN_RESULTS
    UINT64 timestamp;
    #else
    UINT8 timestamp[8];
    #endif
    UINT16 beacon_int;
    UINT16 capab_info;

    /* followed by some of SSID, Supported rates,
     * FH Params, DS Params, CF Params, IBSS Params */
    UINT8 variable[1];
} __attribute__ ((packed)) PROBE_RSP_T;

typedef struct ieee802_11_probe_rsp {
    UINT16 frame_control;
    UINT16 duration;
    UINT8 da[6];
    UINT8 sa[6];
    UINT8 bssid[6];
    UINT16 seq_ctrl;
    struct probe_rsp rsp;
} __attribute__ ((packed)) IEEE802_11_PROBE_RSP_T, *IEEE802_11_PROBE_RSP_PTR;

typedef struct {
	uint32_t sta_start_tick;		/**< sta start connect tick */
	uint32_t sta_assoc_tick;		/**< sta complete assoc tick */
	uint32_t sta_eapol_tick;		/**< sta complete eapol tick */
	uint32_t sta_ip_tick;		/**< sta got ip tick */
} wifi_connect_tick_t;

void sa_station_init(void);
void sa_station_uninit(void);
void sa_station_set_reconnect_timer(void);
void sa_reconnect_init(void);

#ifdef __cplusplus
}
#endif

