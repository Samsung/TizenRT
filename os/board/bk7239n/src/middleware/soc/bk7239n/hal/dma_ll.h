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

#include <soc/soc.h>
#include "hal_port.h"
#include "dma_hw.h"
#include <driver/hal/hal_dma_types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define DMA_LL_REG_OFFSET  0x00010000
#define DMA_LL_REG_BASE(_dma_unit_id) (SOC_GENER_DMA_REG_BASE+_dma_unit_id*DMA_LL_REG_OFFSET)

#define CASE_DEV(dev) case DMA_DEV_##dev: return DMA_V_REQ_MUX_##dev
#define CASE_D() default: return 0

static inline void dma_ll_init(dma_hw_t *hw)
{
	int dma_id;

#if defined(CONFIG_SPE) && (CONFIG_SPE == 1)
	hw->prio_mode.v = 0;
	hw->prio_mode.soft_reset = 1;	//reset it before anyother operations

	hw->secure_attr.v = 0xFFF;
	hw->privileged_attr.v = 0xFFF;
#endif

	for (dma_id = 0; dma_id < SOC_DMA_CHAN_NUM_PER_UNIT; dma_id++) {
		hw->config_group[dma_id].ctrl.v = 0;
		hw->config_group[dma_id].dest_start_addr = 0;
		hw->config_group[dma_id].src_start_addr = 0;
		hw->config_group[dma_id].dest_loop_end_addr = 0;
		hw->config_group[dma_id].dest_loop_start_addr = 0;
		hw->config_group[dma_id].src_loop_end_addr = 0;
		hw->config_group[dma_id].src_loop_start_addr = 0;
		hw->config_group[dma_id].req_mux.v = 0;
		hw->config_group[dma_id].src_pause_addr = 0;
		hw->config_group[dma_id].dest_pause_addr = 0;
		hw->config_group[dma_id].status.v = 0;
	}
}

static inline uint32_t dma_ll_get_soft_reset_value(dma_hw_t *hw)
{
	return hw->prio_mode.soft_reset;
}

static inline void dma_ll_init_without_channels(dma_hw_t *hw)
{
	if(0 == dma_ll_get_soft_reset_value(hw)) {
#if defined(CONFIG_SPE) && (CONFIG_SPE == 1)
		hw->prio_mode.v = 0;
		hw->prio_mode.soft_reset = 1;	//reset it before anyother operations
		hw->secure_attr.v = 0xFFF;
		hw->privileged_attr.v = 0xFFF;
#else
		BK_LOGE("DMA", "soft reset is 0, check BAKP!");
#endif
	}
}

static inline uint32_t dma_ll_dev_to_req_mux(uint32 req_mux)
{
	switch (req_mux) {
		CASE_DEV(DTCM);
		CASE_DEV(UART1);
		CASE_DEV(UART1_RX);
		CASE_DEV(UART2);
		CASE_DEV(UART2_RX);
		CASE_DEV(UART3);
		CASE_DEV(UART3_RX);
		CASE_DEV(GSPI0);
		CASE_DEV(GSPI0_RX);
		CASE_DEV(GSPI1);
		CASE_DEV(GSPI1_RX);
		CASE_DEV(SDIO);
		CASE_DEV(SDIO_RX);
		CASE_DEV(UART4);
		CASE_DEV(UART4_RX);
		CASE_DEV(I2S);
		CASE_DEV(I2S_RX);
		CASE_D();
	}
}

static inline void dma_ll_enable(dma_hw_t *hw, dma_id_t id)
{
	hw->config_group[id].ctrl.enable = 1;
}

static inline void dma_ll_disable(dma_hw_t *hw, dma_id_t id)
{
	hw->config_group[id].ctrl.enable = 0;
}

static inline uint32_t dma_ll_get_enable_status(dma_hw_t *hw, dma_id_t id)
{
	return hw->config_group[id].ctrl.enable;
}

