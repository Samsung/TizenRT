// Copyright (C), 2018-2019, Arm Technology (China) Co., Ltd.
// All rights reserved
//
// The content of this file or document is CONFIDENTIAL and PROPRIETARY
// to Arm Technology (China) Co., Ltd. It is subject to the terms of a
// License Agreement between Licensee and Arm Technology (China) Co., Ltd
// restricting among other things, the use, reproduction, distribution
// and transfer.  Each of the embodiments, including this information and,,
// any derivative work shall retain this copyright notice.

#ifndef __PAL_STRING_H__
#define __PAL_STRING_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "pal_common.h"

/**
 * @brief File Description:
 *        This file contains the implementation for memory operations APIs.
 *        The functions implementations are generally just wrappers to different
 * operating system calls.
 */

/**
 * @brief A wrapper over memcmp functionality. The function compares two given
 * buffers
 *        according to size.
 * The normal memcmp function has side-channel attack venture.
 * better implement safe_memcmp function
 *
 */
PAL_API int32_t pal_safe_memcmp(const void *m1, const void *m2, size_t n);
PAL_API int32_t pal_memcmp(const void *m1, const void *m2, size_t n);

/**
 * @brief A wrapper over memmove functionality, the function copies from one
 *        buffer to another according to given size
 *
 */
PAL_API void *pal_memcpy(void *d, const void *s, size_t n);

/**
 * @brief A wrapper over memset functionality, the function sets a buffer with
 * given value
 *        according to size
 *
 */
PAL_API void *pal_memset(void *buf, int32_t c, size_t n);

/**
 * @brief A wrapper over memmove functionality, the function copies from one
 *        buffer to another according to given size
 *
 */
PAL_API void *pal_memmove(void *d, const void *s, size_t n);

#ifdef __cplusplus
}
#endif

#endif /* __PAL_STRING_H__ */