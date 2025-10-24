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

// This is a generated file, if you need to modify it, use the script to
// generate and modify all the struct.h, ll.h, reg.h, debug_dump.c files!

#pragma once

#include <soc/soc.h>
#include "hal_port.h"
#include "ckmn_hw.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CKMN_LL_REG_BASE   SOC_CKMN_REG_BASE

//reg deviceid:

static inline uint32_t ckmn_ll_get_deviceid(ckmn_hw_t *hw)
{
	return hw->deviceid;
}

//reg versionid:

static inline uint32_t ckmn_ll_get_versionid(ckmn_hw_t *hw)
{
	return hw->versionid;
}

//reg global_ctrl:

static inline void ckmn_ll_set_global_ctrl(ckmn_hw_t *hw, uint32_t v)
{
	hw->global_ctrl.v = v;
}

static inline uint32_t ckmn_ll_get_global_ctrl(ckmn_hw_t *hw)
{
	return hw->global_ctrl.v;
}

static inline void ckmn_ll_set_soft_reset(ckmn_hw_t *hw)
{
	hw->global_ctrl.soft_reset &=  ~CKMN_CTRL_SOFT_RESET_MASK;
	hw->global_ctrl.soft_reset |= CKMN_CTRL_SOFT_RESET_MASK;
}

static inline void ckmn_ll_set_ckg_bypass(ckmn_hw_t *hw, uint32_t v)
{
	hw->global_ctrl.ckg_bypass = v & CKMN_CTRL_CKG_BYPASS_MASK;
}

//reg device_status:

static inline uint32_t ckmn_ll_get_device_status(ckmn_hw_t *hw)
{
	return hw->devicestatus;
}

//reg rc32k_counter:

static inline void ckmn_ll_set_rc32k_count(ckmn_hw_t *hw, uint32_t v)
{
	hw->rc32k_count.ckes_count = v & CKMN_RC32K_COUNT_MASK;
}

static inline uint32_t ckmn_ll_get_rc32k_count(ckmn_hw_t *hw)
{
	return hw->rc32k_count.ckes_count;
}

//reg rc32k_ctrl:

static inline void ckmn_ll_set_rc32k_ctrl(ckmn_hw_t *hw, uint32_t v) 
{
	hw->rc32k_ctrl.v = v;
}

static inline uint32_t ckmn_ll_get_rc32k_ctrl(ckmn_hw_t *hw)
{
	return hw->rc32k_ctrl.v;
}

static inline void ckmn_ll_rc32k_enable(ckmn_hw_t *hw)
{
	hw->rc32k_ctrl.ckes_enable |= CKMN_RC32K_CTRL_ENABLE_MASK;
}

static inline void ckmn_ll_rc32k_disable(ckmn_hw_t *hw)
{
	hw->rc32k_ctrl.ckes_enable &= ~(CKMN_RC32K_CTRL_ENABLE_MASK);
}

static inline void ckmn_ll_rc32k_intr_enable(ckmn_hw_t *hw)
{
	hw->rc32k_ctrl.ckes_intr_enable |= CKMN_RC32K_CTRL_INTR_ENABLE_MASK;
}

static inline void ckmn_ll_rc32k_intr_disable(ckmn_hw_t *hw)
{
	hw->rc32k_ctrl.ckes_intr_enable &= ~(CKMN_RC32K_CTRL_INTR_ENABLE_MASK);
}

//reg rc26m_count:

static inline uint32_t ckmn_ll_get_rc26m_count(ckmn_hw_t *hw)
{
	return hw->rc26m_count.ckes_number;
}

//reg corr_cfg:

static inline void ckmn_ll_set_corr_cfg(ckmn_hw_t *hw, uint32_t v)
{
	hw->corr_cfg.v = v;
}

static inline uint32_t ckmn_ll_get_corr_cfg(ckmn_hw_t *hw)
{
	return hw->corr_cfg.v;
}

static inline void ckmn_ll_set_26m_target(ckmn_hw_t *hw, uint32_t v)
{
	hw->corr_cfg.corr_26m_target = v & CKMN_26M_CORR_TARGET_MASK;
}

static inline uint32_t ckmn_ll_get_26m_target(ckmn_hw_t *hw)
{
	return hw->corr_cfg.corr_26m_target;
}

