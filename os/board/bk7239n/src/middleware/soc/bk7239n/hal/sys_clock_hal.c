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

#include <common/bk_include.h>
#include <driver/hal/hal_clock_types.h>
#include "aon_pmu_ll.h"
#include "sys_ll.h"
#include "sys_ana_ll.h"


#if defined(CONFIG_GPIO_CLOCK_PIN_SUPPORT)
void sys_hal_clk_pin_ana_open(clk_pin_ana_src_t src, clk_pin_div_t div, bool en)
{
    switch (src)
    {
    case CLK_PIN_ANA_ROSC:
        sys_ana_ll_set_ana_reg5_cktst_sel(0x0);
        sys_ana_ll_set_ana_reg4_ck_tst_en(en);
        sys_ana_ll_set_ana_reg5_rosc_tsten(en);
        break;

    case CLK_PIN_ANA_XTAL:
        sys_ana_ll_set_ana_reg5_en_xtall(1);
        sys_ana_ll_set_ana_reg5_cktst_sel(0x1);
        sys_ana_ll_set_ana_reg4_ck_tst_en(en);
        sys_ana_ll_set_ana_reg5_xtall_ten(en);
        break;

    case CLK_PIN_ANA_DPLL:
        sys_ana_ll_set_ana_reg5_cktst_sel(0x3);
        sys_ana_ll_set_ana_reg4_ck_tst_en(en);
        sys_ana_ll_set_ana_reg0_dpll_tsten(en);
        break;

    default:
        break;
    }
}

void sys_hal_clk_pin_dig_open(clk_pin_dig_src_t src, clk_pin_div_t div, bool en)
{
    switch (src)
    {
    case CLK_PIN_DIG_XTAL:
        sys_ll_set_cpu_26m_wdt_clk_div_ckdiv_xtal(0);
        break;
    case CLK_PIN_DIG_XTAL_DIV:
        sys_ll_set_cpu_26m_wdt_clk_div_ckdiv_xtal(div);
        break;

    case CLK_PIN_DIG_LPO_CLK_ROSC:
        aon_pmu_ll_set_r41_lpo_config(0x2);
        break;
    case CLK_PIN_DIG_LPO_CLK_XTALL:
        sys_ana_ll_set_ana_reg5_en_xtall(en);
        aon_pmu_ll_set_r41_lpo_config(0x1);
        break;
    case CLK_PIN_DIG_LPO_CLK_XTALH:
        aon_pmu_ll_set_r41_lpo_config(0x0);
        break;

    case CLK_PIN_DIG_CLK_AUXS_CLK_480M:
        sys_ll_set_cpu_clk_div_mode2_cksel_auxs(0x3);
        sys_ll_set_cpu_clk_div_mode2_ckdiv_auxs(div & 0xF);
        sys_ll_set_cpu_clk_div_mode2_clkdiv_auxs(div >> 4);
        sys_ll_set_cpu_device_clk_enable_auxs_cken(en);
        break;

    default:
        break;
    }
}
#endif