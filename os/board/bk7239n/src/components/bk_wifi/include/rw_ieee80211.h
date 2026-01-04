#ifndef _RW_IEEE80211_H_
#define _RW_IEEE80211_H_

#include "../wpa_supplicant-2.10/src/common/ieee802_11_defs.h"
#include "../wpa_supplicant-2.10/src/utils/platform.h"
#include <modules/wifi_types.h>
#include "bk_list.h"

#ifndef ETH_P_PAE
#define ETH_P_PAE         0x888E /* Port Access Entity (IEEE 802.1X) */
#endif /* ETH_P_PAE */

#define ETH_P_PREAUTH     0x88C7 /* IEEE 802.11i pre-authentication */

#define ETH_P_ECONET	  0x0018

#ifndef ETH_P_80211_RAW
#define ETH_P_80211_RAW           (ETH_P_ECONET + 1)
#endif

/* number of ACs */
#define IEEE80211_NUM_ACS		4

/* convert frequencies */
#define MHZ_TO_KHZ(freq) ((freq) * 1000)
#define KHZ_TO_MHZ(freq) ((freq) / 1000)
#define PR_KHZ(f) KHZ_TO_MHZ(f), f % 1000
#define KHZ_F "%d.%03d"

/* convert powers */
#define DBI_TO_MBI(gain) ((gain) * 100)
#define MBI_TO_DBI(gain) ((gain) / 100)
#define DBM_TO_MBM(gain) ((gain) * 100)
#define MBM_TO_DBM(gain) ((gain) / 100)

#define CHAN2G(_channel, _freq, _flags) {   \
	.band           = IEEE80211_BAND_2GHZ,  \
	.hw_value       = (_channel),           \
	.center_freq    = (_freq),              \
	.flags          = (_flags),             \
	.max_antenna_gain   = 0,                \
	.max_power      = 30,                   \
	}

#define CHAN5G(_channel, _flags) {		    \
	.band           = IEEE80211_BAND_5GHZ,      \
	.hw_value       = (_channel),               \
	.center_freq    = 5000 + (5 * (_channel)),  \
	.flags          = (_flags),                 \
	.max_antenna_gain   = 0,                    \
	.max_power      = 30,                       \
	}


/* 802.11n HT capabilities masks (for cap_info) */
#define IEEE80211_HT_CAP_LDPC_CODING		0x0001
#define IEEE80211_HT_CAP_SUP_WIDTH_20_40	0x0002
#define IEEE80211_HT_CAP_SM_PS			0x000C
#define		IEEE80211_HT_CAP_SM_PS_SHIFT	2
#define IEEE80211_HT_CAP_GRN_FLD		0x0010
#define IEEE80211_HT_CAP_SGI_20			0x0020
#define IEEE80211_HT_CAP_SGI_40			0x0040
#define IEEE80211_HT_CAP_TX_STBC		0x0080
#define IEEE80211_HT_CAP_RX_STBC		0x0300
#define		IEEE80211_HT_CAP_RX_STBC_SHIFT	8
#define IEEE80211_HT_CAP_DELAY_BA		0x0400
#define IEEE80211_HT_CAP_MAX_AMSDU		0x0800
#define IEEE80211_HT_CAP_DSSSCCK40		0x1000
#define IEEE80211_HT_CAP_RESERVED		0x2000
#define IEEE80211_HT_CAP_40MHZ_INTOLERANT	0x4000
#define IEEE80211_HT_CAP_LSIG_TXOP_PROT		0x8000

/* 802.11n HT extended capabilities masks (for extended_ht_cap_info) */
#define IEEE80211_HT_EXT_CAP_PCO		0x0001
#define IEEE80211_HT_EXT_CAP_PCO_TIME		0x0006
#define		IEEE80211_HT_EXT_CAP_PCO_TIME_SHIFT	1
#define IEEE80211_HT_EXT_CAP_MCS_FB		0x0300
#define		IEEE80211_HT_EXT_CAP_MCS_FB_SHIFT	8
#define IEEE80211_HT_EXT_CAP_HTC_SUP		0x0400
#define IEEE80211_HT_EXT_CAP_RD_RESPONDER	0x0800

/* 802.11n HT capability AMPDU settings (for ampdu_params_info) */
#define IEEE80211_HT_AMPDU_PARM_FACTOR		0x03
#define IEEE80211_HT_AMPDU_PARM_DENSITY		0x1C
#define		IEEE80211_HT_AMPDU_PARM_DENSITY_SHIFT	2

#define NL80211_MAX_SUPP_REG_RULES		128

/**
 * enum nl80211_reg_rule_flags - regulatory rule flags
 *
 * @NL80211_RRF_NO_OFDM: OFDM modulation not allowed
 * @NL80211_RRF_NO_CCK: CCK modulation not allowed
 * @NL80211_RRF_NO_INDOOR: indoor operation not allowed
 * @NL80211_RRF_NO_OUTDOOR: outdoor operation not allowed
 * @NL80211_RRF_DFS: DFS support is required to be used
 * @NL80211_RRF_PTP_ONLY: this is only for Point To Point links
 * @NL80211_RRF_PTMP_ONLY: this is only for Point To Multi Point links
 * @NL80211_RRF_NO_IR: no mechanisms that initiate radiation are allowed,
 *	this includes probe requests or modes of operation that require
 *	beaconing.
 * @NL80211_RRF_AUTO_BW: maximum available bandwidth should be calculated
 *	base on contiguous rules and wider channels will be allowed to cross
 *	multiple contiguous/overlapping frequency ranges.
 * @NL80211_RRF_IR_CONCURRENT: See %NL80211_FREQUENCY_ATTR_IR_CONCURRENT
 * @NL80211_RRF_NO_HT40MINUS: channels can't be used in HT40- operation
 * @NL80211_RRF_NO_HT40PLUS: channels can't be used in HT40+ operation
 * @NL80211_RRF_NO_80MHZ: 80MHz operation not allowed
 * @NL80211_RRF_NO_160MHZ: 160MHz operation not allowed
 * @NL80211_RRF_NO_HE: HE operation not allowed
 */
enum nl80211_reg_rule_flags {
	NL80211_RRF_NO_OFDM 	= 1<<0,
	NL80211_RRF_NO_CCK		= 1<<1,
	NL80211_RRF_NO_INDOOR		= 1<<2,
	NL80211_RRF_NO_OUTDOOR		= 1<<3,
	NL80211_RRF_DFS 		= 1<<4,
	NL80211_RRF_PTP_ONLY		= 1<<5,
	NL80211_RRF_PTMP_ONLY		= 1<<6,
	NL80211_RRF_NO_IR		= 1<<7,
	__NL80211_RRF_NO_IBSS		= 1<<8,
	NL80211_RRF_AUTO_BW 	= 1<<11,
	NL80211_RRF_IR_CONCURRENT	= 1<<12,
	NL80211_RRF_NO_HT40MINUS	= 1<<13,
	NL80211_RRF_NO_HT40PLUS 	= 1<<14,
	NL80211_RRF_NO_80MHZ		= 1<<15,
	NL80211_RRF_NO_160MHZ		= 1<<16,
	NL80211_RRF_NO_HE		= 1<<17,
	NL80211_RRF_NO_320MHZ		= 1<<18,
	NL80211_RRF_NO_EHT		= 1<<19,
};

#define NL80211_RRF_PASSIVE_SCAN	NL80211_RRF_NO_IR
#define NL80211_RRF_NO_IBSS		NL80211_RRF_NO_IR
#define NL80211_RRF_NO_IR		NL80211_RRF_NO_IR
#define NL80211_RRF_NO_HT40		(NL80211_RRF_NO_HT40MINUS |\
						 NL80211_RRF_NO_HT40PLUS)
#define NL80211_RRF_GO_CONCURRENT	NL80211_RRF_IR_CONCURRENT

	/* For backport compatibility with older userspace */
#define NL80211_RRF_NO_IR_ALL		(NL80211_RRF_NO_IR | __NL80211_RRF_NO_IBSS)

/*
 * Maximum length of AMPDU that the STA can receive in high-throughput (HT).
 * Length = 2 ^ (13 + max_ampdu_length_exp) - 1 (octets)
 */
enum ieee80211_max_ampdu_length_exp {
	IEEE80211_HT_MAX_AMPDU_8K = 0,
	IEEE80211_HT_MAX_AMPDU_16K = 1,
	IEEE80211_HT_MAX_AMPDU_32K = 2,
	IEEE80211_HT_MAX_AMPDU_64K = 3
};

/*
 * Maximum length of AMPDU that the STA can receive in VHT.
 * Length = 2 ^ (13 + max_ampdu_length_exp) - 1 (octets)
 */
enum ieee80211_vht_max_ampdu_length_exp {
	IEEE80211_VHT_MAX_AMPDU_8K = 0,
	IEEE80211_VHT_MAX_AMPDU_16K = 1,
	IEEE80211_VHT_MAX_AMPDU_32K = 2,
	IEEE80211_VHT_MAX_AMPDU_64K = 3,
	IEEE80211_VHT_MAX_AMPDU_128K = 4,
	IEEE80211_VHT_MAX_AMPDU_256K = 5,
	IEEE80211_VHT_MAX_AMPDU_512K = 6,
	IEEE80211_VHT_MAX_AMPDU_1024K = 7
};

#define IEEE80211_HT_MAX_AMPDU_FACTOR         13

/* Minimum MPDU start spacing */
enum ieee80211_min_mpdu_spacing {
	IEEE80211_HT_MPDU_DENSITY_NONE = 0,	/* No restriction */
	IEEE80211_HT_MPDU_DENSITY_0_25 = 1,	/* 1/4 usec */
	IEEE80211_HT_MPDU_DENSITY_0_5  = 2,	/* 1/2 usec */
	IEEE80211_HT_MPDU_DENSITY_1    = 3,	/* 1 usec */
	IEEE80211_HT_MPDU_DENSITY_2    = 4,	/* 2 usec */
	IEEE80211_HT_MPDU_DENSITY_4    = 5,	/* 4 usec */
	IEEE80211_HT_MPDU_DENSITY_8    = 6,	/* 8 usec */
	IEEE80211_HT_MPDU_DENSITY_16   = 7	/* 16 usec */
};

/* 802.11n HT capability MSC set */
#define IEEE80211_HT_MCS_RX_HIGHEST_MASK	    0x3ff
#define IEEE80211_HT_MCS_TX_DEFINED		        0x01
#define IEEE80211_HT_MCS_TX_RX_DIFF		        0x02

