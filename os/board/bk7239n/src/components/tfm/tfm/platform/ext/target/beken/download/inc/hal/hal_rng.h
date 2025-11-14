// Copyright (C), 2018-2019, Arm Technology (China) Co., Ltd.
// All rights reserved
//
// The content of this file or document is CONFIDENTIAL and PROPRIETARY
// to Arm Technology (China) Co., Ltd. It is subject to the terms of a
// License Agreement between Licensee and Arm Technology (China) Co., Ltd
// restricting among other things, the use, reproduction, distribution
// and transfer.  Each of the embodiments, including this information and,,
// any derivative work shall retain this copyright notice.

#ifndef __HAL_RNG_H__
#define __HAL_RNG_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "hal_common.h"

HAL_API hal_ret_t hal_random_reseed(const uint8_t *seed, size_t seed_size);
HAL_API hal_ret_t hal_gen_random_data(uint8_t *data, size_t data_size);

#ifdef __cplusplus
}
#endif

#endif /* __HAL_RNG_H__ */