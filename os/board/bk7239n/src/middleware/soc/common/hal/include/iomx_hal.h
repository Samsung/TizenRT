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
#include "iomx_hw.h"
#include "iomx_ll.h"

#include <driver/hal/hal_io_matrix_types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	iomx_hw_t *hw;
	iomx_unit_t id;
} iomx_hal_t;

#define IOMUX_CFG_GPIO_CNT             (4)
#define IOMUX_CFG_GPIO_FIELD_BITS      (8)
#define IOMUX_FUNC_CODE_MASK           (0xFF)

bk_err_t iomx_hal_init(void);
bk_err_t iomx_hal_deinit(void);
void iomx_hal_set_value(gpio_id_t id, uint32_t v);
uint32_t iomx_hal_get_value(gpio_id_t id);
bk_err_t iomx_hal_set_func_code(gpio_id_t gpio_id, uint32_t code);
uint32_t iomx_hal_get_func_code(gpio_id_t gpio_id);
bk_err_t iomx_hal_pull_up_enable(gpio_id_t gpio_id, uint32 enable);
bk_err_t iomx_hal_pull_enable(gpio_id_t gpio_id, uint32 enable);
bk_err_t iomx_hal_monitor_input_enable(gpio_id_t gpio_id, uint32 enable);
bk_err_t iomx_hal_set_capacity(gpio_id_t gpio_id, uint32 capacity);
bk_err_t iomx_hal_set_output_value(gpio_id_t gpio_id, uint32 output_value);
bk_err_t iomx_hal_get_input(gpio_id_t gpio_id);
bk_err_t iomx_hal_get_output(gpio_id_t gpio_id);
bk_err_t iomx_hal_set_int_type(gpio_id_t gpio_id, uint32_t type);
bk_err_t iomx_hal_enable_interrupt(gpio_id_t gpio_id);
bk_err_t iomx_hal_enable_multi_interrupts(uint64_t gpio_idx);
bk_err_t iomx_hal_disable_all_interrupts(void);
bk_err_t iomx_hal_get_interrupt_overview(void);
bk_err_t iomx_hal_bakup_configs(uint32_t *iomx_gpio_cfgs);
bk_err_t iomx_hal_restore_configs(uint32_t *iomx_gpio_cfgs);

#define iomx_hal_disable_interrupt(id)                  iomx_ll_set_gpio_interrupt_ena(0, id)

#define iomx_hal_get_interrupt_status(status)           iomx_ll_get_interrupt_status(status)
#define iomx_hal_clear_interrupt_status(tatus)
#define iomx_hal_is_interrupt_triggered(id, status)     iomx_ll_is_interrupt_triggered(id, status)
#define iomx_hal_clear_chan_interrupt_status(id)        iomx_ll_set_gpio_interrupt_clear(1, id); \
                                                        iomx_ll_set_gpio_interrupt_clear(0, id) // workaround fix

#if CFG_HAL_DEBUG_IOMX
void gpio_regs_dump(void);
void gpio_struct_dump(gpio_id_t gpio_index);
void iomx_struct_dump(uint32_t start, uint32_t end);
#else
#define gpio_regs_dump()
#define gpio_struct_dump()
#define iomx_struct_dump()
#endif

#ifdef __cplusplus
}
#endif

