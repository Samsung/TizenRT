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

#include "hal_config.h"
#include "jpeg_hw.h"
#include "jpeg_ll.h"
#include <driver/hal/hal_jpeg_types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	jpeg_hw_t *hw;
	jpeg_unit_t id;
} jpeg_hal_t;

typedef struct {
	gpio_id_t gpio_id;
	gpio_dev_t dev;
} jpeg_gpio_map_t;

#define jpeg_hal_soft_reset(hal) jpeg_ll_soft_reset((hal)->hw)
#define jpeg_hal_set_global_ctrl(hal) jpeg_ll_init((hal)->hw)
#define jpeg_hal_enable_end_yuv_int(hal) jpeg_ll_enable_end_yuv_int((hal)->hw)
#define jpeg_hal_disable_end_yuv_int(hal) jpeg_ll_disable_end_yuv_int((hal)->hw)
#define jpeg_hal_enable_head_output_int(hal) jpeg_ll_enable_head_output_int((hal)->hw)
#define jpeg_hal_disable_head_output_int(hal) jpeg_ll_disable_head_output_int((hal)->hw)
#define jpeg_hal_enable_start_frame_int(hal) jpeg_ll_enable_start_frame_int((hal)->hw)
#define jpeg_hal_disable_start_frame_int(hal) jpeg_ll_disable_start_frame_int((hal)->hw)
#define jpeg_hal_enable_end_frame_int(hal) jpeg_ll_enable_end_frame_int((hal)->hw)
#define jpeg_hal_disable_end_frame_int(hal) jpeg_ll_disable_end_frame_int((hal)->hw)
#define jpeg_hal_enable_vsync_negedge_int(hal) jpeg_ll_enable_vsync_negedge_int((hal)->hw)
#define jpeg_hal_disable_vsync_negedge_int(hal) jpeg_ll_disable_vsync_negedge_int((hal)->hw)
#define jpeg_hal_enable_line_clear_int(hal) jpeg_ll_enable_line_clear_int((hal)->hw)
#define jpeg_hal_disable_line_clear_int(hal) jpeg_ll_disable_line_clear_int((hal)->hw)
#define jpeg_hal_enable_frame_error_int(hal) jpeg_ll_enable_frame_error_int((hal)->hw)
#define jpeg_hal_disable_frame_error_int(hal) jpeg_ll_disable_frame_error_int((hal)->hw)

#define jpeg_hal_reset_config_to_default(hal) jpeg_ll_reset_config_to_default((hal)->hw)
#define jpeg_hal_set_x_pixel(hal, x_pixel) jpeg_ll_set_x_pixel((hal)->hw, x_pixel)
#define jpeg_hal_set_y_pixel(hal, x_pixel) jpeg_ll_set_y_pixel((hal)->hw, x_pixel)
#define jpeg_hal_enable_yuv_mode(hal, mode) jpeg_ll_enable_yuv_mode((hal)->hw)
#define jpeg_hal_disable_yuv_mode(hal, mode) jpeg_ll_disable_yuv_mode((hal)->hw)
#define jpeg_hal_enable_jpeg_mode(hal) jpeg_ll_enable_jpeg_mode((hal)->hw)
#define jpeg_hal_disable_jpeg_mode(hal) jpeg_ll_disable_jpeg_mode((hal)->hw)

#define jpeg_hal_get_frame_byte_number(hal) jpeg_ll_get_frame_byte_number((hal)->hw)

#define jpeg_hal_get_interrupt_status(hal) jpeg_ll_get_interrupt_status((hal)->hw)
#define jpeg_hal_clear_interrupt_status(hal, int_status) jpeg_ll_clear_interrupt_status((hal)->hw, int_status)
#define jpeg_hal_is_frame_start_int_triggered(hal, int_status) jpeg_ll_is_frame_start_int_triggered((hal)->hw, int_status)
#define jpeg_hal_is_frame_end_int_triggered(hal, int_status) jpeg_ll_is_frame_end_int_triggered((hal)->hw, int_status)
#define jpeg_hal_is_yuv_end_int_triggered(hal, int_status) jpeg_ll_is_yuv_end_int_triggered((hal)->hw, int_status)
#define jpeg_hal_is_head_output_int_triggered(hal, int_status) jpeg_ll_is_head_output_int_triggered((hal)->hw, int_status)
#define jpeg_hal_is_sync_negedge_int_triggered(hal, int_status) jpeg_ll_is_vsync_negedge_int_triggered((hal)->hw, int_status)
#define jpeg_hal_is_line_clear_int_triggered(hal, int_status) jpeg_ll_is_l_clear_int_triggered((hal)->hw, int_status)
#define jpeg_hal_is_frame_error_int_triggered(hal, int_status) jpeg_ll_is_frame_err_int_triggered((hal)->hw, int_status)

#define jpeg_hal_set_mclk_div(hal, value) jpeg_ll_set_mclk_div((hal)->hw, value)

bk_err_t jpeg_hal_init(jpeg_hal_t *hal);
bk_err_t jpeg_hal_yuv_fmt_sel(jpeg_hal_t *hal, yuv_format_t value);
bk_err_t jpeg_hal_start_common(jpeg_hal_t *hal, yuv_mode_t mode);
bk_err_t jpeg_hal_stop_common(jpeg_hal_t *hal, yuv_mode_t mode);
bk_err_t jpeg_hal_enable_clk(jpeg_hal_t *hal, uint32_t value);
bk_err_t jpeg_hal_enable_partial_display(jpeg_hal_t *hal, uint32_t enable);
bk_err_t jpeg_hal_partial_display_offset_config(jpeg_hal_t *hal, const jpeg_partial_offset_config_t *offset_config);
bk_err_t jpeg_hal_set_em_base_addr(jpeg_hal_t *hal, uint8_t *address);
bk_err_t jpeg_hal_switch_mode(jpeg_hal_t *hal, const jpeg_config_t *config);
bk_err_t jpeg_hal_enable_bitrate_ctrl(jpeg_hal_t *hal, uint8_t enable);
bk_err_t jpeg_hal_set_target_size(jpeg_hal_t *hal, uint32_t up_size, uint32_t low_size);
uint32_t jpeg_hal_get_jpeg_share_mem_addr(void);

#if CFG_HAL_DEBUG_JPEG
void jpeg_struct_dump(void);
#else
#define jpeg_struct_dump()
#endif

#ifdef __cplusplus
}
#endif

