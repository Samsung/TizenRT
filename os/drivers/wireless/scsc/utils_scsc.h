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

#ifndef __packed
#define __packed __attribute__((__packed__))
#endif

#define __force         __attribute__((force))
#define __aligned(x)    __attribute__((aligned(x)))

typedef u16 __bitwise __le16;
typedef u32 __bitwise __le32;
typedef u64 __bitwise __le64;
typedef u16 __bitwise __be16;
typedef u32 __bitwise __be32;

typedef unsigned char __u8;
typedef unsigned short __u16;
#ifndef CONFIG_ENABLE_IOTIVITY
typedef unsigned int __u32;
#endif

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
static inline u16 bswap_16(u16 x)
{
	return ((x & 0xff) << 8) | (x >> 8);
}
static inline u32 bswap_32(u32 x)
{
	return ((x & 0xff) << 24) | ((x & 0xff00) << 8) | ((x & 0xff0000) >> 8) | (x >> 24);
}

#define cpu_to_le16(x) ((__le16)(u16)(x))
#define cpu_to_le32(x) ((__le32)(u32)(x))
#define cpu_to_be16(x) ((__be16)bswap_16(x))
#define cpu_to_be32(x) ((__be32)bswap_32(x))

#define le16_to_cpu(x) ((u16)(__le16)(x))
#define le32_to_cpu(x) ((u32)(__le32)(x))
#define be16_to_cpu(x) bswap_16((u16)(__be16)(x))
#define be32_to_cpu(x) bswap_32((u32)(__be32)(x))

/* MAC Address Length */
#define ETH_ALEN        6
#define ETH_HLEN 14
#define ETH_ZLEN        60		/* Min. octets in frame sans FCS */
#define ETH_DATA_LEN    1500	/* Max. octets in payload        */
#define ETH_P_802_3     0x0001	/* Dummy type for 802.3 frames  */

#define SLSI_MUTEX_INIT(mutex__)                                                 \
	do {                                                                     \
		int __ret;                                                       \
		__ret = pthread_mutex_init(&(mutex__), NULL);                    \
		WARN(__ret, "Failed to initialize the mutex (ret:%d)\n", __ret); \
	} while (0)
#define SLSI_MUTEX_LOCK(mutex_to_lock)                                           \
	do {                                                                     \
		int __ret;                                                       \
		__ret = pthread_mutex_lock(&(mutex_to_lock));                    \
		WARN(__ret, "Failed to lock the mutex (ret:%d)\n", __ret);       \
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

#define mbuf_queue_walk(queue, mbuf) \
	for (mbuf = (queue)->next;                                      \
	     mbuf != (struct max_buff *)(queue);                    \
	     mbuf = mbuf->next)

static inline int is_broadcast_ether_addr(const u8 *a)
{
	return (a[0] & a[1] & a[2] & a[3] & a[4] & a[5]) == 0xff;
}

static inline void ether_addr_copy(u8 *dst, const u8 *src)
{
	u16 *a = (u16 *) dst;
	const u16 *b = (const u16 *)src;

	a[0] = b[0];
	a[1] = b[1];
	a[2] = b[2];
}
#define SLSI_ETHER_COPY(dst, src)       ether_addr_copy((dst), (src))

#define SLSI_MAC_FORMAT "%02x:%02x:%02x:%02x:%02x:%02x"
#define SLSI_MAC_STR(x) (x)[0], (x)[1], (x)[2], (x)[3], (x)[4], (x)[5]

/*
 *      This is an Ethernet frame header.
 */

struct ethhdr {
	unsigned char h_dest[ETH_ALEN];	/* destination eth addr */
	unsigned char h_source[ETH_ALEN];	/* source ether addr    */
	__be16 h_proto;			/* packet type ID field */
} __attribute__((packed));

struct max_buff;

#define SLSI_MBUF_GET_ALIGNMENT_OFFSET(mbuf) 0

/* Get the Compiler to ignore Unused parameters */
#define SLSI_UNUSED_PARAMETER(x) ((void)(x))
#ifdef UFK6_DEBUG
#define SLSI_UNUSED_PARAMETER_NOT_DEBUG(x)
#else
#define SLSI_UNUSED_PARAMETER_NOT_DEBUG(x) ((void)(x))
#endif

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

static inline void slsi_eth_zero_addr(u8 *addr)
{
	memset(addr, 0x00, ETH_ALEN);
}

static inline void slsi_eth_broadcast_addr(u8 *addr)
{
	memset(addr, 0xff, ETH_ALEN);
}

static inline int slsi_str_to_int(char *str, int *result)
{
	int i = 0;

	*result = 0;
	if ((str[i] == '-') || ((str[i] >= '0') && (str[i] <= '9'))) {
		if (str[0] == '-') {
			i++;
		}
		while (str[i] >= '0' && str[i] <= '9') {
			*result *= 10;
			*result += (int)str[i++] - '0';
		}

		*result = ((str[0] == '-') ? (-(*result)) : *result);
	}
	return i;
}

struct slsi_dev;

static inline u32 slsi_get_center_freq1(struct slsi_dev *sdev, u16 chann_info, u16 center_freq)
{
	u32 center_freq1 = 0x0000;

	SLSI_UNUSED_PARAMETER(sdev);

	switch (chann_info & 0xFF) {
	case 40:
		center_freq1 = center_freq - 20 * ((chann_info & 0xFF00) >> 8) + 10;
		break;

	case 80:
		center_freq1 = center_freq - 20 * ((chann_info & 0xFF00) >> 8) + 30;
		break;
	default:
		break;
	}
	return center_freq1;
}

/****************************************************************************
 * __set_bit - Set a bit in memory
 *
 * @nr: the bit to set
 * @addr: the address to start counting from
 *
 * Unlike set_bit(), this function is non-atomic and may be reordered.
 * If it's called on the same region of memory simultaneously, the effect
 * may be that only one operation succeeds.
 *
 ****************************************************************************/

#define BIT(nr)            (1 << (nr))
#define BITS_PER_BYTE        8
#define BIT_MASK(nr)        (1 << ((nr) % BITS_PER_BYTE))
#define BIT_BYTE(nr)        ((nr) / BITS_PER_BYTE)

#ifdef CONFIG_SLSI_WLAN_FAPI_LOG
extern void slsi_wlan_fapi_log_init(void);
extern void slsi_wlan_fapi_log_add_record(u16 signal_id, struct timespec ts);
#endif

/* instead of normal spinlock type*/
typedef uint8_t spinlock_t;

#ifdef __cplusplus
}
#endif

#endif							/* SLSI_UTILS_SCSC_H__ */
