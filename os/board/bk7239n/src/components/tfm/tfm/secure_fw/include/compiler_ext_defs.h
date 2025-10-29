/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __COMPILER_EXT_DEFS_H__
#define __COMPILER_EXT_DEFS_H__

#if defined(__ARMCC_VERSION) || defined(__GNUC__) || defined(__ICCARM__)

#ifndef __naked
#define __naked __attribute__((naked))
#endif

#ifndef __section
#define __section(x) __attribute__((section(x)))
#endif

#ifndef __aligned
#define __aligned(x) __attribute__((aligned(x)))
#endif

#ifndef __used
#define __used __attribute__((used))
#endif

#endif /* __ARMCC_VERSION __GNUC__ __ICCARM__*/

#endif /* __COMPILER_EXT_DEFS_H__ */