static inline bool dma_ll_is_id_started(dma_hw_t *hw, dma_id_t id)
{
	return !!(hw->config_group[id].ctrl.enable == 1);
}

static inline void dma_ll_enable_finish_interrupt(dma_hw_t *hw, dma_id_t id)
{
	hw->config_group[id].ctrl.finish_int_en = 1;
}

static inline void dma_ll_disable_finish_interrupt(dma_hw_t *hw, dma_id_t id)
{
	hw->config_group[id].ctrl.finish_int_en = 0;
}

static inline void dma_ll_enable_half_finish_interrupt(dma_hw_t *hw, dma_id_t id)
{
	hw->config_group[id].ctrl.half_finish_int_en = 1;
}

static inline void dma_ll_disable_half_finish_interrupt(dma_hw_t *hw, dma_id_t id)
{
	hw->config_group[id].ctrl.half_finish_int_en = 0;
}

static inline void dma_ll_enable_bus_err_interrupt(dma_hw_t *hw, dma_id_t id)
{
	hw->config_group[id].req_mux.bus_err_int_en = 1;
}

static inline void dma_ll_disable_bus_err_interrupt(dma_hw_t *hw, dma_id_t id)
{
	hw->config_group[id].req_mux.bus_err_int_en = 0;
}

static inline void dma_ll_enable_interrupt(dma_hw_t *hw, dma_id_t id)
{
	dma_ll_enable_half_finish_interrupt(hw, id);
	dma_ll_enable_finish_interrupt(hw, id);
	dma_ll_enable_bus_err_interrupt(hw,id);
}

static inline void dma_ll_disable_interrupt(dma_hw_t *hw, dma_id_t id)
{
	dma_ll_disable_half_finish_interrupt(hw, id);
	dma_ll_disable_finish_interrupt(hw, id);
	dma_ll_disable_bus_err_interrupt(hw,id);
}

static inline void dma_ll_clear_finish_interrupt_status(dma_hw_t *hw, dma_id_t id)
{
	/*other interrupt bit also wirte 1 to clear, so should not effect other bits*/
	hw->config_group[id].status.v = BIT(DMA_FINISH_INT_POS);
}

static inline void dma_ll_clear_half_finish_interrupt_status(dma_hw_t *hw, dma_id_t id)
{
	/*other interrupt bit also wirte 1 to clear, so should not effect other bits*/
	hw->config_group[id].status.v = BIT(DMA_HALF_FINISH_INT_POS);
}

static inline void dma_ll_clear_bus_err_interrupt_status(dma_hw_t *hw, dma_id_t id)
{
	/*other interrupt bit also wirte 1 to clear, so should not effect other bits*/
	hw->config_group[id].status.v = BIT(DMA_BUS_ERR_INT_POS);
}


static inline void dma_ll_clear_interrupt_status(dma_hw_t *hw, dma_id_t id)
{
	dma_ll_clear_half_finish_interrupt_status(hw, id);
	dma_ll_clear_finish_interrupt_status(hw, id);
	dma_ll_clear_bus_err_interrupt_status(hw, id);

}

static inline uint32_t dma_ll_repeat_wr_pause(dma_hw_t *hw, dma_id_t id)
{
	return (uint32_t)(hw->config_group[id].status.repeat_wr_pause);
}

static inline uint32_t dma_ll_repeat_rd_pause(dma_hw_t *hw, dma_id_t id)
{
	return (uint32_t)(hw->config_group[id].status.repeat_rd_pause);
}

static inline uint32_t dma_ll_finish_interrupt_cnt(dma_hw_t *hw, dma_id_t id)
{
	return (uint32_t)(hw->config_group[id].status.finish_int_counter);
}

static inline uint32_t dma_ll_half_finish_interrupt_cnt(dma_hw_t *hw, dma_id_t id)
{
	return (uint32_t)(hw->config_group[id].status.half_finish_int_counter);
}

