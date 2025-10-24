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

#include <soc/soc.h>
#include "hal_port.h"
#include "aon_pmu_hw.h"
#include "tfm_secure_api.h"
#include "driver/prro.h"

#define PRRO_AON_PMU_SECURE_DECLARATION() uint32_t secure_status

#define PRRO_SET_AON_PMU_SECURE() do { \
	bk_prro_driver_init(); \
	secure_status = bk_prro_get_secure(PRRO_DEV_AON_PMU); \
	bk_prro_set_secure(PRRO_DEV_AON_PMU, PRRO_SECURE); \
} while(0)

#define PRRO_SET_AON_PMU_NON_SECURE() do { \
	bk_prro_set_secure(PRRO_DEV_AON_PMU, secure_status); \
} while(0)

//reg r0:

__tfm_psa_secure_gateway_no_naked_attributes__ void aon_pmu_ll_set_r0(uint32_t v) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r0_t *r = (aon_pmu_r0_t*)(SOC_AON_PMU_REG_BASE + (0x0 << 2));
	r->v = v;
	PRRO_SET_AON_PMU_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t aon_pmu_ll_get_r0(void) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r0_t *r = (aon_pmu_r0_t*)(SOC_AON_PMU_REG_BASE + (0x0 << 2));
	uint32_t tmp = r->v;
	PRRO_SET_AON_PMU_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void aon_pmu_ll_set_r0_memchk_bps(uint32_t v) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r0_t *r = (aon_pmu_r0_t*)(SOC_AON_PMU_REG_BASE + (0x0 << 2));
	r->memchk_bps = v;
	PRRO_SET_AON_PMU_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t aon_pmu_ll_get_r0_memchk_bps(void) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r0_t *r = (aon_pmu_r0_t*)(SOC_AON_PMU_REG_BASE + (0x0 << 2));
	uint32_t tmp = r->memchk_bps;
	PRRO_SET_AON_PMU_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void aon_pmu_ll_set_r0_fast_boot(uint32_t v) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r0_t *r = (aon_pmu_r0_t*)(SOC_AON_PMU_REG_BASE + (0x0 << 2));
	r->fast_boot = v;
	PRRO_SET_AON_PMU_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t aon_pmu_ll_get_r0_fast_boot(void) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r0_t *r = (aon_pmu_r0_t*)(SOC_AON_PMU_REG_BASE + (0x0 << 2));
	uint32_t tmp = r->fast_boot;
	PRRO_SET_AON_PMU_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void aon_pmu_ll_set_r0_gpio_sleep(uint32_t v) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r0_t *r = (aon_pmu_r0_t*)(SOC_AON_PMU_REG_BASE + (0x0 << 2));
	r->gpio_sleep = v;
	PRRO_SET_AON_PMU_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t aon_pmu_ll_get_r0_gpio_sleep(void) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r0_t *r = (aon_pmu_r0_t*)(SOC_AON_PMU_REG_BASE + (0x0 << 2));
	uint32_t tmp = r->gpio_sleep;
	PRRO_SET_AON_PMU_NON_SECURE();
	return tmp;
}

//reg r1:

__tfm_psa_secure_gateway_no_naked_attributes__ void aon_pmu_ll_set_r1(uint32_t v) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r1_t *r = (aon_pmu_r1_t*)(SOC_AON_PMU_REG_BASE + (0x1 << 2));
	r->v = v;
	PRRO_SET_AON_PMU_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t aon_pmu_ll_get_r1(void) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r1_t *r = (aon_pmu_r1_t*)(SOC_AON_PMU_REG_BASE + (0x1 << 2));
	uint32_t tmp = r->v;
	PRRO_SET_AON_PMU_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t aon_pmu_ll_get_r1_touch_select(void) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r1_t *r = (aon_pmu_r1_t*)(SOC_AON_PMU_REG_BASE + (0x1 << 2));
	uint32_t tmp = r->touch_select;
	PRRO_SET_AON_PMU_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void aon_pmu_ll_set_r1_touch_int_en(uint32_t v) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r1_t *r = (aon_pmu_r1_t*)(SOC_AON_PMU_REG_BASE + (0x1 << 2));
	r->touch_int_en = v;
	PRRO_SET_AON_PMU_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t aon_pmu_ll_get_r1_touch_int_en(void) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r1_t *r = (aon_pmu_r1_t*)(SOC_AON_PMU_REG_BASE + (0x1 << 2));
	uint32_t tmp = r->touch_int_en;
	PRRO_SET_AON_PMU_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void aon_pmu_ll_set_r1_usbplug_int_en(uint32_t v) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r1_t *r = (aon_pmu_r1_t*)(SOC_AON_PMU_REG_BASE + (0x1 << 2));
	r->usbplug_int_en = v;
	PRRO_SET_AON_PMU_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t aon_pmu_ll_get_r1_usbplug_int_en(void) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r1_t *r = (aon_pmu_r1_t*)(SOC_AON_PMU_REG_BASE + (0x1 << 2));
	uint32_t tmp = r->usbplug_int_en;
	PRRO_SET_AON_PMU_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t aon_pmu_ll_get_r1_clk_mod(void) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r1_t *r = (aon_pmu_r1_t*)(SOC_AON_PMU_REG_BASE + (0x1 << 2));
	uint32_t tmp = r->clk_mod;
	PRRO_SET_AON_PMU_NON_SECURE();
	return tmp;
}

