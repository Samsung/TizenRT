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

#include "dma_hal.h"
#include "dma_ll.h"

#define DMA_HAL_RETURN_ON_INVALID_ADDR(start_addr, end_addr) do {\
    if ((0 < (end_addr)) && ((end_addr) < (start_addr))) {\
            return BK_ERR_DMA_HAL_INVALID_ADDR;\
        }\
    } while(0)

bk_err_t dma_hal_init(dma_hal_t *hal)
{
   hal->hw = (dma_hw_t *)DMA_LL_REG_BASE(hal->id);
//TODO:only CPU0 init hardware
#if defined(CONFIG_SYS_CPU0)
    dma_ll_init(hal->hw);
#endif
    dma_ll_set_prio_mode_round_robin(hal->hw);
    return BK_OK;
}

void dma_hal_init_without_channels(dma_hal_t *hal)
{
	hal->hw = (dma_hw_t *)DMA_LL_REG_BASE(hal->id);
	//soft reset for all CPUs
	dma_ll_init_without_channels(hal->hw);
}

bk_err_t dma_hal_init_dma(dma_hal_t *hal, dma_id_t id, const dma_config_t *config)
{
    DMA_HAL_RETURN_ON_INVALID_ADDR(config->src.start_addr, config->src.end_addr);
    DMA_HAL_RETURN_ON_INVALID_ADDR(config->dst.start_addr, config->dst.end_addr);

    dma_ll_set_work_mode(hal->hw, id, config->mode);
    dma_ll_set_chan_prio(hal->hw, id, config->chan_prio);

    dma_ll_set_dest_data_width(hal->hw, id, config->dst.width);
    dma_ll_set_src_data_width(hal->hw, id, config->src.width);

    dma_ll_set_dest_req_mux(hal->hw, id, config->dst.dev);
    dma_ll_set_src_req_mux(hal->hw, id, config->src.dev);
#if defined(CONFIG_GDMA_HW_V2PX)
	dma_ll_set_pixel_trans_type(hal->hw, id, config->trans_type);
	dma_ll_set_dest_write_interval(hal->hw, id, config->dest_wr_intlv);
	dma_ll_set_src_read_interval(hal->hw, id, config->src_rd_intlv);
#endif
    dma_ll_set_src_start_addr(hal->hw, id, config->src.start_addr);
    dma_ll_set_src_loop_addr(hal->hw, id, config->src.start_addr, config->src.end_addr);
    dma_ll_set_dest_start_addr(hal->hw, id, config->dst.start_addr);
    dma_ll_set_dest_loop_addr(hal->hw, id, config->dst.start_addr, config->dst.end_addr);

    if (config->src.addr_inc_en) {
        dma_ll_enable_src_addr_inc(hal->hw, id);
    } else {
        dma_ll_disable_src_addr_inc(hal->hw, id);
    }

    if (config->src.addr_loop_en) {
        dma_ll_enable_src_addr_loop(hal->hw, id);
    } else {
        dma_ll_disable_src_addr_loop(hal->hw, id);
    }

    if (config->dst.addr_inc_en) {
        dma_ll_enable_dest_addr_inc(hal->hw, id);
    } else {
        dma_ll_disable_dest_addr_inc(hal->hw, id);
    }

    if (config->dst.addr_loop_en) {
        dma_ll_enable_dest_addr_loop(hal->hw, id);
    } else {
        dma_ll_disable_dest_addr_loop(hal->hw, id);
    }

    return BK_OK;
}

bk_err_t dma_hal_start_common(dma_hal_t *hal, dma_id_t id)
{
    dma_ll_enable(hal->hw, id);
    return BK_OK;
}

bk_err_t dma_hal_stop_common(dma_hal_t *hal, dma_id_t id)
{
    dma_ll_clear_interrupt_status(hal->hw, id);
    dma_ll_disable(hal->hw, id);
    return BK_OK;
}

