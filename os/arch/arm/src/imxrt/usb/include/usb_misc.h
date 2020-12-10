/*
 * Copyright (c) 2015 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016, 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __USB_MISC_H__
#define __USB_MISC_H__

#include "usb_porting.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define USB_SHORT_GET_LOW(x)	(((uint16_t)x) & 0xFFU)
#define USB_SHORT_GET_HIGH(x)	((uint8_t)(((uint16_t)x) >> 8U) & 0xFFU)

#define USB_LONG_GET_BYTE0(x)	((uint8_t)(((uint32_t)(x))) & 0xFFU)
#define USB_LONG_GET_BYTE1(x)	((uint8_t)(((uint32_t)(x)) >> 8U) & 0xFFU)
#define USB_LONG_GET_BYTE2(x)	((uint8_t)(((uint32_t)(x)) >> 16U) & 0xFFU)
#define USB_LONG_GET_BYTE3(x)	((uint8_t)(((uint32_t)(x)) >> 24U) & 0xFFU)

#define USB_MEM4_ALIGN_MASK	(0x03U)

/* accessory macro */
#define USB_MEM4_ALIGN(n)	((n + 3U) & (0xFFFFFFFCu))
#define USB_MEM32_ALIGN(n)	((n + 31U) & (0xFFFFFFE0u))
#define USB_MEM64_ALIGN(n)	((n + 63U) & (0xFFFFFFC0u))

/* big/little endian */
#define SWAP2BYTE_CONST(n)	((((n)&0x00FFU) << 8U) | (((n)&0xFF00U) >> 8U))
#define SWAP4BYTE_CONST(n)	((((n)&0x000000FFU) << 24U) | (((n)&0x0000FF00U) << 8U) | (((n)&0x00FF0000U) >> 8U) | (((n)&0xFF000000U) >> 24U))

#define USB_ASSIGN_VALUE_ADDRESS_LONG_BY_BYTE(n, m)      \
{                                                        \
	*((uint8_t *)&(n))     = *((uint8_t *)&(m));     \
	*((uint8_t *)&(n) + 1) = *((uint8_t *)&(m) + 1); \
	*((uint8_t *)&(n) + 2) = *((uint8_t *)&(m) + 2); \
	*((uint8_t *)&(n) + 3) = *((uint8_t *)&(m) + 3); \
}

#define USB_ASSIGN_VALUE_ADDRESS_SHORT_BY_BYTE(n, m)     \
{                                                        \
	*((uint8_t *)&(n))     = *((uint8_t *)&(m));     \
	*((uint8_t *)&(n) + 1) = *((uint8_t *)&(m) + 1); \
}

#define USB_ASSIGN_MACRO_VALUE_ADDRESS_LONG_BY_BYTE(n, m) \
{                                                         \
	*((uint8_t *)&(n))     = (uint8_t)m;              \
	*((uint8_t *)&(n) + 1) = (uint8_t)(m >> 8);       \
	*((uint8_t *)&(n) + 2) = (uint8_t)(m >> 16);      \
	*((uint8_t *)&(n) + 3) = (uint8_t)(m >> 24);      \
}

#define USB_ASSIGN_MACRO_VALUE_ADDRESS_SHORT_BY_BYTE(n, m) \
{                                                          \
	*((uint8_t *)&(n))     = (uint8_t)m;               \
	*((uint8_t *)&(n) + 1) = (uint8_t)(m >> 8);        \
}

#define USB_SHORT_TO_LITTLE_ENDIAN(n)			(n)
#define USB_LONG_TO_LITTLE_ENDIAN(n)			(n)
#define USB_SHORT_FROM_LITTLE_ENDIAN(n)			(n)
#define USB_LONG_FROM_LITTLE_ENDIAN(n)			(n)

#define USB_SHORT_TO_BIG_ENDIAN(n)			SWAP2BYTE_CONST(n)
#define USB_LONG_TO_BIG_ENDIAN(n)			SWAP4BYTE_CONST(n)
#define USB_SHORT_FROM_BIG_ENDIAN(n)			SWAP2BYTE_CONST(n)
#define USB_LONG_FROM_BIG_ENDIAN(n)			SWAP4BYTE_CONST(n)

#define USB_LONG_TO_LITTLE_ENDIAN_ADDRESS(n, m)    \
{                                                  \
	m[3] = ((((uint32_t)(n)) >> 24U) & 0xFFU); \
	m[2] = ((((uint32_t)(n)) >> 16U) & 0xFFU); \
	m[1] = ((((uint32_t)(n)) >> 8U) & 0xFFU);  \
	m[0] = (((uint32_t)(n)) & 0xFFU);          \
}

#define USB_LONG_FROM_LITTLE_ENDIAN_ADDRESS(n)                                                  \
	((uint32_t)((((uint8_t)n[3]) << 24U) | (((uint8_t)n[2]) << 16U) | (((uint8_t)n[1]) << 8U) | \
		(((uint8_t)n[0]) << 0U)))

