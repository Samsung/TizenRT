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

#include "hal_config.h"
#include "lin_hw.h"
#include "lin_ll.h"
#include <driver/hal/hal_lin_types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	lin_hw_t *hw;
	lin_dev_t id;
} lin_hal_t;

void lin_hal_bypass_ckg(uint32_t v);
void lin_hal_master_cfg(lin_dev_t dev);
uint32_t lin_hal_get_master(void);
void lin_hal_set_data_byte(uint32_t index, uint8_t v);
uint8_t lin_hal_get_data_byte(uint32_t index);
void lin_hal_set_ctrl_start_req(void);
uint32_t lin_hal_get_ctrl_start_req(void);
void lin_hal_set_ctrl_wakeup(void);
uint32_t lin_hal_get_ctrl_wakeup(void);
void lin_hal_set_ctrl_reset_error(void);
void lin_hal_set_ctrl_reset_int(void);
void lin_hal_set_ctrl_data_ack(void);
uint32_t lin_hal_get_ctrl_data_ack(void);
void lin_hal_set_ctrl_transmit(uint32_t v);
uint32_t lin_hal_get_ctrl_transmit(void);
void lin_hal_set_ctrl_sleep(void);
uint32_t lin_hal_get_ctrl_sleep(void);
void lin_hal_set_ctrl_stop(void);
uint32_t lin_hal_get_status_value(void);
uint32_t lin_hal_get_status_complete(void);
uint32_t lin_hal_get_status_wake_up(void);
uint32_t lin_hal_get_status_error(void);
uint32_t lin_hal_get_status_int(void);
uint32_t lin_hal_get_status_data_req(void);
uint32_t lin_hal_get_status_aborted(void);
uint32_t lin_hal_get_status_bus_idle_timeout(void);
uint32_t lin_hal_get_status_lin_active(void);
/* reg :error */
uint32_t lin_hal_get_error_value(void);
uint32_t lin_hal_get_error_bit(void);
uint32_t lin_hal_get_error_chk(void);
uint32_t lin_hal_get_error_timeout(void);
uint32_t lin_hal_get_error_parity(void);
void lin_hal_set_prescl(uint32_t prescl);
uint32_t lin_hal_get_prescl(void);
void lin_hal_set_div(uint32_t div);
uint32_t lin_hal_get_div(void);
void lin_hal_set_mul(uint32_t mul);
uint32_t lin_hal_get_mul(void);
void lin_hal_set_ident_value(uint32_t ident);
uint32_t lin_hal_get_ident_value(void);
void lin_hal_set_ident_id(uint32_t id);
uint32_t lin_hal_get_ident_id(void);
void lin_hal_set_type_data_length(uint32_t data_length);
uint32_t lin_hal_get_type_data_length(void);
void lin_hal_set_type_enh_check(uint32_t enh_check);
uint32_t lin_hal_get_type_enh_check(void);
void lin_hal_set_tcfg_wup_repeat_time(uint32_t time);
uint32_t lin_hal_get_tcfg_wup_repeat_time(void);
void lin_hal_set_tcfg_bus_inactivity_time(uint32_t time);
uint32_t lin_hal_get_tcfg_bus_inactivity_time(void);
void lin_hal_set_rate(double rate);
double lin_hal_get_rate(void);
bk_err_t lin_hal_init(void);
bk_err_t lin_hal_deinit(void);

#if (defined(CONFIG_LIN_PM_CB_SUPPORT))
void lin_hal_backup(uint32_t *pm_backup);
void lin_hal_restore(uint32_t *pm_backup);
#endif

#if CFG_HAL_DEBUG_LIN
void lin_struct_dump(uint32_t start, uint32_t end);
#else
#define lin_struct_dump(start, end)
#endif

#ifdef __cplusplus
}
#endif

