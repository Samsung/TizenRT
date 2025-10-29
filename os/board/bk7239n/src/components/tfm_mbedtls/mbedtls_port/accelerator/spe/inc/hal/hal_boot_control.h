// Copyright (C), 2018-2019, Arm Technology (China) Co., Ltd.
// All rights reserved
//
// The content of this file or document is CONFIDENTIAL and PROPRIETARY
// to Arm Technology (China) Co., Ltd. It is subject to the terms of a
// License Agreement between Licensee and Arm Technology (China) Co., Ltd
// restricting among other things, the use, reproduction, distribution
// and transfer.  Each of the embodiments, including this information and,,
// any derivative work shall retain this copyright notice.

#ifndef __HAL_BOOT_CONTROL_H__
#define __HAL_BOOT_CONTROL_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "hal_common.h"

typedef enum _hal_boot_flag_t {
    HAL_BOOT_FLAG_INVALID  = 0,
    HAL_BOOT_FLAG_PRIMARY  = 1,
    HAL_BOOT_FLAG_RECOVERY = 2,
} hal_boot_flag_t;

HAL_API hal_ret_t hal_read_preferred_boot_flag(hal_boot_flag_t *flag);
HAL_API hal_ret_t hal_write_preferred_boot_flag(const hal_boot_flag_t flag);

#ifdef __cplusplus
}
#endif

#endif /* __HAL_BOOT_CONTROL_H__ */