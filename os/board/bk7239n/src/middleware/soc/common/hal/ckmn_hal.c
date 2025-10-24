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

#include "ckmn_hal.h"
#include "ckmn_ll.h"

bk_err_t ckmn_hal_init(ckmn_hal_t *hal)
{
	hal->hw = (ckmn_hw_t *)SOC_CKMN_REG_BASE;
	ckmn_ll_set_soft_reset(hal->hw);

	return BK_OK;
}

bk_err_t ckmn_hal_set_soft_reset(ckmn_hal_t *hal)
{
	ckmn_ll_set_soft_reset(hal->hw);

	return BK_OK;
}

bk_err_t ckmn_hal_clk_bypass(ckmn_hal_t *hal, uint32_t v)
{
	ckmn_ll_set_ckg_bypass(hal->hw, v);

	return BK_OK;
}

//reg rc32k_counter:

__IRAM_SEC bk_err_t ckmn_hal_set_rc32k_count(ckmn_hal_t *hal, uint32_t v)
{
	ckmn_ll_set_rc32k_count(hal->hw, v);

	return BK_OK;
}

bk_err_t ckmn_hal_get_rc32k_count(ckmn_hal_t *hal)
{
	return ckmn_ll_get_rc32k_count(hal->hw);
}

__IRAM_SEC bk_err_t ckmn_hal_rc32k_enable(ckmn_hal_t *hal)
{
	ckmn_ll_rc32k_enable(hal->hw);

	return BK_OK;
}

bk_err_t ckmn_hal_rc32k_disable(ckmn_hal_t *hal)
{
	ckmn_ll_rc32k_disable(hal->hw);

	return BK_OK;
}

__IRAM_SEC bk_err_t ckmn_hal_rc32k_intr_enable(ckmn_hal_t *hal)
{

	ckmn_ll_rc32k_intr_enable(hal->hw);

	return BK_OK;
}

bk_err_t ckmn_hal_rc32k_intr_disable(ckmn_hal_t *hal)
{
	ckmn_ll_rc32k_intr_disable(hal->hw);

	return BK_OK;
}

//reg rc26m_count:

bk_err_t ckmn_hal_get_rc26m_count(ckmn_hal_t *hal)
{
	return ckmn_ll_get_rc26m_count(hal->hw);
}

//reg corr_cfg:

bk_err_t ckmn_hal_set_26m_target(ckmn_hal_t *hal, ckmn_acc_t target)
{
	ckmn_ll_set_26m_target(hal->hw, target);

	return BK_OK;
}

bk_err_t ckmn_hal_get_26m_target(ckmn_hal_t *hal)
{
	return ckmn_ll_get_26m_target(hal->hw);
}


bk_err_t ckmn_hal_autosw_26m_enable(ckmn_hal_t *hal)
{
	ckmn_ll_autosw_26m_enable(hal->hw);

	return BK_OK;
}

bk_err_t ckmn_hal_autosw_26m_disable(ckmn_hal_t *hal)
{
	ckmn_ll_autosw_26m_disable(hal->hw);

	return BK_OK;
}

bk_err_t ckmn_hal_corr_26m_enable(ckmn_hal_t *hal)
{
	ckmn_ll_corr_26m_enable(hal->hw);

	return BK_OK;
}

bk_err_t ckmn_hal_corr_26m_disable(ckmn_hal_t *hal)
{
	ckmn_ll_corr_26m_disable(hal->hw);

	return BK_OK;
}

bk_err_t ckmn_hal_set_32k_target(ckmn_hal_t *hal, ckmn_acc_t target)
{
	ckmn_ll_set_32k_target(hal->hw, target);

	return BK_OK;
}

bk_err_t ckmn_hal_get_32k_target(ckmn_hal_t *hal)
{
	return ckmn_ll_get_32k_target(hal->hw);
}

bk_err_t ckmn_hal_autosw_32k_enable(ckmn_hal_t *hal)
{

	ckmn_ll_autosw_32k_enable(hal->hw);


	return BK_OK;
}

bk_err_t ckmn_hal_autosw_32k_disable(ckmn_hal_t *hal)
{
	ckmn_ll_autosw_32k_disable(hal->hw);

	return BK_OK;
}

bk_err_t ckmn_hal_corr_32k_enable(ckmn_hal_t *hal)
{
	ckmn_ll_corr_32k_enable(hal->hw);

	return BK_OK;
}

bk_err_t ckmn_hal_corr_32k_disable(ckmn_hal_t *hal)
{
	ckmn_ll_corr_32k_disable(hal->hw);

	return BK_OK;
}

bk_err_t ckmn_hal_cor26m_intr_enable(ckmn_hal_t *hal)
{
	ckmn_ll_26m_intr_enable(hal->hw);

	return BK_OK;
}

bk_err_t ckmn_hal_cor26m_intr_disable(ckmn_hal_t *hal)
{
	ckmn_ll_26m_intr_disable(hal->hw);

	return BK_OK;
}

bk_err_t ckmn_hal_cor32k_intr_enable(ckmn_hal_t *hal)
{
	ckmn_ll_32k_intr_enable(hal->hw);

	return BK_OK;
}

bk_err_t ckmn_hal_cor32k_intr_disable(ckmn_hal_t *hal)
{
	ckmn_ll_32k_intr_disable(hal->hw);

	return BK_OK;
}

//reg int_status:

bk_err_t ckmn_hal_get_intr_status(ckmn_hal_t *hal)
{
	return ckmn_ll_get_intr_status(hal->hw);
}

bk_err_t ckmn_hal_clear_ckest_intr_status(ckmn_hal_t *hal)
{
	ckmn_ll_clear_ckest_intr_status(hal->hw);

	return BK_OK;
}

bk_err_t ckmn_hal_clear_cor26m_intr_status(ckmn_hal_t *hal)
{
	ckmn_ll_clear_cor26m_intr_status(hal->hw);

	return BK_OK;
}

bk_err_t ckmn_hal_clear_cor32k_intr_status(ckmn_hal_t *hal)
{
	ckmn_ll_clear_cor32k_intr_status(hal->hw);

	return BK_OK;
}

bk_err_t ckmn_hal_get_ckest_intr_status(ckmn_hal_t *hal)
{
	return ckmn_ll_get_ckest_intr_status(hal->hw);
}

bk_err_t ckmn_hal_get_cor26m_intr_status(ckmn_hal_t *hal)
{
	return ckmn_ll_get_cor26m_intr_status(hal->hw);
}

bk_err_t ckmn_hal_get_cor32k_intr_status(ckmn_hal_t *hal)
{
	return ckmn_ll_get_cor32k_intr_status(hal->hw);
}
