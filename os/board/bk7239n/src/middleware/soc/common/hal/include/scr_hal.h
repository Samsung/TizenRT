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
#include "scr_hw.h"
#include "driver/scr_types.h"

void scr_hal_soft_reset(void);

void scr_hal_bypass_ckg(void);

void scr_hal_set_ctrl(scr_ctrl_idx_t ctrl_idx, uint32_t enable);

uint32_t scr_hal_get_ctrl(scr_ctrl_idx_t ctrl_idx);

void scr_hal_set_intr(scr_intr_idx_t intr_idx, uint32_t enable);

uint32_t scr_hal_get_intr(scr_intr_idx_t intr_idx);

uint32_t scr_hal_get_intr_status(scr_intr_idx_t intr_idx);

void scr_hal_clr_intr_status(scr_intr_idx_t intr_idx);

uint32_t scr_hal_check_intr_status(scr_intr_idx_t intr_idx, uint32_t intr_status);

/** FIFOCTRL start*/
uint32_t scr_hal_get_tx_fifo_empty(void);

uint32_t scr_hal_get_tx_fifo_full(void);

void scr_hal_set_tx_fifo_flush(void);

uint32_t scr_hal_get_rx_fifo_empty(void);

uint32_t scr_hal_get_rx_fifo_full(void);

void scr_hal_set_rx_fifo_flush(void);
/** FIFOCTRL end*/

void scr_hal_set_rx_fifo_threshold(uint32_t value);

uint32_t scr_hal_get_rx_fifo_threshold(void);

void scr_hal_set_tx_fifo_threshold(uint32_t value);

uint32_t scr_hal_get_tx_fifo_threshold(void);

void scr_hal_set_tx_repeat(uint32_t value);

uint32_t scr_hal_get_tx_repeat(void);

void scr_hal_set_rx_repeat(uint32_t value);

uint32_t scr_hal_get_rx_repeat(void);

void scr_hal_set_smart_card_clock_divisor(uint32_t value);

uint32_t scr_hal_get_smart_card_clock_divisor(void);

void scr_hal_set_baud_clock_divisor(uint32_t value);

uint32_t scr_hal_get_baud_clock_divisor(void);

void scr_hal_set_smart_card_guardtime(uint32_t value);

uint32_t scr_hal_get_smart_card_guardtime(void);

void scr_hal_set_active_deactive_time(uint32_t value);

uint32_t scr_hal_get_active_deactive_time(void);

void scr_hal_set_reset_duration(uint32_t value);

uint32_t scr_hal_get_reset_duration(void);

void scr_hal_set_ATR_start_limit(uint32_t value);

uint32_t scr_hal_get_ATR_start_limit(void);

void scr_hal_set_two_char_delay_limit(uint32_t value);

uint32_t scr_hal_get_two_char_delay_limit(void);

uint32_t scr_hal_get_tx_fifo_counter(void);

uint32_t scr_hal_get_rx_fifo_counter(void);

void scr_hal_set_baud_tune(uint32_t value);

uint32_t scr_hal_get_baud_tune(void);

uint32_t scr_hal_get_fifo_data(void);

void scr_hal_set_fifo_data(uint32_t data);

#ifdef __cplusplus
}
#endif


