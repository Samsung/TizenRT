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
#include <driver/media_types.h>

#ifdef __cplusplus
extern "C" {
#endif


#define scale0_hal_set_clkgate_bypass  scale0_ll_set_0x02_clkg_bypass

#define scale0_hal_set_firstaddr    scale0_ll_set_0x04_first_addr
#define scale0_hal_set_base_addr    scale0_ll_set_0x0a_base_addr
#define scale0_hal_set_dest_addr    scale0_ll_set_0x0c_dest_addr
#define scale0_hal_col_coef         scale0_ll_set_0x06_col_coef      
#define scale0_hal_hloc_index        scale0_ll_set_0x0e_hloc_index    

#define scale0_hal_int_set          scale0_ll_set_0x08_com_cfg
// 4: BURST 4, 8: BURST 8, 16: BURST 16, 32: BURST 32, 64: BURST 64, Others: BURST 32
#define scale0_hal_set_burst_len    scale0_ll_set_0x10_r_write_threshold
#define scale0_hal_start            scale0_ll_set_0x07_module_start


//每行结束后需要设置行压缩因子，比如目标压缩到80行，需要配置param0，param1,....param79
bk_err_t scale0_hal_set_row_coef_loc_params(uint16_t dst_width, uint16_t *params);


void scale0_hal_reset(void);

//h264 mode: 16line, others:8line
bk_err_t  scale0_hal_set_pingpong_line_mode(scale_mode_t format);

bk_err_t scale0_hal_clear_int_status(void);
bool scale0_hal_int_status_is_set(void);
bk_err_t scale0_hal_data_format(pixel_format_t fmt);
bk_err_t scale0_hal_set_pic_width(uint16_t src_width, uint16_t dst_width);



#ifdef __cplusplus
}
#endif


