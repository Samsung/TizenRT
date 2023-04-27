#ifndef __RTW_WIFI_STRUCT_H_
#define __RTW_WIFI_STRUCT_H_

typedef unsigned char	NDIS_802_11_MAC_ADDRESS[6];
typedef long			NDIS_802_11_RSSI;			// in dBm

typedef enum _NDIS_802_11_NETWORK_INFRASTRUCTURE {
	Ndis802_11Infrastructure,
	Ndis802_11AutoUnknown,
	Ndis802_11APMode,
} NDIS_802_11_NETWORK_INFRASTRUCTURE, *PNDIS_802_11_NETWORK_INFRASTRUCTURE;

typedef enum _NDIS_802_11_NETWORK_TYPE {
	Ndis802_11FH,
	Ndis802_11DS,
	Ndis802_11OFDM5,
	Ndis802_11OFDM24,
	Ndis802_11NetworkTypeMax	// not a real type, defined as an upper bound
} NDIS_802_11_NETWORK_TYPE, *PNDIS_802_11_NETWORK_TYPE;

typedef struct _NDIS_802_11_CONFIGURATION_FH {
	u32 		  Length;			  // Length of structure
	u32 		  HopPattern;		  // As defined by 802.11, MSB set
	u32 		  HopSet;			  // to one if non-802.11
	u32 		  DwellTime;		  // units are Kusec
} NDIS_802_11_CONFIGURATION_FH;

typedef struct _NDIS_802_11_CONFIGURATION {
	u32 		  BeaconPeriod; 	  // units are Kusec
	u32 		  ATIMWindow;		  // units are Kusec
	u32 		  DSConfig; 		  // Frequency, units are kHz
	NDIS_802_11_CONFIGURATION_FH	FHConfig;
} NDIS_802_11_CONFIGURATION;

struct _WLAN_PHY_INFO {
	u8	SignalStrength; 	//(in percentage)
	u8	SignalQuality;		//(in percentage)
	u8	Optimum_antenna;	//for Antenna diversity
	u8		Reserved_0;
#ifdef CONFIG_IEEE80211K
	u8	is_cck_rate;	/* 1:cck_rate */
	s8	rx_snr[4];
	u32 free_cnt;	/* freerun counter */
#endif
} _PACKED_;
typedef struct _WLAN_PHY_INFO WLAN_PHY_INFO;

typedef struct _WLAN_BCN_INFO {
	/* these infor get from rtw_get_encrypt_info when
	 *	 * translate scan to UI */
	u8	encryp_protocol;	//ENCRYP_PROTOCOL_E: OPEN/WEP/WPA/WPA2/WAPI
	int group_cipher;		//WPA/WPA2 group cipher
	int pairwise_cipher;	//WPA/WPA2/WEP pairwise cipher
	int is_8021x;
	u8 is_1M_beacon;		// check if the beacon uses 1M data rate

	/* bwmode 20/40 and ch_offset UP/LOW */
	unsigned short	ht_cap_info;
	unsigned char	ht_info_infos_0;
} WLAN_BCN_INFO;

#ifdef CONFIG_MBSSID_AX
#define MAX_VIRTUAL_AP_NUM	8
typedef struct _mbssid_virtual_ap_info {
	u8					bssid_idx;	// from bssid-index ie
	NDIS_802_11_SSID	Ssid;		//from ssid ie
} mbssid_virtual_ap_info_t;

typedef struct _mbssid_info {
	u8 mbssid_enable;	// AP support mbssid
	u8 mbssid_complete; // MBSSID element carriy complete non-transmitted BSSID profile
	u8 mbssid_ema;		//enhanced MBSSID advertisement
	u8 bssid_max;		//max_bssid_indicator
	u8 bssid_ref[6];		// transmitted-BSSID
	u8 virtual_ap_num;	// num of non-transmitted BSSID
	u8 bssid_idx_to_join;	//bssid index of non-transmitted BSSID to join
	u8 is_virtual_ap;		// 1: ap to join is non-transmitted
	mbssid_virtual_ap_info_t virtual_ap_info[MAX_VIRTUAL_AP_NUM];	// store non-transmitted BSSID profile
	u8 *mbssid_ie;		//start with non-transmitted BSSID profile
	u16 mbssid_ie_len;
} mbssid_info_t;
#endif

