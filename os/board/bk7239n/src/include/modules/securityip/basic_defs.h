/* basic_defs.h
 *
 * Driver Framework v4, Basic Definitions.
 *
 * This file provides a number of basic definitions and can be customized for
 * any compiler.
 */

/*****************************************************************************
* Copyright (c) 2007-2016 INSIDE Secure B.V. All Rights Reserved.
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 2 of the License, or
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/

#ifndef INCLUDE_GUARD_BASIC_DEFS_H
#define INCLUDE_GUARD_BASIC_DEFS_H


/* ============ Compiler-based Configuration ============ */

//#if __STDC_VERSION__ < 199901L
//#error "BasicDefs: C standards older than C99 not supported"
//#endif

#ifdef __cplusplus
#error "BasicDefs: C++ standard not supported"
#endif

#ifdef _MSC_VER
#define BASICDEFS_COMPILER_MICROSOFT_V1
#define BASICDEFS_TYPES_V1
#define BASICDEFS_BOOL_V1
// Microsoft compiler only supports "inline" in C++ mode and
// expects __inline in C mode
#define BASICDEFS_INLINE_V1
#endif

#ifdef linux
#ifdef MODULE
// kernel loadable module
#define BASICDEFS_COMPILER_LINUX_V1
#define BASICDEFS_INLINE_BUILTIN
#include <linux/types.h>
#else
// user mode
#define BASICDEFS_COMPILER_LINUX_V1
#define BASICDEFS_TYPES_STDINT
#define BASICDEFS_BOOL_V1
#define BASICDEFS_INLINE_BUILTIN
#endif
#endif

#ifdef __STDC_VERSION__
#if __STDC_VERSION__ >= 199901L
#define BASICDEFS_TYPES_STDINT
#define BASICDEFS_BOOL_V1
#define BASICDEFS_INLINE_BUILTIN
#endif
#endif

#ifdef __CYGWIN__
#define BASICDEFS_COMPILER_CYGWIN_V1
#define BASICDEFS_TYPES_STDINT
#define BASICDEFS_BOOL_V1
#define BASICDEFS_INLINE_BUILTIN
#endif

/* ============ Basic Types ============ */

#ifdef BASICDEFS_TYPES_STDINT
// ISO-C99
#include <stdint.h>
#endif

#ifdef BASICDEFS_TYPES_V1
typedef unsigned char  uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int   uint32_t;
#endif

/* ============ Boolean type ============ */

#ifdef BASICDEFS_BOOL_V1
// ISO-C99
#include <stdbool.h>
#endif

/* ============ NULL ============ */

#ifndef NULL
#define NULL 0
#endif

/* ============ MIN, MAX ============ */

// warning for side-effects on the following two macros since the arguments
// are evaluated twice changing this to inline functions is problematic
// because of type incompatibilities
#ifndef MIN
#define MIN(_x, _y) ((_x) < (_y) ? (_x) : (_y))
#endif
#ifndef MAX
#define MAX(_x, _y) ((_x) > (_y) ? (_x) : (_y))
#endif


/* ============ BIT_n ============ */

// using postfix "U" to be compatible with uint32
// ("UL" is not needed and gives lint warning)
#define BIT_0   0x00000001U
#define BIT_1   0x00000002U
#define BIT_2   0x00000004U
#define BIT_3   0x00000008U
#define BIT_4   0x00000010U
#define BIT_5   0x00000020U
#define BIT_6   0x00000040U
#define BIT_7   0x00000080U
#define BIT_8   0x00000100U
#define BIT_9   0x00000200U
#define BIT_10  0x00000400U
#define BIT_11  0x00000800U
#define BIT_12  0x00001000U
#define BIT_13  0x00002000U
#define BIT_14  0x00004000U
#define BIT_15  0x00008000U
#define BIT_16  0x00010000U
#define BIT_17  0x00020000U
#define BIT_18  0x00040000U
#define BIT_19  0x00080000U
#define BIT_20  0x00100000U
#define BIT_21  0x00200000U
#define BIT_22  0x00400000U
#define BIT_23  0x00800000U
#define BIT_24  0x01000000U
#define BIT_25  0x02000000U
#define BIT_26  0x04000000U
#define BIT_27  0x08000000U
#define BIT_28  0x10000000U
#define BIT_29  0x20000000U
#define BIT_30  0x40000000U
#define BIT_31  0x80000000U

/* ============ MASK_n_BITS ============ */

#define MASK_1_BIT      (BIT_1 - 1)
#define MASK_2_BITS     (BIT_2 - 1)
#define MASK_3_BITS     (BIT_3 - 1)
#define MASK_4_BITS     (BIT_4 - 1)
#define MASK_5_BITS     (BIT_5 - 1)
#define MASK_6_BITS     (BIT_6 - 1)
#define MASK_7_BITS     (BIT_7 - 1)
#define MASK_8_BITS     (BIT_8 - 1)
#define MASK_9_BITS     (BIT_9 - 1)
#define MASK_10_BITS    (BIT_10 - 1)
#define MASK_11_BITS    (BIT_11 - 1)
#define MASK_12_BITS    (BIT_12 - 1)
#define MASK_13_BITS    (BIT_13 - 1)
#define MASK_14_BITS    (BIT_14 - 1)
#define MASK_15_BITS    (BIT_15 - 1)
#define MASK_16_BITS    (BIT_16 - 1)
#define MASK_17_BITS    (BIT_17 - 1)
#define MASK_18_BITS    (BIT_18 - 1)
#define MASK_19_BITS    (BIT_19 - 1)
#define MASK_20_BITS    (BIT_20 - 1)
#define MASK_21_BITS    (BIT_21 - 1)
#define MASK_22_BITS    (BIT_22 - 1)
#define MASK_23_BITS    (BIT_23 - 1)
#define MASK_24_BITS    (BIT_24 - 1)
#define MASK_25_BITS    (BIT_25 - 1)
#define MASK_26_BITS    (BIT_26 - 1)
#define MASK_27_BITS    (BIT_27 - 1)
#define MASK_28_BITS    (BIT_28 - 1)
#define MASK_29_BITS    (BIT_29 - 1)
#define MASK_30_BITS    (BIT_30 - 1)
#define MASK_31_BITS    (BIT_31 - 1)

/* ============ IDENTIFIER_NOT_USED ============ */

#define IDENTIFIER_NOT_USED(_v)   (void)(_v)

/* ============ inline ============ */

#ifdef BASICDEFS_INLINE_V1
#define inline __inline
#endif


#endif /* Inclusion Guard */


/* end of file basic_defs.h */
