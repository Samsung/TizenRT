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

#include "hal_config.h"
#include "aon_pmu_hw.h"
#include "aon_pmu_hal.h"

typedef void (*aon_pmu_dump_fn_t)(void);
typedef struct {
	uint32_t start;
	uint32_t end;
	aon_pmu_dump_fn_t fn;
} aon_pmu_reg_fn_map_t;

static void aon_pmu_dump_r0(void)
{
	aon_pmu_r0_t *r = (aon_pmu_r0_t *)(SOC_AON_PMU_REG_BASE + (0x0 << 2));

	SOC_LOGI("r0: %-8x\r\n", REG_READ(SOC_AON_PMU_REG_BASE + (0x0 << 2)));
	SOC_LOGI("	memchk_bps: %-8x\r\n", r->memchk_bps);
	SOC_LOGI("	fast_boot: %-8x\r\n", r->fast_boot);
	SOC_LOGI("	dig_wake_en: %-8x\r\n", r->dig_wake_en);
	SOC_LOGI("	reserved_bit_3_12: %-8x\r\n", r->reserved_bit_3_12);
	SOC_LOGI("	saved_time: %-8x\r\n", r->saved_time);
	SOC_LOGI("	reset_reason: %-8x\r\n", r->reset_reason);
	SOC_LOGI("	gpio_sleep: %-8x\r\n", r->gpio_sleep);
}

static void aon_pmu_dump_rsv_1_1(void)
{
	for (uint32_t idx = 0; idx < 1; idx++) {
		SOC_LOGI("rsv_1_1: %-8x\r\n", REG_READ(SOC_AON_PMU_REG_BASE + ((0x1 + idx) << 2)));
	}
}

static void aon_pmu_dump_r2(void)
{
	aon_pmu_r2_t *r = (aon_pmu_r2_t *)(SOC_AON_PMU_REG_BASE + (0x2 << 2));

	SOC_LOGI("r2: %-8x\r\n", REG_READ(SOC_AON_PMU_REG_BASE + (0x2 << 2)));
	SOC_LOGI("	wdt_rst_ana: %-8x\r\n", r->wdt_rst_ana);
	SOC_LOGI("	wdt_rst_top: %-8x\r\n", r->wdt_rst_top);
	SOC_LOGI("	wdt_rst_aon: %-8x\r\n", r->wdt_rst_aon);
	SOC_LOGI("	reserved_bit_3_7: %-8x\r\n", r->reserved_bit_3_7);
	SOC_LOGI("	rtc_value_samp_sel: %-8x\r\n", r->rtc_value_samp_sel);
	SOC_LOGI("	reserved_bit_9_30: %-8x\r\n", r->reserved_bit_9_30);
	SOC_LOGI("	otp_vdd_en: %-8x\r\n", r->otp_vdd_en);
}

static void aon_pmu_dump_rsv_3_40(void)
{
	for (uint32_t idx = 0; idx < 61; idx++) {
		SOC_LOGI("rsv_3_40: %-8x\r\n", REG_READ(SOC_AON_PMU_REG_BASE + ((0x3 + idx) << 2)));
	}
}

static void aon_pmu_dump_r40(void)
{
	aon_pmu_r40_t *r = (aon_pmu_r40_t *)(SOC_AON_PMU_REG_BASE + (0x40 << 2));

	SOC_LOGI("r40: %-8x\r\n", REG_READ(SOC_AON_PMU_REG_BASE + (0x40 << 2)));
	SOC_LOGI("	wake1_delay: %-8x\r\n", r->wake1_delay);
	SOC_LOGI("	wake2_delay: %-8x\r\n", r->wake2_delay);
	SOC_LOGI("	wake3_delay: %-8x\r\n", r->wake3_delay);
	SOC_LOGI("	halt1_delay: %-8x\r\n", r->halt1_delay);
	SOC_LOGI("	halt2_delay: %-8x\r\n", r->halt2_delay);
	SOC_LOGI("	halt3_delay: %-8x\r\n", r->halt3_delay);
	SOC_LOGI("	halt_volt: %-8x\r\n", r->halt_volt);
	SOC_LOGI("	halt_xtal: %-8x\r\n", r->halt_xtal);
	SOC_LOGI("	halt_core: %-8x\r\n", r->halt_core);
	SOC_LOGI("	halt_flash: %-8x\r\n", r->halt_flash);
	SOC_LOGI("	halt_rosc: %-8x\r\n", r->halt_rosc);
	SOC_LOGI("	halt_resten: %-8x\r\n", r->halt_resten);
	SOC_LOGI("	halt_isolat: %-8x\r\n", r->halt_isolat);
	SOC_LOGI("	halt_clkena: %-8x\r\n", r->halt_clkena);
}

