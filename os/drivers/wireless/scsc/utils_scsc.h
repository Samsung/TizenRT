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

#ifndef SLSI_UTILS_SCSC_H__
#define SLSI_UTILS_SCSC_H__
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <sys/types.h>
#include <tinyara/wqueue.h>

#ifdef __cplusplus
extern "C" {
#endif

#define WARN(condition, format, ...) ({                               \
	int __ret_warn_on = !!(condition);                            \
	if (__ret_warn_on)                                            \
		lldbg("WARNING: at %s:%d %s() " format,               \
			__FILE__, __LINE__, __func__, ##__VA_ARGS__); \
	__ret_warn_on;                                                \
	})

#define WARN_ON(condition) WARN(condition, "\n")

#ifdef __CHECKER__
#define __bitwise       __attribute__((__bitwise__))
#else
#define __bitwise
#endif

#define STRUCT_PACKED __attribute__ ((__packed__))

#ifndef __packed
#define __packed __attribute__((__packed__))
#endif

#define __force         __attribute__((force))
#define __aligned(x)    __attribute__((aligned(x)))

typedef u16 __bitwise le16;
typedef u32 __bitwise le32;
typedef u64 __bitwise le64;
typedef u16 __bitwise be16;
typedef u32 __bitwise be32;

typedef unsigned char __u8;
typedef unsigned short __u16;
#ifndef CONFIG_ENABLE_IOTIVITY
typedef unsigned int __u32;
#endif

#define BIT(nr)            (1 << (nr))
#define BITS_PER_BYTE        8
#define BIT_MASK(nr)        (1 << ((nr) % BITS_PER_BYTE))
#define BIT_BYTE(nr)        ((nr) / BITS_PER_BYTE)

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
static inline u16 bswap_16(u16 x)
{
	return ((x & 0xff) << 8) | (x >> 8);
}
static inline u32 bswap_32(u32 x)
{
	return ((x & 0xff) << 24) | ((x & 0xff00) << 8) | ((x & 0xff0000) >> 8) | (x >> 24);
}

#define cpu_to_le16(x) ((le16)(u16)(x))
#define cpu_to_le32(x) ((le32)(u32)(x))
#define cpu_to_be16(x) ((be16)bswap_16(x))
#define cpu_to_be32(x) ((be32)bswap_32(x))

#define le16_to_cpu(x) ((u16)(le16)(x))
#define le32_to_cpu(x) ((u32)(le32)(x))
#define be16_to_cpu(x) bswap_16((u16)(be16)(x))
#define be32_to_cpu(x) bswap_32((u32)(be32)(x))

/* MAC Address Length */
#define ETH_ALEN        6
#define ETH_HLEN 14
#define ETH_ZLEN        60		/* Min. octets in frame sans FCS */
#define ETH_DATA_LEN    1500	/* Max. octets in payload        */

#define SLSI_MUTEX_INIT(mutex__)                                                 \
	do {                                                                     \
		int __ret;                                                       \
		__ret = pthread_mutex_init(&(mutex__), NULL);                    \
		WARN(__ret, "Failed to initialize the mutex (ret:%d)\n", __ret); \
	} while (0)

#define SLSI_MUTEX_LOCK(mutex_to_lock)                                           \
	do {                                                                     \
		int __ret;                                                       \
		while ((__ret = pthread_mutex_lock(&(mutex_to_lock))) != OK) {   \
			if (__ret != EINTR) {                                    \
				WARN(__ret, "Failed to lock the mutex (ret:%d)\n", __ret);       \
				break;                                           \
			}                                                        \
		}										\
	} while (0)

#define SLSI_MUTEX_UNLOCK(mutex_to_unlock)                                       \
	do {                                                                     \
		int __ret;                                                       \
		__ret = pthread_mutex_unlock(&(mutex_to_unlock));                \
		WARN(__ret, "Failed to unlock the mutex (ret:%d)\n", __ret);     \
	} while (0)

#define SLSI_MUTEX_IS_LOCKED(mutex__)      (!(mutex__.sem.semcount == 1))

#define SLSI_NETIF_MBUF_HEADROOM (68 + 160)	/* sizeof ma_unitdata_req [36] + pad [30] + pad_words [2]  */
#define SLSI_NETIF_MBUF_TAILROOM 0

#define WLAN_REASON_DEAUTH_LEAVING 3
#define WLAN_REASON_DISASSOC_DUE_TO_INACTIVITY 4
#define WLAN_REASON_DISASSOC_STA_HAS_LEFT 8

