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
#ifndef __WLAN_BSSDEF_H__
#define __WLAN_BSSDEF_H__


#define MAX_IE_SZ	768 //384//
#define NDIS_802_11_LENGTH_SSID         32
#define NDIS_802_11_LENGTH_RATES        12
#define NDIS_802_11_LENGTH_RATES_EX     16

typedef unsigned char   NDIS_802_11_MAC_ADDRESS[6];
typedef long    		NDIS_802_11_RSSI;           // in dBm
typedef unsigned char   NDIS_802_11_RATES[NDIS_802_11_LENGTH_RATES];        // Set of 8 data rates
typedef unsigned char   NDIS_802_11_RATES_EX[NDIS_802_11_LENGTH_RATES_EX];  // Set of 16 data rates


typedef  u32  NDIS_802_11_KEY_INDEX;
typedef unsigned long long NDIS_802_11_KEY_RSC;


typedef struct _NDIS_802_11_SSID {
	u32  SsidLength;
	u8  Ssid[NDIS_802_11_LENGTH_SSID + 4];
}
#ifdef __CC_ARM
__attribute__((packed))
#endif
NDIS_802_11_SSID, *PNDIS_802_11_SSID;

typedef enum _NDIS_802_11_NETWORK_TYPE {
	Ndis802_11FH,
	Ndis802_11DS,
	Ndis802_11OFDM5,
	Ndis802_11OFDM24,
	Ndis802_11NetworkTypeMax    // not a real type, defined as an upper bound
} NDIS_802_11_NETWORK_TYPE, *PNDIS_802_11_NETWORK_TYPE;

typedef struct _NDIS_802_11_CONFIGURATION_FH {
	u32           Length;             // Length of structure
	u32           HopPattern;         // As defined by 802.11, MSB set
	u32           HopSet;             // to one if non-802.11
	u32           DwellTime;          // units are Kusec
}
#ifdef __CC_ARM
__attribute__((packed))
#endif
NDIS_802_11_CONFIGURATION_FH, *PNDIS_802_11_CONFIGURATION_FH;


/*
	FW will only save the channel number in DSConfig.
	ODI Handler will convert the channel number to freq. number.
*/
typedef struct _NDIS_802_11_CONFIGURATION {
	u32           Length;             // Length of structure
	u32           BeaconPeriod;       // units are Kusec
	u32           ATIMWindow;         // units are Kusec
	u32           DSConfig;           // Frequency, units are kHz
	NDIS_802_11_CONFIGURATION_FH    FHConfig;
}
#ifdef __CC_ARM
__attribute__((packed))
#endif
NDIS_802_11_CONFIGURATION, *PNDIS_802_11_CONFIGURATION;



typedef enum _NDIS_802_11_NETWORK_INFRASTRUCTURE {
	Ndis802_11Infrastructure,
	Ndis802_11AutoUnknown,
	Ndis802_11InfrastructureMax,     // Not a real value, defined as upper bound
	Ndis802_11APMode,
	Ndis802_11Monitor
} NDIS_802_11_NETWORK_INFRASTRUCTURE, *PNDIS_802_11_NETWORK_INFRASTRUCTURE;





typedef struct _NDIS_802_11_FIXED_IEs {
	u8  Timestamp[8];
	u16  BeaconInterval;
	u16  Capabilities;
} NDIS_802_11_FIXED_IEs, *PNDIS_802_11_FIXED_IEs;



typedef struct _NDIS_802_11_VARIABLE_IEs {
	u8  ElementID;
	u8  Length;
	u8  data[1];
} NDIS_802_11_VARIABLE_IEs, *PNDIS_802_11_VARIABLE_IEs;



/*



Length is the 4 bytes multiples of the sume of
	sizeof (NDIS_802_11_MAC_ADDRESS) + 2 + sizeof (NDIS_802_11_SSID) + sizeof (u32)
+   sizeof (NDIS_802_11_RSSI) + sizeof (NDIS_802_11_NETWORK_TYPE) + sizeof (NDIS_802_11_CONFIGURATION)
+   sizeof (NDIS_802_11_RATES_EX) + IELength

Except the IELength, all other fields are fixed length. Therefore, we can define a marco to present the
partial sum.

*/

