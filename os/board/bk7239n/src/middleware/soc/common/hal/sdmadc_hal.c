// Copyright 2022-2023 Beken
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

#include "sdmadc_hal.h"

bk_err_t sdmadc_hal_set_sample_mode(sdmadc_mode_t mode)
{
	if (mode == SDMADC_SINGLE_STEP_MODE) {
		sdmadc_ll_set_REG0x5_sample_mode(1);
		return BK_OK;
	} else if (mode == SDMADC_CONTINUOUS_MODE) {
		sdmadc_ll_set_REG0x5_sample_mode(0);
		return BK_OK;
	} else {
		return BK_FAIL;
	}
}

bk_err_t sdmadc_hal_set_sample_numb(sdmadc_sample_numb_t numb)
{
	if (numb == ONEPOINT_PER_STEP) {
		sdmadc_ll_set_REG0x5_sample_numb(0);
		return BK_OK;
	} else if (numb == TWOPOINT_PER_STEP) {
		sdmadc_ll_set_REG0x5_sample_numb(1);
		return BK_OK;
	} else if (numb == FOURPOINT_PER_STEP) {
		sdmadc_ll_set_REG0x5_sample_numb(2);
		return BK_OK;
	} else if (numb == EIGHTPOINT_PER_STEP) {
		sdmadc_ll_set_REG0x5_sample_numb(3);
		return BK_OK;
	} else {
		return BK_FAIL;
	}
}

bk_err_t sdmadc_hal_set_sample_status(uint32_t stat)
{
	sdmadc_ll_set_REG0x7_value(stat);
	return BK_OK;
}

bk_err_t sdmadc_hal_clear_interrupt_status(uint32 stat)
{
	sdmadc_ll_set_REG0x7_value(stat);
	return BK_OK;
}

bool smdadc_hal_is_fifo_empty_int_triggered()
{
	return !((sdmadc_ll_get_REG0x7_sadc_status() & 0x4) >> 2);
}