#define mbuf_queue_walk(queue, mbuf) \
	for (mbuf = (queue)->next;                                      \
	     mbuf != (struct max_buff *)(queue);                    \
	     mbuf = mbuf->next)

static inline void ether_addr_copy(u8 *dst, const u8 *src)
{
	u16 *a = (u16 *) dst;
	const u16 *b = (const u16 *)src;

	a[0] = b[0];
	a[1] = b[1];
	a[2] = b[2];
}
#define SLSI_ETHER_COPY(dst, src)       ether_addr_copy((dst), (src))

#define SLSI_MAC_LEN	(6)
#define SLSI_MAC_FORMAT "%02x:%02x:%02x:%02x:%02x:%02x"
#define SLSI_MAC_STR(x) (x)[0], (x)[1], (x)[2], (x)[3], (x)[4], (x)[5]

/* Ethernet frame header */
struct ethhdr {
	unsigned char h_dest[ETH_ALEN];	/* destination eth addr */
	unsigned char h_source[ETH_ALEN];	/* source ether addr    */
	be16 h_proto;			/* packet type ID field */
} __attribute__((packed));

/* Get the Compiler to ignore Unused parameters */
#define SLSI_UNUSED_PARAMETER(x) ((void)(x))

/* Helper ERROR Macros */
#define SLSI_ECR(func) \
	do { \
		int _err = (func); \
		if (_err != 0) { \
			SLSI_ERR_NODEV("e=%d\n", _err); \
			return _err; \
		} \
	} while (0)

#define SLSI_EC(func) \
	do { \
		int _err = (func); \
		if (_err != 0) { \
			SLSI_ERR_NODEV("e=%d\n", _err); \
			return; \
		} \
	} while (0)

#define SLSI_EC_GOTO(func, err, label) \
	do { \
		(err) = func; \
		if ((err) != 0) { \
			WARN_ON(1); \
			SLSI_ERR(sdev, "fail at line:%d\n", __LINE__); \
			goto label; \
		} \
	} while (0)

/*------------------------------------------------------------------*/
/* Endian conversion */
/*------------------------------------------------------------------*/
#define SLSI_BUFF_LE_TO_U16(ptr)        (((u16)((u8 *)(ptr))[0]) | ((u16)((u8 *)(ptr))[1]) << 8)
#define SLSI_U16_TO_BUFF_LE(uint, ptr) \
	do { \
		u32 local_uint_tmp = (uint); \
		((u8 *)(ptr))[0] = ((u8)((local_uint_tmp & 0x00FF))); \
		((u8 *)(ptr))[1] = ((u8)(local_uint_tmp >> 8)); \
	} while (0)

#define SLSI_U32_TO_BUFF_LE(uint, ptr) ((*(u32 *)ptr) = cpu_to_le32(uint))

#define SLSI_BUFF_LE_TO_U16_P(output, input) \
	do { \
		(output) = (u16)((((u16)(input)[1]) << 8) | ((u16)(input)[0])); \
		(input) += 2; \
	} while (0)

#define SLSI_BUFF_LE_TO_U32_P(output, input) \
	do { \
		(output) = le32_to_cpu(*(u32 *)input); \
		(input) += 4; \
	} while (0)

#define SLSI_U16_TO_BUFF_LE_P(output, input) \
	do { \
		(output)[0] = ((u8)((input) & 0x00FF));  \
		(output)[1] = ((u8)((input) >> 8)); \
		(output) += 2; \
	} while (0)

#define SLSI_U32_TO_BUFF_LE_P(output, input) \
	do { \
		(*(u32 *)output) = cpu_to_le32(input); \
		(output) += 4; \
	} while (0)

/*------------------------------------------------------------------*/
/* IEEE 802.11 Frame and IE related fields */
/*------------------------------------------------------------------*/

/* Frame Control Fields and Flags related masks */
#define SLSI_WLAN_FRM_CTRL_TYPE			(0x000C)
#define SLSI_WLAN_FRM_CTRL_SUB_TYPE		(0x00F0)
#define SLSI_WLAN_FRM_CTRL_TYPE_MGMT		(0x0000)

#define SLSI_WLAN_FRM_CTRL_TYPE_SUB_TYPE	(SLSI_WLAN_FRM_CTRL_TYPE | SLSI_WLAN_FRM_CTRL_SUB_TYPE)

#define SLSI_WLAN_FRM_CTRL_TO_DS		(0x0100)

/* Capability Information Field */
#define SLSI_WLAN_CAPAB_INFO_PRIVACY		BIT(4)

