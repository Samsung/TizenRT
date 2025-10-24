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

#include <driver/hal/hal_dma_types.h>

#ifdef __cplusplus
extern "C" {
#endif

bk_err_t dma_set_src_pause_addr(dma_id_t id, uint32_t addr);
bk_err_t dma_set_dst_pause_addr(dma_id_t id, uint32_t addr);
uint32_t dma_get_src_read_addr(dma_id_t id);
uint32_t dma_get_dest_write_addr(dma_id_t id);
bk_err_t dma_memcpy(void *out, const void *in, uint32_t len);
bk_err_t dma_memcpy_by_chnl(void *out, const void *in, uint32_t len, dma_id_t cpy_chnl);
bk_err_t bk_dma_set_src_data_width(dma_id_t id, dma_data_width_t data_width);
bk_err_t bk_dma_set_dest_data_width(dma_id_t id, dma_data_width_t data_width);
uint32_t bk_dma_get_repeat_wr_pause(dma_id_t id);
uint32_t bk_dma_get_repeat_rd_pause(dma_id_t id);
uint32_t bk_dma_get_finish_interrupt_cnt(dma_id_t id);
uint32_t bk_dma_get_half_finish_interrupt_cnt(dma_id_t id);
bk_err_t bk_dma_stateless_judgment_configuration(void *out, const void *in, uint32_t len, dma_id_t cpy_chnl, void *finish_isr);

#ifdef __cplusplus
}
#endif

