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

#include "timer_hal.h"
#include "timer_ll.h"
#include "sys_hal.h"

/*
 * timer_s = counter_value * (1 / (freq /div))
 */
uint32_t timer_hal_cal_end_count(timer_id_t chan, uint64_t time, uint32_t div, timer_value_unit_t unit_type)
{
	if (div == 0) {
		div = 1;
	}
	uint64_t value = 0;
	uint16_t unit_factor = 1;

	unit_factor = (unit_type == TIMER_UNIT_MS) ? 1 : 1000;

	uint32_t group_index = 0;
	uint32_t timer_clock = TIMER_SCLK_XTAL;

	group_index = chan / SOC_TIMER_CHAN_NUM_PER_GROUP;
	switch(group_index)
	{
		case 0:
			timer_clock = sys_hal_timer_select_clock_get(SYS_SEL_TIMER0);
			break;
		case 1:
			timer_clock = sys_hal_timer_select_clock_get(SYS_SEL_TIMER1);
			break;
		default:
			break;
	}

	if(timer_clock == TIMER_SCLK_XTAL) {
		value = time * TIMER_CLOCK_FREQ_XTAL / unit_factor / div;
	} else {
			value = time * TIMER_CLOCK_FREQ_32K / unit_factor / div;
	}

	if (value > 0xffffffff)
		value = 0xffffffff;

	return (uint32_t)value;
}

bk_err_t timer_hal_init(timer_hal_t *hal)
{
	hal->hw = (timer_hw_t *)TIMER_LL_REG_BASE(hal->id);

	for (int chan = 0; chan < SOC_TIMER_CHAN_NUM_PER_UNIT; chan++) {
		timer_ll_init(hal->hw, chan);
	}

	return BK_OK;
}

bk_err_t timer_hal_init_timer(timer_hal_t *hal, timer_id_t chan, uint64_t time, timer_value_unit_t unit_type)
{
	uint32_t end_count = timer_hal_cal_end_count(chan, time, 1, unit_type);
	timer_ll_set_end_count(hal->hw, chan, end_count);
	timer_ll_set_clk_div(hal->hw, chan, 0);
	timer_ll_clear_chan_interrupt_status(hal->hw, chan);
	return BK_OK;
}

bk_err_t timer_hal_set_period(timer_hal_t *hal, timer_id_t chan, uint32_t time_ms)
{
	uint32_t end_count = timer_hal_cal_end_count(chan, time_ms, 1, TIMER_UNIT_MS);
	timer_ll_set_end_count(hal->hw, chan, end_count);
	return BK_OK;
}


bk_err_t timer_hal_start_common(timer_hal_t *hal, timer_id_t chan)
{
	timer_ll_enable(hal->hw, chan);
	return BK_OK;
}

bk_err_t timer_hal_stop_common(timer_hal_t *hal, timer_id_t chan)
{
	timer_ll_disable_interrupt(hal->hw, chan);
	timer_ll_clear_chan_interrupt_status(hal->hw, chan);
	timer_ll_disable(hal->hw, chan);
	return BK_OK;
}

uint32_t timer_hal_get_count(timer_hal_t *hal, timer_id_t chan)
{
	uint32_t en_status = timer_ll_get_enable_status(hal->hw);
	if (!(en_status & BIT(chan))) {
		return 0;
	}
	timer_ll_set_read_index(hal->hw, chan);
	timer_ll_set_cnt_read(hal->hw, chan);

	//Wait hardware to prepare the data
	BK_WHILE (!timer_ll_is_cnt_read_valid(hal->hw, chan));

	return timer_ll_get_timer_count(hal->hw, chan);
}

