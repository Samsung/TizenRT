// Copyright 2020-2021 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include <components/video_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/* @brief Overview about this API header
 *
 */

/**
 * @brief     video transfer init
 *
 * This API will create video thread, init msg queue, and excute camera init
 *
 * @param setup_cfg configure of the video transfer include packet process, method of transfer, etc.
 *
 * @return
 *    - kNoErr: succeed
 *    - others: other errors.
 */
bk_err_t bk_video_transfer_init(video_setup_t *setup_cfg);

/**
 * @brief     video transfer deinit
 *
 * This API will quit video thread, and free all reasource
 *
 * @return
 *    - kNoErr: succeed
 *    - others: other errors.
 */
bk_err_t bk_video_transfer_deinit(void);

/**
 * @brief     set video transfer param
 *
 * This API will open video tranfer data_buffer, and star transfer
 *
 * @param device referenc type : meida_camera_device_t
 *
 * @attention 1. when call this function, the video date will transfer to wifi(UDP/others)
 *
 * @return
 *    - kNoErr: succeed
 *    - others: other errors.
 */
bk_err_t bk_video_buffer_open(media_camera_device_t *device);

/**
 * @brief     set video buffer close
 *
 * This API will deinit video tranfer, stop transfer video data
 *
 * @return
 *    - kNoErr: succeed
 *    - others: other errors.
 */
bk_err_t bk_video_buffer_close(void);

/**
 * @brief     read video buffer frame
 *
 * This API will malloc a data_buffer, and save video data to this buffer
 *
 * @param buf malloc buf pointer
 * @param buf_len buf length
 * @param err_code  
 *            - 0: success
 *            - 1: param error
 *            - 2: buffer full
 *            - 3: frame data err
 *            - 4: timeout
 *            - 5: unknow err
 * @param timeout read frame data timeout
 *
 * @return
 *    - 0: failed
 *    - other: frame_length.
 */
uint32_t bk_video_buffer_read_frame(uint8_t *buf, uint32_t buf_len, int *err_code, uint32_t timeout);

/**
 * @brief     read video buffer frame
 *
 * This API will stop camera, but not free malloc memory for transfer
 *
 * @return
 *    - kNoErr: succeed
 *    - others: other errors.
 */
bk_err_t bk_video_transfer_stop(void);

/**
 * @brief     read video buffer frame
 *
 * This API will start camera after stopping camera
 *
 * @attation 1. this function must called after bk_video_transfer_stop
 *
 * @return
 *    - kNoErr: succeed
 *    - others: other errors.
 */
bk_err_t bk_video_transfer_start(void);
/**
 * @}
 */

#ifdef __cplusplus
}
#endif