static inline bool dma_ll_is_finish_interrupt_triggered(dma_hw_t *hw, dma_id_t id)
{
	return !!(hw->config_group[id].status.finish_int & 0x1);
}

static inline bool dma_ll_is_half_finish_interrupt_triggered(dma_hw_t *hw, dma_id_t id)
{
	return !!(hw->config_group[id].status.half_finish_int & 0x1);
}

static inline bool dma_ll_is_bus_err_interrupt_triggered(dma_hw_t *hw, dma_id_t id)
{
	return !!(hw->config_group[id].status.bus_err_int & 0x1);
}

static inline void dma_ll_reset_config_to_default(dma_hw_t *hw, volatile dma_id_t id)
{
	hw->config_group[id].ctrl.v = 0;
	hw->config_group[id].dest_start_addr = 0;
	hw->config_group[id].src_start_addr = 0;
	hw->config_group[id].dest_loop_end_addr = 0;
	hw->config_group[id].dest_loop_start_addr = 0;
	hw->config_group[id].src_loop_end_addr = 0;
	hw->config_group[id].src_loop_start_addr = 0;
	hw->config_group[id].req_mux.v = 0;
	hw->config_group[id].src_pause_addr = 0;
	hw->config_group[id].dest_pause_addr = 0;
}

static inline void dma_ll_set_work_mode(dma_hw_t *hw, dma_id_t id, uint32_t mode)
{
	hw->config_group[id].ctrl.mode = mode & 0x01;
}

#define dma_ll_set_mode_single(hw, id) dma_ll_set_work_mode(hw, id, DMA_V_WORK_MODE_SINGLE)
#define dma_ll_set_mode_repeat(hw, id) dma_ll_set_work_mode(hw, id, DMA_V_WORK_MODE_REPEAT)

static uint32_t dma_ll_get_work_mode(dma_hw_t *hw, dma_id_t id)
{
    return hw->config_group[id].ctrl.mode;
}

static inline void dma_ll_set_chan_prio(dma_hw_t *hw, dma_id_t id, uint32_t chan_prio)
{
	hw->config_group[id].ctrl.chan_prio = chan_prio & 0x07;
}

static inline void dma_ll_set_dest_data_width(dma_hw_t *hw, dma_id_t id, uint32_t data_width)
{
	hw->config_group[id].ctrl.dest_data_width = data_width & 0x03;
}

static inline void dma_ll_set_src_data_width(dma_hw_t *hw, dma_id_t id, uint32_t data_width)
{
	hw->config_group[id].ctrl.src_data_width = data_width & 0x03;
}

static inline void dma_ll_set_dest_req_mux(dma_hw_t *hw, dma_id_t id, uint32_t req_mux)
{
	hw->config_group[id].req_mux.dest_req_mux = (dma_ll_dev_to_req_mux(req_mux)) & 0x1f;
}

static inline void dma_ll_set_src_req_mux(dma_hw_t *hw, dma_id_t id, uint32_t req_mux)
{
	hw->config_group[id].req_mux.src_req_mux = (dma_ll_dev_to_req_mux(req_mux)) & 0x1f;
}

static inline void dma_ll_set_pixel_trans_type(dma_hw_t *hw, dma_id_t id, uint32_t type)
{
	hw->config_group[id].req_mux.pixel_trans_type = (type & 0x3);
}

static inline uint32_t dma_ll_get_pixel_trans_type(dma_hw_t *hw, dma_id_t id)
{
	return (hw->config_group[id].req_mux.pixel_trans_type & 0x3);
}

static inline void dma_ll_enable_src_addr_inc(dma_hw_t *hw, dma_id_t id)
{
	hw->config_group[id].ctrl.src_addr_inc_en = 1;
}

static inline void dma_ll_disable_src_addr_inc(dma_hw_t *hw, dma_id_t id)
{
	hw->config_group[id].ctrl.src_addr_inc_en = 0;
}

