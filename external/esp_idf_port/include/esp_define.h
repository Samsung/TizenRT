/******************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************/

#ifndef ESP_DEFINE_H
#define ESP_DEFINE_H

/* Type definitions. */

typedef _int8_t int8_t;
typedef _uint8_t uint8_t;

typedef _int16_t int16_t;
typedef _uint16_t uint16_t;

typedef _int32_t int32_t;
typedef _uint32_t uint32_t;

#ifdef __INT64_DEFINED
typedef _int64_t int64_t;
typedef _uint64_t uint64_t;

#endif							/*
								 */

#define port_char		int8_t
#define port_float		float
#define port_double		double
#define port_long		int32_t
#define port_short		int16_t
#define port_stack_type	uint8_t
#define port_base_type	int

typedef port_stack_type stack_type_t;
typedef port_base_type base_type_t;
typedef unsigned port_base_type u_base_type_t;

#if defined(CONFIG_USE_16_BIT_TICKS)
typedef uint16_t tick_type_t;

#define port_max_delay (tick_type_t) 0xffff
#else							/*
								 */
typedef uint32_t tick_type_t;

#define port_max_delay (tick_type_t) 0xffffffffUL
#endif							/*
								 */

#define WIFI_ADAPTER_FALSE			((base_type_t)0)
#define WIFI_ADAPTER_TRUE			((base_type_t)1)
#define pdPASS          (WIFI_ADAPTER_TRUE)
#define pdFAIL          (WIFI_ADAPTER_FALSE)

#ifndef BIT0

#define BIT0   0x00000001  /**< preprocessor alias for 32-bit value with bit 0 set, used to specify this single bit */
#define BIT1   0x00000002  /**< preprocessor alias for 32-bit value with bit 1 set, used to specify this single bit */
#define BIT2   0x00000004  /**< preprocessor alias for 32-bit value with bit 2 set, used to specify this single bit */
#define BIT3   0x00000008  /**< preprocessor alias for 32-bit value with bit 3 set, used to specify this single bit */
#define BIT4   0x00000010  /**< preprocessor alias for 32-bit value with bit 4 set, used to specify this single bit */
#define BIT5   0x00000020  /**< preprocessor alias for 32-bit value with bit 5 set, used to specify this single bit */
#define BIT6   0x00000040  /**< preprocessor alias for 32-bit value with bit 6 set, used to specify this single bit */
#define BIT7   0x00000080  /**< preprocessor alias for 32-bit value with bit 7 set, used to specify this single bit */
#define BIT8   0x00000100  /**< preprocessor alias for 32-bit value with bit 8 set, used to specify this single bit */
#define BIT9   0x00000200  /**< preprocessor alias for 32-bit value with bit 9 set, used to specify this single bit */
#define BIT10  0x00000400  /**< preprocessor alias for 32-bit value with bit 10 set, used to specify this single bit */
#define BIT11  0x00000800  /**< preprocessor alias for 32-bit value with bit 11 set, used to specify this single bit */
#define BIT12  0x00001000  /**< preprocessor alias for 32-bit value with bit 12 set, used to specify this single bit */
#define BIT13  0x00002000  /**< preprocessor alias for 32-bit value with bit 13 set, used to specify this single bit */
#define BIT14  0x00004000  /**< preprocessor alias for 32-bit value with bit 14 set, used to specify this single bit */
#define BIT15  0x00008000  /**< preprocessor alias for 32-bit value with bit 15 set, used to specify this single bit */
#define BIT16  0x00010000  /**< preprocessor alias for 32-bit value with bit 16 set, used to specify this single bit */
#define BIT17  0x00020000  /**< preprocessor alias for 32-bit value with bit 17 set, used to specify this single bit */
#define BIT18  0x00040000  /**< preprocessor alias for 32-bit value with bit 18 set, used to specify this single bit */
#define BIT19  0x00080000  /**< preprocessor alias for 32-bit value with bit 19 set, used to specify this single bit */
#define BIT20  0x00100000  /**< preprocessor alias for 32-bit value with bit 20 set, used to specify this single bit */
#define BIT21  0x00200000  /**< preprocessor alias for 32-bit value with bit 21 set, used to specify this single bit */
#define BIT22  0x00400000  /**< preprocessor alias for 32-bit value with bit 22 set, used to specify this single bit */
#define BIT23  0x00800000  /**< preprocessor alias for 32-bit value with bit 23 set, used to specify this single bit */
#define BIT24  0x01000000  /**< preprocessor alias for 32-bit value with bit 24 set, used to specify this single bit */
#define BIT25  0x02000000  /**< preprocessor alias for 32-bit value with bit 25 set, used to specify this single bit */
#define BIT26  0x04000000  /**< preprocessor alias for 32-bit value with bit 26 set, used to specify this single bit */
#define BIT27  0x08000000  /**< preprocessor alias for 32-bit value with bit 27 set, used to specify this single bit */
#define BIT28  0x10000000  /**< preprocessor alias for 32-bit value with bit 28 set, used to specify this single bit */
#define BIT29  0x20000000  /**< preprocessor alias for 32-bit value with bit 29 set, used to specify this single bit */
#define BIT30  0x40000000  /**< preprocessor alias for 32-bit value with bit 30 set, used to specify this single bit */
#define BIT31  0x80000000  /**< preprocessor alias for 32-bit value with bit 31 set, used to specify this single bit */

#endif							/* BIT0 et al */

#endif