/* Management Frames */
#define SLSI_MGMT_SUB_TYPE_ASSOC_REQ		(0x0000)
#define SLSI_MGMT_SUB_TYPE_ASSOC_RSP		(0x0010)
#define SLSI_MGMT_SUB_TYPE_REASSOC_REQ		(0x0020)
#define SLSI_MGMT_SUB_TYPE_REASSOC_RSP		(0x0030)
#define SLSI_MGMT_SUB_TYPE_PROBE_REQ		(0x0040)
#define SLSI_MGMT_SUB_TYPE_PROBE_RSP		(0x0050)
#define SLSI_MGMT_SUB_TYPE_BEACON		(0x0080)
#define SLSI_MGMT_SUB_TYPE_DISASSOC		(0x00A0)
#define SLSI_MGMT_SUB_TYPE_AUTH			(0x00B0)
#define SLSI_MGMT_SUB_TYPE_DEAUTH		(0x00C0)
#define SLSI_MGMT_SUB_TYPE_ACTION		(0x00D0)

/* WLAN Element IDs */
#define SLSI_WLAN_EID_SSID			(0)
#define SLSI_WLAN_EID_DS_PARAMS			(3)
#define SLSI_WLAN_EID_COUNTRY			(7)
#define SLSI_WLAN_EID_HT_CAPAB			(45)
#define SLSI_WLAN_EID_RSN			(48)
#define SLSI_WLAN_EID_VHT_CAPAB			(191)
#define SLSI_WLAN_EID_VHT_OPER			(192)
#define SLSI_WLAN_EID_VENDOR_SPEC		(221)

/* HT Capabilities Info Mask */
#define SLSI_WLAN_HT_CAPAB_SUP_WIDTH_20_40	(0x0002)
#define SLSI_WLAN_HT_CAPAB_SGI_20		(0x0020)
#define SLSI_WLAN_HT_CAPAB_SGI_40		(0x0040)
#define SLSI_WLAN_HT_CAPAB_RX_STBC		(0x0300)

/* Maximum A-MPDU Length Exponent */
#define SLSI_WLAN_HT_MAX_AMPDU_64K		(3)	/* 2 ^ (13 + 3) - 1 ~ 64 KB */

/* Minimum MPDU start spacing */
#define SLSI_WLAN_HT_MPDU_MIN_SPACING_16	(7)	/* 16 usec */

/* HT Operation Info - Secondary Channel Offset */
#define SLSI_WLAN_HT_OPER_SEC_CHAN_NONE		(0x00)
#define SLSI_WLAN_HT_OPER_SEC_CHAN_ABOVE	(0x01)
#define SLSI_WLAN_HT_OPER_SEC_CHAN_BELOW	(0x03)

/* HT Operation Info - STA Channel Width */
#define SLSI_WLAN_HT_OPER_CHAN_WIDTH_ANY	BIT(2)

/* Cipher suite type */
#define SLSI_WLAN_CIPHER_SUITE_WEP_40		(0x000FAC01)
#define SLSI_WLAN_CIPHER_SUITE_TKIP		(0x000FAC02)
#define SLSI_WLAN_CIPHER_SUITE_CCMP		(0x000FAC04)
#define SLSI_WLAN_CIPHER_SUITE_WEP_104		(0x000FAC05)
#define SLSI_WLAN_CIPHER_SUITE_AES_CMAC		(0x000FAC06)
#define SLSI_WLAN_CIPHER_SUITE_GCMP		(0x000FAC08)
#define SLSI_WLAN_CIPHER_SUITE_SMS4		(0x00147201)	/* WAPI */

/* Vendor Specific OUI */
#define SLSI_WLAN_VS_OUI_MICROSOFT		(0x0050F2)
#define SLSI_WLAN_VS_OUI_WFA			(0x506F9A)

/* Vendor Specific MICROSOFT OUI Type */
#define SLSI_WLAN_VS_OUI_TYPE_MS_WPA		(1)
#define SLSI_WLAN_VS_OUI_TYPE_MS_WMM		(2)
#define SLSI_WLAN_VS_OUI_TYPE_MS_WPS		(4)

/* Vendor Specific WFA OUI Type */
#define SLSI_WLAN_VS_OUI_TYPE_WFA_P2P		(9)

/* Supplicant Protocol Version */
#define SLSI_WPA_PROTOCOL_WPA			BIT(0)
#define SLSI_WPA_PROTOCOL_RSN			BIT(1)

/* Supplicant Cipher Suite */
#define SLSI_WPA_CIPHER_WEP_40			BIT(1)
#define SLSI_WPA_CIPHER_WEP_104			BIT(2)

/* Conversions */
#define SLSI_MHZ_TO_KHZ(freq)			((freq) * 1000)
#define SLSI_DBI_TO_MBI(gain)			((gain) * 100)
#define SLSI_DBM_TO_MBM(gain)			((gain) * 100)