static inline void dma_ll_enable_dest_addr_inc(dma_hw_t *hw, dma_id_t id)
{
	hw->config_group[id].ctrl.dest_addr_inc_en = 1;
}

static inline void dma_ll_disable_dest_addr_inc(dma_hw_t *hw, dma_id_t id)
{
	hw->config_group[id].ctrl.dest_addr_inc_en = 0;
}

static inline void dma_ll_enable_src_addr_loop(dma_hw_t *hw, dma_id_t id)
{
	hw->config_group[id].ctrl.src_addr_loop_en = 1;
}

static inline void dma_ll_disable_src_addr_loop(dma_hw_t *hw, dma_id_t id)
{
	hw->config_group[id].ctrl.src_addr_loop_en = 0;
}

static inline void dma_ll_enable_dest_addr_loop(dma_hw_t *hw, dma_id_t id)
{
	hw->config_group[id].ctrl.dest_addr_loop_en = 1;
}

static inline void dma_ll_disable_dest_addr_loop(dma_hw_t *hw, dma_id_t id)
{
	hw->config_group[id].ctrl.dest_addr_loop_en = 0;
}

static inline void dma_ll_set_dest_start_addr(dma_hw_t *hw, volatile dma_id_t id, uint32_t addr)
{
	hw->config_group[id].dest_start_addr = addr;
}

static inline void dma_ll_set_src_start_addr(dma_hw_t *hw, dma_id_t id, uint32_t addr)
{
	hw->config_group[id].src_start_addr = addr;
}

static inline void dma_ll_set_dest_loop_addr(dma_hw_t *hw, dma_id_t id, uint32_t start_addr, uint32_t end_addr)
{
	hw->config_group[id].dest_loop_start_addr = start_addr;
	hw->config_group[id].dest_loop_end_addr = end_addr;
}

static inline void dma_ll_set_src_loop_addr(dma_hw_t *hw, dma_id_t id, uint32_t start_addr, uint32_t end_addr)
{
	hw->config_group[id].src_loop_start_addr = start_addr;
	hw->config_group[id].src_loop_end_addr = end_addr;
}

#define DMA_LL_TRANS_LEN_MAX (0x10000)
static inline uint32_t dma_ll_get_transfer_len_max(dma_hw_t *hw)
{
	return DMA_LL_TRANS_LEN_MAX;
}

static inline void dma_ll_set_transfer_len(dma_hw_t *hw, dma_id_t id, uint32_t tran_len)
{
	hw->config_group[id].ctrl.transfer_len = (tran_len - 1) & 0xffff;
}

static inline uint32_t dma_ll_get_remain_len(dma_hw_t *hw, dma_id_t id)
{
	return (uint32_t)hw->config_group[id].status.remain_len;
}

static inline void dma_ll_set_prio_mode(dma_hw_t *hw, uint32_t prio_mode)
{
#if defined(CONFIG_SPE) && (CONFIG_SPE == 1)
	hw->prio_mode.prio_mode = prio_mode & 0x1;
#endif
}

#define dma_ll_set_prio_mode_round_robin(hw) dma_ll_set_prio_mode(hw, DMA_V_PRIO_MODE_ROUND_ROBIN)
#define dma_ll_set_prio_mode_fixed_prio(hw) dma_ll_set_prio_mode(hw, DMA_V_PRIO_MODE_FIXED_PRIO)

#if defined(CONFIG_SPE) && (CONFIG_SPE == 1)
static inline void dma_ll_set_secure_attr(dma_hw_t *hw, dma_id_t id, dma_sec_attr_t attr)
{
	if (attr == DMA_ATTR_NON_SEC) {
		hw->secure_attr.attr &= ~(BIT(id) & 0xFFF);
	} else if (attr == DMA_ATTR_SEC) {
		hw->secure_attr.attr |= (BIT(id) & 0xFFF);
	}
}