typedef enum _NDIS_802_11_AUTHENTICATION_MODE {
	Ndis802_11AuthModeOpen,
	Ndis802_11AuthModeShared,
	Ndis802_11AuthModeAutoSwitch,
	Ndis802_11AuthModeWPA,
	Ndis802_11AuthModeWPAPSK,
	Ndis802_11AuthModeWPANone,
	Ndis802_11AuthModeWAPI,
	Ndis802_11AuthModeWPA2,
	Ndis802_11AuthModeWPA2PSK,
	Ndis802_11AuthModeWPA3,
	Ndis802_11AuthModeWPA3PSK,
	Ndis802_11AuthModeMax               // Not a real mode, defined as upper bound
} NDIS_802_11_AUTHENTICATION_MODE, *PNDIS_802_11_AUTHENTICATION_MODE;

typedef enum _NDIS_802_11_WEP_STATUS {
	Ndis802_11WEPEnabled,
	Ndis802_11Encryption1Enabled = Ndis802_11WEPEnabled,
	Ndis802_11WEPDisabled,
	Ndis802_11EncryptionDisabled = Ndis802_11WEPDisabled,
	Ndis802_11WEPKeyAbsent,
	Ndis802_11Encryption1KeyAbsent = Ndis802_11WEPKeyAbsent,
	Ndis802_11WEPNotSupported,
	Ndis802_11EncryptionNotSupported = Ndis802_11WEPNotSupported,
	Ndis802_11Encryption2Enabled,
	Ndis802_11Encryption2KeyAbsent,
	Ndis802_11Encryption3Enabled,
	Ndis802_11Encryption3KeyAbsent,
	Ndis802_11_EncrypteionWAPI
} NDIS_802_11_WEP_STATUS, *PNDIS_802_11_WEP_STATUS,
NDIS_802_11_ENCRYPTION_STATUS, *PNDIS_802_11_ENCRYPTION_STATUS;


#define NDIS_802_11_AI_REQFI_CAPABILITIES      1
#define NDIS_802_11_AI_REQFI_LISTENINTERVAL    2
#define NDIS_802_11_AI_REQFI_CURRENTAPADDRESS  4

#define NDIS_802_11_AI_RESFI_CAPABILITIES      1
#define NDIS_802_11_AI_RESFI_STATUSCODE        2
#define NDIS_802_11_AI_RESFI_ASSOCIATIONID     4

typedef struct _NDIS_802_11_AI_REQFI {
	u16 Capabilities;
	u16 ListenInterval;
	NDIS_802_11_MAC_ADDRESS  CurrentAPAddress;
} NDIS_802_11_AI_REQFI, *PNDIS_802_11_AI_REQFI;

typedef struct _NDIS_802_11_AI_RESFI {
	u16 Capabilities;
	u16 StatusCode;
	u16 AssociationId;
} NDIS_802_11_AI_RESFI, *PNDIS_802_11_AI_RESFI;

typedef struct _NDIS_802_11_ASSOCIATION_INFORMATION {
	u32                   Length;
	u16                  AvailableRequestFixedIEs;
	NDIS_802_11_AI_REQFI    RequestFixedIEs;
	u32                   RequestIELength;
	u32                   OffsetRequestIEs;
	u16                  AvailableResponseFixedIEs;
	NDIS_802_11_AI_RESFI    ResponseFixedIEs;
	u32                   ResponseIELength;
	u32                   OffsetResponseIEs;
} NDIS_802_11_ASSOCIATION_INFORMATION, *PNDIS_802_11_ASSOCIATION_INFORMATION;

typedef enum _NDIS_802_11_RELOAD_DEFAULTS {
	Ndis802_11ReloadWEPKeys
} NDIS_802_11_RELOAD_DEFAULTS, *PNDIS_802_11_RELOAD_DEFAULTS;


// Key mapping keys require a BSSID
typedef struct _NDIS_802_11_KEY {
	u32           Length;             // Length of this structure
	u32           KeyIndex;
	u32           KeyLength;          // length of key in bytes
	NDIS_802_11_MAC_ADDRESS BSSID;
	NDIS_802_11_KEY_RSC KeyRSC;
	u8           KeyMaterial[32];     // variable length depending on above field
} NDIS_802_11_KEY, *PNDIS_802_11_KEY;

typedef struct _NDIS_802_11_REMOVE_KEY {
	u32                   Length;        // Length of this structure
	u32                   KeyIndex;
	NDIS_802_11_MAC_ADDRESS BSSID;
} NDIS_802_11_REMOVE_KEY, *PNDIS_802_11_REMOVE_KEY;

typedef struct _NDIS_802_11_WEP {
	u32     Length;        // Length of this structure
	u32     KeyIndex;      // 0 is the per-client key, 1-N are the global keys
	u32     KeyLength;     // length of key in bytes
	u8     KeyMaterial[16];// variable length depending on above field
} NDIS_802_11_WEP, *PNDIS_802_11_WEP;