/* value 0 == 1 stream etc */
#define IEEE80211_HT_MCS_TX_MAX_STREAMS_MASK	0x0C
#define IEEE80211_HT_MCS_TX_MAX_STREAMS_SHIFT	2
#define	IEEE80211_HT_MCS_TX_MAX_STREAMS	        4
#define IEEE80211_HT_MCS_TX_UNEQUAL_MODULATION	0x10

/*
 * 802.11n D5.0 20.3.5 / 20.6 says:
 * - indices 0 to 7 and 32 are single spatial stream
 * - 8 to 31 are multiple spatial streams using equal modulation
 *   [8..15 for two streams, 16..23 for three and 24..31 for four]
 * - remainder are multiple spatial streams using unequal modulation
 */
#define IEEE80211_HT_MCS_UNEQUAL_MODULATION_START 33
#define IEEE80211_HT_MCS_UNEQUAL_MODULATION_START_BYTE \
	(IEEE80211_HT_MCS_UNEQUAL_MODULATION_START / 8)

/**
 * enum ieee80211_channel_flags - channel flags
 *
 * Channel flags set by the regulatory control code.
 *
 * @IEEE80211_CHAN_DISABLED: This channel is disabled.
 * @IEEE80211_CHAN_NO_IR: do not initiate radiation, this includes
 *	sending probe requests or beaconing.
 * @IEEE80211_CHAN_RADAR: Radar detection is required on this channel.
 * @IEEE80211_CHAN_NO_HT40PLUS: extension channel above this channel
 *	is not permitted.
 * @IEEE80211_CHAN_NO_HT40MINUS: extension channel below this channel
 *	is not permitted.
 * @IEEE80211_CHAN_NO_OFDM: OFDM is not allowed on this channel.
 * @IEEE80211_CHAN_NO_80MHZ: If the driver supports 80 MHz on the band,
 *	this flag indicates that an 80 MHz channel cannot use this
 *	channel as the control or any of the secondary channels.
 *	This may be due to the driver or due to regulatory bandwidth
 *	restrictions.
 * @IEEE80211_CHAN_NO_160MHZ: If the driver supports 160 MHz on the band,
 *	this flag indicates that an 160 MHz channel cannot use this
 *	channel as the control or any of the secondary channels.
 *	This may be due to the driver or due to regulatory bandwidth
 *	restrictions.
 * @IEEE80211_CHAN_INDOOR_ONLY: see %NL80211_FREQUENCY_ATTR_INDOOR_ONLY
 * @IEEE80211_CHAN_IR_CONCURRENT: see %NL80211_FREQUENCY_ATTR_IR_CONCURRENT
 * @IEEE80211_CHAN_NO_20MHZ: 20 MHz bandwidth is not permitted
 *	on this channel.
 * @IEEE80211_CHAN_NO_10MHZ: 10 MHz bandwidth is not permitted
 *	on this channel.
 * @IEEE80211_CHAN_NO_HE: HE operation is not permitted on this channel.
 * @IEEE80211_CHAN_1MHZ: 1 MHz bandwidth is permitted
 *	on this channel.
 * @IEEE80211_CHAN_2MHZ: 2 MHz bandwidth is permitted
 *	on this channel.
 * @IEEE80211_CHAN_4MHZ: 4 MHz bandwidth is permitted
 *	on this channel.
 * @IEEE80211_CHAN_8MHZ: 8 MHz bandwidth is permitted
 *	on this channel.
 * @IEEE80211_CHAN_16MHZ: 16 MHz bandwidth is permitted
 *	on this channel.
 *
 */
enum ieee80211_channel_flags {
	IEEE80211_CHAN_DISABLED		= 1<<0,
	IEEE80211_CHAN_NO_IR		= 1<<1,
	/* hole at 1<<2 */
	IEEE80211_CHAN_RADAR		= 1<<3,
	IEEE80211_CHAN_NO_HT40PLUS	= 1<<4,
	IEEE80211_CHAN_NO_HT40MINUS	= 1<<5,
	IEEE80211_CHAN_NO_OFDM		= 1<<6,
	IEEE80211_CHAN_NO_80MHZ		= 1<<7,
	IEEE80211_CHAN_NO_160MHZ	= 1<<8,
	IEEE80211_CHAN_INDOOR_ONLY	= 1<<9,
	IEEE80211_CHAN_IR_CONCURRENT	= 1<<10,
	IEEE80211_CHAN_NO_20MHZ		= 1<<11,
	IEEE80211_CHAN_NO_10MHZ		= 1<<12,
	IEEE80211_CHAN_NO_HE		= 1<<13,
	IEEE80211_CHAN_1MHZ		= 1<<14,
	IEEE80211_CHAN_2MHZ		= 1<<15,
	IEEE80211_CHAN_4MHZ		= 1<<16,
	IEEE80211_CHAN_8MHZ		= 1<<17,
	IEEE80211_CHAN_16MHZ		= 1<<18,
	IEEE80211_CHAN_NO_320MHZ	= 1<<19,
	IEEE80211_CHAN_NO_EHT		= 1<<20,
};

#define IEEE80211_CHAN_NO_HT40 \
		(IEEE80211_CHAN_NO_HT40PLUS | IEEE80211_CHAN_NO_HT40MINUS)

#define IEEE80211_DFS_MIN_CAC_TIME_MS		60000
#define IEEE80211_DFS_MIN_NOP_TIME_MS		(30 * 60 * 1000)

/**
 * enum nl80211_dfs_state - DFS states for channels
 *
 * Channel states used by the DFS code.
 *
 * @NL80211_DFS_USABLE: The channel can be used, but channel availability
 *	check (CAC) must be performed before using it for AP or IBSS.
 * @NL80211_DFS_UNAVAILABLE: A radar has been detected on this channel, it
 *	is therefore marked as not available.
 * @NL80211_DFS_AVAILABLE: The channel has been CAC checked and is available.
 */
enum nl80211_dfs_state {
	NL80211_DFS_USABLE,
	NL80211_DFS_UNAVAILABLE,
	NL80211_DFS_AVAILABLE,
};

/**
 * struct ieee80211_channel - channel definition
 *
 * This structure describes a single channel for use
 * with cfg80211.
 *
 * @center_freq: center frequency in MHz
 * @freq_offset: offset from @center_freq, in KHz
 * @hw_value: hardware-specific value for the channel
 * @flags: channel flags from &enum ieee80211_channel_flags.
 * @orig_flags: channel flags at registration time, used by regulatory
 *	code to support devices with additional restrictions
 * @band: band this channel belongs to.
 * @max_antenna_gain: maximum antenna gain in dBi
 * @max_power: maximum transmission power (in dBm)
 * @max_reg_power: maximum regulatory transmission power (in dBm)
 * @beacon_found: helper to regulatory code to indicate when a beacon
 *	has been found on this channel. Use regulatory_hint_found_beacon()
 *	to enable this, this is useful only on 5 GHz band.
 * @orig_mag: internal use
 * @orig_mpwr: internal use
 * @dfs_state: current state of this channel. Only relevant if radar is required
 *	on this channel.
 * @dfs_state_entered: timestamp (jiffies) when the dfs state was entered.
 * @dfs_cac_ms: DFS CAC time in milliseconds, this is valid for DFS channels.
 */
struct ieee80211_channel {
	wifi_band_t band;
	UINT16 center_freq;
	UINT16 freq_offset;
	UINT16 hw_value;
	UINT32 flags;
	INT32 max_antenna_gain;
	INT32 max_power;
	int max_reg_power;
	bool beacon_found;
	u32 orig_flags;
	int orig_mag, orig_mpwr;
	enum nl80211_dfs_state dfs_state;
	unsigned long dfs_state_entered;
	unsigned int dfs_cac_ms;
};

/* Extended Channel Switching capability to be set in the 1st byte of
 * the @WLAN_EID_EXT_CAPABILITY information element
 */
#define WLAN_EXT_CAPA1_EXT_CHANNEL_SWITCHING	BIT(2)

/* Multiple BSSID capability is set in the 6th bit of 3rd byte of the
 * @WLAN_EID_EXT_CAPABILITY information element
 */
#define WLAN_EXT_CAPA3_MULTI_BSSID_SUPPORT	BIT(6)

/* Timing Measurement protocol for time sync is set in the 7th bit of 3rd byte
 * of the @WLAN_EID_EXT_CAPABILITY information element
 */
#define WLAN_EXT_CAPA3_TIMING_MEASUREMENT_SUPPORT	BIT(7)

/* TDLS capabilities in the 4th byte of @WLAN_EID_EXT_CAPABILITY */
#define WLAN_EXT_CAPA4_TDLS_BUFFER_STA		BIT(4)
#define WLAN_EXT_CAPA4_TDLS_PEER_PSM		BIT(5)
#define WLAN_EXT_CAPA4_TDLS_CHAN_SWITCH		BIT(6)

/* Interworking capabilities are set in 7th bit of 4th byte of the
 * @WLAN_EID_EXT_CAPABILITY information element
 */
#define WLAN_EXT_CAPA4_INTERWORKING_ENABLED	BIT(7)

/*
 * TDLS capabililites to be enabled in the 5th byte of the
 * @WLAN_EID_EXT_CAPABILITY information element
 */
#define WLAN_EXT_CAPA5_TDLS_ENABLED	BIT(5)
#define WLAN_EXT_CAPA5_TDLS_PROHIBITED	BIT(6)
#define WLAN_EXT_CAPA5_TDLS_CH_SW_PROHIBITED	BIT(7)

#define WLAN_EXT_CAPA8_TDLS_WIDE_BW_ENABLED	BIT(5)
#define WLAN_EXT_CAPA8_OPMODE_NOTIF	BIT(6)

/* Defines the maximal number of MSDUs in an A-MSDU. */
#define WLAN_EXT_CAPA8_MAX_MSDU_IN_AMSDU_LSB	BIT(7)
#define WLAN_EXT_CAPA9_MAX_MSDU_IN_AMSDU_MSB	BIT(0)

/*
 * Fine Timing Measurement Initiator - bit 71 of @WLAN_EID_EXT_CAPABILITY
 * information element
 */
#define WLAN_EXT_CAPA9_FTM_INITIATOR	BIT(7)

/* Defines support for TWT Requester and TWT Responder */
#define WLAN_EXT_CAPA10_TWT_REQUESTER_SUPPORT	BIT(5)
#define WLAN_EXT_CAPA10_TWT_RESPONDER_SUPPORT	BIT(6)

/*
 * When set, indicates that the AP is able to tolerate 26-tone RU UL
 * OFDMA transmissions using HE TB PPDU from OBSS (not falsely classify the
 * 26-tone RU UL OFDMA transmissions as radar pulses).
 */
#define WLAN_EXT_CAPA10_OBSS_NARROW_BW_RU_TOLERANCE_SUPPORT BIT(7)