//reg r2:

__tfm_psa_secure_gateway_no_naked_attributes__ void aon_pmu_ll_set_r2(uint32_t v) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r2_t *r = (aon_pmu_r2_t*)(SOC_AON_PMU_REG_BASE + (0x2 << 2));
	r->v = v;
	PRRO_SET_AON_PMU_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t aon_pmu_ll_get_r2(void) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r2_t *r = (aon_pmu_r2_t*)(SOC_AON_PMU_REG_BASE + (0x2 << 2));
	uint32_t tmp = r->v;
	PRRO_SET_AON_PMU_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void aon_pmu_ll_set_r2_wdt_rst_ana(uint32_t v) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r2_t *r = (aon_pmu_r2_t*)(SOC_AON_PMU_REG_BASE + (0x2 << 2));
	r->wdt_rst_ana = v;
	PRRO_SET_AON_PMU_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t aon_pmu_ll_get_r2_wdt_rst_ana(void) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r2_t *r = (aon_pmu_r2_t*)(SOC_AON_PMU_REG_BASE + (0x2 << 2));
	uint32_t tmp = r->wdt_rst_ana;
	PRRO_SET_AON_PMU_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void aon_pmu_ll_set_r2_wdt_rst_top(uint32_t v) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r2_t *r = (aon_pmu_r2_t*)(SOC_AON_PMU_REG_BASE + (0x2 << 2));
	r->wdt_rst_top = v;
	PRRO_SET_AON_PMU_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t aon_pmu_ll_get_r2_wdt_rst_top(void) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r2_t *r = (aon_pmu_r2_t*)(SOC_AON_PMU_REG_BASE + (0x2 << 2));
	uint32_t tmp = r->wdt_rst_top;
	PRRO_SET_AON_PMU_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void aon_pmu_ll_set_r2_wdt_rst_aon(uint32_t v) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r2_t *r = (aon_pmu_r2_t*)(SOC_AON_PMU_REG_BASE + (0x2 << 2));
	r->wdt_rst_aon = v;
	PRRO_SET_AON_PMU_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t aon_pmu_ll_get_r2_wdt_rst_aon(void) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r2_t *r = (aon_pmu_r2_t*)(SOC_AON_PMU_REG_BASE + (0x2 << 2));
	uint32_t tmp = r->wdt_rst_aon;
	PRRO_SET_AON_PMU_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void aon_pmu_ll_set_r2_wdt_rst_awt(uint32_t v) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r2_t *r = (aon_pmu_r2_t*)(SOC_AON_PMU_REG_BASE + (0x2 << 2));
	r->wdt_rst_awt = v;
	PRRO_SET_AON_PMU_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t aon_pmu_ll_get_r2_wdt_rst_awt(void) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r2_t *r = (aon_pmu_r2_t*)(SOC_AON_PMU_REG_BASE + (0x2 << 2));
	uint32_t tmp = r->wdt_rst_awt;
	PRRO_SET_AON_PMU_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void aon_pmu_ll_set_r2_wdt_rst_gpio(uint32_t v) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r2_t *r = (aon_pmu_r2_t*)(SOC_AON_PMU_REG_BASE + (0x2 << 2));
	r->wdt_rst_gpio = v;
	PRRO_SET_AON_PMU_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t aon_pmu_ll_get_r2_wdt_rst_gpio(void) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r2_t *r = (aon_pmu_r2_t*)(SOC_AON_PMU_REG_BASE + (0x2 << 2));
	uint32_t tmp = r->wdt_rst_gpio;
	PRRO_SET_AON_PMU_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void aon_pmu_ll_set_r2_wdt_rst_rtc(uint32_t v) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r2_t *r = (aon_pmu_r2_t*)(SOC_AON_PMU_REG_BASE + (0x2 << 2));
	r->wdt_rst_rtc = v;
	PRRO_SET_AON_PMU_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t aon_pmu_ll_get_r2_wdt_rst_rtc(void) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r2_t *r = (aon_pmu_r2_t*)(SOC_AON_PMU_REG_BASE + (0x2 << 2));
	uint32_t tmp = r->wdt_rst_rtc;
	PRRO_SET_AON_PMU_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void aon_pmu_ll_set_r2_wdt_rst_wdt(uint32_t v) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r2_t *r = (aon_pmu_r2_t*)(SOC_AON_PMU_REG_BASE + (0x2 << 2));
	r->wdt_rst_wdt = v;
	PRRO_SET_AON_PMU_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t aon_pmu_ll_get_r2_wdt_rst_wdt(void) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r2_t *r = (aon_pmu_r2_t*)(SOC_AON_PMU_REG_BASE + (0x2 << 2));
	uint32_t tmp = r->wdt_rst_wdt;
	PRRO_SET_AON_PMU_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void aon_pmu_ll_set_r2_wdt_rst_pmu(uint32_t v) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r2_t *r = (aon_pmu_r2_t*)(SOC_AON_PMU_REG_BASE + (0x2 << 2));
	r->wdt_rst_pmu = v;
	PRRO_SET_AON_PMU_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t aon_pmu_ll_get_r2_wdt_rst_pmu(void) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r2_t *r = (aon_pmu_r2_t*)(SOC_AON_PMU_REG_BASE + (0x2 << 2));
	uint32_t tmp = r->wdt_rst_pmu;
	PRRO_SET_AON_PMU_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void aon_pmu_ll_set_r2_wdt_rst_phy(uint32_t v) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r2_t *r = (aon_pmu_r2_t*)(SOC_AON_PMU_REG_BASE + (0x2 << 2));
	r->wdt_rst_phy = v;
	PRRO_SET_AON_PMU_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t aon_pmu_ll_get_r2_wdt_rst_phy(void) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r2_t *r = (aon_pmu_r2_t*)(SOC_AON_PMU_REG_BASE + (0x2 << 2));
	uint32_t tmp = r->wdt_rst_phy;
	PRRO_SET_AON_PMU_NON_SECURE();
	return tmp;
}

