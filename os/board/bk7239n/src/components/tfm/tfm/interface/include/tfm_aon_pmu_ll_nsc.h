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
#include "aon_pmu_hw.h"

#ifdef __cplusplus
extern "C" {
#endif

#define AON_PMU_LL_REG_BASE   SOC_AON_PMU_REG_BASE

//reg r0:

void aon_pmu_ll_set_r0(uint32_t v);

uint32_t aon_pmu_ll_get_r0(void);

void aon_pmu_ll_set_r0_memchk_bps(uint32_t v);

uint32_t aon_pmu_ll_get_r0_memchk_bps(void);

void aon_pmu_ll_set_r0_fast_boot(uint32_t v);

uint32_t aon_pmu_ll_get_r0_fast_boot(void);

void aon_pmu_ll_set_r0_gpio_sleep(uint32_t v);

uint32_t aon_pmu_ll_get_r0_gpio_sleep(void);

//reg r1:

void aon_pmu_ll_set_r1(uint32_t v);

uint32_t aon_pmu_ll_get_r1(void);

uint32_t aon_pmu_ll_get_r1_touch_select(void);

void aon_pmu_ll_set_r1_touch_int_en(uint32_t v);

uint32_t aon_pmu_ll_get_r1_touch_int_en(void);

void aon_pmu_ll_set_r1_usbplug_int_en(uint32_t v);

uint32_t aon_pmu_ll_get_r1_usbplug_int_en(void);

uint32_t aon_pmu_ll_get_r1_clk_mod(void);

//reg r2:

void aon_pmu_ll_set_r2(uint32_t v);

uint32_t aon_pmu_ll_get_r2(void);

void aon_pmu_ll_set_r2_wdt_rst_ana(uint32_t v);

uint32_t aon_pmu_ll_get_r2_wdt_rst_ana(void);

void aon_pmu_ll_set_r2_wdt_rst_top(uint32_t v);

uint32_t aon_pmu_ll_get_r2_wdt_rst_top(void);

void aon_pmu_ll_set_r2_wdt_rst_aon(uint32_t v);

uint32_t aon_pmu_ll_get_r2_wdt_rst_aon(void);

void aon_pmu_ll_set_r2_wdt_rst_awt(uint32_t v);

uint32_t aon_pmu_ll_get_r2_wdt_rst_awt(void);

void aon_pmu_ll_set_r2_wdt_rst_gpio(uint32_t v);

uint32_t aon_pmu_ll_get_r2_wdt_rst_gpio(void);

void aon_pmu_ll_set_r2_wdt_rst_rtc(uint32_t v);

uint32_t aon_pmu_ll_get_r2_wdt_rst_rtc(void);

void aon_pmu_ll_set_r2_wdt_rst_wdt(uint32_t v);

uint32_t aon_pmu_ll_get_r2_wdt_rst_wdt(void);

void aon_pmu_ll_set_r2_wdt_rst_pmu(uint32_t v);

uint32_t aon_pmu_ll_get_r2_wdt_rst_pmu(void);

void aon_pmu_ll_set_r2_wdt_rst_phy(uint32_t v);

uint32_t aon_pmu_ll_get_r2_wdt_rst_phy(void);

//reg r3:

void aon_pmu_ll_set_r3(uint32_t v);

uint32_t aon_pmu_ll_get_r3(void);

void aon_pmu_ll_set_r3_sd_en(uint32_t v);

uint32_t aon_pmu_ll_get_r3_sd_en(void);

//reg r25:

void aon_pmu_ll_set_r25(uint32_t v);

uint32_t aon_pmu_ll_get_r25(void);

//reg r40:

void aon_pmu_ll_set_r40(uint32_t v);

uint32_t aon_pmu_ll_get_r40(void);

void aon_pmu_ll_set_r40_wake1_delay(uint32_t v);

uint32_t aon_pmu_ll_get_r40_wake1_delay(void);

void aon_pmu_ll_set_r40_wake2_delay(uint32_t v);

uint32_t aon_pmu_ll_get_r40_wake2_delay(void);

void aon_pmu_ll_set_r40_wake3_delay(uint32_t v);

uint32_t aon_pmu_ll_get_r40_wake3_delay(void);

void aon_pmu_ll_set_r40_halt1_delay(uint32_t v);

uint32_t aon_pmu_ll_get_r40_halt1_delay(void);

void aon_pmu_ll_set_r40_halt2_delay(uint32_t v);

uint32_t aon_pmu_ll_get_r40_halt2_delay(void);

void aon_pmu_ll_set_r40_halt3_delay(uint32_t v);

uint32_t aon_pmu_ll_get_r40_halt3_delay(void);

void aon_pmu_ll_set_r40_halt_volt(uint32_t v);

uint32_t aon_pmu_ll_get_r40_halt_volt(void);

void aon_pmu_ll_set_r40_halt_xtal(uint32_t v);

uint32_t aon_pmu_ll_get_r40_halt_xtal(void);

void aon_pmu_ll_set_r40_halt_core(uint32_t v);

uint32_t aon_pmu_ll_get_r40_halt_core(void);

void aon_pmu_ll_set_r40_halt_flash(uint32_t v);

uint32_t aon_pmu_ll_get_r40_halt_flash(void);

void aon_pmu_ll_set_r40_halt_rosc(uint32_t v);

uint32_t aon_pmu_ll_get_r40_halt_rosc(void);

void aon_pmu_ll_set_r40_halt_resten(uint32_t v);

uint32_t aon_pmu_ll_get_r40_halt_resten(void);

void aon_pmu_ll_set_r40_halt_isolat(uint32_t v);

uint32_t aon_pmu_ll_get_r40_halt_isolat(void);

void aon_pmu_ll_set_r40_halt_clkena(uint32_t v);

uint32_t aon_pmu_ll_get_r40_halt_clkena(void);

//reg r41:

void aon_pmu_ll_set_r41(uint32_t v);

uint32_t aon_pmu_ll_get_r41(void);

void aon_pmu_ll_set_r41_lpo_config(uint32_t v);

uint32_t aon_pmu_ll_get_r41_lpo_config(void);

void aon_pmu_ll_set_r41_flshsck_iocap(uint32_t v);

uint32_t aon_pmu_ll_get_r41_flshsck_iocap(void);

void aon_pmu_ll_set_r41_wakeup_ena(uint32_t v);

uint32_t aon_pmu_ll_get_r41_wakeup_ena(void);

void aon_pmu_ll_set_r41_io_drv(uint32_t v);

uint32_t aon_pmu_ll_get_r41_io_drv(void);

void aon_pmu_ll_set_r41_xtal_sel(uint32_t v);

uint32_t aon_pmu_ll_get_r41_xtal_sel(void);

void aon_pmu_ll_set_r41_halt_lpo(uint32_t v);

uint32_t aon_pmu_ll_get_r41_halt_lpo(void);

void aon_pmu_ll_set_r41_halt_busrst(uint32_t v);

uint32_t aon_pmu_ll_get_r41_halt_busrst(void);

void aon_pmu_ll_set_r41_halt_busiso(uint32_t v);

uint32_t aon_pmu_ll_get_r41_halt_busiso(void);

void aon_pmu_ll_set_r41_halt_buspwd(uint32_t v);

uint32_t aon_pmu_ll_get_r41_halt_buspwd(void);

void aon_pmu_ll_set_r41_halt_blpiso(uint32_t v);

uint32_t aon_pmu_ll_get_r41_halt_blpiso(void);

void aon_pmu_ll_set_r41_halt_blppwd(uint32_t v);

uint32_t aon_pmu_ll_get_r41_halt_blppwd(void);

void aon_pmu_ll_set_r41_halt_wlpiso(uint32_t v);

uint32_t aon_pmu_ll_get_r41_halt_wlpiso(void);

void aon_pmu_ll_set_r41_halt_wlppwd(uint32_t v);

uint32_t aon_pmu_ll_get_r41_halt_wlppwd(void);

//reg r43:

void aon_pmu_ll_set_r43(uint32_t v);

uint32_t aon_pmu_ll_get_r43(void);

void aon_pmu_ll_set_r43_clr_int_touched(uint32_t v);

uint32_t aon_pmu_ll_get_r43_clr_int_touched(void);

void aon_pmu_ll_set_r43_clr_int_usbplug(uint32_t v);

uint32_t aon_pmu_ll_get_r43_clr_int_usbplug(void);

void aon_pmu_ll_set_r43_clr_wakeup(uint32_t v);

uint32_t aon_pmu_ll_get_r43_clr_wakeup(void);

//reg r70:

void aon_pmu_ll_set_r70(uint32_t v);

uint32_t aon_pmu_ll_get_r70(void);

uint32_t aon_pmu_ll_get_r70_int_touched(void);

uint32_t aon_pmu_ll_get_r70_int_usbplug(void);

//reg r71:

void aon_pmu_ll_set_r71(uint32_t v);

uint32_t aon_pmu_ll_get_r71(void);

uint32_t aon_pmu_ll_get_r71_touch_state(void);

uint32_t aon_pmu_ll_get_r71_usbplug_state(void);

uint32_t aon_pmu_ll_get_r71_wakeup_source(void);

//reg reg72:

void aon_pmu_ll_set_reg72_value(uint32_t v);

uint32_t aon_pmu_ll_get_reg72_value(void);

uint32_t aon_pmu_ll_get_reg72_vdw_soft1v(void);

uint32_t aon_pmu_ll_get_reg72_vup_soft1v(void);

//reg reg73:

void aon_pmu_ll_set_reg73_value(uint32_t v);

uint32_t aon_pmu_ll_get_reg73_value(void);

uint32_t aon_pmu_ll_get_reg73_mul_touch_int(void);

uint32_t aon_pmu_ll_get_reg73_wake_up_soft(void);

uint32_t aon_pmu_ll_get_reg73_cap_cal(void);

uint32_t aon_pmu_ll_get_reg73_cal_done(void);

//reg r7c:

void aon_pmu_ll_set_r7c(uint32_t v);

uint32_t aon_pmu_ll_get_r7c(void);

uint32_t aon_pmu_ll_get_r7c_id(void);

//reg r7d:

void aon_pmu_ll_set_r7d(uint32_t v);

uint32_t aon_pmu_ll_get_r7d(void);

uint32_t aon_pmu_ll_get_r7d_lcal(void);

uint32_t aon_pmu_ll_get_r7d_l(void);

uint32_t aon_pmu_ll_get_r7d_adc_cal(void);

uint32_t aon_pmu_ll_get_r7d_bgcal(void);

uint32_t aon_pmu_ll_get_r7d_26mpll_unlock(void);

uint32_t aon_pmu_ll_get_r7d_dpll_unlock(void);

uint32_t aon_pmu_ll_get_r7d_h1(void);

//reg r7e:

void aon_pmu_ll_set_r7e(uint32_t v);

uint32_t aon_pmu_ll_get_r7e(void);

uint32_t aon_pmu_ll_get_r7e_cbcal(void);

uint32_t aon_pmu_ll_get_r7e_ad_state(void);

uint32_t aon_pmu_ll_get_r7e_td_states1(void);

uint32_t aon_pmu_ll_get_r7e_h2(void);

#ifdef __cplusplus
}
#endif