static inline void ckmn_ll_set_32k_target(ckmn_hw_t *hw, uint32_t v)
{
	hw->corr_cfg.corr_32k_target = v & CKMN_32K_CORR_TARGET_MASK;
}

static inline uint32_t ckmn_ll_get_32k_target(ckmn_hw_t *hw)
{
	return hw->corr_cfg.corr_32k_target;
}

static inline void ckmn_ll_autosw_26m_enable(ckmn_hw_t *hw)
{
	hw->corr_cfg.autosw_26m_enable |= CKMN_26M_AUTOSW_ENABLE_MASK;
}

static inline void ckmn_ll_autosw_26m_disable(ckmn_hw_t *hw)
{
	hw->corr_cfg.autosw_26m_enable &= ~(CKMN_26M_AUTOSW_ENABLE_MASK);
}

static inline void ckmn_ll_corr_26m_enable(ckmn_hw_t *hw)
{
	hw->corr_cfg.corr_26m_enable |= CKMN_26M_CORR_ENABLE_MASK;
}

static inline void ckmn_ll_corr_26m_disable(ckmn_hw_t *hw)
{
	hw->corr_cfg.corr_26m_enable &= ~(CKMN_26M_CORR_ENABLE_MASK);
}

static inline void ckmn_ll_autosw_32k_enable(ckmn_hw_t *hw)
{
	hw->corr_cfg.autosw_32k_enable |= CKMN_32K_AUTOSW_ENABLE_MASK;
}

static inline void ckmn_ll_autosw_32k_disable(ckmn_hw_t *hw)
{
	hw->corr_cfg.autosw_32k_enable &= ~(CKMN_32K_AUTOSW_ENABLE_MASK);
}

static inline void ckmn_ll_corr_32k_enable(ckmn_hw_t *hw)
{
	hw->corr_cfg.corr_32k_enable |= CKMN_32K_CORR_ENABLE_MASK;
}

static inline void ckmn_ll_corr_32k_disable(ckmn_hw_t *hw)
{
	hw->corr_cfg.corr_32k_enable &= ~(CKMN_32K_CORR_ENABLE_MASK);
}

static inline void ckmn_ll_26m_intr_enable(ckmn_hw_t *hw)
{
	hw->corr_cfg.cor26m_intr_enable |= CKMN_COR26M_INTR_ENABLE_MASK;
}

static inline void ckmn_ll_26m_intr_disable(ckmn_hw_t *hw)
{
	hw->corr_cfg.cor26m_intr_enable &= ~(CKMN_COR26M_INTR_ENABLE_MASK);
}

static inline void ckmn_ll_32k_intr_enable(ckmn_hw_t *hw)
{
	hw->corr_cfg.cor32k_intr_enable |= CKMN_COR32K_INTR_ENABLE_MASK;
}

static inline void ckmn_ll_32k_intr_disable(ckmn_hw_t *hw)
{
	hw->corr_cfg.cor32k_intr_enable &= ~(CKMN_COR32K_INTR_ENABLE_MASK);
}


//reg int_status:

static inline uint32_t ckmn_ll_get_intr_status(ckmn_hw_t *hw)
{
	return hw->intr.v;
}

static inline void ckmn_ll_clear_ckest_intr_status(ckmn_hw_t *hw)
{
	 hw->intr.ckest_intr_status |= CKMN_CKEST_INTR_STATUS_MASK;
}

static inline void ckmn_ll_clear_cor26m_intr_status(ckmn_hw_t *hw)
{
	hw->intr.cor26m_intr_status |= CKMN_COR26M_INTR_STATUS_MASK;
}

static inline void ckmn_ll_clear_cor32k_intr_status(ckmn_hw_t *hw)
{
	hw->intr.cor32k_intr_status |= CKMN_COR32K_INTR_STATUS_MASK;
}

static inline uint32_t ckmn_ll_get_ckest_intr_status(ckmn_hw_t *hw)
{
	return hw->intr.ckest_intr_status;
}

static inline uint32_t ckmn_ll_get_cor26m_intr_status(ckmn_hw_t *hw)
{
	return hw->intr.cor26m_intr_status;
}

static inline uint32_t ckmn_ll_get_cor32k_intr_status(ckmn_hw_t *hw)
{
	return hw->intr.cor32k_intr_status;
}

#ifdef __cplusplus
}
#endif
