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

#pragma once

#include <driver/sys_pm_types.h>
#include <sys_types.h>

//Buck
int sys_pm_set_buck(sys_buck_type_t buck, bool ena);
int sys_pm_set_buck_pfm(sys_buck_type_t buck, bool ena);
int sys_pm_set_buck_burst(sys_buck_type_t buck, bool ena);
int sys_pm_set_buck_mpo(sys_buck_type_t buck, bool ena);

int sys_pm_set_ldo_self_lp(sys_ldo_type_t ldo, bool ena);
int sys_pm_set_ldo_current_limit(sys_ldo_type_t ldo, bool ena);
int sys_pm_set_aon_power(sys_aon_power_t power);

int sys_pm_set_aon_ldo_volt(uint32_t volt);
int sys_pm_set_io_ldo_volt(uint32_t volt);
int sys_pm_set_ana_ldo_volt(bool trsw_ena, uint32_t rx_volt, uint32_t tx_volt);
int sys_pm_set_digital_ldo_volt(bool lp_ena, uint32_t low_volt, uint32_t high_volt);
int sys_pm_set_core_ldo_volt(bool lp_ena, uint32_t low_volt, uint32_t high_volt);

int sys_pm_set_lv_ctrl_pd(bool ena);
int sys_pm_set_lv_ctrl_hf(bool ena);
int sys_pm_set_lv_ctrl_flash(bool ena);
int sys_pm_set_lv_ctrl_core(bool ena);

int sys_pm_set_lpo_src(sys_lpo_src_t src);

void sys_pm_dump_ctrl(void);

void sys_pm_set_power(power_module_name_t module, power_module_state_t state);
uint32_t sys_pm_get_power(power_module_name_t module);


//ROSC
