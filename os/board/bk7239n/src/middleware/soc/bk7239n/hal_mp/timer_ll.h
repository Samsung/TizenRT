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
#include "timer_hw.h"
#include "hal_port.h"
#include <driver/hal/hal_timer_types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define TIMER_LL_REG_BASE(_timer_unit_id) (SOC_TIMER0_REG_BASE)
#define TIMER_LL_GROUP(chan) ((chan) / SOC_TIMER_CHAN_NUM_PER_GROUP)
#define TIMER_LL_CHAN(chan) ((chan) % SOC_TIMER_CHAN_NUM_PER_GROUP)

typedef enum {
	TIMER_GROUP_CHAN0 = 0,
	TIMER_GROUP_CHAN1 = 1,
	TIMRE_GROUP_CHAN2 = 2,
} timer_group_chan_t;

static inline void timer_ll_soft_reset(timer_hw_t *hw, uint32_t group)
{
	hw->group[group].global_ctrl.soft_reset = 1;
}

static inline uint32_t timer_ll_get_device_id(timer_hw_t *hw, uint32_t group)
{
	return hw->group[group].dev_id;
}

static inline uint32_t timer_ll_get_version_id(timer_hw_t *hw, uint32_t group)
{
	return hw->group[group].dev_version;
}

static inline uint32_t timer_ll_get_dev_status(timer_hw_t *hw, uint32_t group)
{
	return hw->group[group].dev_status;
}

static inline void timer_ll_init(timer_hw_t *hw, uint32_t chan)
{
#if defined(CONFIG_TIMER_SUPPORT_ID_BITS)
    if ((~CONFIG_TIMER_SUPPORT_ID_BITS) & BIT(chan)) {
        return;
    }
#endif

    uint32_t group = TIMER_LL_GROUP(chan);
    uint32_t group_chan = TIMER_LL_CHAN(chan);

    if (TIMER_GROUP_CHAN0 == group_chan) {
        hw->group[group].ctrl.timer0_en = 0;
        hw->group[group].ctrl.timer0_int_en = 0;
    } else if (TIMER_GROUP_CHAN1 == group_chan) {
        hw->group[group].ctrl.timer1_en = 0;
        hw->group[group].ctrl.timer1_int_en = 0;
    } else {
        hw->group[group].ctrl.timer2_en = 0;
        hw->group[group].ctrl.timer2_int_en = 0;
    }

	hw->group[group].read_ctrl.v = 0;
	hw->group[group].timer_read_value = 0;
	timer_ll_soft_reset(hw, group);
}

static inline void timer_ll_set_enable(timer_hw_t *hw, uint32_t group, uint32_t chan, uint32_t value)
{
	if (chan == TIMER_GROUP_CHAN0)
		hw->group[group].ctrl.timer0_en = value;
	else if (chan == TIMER_GROUP_CHAN1)
		hw->group[group].ctrl.timer1_en = value;
	else
		hw->group[group].ctrl.timer2_en = value;
}

static inline void timer_ll_enable(timer_hw_t *hw, uint32_t chan)
{
	timer_ll_set_enable(hw, TIMER_LL_GROUP(chan), TIMER_LL_CHAN(chan), 1);
}

static inline void timer_ll_disable(timer_hw_t *hw, uint32_t chan)
{
	timer_ll_set_enable(hw, TIMER_LL_GROUP(chan), TIMER_LL_CHAN(chan), 0);
}

static inline uint32_t timer_ll_get_enable_status(timer_hw_t *hw)
{
	uint32_t bit_pos = 0;
	uint32_t status = 0;

	for (int group = 0; group < SOC_TIMER_GROUP_NUM; group++) {
		status |= hw->group[group].ctrl.timer0_en << (bit_pos++);
		status |= hw->group[group].ctrl.timer1_en << (bit_pos++);
		status |= hw->group[group].ctrl.timer2_en << (bit_pos++);
	}

    return status;
}

static inline void timer_ll_set_interrupt(timer_hw_t *hw, uint32_t group, uint32_t chan, uint32_t value)
{
	if (chan == TIMER_GROUP_CHAN0)
		hw->group[group].ctrl.timer0_int_en = value;
	else if (chan == TIMER_GROUP_CHAN1)
		hw->group[group].ctrl.timer1_int_en = value;
	else
		hw->group[group].ctrl.timer2_int_en = value;
}

static inline void timer_ll_enable_interrupt(timer_hw_t *hw, uint32_t chan)
{
	timer_ll_set_interrupt(hw, TIMER_LL_GROUP(chan), TIMER_LL_CHAN(chan), 1);
}

static inline void timer_ll_disable_interrupt(timer_hw_t *hw, uint32_t chan)
{
	timer_ll_set_interrupt(hw, TIMER_LL_GROUP(chan), TIMER_LL_CHAN(chan), 0);
}

static inline bool timer_ll_is_interrupt_triggered(timer_hw_t *hw, uint32_t chan, uint32_t status)
{
	return (status & BIT(chan));
}

static inline void timer_ll_set_chan_interrupt_status(timer_hw_t *hw, uint32_t chan, uint32_t value)
{
    uint32_t group = TIMER_LL_GROUP(chan);
    uint32_t group_chan = TIMER_LL_CHAN(chan);

    if (group_chan == TIMER_GROUP_CHAN0)
        hw->group[group].ctrl.timer0_int_en = value;
    else if (group_chan == TIMER_GROUP_CHAN1)
        hw->group[group].ctrl.timer1_int_en = value;
    else
        hw->group[group].ctrl.timer2_int_en = value;
}

