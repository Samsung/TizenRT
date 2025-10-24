// Copyright 2022-2023 Beken
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
//
#pragma once

#include "hal_config.h"
#include "h264_hw.h"
#include "h264_ll.h"
#include <driver/hal/hal_h264_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/* control mode */

typedef struct {
	h264_hw_t *hw;
}h264_hal_t;

typedef struct {
	gpio_id_t gpio_id;
	gpio_dev_t dev;
} h264_gpio_map_t;

#define h264_hal_get_int_stat(hal) h264_ll_get_int_stat((hal)->hw)

#define h264_hal_int_clear(hal,mode) h264_ll_set_int_stat((hal)->hw,(mode))

#define h264_hal_encode_enable(hal) h264_ll_encode_enable((hal)->hw)

#define h264_hal_encode_disable(hal) h264_ll_encode_disable((hal)->hw)

#define h264_hal_set_global_ctrl(hal) h264_ll_init((hal)->hw)

#define h264_hal_get_encode_count(hal) h264_ll_get_encode_count((hal)->hw)

#define h264_hal_get_num_pframe(hal) h264_ll_get_num_pframe((hal)->hw)

#define h264_hal_enable_external_clk_en(hal) h264_ll_enable_r_h264_clk_en((hal)->hw)

#define h264_hal_disable_external_clk_en(hal) h264_ll_disable_r_h264_clk_en((hal)->hw)

#define h264_hal_enable_internal_en(hal) h264_ll_enable_r_h264_en_bps((hal)->hw)

#define h264_hal_disable_internal_en(hal) h264_ll_disable_r_h264_en_bps((hal)->hw)

#define h264_hal_reset(hal)   h264_ll_reset((hal)->hw)

#define h264_hal_enable_stream_swap(hal) h264_ll_enable_stream_swap((hal)->hw)

#define h264_hal_disable_stream_swap(hal) h264_ll_disable_stream_swap((hal)->hw)

bk_err_t h264_hal_init(h264_hal_t *hal);

bk_err_t h264_hal_set_img_width(h264_hal_t *hal,uint16_t media_width, uint16 media_height);

void h264_hal_gop_config(h264_hal_t *hal,const h264_config_t *config);

void h264_hal_qp_config(h264_hal_t *hal,const h264_config_t *config);

void h264_hal_frame_cbr_config(h264_hal_t *hal,const h264_config_t *config);

void h264_hal_idc_config(h264_hal_t *hal,const h264_config_t *config);

bk_err_t h264_hal_filter_config(h264_hal_t *hal,const h264_config_t *config,uint32_t alpha_off,uint32_t beta_off);

void h264_hal_mb_config(h264_hal_t *hal,const h264_config_t *config);

void h264_hal_mb_bits_config(h264_hal_t *hal,const h264_config_t *config);

void h264_hal_scale_factor_config(h264_hal_t *hal);

void h264_hal_vui_config(h264_hal_t *hal,const h264_config_t *config);

void h264_hal_cpb_vui_config(h264_hal_t *hal,const h264_config_t *config);

void h264_hal_soft_reset_active(h264_hal_t *hal);

void h264_hal_soft_reset_deactive(h264_hal_t *hal);

void h264_hal_local_info(h264_hal_t *hal);

void h264_hal_int_config(h264_hal_t *hal,uint32_t int_mode);

void h264_hal_set_vui_fps(h264_hal_t *hal,uint32_t fps);

#ifdef __cplusplus
}
#endif