/* Defines support for enhanced multi-bssid advertisement*/
#define WLAN_EXT_CAPA11_EMA_SUPPORT	BIT(3)

/* U-APSD queue for WMM IEs sent by AP */
#define IEEE80211_WMM_IE_AP_QOSINFO_UAPSD	(1<<7)
#define IEEE80211_WMM_IE_AP_QOSINFO_PARAM_SET_CNT_MASK	0x0f

/* U-APSD queues for WMM IEs sent by STA */
#define IEEE80211_WMM_IE_STA_QOSINFO_AC_VO	(1<<0)
#define IEEE80211_WMM_IE_STA_QOSINFO_AC_VI	(1<<1)
#define IEEE80211_WMM_IE_STA_QOSINFO_AC_BK	(1<<2)
#define IEEE80211_WMM_IE_STA_QOSINFO_AC_BE	(1<<3)
#define IEEE80211_WMM_IE_STA_QOSINFO_AC_MASK	0x0f

/* U-APSD max SP length for WMM IEs sent by STA */
#define IEEE80211_WMM_IE_STA_QOSINFO_SP_ALL	0x00
#define IEEE80211_WMM_IE_STA_QOSINFO_SP_2	0x01
#define IEEE80211_WMM_IE_STA_QOSINFO_SP_4	0x02
#define IEEE80211_WMM_IE_STA_QOSINFO_SP_6	0x03
#define IEEE80211_WMM_IE_STA_QOSINFO_SP_MASK	0x03
#define IEEE80211_WMM_IE_STA_QOSINFO_SP_SHIFT	5

#define IEEE80211_HT_MCS_MASK_LEN		10

/* 802.11ac VHT Capabilities */
#define IEEE80211_VHT_CAP_MAX_MPDU_LENGTH_3895			0x00000000
#define IEEE80211_VHT_CAP_MAX_MPDU_LENGTH_7991			0x00000001
#define IEEE80211_VHT_CAP_MAX_MPDU_LENGTH_11454			0x00000002
#define IEEE80211_VHT_CAP_MAX_MPDU_MASK				0x00000003
#define IEEE80211_VHT_CAP_SUPP_CHAN_WIDTH_160MHZ		0x00000004
#define IEEE80211_VHT_CAP_SUPP_CHAN_WIDTH_160_80PLUS80MHZ	0x00000008
#define IEEE80211_VHT_CAP_SUPP_CHAN_WIDTH_MASK			0x0000000C
#define IEEE80211_VHT_CAP_RXLDPC				0x00000010
#define IEEE80211_VHT_CAP_SHORT_GI_80				0x00000020
#define IEEE80211_VHT_CAP_SHORT_GI_160				0x00000040
#define IEEE80211_VHT_CAP_TXSTBC				0x00000080
#define IEEE80211_VHT_CAP_RXSTBC_1				0x00000100
#define IEEE80211_VHT_CAP_RXSTBC_2				0x00000200
#define IEEE80211_VHT_CAP_RXSTBC_3				0x00000300
#define IEEE80211_VHT_CAP_RXSTBC_4				0x00000400
#define IEEE80211_VHT_CAP_RXSTBC_MASK				0x00000700
#define IEEE80211_VHT_CAP_SU_BEAMFORMER_CAPABLE			0x00000800
#define IEEE80211_VHT_CAP_SU_BEAMFORMEE_CAPABLE			0x00001000
#define IEEE80211_VHT_CAP_BEAMFORMEE_STS_SHIFT                  13
#define IEEE80211_VHT_CAP_BEAMFORMEE_STS_MASK			\
	(7 << IEEE80211_VHT_CAP_BEAMFORMEE_STS_SHIFT)
#define IEEE80211_VHT_CAP_SOUNDING_DIMENSIONS_SHIFT		16
#define IEEE80211_VHT_CAP_SOUNDING_DIMENSIONS_MASK		\
	(7 << IEEE80211_VHT_CAP_SOUNDING_DIMENSIONS_SHIFT)
#define IEEE80211_VHT_CAP_MU_BEAMFORMER_CAPABLE			0x00080000
#define IEEE80211_VHT_CAP_MU_BEAMFORMEE_CAPABLE			0x00100000
#define IEEE80211_VHT_CAP_VHT_TXOP_PS				0x00200000
#define IEEE80211_VHT_CAP_HTC_VHT				0x00400000
#define IEEE80211_VHT_CAP_MAX_A_MPDU_LENGTH_EXPONENT_SHIFT	23
#define IEEE80211_VHT_CAP_MAX_A_MPDU_LENGTH_EXPONENT_MASK	\
	(7 << IEEE80211_VHT_CAP_MAX_A_MPDU_LENGTH_EXPONENT_SHIFT)
#define IEEE80211_VHT_CAP_VHT_LINK_ADAPTATION_VHT_UNSOL_MFB	0x08000000
#define IEEE80211_VHT_CAP_VHT_LINK_ADAPTATION_VHT_MRQ_MFB	0x0c000000
#define IEEE80211_VHT_CAP_RX_ANTENNA_PATTERN			0x10000000
#define IEEE80211_VHT_CAP_TX_ANTENNA_PATTERN			0x20000000
#define IEEE80211_VHT_CAP_EXT_NSS_BW_SHIFT			30
#define IEEE80211_VHT_CAP_EXT_NSS_BW_MASK			0xc0000000



/**
 * enum ieee80211_vht_mcs_support - VHT MCS support definitions
 * @IEEE80211_VHT_MCS_SUPPORT_0_7: MCSes 0-7 are supported for the
 *	number of streams
 * @IEEE80211_VHT_MCS_SUPPORT_0_8: MCSes 0-8 are supported
 * @IEEE80211_VHT_MCS_SUPPORT_0_9: MCSes 0-9 are supported
 * @IEEE80211_VHT_MCS_NOT_SUPPORTED: This number of streams isn't supported
 *
 * These definitions are used in each 2-bit subfield of the @rx_mcs_map
 * and @tx_mcs_map fields of &struct ieee80211_vht_mcs_info, which are
 * both split into 8 subfields by number of streams. These values indicate
 * which MCSes are supported for the number of streams the value appears
 * for.
 */
enum ieee80211_vht_mcs_support {
	IEEE80211_VHT_MCS_SUPPORT_0_7	= 0,
	IEEE80211_VHT_MCS_SUPPORT_0_8	= 1,
	IEEE80211_VHT_MCS_SUPPORT_0_9	= 2,
	IEEE80211_VHT_MCS_NOT_SUPPORTED	= 3,
};

/* 802.11ax HE MAC capabilities */
#define IEEE80211_HE_MAC_CAP0_HTC_HE				0x01
#define IEEE80211_HE_MAC_CAP0_TWT_REQ				0x02
#define IEEE80211_HE_MAC_CAP0_TWT_RES				0x04
#define IEEE80211_HE_MAC_CAP0_DYNAMIC_FRAG_NOT_SUPP		0x00
#define IEEE80211_HE_MAC_CAP0_DYNAMIC_FRAG_LEVEL_1		0x08
#define IEEE80211_HE_MAC_CAP0_DYNAMIC_FRAG_LEVEL_2		0x10
#define IEEE80211_HE_MAC_CAP0_DYNAMIC_FRAG_LEVEL_3		0x18
#define IEEE80211_HE_MAC_CAP0_DYNAMIC_FRAG_MASK			0x18
#define IEEE80211_HE_MAC_CAP0_MAX_NUM_FRAG_MSDU_1		0x00
#define IEEE80211_HE_MAC_CAP0_MAX_NUM_FRAG_MSDU_2		0x20
#define IEEE80211_HE_MAC_CAP0_MAX_NUM_FRAG_MSDU_4		0x40
#define IEEE80211_HE_MAC_CAP0_MAX_NUM_FRAG_MSDU_8		0x60
#define IEEE80211_HE_MAC_CAP0_MAX_NUM_FRAG_MSDU_16		0x80
#define IEEE80211_HE_MAC_CAP0_MAX_NUM_FRAG_MSDU_32		0xa0
#define IEEE80211_HE_MAC_CAP0_MAX_NUM_FRAG_MSDU_64		0xc0
#define IEEE80211_HE_MAC_CAP0_MAX_NUM_FRAG_MSDU_UNLIMITED	0xe0
#define IEEE80211_HE_MAC_CAP0_MAX_NUM_FRAG_MSDU_MASK		0xe0

#define IEEE80211_HE_MAC_CAP1_MIN_FRAG_SIZE_UNLIMITED		0x00
#define IEEE80211_HE_MAC_CAP1_MIN_FRAG_SIZE_128			0x01
#define IEEE80211_HE_MAC_CAP1_MIN_FRAG_SIZE_256			0x02
#define IEEE80211_HE_MAC_CAP1_MIN_FRAG_SIZE_512			0x03
#define IEEE80211_HE_MAC_CAP1_MIN_FRAG_SIZE_MASK		0x03
#define IEEE80211_HE_MAC_CAP1_TF_MAC_PAD_DUR_0US		0x00
#define IEEE80211_HE_MAC_CAP1_TF_MAC_PAD_DUR_8US		0x04
#define IEEE80211_HE_MAC_CAP1_TF_MAC_PAD_DUR_16US		0x08
#define IEEE80211_HE_MAC_CAP1_TF_MAC_PAD_DUR_MASK		0x0c
#define IEEE80211_HE_MAC_CAP1_MULTI_TID_AGG_RX_QOS_1		0x00
#define IEEE80211_HE_MAC_CAP1_MULTI_TID_AGG_RX_QOS_2		0x10
#define IEEE80211_HE_MAC_CAP1_MULTI_TID_AGG_RX_QOS_3		0x20
#define IEEE80211_HE_MAC_CAP1_MULTI_TID_AGG_RX_QOS_4		0x30
#define IEEE80211_HE_MAC_CAP1_MULTI_TID_AGG_RX_QOS_5		0x40
#define IEEE80211_HE_MAC_CAP1_MULTI_TID_AGG_RX_QOS_6		0x50
#define IEEE80211_HE_MAC_CAP1_MULTI_TID_AGG_RX_QOS_7		0x60
#define IEEE80211_HE_MAC_CAP1_MULTI_TID_AGG_RX_QOS_8		0x70
#define IEEE80211_HE_MAC_CAP1_MULTI_TID_AGG_RX_QOS_MASK		0x70

/* Link adaptation is split between byte HE_MAC_CAP1 and
 * HE_MAC_CAP2. It should be set only if IEEE80211_HE_MAC_CAP0_HTC_HE
 * in which case the following values apply:
 * 0 = No feedback.
 * 1 = reserved.
 * 2 = Unsolicited feedback.
 * 3 = both
 */
