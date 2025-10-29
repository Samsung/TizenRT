// Copyright (C), 2018-2019, Arm Technology (China) Co., Ltd.
// All rights reserved
//
// The content of this file or document is CONFIDENTIAL and PROPRIETARY
// to Arm Technology (China) Co., Ltd. It is subject to the terms of a
// License Agreement between Licensee and Arm Technology (China) Co., Ltd
// restricting among other things, the use, reproduction, distribution
// and transfer.  Each of the embodiments, including this information and,,
// any derivative work shall retain this copyright notice.

#ifndef __PAL_LOG_H__
#define __PAL_LOG_H__

#ifdef __cplusplus
extern "C" {
#endif

#define PAL_LOG_PREFIX_NAME "[IPSS-M]"
#define PAL_MAX_LOG_LEVEL 2

#include "pal_common.h"

/*!
   @file
   @brief This file contains the PAL layer log definitions
 */

/* the PAL LOG API */
PAL_API void pal_log_set_level(int32_t level);
PAL_API int32_t pal_log_get_level(void);
PAL_API int32_t pal_platform_printf(const char *__fmt__, ...);
PAL_API int _pal_platform_printf(const char *__fmt__, ...);

#define __PAL_LOG_PLAT(__fmt__, ...)                                           \
    pal_platform_printf(__fmt__, ##__VA_ARGS__);
/* PAL log levels */
#define PAL_LOG_LEVEL_NULL (-1)
#define PAL_LOG_LEVEL_ERR (0)
#define PAL_LOG_LEVEL_WARN (1)
#define PAL_LOG_LEVEL_INFO (2)
#define PAL_LOG_LEVEL_DEBUG (3)
#define PAL_LOG_LEVEL_TRACE (4)
#define PAL_LOG_LEVEL_DATA (5)

#ifndef PAL_LOG_PREFIX_NAME
#warning "PAL_LOG_PREFIX_NAME is not defined, use default: arm CHINA"
#define PAL_LOG_PREFIX_NAME "arm CHINA"
#endif

#ifndef PAL_MAX_LOG_LEVEL
#error "PAL_MAX_LOG_LEVEL is not defined!"
#endif /*PAL_MAX_LOG_LEVEL*/

/* Evaluate PAL_MAX_LOG_LEVEL in case provided by caller */
#define __PAL_LOG_LEVEL_EVAL(level) level
#define _PAL_MAX_LOG_LEVEL __PAL_LOG_LEVEL_EVAL(PAL_MAX_LOG_LEVEL)

/* Filter logging based on logMask and dispatch to platform specific logging
 * mechanism */
#define __PAL_LOG(level, __fmt__, ...)                                         \
    do {                                                                       \
        if (pal_log_get_level() >= (PAL_LOG_LEVEL_##level)) {                  \
            __PAL_LOG_PLAT(PAL_LOG_PREFIX_NAME __fmt__, ##__VA_ARGS__);        \
        }                                                                      \
    } while (0)

#define __PAL_LOG_RAW(level, __fmt__, ...)                                     \
    do {                                                                       \
        if (pal_log_get_level() >= (PAL_LOG_LEVEL_##level)) {                  \
            __PAL_LOG_PLAT(__fmt__, ##__VA_ARGS__);                            \
        }                                                                      \
    } while (0)

#if (_PAL_MAX_LOG_LEVEL >= PAL_LOG_LEVEL_ERR)
#define PAL_LOG_ERR(__fmt__, ...) __PAL_LOG(ERR, __fmt__, ##__VA_ARGS__)
#else
#define PAL_LOG_ERR(__fmt__, ...)
#endif

#if (_PAL_MAX_LOG_LEVEL >= PAL_LOG_LEVEL_WARN)
#define PAL_LOG_WARN(__fmt__, ...) __PAL_LOG(WARN, __fmt__, ##__VA_ARGS__)
#else
#define PAL_LOG_WARN(__fmt__, ...)
#endif

#if (_PAL_MAX_LOG_LEVEL >= PAL_LOG_LEVEL_INFO)
#define PAL_LOG_INFO(__fmt__, ...) __PAL_LOG(INFO, __fmt__, ##__VA_ARGS__)
#else
#define PAL_LOG_INFO(__fmt__, ...)
#endif

#if (_PAL_MAX_LOG_LEVEL >= PAL_LOG_LEVEL_DEBUG)
#define PAL_LOG_DEBUG(__fmt__, ...) __PAL_LOG(DEBUG, __fmt__, ##__VA_ARGS__)

#define PAL_LOG_DUMP_BUF(__msg__, __buf__, __size__)                           \
    do {                                                                       \
        int32_t __i__;                                                         \
        uint8_t *__ptr__ = (uint8_t *)__buf__;                                 \
        __PAL_LOG(DEBUG, "%s (%d):\n", __msg__, (int)__size__);                \
        for (__i__ = 0; __i__ < (int)__size__; __i__++) {                      \
            __PAL_LOG_RAW(DEBUG, "0x%02X ", __ptr__[__i__]);                   \
            if ((__i__ & 0xF) == 0xF) {                                        \
                __PAL_LOG_RAW(DEBUG, "\n");                                    \
            }                                                                  \
        }                                                                      \
        __PAL_LOG_RAW(DEBUG, "\n");                                            \
    } while (0)
#else
#define PAL_LOG_DEBUG(__fmt__, ...)
#define PAL_LOG_DUMP_BUF(__msg__, __buf__, __size__)
#endif

#if (_PAL_MAX_LOG_LEVEL >= PAL_LOG_LEVEL_TRACE)
#define PAL_LOG_TRACE(__fmt__, ...) __PAL_LOG(TRACE, __fmt__, ##__VA_ARGS__)
#else
#define PAL_LOG_TRACE(__fmt__, ...)
#endif

#if (_PAL_MAX_LOG_LEVEL >= PAL_LOG_LEVEL_DATA)
#define PAL_LOG_DATA(__fmt__, ...) __PAL_LOG(DATA, __fmt__, ##__VA_ARGS__)
#else
#define PAL_LOG_DATA(__fmt__, ...)
#endif

#if defined(PAL_ASSERT_ENABLE)
#define PAL_ASSERT_MSG(__x__, __fmt__, ...)                                    \
    do {                                                                       \
        if (!(__x__)) {                                                        \
            PAL_LOG_ERR("ASSERT FAILURE:\n");                                  \
            PAL_LOG_ERR("%s (%d): %s\n", __func__, __LINE__, __FUNCTION__);    \
            PAL_LOG_ERR(__fmt__, ##__VA_ARGS__);                               \
            while (1) /* loop */                                               \
                ;                                                              \
        }                                                                      \
    } while (0)

#define PAL_ASSERT(__x__)                                                      \
    do {                                                                       \
        if (!(__x__)) {                                                        \
            PAL_LOG_ERR("ASSERT FAILURE:\n");                                  \
            PAL_LOG_ERR("%s (%d): %s\n", __func__, __LINE__, __FUNCTION__);    \
            while (1) /* loop */                                               \
                ;                                                              \
        }                                                                      \
    } while (0)
#else /* !PAL_ASSERT_ENABLE */
#define PAL_ASSERT_MSG(__x__, __fmt__, ...)
#define PAL_ASSERT(__x__)
#endif /* PAL_ASSERT_ENABLE */

#ifdef __cplusplus
}
#endif

#endif /* __PAL_LOG_H__ */
