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

#include <common/bk_include.h>
#include <driver/yuv_buf_types.h>
#include <driver/jpeg_enc_types.h>

#ifdef __cplusplus
extern "C" {
#endif

bk_err_t bk_yuv_buf_driver_init(void);

bk_err_t bk_yuv_buf_driver_deinit(void);

bk_err_t bk_yuv_buf_init(const yuv_buf_config_t *config);

bk_err_t bk_yuv_buf_deinit(void);

bk_err_t bk_yuv_buf_start(yuv_buf_work_mode_t work_mode);

bk_err_t bk_yuv_buf_stop(yuv_buf_work_mode_t work_mode);

bk_err_t bk_yuv_buf_send_data_to_h264(void);

bk_err_t bk_yuv_buf_enable_h264_nosensor_mode(void);

bk_err_t bk_yuv_buf_enable_mclk(yuv_buf_work_mode_t work_mode);

bk_err_t bk_yuv_buf_set_em_base_addr(uint32_t em_base_addr);

bk_err_t bk_yuv_buf_register_isr(yuv_buf_isr_type_t type_id, yuv_buf_isr_t isr, void *param);

bk_err_t bk_yuv_buf_unregister_isr(yuv_buf_isr_type_t type_id);

bk_err_t bk_yuv_buf_init_partial_display(const yuv_buf_partial_offset_config_t *offset_config);

bk_err_t bk_yuv_buf_deinit_partial_display(void);

bk_err_t bk_yuv_buf_set_video_module_clk(jpeg_clk_t clk);

bk_err_t bk_yuv_buf_soft_reset(void);


#ifdef __cplusplus
}
#endif

