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

#ifdef __cplusplus
extern "C" {
#endif

#include "hal_config.h"
#include "timer_hw.h"
#include "timer_ll.h"
#include <driver/hal/hal_timer_types.h>

typedef struct {
    timer_hw_t *hw;
    timer_unit_t id;
} timer_hal_t;

#define timer_hal_enable(hal, chan) timer_ll_enable((hal)->hw, chan)
#define timer_hal_disable(hal, chan) timer_ll_disable((hal)->hw, chan)
#define timer_hal_get_enable_status(hal) timer_ll_get_enable_status((hal)->hw)
#define timer_hal_get_chan_enable_status(hal, chan) timer_ll_get_chan_enable_status((hal)->hw, chan)
#define timer_hal_enable_interrupt(hal, chan) timer_ll_enable_interrupt((hal)->hw, chan)
#define timer_hal_disable_interrupt(hal, chan) timer_ll_disable_interrupt((hal)->hw, chan)
#define timer_hal_get_interrupt_status(hal) timer_ll_get_interrupt_status((hal)->hw)
#define timer_hal_clear_interrupt_status(hal, status) timer_ll_clear_interrupt_status((hal)->hw, status)
#define timer_hal_clear_chan_interrupt_status(hal, chan) timer_ll_clear_chan_interrupt_status((hal)->hw, chan)
#define timer_hal_is_interrupt_triggered(hal, chan, status) timer_ll_is_interrupt_triggered((hal)->hw, (chan), (status))
#define timer_hal_reset_config_to_default(hal, chan) timer_ll_reset_config_to_default((hal)->hw, (chan))
#define timer_hal_get_end_count(hal, chan) timer_ll_get_end_count((hal)->hw, chan)

bk_err_t timer_hal_init(timer_hal_t *hal);
bk_err_t timer_hal_init_timer(timer_hal_t *hal, timer_id_t chan, uint64_t time, timer_value_unit_t unit_type);
bk_err_t timer_hal_set_period(timer_hal_t *hal, timer_id_t chan, uint32_t time_ms);
bk_err_t timer_hal_start_common(timer_hal_t *hal, timer_id_t chan);
bk_err_t timer_hal_stop_common(timer_hal_t *hal, timer_id_t chan);
uint32_t timer_hal_get_count(timer_hal_t *hal, timer_id_t chan);

uint32_t timer_hal_cal_end_count(timer_id_t chan, uint64_t time, uint32_t div, timer_value_unit_t unit_type);
void timer_hal_us_init(void);
void timer_hal_delay_us(uint32_t us);

#if CFG_HAL_DEBUG_TIMER
void timer_struct_dump(void);
#else
#define timer_struct_dump()
#endif

#ifdef __cplusplus
}
#endif

