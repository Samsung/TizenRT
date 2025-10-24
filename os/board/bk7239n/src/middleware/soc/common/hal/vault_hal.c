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

#include "vault_hal.h"
#include "vault_ll.h"

bk_err_t vault_hal_init(vault_hal_t *hal)
{
	hal->hw = (vault_hw_t *)VAULT_LL_REG_BASE(hal->id);
	vault_ll_init(hal->hw);
	return BK_OK;
}

//extern void delay_us(UINT32 ms_count);
bk_err_t vault_hal_start_common(vault_hal_t *hal)
{
	vault_ll_set_OTP_power(hal->hw);
	//delay_us(70);
	vault_ll_soft_reset_disable(hal->hw);
	vault_ll_slv_reset(hal->hw);
	vault_ll_clk_man_reset(hal->hw);
	vault_ll_cm_reset(hal->hw);
	vault_ll_ctr_reset(hal->hw);
	return BK_OK;
}

bk_err_t vault_hal_stop_common(vault_hal_t *hal)
{
	vault_ll_clr_OTP_power(hal->hw);
	return BK_OK;
}