//reg r3:

__tfm_psa_secure_gateway_no_naked_attributes__ void aon_pmu_ll_set_r3(uint32_t v) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r3_t *r = (aon_pmu_r3_t*)(SOC_AON_PMU_REG_BASE + (0x3 << 2));
	r->v = v;
	PRRO_SET_AON_PMU_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t aon_pmu_ll_get_r3(void) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r3_t *r = (aon_pmu_r3_t*)(SOC_AON_PMU_REG_BASE + (0x3 << 2));
	uint32_t tmp = r->v;
	PRRO_SET_AON_PMU_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void aon_pmu_ll_set_r3_sd_en(uint32_t v) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r3_t *r = (aon_pmu_r3_t*)(SOC_AON_PMU_REG_BASE + (0x3 << 2));
	r->sd_en = v;
	PRRO_SET_AON_PMU_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t aon_pmu_ll_get_r3_sd_en(void) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r3_t *r = (aon_pmu_r3_t*)(SOC_AON_PMU_REG_BASE + (0x3 << 2));
	uint32_t tmp = r->sd_en;
	PRRO_SET_AON_PMU_NON_SECURE();
	return tmp;
}

//reg r25:

__tfm_psa_secure_gateway_no_naked_attributes__ void aon_pmu_ll_set_r25(uint32_t v) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r25_t *r = (aon_pmu_r25_t*)(SOC_AON_PMU_REG_BASE + (0x25 << 2));
	r->v = v;
	PRRO_SET_AON_PMU_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t aon_pmu_ll_get_r25(void) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r25_t *r = (aon_pmu_r25_t*)(SOC_AON_PMU_REG_BASE + (0x25 << 2));
	uint32_t tmp = r->v;
	PRRO_SET_AON_PMU_NON_SECURE();
	return tmp;
}

//reg r40:

__tfm_psa_secure_gateway_no_naked_attributes__ void aon_pmu_ll_set_r40(uint32_t v) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r40_t *r = (aon_pmu_r40_t*)(SOC_AON_PMU_REG_BASE + (0x40 << 2));
	r->v = v;
	PRRO_SET_AON_PMU_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t aon_pmu_ll_get_r40(void) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r40_t *r = (aon_pmu_r40_t*)(SOC_AON_PMU_REG_BASE + (0x40 << 2));
	uint32_t tmp = r->v;
	PRRO_SET_AON_PMU_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void aon_pmu_ll_set_r40_wake1_delay(uint32_t v) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r40_t *r = (aon_pmu_r40_t*)(SOC_AON_PMU_REG_BASE + (0x40 << 2));
	r->wake1_delay = v;
	PRRO_SET_AON_PMU_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t aon_pmu_ll_get_r40_wake1_delay(void) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r40_t *r = (aon_pmu_r40_t*)(SOC_AON_PMU_REG_BASE + (0x40 << 2));
	uint32_t tmp = r->wake1_delay;
	PRRO_SET_AON_PMU_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void aon_pmu_ll_set_r40_wake2_delay(uint32_t v) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r40_t *r = (aon_pmu_r40_t*)(SOC_AON_PMU_REG_BASE + (0x40 << 2));
	r->wake2_delay = v;
	PRRO_SET_AON_PMU_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t aon_pmu_ll_get_r40_wake2_delay(void) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r40_t *r = (aon_pmu_r40_t*)(SOC_AON_PMU_REG_BASE + (0x40 << 2));
	uint32_t tmp = r->wake2_delay;
	PRRO_SET_AON_PMU_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void aon_pmu_ll_set_r40_wake3_delay(uint32_t v) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r40_t *r = (aon_pmu_r40_t*)(SOC_AON_PMU_REG_BASE + (0x40 << 2));
	r->wake3_delay = v;
	PRRO_SET_AON_PMU_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t aon_pmu_ll_get_r40_wake3_delay(void) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r40_t *r = (aon_pmu_r40_t*)(SOC_AON_PMU_REG_BASE + (0x40 << 2));
	uint32_t tmp = r->wake3_delay;
	PRRO_SET_AON_PMU_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void aon_pmu_ll_set_r40_halt1_delay(uint32_t v) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r40_t *r = (aon_pmu_r40_t*)(SOC_AON_PMU_REG_BASE + (0x40 << 2));
	r->halt1_delay = v;
	PRRO_SET_AON_PMU_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t aon_pmu_ll_get_r40_halt1_delay(void) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r40_t *r = (aon_pmu_r40_t*)(SOC_AON_PMU_REG_BASE + (0x40 << 2));
	uint32_t tmp = r->halt1_delay;
	PRRO_SET_AON_PMU_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void aon_pmu_ll_set_r40_halt2_delay(uint32_t v) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r40_t *r = (aon_pmu_r40_t*)(SOC_AON_PMU_REG_BASE + (0x40 << 2));
	r->halt2_delay = v;
	PRRO_SET_AON_PMU_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t aon_pmu_ll_get_r40_halt2_delay(void) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r40_t *r = (aon_pmu_r40_t*)(SOC_AON_PMU_REG_BASE + (0x40 << 2));
	uint32_t tmp = r->halt2_delay;
	PRRO_SET_AON_PMU_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void aon_pmu_ll_set_r40_halt3_delay(uint32_t v) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r40_t *r = (aon_pmu_r40_t*)(SOC_AON_PMU_REG_BASE + (0x40 << 2));
	r->halt3_delay = v;
	PRRO_SET_AON_PMU_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t aon_pmu_ll_get_r40_halt3_delay(void) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r40_t *r = (aon_pmu_r40_t*)(SOC_AON_PMU_REG_BASE + (0x40 << 2));
	uint32_t tmp = r->halt3_delay;
	PRRO_SET_AON_PMU_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void aon_pmu_ll_set_r40_halt_volt(uint32_t v) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r40_t *r = (aon_pmu_r40_t*)(SOC_AON_PMU_REG_BASE + (0x40 << 2));
	r->halt_volt = v;
	PRRO_SET_AON_PMU_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t aon_pmu_ll_get_r40_halt_volt(void) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r40_t *r = (aon_pmu_r40_t*)(SOC_AON_PMU_REG_BASE + (0x40 << 2));
	uint32_t tmp = r->halt_volt;
	PRRO_SET_AON_PMU_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void aon_pmu_ll_set_r40_halt_xtal(uint32_t v) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r40_t *r = (aon_pmu_r40_t*)(SOC_AON_PMU_REG_BASE + (0x40 << 2));
	r->halt_xtal = v;
	PRRO_SET_AON_PMU_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t aon_pmu_ll_get_r40_halt_xtal(void) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r40_t *r = (aon_pmu_r40_t*)(SOC_AON_PMU_REG_BASE + (0x40 << 2));
	uint32_t tmp = r->halt_xtal;
	PRRO_SET_AON_PMU_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void aon_pmu_ll_set_r40_halt_core(uint32_t v) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r40_t *r = (aon_pmu_r40_t*)(SOC_AON_PMU_REG_BASE + (0x40 << 2));
	r->halt_core = v;
	PRRO_SET_AON_PMU_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t aon_pmu_ll_get_r40_halt_core(void) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r40_t *r = (aon_pmu_r40_t*)(SOC_AON_PMU_REG_BASE + (0x40 << 2));
	uint32_t tmp = r->halt_core;
	PRRO_SET_AON_PMU_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void aon_pmu_ll_set_r40_halt_flash(uint32_t v) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r40_t *r = (aon_pmu_r40_t*)(SOC_AON_PMU_REG_BASE + (0x40 << 2));
	r->halt_flash = v;
	PRRO_SET_AON_PMU_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t aon_pmu_ll_get_r40_halt_flash(void) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r40_t *r = (aon_pmu_r40_t*)(SOC_AON_PMU_REG_BASE + (0x40 << 2));
	uint32_t tmp = r->halt_flash;
	PRRO_SET_AON_PMU_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void aon_pmu_ll_set_r40_halt_rosc(uint32_t v) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r40_t *r = (aon_pmu_r40_t*)(SOC_AON_PMU_REG_BASE + (0x40 << 2));
	r->halt_rosc = v;
	PRRO_SET_AON_PMU_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t aon_pmu_ll_get_r40_halt_rosc(void) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r40_t *r = (aon_pmu_r40_t*)(SOC_AON_PMU_REG_BASE + (0x40 << 2));
	uint32_t tmp = r->halt_rosc;
	PRRO_SET_AON_PMU_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void aon_pmu_ll_set_r40_halt_resten(uint32_t v) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r40_t *r = (aon_pmu_r40_t*)(SOC_AON_PMU_REG_BASE + (0x40 << 2));
	r->halt_resten = v;
	PRRO_SET_AON_PMU_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t aon_pmu_ll_get_r40_halt_resten(void) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r40_t *r = (aon_pmu_r40_t*)(SOC_AON_PMU_REG_BASE + (0x40 << 2));
	uint32_t tmp = r->halt_resten;
	PRRO_SET_AON_PMU_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void aon_pmu_ll_set_r40_halt_isolat(uint32_t v) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r40_t *r = (aon_pmu_r40_t*)(SOC_AON_PMU_REG_BASE + (0x40 << 2));
	r->halt_isolat = v;
	PRRO_SET_AON_PMU_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t aon_pmu_ll_get_r40_halt_isolat(void) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r40_t *r = (aon_pmu_r40_t*)(SOC_AON_PMU_REG_BASE + (0x40 << 2));
	uint32_t tmp = r->halt_isolat;
	PRRO_SET_AON_PMU_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void aon_pmu_ll_set_r40_halt_clkena(uint32_t v) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r40_t *r = (aon_pmu_r40_t*)(SOC_AON_PMU_REG_BASE + (0x40 << 2));
	r->halt_clkena = v;
	PRRO_SET_AON_PMU_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t aon_pmu_ll_get_r40_halt_clkena(void) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r40_t *r = (aon_pmu_r40_t*)(SOC_AON_PMU_REG_BASE + (0x40 << 2));
	uint32_t tmp = r->halt_clkena;
	PRRO_SET_AON_PMU_NON_SECURE();
	return tmp;
}