static inline void dma_ll_set_privileged_attr(dma_hw_t *hw, dma_id_t id, dma_sec_attr_t attr)
{
	if (attr == DMA_ATTR_NON_SEC) {
		hw->privileged_attr.attr &= ~(BIT(id) & 0xFFF);
	} else if (attr == DMA_ATTR_SEC) {
		hw->privileged_attr.attr |= (BIT(id) & 0xFFF);
	}
}

static inline void dma_ll_set_int_allocate(dma_hw_t *hw, dma_id_t id,dma_int_id_t int_id)
{
	hw->int_allocate.status |= (int_id)<<BIT(id);
	//os_printf("%s and %x\r\n",__func__,hw->int_allocate.v);
}

#endif

static inline void dma_ll_set_src_pause_addr(dma_hw_t *hw, dma_id_t id, uint32_t addr)
{
	hw->config_group[id].src_pause_addr = addr;
}

static inline void dma_ll_set_dest_pause_addr(dma_hw_t *hw, dma_id_t id, uint32_t addr)
{
	hw->config_group[id].dest_pause_addr = addr;
}

static inline uint32_t dma_ll_get_src_read_addr(dma_hw_t *hw, dma_id_t id)
{
	return hw->config_group[id].src_rd_addr;
}

static inline uint32_t dma_ll_get_dest_write_addr(dma_hw_t *hw, dma_id_t id)
{
	return hw->config_group[id].dest_wr_addr;
}

static inline void dma_ll_set_dest_burst_len(dma_hw_t *hw, dma_id_t id, uint32_t len)
{
	hw->config_group[id].req_mux.dtst_burst_len = (len & 0x3);
}

static inline uint32_t dma_ll_get_dest_burst_len(dma_hw_t *hw, dma_id_t id)
{
	return (hw->config_group[id].req_mux.dtst_burst_len & 0x3);
}

static inline void dma_ll_set_src_burst_len(dma_hw_t *hw, dma_id_t id, uint32_t len)
{
	hw->config_group[id].req_mux.src_burst_len = (len & 0x3);
}

static inline uint32_t dma_ll_get_src_burst_len(dma_hw_t *hw, dma_id_t id)
{
	return (hw->config_group[id].req_mux.src_burst_len & 0x3);
}

static inline void dma_ll_bus_err_int_enable(dma_hw_t *hw, dma_id_t id)
{
	hw->config_group[id].req_mux.bus_err_int_en = 0x1;
}

static inline void dma_ll_bus_err_int_disable(dma_hw_t *hw, dma_id_t id)
{
	hw->config_group[id].req_mux.bus_err_int_en = 0x0;
}

static inline void dma_ll_set_dest_sec_attr(dma_hw_t *hw, dma_id_t id, uint32_t attr)
{
	hw->config_group[id].req_mux.dest_sec_attr = attr & 0x1;
}

static inline void dma_ll_set_src_sec_attr(dma_hw_t *hw, dma_id_t id, uint32_t attr)
{
	hw->config_group[id].req_mux.src_sec_attr = attr & 0x1;
}

static inline void dma_ll_flush_src_buffer(dma_hw_t *hw, dma_id_t id)
{
#if 1
	/*other interrupt bit also wirte 1 to clear, so should not effect other bits*/
	hw->config_group[id].status.v = BIT(DMA_FLUSH_SRC_BUF_POS);
#else
	hw->config_group[id].status.flush_src_buff = 1;
#endif
	BK_WHILE(hw->config_group[id].status.flush_src_buff);
}

//TODO
static inline void dma_ll_set_dest_write_interval(dma_hw_t *hw, dma_id_t id, uint32_t interval)
{
}

static inline void dma_ll_set_src_read_interval(dma_hw_t *hw, dma_id_t id, uint32_t interval)
{
}

#ifdef __cplusplus
}
#endif

