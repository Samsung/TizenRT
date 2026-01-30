// Copyright 2022-2025 Beken
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

#include <common/bk_include.h>
#include "sys_hal.h"
#include "sys_pm_hal_ctrl.h"
#include "sys_pm_hal_debug.h"

#if defined(CONFIG_PM_HAL_CTRL)

sys_pm_ctrl_t s_sys_pm = {
	.aon_ldo_self_lp       = 0,
	.io_ldo_self_lp        = 0,
	.ana_ldo_self_lp       = 0,
	.digital_ldo_self_lp   = 0,
	.core_ldo_self_lp      = 0,

	.aon_ldo_volt          = 6, // 0.9V
	.io_ldo_volt           = 2, // 3.1V
	.ana_trsw_en           = 1,
	.ana_ldo_rx_volt       = 4, // 1.45V
	.ana_ldo_tx_volt       = 4, // 1.45V
	.digital_ldo_lp        = 0,
	.digital_ldo_low_volt  = 3, // 0.6V
	.digital_ldo_high_volt = 6, // 1.0V
	.core_ldo_lp           = 0,
	.core_ldo_low_volt     = 7,  // 0.625V
	.core_ldo_high_volt    = 0xB,// 0.875V

	.io_ldo_current_limit  = 0,
	.ana_ldo_current_limit = 0,
	.aon_ldo_power         = 0, // VDDIO
	.io_ldo_current_limit  = 0,
	.ana_ldo_current_limit = 0,

	.lv_ctrl_pd = 1,
	.lv_ctrl_hf = 1,
	.lv_ctrl_flash = 1,
	.lv_ctrl_core = 1,
};

int sys_hal_set_buck(sys_buck_type_t buck, bool ena)
{
	PM_HAL_LOGD("set buck%d ena=%d\r\n", buck, !!ena);
	if (buck == SYS_BUCK_ANALOG) {
		sys_ll_set_ana_reg11_aldosel(!ena);
	} else if (buck == SYS_BUCK_DIGITAL) {
		sys_ll_set_ana_reg12_dldosel(!ena);
	} else if (buck == SYS_BUCK_PA) {
		sys_ll_set_ana_reg13_enbuckpa(!!ena);
	} else {
		return BK_ERR_PARAM;
	}

	return BK_OK;
}

int sys_hal_set_buck_pfm(sys_buck_type_t buck, bool ena)
{
	PM_HAL_LOGD("set buck%d pfm=%d\r\n", buck, !!ena);
	if (buck == SYS_BUCK_ANALOG) {
		sys_ll_set_ana_reg11_apfmen(!!ena);
	} else if (buck == SYS_BUCK_DIGITAL) {
		sys_ll_set_ana_reg12_dpfmen(!!ena);
	} else if (buck == SYS_BUCK_PA) {
		sys_ll_set_ana_reg13_papfmen(!!ena);
	} else {
		return BK_ERR_PARAM;
	}
	return BK_OK;
}

int sys_hal_set_buck_burst(sys_buck_type_t buck, bool ena)
{
	PM_HAL_LOGD("set buck%d burst=%d\r\n", buck, !!ena);
	if (buck == SYS_BUCK_ANALOG) {
		sys_ll_set_ana_reg11_apfmen(!!ena);
	} else if (buck == SYS_BUCK_DIGITAL) {
		sys_ll_set_ana_reg12_dpfmen(!!ena);
	} else if (buck == SYS_BUCK_PA) {
		sys_ll_set_ana_reg13_papfmen(!!ena);
	} else {
		return BK_ERR_PARAM;
	}
	return BK_OK;
}

int sys_hal_set_buck_mpo(sys_buck_type_t buck, bool ena)
{
	PM_HAL_LOGD("set buck%d mpo=%d\r\n", buck, !!ena);
	if (buck == SYS_BUCK_ANALOG) {
		sys_ll_set_ana_reg11_ampoen(!!ena);
	} else if (buck == SYS_BUCK_DIGITAL) {
		sys_ll_set_ana_reg12_dmpoen(!!ena);
	} else if (buck == SYS_BUCK_PA) {
		sys_ll_set_ana_reg13_dmpoen(!!ena);
	} else {
		return BK_ERR_PARAM;
	}
	return BK_OK;
}

