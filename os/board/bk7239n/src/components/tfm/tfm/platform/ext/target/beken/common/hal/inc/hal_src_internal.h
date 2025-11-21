// Copyright (C), 2018-2018, Arm Technology (China) Co., Ltd.
// All rights reserved
//
// The content of this file or document is CONFIDENTIAL and PROPRIETARY
// to Arm Technology (China) Co., Ltd. It is subject to the terms of a
// License Agreement between Licensee and Arm Technology (China) Co., Ltd
// restricting among other things, the use, reproduction, distribution
// and transfer.  Each of the embodiments, including this information and,,
// any derivative work shall retain this copyright notice.

#ifndef __HAL_SRC_INTERNAL_H__
#define __HAL_SRC_INTERNAL_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "hal.h"
#include "pal.h"

#define _HAL_INTERNAL_MAX_UINT32_VAL (0xffffffff)

/* check if "x" is align with "align"(must be a power of 2).
   0: NOT ALIGN
   1: ALIGN */
#define _HAL_INTERNAL_IS_ALIGN(x, align)                                       \
    (!(((unsigned long)(x)) & ((unsigned long)align - 1)))

#define HAL_CHECK_RET(...)                                                     \
    do {                                                                       \
        if ((HAL_OK) != (ret)) {                                               \
            PAL_LOG_ERR("%s line:%d. Error number is 0x%x \n",                 \
                        __func__,                                              \
                        __LINE__,                                              \
                        ret);                                                  \
            PAL_LOG_ERR(__VA_ARGS__);                                          \
            goto finish;                                                       \
        }                                                                      \
    } while (0)

#define HAL_CHECK_CONDITION(__true_condition__, __ret_code__, ...)             \
    do {                                                                       \
        if (!(__true_condition__)) {                                           \
            PAL_LOG_ERR("%s line:%d\n",                                        \
                        __func__,                                              \
                        __LINE__,                                              \
                        ret);                                                  \
            PAL_LOG_ERR(__VA_ARGS__);                                          \
            ret = (__ret_code__);                                              \
            goto finish;                                                       \
        }                                                                      \
    } while (0)

/* compile-time assertion that makes the build fail */
#define HAL_BUILD_ASSERT(__EXPR__)                                             \
    typedef char __build_assert_failure[(__EXPR__) ? 1 : -1]

#ifdef __cplusplus
}
#endif

#endif /* __HAL_SRC_INTERNAL_H__ */