#define IEEE80211_HE_MAC_CAP1_LINK_ADAPTATION			0x80

#define IEEE80211_HE_MAC_CAP2_LINK_ADAPTATION			0x01
#define IEEE80211_HE_MAC_CAP2_ALL_ACK				0x02
#define IEEE80211_HE_MAC_CAP2_TRS				0x04
#define IEEE80211_HE_MAC_CAP2_BSR				0x08
#define IEEE80211_HE_MAC_CAP2_BCAST_TWT				0x10
#define IEEE80211_HE_MAC_CAP2_32BIT_BA_BITMAP			0x20
#define IEEE80211_HE_MAC_CAP2_MU_CASCADING			0x40
#define IEEE80211_HE_MAC_CAP2_ACK_EN				0x80

#define IEEE80211_HE_MAC_CAP3_OMI_CONTROL			0x02
#define IEEE80211_HE_MAC_CAP3_OFDMA_RA				0x04

/* The maximum length of an A-MDPU is defined by the combination of the Maximum
 * A-MDPU Length Exponent field in the HT capabilities, VHT capabilities and the
 * same field in the HE capabilities.
 */
#define IEEE80211_HE_MAC_CAP3_MAX_AMPDU_LEN_EXP_USE_VHT	0x00
#define IEEE80211_HE_MAC_CAP3_MAX_AMPDU_LEN_EXP_VHT_1		0x08
#define IEEE80211_HE_MAC_CAP3_MAX_AMPDU_LEN_EXP_VHT_2		0x10
#define IEEE80211_HE_MAC_CAP3_MAX_AMPDU_LEN_EXP_RESERVED	0x18
#define IEEE80211_HE_MAC_CAP3_MAX_AMPDU_LEN_EXP_MASK		0x18
#define IEEE80211_HE_MAC_CAP3_AMSDU_FRAG			0x20
#define IEEE80211_HE_MAC_CAP3_FLEX_TWT_SCHED			0x40
#define IEEE80211_HE_MAC_CAP3_RX_CTRL_FRAME_TO_MULTIBSS		0x80

#define IEEE80211_HE_MAC_CAP4_BSRP_BQRP_A_MPDU_AGG		0x01
#define IEEE80211_HE_MAC_CAP4_QTP				0x02
#define IEEE80211_HE_MAC_CAP4_BQR				0x04
#define IEEE80211_HE_MAC_CAP4_SRP_RESP				0x08
#define IEEE80211_HE_MAC_CAP4_NDP_FB_REP			0x10
#define IEEE80211_HE_MAC_CAP4_OPS				0x20
#define IEEE80211_HE_MAC_CAP4_AMDSU_IN_AMPDU			0x40
/* Multi TID agg TX is split between byte #4 and #5
 * The value is a combination of B39,B40,B41
 */
#define IEEE80211_HE_MAC_CAP4_MULTI_TID_AGG_TX_QOS_B39		0x80

#define IEEE80211_HE_MAC_CAP5_MULTI_TID_AGG_TX_QOS_B40		0x01
#define IEEE80211_HE_MAC_CAP5_MULTI_TID_AGG_TX_QOS_B41		0x02
#define IEEE80211_HE_MAC_CAP5_SUBCHAN_SELECVITE_TRANSMISSION	0x04
#define IEEE80211_HE_MAC_CAP5_UL_2x996_TONE_RU			0x08
#define IEEE80211_HE_MAC_CAP5_OM_CTRL_UL_MU_DATA_DIS_RX		0x10
#define IEEE80211_HE_MAC_CAP5_HE_DYNAMIC_SM_PS			0x20
#define IEEE80211_HE_MAC_CAP5_PUNCTURED_SOUNDING		0x40
#define IEEE80211_HE_MAC_CAP5_HT_VHT_TRIG_FRAME_RX		0x80

/* 802.11ax HE PHY capabilities */
#define IEEE80211_HE_PHY_CAP0_CHANNEL_WIDTH_SET_40MHZ_IN_2G		0x02
#define IEEE80211_HE_PHY_CAP0_CHANNEL_WIDTH_SET_40MHZ_80MHZ_IN_5G	0x04
#define IEEE80211_HE_PHY_CAP0_CHANNEL_WIDTH_SET_160MHZ_IN_5G		0x08
#define IEEE80211_HE_PHY_CAP0_CHANNEL_WIDTH_SET_80PLUS80_MHZ_IN_5G	0x10
#define IEEE80211_HE_PHY_CAP0_CHANNEL_WIDTH_SET_RU_MAPPING_IN_2G	0x20
#define IEEE80211_HE_PHY_CAP0_CHANNEL_WIDTH_SET_RU_MAPPING_IN_5G	0x40
#define IEEE80211_HE_PHY_CAP0_CHANNEL_WIDTH_SET_MASK			0xfe

#define IEEE80211_HE_PHY_CAP1_PREAMBLE_PUNC_RX_80MHZ_ONLY_SECOND_20MHZ	0x01
#define IEEE80211_HE_PHY_CAP1_PREAMBLE_PUNC_RX_80MHZ_ONLY_SECOND_40MHZ	0x02
#define IEEE80211_HE_PHY_CAP1_PREAMBLE_PUNC_RX_160MHZ_ONLY_SECOND_20MHZ	0x04
#define IEEE80211_HE_PHY_CAP1_PREAMBLE_PUNC_RX_160MHZ_ONLY_SECOND_40MHZ	0x08
#define IEEE80211_HE_PHY_CAP1_PREAMBLE_PUNC_RX_MASK			0x0f
#define IEEE80211_HE_PHY_CAP1_DEVICE_CLASS_A				0x10
#define IEEE80211_HE_PHY_CAP1_LDPC_CODING_IN_PAYLOAD			0x20
#define IEEE80211_HE_PHY_CAP1_HE_LTF_AND_GI_FOR_HE_PPDUS_0_8US		0x40
/* Midamble RX/TX Max NSTS is split between byte #2 and byte #3 */
#define IEEE80211_HE_PHY_CAP1_MIDAMBLE_RX_TX_MAX_NSTS			0x80

#define IEEE80211_HE_PHY_CAP2_MIDAMBLE_RX_TX_MAX_NSTS			0x01
#define IEEE80211_HE_PHY_CAP2_NDP_4x_LTF_AND_3_2US			0x02
#define IEEE80211_HE_PHY_CAP2_STBC_TX_UNDER_80MHZ			0x04
#define IEEE80211_HE_PHY_CAP2_STBC_RX_UNDER_80MHZ			0x08
#define IEEE80211_HE_PHY_CAP2_DOPPLER_TX				0x10
#define IEEE80211_HE_PHY_CAP2_DOPPLER_RX				0x20

/* Note that the meaning of UL MU below is different between an AP and a non-AP
 * sta, where in the AP case it indicates support for Rx and in the non-AP sta
 * case it indicates support for Tx.
 */
#define IEEE80211_HE_PHY_CAP2_UL_MU_FULL_MU_MIMO			0x40
#define IEEE80211_HE_PHY_CAP2_UL_MU_PARTIAL_MU_MIMO			0x80

#define IEEE80211_HE_PHY_CAP3_DCM_MAX_CONST_TX_NO_DCM			0x00
#define IEEE80211_HE_PHY_CAP3_DCM_MAX_CONST_TX_BPSK			0x01
#define IEEE80211_HE_PHY_CAP3_DCM_MAX_CONST_TX_QPSK			0x02
#define IEEE80211_HE_PHY_CAP3_DCM_MAX_CONST_TX_16_QAM			0x03
#define IEEE80211_HE_PHY_CAP3_DCM_MAX_CONST_TX_MASK			0x03
#define IEEE80211_HE_PHY_CAP3_DCM_MAX_TX_NSS_1				0x00
#define IEEE80211_HE_PHY_CAP3_DCM_MAX_TX_NSS_2				0x04
#define IEEE80211_HE_PHY_CAP3_DCM_MAX_CONST_RX_NO_DCM			0x00
#define IEEE80211_HE_PHY_CAP3_DCM_MAX_CONST_RX_BPSK			0x08
#define IEEE80211_HE_PHY_CAP3_DCM_MAX_CONST_RX_QPSK			0x10
#define IEEE80211_HE_PHY_CAP3_DCM_MAX_CONST_RX_16_QAM			0x18
#define IEEE80211_HE_PHY_CAP3_DCM_MAX_CONST_RX_MASK			0x18
#define IEEE80211_HE_PHY_CAP3_DCM_MAX_RX_NSS_1				0x00
#define IEEE80211_HE_PHY_CAP3_DCM_MAX_RX_NSS_2				0x20
#define IEEE80211_HE_PHY_CAP3_RX_PARTIAL_BW_SU_IN_20MHZ_MU		0x40
#define IEEE80211_HE_PHY_CAP3_SU_BEAMFORMER				0x80

#define IEEE80211_HE_PHY_CAP4_SU_BEAMFORMEE				0x01
#define IEEE80211_HE_PHY_CAP4_MU_BEAMFORMER				0x02

/* Minimal allowed value of Max STS under 80MHz is 3 */
#define IEEE80211_HE_PHY_CAP4_BEAMFORMEE_MAX_STS_UNDER_80MHZ_4		0x0c
#define IEEE80211_HE_PHY_CAP4_BEAMFORMEE_MAX_STS_UNDER_80MHZ_5		0x10
#define IEEE80211_HE_PHY_CAP4_BEAMFORMEE_MAX_STS_UNDER_80MHZ_6		0x14
#define IEEE80211_HE_PHY_CAP4_BEAMFORMEE_MAX_STS_UNDER_80MHZ_7		0x18
#define IEEE80211_HE_PHY_CAP4_BEAMFORMEE_MAX_STS_UNDER_80MHZ_8		0x1c
#define IEEE80211_HE_PHY_CAP4_BEAMFORMEE_MAX_STS_UNDER_80MHZ_MASK	0x1c

/* Minimal allowed value of Max STS above 80MHz is 3 */
#define IEEE80211_HE_PHY_CAP4_BEAMFORMEE_MAX_STS_ABOVE_80MHZ_4		0x60
#define IEEE80211_HE_PHY_CAP4_BEAMFORMEE_MAX_STS_ABOVE_80MHZ_5		0x80
#define IEEE80211_HE_PHY_CAP4_BEAMFORMEE_MAX_STS_ABOVE_80MHZ_6		0xa0
#define IEEE80211_HE_PHY_CAP4_BEAMFORMEE_MAX_STS_ABOVE_80MHZ_7		0xc0
#define IEEE80211_HE_PHY_CAP4_BEAMFORMEE_MAX_STS_ABOVE_80MHZ_8		0xe0
#define IEEE80211_HE_PHY_CAP4_BEAMFORMEE_MAX_STS_ABOVE_80MHZ_MASK	0xe0

