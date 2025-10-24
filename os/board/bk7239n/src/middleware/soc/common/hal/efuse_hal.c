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

#include "efuse_hal.h"
#include "efuse_ll.h"
#include <driver/hal/hal_efuse_types.h>

static int efuse_hal_check_can_write(uint8_t old_byte, uint8_t new_byte)
{
	if (new_byte == old_byte) {
		/* no need to read */
		return -1;
	}

	for (int i = 0; i < 8; i++) {
		uint8_t old_bit = ((old_byte >> i) & 0x01);
		uint8_t new_bit = ((new_byte >> i) & 0x01);

		if ((old_bit) && (!new_bit)) {
			/* can not change old from 1 to 0 */
			HAL_LOGW("[efuse] cannot change bit[%d] from 1 to 0\r\n", i);
			return -2;
		}
	}

	return 0;
}

bk_err_t efuse_hal_init(efuse_hal_t *hal)
{
	hal->hw = (efuse_hw_t *)EFUSE_LL_REG_BASE(hal->id);
	efuse_ll_init(hal->hw);
	return BK_OK;
}

/* 1. check if can write
 * 2. enable efuse vdd2.5v
 * 3. set dir, addr, data, enable
 * 4. wait for operate finished
 * 5. disable efuse vdd2.5v
 * 6. check if write success
 */
bk_err_t efuse_hal_write(efuse_hal_t *hal, uint8_t addr, uint8_t wr_data)
{
	/* read before write, ensure this byte and this bit no wrote */
	int ret;
	uint8_t read_data = 0;

	ret = efuse_hal_read(hal, addr, &read_data);
	if(ret != BK_OK){
		return BK_ERR_EFUSE_READ_FAIL;
	}

	ret = efuse_hal_check_can_write(read_data, wr_data);
	if (ret == -1) {
		return BK_OK;
	}
	if (ret == -2) {
		return BK_ERR_EFUSE_CANNOT_WRTIE;
	}

	efuse_ll_enable_vdd25(hal->hw);

	efuse_ll_set_direction_write(hal->hw);
	efuse_ll_set_addr(hal->hw, addr);
	efuse_ll_set_wr_data(hal->hw, wr_data);
	efuse_ll_enable(hal->hw);

	BK_WHILE (!efuse_ll_is_operate_finished(hal->hw));

	efuse_ll_disable_vdd25(hal->hw);

	ret = efuse_hal_read(hal, addr, &read_data);
	if(ret != BK_OK){
		return BK_ERR_EFUSE_READ_FAIL;
	}

	if (read_data != wr_data) {
		return BK_ERR_EFUSE_WRTIE_NOT_EQUAL;
	}

	return BK_OK;
}

/* 1. set addr & direction=read
 * 2. enable efuse
 * 3. wait for efuse operate finished
 * 4. if rd_data valid, read data
 */
bk_err_t efuse_hal_read(efuse_hal_t *hal, uint8_t addr, uint8_t *data)
{
	efuse_ll_set_direction_read(hal->hw);
	efuse_ll_set_addr(hal->hw, addr);
	efuse_ll_enable(hal->hw);

	/* wait for efuse operate finished */
	BK_WHILE (!efuse_ll_is_operate_finished(hal->hw));

	if (efuse_ll_is_rd_data_valid(hal->hw)) {
		*data = efuse_ll_get_rd_data(hal->hw);
		return BK_OK;
	} else {
		return BK_ERR_EFUSE_READ_FAIL;
	}
}

