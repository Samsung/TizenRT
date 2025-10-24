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

#include <common/bk_include.h>
#include <driver/h264_types.h>
#include <driver/media_types.h>

#ifdef __cplusplus
extern "C"
{
#endif

bk_err_t bk_h264_driver_init(void);

bk_err_t bk_h264_driver_deinit(void);

bk_err_t bk_h264_init(media_ppi_t media_ppi);

bk_err_t bk_h264_set_pframe_num(uint32_t pframe_number);

bk_err_t bk_h264_set_qp(uint32_t qp);

bk_err_t bk_h264_set_quality(uint32_t imb_bits, uint32_t pmb_bits);

bk_err_t bk_h264_encode_enable(void);

bk_err_t bk_h264_encode_disable(void);

bk_err_t bk_h264_reset(void);

bk_err_t bk_h264_clk_check(void);

bk_err_t bk_h264_get_fifo_addr(uint32_t *fifo_addr);

bk_err_t bk_h264_register_isr(h264_isr_type_t type_id, h264_isr_t isr, void *param);

bk_err_t bk_h264_unregister_isr(h264_isr_type_t type_id);

uint32_t bk_h264_get_encode_count(void);

bk_err_t bk_h264_enc_lcd_dma_cpy(void *out, const void *in, uint32_t len, dma_id_t cpy_chnl);


#ifdef __cplusplus
}
#endif