//reg r41:

__tfm_psa_secure_gateway_no_naked_attributes__ void aon_pmu_ll_set_r41(uint32_t v) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r41_t *r = (aon_pmu_r41_t*)(SOC_AON_PMU_REG_BASE + (0x41 << 2));
	r->v = v;
	PRRO_SET_AON_PMU_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t aon_pmu_ll_get_r41(void) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r41_t *r = (aon_pmu_r41_t*)(SOC_AON_PMU_REG_BASE + (0x41 << 2));
	uint32_t tmp = r->v;
	PRRO_SET_AON_PMU_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void aon_pmu_ll_set_r41_lpo_config(uint32_t v) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r41_t *r = (aon_pmu_r41_t*)(SOC_AON_PMU_REG_BASE + (0x41 << 2));
	r->lpo_config = v;
	PRRO_SET_AON_PMU_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t aon_pmu_ll_get_r41_lpo_config(void) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r41_t *r = (aon_pmu_r41_t*)(SOC_AON_PMU_REG_BASE + (0x41 << 2));
	uint32_t tmp = r->lpo_config;
	PRRO_SET_AON_PMU_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void aon_pmu_ll_set_r41_flshsck_iocap(uint32_t v) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r41_t *r = (aon_pmu_r41_t*)(SOC_AON_PMU_REG_BASE + (0x41 << 2));
	r->flshsck_iocap = v;
	PRRO_SET_AON_PMU_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t aon_pmu_ll_get_r41_flshsck_iocap(void) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r41_t *r = (aon_pmu_r41_t*)(SOC_AON_PMU_REG_BASE + (0x41 << 2));
	uint32_t tmp = r->flshsck_iocap;
	PRRO_SET_AON_PMU_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void aon_pmu_ll_set_r41_wakeup_ena(uint32_t v) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r41_t *r = (aon_pmu_r41_t*)(SOC_AON_PMU_REG_BASE + (0x41 << 2));
	r->wakeup_ena = v;
	PRRO_SET_AON_PMU_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t aon_pmu_ll_get_r41_wakeup_ena(void) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r41_t *r = (aon_pmu_r41_t*)(SOC_AON_PMU_REG_BASE + (0x41 << 2));
	uint32_t tmp = r->wakeup_ena;
	PRRO_SET_AON_PMU_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void aon_pmu_ll_set_r41_io_drv(uint32_t v) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r41_t *r = (aon_pmu_r41_t*)(SOC_AON_PMU_REG_BASE + (0x41 << 2));
	r->io_drv = v;
	PRRO_SET_AON_PMU_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t aon_pmu_ll_get_r41_io_drv(void) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r41_t *r = (aon_pmu_r41_t*)(SOC_AON_PMU_REG_BASE + (0x41 << 2));
	uint32_t tmp = r->io_drv;
	PRRO_SET_AON_PMU_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void aon_pmu_ll_set_r41_xtal_sel(uint32_t v) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r41_t *r = (aon_pmu_r41_t*)(SOC_AON_PMU_REG_BASE + (0x41 << 2));
	r->xtal_sel = v;
	PRRO_SET_AON_PMU_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t aon_pmu_ll_get_r41_xtal_sel(void) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r41_t *r = (aon_pmu_r41_t*)(SOC_AON_PMU_REG_BASE + (0x41 << 2));
	uint32_t tmp = r->xtal_sel;
	PRRO_SET_AON_PMU_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void aon_pmu_ll_set_r41_halt_lpo(uint32_t v) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r41_t *r = (aon_pmu_r41_t*)(SOC_AON_PMU_REG_BASE + (0x41 << 2));
	r->halt_lpo = v;
	PRRO_SET_AON_PMU_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t aon_pmu_ll_get_r41_halt_lpo(void) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r41_t *r = (aon_pmu_r41_t*)(SOC_AON_PMU_REG_BASE + (0x41 << 2));
	uint32_t tmp = r->halt_lpo;
	PRRO_SET_AON_PMU_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void aon_pmu_ll_set_r41_halt_busrst(uint32_t v) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r41_t *r = (aon_pmu_r41_t*)(SOC_AON_PMU_REG_BASE + (0x41 << 2));
	r->halt_busrst = v;
	PRRO_SET_AON_PMU_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t aon_pmu_ll_get_r41_halt_busrst(void) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r41_t *r = (aon_pmu_r41_t*)(SOC_AON_PMU_REG_BASE + (0x41 << 2));
	uint32_t tmp = r->halt_busrst;
	PRRO_SET_AON_PMU_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void aon_pmu_ll_set_r41_halt_busiso(uint32_t v) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r41_t *r = (aon_pmu_r41_t*)(SOC_AON_PMU_REG_BASE + (0x41 << 2));
	r->halt_busiso = v;
	PRRO_SET_AON_PMU_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t aon_pmu_ll_get_r41_halt_busiso(void) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r41_t *r = (aon_pmu_r41_t*)(SOC_AON_PMU_REG_BASE + (0x41 << 2));
	uint32_t tmp = r->halt_busiso;
	PRRO_SET_AON_PMU_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void aon_pmu_ll_set_r41_halt_buspwd(uint32_t v) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r41_t *r = (aon_pmu_r41_t*)(SOC_AON_PMU_REG_BASE + (0x41 << 2));
	r->halt_buspwd = v;
	PRRO_SET_AON_PMU_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t aon_pmu_ll_get_r41_halt_buspwd(void) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r41_t *r = (aon_pmu_r41_t*)(SOC_AON_PMU_REG_BASE + (0x41 << 2));
	uint32_t tmp = r->halt_buspwd;
	PRRO_SET_AON_PMU_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void aon_pmu_ll_set_r41_halt_blpiso(uint32_t v) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r41_t *r = (aon_pmu_r41_t*)(SOC_AON_PMU_REG_BASE + (0x41 << 2));
	r->halt_blpiso = v;
	PRRO_SET_AON_PMU_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t aon_pmu_ll_get_r41_halt_blpiso(void) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r41_t *r = (aon_pmu_r41_t*)(SOC_AON_PMU_REG_BASE + (0x41 << 2));
	uint32_t tmp = r->halt_blpiso;
	PRRO_SET_AON_PMU_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void aon_pmu_ll_set_r41_halt_blppwd(uint32_t v) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r41_t *r = (aon_pmu_r41_t*)(SOC_AON_PMU_REG_BASE + (0x41 << 2));
	r->halt_blppwd = v;
	PRRO_SET_AON_PMU_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t aon_pmu_ll_get_r41_halt_blppwd(void) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r41_t *r = (aon_pmu_r41_t*)(SOC_AON_PMU_REG_BASE + (0x41 << 2));
	uint32_t tmp = r->halt_blppwd;
	PRRO_SET_AON_PMU_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void aon_pmu_ll_set_r41_halt_wlpiso(uint32_t v) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r41_t *r = (aon_pmu_r41_t*)(SOC_AON_PMU_REG_BASE + (0x41 << 2));
	r->halt_wlpiso = v;
	PRRO_SET_AON_PMU_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t aon_pmu_ll_get_r41_halt_wlpiso(void) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r41_t *r = (aon_pmu_r41_t*)(SOC_AON_PMU_REG_BASE + (0x41 << 2));
	uint32_t tmp = r->halt_wlpiso;
	PRRO_SET_AON_PMU_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void aon_pmu_ll_set_r41_halt_wlppwd(uint32_t v) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r41_t *r = (aon_pmu_r41_t*)(SOC_AON_PMU_REG_BASE + (0x41 << 2));
	r->halt_wlppwd = v;
	PRRO_SET_AON_PMU_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t aon_pmu_ll_get_r41_halt_wlppwd(void) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r41_t *r = (aon_pmu_r41_t*)(SOC_AON_PMU_REG_BASE + (0x41 << 2));
	uint32_t tmp = r->halt_wlppwd;
	PRRO_SET_AON_PMU_NON_SECURE();
	return tmp;
}

