// Copyright (C), 2018-2019, Arm Technology (China) Co., Ltd.
// All rights reserved
//
// The content of this file or document is CONFIDENTIAL and PROPRIETARY
// to Arm Technology (China) Co., Ltd. It is subject to the terms of a
// License Agreement between Licensee and Arm Technology (China) Co., Ltd
// restricting among other things, the use, reproduction, distribution
// and transfer.  Each of the embodiments, including this information and,,
// any derivative work shall retain this copyright notice.

#ifndef __HAL_SECURE_BOOT_H__
#define __HAL_SECURE_BOOT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "hal_common.h"
#include "hal_boot_control.h"

typedef enum _hal_manifest_type_t {
    HAL_PRIMARY_MANIFEST  = 1,
    HAL_RECOVERY_MANIFEST = 2,
} hal_manifest_type_t;

HAL_API hal_ret_t hal_get_manifest_partition_info(hal_manifest_type_t type,
                                                  hal_addr_t *addr,
                                                  size_t *size);

#ifdef __cplusplus
}
#endif

#endif /* __HAL_SECURE_BOOT_H__ */