#define USB_LONG_TO_BIG_ENDIAN_ADDRESS(n, m)       \
{                                                  \
	m[0] = ((((uint32_t)(n)) >> 24U) & 0xFFU); \
	m[1] = ((((uint32_t)(n)) >> 16U) & 0xFFU); \
	m[2] = ((((uint32_t)(n)) >> 8U) & 0xFFU);  \
	m[3] = (((uint32_t)(n)) & 0xFFU);          \
}

#define USB_LONG_FROM_BIG_ENDIAN_ADDRESS(n)                                                     \
	((uint32_t)((((uint8_t)n[0]) << 24U) | (((uint8_t)n[1]) << 16U) | (((uint8_t)n[2]) << 8U) | \
	(((uint8_t)n[3]) << 0U)))

#define USB_SHORT_TO_LITTLE_ENDIAN_ADDRESS(n, m)  \
{                                                 \
	m[1] = ((((uint16_t)(n)) >> 8U) & 0xFFU); \
	m[0] = (((uint16_t)(n)) & 0xFFU);         \
}

#define USB_SHORT_FROM_LITTLE_ENDIAN_ADDRESS(n) ((uint32_t)((((uint8_t)n[1]) << 8U) | (((uint8_t)n[0]) << 0U)))

#define USB_SHORT_TO_BIG_ENDIAN_ADDRESS(n, m)     \
{                                                 \
	m[0] = ((((uint16_t)(n)) >> 8U) & 0xFFU); \
	m[1] = (((uint16_t)(n)) & 0xFFU);         \
}

#define USB_SHORT_FROM_BIG_ENDIAN_ADDRESS(n) ((uint32_t)((((uint8_t)n[0]) << 8U) | (((uint8_t)n[1]) << 0U)))

#define USB_LONG_TO_LITTLE_ENDIAN_DATA(n, m)                         \
{                                                                    \
	*((uint8_t *)&(m) + 3) = ((((uint32_t)(n)) >> 24U) & 0xFFU); \
	*((uint8_t *)&(m) + 2) = ((((uint32_t)(n)) >> 16U) & 0xFFU); \
	*((uint8_t *)&(m) + 1) = ((((uint32_t)(n)) >> 8U) & 0xFFU);  \
	*((uint8_t *)&(m) + 0) = (((uint32_t)(n)) & 0xFFU);          \
}

#define USB_LONG_FROM_LITTLE_ENDIAN_DATA(n)                                             \
	((uint32_t)(((*((uint8_t *)&(n) + 3)) << 24U) | ((*((uint8_t *)&(n) + 2)) << 16U) | \
		((*((uint8_t *)&(n) + 1)) << 8U) | ((*((uint8_t *)&(n))) << 0U)))

#define USB_SHORT_TO_LITTLE_ENDIAN_DATA(n, m)                       \
{                                                                   \
	*((uint8_t *)&(m) + 1) = ((((uint16_t)(n)) >> 8U) & 0xFFU); \
	*((uint8_t *)&(m))     = ((((uint16_t)(n))) & 0xFFU);       \
}

#define USB_SHORT_FROM_LITTLE_ENDIAN_DATA(n) ((uint32_t)(((*((uint8_t *)&(n) + 1)) << 8U) | ((*((uint8_t *)&(n))))))

#define FSL_FEATURE_L1DCACHE_LINESIZE_BYTE	(32)
#define USB_CACHE_LINESIZE					MAX(0, FSL_FEATURE_L1DCACHE_LINESIZE_BYTE)

#define USB_DATA_ALIGN						(4)
#define USB_DATA_ALIGN_SIZE					MAX(USB_CACHE_LINESIZE, USB_DATA_ALIGN)
#define USB_DATA_ALIGN_SIZE_MULTIPLE(n)		((n + USB_DATA_ALIGN_SIZE - 1U) & (~(USB_DATA_ALIGN_SIZE - 1U)))

#define USB_RAM_ADDRESS_ALIGNMENT(n)		__attribute__((aligned(n)))
#define USB_DMA_NONINIT_DATA_ALIGN(n)		USB_RAM_ADDRESS_ALIGNMENT(n) __attribute__((section("m_usb_dma_noninit_data, \"aw\", %nobits @")))

#ifdef USB_TIZEN_RT
#define USB_CONTROLLER_DATA					__attribute__((section(".non_cacheable_buffer_section")))
#else
#define USB_CONTROLLER_DATA					__attribute__((section("NonCacheable, \"aw\", %nobits @")))
#endif
/*! @brief Define default timeout value */
//#define USB_OSA_WAIT_TIMEOUT              ((uint32_t)(-1))

#endif	/* __USB_MISC_H__ */