static void aon_pmu_dump_r41(void)
{
	aon_pmu_r41_t *r = (aon_pmu_r41_t *)(SOC_AON_PMU_REG_BASE + (0x41 << 2));

	SOC_LOGI("r41: %-8x\r\n", REG_READ(SOC_AON_PMU_REG_BASE + (0x41 << 2)));
	SOC_LOGI("	lpo_config: %-8x\r\n", r->lpo_config);
	SOC_LOGI("	flshsck_iocap: %-8x\r\n", r->flshsck_iocap);
	SOC_LOGI("	wakeup_ena: %-8x\r\n", r->wakeup_ena);
	SOC_LOGI("	gpio_clksel: %-8x\r\n", r->gpio_clksel);
	SOC_LOGI("	reserved_bit_11_23: %-8x\r\n", r->reserved_bit_11_23);
	SOC_LOGI("	halt_lpo: %-8x\r\n", r->halt_lpo);
	SOC_LOGI("	halt_cpu: %-8x\r\n", r->halt_cpu);
	SOC_LOGI("	dpll_lpen: %-8x\r\n", r->dpll_lpen);
	SOC_LOGI("	reserved_bit_28_31: %-8x\r\n", r->reserved_bit_28_31);
}

static void aon_pmu_dump_rsv_42_70(void)
{
	for (uint32_t idx = 0; idx < 47; idx++) {
		SOC_LOGI("rsv_42_70: %-8x\r\n", REG_READ(SOC_AON_PMU_REG_BASE + ((0x42 + idx) << 2)));
	}
}

static void aon_pmu_dump_r71(void)
{
	aon_pmu_r71_t *r = (aon_pmu_r71_t *)(SOC_AON_PMU_REG_BASE + (0x71 << 2));

	SOC_LOGI("r71: %-8x\r\n", REG_READ(SOC_AON_PMU_REG_BASE + (0x71 << 2)));
	SOC_LOGI("	reserved_bit_0_19: %-8x\r\n", r->reserved_bit_0_19);
	SOC_LOGI("	wakeup_source: %-8x\r\n", r->wakeup_source);
	SOC_LOGI("	abnormal_wakeup_source: %-8x\r\n", r->abnormal_wakeup_source);
	SOC_LOGI("	reserved_bit_24_31: %-8x\r\n", r->reserved_bit_24_31);
}

static void aon_pmu_dump_r72(void)
{
	aon_pmu_r72_t *r = (aon_pmu_r72_t *)(SOC_AON_PMU_REG_BASE + (0x72 << 2));

	SOC_LOGI("r72: %-8x\r\n", REG_READ(SOC_AON_PMU_REG_BASE + (0x72 << 2)));
	SOC_LOGI("	finecnt_32k_samp: %-8x\r\n", r->finecnt_32k_samp);
	SOC_LOGI("	ble_cnt_rvld: %-8x\r\n", r->ble_cnt_rvld);
	SOC_LOGI("	reserved_bit_11_31: %-8x\r\n", r->reserved_bit_11_31);
}

static void aon_pmu_dump_r73(void)
{
	aon_pmu_r73_t *r = (aon_pmu_r73_t *)(SOC_AON_PMU_REG_BASE + (0x73 << 2));

	SOC_LOGI("r73: %-8x\r\n", REG_READ(SOC_AON_PMU_REG_BASE + (0x73 << 2)));
	SOC_LOGI("	clkcnt_32k_samp: %-8x\r\n", r->clkcnt_32k_samp);
	SOC_LOGI("	reserved_bit_28_31: %-8x\r\n", r->reserved_bit_28_31);
}

