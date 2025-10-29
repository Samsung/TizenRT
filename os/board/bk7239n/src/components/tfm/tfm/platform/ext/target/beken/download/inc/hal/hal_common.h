// Copyright (C), 2018-2019, Arm Technology (China) Co., Ltd.
// All rights reserved
//
// The content of this file or document is CONFIDENTIAL and PROPRIETARY
// to Arm Technology (China) Co., Ltd. It is subject to the terms of a
// License Agreement between Licensee and Arm Technology (China) Co., Ltd
// restricting among other things, the use, reproduction, distribution
// and transfer.  Each of the embodiments, including this information and,,
// any derivative work shall retain this copyright notice.

#ifndef __HAL_COMMON_H__
#define __HAL_COMMON_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * This file defines all the necessary types used by HAL and upper software.
 *
 */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/**
 *  hal return value enumeration, used for most hal/pal interfaces' return
 */
typedef enum _hal_ret_t {
    HAL_OK,
    HAL_ERR_GENERIC        = 0xFFFFFFFF,
    HAL_ERR_VRFY_FAILED    = 0xFFFFFFFE,
    HAL_ERR_BAD_PARAM      = 0xFFFFFFFD,
    HAL_ERR_INTERNAL_FATAL = 0xFFFFFFFC,
    HAL_ERR_NOT_SUPPORT    = 0xFFFFFFFB,
    HAL_ERR_OUT_OF_MEMORY  = 0xFFFFFFFA,
    HAL_ERR_TIMEOUT        = 0xFFFFFFF9,
} hal_ret_t;

/**
 * Others
 */
typedef unsigned long hal_addr_t;
/**
 *  macro
 */
/* define hal API declearation */
#define HAL_API

#ifdef __cplusplus
}
#endif

#endif /* __HAL_COMMON_H__ */