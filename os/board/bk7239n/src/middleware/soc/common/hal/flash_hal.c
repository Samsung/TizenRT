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

#include "flash_hal.h"
#include "flash_ll.h"
#include <driver/hal/hal_flash_types.h>

bk_err_t flash_hal_init(flash_hal_t *hal)
{
	hal->hw = (flash_hw_t *)FLASH_LL_REG_BASE(hal->id);
	flash_ll_init(hal->hw);
	return BK_OK;
}

uint16_t flash_hal_get_protect_value(flash_hal_t *hal, uint8_t status_reg_size, uint8_t protect_post, uint8_t protect_mask, uint8_t cmp_post)
{
	uint16_t sr_value = flash_ll_read_status_reg(hal->hw, status_reg_size);
	uint16_t param = (sr_value >> protect_post) & protect_mask;
	uint16_t cmp = (sr_value >> cmp_post) & 0x01;
	uint16_t protect_value = (cmp << 8) | param;

	return protect_value;
}


