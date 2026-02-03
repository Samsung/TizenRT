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

// This is a generated file, if you need to modify it, use the script to
// generate and modify all the struct.h, ll.h, reg.h, debug_dump.c files!

#pragma once

#include <soc/soc.h>
#include "hal_port.h"
#include "aon_pmu_hw.h"

#if defined(CONFIG_TFM_AON_PMU_LL_NSC) && (CONFIG_TFM_AON_PMU_LL_NSC == 1)
#include "tfm_aon_pmu_ll_nsc.h"
#else

#ifdef __cplusplus
extern "C" {
#endif

#define AON_PMU_LL_REG_BASE   SOC_AON_PMU_REG_BASE

//reg r0:

static inline void aon_pmu_ll_set_r0(uint32_t v) {
	aon_pmu_r0_t *r = (aon_pmu_r0_t*)(SOC_AON_PMU_REG_BASE + (0x0 << 2));
	r->v = v;
}

static inline uint32_t aon_pmu_ll_get_r0(void) {
	aon_pmu_r0_t *r = (aon_pmu_r0_t*)(SOC_AON_PMU_REG_BASE + (0x0 << 2));
	return r->v;
}

static inline void aon_pmu_ll_set_r0_memchk_bps(uint32_t v) {
	aon_pmu_r0_t *r = (aon_pmu_r0_t*)(SOC_AON_PMU_REG_BASE + (0x0 << 2));
	r->memchk_bps = v;
}

static inline uint32_t aon_pmu_ll_get_r0_memchk_bps(void) {
	aon_pmu_r0_t *r = (aon_pmu_r0_t*)(SOC_AON_PMU_REG_BASE + (0x0 << 2));
	return r->memchk_bps;
}

static inline uint32_t aon_pmu_ll_get_r7b_memchk_bps(void) {
	aon_pmu_r0_t *r = (aon_pmu_r0_t*)(SOC_AON_PMU_REG_BASE + (0x7b << 2));
	return r->memchk_bps;
}

static inline void aon_pmu_ll_set_r0_fast_boot(uint32_t v) {
	aon_pmu_r0_t *r = (aon_pmu_r0_t*)(SOC_AON_PMU_REG_BASE + (0x0 << 2));
	r->fast_boot = v;
}

static inline uint32_t aon_pmu_ll_get_r0_fast_boot(void) {
	aon_pmu_r0_t *r = (aon_pmu_r0_t*)(SOC_AON_PMU_REG_BASE + (0x0 << 2));
	return r->fast_boot;
}

static inline uint32_t aon_pmu_ll_get_r7b_fast_boot(void) {
	aon_pmu_r0_t *r = (aon_pmu_r0_t*)(SOC_AON_PMU_REG_BASE + (0x7b << 2));
	return r->fast_boot;
}

static inline void aon_pmu_ll_set_r0_dig_wake_en(uint32_t v) {
	aon_pmu_r0_t *r = (aon_pmu_r0_t*)(SOC_AON_PMU_REG_BASE + (0x0 << 2));
	r->dig_wake_en = v;
}

static inline uint32_t aon_pmu_ll_get_r0_dig_wake_en(void) {
	aon_pmu_r0_t *r = (aon_pmu_r0_t*)(SOC_AON_PMU_REG_BASE + (0x0 << 2));
	return r->dig_wake_en;
}

static inline uint32_t aon_pmu_ll_get_r7b_dig_wake_en(void) {
	aon_pmu_r0_t *r = (aon_pmu_r0_t*)(SOC_AON_PMU_REG_BASE + (0x7b << 2));
	return r->dig_wake_en;
}

static inline void aon_pmu_ll_set_r0_reset_reason(uint32_t v) {
	aon_pmu_r0_t *r = (aon_pmu_r0_t*)(SOC_AON_PMU_REG_BASE + (0x0 << 2));
	r->reset_reason = v;
}

static inline uint32_t aon_pmu_ll_get_r0_reset_reason(void) {
	aon_pmu_r0_t *r = (aon_pmu_r0_t*)(SOC_AON_PMU_REG_BASE + (0x0 << 2));
	return r->reset_reason;
}

static inline uint32_t aon_pmu_ll_get_r7a_reset_reason(void) {
	aon_pmu_r0_t *r = (aon_pmu_r0_t*)(SOC_AON_PMU_REG_BASE + (0x7a << 2));
	return r->reset_reason;
}

static inline uint32_t aon_pmu_ll_get_r7b_reset_reason(void) {
	aon_pmu_r0_t *r = (aon_pmu_r0_t*)(SOC_AON_PMU_REG_BASE + (0x7b << 2));
	return r->reset_reason;
}

static inline void aon_pmu_ll_set_r0_gpio_sleep(uint32_t v) {
	aon_pmu_r0_t *r = (aon_pmu_r0_t*)(SOC_AON_PMU_REG_BASE + (0x0 << 2));
	r->gpio_sleep = v;
}

static inline uint32_t aon_pmu_ll_get_r0_gpio_sleep(void) {
	aon_pmu_r0_t *r = (aon_pmu_r0_t*)(SOC_AON_PMU_REG_BASE + (0x0 << 2));
	return r->gpio_sleep;
}

static inline uint32_t aon_pmu_ll_get_r7b_gpio_sleep(void) {
	aon_pmu_r0_t *r = (aon_pmu_r0_t*)(SOC_AON_PMU_REG_BASE + (0x7b << 2));
	return r->gpio_sleep;
}

static inline void aon_pmu_ll_set_r0_dlv_startup(uint32_t v) {
	aon_pmu_r0_t *r = (aon_pmu_r0_t*)(SOC_AON_PMU_REG_BASE + (0x0 << 2));
	r->dlv_startup = v;
}

static inline uint32_t aon_pmu_ll_get_r0_dlv_startup(void) {
	aon_pmu_r0_t *r = (aon_pmu_r0_t*)(SOC_AON_PMU_REG_BASE + (0x0 << 2));
	return r->dlv_startup;
}

static inline uint32_t aon_pmu_ll_get_r7b_dlv_startup(void) {
	aon_pmu_r0_t *r = (aon_pmu_r0_t*)(SOC_AON_PMU_REG_BASE + (0x7b << 2));
	return r->dlv_startup;
}

static inline void aon_pmu_ll_set_r0_saved_time(uint32_t v) {
	aon_pmu_r0_t *r = (aon_pmu_r0_t*)(SOC_AON_PMU_REG_BASE + (0x0 << 2));
	r->saved_time = v;
}

static inline uint32_t aon_pmu_ll_get_r0_saved_time(void) {
	aon_pmu_r0_t *r = (aon_pmu_r0_t*)(SOC_AON_PMU_REG_BASE + (0x0 << 2));
	return r->saved_time;
}

static inline uint32_t aon_pmu_ll_get_r7b_saved_time(void) {
	aon_pmu_r0_t *r = (aon_pmu_r0_t*)(SOC_AON_PMU_REG_BASE + (0x7b << 2));
	return r->saved_time;
}

//reg r2:

static inline void aon_pmu_ll_set_r2(uint32_t v) {
	aon_pmu_r2_t *r = (aon_pmu_r2_t*)(SOC_AON_PMU_REG_BASE + (0x2 << 2));
	r->v = v;
}

static inline uint32_t aon_pmu_ll_get_r2(void) {
	aon_pmu_r2_t *r = (aon_pmu_r2_t*)(SOC_AON_PMU_REG_BASE + (0x2 << 2));
	return r->v;
}

static inline void aon_pmu_ll_set_r2_wdt_rst_ana(uint32_t v) {
	aon_pmu_r2_t *r = (aon_pmu_r2_t*)(SOC_AON_PMU_REG_BASE + (0x2 << 2));
	r->wdt_rst_ana = v;
}

static inline uint32_t aon_pmu_ll_get_r2_wdt_rst_ana(void) {
	aon_pmu_r2_t *r = (aon_pmu_r2_t*)(SOC_AON_PMU_REG_BASE + (0x2 << 2));
	return r->wdt_rst_ana;
}

static inline void aon_pmu_ll_set_r2_wdt_rst_top(uint32_t v) {
	aon_pmu_r2_t *r = (aon_pmu_r2_t*)(SOC_AON_PMU_REG_BASE + (0x2 << 2));
	r->wdt_rst_top = v;
}

static inline uint32_t aon_pmu_ll_get_r2_wdt_rst_top(void) {
	aon_pmu_r2_t *r = (aon_pmu_r2_t*)(SOC_AON_PMU_REG_BASE + (0x2 << 2));
	return r->wdt_rst_top;
}

static inline void aon_pmu_ll_set_r2_wdt_rst_aon(uint32_t v) {
	aon_pmu_r2_t *r = (aon_pmu_r2_t*)(SOC_AON_PMU_REG_BASE + (0x2 << 2));
	r->wdt_rst_aon = v;
}

static inline uint32_t aon_pmu_ll_get_r2_wdt_rst_aon(void) {
	aon_pmu_r2_t *r = (aon_pmu_r2_t*)(SOC_AON_PMU_REG_BASE + (0x2 << 2));
	return r->wdt_rst_aon;
}

static inline void aon_pmu_ll_set_r2_rtc_value_samp_sel(uint32_t v) {
	aon_pmu_r2_t *r = (aon_pmu_r2_t*)(SOC_AON_PMU_REG_BASE + (0x2 << 2));
	r->rtc_value_samp_sel = v;
}

static inline uint32_t aon_pmu_ll_get_r2_rtc_value_samp_sel(void) {
	aon_pmu_r2_t *r = (aon_pmu_r2_t*)(SOC_AON_PMU_REG_BASE + (0x2 << 2));
	return r->rtc_value_samp_sel;
}

static inline void aon_pmu_ll_set_r2_otp_vdd_en(uint32_t v) {
	aon_pmu_r2_t *r = (aon_pmu_r2_t*)(SOC_AON_PMU_REG_BASE + (0x2 << 2));
	r->otp_vdd_en = v;
}

static inline uint32_t aon_pmu_ll_get_r2_otp_vdd_en(void) {
	aon_pmu_r2_t *r = (aon_pmu_r2_t*)(SOC_AON_PMU_REG_BASE + (0x2 << 2));
	return r->otp_vdd_en;
}

//reg r25:

static inline void aon_pmu_ll_set_r25(uint32_t v) {
	aon_pmu_r25_t *r = (aon_pmu_r25_t*)(SOC_AON_PMU_REG_BASE + (0x25 << 2));
	r->v = v;
}

static inline uint32_t aon_pmu_ll_get_r25(void) {
	aon_pmu_r25_t *r = (aon_pmu_r25_t*)(SOC_AON_PMU_REG_BASE + (0x25 << 2));
	return r->v;
}

//reg r40:

static inline void aon_pmu_ll_set_r40(uint32_t v) {
	aon_pmu_r40_t *r = (aon_pmu_r40_t*)(SOC_AON_PMU_REG_BASE + (0x40 << 2));
	r->v = v;
}

static inline uint32_t aon_pmu_ll_get_r40(void) {
	aon_pmu_r40_t *r = (aon_pmu_r40_t*)(SOC_AON_PMU_REG_BASE + (0x40 << 2));
	return r->v;
}

static inline void aon_pmu_ll_set_r40_wake1_delay(uint32_t v) {
	aon_pmu_r40_t *r = (aon_pmu_r40_t*)(SOC_AON_PMU_REG_BASE + (0x40 << 2));
	r->wake1_delay = v;
}

static inline uint32_t aon_pmu_ll_get_r40_wake1_delay(void) {
	aon_pmu_r40_t *r = (aon_pmu_r40_t*)(SOC_AON_PMU_REG_BASE + (0x40 << 2));
	return r->wake1_delay;
}

static inline void aon_pmu_ll_set_r40_wake2_delay(uint32_t v) {
	aon_pmu_r40_t *r = (aon_pmu_r40_t*)(SOC_AON_PMU_REG_BASE + (0x40 << 2));
	r->wake2_delay = v;
}

static inline uint32_t aon_pmu_ll_get_r40_wake2_delay(void) {
	aon_pmu_r40_t *r = (aon_pmu_r40_t*)(SOC_AON_PMU_REG_BASE + (0x40 << 2));
	return r->wake2_delay;
}

static inline void aon_pmu_ll_set_r40_wake3_delay(uint32_t v) {
	aon_pmu_r40_t *r = (aon_pmu_r40_t*)(SOC_AON_PMU_REG_BASE + (0x40 << 2));
	r->wake3_delay = v;
}

static inline uint32_t aon_pmu_ll_get_r40_wake3_delay(void) {
	aon_pmu_r40_t *r = (aon_pmu_r40_t*)(SOC_AON_PMU_REG_BASE + (0x40 << 2));
	return r->wake3_delay;
}

static inline void aon_pmu_ll_set_r40_halt1_delay(uint32_t v) {
	aon_pmu_r40_t *r = (aon_pmu_r40_t*)(SOC_AON_PMU_REG_BASE + (0x40 << 2));
	r->halt1_delay = v;
}

static inline uint32_t aon_pmu_ll_get_r40_halt1_delay(void) {
	aon_pmu_r40_t *r = (aon_pmu_r40_t*)(SOC_AON_PMU_REG_BASE + (0x40 << 2));
	return r->halt1_delay;
}

static inline void aon_pmu_ll_set_r40_halt2_delay(uint32_t v) {
	aon_pmu_r40_t *r = (aon_pmu_r40_t*)(SOC_AON_PMU_REG_BASE + (0x40 << 2));
	r->halt2_delay = v;
}

static inline uint32_t aon_pmu_ll_get_r40_halt2_delay(void) {
	aon_pmu_r40_t *r = (aon_pmu_r40_t*)(SOC_AON_PMU_REG_BASE + (0x40 << 2));
	return r->halt2_delay;
}

static inline void aon_pmu_ll_set_r40_halt3_delay(uint32_t v) {
	aon_pmu_r40_t *r = (aon_pmu_r40_t*)(SOC_AON_PMU_REG_BASE + (0x40 << 2));
	r->halt3_delay = v;
}

static inline uint32_t aon_pmu_ll_get_r40_halt3_delay(void) {
	aon_pmu_r40_t *r = (aon_pmu_r40_t*)(SOC_AON_PMU_REG_BASE + (0x40 << 2));
	return r->halt3_delay;
}

static inline void aon_pmu_ll_set_r40_halt_volt(uint32_t v) {
	aon_pmu_r40_t *r = (aon_pmu_r40_t*)(SOC_AON_PMU_REG_BASE + (0x40 << 2));
	r->halt_volt = v;
}

static inline uint32_t aon_pmu_ll_get_r40_halt_volt(void) {
	aon_pmu_r40_t *r = (aon_pmu_r40_t*)(SOC_AON_PMU_REG_BASE + (0x40 << 2));
	return r->halt_volt;
}

static inline void aon_pmu_ll_set_r40_halt_xtal(uint32_t v) {
	aon_pmu_r40_t *r = (aon_pmu_r40_t*)(SOC_AON_PMU_REG_BASE + (0x40 << 2));
	r->halt_xtal = v;
}

static inline uint32_t aon_pmu_ll_get_r40_halt_xtal(void) {
	aon_pmu_r40_t *r = (aon_pmu_r40_t*)(SOC_AON_PMU_REG_BASE + (0x40 << 2));
	return r->halt_xtal;
}

static inline void aon_pmu_ll_set_r40_halt_core(uint32_t v) {
	aon_pmu_r40_t *r = (aon_pmu_r40_t*)(SOC_AON_PMU_REG_BASE + (0x40 << 2));
	r->halt_core = v;
}

static inline uint32_t aon_pmu_ll_get_r40_halt_core(void) {
	aon_pmu_r40_t *r = (aon_pmu_r40_t*)(SOC_AON_PMU_REG_BASE + (0x40 << 2));
	return r->halt_core;
}

static inline void aon_pmu_ll_set_r40_halt_flash(uint32_t v) {
	aon_pmu_r40_t *r = (aon_pmu_r40_t*)(SOC_AON_PMU_REG_BASE + (0x40 << 2));
	r->halt_flash = v;
}

static inline uint32_t aon_pmu_ll_get_r40_halt_flash(void) {
	aon_pmu_r40_t *r = (aon_pmu_r40_t*)(SOC_AON_PMU_REG_BASE + (0x40 << 2));
	return r->halt_flash;
}

static inline void aon_pmu_ll_set_r40_halt_rosc(uint32_t v) {
	aon_pmu_r40_t *r = (aon_pmu_r40_t*)(SOC_AON_PMU_REG_BASE + (0x40 << 2));
	r->halt_rosc = v;
}

static inline uint32_t aon_pmu_ll_get_r40_halt_rosc(void) {
	aon_pmu_r40_t *r = (aon_pmu_r40_t*)(SOC_AON_PMU_REG_BASE + (0x40 << 2));
	return r->halt_rosc;
}

static inline void aon_pmu_ll_set_r40_halt_resten(uint32_t v) {
	aon_pmu_r40_t *r = (aon_pmu_r40_t*)(SOC_AON_PMU_REG_BASE + (0x40 << 2));
	r->halt_resten = v;
}

static inline uint32_t aon_pmu_ll_get_r40_halt_resten(void) {
	aon_pmu_r40_t *r = (aon_pmu_r40_t*)(SOC_AON_PMU_REG_BASE + (0x40 << 2));
	return r->halt_resten;
}

static inline void aon_pmu_ll_set_r40_halt_isolat(uint32_t v) {
	aon_pmu_r40_t *r = (aon_pmu_r40_t*)(SOC_AON_PMU_REG_BASE + (0x40 << 2));
	r->halt_isolat = v;
}

static inline uint32_t aon_pmu_ll_get_r40_halt_isolat(void) {
	aon_pmu_r40_t *r = (aon_pmu_r40_t*)(SOC_AON_PMU_REG_BASE + (0x40 << 2));
	return r->halt_isolat;
}

static inline void aon_pmu_ll_set_r40_halt_clkena(uint32_t v) {
	aon_pmu_r40_t *r = (aon_pmu_r40_t*)(SOC_AON_PMU_REG_BASE + (0x40 << 2));
	r->halt_clkena = v;
}

static inline uint32_t aon_pmu_ll_get_r40_halt_clkena(void) {
	aon_pmu_r40_t *r = (aon_pmu_r40_t*)(SOC_AON_PMU_REG_BASE + (0x40 << 2));
	return r->halt_clkena;
}

//reg r41:

static inline void aon_pmu_ll_set_r41(uint32_t v) {
	aon_pmu_r41_t *r = (aon_pmu_r41_t*)(SOC_AON_PMU_REG_BASE + (0x41 << 2));
	r->v = v;
}

static inline uint32_t aon_pmu_ll_get_r41(void) {
	aon_pmu_r41_t *r = (aon_pmu_r41_t*)(SOC_AON_PMU_REG_BASE + (0x41 << 2));
	return r->v;
}

__FLASH_BOOT_CODE static inline void aon_pmu_ll_set_r41_lpo_config(uint32_t v) {
	aon_pmu_r41_t *r = (aon_pmu_r41_t*)(SOC_AON_PMU_REG_BASE + (0x41 << 2));
	r->lpo_config = v;
}

__FLASH_BOOT_CODE static inline uint32_t aon_pmu_ll_get_r41_lpo_config(void) {
	aon_pmu_r41_t *r = (aon_pmu_r41_t*)(SOC_AON_PMU_REG_BASE + (0x41 << 2));
	return r->lpo_config;
}

static inline void aon_pmu_ll_set_r41_flshsck_iocap(uint32_t v) {
	aon_pmu_r41_t *r = (aon_pmu_r41_t*)(SOC_AON_PMU_REG_BASE + (0x41 << 2));
	r->flshsck_iocap = v;
}

static inline uint32_t aon_pmu_ll_get_r41_flshsck_iocap(void) {
	aon_pmu_r41_t *r = (aon_pmu_r41_t*)(SOC_AON_PMU_REG_BASE + (0x41 << 2));
	return r->flshsck_iocap;
}

static inline void aon_pmu_ll_set_r41_gpio_clksel(uint32_t v) {
	aon_pmu_r41_t *r = (aon_pmu_r41_t*)(SOC_AON_PMU_REG_BASE + (0x41 << 2));
	r->gpio_clksel = v;
}

static inline uint32_t aon_pmu_ll_get_r41_gpio_clksel(void) {
	aon_pmu_r41_t *r = (aon_pmu_r41_t*)(SOC_AON_PMU_REG_BASE + (0x41 << 2));
	return r->gpio_clksel;
}

static inline void aon_pmu_ll_set_r41_halt_lpo(uint32_t v) {
	aon_pmu_r41_t *r = (aon_pmu_r41_t*)(SOC_AON_PMU_REG_BASE + (0x41 << 2));
	r->halt_lpo = v;
}

static inline uint32_t aon_pmu_ll_get_r41_halt_lpo(void) {
	aon_pmu_r41_t *r = (aon_pmu_r41_t*)(SOC_AON_PMU_REG_BASE + (0x41 << 2));
	return r->halt_lpo;
}

static inline void aon_pmu_ll_set_r41_halt_cpu(uint32_t v) {
	aon_pmu_r41_t *r = (aon_pmu_r41_t*)(SOC_AON_PMU_REG_BASE + (0x41 << 2));
	r->halt_cpu = v;
}

static inline uint32_t aon_pmu_ll_get_r41_halt_cpu(void) {
	aon_pmu_r41_t *r = (aon_pmu_r41_t*)(SOC_AON_PMU_REG_BASE + (0x41 << 2));
	return r->halt_cpu;
}

static inline void aon_pmu_ll_set_r41_halt_anareg(uint32_t v) {
	aon_pmu_r41_t *r = (aon_pmu_r41_t*)(SOC_AON_PMU_REG_BASE + (0x41 << 2));
	r->halt_anareg = v;
}

static inline uint32_t aon_pmu_ll_get_r41_halt_anareg(void) {
	aon_pmu_r41_t *r = (aon_pmu_r41_t*)(SOC_AON_PMU_REG_BASE + (0x41 << 2));
	return r->halt_anareg;
}

static inline void aon_pmu_ll_set_r41_dpll_lpen(uint32_t v) {
	aon_pmu_r41_t *r = (aon_pmu_r41_t*)(SOC_AON_PMU_REG_BASE + (0x41 << 2));
	r->dpll_lpen = v;
}

static inline uint32_t aon_pmu_ll_get_r41_dpll_lpen(void) {
	aon_pmu_r41_t *r = (aon_pmu_r41_t*)(SOC_AON_PMU_REG_BASE + (0x41 << 2));
	return r->dpll_lpen;
}

//reg r71:

static inline void aon_pmu_ll_set_r71(uint32_t v) {
	aon_pmu_r71_t *r = (aon_pmu_r71_t*)(SOC_AON_PMU_REG_BASE + (0x71 << 2));
	r->v = v;
}

static inline uint32_t aon_pmu_ll_get_r71(void) {
	aon_pmu_r71_t *r = (aon_pmu_r71_t*)(SOC_AON_PMU_REG_BASE + (0x71 << 2));
	return r->v;
}

static inline uint32_t aon_pmu_ll_get_r71_wakeup_source(void) {
	aon_pmu_r71_t *r = (aon_pmu_r71_t*)(SOC_AON_PMU_REG_BASE + (0x71 << 2));
	return r->wakeup_source;
}

static inline uint32_t aon_pmu_ll_get_r71_abnormal_wakeup_source(void) {
	aon_pmu_r71_t *r = (aon_pmu_r71_t*)(SOC_AON_PMU_REG_BASE + (0x71 << 2));
	return r->abnormal_wakeup_source;
}

//reg reg72:

static inline void aon_pmu_ll_set_reg72_value(uint32_t v) {
	aon_pmu_r72_t *r = (aon_pmu_r72_t*)(SOC_AON_PMU_REG_BASE + (0x72 << 2));
	r->v = v;
}

static inline uint32_t aon_pmu_ll_get_reg72_value(void) {
	aon_pmu_r72_t *r = (aon_pmu_r72_t*)(SOC_AON_PMU_REG_BASE + (0x72 << 2));
	return r->v;
}

static inline uint32_t aon_pmu_ll_get_reg72_finecnt_32k_samp(void) {
	aon_pmu_r72_t *r = (aon_pmu_r72_t*)(SOC_AON_PMU_REG_BASE + (0x72 << 2));
	return r->finecnt_32k_samp;
}

static inline uint32_t aon_pmu_ll_get_reg72_ble_cnt_rvld(void) {
	aon_pmu_r72_t *r = (aon_pmu_r72_t*)(SOC_AON_PMU_REG_BASE + (0x72 << 2));
	return r->ble_cnt_rvld;
}

//reg reg73:

static inline void aon_pmu_ll_set_reg73_value(uint32_t v) {
	aon_pmu_r73_t *r = (aon_pmu_r73_t*)(SOC_AON_PMU_REG_BASE + (0x73 << 2));
	r->v = v;
}

static inline uint32_t aon_pmu_ll_get_reg73_value(void) {
	aon_pmu_r73_t *r = (aon_pmu_r73_t*)(SOC_AON_PMU_REG_BASE + (0x73 << 2));
	return r->v;
}

static inline uint32_t aon_pmu_ll_get_reg73_clkcnt_32k_samp(void) {
	aon_pmu_r73_t *r = (aon_pmu_r73_t*)(SOC_AON_PMU_REG_BASE + (0x73 << 2));
	return r->clkcnt_32k_samp;
}

//reg reg74:

static inline void aon_pmu_ll_set_reg74_value(uint32_t v) {
	aon_pmu_r74_t *r = (aon_pmu_r74_t*)(SOC_AON_PMU_REG_BASE + (0x74 << 2));
	r->v = v;
}

static inline uint32_t aon_pmu_ll_get_reg74_value(void) {
	aon_pmu_r74_t *r = (aon_pmu_r74_t*)(SOC_AON_PMU_REG_BASE + (0x74 << 2));
	return r->v;
}

static inline uint32_t aon_pmu_ll_get_reg74_isocnt_32k_samp(void) {
	aon_pmu_r74_t *r = (aon_pmu_r74_t*)(SOC_AON_PMU_REG_BASE + (0x74 << 2));
	return r->isocnt_32k_samp;
}

//reg r77:

static inline uint32_t aon_pmu_ll_get_r77(void) {
	aon_pmu_r77_t *r = (aon_pmu_r77_t*)(SOC_AON_PMU_REG_BASE + (0x77 << 2));
	return r->v;
}

static inline uint32_t aon_pmu_ll_get_r77_ana_gpio_sta(void) {
	aon_pmu_r77_t *r = (aon_pmu_r77_t*)(SOC_AON_PMU_REG_BASE + (0x77 << 2));
	return r->ana_gpio_sta;
}

static inline uint32_t aon_pmu_ll_get_r80_L(void) {
	aon_pmu_r77_t *r = (aon_pmu_r77_t*)(SOC_AON_PMU_REG_BASE + (0x77 << 2));
	return r->L;
}


//reg r78:

static inline uint32_t aon_pmu_ll_get_r78(void) {
	aon_pmu_r78_t *r = (aon_pmu_r78_t*)(SOC_AON_PMU_REG_BASE + (0x78 << 2));
	return r->v;
}

static inline uint32_t aon_pmu_ll_get_r78_rtc_tick_h(void) {
	aon_pmu_r78_t *r = (aon_pmu_r78_t*)(SOC_AON_PMU_REG_BASE + (0x78 << 2));
	return r->rtc_tick_h;
}

//reg r79:

static inline uint32_t aon_pmu_ll_get_r79(void) {
	aon_pmu_r79_t *r = (aon_pmu_r79_t*)(SOC_AON_PMU_REG_BASE + (0x79 << 2));
	return r->v;
}

static inline uint32_t aon_pmu_ll_get_r79_rtc_tick_l(void) {
	aon_pmu_r79_t *r = (aon_pmu_r79_t*)(SOC_AON_PMU_REG_BASE + (0x79 << 2));
	return r->rtc_tick_l;
}

//reg r7a:

static inline uint32_t aon_pmu_ll_get_r7a(void) {
	aon_pmu_r7a_t *r = (aon_pmu_r7a_t*)(SOC_AON_PMU_REG_BASE + (0x7a << 2));
	return r->v;
}

//reg r7b:

static inline uint32_t aon_pmu_ll_get_r7b(void) {
	aon_pmu_r7b_t *r = (aon_pmu_r7b_t*)(SOC_AON_PMU_REG_BASE + (0x7b << 2));
	return r->v;
}

//reg r7c:

static inline void aon_pmu_ll_set_r7c(uint32_t v) {
	aon_pmu_r7c_t *r = (aon_pmu_r7c_t*)(SOC_AON_PMU_REG_BASE + (0x7c << 2));
	r->v = v;
}

static inline uint32_t aon_pmu_ll_get_r7c(void) {
	aon_pmu_r7c_t *r = (aon_pmu_r7c_t*)(SOC_AON_PMU_REG_BASE + (0x7c << 2));
	return r->v;
}

static inline uint32_t aon_pmu_ll_get_r7c_id(void) {
	aon_pmu_r7c_t *r = (aon_pmu_r7c_t*)(SOC_AON_PMU_REG_BASE + (0x7c << 2));
	return r->id;
}

//reg r7d:

static inline void aon_pmu_ll_set_r7d(uint32_t v) {
	aon_pmu_r7d_t *r = (aon_pmu_r7d_t*)(SOC_AON_PMU_REG_BASE + (0x7d << 2));
	r->v = v;
}

static inline uint32_t aon_pmu_ll_get_r7d(void) {
	aon_pmu_r7d_t *r = (aon_pmu_r7d_t*)(SOC_AON_PMU_REG_BASE + (0x7d << 2));
	return r->v;
}

static inline uint32_t aon_pmu_ll_get_r7d_adc_cal(void) {
	aon_pmu_r7d_t *r = (aon_pmu_r7d_t*)(SOC_AON_PMU_REG_BASE + (0x7d << 2));
	return r->adc_cal;
}

static inline uint32_t aon_pmu_ll_get_r7d_bgcal(void) {
	aon_pmu_r7d_t *r = (aon_pmu_r7d_t*)(SOC_AON_PMU_REG_BASE + (0x7d << 2));
	return r->bgcal;
}

static inline uint32_t aon_pmu_ll_get_r7d_dpll_unlockL(void) {
	aon_pmu_r7d_t *r = (aon_pmu_r7d_t*)(SOC_AON_PMU_REG_BASE + (0x7d << 2));
	return r->dpll_unlockL;
}

static inline uint32_t aon_pmu_ll_get_r7d_dpll_unlockH(void) {
	aon_pmu_r7d_t *r = (aon_pmu_r7d_t*)(SOC_AON_PMU_REG_BASE + (0x7d << 2));
	return r->dpll_unlockH;
}

static inline uint32_t aon_pmu_ll_get_r7d_dpll_band(void) {
	aon_pmu_r7d_t *r = (aon_pmu_r7d_t*)(SOC_AON_PMU_REG_BASE + (0x7d << 2));
	return r->dpll_band;
}

static inline uint32_t aon_pmu_ll_get_r7d_L(void) {
	aon_pmu_r7d_t *r = (aon_pmu_r7d_t*)(SOC_AON_PMU_REG_BASE + (0x7d << 2));
	return r->L;
}

//reg r7e:

static inline void aon_pmu_ll_set_r7e(uint32_t v) {
	aon_pmu_r7e_t *r = (aon_pmu_r7e_t*)(SOC_AON_PMU_REG_BASE + (0x7e << 2));
	r->v = v;
}

static inline uint32_t aon_pmu_ll_get_r7e(void) {
	aon_pmu_r7e_t *r = (aon_pmu_r7e_t*)(SOC_AON_PMU_REG_BASE + (0x7e << 2));
	return r->v;
}

static inline uint32_t aon_pmu_ll_get_r7e_cbcal(void) {
	aon_pmu_r7e_t *r = (aon_pmu_r7e_t*)(SOC_AON_PMU_REG_BASE + (0x7e << 2));
	return r->cbcal;
}

static inline uint32_t aon_pmu_ll_get_r7e_band_cal(void) {
	aon_pmu_r7e_t *r = (aon_pmu_r7e_t*)(SOC_AON_PMU_REG_BASE + (0x7e << 2));
	return r->band_cal;
}

static inline uint32_t aon_pmu_ll_get_r7e_td_zcd_cal(void) {
	aon_pmu_r7e_t *r = (aon_pmu_r7e_t*)(SOC_AON_PMU_REG_BASE + (0x7e << 2));
	return r->zcd_cal;
}

static inline uint32_t aon_pmu_ll_get_r7e_L(void) {
	aon_pmu_r7e_t *r = (aon_pmu_r7e_t*)(SOC_AON_PMU_REG_BASE + (0x7e << 2));
	return r->L;
}

//reg r7f:

static inline uint32_t aon_pmu_ll_get_r7f(void) {
	aon_pmu_r7f_t *r = (aon_pmu_r7f_t*)(SOC_AON_PMU_REG_BASE + (0x7f << 2));
	return r->v;
}

static inline uint32_t aon_pmu_ll_get_r7f_td_int(void) {
	aon_pmu_r7f_t *r = (aon_pmu_r7f_t*)(SOC_AON_PMU_REG_BASE + (0x7f << 2));
	return r->td_int;
}

static inline uint32_t aon_pmu_ll_get_r7f_td_chen(void) {
	aon_pmu_r7f_t *r = (aon_pmu_r7f_t*)(SOC_AON_PMU_REG_BASE + (0x7f << 2));
	return r->td_chen;
}

static inline uint32_t aon_pmu_ll_get_r7f_ad_state(void) {
	aon_pmu_r7f_t *r = (aon_pmu_r7f_t*)(SOC_AON_PMU_REG_BASE + (0x7f << 2));
	return r->ad_state;
}

static inline uint32_t aon_pmu_ll_get_r7f_td_do(void) {
	aon_pmu_r7f_t *r = (aon_pmu_r7f_t*)(SOC_AON_PMU_REG_BASE + (0x7f << 2));
	return r->td_do;
}

#ifdef __cplusplus
}
#endif

#endif // CONFIG_TFM_AON_PMU_LL_NSC