static void aon_pmu_dump_rsv_74_76(void)
{
	for (uint32_t idx = 0; idx < 3; idx++) {
		SOC_LOGI("rsv_74_76: %-8x\r\n", REG_READ(SOC_AON_PMU_REG_BASE + ((0x74 + idx) << 2)));
	}
}

static void aon_pmu_dump_r77(void)
{
	aon_pmu_r77_t *r = (aon_pmu_r77_t *)(SOC_AON_PMU_REG_BASE + (0x77 << 2));

	SOC_LOGI("r77: %-8x\r\n", REG_READ(SOC_AON_PMU_REG_BASE + (0x77 << 2)));
	SOC_LOGI("	ana_gpio_sta: %-8x\r\n", r->ana_gpio_sta);
	SOC_LOGI("	l: %-8x\r\n", r->L);
}

static void aon_pmu_dump_r78(void)
{
	aon_pmu_r78_t *r = (aon_pmu_r78_t *)(SOC_AON_PMU_REG_BASE + (0x78 << 2));

	SOC_LOGI("r78: %-8x\r\n", REG_READ(SOC_AON_PMU_REG_BASE + (0x78 << 2)));
	SOC_LOGI("	rtc_tick_h: %-8x\r\n", r->rtc_tick_h);
	SOC_LOGI("	reserved_bit_4_31: %-8x\r\n", r->reserved_bit_4_31);
}

static void aon_pmu_dump_r79(void)
{
	aon_pmu_r79_t *r = (aon_pmu_r79_t *)(SOC_AON_PMU_REG_BASE + (0x79 << 2));

	SOC_LOGI("r79: %-8x\r\n", REG_READ(SOC_AON_PMU_REG_BASE + (0x79 << 2)));
	SOC_LOGI("	rtc_tick_l: %-8x\r\n", r->rtc_tick_l);
}

static void aon_pmu_dump_r7a(void)
{
	aon_pmu_r0_t *r = (aon_pmu_r0_t *)(SOC_AON_PMU_REG_BASE + (0x7a << 2));

	SOC_LOGI("r7a: %-8x\r\n", REG_READ(SOC_AON_PMU_REG_BASE + (0x7a << 2)));
	SOC_LOGI("	memchk_bps: %-8x\r\n", r->memchk_bps);
	SOC_LOGI("	fast_boot: %-8x\r\n", r->fast_boot);
	SOC_LOGI("	dig_wake_en: %-8x\r\n", r->dig_wake_en);
	SOC_LOGI("	reserved_bit_3_12: %-8x\r\n", r->reserved_bit_3_12);
	SOC_LOGI("	saved_time: %-8x\r\n", r->saved_time);
	SOC_LOGI("	reset_reason: %-8x\r\n", r->reset_reason);
	SOC_LOGI("	gpio_sleep: %-8x\r\n", r->gpio_sleep);
}

static void aon_pmu_dump_r7b(void)
{
	aon_pmu_r0_t *r = (aon_pmu_r0_t *)(SOC_AON_PMU_REG_BASE + (0x7b << 2));

	SOC_LOGI("r7b: %-8x\r\n", REG_READ(SOC_AON_PMU_REG_BASE + (0x7b << 2)));
	SOC_LOGI("	memchk_bps: %-8x\r\n", r->memchk_bps);
	SOC_LOGI("	fast_boot: %-8x\r\n", r->fast_boot);
	SOC_LOGI("	dig_wake_en: %-8x\r\n", r->dig_wake_en);
	SOC_LOGI("	reserved_bit_3_12: %-8x\r\n", r->reserved_bit_3_12);
	SOC_LOGI("	saved_time: %-8x\r\n", r->saved_time);
	SOC_LOGI("	reset_reason: %-8x\r\n", r->reset_reason);
	SOC_LOGI("	gpio_sleep: %-8x\r\n", r->gpio_sleep);
}