int sys_hal_set_ldo_self_lp(sys_ldo_type_t ldo, bool ena)
{
	PM_HAL_LOGD("set ldo%d self_lp=%d\r\n", ldo, !!ena);

	switch (ldo) {
	case SYS_LDO_AON:
		s_sys_pm.aon_ldo_self_lp = !!ena;
		break;

	case SYS_LDO_IO:
		s_sys_pm.io_ldo_self_lp = !!ena;
		break;

	case SYS_LDO_ANALOG:
		s_sys_pm.ana_ldo_self_lp = !!ena;
		break;

	case SYS_LDO_DIGITAL:
		s_sys_pm.digital_ldo_self_lp = !!ena;
		break;

	case SYS_LDO_CORE:
		s_sys_pm.core_ldo_self_lp = !!ena;
		break;

	default:
		return BK_ERR_PARAM;
	}

	return BK_OK;
}

int sys_hal_set_ldo_current_limit(sys_ldo_type_t ldo, bool ena)
{
	PM_HAL_LOGD("set ldo%d current_limit=%d\r\n", ldo, !!ena);

	if (ldo == SYS_LDO_IO) {
		s_sys_pm.io_ldo_current_limit = !!ena;
	} else if (ldo == SYS_LDO_ANALOG) {
		s_sys_pm.ana_ldo_current_limit = !!ena;
	} else {
		return BK_ERR_PARAM;
	}
	return BK_OK;
}

int sys_hal_set_aon_power(sys_aon_power_t power)
{
	if (power == SYS_AON_POWER_VDDIO) {
		s_sys_pm.aon_ldo_power = 0;
	} else if (power == SYS_AON_POWER_VDDANA) {
		s_sys_pm.aon_ldo_power = 1;
	} else {
		return BK_ERR_PARAM;
	}

	return BK_OK;
}

static int sys_hal_get_volt_reg_value(const sys_pm_volt_reg_map_t *map, uint32_t size, uint32_t volt, uint8_t *reg_value)
{
	for (int idx = 0; idx < size; idx++) {
		if (volt == map[idx].volt_mv) {
			*reg_value = map[idx].reg_value;
			return BK_OK;
		}
	}
	return BK_FAIL;
}

int sys_hal_set_aon_ldo_volt(uint32_t volt)
{
	SYS_PM_AON_VOLT_MAP(map);
	uint8_t reg_value = 0;

	if (sys_hal_get_volt_reg_value(map, sizeof(map)/sizeof(sys_pm_volt_reg_map_t), volt, &reg_value) == BK_OK) {
		s_sys_pm.aon_ldo_volt = reg_value;
		PM_HAL_LOGD("set aon ldo volt=%u, reg=%u\r\n", volt, reg_value);
		return BK_OK;
	} else {
		return BK_ERR_PARAM;
	}
}

int sys_hal_set_io_ldo_volt(uint32_t volt)
{
	SYS_PM_IO_VOLT_MAP(map);
	uint8_t reg_value = 0;

	if (sys_hal_get_volt_reg_value(map, sizeof(map)/sizeof(sys_pm_volt_reg_map_t), volt, &reg_value) == BK_OK) {
		s_sys_pm.io_ldo_volt = reg_value;
		PM_HAL_LOGD("set io ldo volt=%u, reg=%u\r\n", volt, reg_value);
	} else {
		return BK_ERR_PARAM;
	}
	return BK_OK;
}