#define IEEE80211_HE_PHY_CAP5_BEAMFORMEE_NUM_SND_DIM_UNDER_80MHZ_1	0x00
#define IEEE80211_HE_PHY_CAP5_BEAMFORMEE_NUM_SND_DIM_UNDER_80MHZ_2	0x01
#define IEEE80211_HE_PHY_CAP5_BEAMFORMEE_NUM_SND_DIM_UNDER_80MHZ_3	0x02
#define IEEE80211_HE_PHY_CAP5_BEAMFORMEE_NUM_SND_DIM_UNDER_80MHZ_4	0x03
#define IEEE80211_HE_PHY_CAP5_BEAMFORMEE_NUM_SND_DIM_UNDER_80MHZ_5	0x04
#define IEEE80211_HE_PHY_CAP5_BEAMFORMEE_NUM_SND_DIM_UNDER_80MHZ_6	0x05
#define IEEE80211_HE_PHY_CAP5_BEAMFORMEE_NUM_SND_DIM_UNDER_80MHZ_7	0x06
#define IEEE80211_HE_PHY_CAP5_BEAMFORMEE_NUM_SND_DIM_UNDER_80MHZ_8	0x07
#define IEEE80211_HE_PHY_CAP5_BEAMFORMEE_NUM_SND_DIM_UNDER_80MHZ_MASK	0x07

#define IEEE80211_HE_PHY_CAP5_BEAMFORMEE_NUM_SND_DIM_ABOVE_80MHZ_1	0x00
#define IEEE80211_HE_PHY_CAP5_BEAMFORMEE_NUM_SND_DIM_ABOVE_80MHZ_2	0x08
#define IEEE80211_HE_PHY_CAP5_BEAMFORMEE_NUM_SND_DIM_ABOVE_80MHZ_3	0x10
#define IEEE80211_HE_PHY_CAP5_BEAMFORMEE_NUM_SND_DIM_ABOVE_80MHZ_4	0x18
#define IEEE80211_HE_PHY_CAP5_BEAMFORMEE_NUM_SND_DIM_ABOVE_80MHZ_5	0x20
#define IEEE80211_HE_PHY_CAP5_BEAMFORMEE_NUM_SND_DIM_ABOVE_80MHZ_6	0x28
#define IEEE80211_HE_PHY_CAP5_BEAMFORMEE_NUM_SND_DIM_ABOVE_80MHZ_7	0x30
#define IEEE80211_HE_PHY_CAP5_BEAMFORMEE_NUM_SND_DIM_ABOVE_80MHZ_8	0x38
#define IEEE80211_HE_PHY_CAP5_BEAMFORMEE_NUM_SND_DIM_ABOVE_80MHZ_MASK	0x38

#define IEEE80211_HE_PHY_CAP5_NG16_SU_FEEDBACK				0x40
#define IEEE80211_HE_PHY_CAP5_NG16_MU_FEEDBACK				0x80

#define IEEE80211_HE_PHY_CAP6_CODEBOOK_SIZE_42_SU			0x01
#define IEEE80211_HE_PHY_CAP6_CODEBOOK_SIZE_75_MU			0x02
#define IEEE80211_HE_PHY_CAP6_TRIG_SU_BEAMFORMING_FB			0x04
#define IEEE80211_HE_PHY_CAP6_TRIG_MU_BEAMFORMING_PARTIAL_BW_FB		0x08
#define IEEE80211_HE_PHY_CAP6_TRIG_CQI_FB				0x10
#define IEEE80211_HE_PHY_CAP6_PARTIAL_BW_EXT_RANGE			0x20
#define IEEE80211_HE_PHY_CAP6_PARTIAL_BANDWIDTH_DL_MUMIMO		0x40
#define IEEE80211_HE_PHY_CAP6_PPE_THRESHOLD_PRESENT			0x80

#define IEEE80211_HE_PHY_CAP7_SRP_BASED_SR				0x01
#define IEEE80211_HE_PHY_CAP7_POWER_BOOST_FACTOR_AR			0x02
#define IEEE80211_HE_PHY_CAP7_HE_SU_MU_PPDU_4XLTF_AND_08_US_GI		0x04
#define IEEE80211_HE_PHY_CAP7_MAX_NC_1					0x08
#define IEEE80211_HE_PHY_CAP7_MAX_NC_2					0x10
#define IEEE80211_HE_PHY_CAP7_MAX_NC_3					0x18
#define IEEE80211_HE_PHY_CAP7_MAX_NC_4					0x20
#define IEEE80211_HE_PHY_CAP7_MAX_NC_5					0x28
#define IEEE80211_HE_PHY_CAP7_MAX_NC_6					0x30
#define IEEE80211_HE_PHY_CAP7_MAX_NC_7					0x38
#define IEEE80211_HE_PHY_CAP7_MAX_NC_MASK				0x38
#define IEEE80211_HE_PHY_CAP7_STBC_TX_ABOVE_80MHZ			0x40
#define IEEE80211_HE_PHY_CAP7_STBC_RX_ABOVE_80MHZ			0x80

#define IEEE80211_HE_PHY_CAP8_HE_ER_SU_PPDU_4XLTF_AND_08_US_GI		0x01
#define IEEE80211_HE_PHY_CAP8_20MHZ_IN_40MHZ_HE_PPDU_IN_2G		0x02
#define IEEE80211_HE_PHY_CAP8_20MHZ_IN_160MHZ_HE_PPDU			0x04
#define IEEE80211_HE_PHY_CAP8_80MHZ_IN_160MHZ_HE_PPDU			0x08
#define IEEE80211_HE_PHY_CAP8_HE_ER_SU_1XLTF_AND_08_US_GI		0x10
#define IEEE80211_HE_PHY_CAP8_MIDAMBLE_RX_TX_2X_AND_1XLTF		0x20
#define IEEE80211_HE_PHY_CAP8_DCM_MAX_RU_242				0x00
#define IEEE80211_HE_PHY_CAP8_DCM_MAX_RU_484				0x40
#define IEEE80211_HE_PHY_CAP8_DCM_MAX_RU_996				0x80
#define IEEE80211_HE_PHY_CAP8_DCM_MAX_RU_2x996				0xc0
#define IEEE80211_HE_PHY_CAP8_DCM_MAX_RU_MASK				0xc0

#define IEEE80211_HE_PHY_CAP9_LONGER_THAN_16_SIGB_OFDM_SYM		0x01
#define IEEE80211_HE_PHY_CAP9_NON_TRIGGERED_CQI_FEEDBACK		0x02
#define IEEE80211_HE_PHY_CAP9_TX_1024_QAM_LESS_THAN_242_TONE_RU		0x04
#define IEEE80211_HE_PHY_CAP9_RX_1024_QAM_LESS_THAN_242_TONE_RU		0x08
#define IEEE80211_HE_PHY_CAP9_RX_FULL_BW_SU_USING_MU_WITH_COMP_SIGB	0x10
#define IEEE80211_HE_PHY_CAP9_RX_FULL_BW_SU_USING_MU_WITH_NON_COMP_SIGB	0x20
#define IEEE80211_HE_PHY_CAP9_NOMIMAL_PKT_PADDING_0US			0x00
#define IEEE80211_HE_PHY_CAP9_NOMIMAL_PKT_PADDING_8US			0x40
#define IEEE80211_HE_PHY_CAP9_NOMIMAL_PKT_PADDING_16US			0x80
#define IEEE80211_HE_PHY_CAP9_NOMIMAL_PKT_PADDING_RESERVED		0xc0
#define IEEE80211_HE_PHY_CAP9_NOMIMAL_PKT_PADDING_MASK			0xc0

/* 802.11ax HE TX/RX MCS NSS Support  */
#define IEEE80211_TX_RX_MCS_NSS_SUPP_HIGHEST_MCS_POS			(3)
#define IEEE80211_TX_RX_MCS_NSS_SUPP_TX_BITMAP_POS			(6)
#define IEEE80211_TX_RX_MCS_NSS_SUPP_RX_BITMAP_POS			(11)
#define IEEE80211_TX_RX_MCS_NSS_SUPP_TX_BITMAP_MASK			0x07c0
#define IEEE80211_TX_RX_MCS_NSS_SUPP_RX_BITMAP_MASK			0xf800


/* TX/RX HE MCS Support field Highest MCS subfield encoding */
enum ieee80211_he_highest_mcs_supported_subfield_enc {
	HIGHEST_MCS_SUPPORTED_MCS7 = 0,
	HIGHEST_MCS_SUPPORTED_MCS8,
	HIGHEST_MCS_SUPPORTED_MCS9,
	HIGHEST_MCS_SUPPORTED_MCS10,
	HIGHEST_MCS_SUPPORTED_MCS11,
};

/* 802.11ax HE PPE Thresholds */
#define IEEE80211_PPE_THRES_NSS_SUPPORT_2NSS			(1)
#define IEEE80211_PPE_THRES_NSS_POS				(0)
#define IEEE80211_PPE_THRES_NSS_MASK				(7)
#define IEEE80211_PPE_THRES_RU_INDEX_BITMASK_2x966_AND_966_RU	\
	(BIT(5) | BIT(6))
#define IEEE80211_PPE_THRES_RU_INDEX_BITMASK_MASK		0x78
#define IEEE80211_PPE_THRES_RU_INDEX_BITMASK_POS		(3)
#define IEEE80211_PPE_THRES_INFO_PPET_SIZE			(3)

/* HE Operation defines */
#define IEEE80211_HE_OPERATION_DFLT_PE_DURATION_MASK		0x00000003
#define IEEE80211_HE_OPERATION_TWT_REQUIRED			0x00000008
#define IEEE80211_HE_OPERATION_RTS_THRESHOLD_MASK		0x00003ff0
#define IEEE80211_HE_OPERATION_RTS_THRESHOLD_OFFSET		4
#define IEEE80211_HE_OPERATION_VHT_OPER_INFO			0x00004000
#define IEEE80211_HE_OPERATION_CO_HOSTED_BSS			0x00008000
#define IEEE80211_HE_OPERATION_ER_SU_DISABLE			0x00010000
#define IEEE80211_HE_OPERATION_6GHZ_OP_INFO			0x00020000
#define IEEE80211_HE_OPERATION_BSS_COLOR_MASK			0x3f000000
#define IEEE80211_HE_OPERATION_BSS_COLOR_OFFSET		24
#define IEEE80211_HE_OPERATION_PARTIAL_BSS_COLOR		0x40000000
#define IEEE80211_HE_OPERATION_BSS_COLOR_DISABLED		0x80000000