//reg r43:

__tfm_psa_secure_gateway_no_naked_attributes__ void aon_pmu_ll_set_r43(uint32_t v) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r43_t *r = (aon_pmu_r43_t*)(SOC_AON_PMU_REG_BASE + (0x43 << 2));
	r->v = v;
	PRRO_SET_AON_PMU_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t aon_pmu_ll_get_r43(void) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r43_t *r = (aon_pmu_r43_t*)(SOC_AON_PMU_REG_BASE + (0x43 << 2));
	uint32_t tmp = r->v;
	PRRO_SET_AON_PMU_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void aon_pmu_ll_set_r43_clr_int_touched(uint32_t v) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r43_t *r = (aon_pmu_r43_t*)(SOC_AON_PMU_REG_BASE + (0x43 << 2));
	r->clr_int_touched = v;
	PRRO_SET_AON_PMU_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t aon_pmu_ll_get_r43_clr_int_touched(void) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r43_t *r = (aon_pmu_r43_t*)(SOC_AON_PMU_REG_BASE + (0x43 << 2));
	uint32_t tmp = r->clr_int_touched;
	PRRO_SET_AON_PMU_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void aon_pmu_ll_set_r43_clr_int_usbplug(uint32_t v) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r43_t *r = (aon_pmu_r43_t*)(SOC_AON_PMU_REG_BASE + (0x43 << 2));
	r->clr_int_usbplug = v;
	PRRO_SET_AON_PMU_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t aon_pmu_ll_get_r43_clr_int_usbplug(void) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r43_t *r = (aon_pmu_r43_t*)(SOC_AON_PMU_REG_BASE + (0x43 << 2));
	uint32_t tmp = r->clr_int_usbplug;
	PRRO_SET_AON_PMU_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ void aon_pmu_ll_set_r43_clr_wakeup(uint32_t v) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r43_t *r = (aon_pmu_r43_t*)(SOC_AON_PMU_REG_BASE + (0x43 << 2));
	r->clr_wakeup = v;
	PRRO_SET_AON_PMU_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t aon_pmu_ll_get_r43_clr_wakeup(void) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r43_t *r = (aon_pmu_r43_t*)(SOC_AON_PMU_REG_BASE + (0x43 << 2));
	uint32_t tmp = r->clr_wakeup;
	PRRO_SET_AON_PMU_NON_SECURE();
	return tmp;
}

//reg r70:

