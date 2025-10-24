// Copyright (C), 2018-2019, Arm Technology (China) Co., Ltd.
// All rights reserved
//
// The content of this file or document is CONFIDENTIAL and PROPRIETARY
// to Arm Technology (China) Co., Ltd. It is subject to the terms of a
// License Agreement between Licensee and Arm Technology (China) Co., Ltd
// restricting among other things, the use, reproduction, distribution
// and transfer.  Each of the embodiments, including this information and,,
// any derivative work shall retain this copyright notice.

#ifndef __HAL_TRNG_CALIBRATION_H__
#define __HAL_TRNG_CALIBRATION_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "hal_common.h"

HAL_API bool hal_trng_calibration_is_enabled(void);
HAL_API void hal_trng_calibration_indicate_on(void);
HAL_API void hal_trng_calibration_indicate_off(void);
HAL_API void hal_trng_calibration_get_timeout_setting(uint32_t *timeout);

#ifdef __cplusplus
}
#endif

#endif /* __HAL_TRNG_CALIBRATION_H__ */