typedef struct _NDIS_802_11_AUTHENTICATION_REQUEST {
	u32 Length;            // Length of structure
	NDIS_802_11_MAC_ADDRESS Bssid;
	u32 Flags;
} NDIS_802_11_AUTHENTICATION_REQUEST, *PNDIS_802_11_AUTHENTICATION_REQUEST;

typedef enum _NDIS_802_11_STATUS_TYPE {
	Ndis802_11StatusType_Authentication,
	Ndis802_11StatusType_MediaStreamMode,
	Ndis802_11StatusType_PMKID_CandidateList,
	Ndis802_11StatusTypeMax    // not a real type, defined as an upper bound
} NDIS_802_11_STATUS_TYPE, *PNDIS_802_11_STATUS_TYPE;

typedef struct _NDIS_802_11_STATUS_INDICATION {
	NDIS_802_11_STATUS_TYPE StatusType;
} NDIS_802_11_STATUS_INDICATION, *PNDIS_802_11_STATUS_INDICATION;

// mask for authentication/integrity fields
#define NDIS_802_11_AUTH_REQUEST_AUTH_FIELDS        0x0f
#define NDIS_802_11_AUTH_REQUEST_REAUTH			0x01
#define NDIS_802_11_AUTH_REQUEST_KEYUPDATE		0x02
#define NDIS_802_11_AUTH_REQUEST_PAIRWISE_ERROR		0x06
#define NDIS_802_11_AUTH_REQUEST_GROUP_ERROR		0x0E

// MIC check time, 60 seconds.
#define MIC_CHECK_TIME	60000000

typedef struct _NDIS_802_11_AUTHENTICATION_EVENT {
	NDIS_802_11_STATUS_INDICATION       Status;
	NDIS_802_11_AUTHENTICATION_REQUEST  Request[1];
} NDIS_802_11_AUTHENTICATION_EVENT, *PNDIS_802_11_AUTHENTICATION_EVENT;

typedef struct _NDIS_802_11_TEST {
	u32 Length;
	u32 Type;
	union {
		NDIS_802_11_AUTHENTICATION_EVENT AuthenticationEvent;
		NDIS_802_11_RSSI RssiTrigger;
	} tt;
} NDIS_802_11_TEST, *PNDIS_802_11_TEST;

typedef struct _WLAN_PHY_INFO {
	u8	SignalStrength;		//(in percentage)
	u8	SignalQuality;		//(in percentage)
	u8	Optimum_antenna;  	//for Antenna diversity
	u8  	Reserved_0;
#ifdef CONFIG_IEEE80211K
	u8	is_cck_rate;	/* 1:cck_rate */
	s8	rx_snr[4];
	u32	free_cnt; 	/* freerun counter */
#endif
}
#ifdef __CC_ARM
__attribute__((packed))
#endif
WLAN_PHY_INFO, *PWLAN_PHY_INFO;

typedef struct _WLAN_BCN_INFO {
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
} WLAN_BCN_INFO, *PWLAN_BCN_INFO;

#ifdef CONFIG_MBSSID_AX
#define MAX_VIRTUAL_AP_NUM	8
typedef struct _mbssid_virtual_ap_info {
	u8					bssid_idx;	// from bssid-index ie
	NDIS_802_11_SSID	Ssid;		//from ssid ie
} mbssid_virtual_ap_info_t;

typedef struct _mbssid_info {
	u8 mbssid_enable;	// AP support mbssid
	u8 mbssid_complete;	// MBSSID element carriy complete non-transmitted BSSID profile
	u8 mbssid_ema;		//enhanced MBSSID advertisement
	u8 bssid_max;		//max_bssid_indicator
	u8 bssid_ref[6];		// transmitted-BSSID
	u8 virtual_ap_num;	// num of non-transmitted BSSID
	u8 bssid_idx_to_join;	//bssid index of non-transmitted BSSID to join
	u8 is_virtual_ap;		// 1: ap to join is non-transmitted
	mbssid_virtual_ap_info_t virtual_ap_info[MAX_VIRTUAL_AP_NUM]; 	// store non-transmitted BSSID profile
	u8 *mbssid_ie; 		//start with non-transmitted BSSID profile
	u16 mbssid_ie_len;
} mbssid_info_t;
#endif

/* temporally add #pragma pack for structure alignment issue of
*   WLAN_BSSID_EX and get_WLAN_BSSID_EX_sz()
*/
#ifdef RTW_PACK_STRUCT_USE_INCLUDES
#  include "pack_begin.h"
#endif