int sys_hal_set_ana_ldo_volt(bool trsw_ena, uint32_t rx_volt, uint32_t tx_volt)
{
	SYS_PM_ANA_VOLT_MAP(map);
	uint8_t reg_value = 0;

	if (sys_hal_get_volt_reg_value(map, sizeof(map)/sizeof(sys_pm_volt_reg_map_t), rx_volt, &reg_value) == BK_OK) {
		s_sys_pm.ana_ldo_rx_volt = reg_value;
		PM_HAL_LOGD("set ana ldo rx volt=%u, reg=%u\r\n", rx_volt, reg_value);
	} else {
		return BK_ERR_PARAM;
	}

	if (sys_hal_get_volt_reg_value(map, sizeof(map)/sizeof(sys_pm_volt_reg_map_t), tx_volt, &reg_value) == BK_OK) {
		s_sys_pm.ana_ldo_tx_volt = reg_value;
		PM_HAL_LOGD("set ana ldo tx volt=%u, reg=%u\r\n", tx_volt, reg_value);
	} else {
		return BK_ERR_PARAM;
	}

	s_sys_pm.ana_trsw_en = !!trsw_ena;
	PM_HAL_LOGD("set ana ldo trsw=%u\r\n", s_sys_pm.ana_trsw_en);
	return BK_OK;
}

int sys_hal_set_digital_ldo_volt(bool lp_ena, uint32_t low_volt, uint32_t high_volt)
{
	SYS_PM_DIGITAL_LOW_VOLT_MAP(low_map);
	SYS_PM_DIGITAL_HIGH_VOLT_MAP(high_map);
	uint8_t reg_value = 0;

	if (sys_hal_get_volt_reg_value(low_map, sizeof(low_map)/sizeof(sys_pm_volt_reg_map_t), low_volt, &reg_value) == BK_OK) {
		s_sys_pm.digital_ldo_low_volt = reg_value;
		PM_HAL_LOGD("set digital ldo low volt=%u, reg=%u\r\n", low_volt, reg_value);
	} else {
		return BK_ERR_PARAM;
	}

	if (sys_hal_get_volt_reg_value(high_map, sizeof(high_map)/sizeof(sys_pm_volt_reg_map_t), high_volt, &reg_value) == BK_OK) {
		s_sys_pm.digital_ldo_high_volt = reg_value;
		PM_HAL_LOGD("set digital ldo high volt=%u, reg=%u\r\n", high_volt, reg_value);
	} else {
		return BK_ERR_PARAM;
	}

	s_sys_pm.digital_ldo_lp = !!lp_ena;
	PM_HAL_LOGD("set digital ldo lp=%u\r\n", s_sys_pm.digital_ldo_lp);
	return BK_OK;
}

int sys_hal_set_core_ldo_volt(bool lp_ena, uint32_t low_volt, uint32_t high_volt)
{
	SYS_PM_CORE_LOW_VOLT_MAP(low_map);
	SYS_PM_CORE_HIGH_VOLT_MAP(high_map);
	uint8_t reg_value = 0;

	if (sys_hal_get_volt_reg_value(low_map, sizeof(low_map)/sizeof(sys_pm_volt_reg_map_t), low_volt, &reg_value) == BK_OK) {
		s_sys_pm.core_ldo_low_volt = reg_value;
		PM_HAL_LOGD("set core ldo low volt=%u, reg=%u\r\n", low_volt, reg_value);
	} else {
		return BK_ERR_PARAM;
	}

	if (sys_hal_get_volt_reg_value(high_map, sizeof(high_map)/sizeof(sys_pm_volt_reg_map_t), high_volt, &reg_value) == BK_OK) {
		s_sys_pm.core_ldo_high_volt = reg_value;
		PM_HAL_LOGD("set core ldo high volt=%u, reg=%u\r\n", high_volt, reg_value);
	} else {
		return BK_ERR_PARAM;
	}

	s_sys_pm.core_ldo_lp = !!lp_ena;
	PM_HAL_LOGD("set core ldo lp=%u\r\n", s_sys_pm.core_ldo_lp);
	return BK_OK;
}

int sys_hal_set_lv_ctrl_pd(bool ena)
{
	if (ena) {
		s_sys_pm.lv_ctrl_pd = 1;
	} else {
		s_sys_pm.lv_ctrl_pd = 0;
	}
	return BK_OK;
}

int sys_hal_set_lv_ctrl_hf(bool ena)
{
	if (ena) {
		s_sys_pm.lv_ctrl_hf = 1;
	} else {
		s_sys_pm.lv_ctrl_hf = 0;
	}
	return BK_OK;
}