typedef struct _WLAN_BSSID_EX {
	u32 				Length;
	u8					MacAddress[6];
#ifdef CONFIG_MBSSID_AX
	mbssid_info_t mbssid_info;
#endif

	u8					Reserved[2];		//[0]: IS beacon frame
	NDIS_802_11_SSID			Ssid;
	u32 				Privacy;
	NDIS_802_11_RSSI			Rssi;			//(in dBM,raw data ,get from PHY)
	NDIS_802_11_NETWORK_TYPE		NetworkTypeInUse;
	NDIS_802_11_CONFIGURATION		Configuration;
	NDIS_802_11_RATES_EX		SupportedRates;
	WLAN_PHY_INFO				PhyInfo;
	u32 				IELength;
	u8					IEs[MAX_IE_SZ]; 	//(timestamp, beacon interval, and capability information)
} WLAN_BSSID_EX;

struct	wlan_network {
	_list	list;
	int network_type;	//refer to ieee80211.h for WIRELESS_11A/B/G
	int fixed;			// set to fixed when not to be removed as site-surveying
	unsigned long	last_scanned; //timestamp for the network
	int aid;			//will only be valid when a BSS is joinned.
	int join_res;
	WLAN_BSSID_EX	network; //must be the last item
	WLAN_BCN_INFO	BcnInfo;
};

struct net_device_stats {

	unsigned long	rx_packets;				/*!< total packets received       */
	unsigned long	tx_packets;				/*!<total packets transmitted    */
	unsigned long	rx_dropped;				/*!< no space in buffers    */
	unsigned long	tx_dropped;				/*!< no space available  */
	unsigned long	rx_bytes;				/*!< total bytes received         */
	unsigned long	tx_bytes;				/*!< total bytes transmitted      */
	unsigned long	rx_overflow;			/*!< rx fifo overflow count       */

	u64		last_tx_bytes;
	u64		last_tx_pkts;
	u64		last_rx_bytes;
	u16		rx_reorder_drop_cnt;
	u16		rx_reorder_timeout_cnt;
	_timer		signal_stat_timer;
	u32		is_any_non_be_pkts;
	struct signal_stat signal_qual_data;
	struct signal_stat signal_strength_data;
	struct signal_stat signal_snr_data;

	/* For display the phy informatiom */
	int RxSNRdB[2];
	s8 rssi;
	s8 snr;
	u8 signal_strength;
	u8 signal_qual;
#if defined(CONFIG_RTL8735B)
	u8 signal_strength_backup;
#endif // defined(CONFIG_RTL8735B)
};

/* ------------------------ iwpriv related ------------------------ */
#undef IFNAMSIZ
#define IFNAMSIZ	16

/*
 *	Generic format for most parameters that fit in an int
 */
struct	rtw_param {
	__s32		value;		/* The value of the parameter itself */
	__u8		fixed;		/* Hardware should not use auto select */
	__u8		disabled;	/* Disable the feature */
	__u16		flags;		/* Various specifc flags (if any) */
};

struct	rtw_point {
	void		*pointer;	/* Pointer to the data  (in user space) */
	__u16		length;		/* number of fields or size in bytes */
	__u16		flags;		/* Optional params */
};

struct	rtw_freq {
	__s32		m;		/* Mantissa */
	__s16		e;		/* Exponent */
	__u8		i;		/* List index (when in range struct) */
	__u8		flags;		/* Flags (fixed/auto) */
};


union rtwreq_data {
	/* Config - generic */
	char			name[IFNAMSIZ];	/* Name : used to verify the presence of  wireless extensions. * Name of the protocol/provider... */
	struct rtw_point	essid;		/* Extended network name */
	struct rtw_param	nwid;		/* network id (or domain - the cell) */
	struct rtw_freq		freq;		/* frequency or channel : * 0-1000 = channel * > 1000 = frequency in Hz */
	struct rtw_param	sens;		/* signal level threshold */
	struct rtw_param	snr;		/* signal noise ratio */
	struct rtw_param	bitrate;	/* default bit rate */
	struct rtw_param	txpower;	/* default transmit power */
	struct rtw_param	rts;		/* RTS threshold threshold */
	struct rtw_param	frag;		/* Fragmentation threshold */
	__u32			mode;		/* Operation mode */
	struct rtw_param	retry;		/* Retry limits & lifetime */
	struct rtw_point	encoding;	/* Encoding stuff : tokens */
	struct rtw_param	power;		/* PM duration/timeout */
	struct sockaddr_t	ap_addr;	/* Access point address */
	struct sockaddr_t	addr;		/* Destination address (hw/mac) */
	struct rtw_param	param;		/* Other small parameters */
	struct rtw_point	data;		/* Other large parameters */
	struct rtw_point	passphrase;	/* Extended network name */
};

struct rtwreq {
	char		ifr_name[IFNAMSIZ];	/* if name, e.g. "eth0" */
	union		rtwreq_data	u;
};

struct rtw_priv_args {
	__u32		cmd;		/* Number of the ioctl to issue */
	char		name[IFNAMSIZ];	/* Name of the extension */
};

#endif
