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

#include <components/log.h>
#include <driver/hal/hal_io_matrix_types.h>

#define IOMX_TAG "iomx"
#define IOMX_LOGI(...) BK_LOGI(IOMX_TAG, ##__VA_ARGS__)
#define IOMX_LOGW(...) BK_LOGW(IOMX_TAG, ##__VA_ARGS__)
#define IOMX_LOGE(...) BK_LOGE(IOMX_TAG, ##__VA_ARGS__)
#define IOMX_LOGD(...) BK_LOGD(IOMX_TAG, ##__VA_ARGS__)

bk_err_t iomx_enable_multi_interrupts(uint64_t gpio_idx);
bk_err_t iomx_disable_all_interrupts(void);
bk_err_t iomx_get_interrupt_overview(void);
bk_err_t iomx_exec_iomx_gpio_isr(gpio_id_t gpio_id);

bk_err_t iomx_backup_gpio_configs(void);
bk_err_t iomx_restore_gpio_configs(void);
void iomx_dump_baked_regs(void);
void iomx_dump_regs(bool config, bool overview, bool atpg);

// eof