/**
 * enum ieee80211_rate_flags - rate flags
 *
 * Hardware/specification flags for rates. These are structured
 * in a way that allows using the same bitrate structure for
 * different bands/PHY modes.
 *
 * @IEEE80211_RATE_SHORT_PREAMBLE: Hardware can send with short
 *	preamble on this bitrate; only relevant in 2.4GHz band and
 *	with CCK rates.
 * @IEEE80211_RATE_MANDATORY_A: This bitrate is a mandatory rate
 *	when used with 802.11a (on the 5 GHz band); filled by the
 *	core code when registering the wiphy.
 * @IEEE80211_RATE_MANDATORY_B: This bitrate is a mandatory rate
 *	when used with 802.11b (on the 2.4 GHz band); filled by the
 *	core code when registering the wiphy.
 * @IEEE80211_RATE_MANDATORY_G: This bitrate is a mandatory rate
 *	when used with 802.11g (on the 2.4 GHz band); filled by the
 *	core code when registering the wiphy.
 * @IEEE80211_RATE_ERP_G: This is an ERP rate in 802.11g mode.
 * @IEEE80211_RATE_SUPPORTS_5MHZ: Rate can be used in 5 MHz mode
 * @IEEE80211_RATE_SUPPORTS_10MHZ: Rate can be used in 10 MHz mode
 */
enum ieee80211_rate_flags {
	IEEE80211_RATE_SHORT_PREAMBLE	= 1 << 0,
	IEEE80211_RATE_MANDATORY_A	= 1 << 1,
	IEEE80211_RATE_MANDATORY_B	= 1 << 2,
	IEEE80211_RATE_MANDATORY_G	= 1 << 3,
	IEEE80211_RATE_ERP_G		= 1 << 4,
	IEEE80211_RATE_SUPPORTS_5MHZ	= 1 << 5,
	IEEE80211_RATE_SUPPORTS_10MHZ	= 1 << 6,
};

/*
 * ieee80211_he_oper_size - calculate 802.11ax HE Operations IE size
 * @he_oper_ie: byte data of the He Operations IE, stating from the the byte
 *	after the ext ID byte. It is assumed that he_oper_ie has at least
 *	sizeof(struct ieee80211_he_operation) bytes, checked already in
 *	ieee802_11_parse_elems_crc()
 * @return the actual size of the IE data (not including header), or 0 on error
 */
#if 0//comments out the unused function to avoid build error
static inline u8
ieee80211_he_oper_size(const u8 *he_oper_ie)
{
	struct ieee80211_he_operation *he_oper = (void *)he_oper_ie;
	u8 oper_len = sizeof(struct ieee80211_he_operation);
	u32 he_oper_params;

	/* Make sure the input is not NULL */
	if (!he_oper_ie)
		return 0;

	/* Calc required length */
	he_oper_params = le32_to_cpu(he_oper->he_oper_params);
	if (he_oper_params & IEEE80211_HE_OPERATION_VHT_OPER_INFO)
		oper_len += 3;
	if (he_oper_params & IEEE80211_HE_OPERATION_CO_HOSTED_BSS)
		oper_len++;
	if (he_oper_params & IEEE80211_HE_OPERATION_6GHZ_OP_INFO)
		oper_len += 4;

	/* Add the first byte (extension ID) to the total length */
	oper_len++;

	return oper_len;
}
#endif

/**
 * struct ieee80211_mcs_info - MCS information
 * @rx_mask: RX mask
 * @rx_highest: highest supported RX rate. If set represents
 *	the highest supported RX data rate in units of 1 Mbps.
 *	If this field is 0 this value should not be used to
 *	consider the highest RX data rate supported.
 * @tx_params: TX parameters
 */
struct ieee80211_mcs_info {
	UINT8 rx_mask[IEEE80211_HT_MCS_MASK_LEN];
	UINT16 rx_highest;
	UINT8 tx_params;
	UINT8 reserved[3];
} __attribute__((packed));

/**
 * struct ieee80211_sta_erp_cap - STA's ERP capabilities
 *
 * This structure describes most essential parameters needed
 * to describe 802.11g ERP capabilities for an STA.
 *
 * @erp_supported: is ERP supported by the STA
 */
struct ieee80211_sta_erp_cap {
	UINT8 erp_supported;
};

/**
 * struct ieee80211_sta_ht_cap - STA's HT capabilities
 *
 * This structure describes most essential parameters needed
 * to describe 802.11n HT capabilities for an STA.
 *
 * @ht_supported: is HT supported by the STA
 * @cap: HT capabilities map as described in 802.11n spec
 * @ampdu_factor: Maximum A-MPDU length factor
 * @ampdu_density: Minimum A-MPDU spacing
 * @mcs: Supported MCS rates
 */
struct ieee80211_sta_ht_cap {
	UINT16 cap; /* use IEEE80211_HT_CAP_ */
	UINT8 ht_supported;
	UINT8 ampdu_factor;
	UINT8 ampdu_density;
	struct ieee80211_mcs_info mcs;
};

/**
 * struct ieee80211_vht_mcs_info - VHT MCS information
 * @rx_mcs_map: RX MCS map 2 bits for each stream, total 8 streams
 * @rx_highest: Indicates highest long GI VHT PPDU data rate
 *	STA can receive. Rate expressed in units of 1 Mbps.
 *	If this field is 0 this value should not be used to
 *	consider the highest RX data rate supported.
 *	The top 3 bits of this field are reserved.
 * @tx_mcs_map: TX MCS map 2 bits for each stream, total 8 streams
 * @tx_highest: Indicates highest long GI VHT PPDU data rate
 *	STA can transmit. Rate expressed in units of 1 Mbps.
 *	If this field is 0 this value should not be used to
 *	consider the highest TX data rate supported.
 *	The top 3 bits of this field are reserved.
 */
struct ieee80211_vht_mcs_info {
	UINT16 rx_mcs_map;
	UINT16 rx_highest;
	UINT16 tx_mcs_map;
	UINT16 tx_highest;
} __attribute__((packed));

/**
 * struct ieee80211_sta_vht_cap - STA's VHT capabilities
 *
 * This structure describes most essential parameters needed
 * to describe 802.11ac VHT capabilities for an STA.
 *
 * @vht_supported: is VHT supported by the STA
 * @cap: VHT capabilities map as described in 802.11ac spec
 * @vht_mcs: Supported VHT MCS rates
 */
struct ieee80211_sta_vht_cap {
	UINT8 vht_supported;
	UINT32 cap; /* use IEEE80211_VHT_CAP_ */
	struct ieee80211_vht_mcs_info vht_mcs;
};

/**
 * struct ieee80211_he_cap_elem - HE capabilities element
 *
 * This structure is the "HE capabilities element" fixed fields as
 * described in P802.11ax_D4.0 section 9.4.2.242.2 and 9.4.2.242.3
 */
struct ieee80211_he_cap_elem {
	u8 mac_cap_info[6];
	u8 phy_cap_info[11];
} __attribute__((packed));;

/* Calculate 802.11ax HE capabilities IE Tx/Rx HE MCS NSS Support Field size */
static inline u8
ieee80211_he_mcs_nss_size(const struct ieee80211_he_cap_elem *he_cap)
{
	u8 count = 4;

	if (he_cap->phy_cap_info[0] &
		IEEE80211_HE_PHY_CAP0_CHANNEL_WIDTH_SET_160MHZ_IN_5G)
		count += 4;

	if (he_cap->phy_cap_info[0] &
		IEEE80211_HE_PHY_CAP0_CHANNEL_WIDTH_SET_80PLUS80_MHZ_IN_5G)
		count += 4;

	return count;
}

#define IEEE80211_TX_RX_MCS_NSS_DESC_MAX_LEN	5

/**
 * enum ieee80211_he_mcs_support - HE MCS support definitions
 * @IEEE80211_HE_MCS_SUPPORT_0_7: MCSes 0-7 are supported for the
 *	number of streams
 * @IEEE80211_HE_MCS_SUPPORT_0_9: MCSes 0-9 are supported
 * @IEEE80211_HE_MCS_SUPPORT_0_11: MCSes 0-11 are supported
 * @IEEE80211_HE_MCS_NOT_SUPPORTED: This number of streams isn't supported
 *
 * These definitions are used in each 2-bit subfield of the rx_mcs_*
 * and tx_mcs_* fields of &struct ieee80211_he_mcs_nss_supp, which are
 * both split into 8 subfields by number of streams. These values indicate
 * which MCSes are supported for the number of streams the value appears
 * for.
 */
enum ieee80211_he_mcs_support {
	IEEE80211_HE_MCS_SUPPORT_0_7	= 0,
	IEEE80211_HE_MCS_SUPPORT_0_9	= 1,
	IEEE80211_HE_MCS_SUPPORT_0_11	= 2,
	IEEE80211_HE_MCS_NOT_SUPPORTED	= 3,
};

/**
 * struct ieee80211_he_mcs_nss_supp - HE Tx/Rx HE MCS NSS Support Field
 *
 * This structure holds the data required for the Tx/Rx HE MCS NSS Support Field
 * described in P802.11ax_D2.0 section 9.4.2.237.4
 *
 * @rx_mcs_80: Rx MCS map 2 bits for each stream, total 8 streams, for channel
 *     widths less than 80MHz.
 * @tx_mcs_80: Tx MCS map 2 bits for each stream, total 8 streams, for channel
 *     widths less than 80MHz.
 * @rx_mcs_160: Rx MCS map 2 bits for each stream, total 8 streams, for channel
 *     width 160MHz.
 * @tx_mcs_160: Tx MCS map 2 bits for each stream, total 8 streams, for channel
 *     width 160MHz.
 * @rx_mcs_80p80: Rx MCS map 2 bits for each stream, total 8 streams, for
 *     channel width 80p80MHz.
 * @tx_mcs_80p80: Tx MCS map 2 bits for each stream, total 8 streams, for
 *     channel width 80p80MHz.
 */
struct ieee80211_he_mcs_nss_supp {
	__le16 rx_mcs_80;
	__le16 tx_mcs_80;
	__le16 rx_mcs_160;
	__le16 tx_mcs_160;
	__le16 rx_mcs_80p80;
	__le16 tx_mcs_80p80;
} __attribute__((packed));

#define IEEE80211_HE_PPE_THRES_MAX_LEN		25

