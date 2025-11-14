/*
 * Copyright (C), 2018-2019, Arm Technology (China) Co., Ltd.
 * All rights reserved
 *
 * The content of this file or document is CONFIDENTIAL and PROPRIETARY
 * to Arm Technology (China) Co., Ltd. It is subject to the terms of a
 * License Agreement between Licensee and Arm Technology (China) Co., Ltd
 * restricting among other things, the use, reproduction, distribution
 * and transfer.  Each of the embodiments, including this information and,,
 * any derivative work shall retain this copyright notice.
 */

#ifndef __COMPILER_H__
#define __COMPILER_H__

/* MACRO in c language */
#ifndef __ASSEMBLY__

#if __GNUC__

#define likely(x)       __builtin_expect(!!(x), 1L)
#define unlikely(x)     __builtin_expect(!!(x), 0L)
#define __PRINTFLIKE(f, v)   __attribute__((format (printf, f, v)))
#define __UNUSED __attribute__((__unused__))
#ifndef __USED
#define __USED __attribute__((__used__))
#endif
#ifndef __PACKED
#define __PACKED __attribute__((packed))
#endif
#ifndef __ALIGNED
#define __ALIGNED(x) __attribute__((aligned(x)))
#endif
/* look for gcc 3.0 and above */
#if (__GNUC__ > 3) || (__GNUC__ == 3 && __GNUC_MINOR__ >= 0)
#define __ALWAYS_INLINE __attribute__((always_inline))
#else
#define __ALWAYS_INLINE
#endif
#if (__GNUC__ > 3) || (__GNUC__ == 3 && __GNUC_MINOR__ >= 3)
/* the may_alias attribute was introduced in gcc 3.3; before that, there
 * was no way to specify aliasiang rules on a type-by-type basis */
#define __MAY_ALIAS __attribute__((may_alias))

/* nonnull was added in gcc 3.3 as well */
#define __NONNULL(x) __attribute((nonnull x))
#else
#define __MAY_ALIAS
#define __NONNULL(x)
#endif
#define __SECTION(x) __attribute((section(x)))
#define __WEAK __attribute__((weak))
#define __GNU_INLINE __attribute__((gnu_inline))
#define __NAKED __attribute__((naked))
#define __NO_INLINE __attribute((noinline))
/* compiler_barrier */
#define __CB() do { \
	__asm__ __volatile__ ("" ::: "memory"); \
} while ((0))

#else /* ! __GNUC__ */

#define likely(x)       (x)
#define unlikely(x)     (x)
#define __PRINTFLIKE(f, v)
#define __UNUSED
#define __USED
#define __PACKED
#define __ALIGNED(x)
#define __ALWAYS_INLINE
#define __MAY_ALIAS
#define __SECTION(x)
#define __WEAK

#endif /* __GNUC__ */

#endif /* __ASSEMBLY__ */

#endif /* __COMPILER_H__ */
