// Copyright (C), 2018-2019, Arm Technology (China) Co., Ltd.
// All rights reserved
//
// The content of this file or document is CONFIDENTIAL and PROPRIETARY
// to Arm Technology (China) Co., Ltd. It is subject to the terms of a
// License Agreement between Licensee and Arm Technology (China) Co., Ltd
// restricting among other things, the use, reproduction, distribution
// and transfer.  Each of the embodiments, including this information and,,
// any derivative work shall retain this copyright notice.

#ifndef __HAL_FOTA_H__
#define __HAL_FOTA_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "hal_common.h"
#include "hal_crypto.h"

HAL_API hal_ret_t hal_fota_get_pubkey(hal_crypto_key_t *key);
HAL_API hal_ret_t hal_fota_get_cache_partition_info(hal_addr_t *addr,
                                                    size_t *size);

#ifdef __cplusplus
}
#endif

#endif /* __HAL_FOTA_H__ */