__tfm_psa_secure_gateway_no_naked_attributes__ void aon_pmu_ll_set_r70(uint32_t v) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r70_t *r = (aon_pmu_r70_t*)(SOC_AON_PMU_REG_BASE + (0x70 << 2));
	r->v = v;
	PRRO_SET_AON_PMU_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t aon_pmu_ll_get_r70(void) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r70_t *r = (aon_pmu_r70_t*)(SOC_AON_PMU_REG_BASE + (0x70 << 2));
	uint32_t tmp = r->v;
	PRRO_SET_AON_PMU_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t aon_pmu_ll_get_r70_int_touched(void) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r70_t *r = (aon_pmu_r70_t*)(SOC_AON_PMU_REG_BASE + (0x70 << 2));
	uint32_t tmp = r->int_touched;
	PRRO_SET_AON_PMU_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t aon_pmu_ll_get_r70_int_usbplug(void) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r70_t *r = (aon_pmu_r70_t*)(SOC_AON_PMU_REG_BASE + (0x70 << 2));
	uint32_t tmp = r->int_usbplug;
	PRRO_SET_AON_PMU_NON_SECURE();
	return tmp;
}

//reg r71:

__tfm_psa_secure_gateway_no_naked_attributes__ void aon_pmu_ll_set_r71(uint32_t v) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r71_t *r = (aon_pmu_r71_t*)(SOC_AON_PMU_REG_BASE + (0x71 << 2));
	r->v = v;
	PRRO_SET_AON_PMU_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t aon_pmu_ll_get_r71(void) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r71_t *r = (aon_pmu_r71_t*)(SOC_AON_PMU_REG_BASE + (0x71 << 2));
	uint32_t tmp = r->v;
	PRRO_SET_AON_PMU_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t aon_pmu_ll_get_r71_touch_state(void) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r71_t *r = (aon_pmu_r71_t*)(SOC_AON_PMU_REG_BASE + (0x71 << 2));
	uint32_t tmp = r->touch_state;
	PRRO_SET_AON_PMU_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t aon_pmu_ll_get_r71_usbplug_state(void) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r71_t *r = (aon_pmu_r71_t*)(SOC_AON_PMU_REG_BASE + (0x71 << 2));
	uint32_t tmp = r->usbplug_state;
	PRRO_SET_AON_PMU_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t aon_pmu_ll_get_r71_wakeup_source(void) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r71_t *r = (aon_pmu_r71_t*)(SOC_AON_PMU_REG_BASE + (0x71 << 2));
	uint32_t tmp = r->wakeup_source;
	PRRO_SET_AON_PMU_NON_SECURE();
	return tmp;
}

//reg reg72:

__tfm_psa_secure_gateway_no_naked_attributes__ void aon_pmu_ll_set_reg72_value(uint32_t v) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r72_t *r = (aon_pmu_r72_t*)(SOC_AON_PMU_REG_BASE + (0x72 << 2));
	r->v = v;
	PRRO_SET_AON_PMU_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t aon_pmu_ll_get_reg72_value(void) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r72_t *r = (aon_pmu_r72_t*)(SOC_AON_PMU_REG_BASE + (0x72 << 2));
	uint32_t tmp = r->v;
	PRRO_SET_AON_PMU_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t aon_pmu_ll_get_reg72_vdw_soft1v(void) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r72_t *r = (aon_pmu_r72_t*)(SOC_AON_PMU_REG_BASE + (0x72 << 2));
	uint32_t tmp = r->vdw_soft1v;
	PRRO_SET_AON_PMU_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t aon_pmu_ll_get_reg72_vup_soft1v(void) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r72_t *r = (aon_pmu_r72_t*)(SOC_AON_PMU_REG_BASE + (0x72 << 2));
	uint32_t tmp = r->vup_soft1v;
	PRRO_SET_AON_PMU_NON_SECURE();
	return tmp;
}

//reg reg73:

__tfm_psa_secure_gateway_no_naked_attributes__ void aon_pmu_ll_set_reg73_value(uint32_t v) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r73_t *r = (aon_pmu_r73_t*)(SOC_AON_PMU_REG_BASE + (0x73 << 2));
	r->v = v;
	PRRO_SET_AON_PMU_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t aon_pmu_ll_get_reg73_value(void) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r73_t *r = (aon_pmu_r73_t*)(SOC_AON_PMU_REG_BASE + (0x73 << 2));
	uint32_t tmp = r->v;
	PRRO_SET_AON_PMU_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t aon_pmu_ll_get_reg73_mul_touch_int(void) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r73_t *r = (aon_pmu_r73_t*)(SOC_AON_PMU_REG_BASE + (0x73 << 2));
	uint32_t tmp = r->mul_touch_int;
	PRRO_SET_AON_PMU_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t aon_pmu_ll_get_reg73_wake_up_soft(void) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r73_t *r = (aon_pmu_r73_t*)(SOC_AON_PMU_REG_BASE + (0x73 << 2));
	uint32_t tmp = r->wake_up_soft;
	PRRO_SET_AON_PMU_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t aon_pmu_ll_get_reg73_cap_cal(void) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r73_t *r = (aon_pmu_r73_t*)(SOC_AON_PMU_REG_BASE + (0x73 << 2));
	uint32_t tmp = r->cap_cal;
	PRRO_SET_AON_PMU_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t aon_pmu_ll_get_reg73_cal_done(void) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r73_t *r = (aon_pmu_r73_t*)(SOC_AON_PMU_REG_BASE + (0x73 << 2));
	uint32_t tmp = r->cal_done;
	PRRO_SET_AON_PMU_NON_SECURE();
	return tmp;
}

//reg r7c:

__tfm_psa_secure_gateway_no_naked_attributes__ void aon_pmu_ll_set_r7c(uint32_t v) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r7c_t *r = (aon_pmu_r7c_t*)(SOC_AON_PMU_REG_BASE + (0x7c << 2));
	r->v = v;
	PRRO_SET_AON_PMU_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t aon_pmu_ll_get_r7c(void) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r7c_t *r = (aon_pmu_r7c_t*)(SOC_AON_PMU_REG_BASE + (0x7c << 2));
	uint32_t tmp = r->v;
	PRRO_SET_AON_PMU_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t aon_pmu_ll_get_r7c_id(void) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r7c_t *r = (aon_pmu_r7c_t*)(SOC_AON_PMU_REG_BASE + (0x7c << 2));
	uint32_t tmp = r->id;
	PRRO_SET_AON_PMU_NON_SECURE();
	return tmp;
}

//reg r7d:

__tfm_psa_secure_gateway_no_naked_attributes__ void aon_pmu_ll_set_r7d(uint32_t v) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r7d_t *r = (aon_pmu_r7d_t*)(SOC_AON_PMU_REG_BASE + (0x7d << 2));
	r->v = v;
	PRRO_SET_AON_PMU_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t aon_pmu_ll_get_r7d(void) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r7d_t *r = (aon_pmu_r7d_t*)(SOC_AON_PMU_REG_BASE + (0x7d << 2));
	uint32_t tmp = r->v;
	PRRO_SET_AON_PMU_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t aon_pmu_ll_get_r7d_lcal(void) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r7d_t *r = (aon_pmu_r7d_t*)(SOC_AON_PMU_REG_BASE + (0x7d << 2));
	uint32_t tmp = r->lcal;
	PRRO_SET_AON_PMU_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t aon_pmu_ll_get_r7d_l(void) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r7d_t *r = (aon_pmu_r7d_t*)(SOC_AON_PMU_REG_BASE + (0x7d << 2));
	uint32_t tmp = r->l;
	PRRO_SET_AON_PMU_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t aon_pmu_ll_get_r7d_adc_cal(void) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r7d_t *r = (aon_pmu_r7d_t*)(SOC_AON_PMU_REG_BASE + (0x7d << 2));
	uint32_t tmp = r->adc_cal;
	PRRO_SET_AON_PMU_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t aon_pmu_ll_get_r7d_bgcal(void) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r7d_t *r = (aon_pmu_r7d_t*)(SOC_AON_PMU_REG_BASE + (0x7d << 2));
	uint32_t tmp = r->bgcal;
	PRRO_SET_AON_PMU_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t aon_pmu_ll_get_r7d_26mpll_unlock(void) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r7d_t *r = (aon_pmu_r7d_t*)(SOC_AON_PMU_REG_BASE + (0x7d << 2));
	uint32_t tmp = r->pll_26m_unlock;
	PRRO_SET_AON_PMU_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t aon_pmu_ll_get_r7d_dpll_unlock(void) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r7d_t *r = (aon_pmu_r7d_t*)(SOC_AON_PMU_REG_BASE + (0x7d << 2));
	uint32_t tmp = r->dpll_unlock;
	PRRO_SET_AON_PMU_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t aon_pmu_ll_get_r7d_h1(void) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r7d_t *r = (aon_pmu_r7d_t*)(SOC_AON_PMU_REG_BASE + (0x7d << 2));
	uint32_t tmp = r->h1;
	PRRO_SET_AON_PMU_NON_SECURE();
	return tmp;
}

//reg r7e:

__tfm_psa_secure_gateway_no_naked_attributes__ void aon_pmu_ll_set_r7e(uint32_t v) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r7e_t *r = (aon_pmu_r7e_t*)(SOC_AON_PMU_REG_BASE + (0x7e << 2));
	r->v = v;
	PRRO_SET_AON_PMU_NON_SECURE();
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t aon_pmu_ll_get_r7e(void) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r7e_t *r = (aon_pmu_r7e_t*)(SOC_AON_PMU_REG_BASE + (0x7e << 2));
	uint32_t tmp = r->v;
	PRRO_SET_AON_PMU_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t aon_pmu_ll_get_r7e_cbcal(void) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r7e_t *r = (aon_pmu_r7e_t*)(SOC_AON_PMU_REG_BASE + (0x7e << 2));
	uint32_t tmp = r->cbcal;
	PRRO_SET_AON_PMU_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t aon_pmu_ll_get_r7e_ad_state(void) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r7e_t *r = (aon_pmu_r7e_t*)(SOC_AON_PMU_REG_BASE + (0x7e << 2));
	uint32_t tmp = r->ad_state;
	PRRO_SET_AON_PMU_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t aon_pmu_ll_get_r7e_td_states1(void) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r7e_t *r = (aon_pmu_r7e_t*)(SOC_AON_PMU_REG_BASE + (0x7e << 2));
	uint32_t tmp = r->td_states1;
	PRRO_SET_AON_PMU_NON_SECURE();
	return tmp;
}

__tfm_psa_secure_gateway_no_naked_attributes__ uint32_t aon_pmu_ll_get_r7e_h2(void) {
	PRRO_AON_PMU_SECURE_DECLARATION();
	PRRO_SET_AON_PMU_SECURE();
	aon_pmu_r7e_t *r = (aon_pmu_r7e_t*)(SOC_AON_PMU_REG_BASE + (0x7e << 2));
	uint32_t tmp = r->h2;
	PRRO_SET_AON_PMU_NON_SECURE();
	return tmp;
}