static inline uint32_t timer_ll_get_chan_interrupt_status(timer_hw_t *hw, uint32_t chan)
{
    uint32_t group = TIMER_LL_GROUP(chan);
    uint32_t group_chan = TIMER_LL_CHAN(chan);

    if (TIMER_GROUP_CHAN0 == group_chan)
        return hw->group[group].ctrl.timer0_int_en;
    else if (TIMER_GROUP_CHAN1 == group_chan)
        return hw->group[group].ctrl.timer1_int_en;
    else
        return hw->group[group].ctrl.timer2_int_en;
}

static inline void timer_ll_clear_chan_interrupt_status(timer_hw_t *hw, uint32_t chan)
{
    BK_DO_WHILE(timer_ll_set_chan_interrupt_status(hw, chan, 1),
                timer_ll_get_chan_interrupt_status(hw, chan));
}

static inline uint32_t timer_ll_get_interrupt_status(timer_hw_t *hw)
{
    uint32_t bit_pos = 0;
    uint32_t status = 0;

    for (int group = 0; group < SOC_TIMER_GROUP_NUM; group++) {
        status |= hw->group[group].ctrl.timer0_int_en << (bit_pos++);
        status |= hw->group[group].ctrl.timer1_int_en << (bit_pos++);
        status |= hw->group[group].ctrl.timer2_int_en << (bit_pos++);
    }

    return status;
}

static inline void timer_ll_clear_interrupt_status(timer_hw_t *hw, uint32_t status)
{
    for (int id = 0; id < SOC_TIMER_CHAN_NUM_PER_UNIT; id++) {
        if (status & BIT(id)) {
            timer_ll_clear_chan_interrupt_status(hw, id);
        }
    }
}

static inline void timer_ll_set_end_count(timer_hw_t *hw, uint32_t chan, uint32_t timer_cnt)
{
    hw->group[TIMER_LL_GROUP(chan)].timer_cnt[TIMER_LL_CHAN(chan)] = timer_cnt;
}

static inline uint32_t timer_ll_get_end_count(timer_hw_t *hw, uint32_t chan)
{
    return hw->group[TIMER_LL_GROUP(chan)].timer_cnt[TIMER_LL_CHAN(chan)];
}

static inline void timer_ll_set_clk_div(timer_hw_t *hw, uint32_t chan, uint32_t clk_div)
{
    hw->group[TIMER_LL_GROUP(chan)].ctrl.clk_div = clk_div & TIMER_F_CLK_DIV_V;
}

static inline void timer_ll_set_cnt_read(timer_hw_t *hw, uint32_t chan)
{
    hw->group[TIMER_LL_GROUP(chan)].read_ctrl.timer_cnt_read = 1;
}

static inline bool timer_ll_is_cnt_read_valid(timer_hw_t *hw, uint32_t chan)
{
    return !(hw->group[TIMER_LL_GROUP(chan)].read_ctrl.timer_cnt_read & 0x1);
}

static inline void timer_ll_set_read_index(timer_hw_t *hw, uint32_t chan)
{
    hw->group[TIMER_LL_GROUP(chan)].read_ctrl.timer_index = TIMER_LL_CHAN(chan);
}

static inline void timer_ll_clear_timer_count(timer_hw_t *hw, uint32_t chan)
{
    hw->group[TIMER_LL_GROUP(chan)].timer_read_value = 0;
}

static inline uint32_t timer_ll_get_timer_count(timer_hw_t *hw, uint32_t chan)
{
    return hw->group[TIMER_LL_GROUP(chan)].timer_read_value;
}

static inline void timer_ll_reset_config_to_default(timer_hw_t *hw, uint32_t chan)
{
    timer_ll_set_end_count(hw, chan, 0);
    timer_ll_set_clk_div(hw, chan, 0);
}

#if defined(CONFIG_TIMER_PM_CB_SUPPORT)
#define TIMER_CTRL_MASK            (0x7F)
#define TIMER_PM_BACKUP_REG_NUM    (5)

static inline void timer_ll_backup(timer_hw_t *hw, uint32_t group_id, uint32_t *pm_backup)
{
    pm_backup[0]  = hw->group[group_id].global_ctrl.v;
    pm_backup[1]  = hw->group[group_id].timer_cnt[0];
    pm_backup[2]  = hw->group[group_id].timer_cnt[1];
    pm_backup[3]  = hw->group[group_id].timer_cnt[2];
    pm_backup[4]  = hw->group[group_id].ctrl.v;
    //confs[5]  = hw->group[group_id].read_ctrl.v;

    for(uint32_t i = 0; i < SOC_TIMER_CHAN_NUM_PER_GROUP; i++) // disable timer after backup
    {
        timer_ll_set_enable(hw, group_id, i, 0);
    }
}

static inline void timer_ll_restore(timer_hw_t *hw, uint32_t group_id, uint32_t *pm_backup)
{
    hw->group[group_id].global_ctrl.v = pm_backup[0];
    hw->group[group_id].timer_cnt[0]  = pm_backup[1];
    hw->group[group_id].timer_cnt[1]  = pm_backup[2];
    hw->group[group_id].timer_cnt[2]  = pm_backup[3];
    hw->group[group_id].ctrl.v        = pm_backup[4];
    //hw->group[group_id].read_ctrl.v   = confs[5];
}
#endif

#ifdef __cplusplus
}
#endif