static void aon_pmu_dump_r7c(void)
{
	SOC_LOGI("r7c: %-8x\r\n", REG_READ(SOC_AON_PMU_REG_BASE + (0x7c << 2)));
}

static void aon_pmu_dump_r7d(void)
{
	aon_pmu_r7d_t *r = (aon_pmu_r7d_t *)(SOC_AON_PMU_REG_BASE + (0x7d << 2));

	SOC_LOGI("r7d: %-8x\r\n", REG_READ(SOC_AON_PMU_REG_BASE + (0x7d << 2)));
	SOC_LOGI("	adc_cal: %-8x\r\n", r->adc_cal);
	SOC_LOGI("	bgcal: %-8x\r\n", r->bgcal);
	SOC_LOGI("	dpll_unlockL: %-8x\r\n", r->dpll_unlockL);
	SOC_LOGI("	dpll_unlockH: %-8x\r\n", r->dpll_unlockH);
	SOC_LOGI("	dpll_band: %-8x\r\n", r->dpll_band);
	SOC_LOGI("	l: %-8x\r\n", r->L);
}

static void aon_pmu_dump_r7e(void)
{
	aon_pmu_r7e_t *r = (aon_pmu_r7e_t *)(SOC_AON_PMU_REG_BASE + (0x7e << 2));

	SOC_LOGI("r7e: %-8x\r\n", REG_READ(SOC_AON_PMU_REG_BASE + (0x7e << 2)));
	SOC_LOGI("	cbcal: %-8x\r\n", r->cbcal);
	SOC_LOGI("	band_cal: %-8x\r\n", r->band_cal);
	SOC_LOGI("	zcd_cal: %-8x\r\n", r->zcd_cal);
	SOC_LOGI("	l: %-8x\r\n", r->L);
}

static void aon_pmu_dump_r7f(void)
{
	aon_pmu_r7f_t *r = (aon_pmu_r7f_t *)(SOC_AON_PMU_REG_BASE + (0x7f << 2));

	SOC_LOGI("r7f: %-8x\r\n", REG_READ(SOC_AON_PMU_REG_BASE + (0x7f << 2)));
	SOC_LOGI("	td_int: %-8x\r\n", r->td_int);
	SOC_LOGI("	td_chen: %-8x\r\n", r->td_chen);
	SOC_LOGI("	ad_state: %-8x\r\n", r->ad_state);
	SOC_LOGI("	td_do: %-8x\r\n", r->td_do);
}

static aon_pmu_reg_fn_map_t s_fn[] =
{
	{0x0, 0x0, aon_pmu_dump_r0},
	{0x2, 0x2, aon_pmu_dump_r2},
	{0x40, 0x40, aon_pmu_dump_r40},
	{0x41, 0x41, aon_pmu_dump_r41},
	{0x71, 0x71, aon_pmu_dump_r71},
	{0x72, 0x72, aon_pmu_dump_r72},
	{0x73, 0x73, aon_pmu_dump_r73},
	{0x77, 0x77, aon_pmu_dump_r77},
	{0x78, 0x78, aon_pmu_dump_r78},
	{0x79, 0x79, aon_pmu_dump_r79},
	{0x7a, 0x7a, aon_pmu_dump_r7a},
	{0x7b, 0x7b, aon_pmu_dump_r7b},
	{0x7c, 0x7c, aon_pmu_dump_r7c},
	{0x7d, 0x7d, aon_pmu_dump_r7d},
	{0x7e, 0x7e, aon_pmu_dump_r7e},
	{0x7f, 0x7f, aon_pmu_dump_r7f},
	{-1, -1, 0}
};

void aon_pmu_struct_dump(uint32_t start, uint32_t end)
{
	uint32_t dump_fn_cnt = sizeof(s_fn)/sizeof(s_fn[0]) - 1;

	for (uint32_t idx = 0; idx < dump_fn_cnt; idx++) {
		if ((start <= s_fn[idx].start) && (end >= s_fn[idx].end)) {
			s_fn[idx].fn();
		}
	}
}