int sys_hal_set_lv_ctrl_flash(bool ena)
{
	if (ena) {
		s_sys_pm.lv_ctrl_flash = 1;
	} else {
		s_sys_pm.lv_ctrl_flash = 0;
	}
	return BK_OK;
}

int sys_hal_set_lv_ctrl_core(bool ena)
{
	if (ena) {
		s_sys_pm.lv_ctrl_core = 1;
	} else {
		s_sys_pm.lv_ctrl_core = 0;
	}
	return BK_OK;
}

void sys_hal_dump_ctrl(void)
{
	PM_HAL_LOGD("%-24s = %d\r\n", "aon_ldo_self_lp", s_sys_pm.aon_ldo_self_lp);
	PM_HAL_LOGD("%-24s = %d\r\n", "io_ldo_self_lp", s_sys_pm.io_ldo_self_lp);
	PM_HAL_LOGD("%-24s = %d\r\n", "ana_ldo_self_lp", s_sys_pm.ana_ldo_self_lp);
	PM_HAL_LOGD("%-24s = %d\r\n", "digital_ldo_self_lp", s_sys_pm.digital_ldo_self_lp);
	PM_HAL_LOGD("%-24s = %d\r\n", "core_ldo_self_lp", s_sys_pm.core_ldo_self_lp);
	PM_HAL_LOGD("%-24s = %d\r\n", "aon_ldo_volt", s_sys_pm.aon_ldo_volt);
	PM_HAL_LOGD("%-24s = %d\r\n", "io_ldo_volt", s_sys_pm.io_ldo_volt);
	PM_HAL_LOGD("%-24s = %d\r\n", "ana_trsw_en", s_sys_pm.ana_trsw_en);
	PM_HAL_LOGD("%-24s = %d\r\n", "ana_ldo_rx_volt", s_sys_pm.ana_ldo_rx_volt);
	PM_HAL_LOGD("%-24s = %d\r\n", "ana_ldo_tx_volt", s_sys_pm.ana_ldo_tx_volt);
	PM_HAL_LOGD("%-24s = %d\r\n", "digital_ldo_lp", s_sys_pm.digital_ldo_lp);
	PM_HAL_LOGD("%-24s = %d\r\n", "digital_ldo_low_volt", s_sys_pm.digital_ldo_low_volt);
	PM_HAL_LOGD("%-24s = %d\r\n", "digital_ldo_high_volt", s_sys_pm.digital_ldo_high_volt);
	PM_HAL_LOGD("%-24s = %d\r\n", "core_ldo_lp", s_sys_pm.core_ldo_lp);
	PM_HAL_LOGD("%-24s = %d\r\n", "core_ldo_low_volt", s_sys_pm.core_ldo_low_volt);
	PM_HAL_LOGD("%-24s = %d\r\n", "core_ldo_high_volt", s_sys_pm.core_ldo_high_volt);
	PM_HAL_LOGD("%-24s = %d\r\n", "io_ldo_current_limit", s_sys_pm.io_ldo_current_limit);
	PM_HAL_LOGD("%-24s = %d\r\n", "ana_ldo_current_limit", s_sys_pm.ana_ldo_current_limit);
	PM_HAL_LOGD("%-24s = %d\r\n", "aon_ldo_power", s_sys_pm.aon_ldo_power);
	PM_HAL_LOGD("%-24s = %d\r\n", "io_ldo_current_limit", s_sys_pm.io_ldo_current_limit);
	PM_HAL_LOGD("%-24s = %d\r\n", "ana_ldo_current_limit", s_sys_pm.ana_ldo_current_limit);
	PM_HAL_LOGD("%-24s = %d\r\n", "lv_ctrl_pd", s_sys_pm.lv_ctrl_pd);
	PM_HAL_LOGD("%-24s = %d\r\n", "lv_ctrl_hf", s_sys_pm.lv_ctrl_hf);
	PM_HAL_LOGD("%-24s = %d\r\n", "lv_ctrl_flash", s_sys_pm.lv_ctrl_flash);
	PM_HAL_LOGD("%-24s = %d\r\n", "lv_ctrl_core", s_sys_pm.lv_ctrl_core);
}

#endif