/* Others */
#define SLSI_WLAN_MAX_SSID_LEN			(32)
#define SLSI_WLAN_HEADER_LENGTH			(24)
#define SLSI_WLAN_MIN_IE_LENGTH			(2)		/* EID(1) + LEN(1) field*/

/* Regulatory Rule Flags */
#define SLSI_REG_RULE_FLAG_NO_OFDM		BIT(0)
#define SLSI_REG_RULE_FLAG_NO_CCK		BIT(1)
#define SLSI_REG_RULE_FLAG_NO_INDOOR		BIT(2)
#define SLSI_REG_RULE_FLAG_NO_OUTDOOR		BIT(3)
#define SLSI_REG_RULE_FLAG_DFS			BIT(4)
#define SLSI_REG_RULE_FLAG_NO_IR		BIT(7)
#define SLSI_REG_RULE_FLAG_PASSIVE_SCAN		SLSI_REG_RULE_FLAG_NO_IR
#define SLSI_REG_RULE_FLAG_NO_IBSS		SLSI_REG_RULE_FLAG_NO_IR

#define SLSI_REG_RULE(start, end, bw, gain, eirp, reg_flags)    \
{								\
	.start_freq_khz = SLSI_MHZ_TO_KHZ(start),		\
	.end_freq_khz = SLSI_MHZ_TO_KHZ(end),			\
	.max_bandwidth_khz = SLSI_MHZ_TO_KHZ(bw),		\
	.max_antenna_gain = SLSI_DBI_TO_MBI(gain),		\
	.max_eirp = SLSI_DBM_TO_MBM(eirp),			\
	.flags = reg_flags,					\
}

/* Management Frame check helper functions and macros */
static inline bool slsi_is_expected_mgmt_frame(le16 frame_ctrl, u16 exp_sub_type)
{
	return ((frame_ctrl & cpu_to_le16(SLSI_WLAN_FRM_CTRL_TYPE_SUB_TYPE)) == cpu_to_le16(exp_sub_type));
}

#define SLSI_IS_MGMT_FRAME_ASSOC_REQ(frame_ctrl)	slsi_is_expected_mgmt_frame(frame_ctrl, SLSI_MGMT_SUB_TYPE_ASSOC_REQ)
#define SLSI_IS_MGMT_FRAME_ASSOC_RSP(frame_ctrl)	slsi_is_expected_mgmt_frame(frame_ctrl, SLSI_MGMT_SUB_TYPE_ASSOC_RSP)
#define SLSI_IS_MGMT_FRAME_REASSOC_REQ(frame_ctrl)	slsi_is_expected_mgmt_frame(frame_ctrl, SLSI_MGMT_SUB_TYPE_REASSOC_REQ)
#define SLSI_IS_MGMT_FRAME_REASSOC_RSP(frame_ctrl)	slsi_is_expected_mgmt_frame(frame_ctrl, SLSI_MGMT_SUB_TYPE_REASSOC_RSP)
#define SLSI_IS_MGMT_FRAME_PROBE_REQ(frame_ctrl)	slsi_is_expected_mgmt_frame(frame_ctrl, SLSI_MGMT_SUB_TYPE_PROBE_REQ)
#define SLSI_IS_MGMT_FRAME_PROBE_RSP(frame_ctrl)	slsi_is_expected_mgmt_frame(frame_ctrl, SLSI_MGMT_SUB_TYPE_PROBE_RSP)
#define SLSI_IS_MGMT_FRAME_ACTION(frame_ctrl)		slsi_is_expected_mgmt_frame(frame_ctrl, SLSI_MGMT_SUB_TYPE_ACTION)

/*------------------------------------------------------------------*/
/* Enums */
/*------------------------------------------------------------------*/
enum slsi_wlan_band {
	SLSI_WLAN_BAND_2GHZ,
	SLSI_WLAN_BAND_5GHZ,
};

enum slsi_wlan_auth_type {
	SLSI_WLAN_AUTHTYPE_OPEN_SYSTEM,
	SLSI_WLAN_AUTHTYPE_SHARED_KEY,
	SLSI_WLAN_AUTHTYPE_FT,
	SLSI_WLAN_AUTHTYPE_AUTOMATIC,
	SLSI_WLAN_AUTHTYPE_NETWORK_EAP,
	SLSI_WLAN_AUTHTYPE_SAE,
	SLSI_WLAN_AUTHTYPE_UNSUPPORTED,
};