/**
 * struct ieee80211_rate - bitrate definition
 *
 * This structure describes a bitrate that an 802.11 PHY can
 * operate with. The two values @hw_value and @hw_value_short
 * are only for driver use when pointers to this structure are
 * passed around.
 *
 * @flags: rate-specific flags
 * @bitrate: bitrate in units of 100 Kbps
 * @hw_value: driver/hardware value for this rate
 * @hw_value_short: driver/hardware value for this rate when
 *	short preamble is used
 */
struct ieee80211_rate {
	u32 flags;
	u16 bitrate;
	u16 hw_value, hw_value_short;
};

/**
 * struct ieee80211_sta_he_cap - STA's HE capabilities
 *
 * This structure describes most essential parameters needed
 * to describe 802.11ax HE capabilities for a STA.
 *
 * @has_he: true iff HE data is valid.
 * @he_cap_elem: Fixed portion of the HE capabilities element.
 * @he_mcs_nss_supp: The supported NSS/MCS combinations.
 * @ppe_thres: Holds the PPE Thresholds data.
 */
struct ieee80211_sta_he_cap {
	bool has_he;
	struct ieee80211_he_cap_elem he_cap_elem;
	struct ieee80211_he_mcs_nss_supp he_mcs_nss_supp;
	u8 ppe_thres[IEEE80211_HE_PPE_THRES_MAX_LEN];
};

/**
 * struct ieee80211_sband_iftype_data
 *
 * This structure encapsulates sband data that is relevant for the
 * interface types defined in @types_mask.  Each type in the
 * @types_mask must be unique across all instances of iftype_data.
 *
 * @types_mask: interface types mask
 * @he_cap: holds the HE capabilities
 */
struct ieee80211_sband_iftype_data {
	u16 types_mask;
	struct ieee80211_sta_he_cap he_cap;
};


struct ieee80211_supported_band {
	struct ieee80211_channel *channels;
	//struct ieee80211_rate *bitrates;
	wifi_band_t band;
	int n_channels;
	//int n_bitrates;
	struct ieee80211_sta_erp_cap erp_cap;
	struct ieee80211_sta_ht_cap ht_cap;
	struct ieee80211_sta_vht_cap vht_cap;
	u16 n_iftype_data;
	const struct ieee80211_sband_iftype_data *iftype_data;
};

struct regulatory_request;

struct wiphy {
	struct ieee80211_supported_band *bands[IEEE80211_NUM_BANDS];
	/* Supported interface modes, OR together BIT(NL80211_IFTYPE_...) */
	/* Lets us get back the wiphy on the callback */
	int (*reg_notifier)(struct wiphy *wiphy,
			    struct regulatory_request *request);
	u16 interface_modes;
	u16 max_remain_on_channel_duration;
	u32 flags, regulatory_flags, features;
	u8 max_num_csa_counters;
	//u8 ext_features[DIV_ROUND_UP(NUM_NL80211_EXT_FEATURES, 8)];
	struct ieee80211_regdomain *regd;
};

/**
 * enum ieee80211_regulatory_flags - device regulatory flags
 *
 * @REGULATORY_CUSTOM_REG: tells us the driver for this device
 *	has its own custom regulatory domain and cannot identify the
 *	ISO / IEC 3166 alpha2 it belongs to. When this is enabled
 *	we will disregard the first regulatory hint (when the
 *	initiator is %REGDOM_SET_BY_CORE). Drivers that use
 *	wiphy_apply_custom_regulatory() should have this flag set
 *	or the regulatory core will set it for the wiphy.
 *	If you use regulatory_hint() *after* using
 *	wiphy_apply_custom_regulatory() the wireless core will
 *	clear the REGULATORY_CUSTOM_REG for your wiphy as it would be
 *	implied that the device somehow gained knowledge of its region.
 * @REGULATORY_STRICT_REG: tells us that the wiphy for this device
 *	has regulatory domain that it wishes to be considered as the
 *	superset for regulatory rules. After this device gets its regulatory
 *	domain programmed further regulatory hints shall only be considered
 *	for this device to enhance regulatory compliance, forcing the
 *	device to only possibly use subsets of the original regulatory
 *	rules. For example if channel 13 and 14 are disabled by this
 *	device's regulatory domain no user specified regulatory hint which
 *	has these channels enabled would enable them for this wiphy,
 *	the device's original regulatory domain will be trusted as the
 *	base. You can program the superset of regulatory rules for this
 *	wiphy with regulatory_hint() for cards programmed with an
 *	ISO3166-alpha2 country code. wiphys that use regulatory_hint()
 *	will have their wiphy->regd programmed once the regulatory
 *	domain is set, and all other regulatory hints will be ignored
 *	until their own regulatory domain gets programmed.
 * @REGULATORY_DISABLE_BEACON_HINTS: enable this if your driver needs to
 *	ensure that passive scan flags and beaconing flags may not be lifted by
 *	cfg80211 due to regulatory beacon hints. For more information on beacon
 *	hints read the documenation for regulatory_hint_found_beacon()
 * @REGULATORY_COUNTRY_IE_FOLLOW_POWER:  for devices that have a preference
 *	that even though they may have programmed their own custom power
 *	setting prior to wiphy registration, they want to ensure their channel
 *	power settings are updated for this connection with the power settings
 *	derived from the regulatory domain. The regulatory domain used will be
 *	based on the ISO3166-alpha2 from country IE provided through
 *	regulatory_hint_country_ie()
 * @REGULATORY_COUNTRY_IE_IGNORE: for devices that have a preference to ignore
 * 	all country IE information processed by the regulatory core. This will
 * 	override %REGULATORY_COUNTRY_IE_FOLLOW_POWER as all country IEs will
 * 	be ignored.
 * @REGULATORY_ENABLE_RELAX_NO_IR: for devices that wish to allow the
 *      NO_IR relaxation, which enables transmissions on channels on which
 *      otherwise initiating radiation is not allowed. This will enable the
 *      relaxations enabled under the CFG80211_REG_RELAX_NO_IR configuration
 *      option
 * @REGULATORY_IGNORE_STALE_KICKOFF: the regulatory core will _not_ make sure
 *	all interfaces on this wiphy reside on allowed channels. If this flag
 *	is not set, upon a regdomain change, the interfaces are given a grace
 *	period (currently 60 seconds) to disconnect or move to an allowed
 *	channel. Interfaces on forbidden channels are forcibly disconnected.
 *	Currently these types of interfaces are supported for enforcement:
 *	NL80211_IFTYPE_ADHOC, NL80211_IFTYPE_STATION, NL80211_IFTYPE_AP,
 *	NL80211_IFTYPE_AP_VLAN, NL80211_IFTYPE_MONITOR,
 *	NL80211_IFTYPE_P2P_CLIENT, NL80211_IFTYPE_P2P_GO,
 *	NL80211_IFTYPE_P2P_DEVICE. The flag will be set by default if a device
 *	includes any modes unsupported for enforcement checking.
 * @REGULATORY_WIPHY_SELF_MANAGED: for devices that employ wiphy-specific
 *	regdom management. These devices will ignore all regdom changes not
 *	originating from their own wiphy.
 *	A self-managed wiphys only employs regulatory information obtained from
 *	the FW and driver and does not use other cfg80211 sources like
 *	beacon-hints, country-code IEs and hints from other devices on the same
 *	system. Conversely, a self-managed wiphy does not share its regulatory
 *	hints with other devices in the system. If a system contains several
 *	devices, one or more of which are self-managed, there might be
 *	contradictory regulatory settings between them. Usage of flag is
 *	generally discouraged. Only use it if the FW/driver is incompatible
 *	with non-locally originated hints.
 *	This flag is incompatible with the flags: %REGULATORY_CUSTOM_REG,
 *	%REGULATORY_STRICT_REG, %REGULATORY_COUNTRY_IE_FOLLOW_POWER,
 *	%REGULATORY_COUNTRY_IE_IGNORE and %REGULATORY_DISABLE_BEACON_HINTS.
 *	Mixing any of the above flags with this flag will result in a failure
 *	to register the wiphy. This flag implies
 *	%REGULATORY_DISABLE_BEACON_HINTS and %REGULATORY_COUNTRY_IE_IGNORE.
 */
enum ieee80211_regulatory_flags {
	REGULATORY_CUSTOM_REG			= BIT(0),
	REGULATORY_STRICT_REG			= BIT(1),
	REGULATORY_DISABLE_BEACON_HINTS		= BIT(2),
	REGULATORY_COUNTRY_IE_FOLLOW_POWER	= BIT(3),
	REGULATORY_COUNTRY_IE_IGNORE		= BIT(4),
	REGULATORY_ENABLE_RELAX_NO_IR           = BIT(5),
	REGULATORY_IGNORE_STALE_KICKOFF         = BIT(6),
	REGULATORY_WIPHY_SELF_MANAGED		= BIT(7),
};

/**
 * enum nl80211_dfs_regions - regulatory DFS regions
 *
 * @NL80211_DFS_UNSET: Country has no DFS master region specified
 * @NL80211_DFS_FCC: Country follows DFS master rules from FCC
 * @NL80211_DFS_ETSI: Country follows DFS master rules from ETSI
 * @NL80211_DFS_JP: Country follows DFS master rules from JP/MKK/Telec
 */
enum nl80211_dfs_regions {
	NL80211_DFS_UNSET	= 0,
	NL80211_DFS_FCC		= 1,
	NL80211_DFS_ETSI	= 2,
	NL80211_DFS_JP		= 3,
};

/**
 * enum nl80211_reg_type - specifies the type of regulatory domain
 * @NL80211_REGDOM_TYPE_COUNTRY: the regulatory domain set is one that pertains
 *	to a specific country. When this is set you can count on the
 *	ISO / IEC 3166 alpha2 country code being valid.
 * @NL80211_REGDOM_TYPE_WORLD: the regulatory set domain is the world regulatory
 * 	domain.
 * @NL80211_REGDOM_TYPE_CUSTOM_WORLD: the regulatory domain set is a custom
 * 	driver specific world regulatory domain. These do not apply system-wide
 * 	and are only applicable to the individual devices which have requested
 * 	them to be applied.
 * @NL80211_REGDOM_TYPE_INTERSECTION: the regulatory domain set is the product
 *	of an intersection between two regulatory domains -- the previously
 *	set regulatory domain on the system and the last accepted regulatory
 *	domain request to be processed.
 */
enum nl80211_reg_type {
	NL80211_REGDOM_TYPE_COUNTRY,
	NL80211_REGDOM_TYPE_WORLD,
	NL80211_REGDOM_TYPE_CUSTOM_WORLD,
	NL80211_REGDOM_TYPE_INTERSECTION,
};

/**
 * enum environment_cap - Environment parsed from country IE
 * @ENVIRON_ANY: indicates country IE applies to both indoor and
 *	outdoor operation.
 * @ENVIRON_INDOOR: indicates country IE applies only to indoor operation
 * @ENVIRON_OUTDOOR: indicates country IE applies only to outdoor operation
 */
