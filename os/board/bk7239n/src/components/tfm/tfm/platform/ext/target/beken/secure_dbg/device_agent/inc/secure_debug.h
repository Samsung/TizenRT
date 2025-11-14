/**
 * Copyright (C), 2018-2019, Arm Technology (China) Co., Ltd.
 * All rights reserved
 *
 * The content of this file or document is CONFIDENTIAL and PROPRIETARY
 * to Arm Technology (China) Co., Ltd. It is subject to the terms of a
 * License Agreement between Licensee and Arm Technology (China) Co., Ltd
 * restricting among other things, the use, reproduction, distribution
 * and transfer.  Each of the embodiments, including this information and
 * any derivative work shall retain this copyright notice.
 *
 */

#ifndef __SECURE_DEBUG_H__
#define __SECURE_DEBUG_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "hal.h"

typedef void *sec_debug_ctx_t;

/**
 * @ingroup securedebug_common
 * @brief	sec_debug_init function, the first function of secure debug
 * library, initial the resources for secure debug library, and get the secure
 * debug context
 * @param[out] sec_debug_ctx secure debug context
 * @param[in] args the args for hal_channel_init
 * @return	HAL_OK on success. error codes otherwise
 */
hal_ret_t sec_debug_init(sec_debug_ctx_t *sec_debug_ctx, void *args);

/**
 * @ingroup securedebug_common
 * @brief	sec_debug_cleanup function
 * @param[in] sec_debug_ctx secure debug context
 * @return	HAL_OK on success. error codes otherwise
 */
hal_ret_t sec_debug_cleanup(sec_debug_ctx_t sec_debug_ctx);

/**
 * @ingroup securedebug_common
 * @brief	sec_debug_enable function
 * @param[in] sec_debug_ctx secure debug context
 * @return	SEC_DEBUG_TRUE when got the enable flag. otherwise
 */
hal_ret_t sec_debug_enable(sec_debug_ctx_t sec_debug_ctx);
#ifdef __cplusplus
}
#endif

#endif /* __SECURE_DEBUG_H__ */

/*************************** The End Of File*****************************/