enum slsi_wlan_iftype {
	SLSI_WLAN_IFTYPE_UNSPECIFIED,
	SLSI_WLAN_IFTYPE_ADHOC,
	SLSI_WLAN_IFTYPE_STATION,
	SLSI_WLAN_IFTYPE_AP,
	SLSI_WLAN_IFTYPE_P2P_CLIENT,
	SLSI_WLAN_IFTYPE_P2P_GO,
	SLSI_WLAN_IFTYPE_P2P_GROUP,
};

enum slsi_wlan_channel_flags {
	SLSI_WLAN_CHAN_DISABLED = 1 << 0,
	SLSI_WLAN_CHAN_NO_IR	= 1 << 1,
	SLSI_WLAN_CHAN_RADAR	= 1 << 2,
};

enum slsi_wpa_alg {
	SLSI_WPA_ALG_NONE,
	SLSI_WPA_ALG_WEP,
	SLSI_WPA_ALG_TKIP,
	SLSI_WPA_ALG_CCMP,
	SLSI_WPA_ALG_IGTK,
	SLSI_WPA_ALG_PMK,
	SLSI_WPA_ALG_GCMP,
	SLSI_WPA_ALG_SMS4
};

/*------------------------------------------------------------------*/
/* Data Structures */
/*------------------------------------------------------------------*/
struct slsi_dev;

struct slsi_key_params {
	const u8 *key;
	const u8 *seq;
	int key_len;
	int seq_len;
	u32 cipher;
};

struct slsi_wlan_reg_rule {
	u32 start_freq_khz;
	u32 end_freq_khz;
	u32 max_bandwidth_khz;
	u32 max_antenna_gain;
	u32 max_eirp;
	u32 flags;
};

struct slsi_wlan_channel {
	enum slsi_wlan_band band;
	u16 center_freq;
	u16 hw_value;
	u32 flags;
} STRUCT_PACKED;

struct slsi_wlan_rate {
	u32 flags;
	u16 bitrate;
	u16 hw_value;
	u16 hw_value_short;
} STRUCT_PACKED;

struct slsi_wlan_frame_header {
	le16 frame_ctrl;
	le16 duration;
	u8 addr_1[SLSI_MAC_LEN];
	u8 addr_2[SLSI_MAC_LEN];
	u8 addr_3[SLSI_MAC_LEN];
	le16 sequence_ctrl;
	/* Other data follows as appropriate */
} STRUCT_PACKED;

struct slsi_wlan_mcs_info {
	le16 rx_highest;
	u8 rx_mask[10];
	u8 tx_params;
} STRUCT_PACKED;

struct slsi_wlan_sta_ht_cap {
	bool ht_supported;
	u16 cap;
	struct slsi_wlan_mcs_info mcs;
	u8 ampdu_factor;
	u8 ampdu_density;
} STRUCT_PACKED;

struct slsi_wlan_supported_band {
	struct slsi_wlan_channel *channels;
	struct slsi_wlan_rate *bitrates;
	enum slsi_wlan_band band;
	int n_channels;
	int n_bitrates;
	struct slsi_wlan_sta_ht_cap ht_cap;
} STRUCT_PACKED;

/*------------------------------------------------------------------*/
/* Helper Functions */
/*------------------------------------------------------------------*/
bool slsi_is_broadcast_addr(const u8 *a);
bool slsi_is_mgmt_frame(le16 frame_ctrl);
u16 slsi_wlan_channel_to_freq(int chan);
u32 slsi_get_center_freq1(struct slsi_dev *sdev, u16 chann_info, u16 center_freq);
const u8 *slsi_wlan_find_ie(u8 eid, const u8 *ies, int len);
u8 *slsi_wlan_find_ie_mod(u8 eid, u8 *ies, int len);
const u8 *slsi_wlan_find_vendor_spec_ie(u32 oui, u8 oui_type, const u8 *ies, int len);
u8 *slsi_wlan_get_dest_addr(struct slsi_wlan_frame_header *hdr);

#ifdef CONFIG_SLSI_WLAN_FAPI_LOG
extern void slsi_wlan_fapi_log_init(void);
extern void slsi_wlan_fapi_log_add_record(u16 signal_id, struct timespec ts);
#endif

#ifdef CONFIG_SLSI_WLAN_UDI
bool udi_enabled;
bool skip_scan_ind;
void slsi_wlan_set_udi_flag(bool udi, bool scan_ind);
extern void slsi_wlan_udi_log_data(struct timespec ts, uint32_t signal_length, unsigned char *log_data, size_t len);
#endif

#ifdef __cplusplus
}
#endif

#endif							/* SLSI_UTILS_SCSC_H__ */
