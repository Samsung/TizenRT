// Copyright 2020-2025 Beken
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

#ifdef __cplusplus
extern "C" {
#endif

#include "hal_config.h"
#include "dma_hw.h"
#include "dma_ll.h"
#include <driver/hal/hal_dma_types.h>

typedef struct {
    dma_hw_t *hw;
    dma_unit_t id;
} dma_hal_t;

#define dma_hal_get_work_mode(hal, id) dma_ll_get_work_mode((hal)->hw, id)
#define dma_hal_repeat_wr_pause(hal, id) dma_ll_repeat_wr_pause((hal)->hw, (id))
#define dma_hal_repeat_rd_pause(hal, id) dma_ll_repeat_rd_pause((hal)->hw, (id))
#define dma_hal_finish_interrupt_cnt(hal, id) dma_ll_finish_interrupt_cnt((hal)->hw, (id))
#define dma_hal_half_finish_interrupt_cnt(hal, id) dma_ll_half_finish_interrupt_cnt((hal)->hw, (id))
#define dma_hal_clear_half_finish_interrupt_status(hal, id) dma_ll_clear_half_finish_interrupt_status((hal)->hw, (id))
#define dma_hal_clear_finish_interrupt_status(hal, id) dma_ll_clear_finish_interrupt_status((hal)->hw, (id))
#define dma_hal_is_half_finish_interrupt_triggered(hal, id) dma_ll_is_half_finish_interrupt_triggered((hal)->hw, id)
#define dma_hal_is_finish_interrupt_triggered(hal, id) dma_ll_is_finish_interrupt_triggered((hal)->hw, id)
#define dma_hal_enable_finish_interrupt(hal, id) dma_ll_enable_finish_interrupt((hal)->hw, id)
#define dma_hal_disable_finish_interrupt(hal, id) dma_ll_disable_finish_interrupt((hal)->hw, id)
#define dma_hal_enable_half_finish_interrupt(hal, id) dma_ll_enable_half_finish_interrupt((hal)->hw, id)
#define dma_hal_disable_half_finish_interrupt(hal, id) dma_ll_disable_half_finish_interrupt((hal)->hw, id)
#define dma_hal_clear_bus_err_interrupt_status(hal, id) dma_ll_clear_bus_err_interrupt_status((hal)->hw, (id))
#define dma_hal_is_bus_err_interrupt_triggered(hal, id) dma_ll_is_bus_err_interrupt_triggered((hal)->hw, id)
#define dma_hal_enable_bus_err_interrupt(hal, id) dma_ll_enable_bus_err_interrupt((hal)->hw, id)
#define dma_hal_disable_bus_err_interrupt(hal, id) dma_ll_disable_bus_err_interrupt((hal)->hw, id)

#define dma_hal_reset_config_to_default(hal, id) dma_ll_reset_config_to_default((hal)->hw, (id))
#define dma_hal_is_id_started(hal, id) dma_ll_is_id_started((hal)->hw, (id))
#define dma_hal_set_transfer_len(hal, id, tran_len) dma_ll_set_transfer_len((hal)->hw, id, tran_len)
#define dma_hal_get_transfer_len_max(hal) dma_ll_get_transfer_len_max((hal)->hw)
#define dma_hal_get_remain_len(hal, id) dma_ll_get_remain_len((hal)->hw, (id))
#define dma_hal_set_prio_mode(hal, prio_mode) dma_ll_set_prio_mode((hal)->hw, prio_mode)
#define dma_hal_get_enable_status(hal, id) dma_ll_get_enable_status((hal)->hw, id)

#define dma_hal_set_src_start_addr(hal, id, addr) dma_ll_set_src_start_addr((hal)->hw, id, addr)
#define dma_hal_set_dest_start_addr(hal, id, addr) dma_ll_set_dest_start_addr((hal)->hw, id, addr)
#define dma_hal_set_src_loop_addr(hal, id, start_addr, end_addr) dma_ll_set_src_loop_addr((hal)->hw, id, start_addr, end_addr)
#define dma_hal_set_dest_loop_addr(hal, id, start_addr, end_addr) dma_ll_set_dest_loop_addr((hal)->hw, id, start_addr, end_addr)
#define dma_hal_enable_src_addr_inc(hal, id) dma_ll_enable_src_addr_inc((hal)->hw, id)
#define dma_hal_disable_src_addr_inc(hal, id) dma_ll_disable_src_addr_inc((hal)->hw, id)
#define dma_hal_enable_dest_addr_inc(hal, id) dma_ll_enable_dest_addr_inc((hal)->hw, id)
#define dma_hal_disable_dest_addr_inc(hal, id) dma_ll_disable_dest_addr_inc((hal)->hw, id)
#define dma_hal_enable_src_addr_loop(hal, id) dma_ll_enable_src_addr_loop((hal)->hw, id)
#define dma_hal_disable_src_addr_loop(hal, id) dma_ll_disable_src_addr_loop((hal)->hw, id)
#define dma_hal_enable_dest_addr_loop(hal, id) dma_ll_enable_dest_addr_loop((hal)->hw, id)
#define dma_hal_disable_dest_addr_loop(hal, id) dma_ll_disable_dest_addr_loop((hal)->hw, id)

#define dma_hal_set_src_pause_addr(hal, id, addr) dma_ll_set_src_pause_addr((hal)->hw, id, addr)
#define dma_hal_set_dest_pause_addr(hal, id, addr) dma_ll_set_dest_pause_addr((hal)->hw, id, addr)
#define dma_hal_get_src_read_addr(hal, id) dma_ll_get_src_read_addr((hal)->hw, id)
#define dma_hal_get_dest_write_addr(hal, id) dma_ll_get_dest_write_addr((hal)->hw, id)

#define dma_hal_set_src_data_width(hal, id, data_width) dma_ll_set_src_data_width((hal)->hw, id, data_width)
#define dma_hal_set_dest_data_width(hal, id, data_width) dma_ll_set_dest_data_width((hal)->hw, id, data_width)

#define dma_hal_flush_src_buffer(hal, id) dma_ll_flush_src_buffer((hal)->hw, id)

#ifdef CONFIG_SPE
#define dma_hal_set_dest_burst_len(hal, id, len) dma_ll_set_dest_burst_len((hal)->hw, id, len)
#define dma_hal_get_dest_burst_len(hal, id) dma_ll_get_dest_burst_len((hal)->hw, id)

#define dma_hal_set_src_burst_len(hal, id, len) dma_ll_set_src_burst_len((hal)->hw, id, len)
#define dma_hal_get_src_burst_len(hal, id) dma_ll_get_src_burst_len((hal)->hw, id)

#define dma_hal_set_dest_sec_attr(hal, id, attr) dma_ll_set_dest_sec_attr((hal)->hw, id, attr)
#define dma_hal_set_src_sec_attr(hal, id, attr) dma_ll_set_src_sec_attr((hal)->hw, id, attr)

#define dma_hal_bus_err_int_enable(hal, id) dma_ll_bus_err_int_enable((hal)->hw, id)
#define dma_hal_bus_err_int_disable(hal, id) dma_ll_bus_err_int_disable((hal)->hw, id)

#define dma_hal_set_pixel_trans_type(hal, id, type) dma_ll_set_pixel_trans_type((hal)->hw, id, type)
#define dma_hal_get_pixel_trans_type(hal, id) dma_ll_get_pixel_trans_type((hal)->hw, id)

#define dma_hal_set_src_read_interval(hal, id, interval) dma_ll_set_src_read_interval((hal)->hw, id, interval)
#define dma_hal_get_src_read_interval(hal, id) dma_ll_get_src_read_interval((hal)->hw, id)

#define dma_hal_set_dest_write_interval(hal, id, interval) dma_ll_set_dest_write_interval((hal)->hw, id, interval)
#define dma_hal_get_dest_write_interval(hal, id) dma_ll_get_dest_write_interval((hal)->hw, id)

#endif

#if defined(CONFIG_SPE) && (CONFIG_SPE == 1)
#define dma_hal_set_sec_attr(hal, id, attr) dma_ll_set_secure_attr((hal)->hw, id, attr)
#define dma_hal_set_privileged_attr(hal, id, attr) dma_ll_set_privileged_attr((hal)->hw, id, attr);
#define dma_hal_set_int_allocate(hal, id, int_id) dma_ll_set_int_allocate((hal)->hw, id, int_id);
#endif


bk_err_t dma_hal_init(dma_hal_t *hal);
void dma_hal_init_without_channels(dma_hal_t *hal);
bk_err_t dma_hal_init_dma(dma_hal_t *hal, dma_id_t id, const dma_config_t *config);
bk_err_t dma_hal_start_common(dma_hal_t *hal, dma_id_t id);
bk_err_t dma_hal_stop_common(dma_hal_t *hal, dma_id_t id);

#if CFG_HAL_DEBUG_DMA
void dma_struct_dump(dma_id_t id);
#else
#define dma_struct_dump(id)
#endif

#ifdef __cplusplus
}
#endif