typedef struct _WLAN_BSSID_EX {
	u32					Length;
	u8					MacAddress[6];
#ifdef CONFIG_MBSSID_AX
	mbssid_info_t mbssid_info;
#endif

	u8					Reserved[2];		//[0]: IS beacon frame
	NDIS_802_11_SSID			Ssid;
	u32					Privacy;
	NDIS_802_11_RSSI			Rssi;			//(in dBM,raw data ,get from PHY)
	NDIS_802_11_NETWORK_TYPE		NetworkTypeInUse;
	NDIS_802_11_CONFIGURATION		Configuration;
	NDIS_802_11_NETWORK_INFRASTRUCTURE	InfrastructureMode;
	NDIS_802_11_RATES_EX  		SupportedRates;
	WLAN_PHY_INFO				PhyInfo;
	u32  				IELength;
	u8  				IEs[MAX_IE_SZ];		//(timestamp, beacon interval, and capability information)
}
WLAN_BSSID_EX, *PWLAN_BSSID_EX;

#ifdef RTW_PACK_STRUCT_USE_INCLUDES
#  include "pack_end.h"
#endif


__inline  static uint get_WLAN_BSSID_EX_sz(WLAN_BSSID_EX *bss)
{
#if 0
	uint t_len;

	t_len = sizeof(u32)
			+ sizeof(NDIS_802_11_MAC_ADDRESS)
			+ 2
			+ sizeof(NDIS_802_11_SSID)
			+ sizeof(u32)
			+ sizeof(NDIS_802_11_RSSI)
			+ sizeof(NDIS_802_11_NETWORK_TYPE)
			+ sizeof(NDIS_802_11_CONFIGURATION)
			+ sizeof(NDIS_802_11_NETWORK_INFRASTRUCTURE)
			+ sizeof(NDIS_802_11_RATES_EX)
			//all new member add here
			+ sizeof(WLAN_PHY_INFO)
			//all new member add here
			+ sizeof(u32)
			+ bss->IELength;
	return t_len;
#else
	return (sizeof(WLAN_BSSID_EX) - MAX_IE_SZ + bss->IELength);
#endif
}

struct	wlan_network {
	_list	list;
	int	network_type;	//refer to ieee80211.h for WIRELESS_11A/B/G
	int	fixed;			// set to fixed when not to be removed as site-surveying
	unsigned long	last_scanned; //timestamp for the network
	int	aid;			//will only be valid when a BSS is joinned.
	int	join_res;
	WLAN_BSSID_EX	network; //must be the last item
	WLAN_BCN_INFO	BcnInfo;
};

enum VCS_TYPE {
	NONE_VCS,
	RTS_CTS,
	CTS_TO_SELF
};




#define PWR_CAM 0
#define PWR_MINPS 1
#define PWR_MAXPS 2
#define PWR_UAPSD 3
#define PWR_VOIP 4


enum UAPSD_MAX_SP {
	NO_LIMIT,
	TWO_MSDU,
	FOUR_MSDU,
	SIX_MSDU
};


#define NUM_PRE_AUTH_KEY 16
#define NUM_PMKID_CACHE NUM_PRE_AUTH_KEY

/*
* 	WPA2
*/

typedef struct _PMKID_CANDIDATE {
	NDIS_802_11_MAC_ADDRESS BSSID;
	u32 Flags;
} PMKID_CANDIDATE, *PPMKID_CANDIDATE;

typedef struct _NDIS_802_11_PMKID_CANDIDATE_LIST {
	u32 Version;       // Version of the structure
	u32 NumCandidates; // No. of pmkid candidates
	PMKID_CANDIDATE CandidateList[1];
} NDIS_802_11_PMKID_CANDIDATE_LIST, *PNDIS_802_11_PMKID_CANDIDATE_LIST;


typedef struct _NDIS_802_11_AUTHENTICATION_ENCRYPTION {
	NDIS_802_11_AUTHENTICATION_MODE AuthModeSupported;
	NDIS_802_11_ENCRYPTION_STATUS EncryptStatusSupported;

} NDIS_802_11_AUTHENTICATION_ENCRYPTION, *PNDIS_802_11_AUTHENTICATION_ENCRYPTION;

typedef struct _NDIS_802_11_CAPABILITY {
	u32  Length;
	u32  Version;
	u32  NoOfPMKIDs;
	u32  NoOfAuthEncryptPairsSupported;
	NDIS_802_11_AUTHENTICATION_ENCRYPTION AuthenticationEncryptionSupported[1];

} NDIS_802_11_CAPABILITY, *PNDIS_802_11_CAPABILITY;

#endif //#ifndef WLAN_BSSDEF_H_

