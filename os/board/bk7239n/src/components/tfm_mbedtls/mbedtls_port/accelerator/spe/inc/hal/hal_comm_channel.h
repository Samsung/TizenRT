// Copyright (C), 2018-2019, Arm Technology (China) Co., Ltd.
// All rights reserved
//
// The content of this file or document is CONFIDENTIAL and PROPRIETARY
// to Arm Technology (China) Co., Ltd. It is subject to the terms of a
// License Agreement between Licensee and Arm Technology (China) Co., Ltd
// restricting among other things, the use, reproduction, distribution
// and transfer.  Each of the embodiments, including this information and,,
// any derivative work shall retain this copyright notice.

#ifndef __HAL_COMM_CHANNEL_H__
#define __HAL_COMM_CHANNEL_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "hal_common.h"

typedef void *hal_chan_t;

HAL_API hal_ret_t hal_channel_init(hal_chan_t *ch, void *args);
HAL_API void hal_channel_cleanup(hal_chan_t ch);
HAL_API hal_ret_t hal_channel_read(hal_chan_t ch,
                                   uint8_t *data,
                                   size_t *data_size);
HAL_API hal_ret_t hal_channel_write(hal_chan_t ch,
                                    const uint8_t *data,
                                    size_t data_size);

#ifdef __cplusplus
}
#endif

#endif /* __HAL_COMM_CHANNEL_H__ */