enum environment_cap {
	ENVIRON_ANY,
	ENVIRON_INDOOR,
	ENVIRON_OUTDOOR,
};

/**
 * enum nl80211_initiator - Indicates the initiator of a reg domain request
 * @NL80211_REGDOM_SET_BY_CORE: Core queried CRDA for a dynamic world
 * 	regulatory domain.
 * @NL80211_REGDOM_SET_BY_USER: User asked the wireless core to set the
 * 	regulatory domain.
 * @NL80211_REGDOM_SET_BY_DRIVER: a wireless drivers has hinted to the
 * 	wireless core it thinks its knows the regulatory domain we should be in.
 * @NL80211_REGDOM_SET_BY_COUNTRY_IE: the wireless core has received an
 * 	802.11 country information element with regulatory information it
 * 	thinks we should consider. cfg80211 only processes the country
 *	code from the IE, and relies on the regulatory domain information
 *	structure passed by userspace (CRDA) from our wireless-regdb.
 *	If a channel is enabled but the country code indicates it should
 *	be disabled we disable the channel and re-enable it upon disassociation.
 */
enum nl80211_reg_initiator {
	NL80211_REGDOM_SET_BY_CORE,
	NL80211_REGDOM_SET_BY_USER,
	NL80211_REGDOM_SET_BY_DRIVER,
	NL80211_REGDOM_SET_BY_COUNTRY_IE,
};

/**
 * enum nl80211_user_reg_hint_type - type of user regulatory hint
 *
 * @NL80211_USER_REG_HINT_USER: a user sent the hint. This is always
 *	assumed if the attribute is not set.
 * @NL80211_USER_REG_HINT_CELL_BASE: the hint comes from a cellular
 *	base station. Device drivers that have been tested to work
 *	properly to support this type of hint can enable these hints
 *	by setting the NL80211_FEATURE_CELL_BASE_REG_HINTS feature
 *	capability on the struct wiphy. The wireless core will
 *	ignore all cell base station hints until at least one device
 *	present has been registered with the wireless core that
 *	has listed NL80211_FEATURE_CELL_BASE_REG_HINTS as a
 *	supported feature.
 * @NL80211_USER_REG_HINT_INDOOR: a user sent an hint indicating that the
 *	platform is operating in an indoor environment.
 */
enum nl80211_user_reg_hint_type {
	NL80211_USER_REG_HINT_USER	= 0,
	NL80211_USER_REG_HINT_CELL_BASE = 1,
	NL80211_USER_REG_HINT_INDOOR    = 2,
};


/**
 * struct regulatory_request - used to keep track of regulatory requests
 *
 * @wiphy_idx: this is set if this request's initiator is
 * 	%REGDOM_SET_BY_COUNTRY_IE or %REGDOM_SET_BY_DRIVER. This
 * 	can be used by the wireless core to deal with conflicts
 * 	and potentially inform users of which devices specifically
 * 	cased the conflicts.
 * @initiator: indicates who sent this request, could be any of
 * 	of those set in nl80211_reg_initiator (%NL80211_REGDOM_SET_BY_*)
 * @alpha2: the ISO / IEC 3166 alpha2 country code of the requested
 * 	regulatory domain. We have a few special codes:
 * 	00 - World regulatory domain
 * 	99 - built by driver but a specific alpha2 cannot be determined
 * 	98 - result of an intersection between two regulatory domains
 *	97 - regulatory domain has not yet been configured
 * @dfs_region: If CRDA responded with a regulatory domain that requires
 *	DFS master operation on a known DFS region (NL80211_DFS_*),
 *	dfs_region represents that region. Drivers can use this and the
 *	@alpha2 to adjust their device's DFS parameters as required.
 * @user_reg_hint_type: if the @initiator was of type
 *	%NL80211_REGDOM_SET_BY_USER, this classifies the type
 *	of hint passed. This could be any of the %NL80211_USER_REG_HINT_*
 *	types.
 * @intersect: indicates whether the wireless core should intersect
 * 	the requested regulatory domain with the presently set regulatory
 * 	domain.
 * @processed: indicates whether or not this requests has already been
 *	processed. When the last request is processed it means that the
 *	currently regulatory domain set on cfg80211 is updated from
 *	CRDA and can be used by other regulatory requests. When a
 *	the last request is not yet processed we must yield until it
 *	is processed before processing any new requests.
 * @country_ie_checksum: checksum of the last processed and accepted
 * 	country IE
 * @country_ie_env: lets us know if the AP is telling us we are outdoor,
 * 	indoor, or if it doesn't matter
 * @list: used to insert into the reg_requests_list linked list
 */
struct regulatory_request {
	int wiphy_idx;
	enum nl80211_reg_initiator initiator;
	enum nl80211_user_reg_hint_type user_reg_hint_type;
	char alpha2[2];
	u8 dfs_region;
	bool intersect;
	bool processed;
	enum environment_cap country_ie_env;
	LIST_HEADER_T list;
};

struct ieee80211_freq_range {
	u32 start_freq_khz;
	u32 end_freq_khz;
	u32 max_bandwidth_khz;
};

struct ieee80211_power_rule {
	u32 max_antenna_gain;
	u32 max_eirp;
};

/**
 * struct ieee80211_wmm_ac - used to store per ac wmm regulatory limitation
 *
 * The information provided in this structure is required for QoS
 * transmit queue configuration. Cf. IEEE 802.11 7.3.2.29.
 *
 * @cw_min: minimum contention window [a value of the form
 *      2^n-1 in the range 1..32767]
 * @cw_max: maximum contention window [like @cw_min]
 * @cot: maximum burst time in units of 32 usecs, 0 meaning disabled
 * @aifsn: arbitration interframe space [0..255]
 *
 */
struct ieee80211_wmm_ac {
	u16 cw_min;
	u16 cw_max;
	u16 cot;
	u8 aifsn;
};

struct ieee80211_wmm_rule {
	struct ieee80211_wmm_ac client[IEEE80211_NUM_ACS];
	struct ieee80211_wmm_ac ap[IEEE80211_NUM_ACS];
};


struct ieee80211_reg_rule {
	struct ieee80211_freq_range freq_range;
	struct ieee80211_power_rule power_rule;
#if CONFIG_WIFI_REGDOMAIN_WMM
	struct ieee80211_wmm_rule wmm_rule;
#endif
	u32 flags;
	u32 dfs_cac_ms;
	bool has_wmm;
};

struct ieee80211_regdomain {
	u32 n_reg_rules;
	char alpha2[3];
	enum nl80211_dfs_regions dfs_region;
	struct ieee80211_reg_rule reg_rules[];
};

#define REG_RULE_EXT(start, end, bw, gain, eirp, dfs_cac, reg_flags)	\
{									\
	.freq_range.start_freq_khz = MHZ_TO_KHZ(start),			\
	.freq_range.end_freq_khz = MHZ_TO_KHZ(end),			\
	.freq_range.max_bandwidth_khz = MHZ_TO_KHZ(bw),			\
	.power_rule.max_antenna_gain = DBI_TO_MBI(gain),		\
	.power_rule.max_eirp = DBM_TO_MBM(eirp),			\
	.flags = reg_flags,						\
	.dfs_cac_ms = dfs_cac,						\
}

#define REG_RULE(start, end, bw, gain, eirp, reg_flags) \
	REG_RULE_EXT(start, end, bw, gain, eirp, 0, reg_flags)

extern struct wiphy g_wiphy;

/*
 * You can use this to mark a wiphy_idx as not having an associated wiphy.
 * It guarantees cfg80211_rdev_by_wiphy_idx(wiphy_idx) will return NULL
 */
#define WIPHY_IDX_INVALID -1

static inline struct wiphy *wiphy_idx_to_wiphy(int index)
{
	if (index == WIPHY_IDX_INVALID)
		return NULL;
	return &g_wiphy;
}

/* Note 0 is valid, hence phy0 */
static inline bool wiphy_idx_valid(int wiphy_idx)
{
	return wiphy_idx >= 0;
}

static inline const char *wiphy_name(const struct wiphy *wiphy)
{
	return "phy0";
}

/**
 * ieee80211_channel_to_khz - convert ieee80211_channel to frequency in KHz
 * @chan: struct ieee80211_channel to convert
 * Return: The corresponding frequency (in KHz)
 */
static inline u32
ieee80211_channel_to_khz(const struct ieee80211_channel *chan)
{
	return MHZ_TO_KHZ(chan->center_freq) + chan->freq_offset;
}

/**
 * ieee80211_channel_equal - compare two struct ieee80211_channel
 *
 * @a: 1st struct ieee80211_channel
 * @b: 2nd struct ieee80211_channel
 * Return: true if center frequency of @a == @b
 */
static inline bool
ieee80211_channel_equal(struct ieee80211_channel *a,
			struct ieee80211_channel *b)
{
	return (a->center_freq == b->center_freq &&
		a->freq_offset == b->freq_offset);
}

/* Although the spec says 8 I'm seeing 6 in practice */
#define IEEE80211_COUNTRY_IE_MIN_LEN	6

/* The Country String field of the element shall be 3 octets in length */
#define IEEE80211_COUNTRY_STRING_LEN	3

void rwnx_hw_reinit(void);
uint64_t rwnx_hw_mm_features(void);

bool country_code_policy_is_auto(void);
UINT8 rw_ieee80211_get_scan_default_chan_num(void);
bool check_non_radar_channel_available(int chan);
int get_wiphy_idx(struct wiphy *wiphy);
int set_regdom(const struct ieee80211_regdomain *rd);
int ieee80211_channel_to_frequency(int chan, wifi_band_t band);
int ieee80211_frequency_to_channel(int freq);
uint16_t get_chan_flags(uint32_t flags);
void regulatory_hint_11d(struct wiphy *wiphy, wifi_band_t band, const u8 *country_ie, u8 country_ie_len);
int regulatory_hint_found_beacon(struct wiphy *wiphy, struct ieee80211_channel *beacon_chan);
struct ieee80211_channel *ieee80211_get_channel(struct wiphy *wiphy, int freq);
void rwnx_regulatory_hint_11d(int freq, const u8 *country_ie, u8 country_ie_len);
int ieee80211_freq_khz_to_channel(u32 freq);
const char *regdom_flag_str(uint32_t channel_flags);
enum nl80211_dfs_regions reg_get_dfs_region(struct wiphy *wiphy);
bool rwnx_ieee80211_check_conn_instrument(const struct mac_ssid *ssid, const struct mac_addr *bssid);
#endif // _RW_IEEE80211_H_
// eof

