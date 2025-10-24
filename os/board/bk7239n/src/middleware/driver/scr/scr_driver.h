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

#include <components/log.h>
#include "driver/scr_types.h"

#define SCR_TAG "scr"
#define SCR_LOGI(...) BK_LOGI(SCR_TAG, ##__VA_ARGS__)
#define SCR_LOGW(...) BK_LOGW(SCR_TAG, ##__VA_ARGS__)
#define SCR_LOGE(...) BK_LOGE(SCR_TAG, ##__VA_ARGS__)
#define SCR_LOGD(...) BK_LOGD(SCR_TAG, ##__VA_ARGS__)

#define SCR_MAX_LEN           (0xFF)
#define SCR_FIFO_SIZE         (64)

typedef void (*scr_isr_t)(void *param);

void scr_isr(void);

bk_err_t bk_scr_gpio_init(gpio_scr_map_group_t group);

bk_err_t bk_scr_gpio_config(gpio_scr_map_group_t group, scr_gpio_type gpio_type);

bk_err_t bk_scr_init(gpio_scr_map_group_t group);

bk_err_t bk_scr_soft_reset(void);

bk_err_t bk_scr_bypass_ckg(void);

bk_err_t bk_scr_set_control(scr_ctrl_idx_t module, uint32_t value);

uint32_t bk_scr_get_control(scr_ctrl_idx_t module);

bk_err_t bk_scr_set_intr(scr_intr_idx_t module, uint32_t value, scr_isr_t isr, void *param);

uint32_t bk_scr_get_intr(scr_intr_idx_t module);

bk_err_t bk_scr_reset_intr_status(void);

uint32_t bk_scr_get_intr_status(void);

uint32_t bk_scr_check_intr_status(scr_intr_idx_t scr_idx, uint32_t intr_status);

uint32_t bk_scr_is_fifo_full(scr_type_t type);

uint32_t bk_scr_is_fifo_empty(scr_type_t type);

bk_err_t bk_scr_flush_fifo(scr_type_t type);

bk_err_t bk_scr_set_fifo_threshold(scr_type_t type, uint32_t value);

uint32_t bk_scr_get_fifo_threshold(scr_type_t type);

uint32_t bk_scr_get_fifo_counter(scr_type_t type);

bk_err_t bk_scr_set_repeat(scr_type_t type, uint32_t value);

uint32_t bk_scr_get_repeat(scr_type_t type);

bk_err_t bk_scr_set_sc_clk_div(uint32_t value);

uint32_t bk_scr_get_sc_clk_div(void);

bk_err_t bk_scr_set_baud_clk_div(uint32_t value);

uint32_t bk_scr_get_baud_clk_div(void);

bk_err_t bk_scr_set_sc_guardtime(uint32_t value);

bk_err_t bk_scr_set_act_deact_time(uint32_t value);

uint32_t bk_scr_get_act_deact_time(void);

bk_err_t bk_scr_set_reset_duration(uint32_t value);

uint32_t bk_scr_get_reset_duration(void);

bk_err_t bk_scr_set_ATR_start_limit(uint32_t value);

uint32_t bk_scr_get_ATR_start_limit(void);

bk_err_t bk_scr_set_c2c_delay_limit(uint32_t value);

uint32_t bk_scr_get_c2c_delay_limit(void);

bk_err_t bk_scr_set_baud_tune(uint32_t value);

uint32_t bk_scr_get_baud_tune(void);

uint32_t bk_scr_read_data(uint8_t *buf, uint32_t len);

bk_err_t bk_scr_write_data(uint8_t *buf, uint32_t len);

bk_err_t bk_scr_register_isr(scr_intr_idx_t scr_idx, scr_isr_t isr, void *param);

bk_err_t bk_scr_unregister_isr(scr_intr_idx_t scr_idx);


