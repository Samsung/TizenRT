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
#include "prro_hw_v5.h"

#ifdef __cplusplus
extern "C" {
#endif

#define PRRO_LL_REG_BASE   SOC_PRRO_REG_BASE

//reg device_id:

static inline void prro_ll_set_device_id_value(uint32_t v) {
	prro_device_id_t *r = (prro_device_id_t*)(SOC_PRRO_REG_BASE + (0x0 << 2));
	r->v = v;
}

static inline uint32_t prro_ll_get_device_id_value(void) {
	prro_device_id_t *r = (prro_device_id_t*)(SOC_PRRO_REG_BASE + (0x0 << 2));
	return r->v;
}

static inline void prro_ll_set_device_id_deviceid(uint32_t v) {
	prro_device_id_t *r = (prro_device_id_t*)(SOC_PRRO_REG_BASE + (0x0 << 2));
	r->deviceid = v;
}

static inline uint32_t prro_ll_get_device_id_deviceid(void) {
	prro_device_id_t *r = (prro_device_id_t*)(SOC_PRRO_REG_BASE + (0x0 << 2));
	return r->deviceid;
}

//reg version_id:

static inline void prro_ll_set_version_id_value(uint32_t v) {
	prro_version_id_t *r = (prro_version_id_t*)(SOC_PRRO_REG_BASE + (0x1 << 2));
	r->v = v;
}

static inline uint32_t prro_ll_get_version_id_value(void) {
	prro_version_id_t *r = (prro_version_id_t*)(SOC_PRRO_REG_BASE + (0x1 << 2));
	return r->v;
}

static inline void prro_ll_set_version_id_versionid(uint32_t v) {
	prro_version_id_t *r = (prro_version_id_t*)(SOC_PRRO_REG_BASE + (0x1 << 2));
	r->versionid = v;
}

static inline uint32_t prro_ll_get_version_id_versionid(void) {
	prro_version_id_t *r = (prro_version_id_t*)(SOC_PRRO_REG_BASE + (0x1 << 2));
	return r->versionid;
}

//reg gbl_control:

static inline void prro_ll_set_gbl_control_value(uint32_t v) {
	prro_gbl_control_t *r = (prro_gbl_control_t*)(SOC_PRRO_REG_BASE + (0x2 << 2));
	r->v = v;
}

static inline uint32_t prro_ll_get_gbl_control_value(void) {
	prro_gbl_control_t *r = (prro_gbl_control_t*)(SOC_PRRO_REG_BASE + (0x2 << 2));
	return r->v;
}

static inline void prro_ll_set_gbl_control_soft_reset(uint32_t v) {
	prro_gbl_control_t *r = (prro_gbl_control_t*)(SOC_PRRO_REG_BASE + (0x2 << 2));
	r->soft_reset = v;
}

static inline uint32_t prro_ll_get_gbl_control_soft_reset(void) {
	prro_gbl_control_t *r = (prro_gbl_control_t*)(SOC_PRRO_REG_BASE + (0x2 << 2));
	return r->soft_reset;
}

static inline void prro_ll_set_gbl_control_clkg_bps(uint32_t v) {
	prro_gbl_control_t *r = (prro_gbl_control_t*)(SOC_PRRO_REG_BASE + (0x2 << 2));
	r->clkg_bps = v;
}

static inline uint32_t prro_ll_get_gbl_control_clkg_bps(void) {
	prro_gbl_control_t *r = (prro_gbl_control_t*)(SOC_PRRO_REG_BASE + (0x2 << 2));
	return r->clkg_bps;
}

static inline void prro_ll_set_gbl_control_reserved_bit_2_31(uint32_t v) {
	prro_gbl_control_t *r = (prro_gbl_control_t*)(SOC_PRRO_REG_BASE + (0x2 << 2));
	r->reserved_bit_2_31 = v;
}

static inline uint32_t prro_ll_get_gbl_control_reserved_bit_2_31(void) {
	prro_gbl_control_t *r = (prro_gbl_control_t*)(SOC_PRRO_REG_BASE + (0x2 << 2));
	return r->reserved_bit_2_31;
}

//reg gbl_status:

static inline void prro_ll_set_gbl_status_value(uint32_t v) {
	prro_gbl_status_t *r = (prro_gbl_status_t*)(SOC_PRRO_REG_BASE + (0x3 << 2));
	r->v = v;
}

static inline uint32_t prro_ll_get_gbl_status_value(void) {
	prro_gbl_status_t *r = (prro_gbl_status_t*)(SOC_PRRO_REG_BASE + (0x3 << 2));
	return r->v;
}

static inline void prro_ll_set_gbl_status_devstatus(uint32_t v) {
	prro_gbl_status_t *r = (prro_gbl_status_t*)(SOC_PRRO_REG_BASE + (0x3 << 2));
	r->devstatus = v;
}

static inline uint32_t prro_ll_get_gbl_status_devstatus(void) {
	prro_gbl_status_t *r = (prro_gbl_status_t*)(SOC_PRRO_REG_BASE + (0x3 << 2));
	return r->devstatus;
}

//reg aonp_gpio_nsec0:

static inline void prro_ll_set_aonp_gpio_nsec0_value(uint32_t v) {
	prro_aonp_gpio_nsec0_t *r = (prro_aonp_gpio_nsec0_t*)(SOC_PRRO_REG_BASE + (0x4 << 2));
	r->v = v;
}

static inline uint32_t prro_ll_get_aonp_gpio_nsec0_value(void) {
	prro_aonp_gpio_nsec0_t *r = (prro_aonp_gpio_nsec0_t*)(SOC_PRRO_REG_BASE + (0x4 << 2));
	return r->v;
}

static inline void prro_ll_set_aonp_gpio_nsec0_aon_gpio_nsec0(uint32_t v) {
	prro_aonp_gpio_nsec0_t *r = (prro_aonp_gpio_nsec0_t*)(SOC_PRRO_REG_BASE + (0x4 << 2));
	r->aon_gpio_nsec0 = v;
}

static inline uint32_t prro_ll_get_aonp_gpio_nsec0_aon_gpio_nsec0(void) {
	prro_aonp_gpio_nsec0_t *r = (prro_aonp_gpio_nsec0_t*)(SOC_PRRO_REG_BASE + (0x4 << 2));
	return r->aon_gpio_nsec0;
}

static inline void prro_ll_set_bit_aon_gpio_nonsec0(uint32_t bit, uint32_t value)
{
	prro_aonp_gpio_nsec0_t *r = (prro_aonp_gpio_nsec0_t*)(SOC_PRRO_REG_BASE + (0x4 << 2));
	if (value) {
		r->aon_gpio_nsec0 |= BIT(bit);
	} else {
		r->aon_gpio_nsec0 &= ~(BIT(bit));
	}
}

//reg aonp_gpio_nsec1:

static inline void prro_ll_set_aonp_gpio_nsec1_value(uint32_t v) {
	prro_aonp_gpio_nsec1_t *r = (prro_aonp_gpio_nsec1_t*)(SOC_PRRO_REG_BASE + (0x5 << 2));
	r->v = v;
}

static inline uint32_t prro_ll_get_aonp_gpio_nsec1_value(void) {
	prro_aonp_gpio_nsec1_t *r = (prro_aonp_gpio_nsec1_t*)(SOC_PRRO_REG_BASE + (0x5 << 2));
	return r->v;
}

static inline void prro_ll_set_aonp_gpio_nsec1_aon_gpio_nsec1(uint32_t v) {
	prro_aonp_gpio_nsec1_t *r = (prro_aonp_gpio_nsec1_t*)(SOC_PRRO_REG_BASE + (0x5 << 2));
	r->aon_gpio_nsec1 = v;
}

static inline uint32_t prro_ll_get_aonp_gpio_nsec1_aon_gpio_nsec1(void) {
	prro_aonp_gpio_nsec1_t *r = (prro_aonp_gpio_nsec1_t*)(SOC_PRRO_REG_BASE + (0x5 << 2));
	return r->aon_gpio_nsec1;
}

static inline void prro_ll_set_bit_aon_gpio_nonsec1(uint32_t bit, uint32_t value)
{
	prro_aonp_gpio_nsec1_t *r = (prro_aonp_gpio_nsec1_t*)(SOC_PRRO_REG_BASE + (0x5 << 2));
	if (value) {
		r->aon_gpio_nsec1 |= BIT(bit);
	} else {
		r->aon_gpio_nsec1 &= ~(BIT(bit));
	}
}

//reg aonp_sap:

static inline void prro_ll_set_aonp_sap_value(uint32_t v) {
	prro_aonp_sap_t *r = (prro_aonp_sap_t*)(SOC_PRRO_REG_BASE + (0x6 << 2));
	r->v = v;
}

static inline uint32_t prro_ll_get_aonp_sap_value(void) {
	prro_aonp_sap_t *r = (prro_aonp_sap_t*)(SOC_PRRO_REG_BASE + (0x6 << 2));
	return r->v;
}

static inline void prro_ll_set_aonp_sap_aon_reg_nsec(uint32_t v) {
	prro_aonp_sap_t *r = (prro_aonp_sap_t*)(SOC_PRRO_REG_BASE + (0x6 << 2));
	r->aon_reg_nsec = v;
}

static inline uint32_t prro_ll_get_aonp_sap_aon_reg_nsec(void) {
	prro_aonp_sap_t *r = (prro_aonp_sap_t*)(SOC_PRRO_REG_BASE + (0x6 << 2));
	return r->aon_reg_nsec;
}

static inline void prro_ll_set_aonp_sap_aon_rtc_nsec(uint32_t v) {
	prro_aonp_sap_t *r = (prro_aonp_sap_t*)(SOC_PRRO_REG_BASE + (0x6 << 2));
	r->aon_rtc_nsec = v;
}

static inline uint32_t prro_ll_get_aonp_sap_aon_rtc_nsec(void) {
	prro_aonp_sap_t *r = (prro_aonp_sap_t*)(SOC_PRRO_REG_BASE + (0x6 << 2));
	return r->aon_rtc_nsec;
}

static inline void prro_ll_set_aonp_sap_aon_gpiohig_nsec(uint32_t v) {
	prro_aonp_sap_t *r = (prro_aonp_sap_t*)(SOC_PRRO_REG_BASE + (0x6 << 2));
	r->aon_gpiohig_nsec = v;
}

static inline uint32_t prro_ll_get_aonp_sap_aon_gpiohig_nsec(void) {
	prro_aonp_sap_t *r = (prro_aonp_sap_t*)(SOC_PRRO_REG_BASE + (0x6 << 2));
	return r->aon_gpiohig_nsec;
}

static inline void prro_ll_set_aonp_sap_aon_wdt_nsec(uint32_t v) {
	prro_aonp_sap_t *r = (prro_aonp_sap_t*)(SOC_PRRO_REG_BASE + (0x6 << 2));
	r->aon_wdt_nsec = v;
}

static inline uint32_t prro_ll_get_aonp_sap_aon_wdt_nsec(void) {
	prro_aonp_sap_t *r = (prro_aonp_sap_t*)(SOC_PRRO_REG_BASE + (0x6 << 2));
	return r->aon_wdt_nsec;
}

static inline void prro_ll_set_aonp_sap_aon_ahb_aon_nsec(uint32_t v) {
	prro_aonp_sap_t *r = (prro_aonp_sap_t*)(SOC_PRRO_REG_BASE + (0x6 << 2));
	r->aon_ahb_aon_nsec = v;
}

static inline uint32_t prro_ll_get_aonp_sap_aon_ahb_aon_nsec(void) {
	prro_aonp_sap_t *r = (prro_aonp_sap_t*)(SOC_PRRO_REG_BASE + (0x6 << 2));
	return r->aon_ahb_aon_nsec;
}

static inline void prro_ll_set_aonp_sap_aon_ahb_sys_nsec(uint32_t v) {
	prro_aonp_sap_t *r = (prro_aonp_sap_t*)(SOC_PRRO_REG_BASE + (0x6 << 2));
	r->aon_ahb_sys_nsec = v;
}

static inline uint32_t prro_ll_get_aonp_sap_aon_ahb_sys_nsec(void) {
	prro_aonp_sap_t *r = (prro_aonp_sap_t*)(SOC_PRRO_REG_BASE + (0x6 << 2));
	return r->aon_ahb_sys_nsec;
}

static inline void prro_ll_set_aonp_sap_aon_ahb_flsh_nsec(uint32_t v) {
	prro_aonp_sap_t *r = (prro_aonp_sap_t*)(SOC_PRRO_REG_BASE + (0x6 << 2));
	r->aon_ahb_flsh_nsec = v;
}

static inline uint32_t prro_ll_get_aonp_sap_aon_ahb_flsh_nsec(void) {
	prro_aonp_sap_t *r = (prro_aonp_sap_t*)(SOC_PRRO_REG_BASE + (0x6 << 2));
	return r->aon_ahb_flsh_nsec;
}

static inline void prro_ll_set_aonp_sap_aon_ahb_apb_nsec(uint32_t v) {
	prro_aonp_sap_t *r = (prro_aonp_sap_t*)(SOC_PRRO_REG_BASE + (0x6 << 2));
	r->aon_ahb_apb_nsec = v;
}

static inline uint32_t prro_ll_get_aonp_sap_aon_ahb_apb_nsec(void) {
	prro_aonp_sap_t *r = (prro_aonp_sap_t*)(SOC_PRRO_REG_BASE + (0x6 << 2));
	return r->aon_ahb_apb_nsec;
}

static inline void prro_ll_set_aonp_sap_aon_ahb_aon_ap(uint32_t v) {
	prro_aonp_sap_t *r = (prro_aonp_sap_t*)(SOC_PRRO_REG_BASE + (0x6 << 2));
	r->aon_ahb_aon_ap = v;
}

static inline uint32_t prro_ll_get_aonp_sap_aon_ahb_aon_ap(void) {
	prro_aonp_sap_t *r = (prro_aonp_sap_t*)(SOC_PRRO_REG_BASE + (0x6 << 2));
	return r->aon_ahb_aon_ap;
}

static inline void prro_ll_set_aonp_sap_aon_ahb_sys_ap(uint32_t v) {
	prro_aonp_sap_t *r = (prro_aonp_sap_t*)(SOC_PRRO_REG_BASE + (0x6 << 2));
	r->aon_ahb_sys_ap = v;
}

static inline uint32_t prro_ll_get_aonp_sap_aon_ahb_sys_ap(void) {
	prro_aonp_sap_t *r = (prro_aonp_sap_t*)(SOC_PRRO_REG_BASE + (0x6 << 2));
	return r->aon_ahb_sys_ap;
}

static inline void prro_ll_set_aonp_sap_aon_ahb_flsh_ap(uint32_t v) {
	prro_aonp_sap_t *r = (prro_aonp_sap_t*)(SOC_PRRO_REG_BASE + (0x6 << 2));
	r->aon_ahb_flsh_ap = v;
}

static inline uint32_t prro_ll_get_aonp_sap_aon_ahb_flsh_ap(void) {
	prro_aonp_sap_t *r = (prro_aonp_sap_t*)(SOC_PRRO_REG_BASE + (0x6 << 2));
	return r->aon_ahb_flsh_ap;
}

static inline void prro_ll_set_aonp_sap_aon_ahb_apb_ap(uint32_t v) {
	prro_aonp_sap_t *r = (prro_aonp_sap_t*)(SOC_PRRO_REG_BASE + (0x6 << 2));
	r->aon_ahb_apb_ap = v;
}

static inline uint32_t prro_ll_get_aonp_sap_aon_ahb_apb_ap(void) {
	prro_aonp_sap_t *r = (prro_aonp_sap_t*)(SOC_PRRO_REG_BASE + (0x6 << 2));
	return r->aon_ahb_apb_ap;
}

static inline void prro_ll_set_aonp_sap_aon_ahb_sresp(uint32_t v) {
	prro_aonp_sap_t *r = (prro_aonp_sap_t*)(SOC_PRRO_REG_BASE + (0x6 << 2));
	r->aon_ahb_sresp = v;
}

static inline uint32_t prro_ll_get_aonp_sap_aon_ahb_sresp(void) {
	prro_aonp_sap_t *r = (prro_aonp_sap_t*)(SOC_PRRO_REG_BASE + (0x6 << 2));
	return r->aon_ahb_sresp;
}

static inline void prro_ll_set_aonp_sap_aon_apb_wdt_nsec(uint32_t v) {
	prro_aonp_sap_t *r = (prro_aonp_sap_t*)(SOC_PRRO_REG_BASE + (0x6 << 2));
	r->aon_apb_wdt_nsec = v;
}

static inline uint32_t prro_ll_get_aonp_sap_aon_apb_wdt_nsec(void) {
	prro_aonp_sap_t *r = (prro_aonp_sap_t*)(SOC_PRRO_REG_BASE + (0x6 << 2));
	return r->aon_apb_wdt_nsec;
}

static inline void prro_ll_set_aonp_sap_aon_apb_timr0_nsec(uint32_t v) {
	prro_aonp_sap_t *r = (prro_aonp_sap_t*)(SOC_PRRO_REG_BASE + (0x6 << 2));
	r->aon_apb_timr0_nsec = v;
}

static inline uint32_t prro_ll_get_aonp_sap_aon_apb_timr0_nsec(void) {
	prro_aonp_sap_t *r = (prro_aonp_sap_t*)(SOC_PRRO_REG_BASE + (0x6 << 2));
	return r->aon_apb_timr0_nsec;
}

static inline void prro_ll_set_aonp_sap_aon_apb_uart0_nsec(uint32_t v) {
	prro_aonp_sap_t *r = (prro_aonp_sap_t*)(SOC_PRRO_REG_BASE + (0x6 << 2));
	r->aon_apb_uart0_nsec = v;
}

static inline uint32_t prro_ll_get_aonp_sap_aon_apb_uart0_nsec(void) {
	prro_aonp_sap_t *r = (prro_aonp_sap_t*)(SOC_PRRO_REG_BASE + (0x6 << 2));
	return r->aon_apb_uart0_nsec;
}

static inline void prro_ll_set_aonp_sap_aon_apb_spi0_nsec(uint32_t v) {
	prro_aonp_sap_t *r = (prro_aonp_sap_t*)(SOC_PRRO_REG_BASE + (0x6 << 2));
	r->aon_apb_spi0_nsec = v;
}

static inline uint32_t prro_ll_get_aonp_sap_aon_apb_spi0_nsec(void) {
	prro_aonp_sap_t *r = (prro_aonp_sap_t*)(SOC_PRRO_REG_BASE + (0x6 << 2));
	return r->aon_apb_spi0_nsec;
}

static inline void prro_ll_set_aonp_sap_aon_apb_efs_nsec(uint32_t v) {
	prro_aonp_sap_t *r = (prro_aonp_sap_t*)(SOC_PRRO_REG_BASE + (0x6 << 2));
	r->aon_apb_efs_nsec = v;
}

static inline uint32_t prro_ll_get_aonp_sap_aon_apb_efs_nsec(void) {
	prro_aonp_sap_t *r = (prro_aonp_sap_t*)(SOC_PRRO_REG_BASE + (0x6 << 2));
	return r->aon_apb_efs_nsec;
}

static inline void prro_ll_set_aonp_sap_aon_apb_mcheck_nsec(uint32_t v) {
	prro_aonp_sap_t *r = (prro_aonp_sap_t*)(SOC_PRRO_REG_BASE + (0x6 << 2));
	r->aon_apb_mcheck_nsec = v;
}

static inline uint32_t prro_ll_get_aonp_sap_aon_apb_mcheck_nsec(void) {
	prro_aonp_sap_t *r = (prro_aonp_sap_t*)(SOC_PRRO_REG_BASE + (0x6 << 2));
	return r->aon_apb_mcheck_nsec;
}

static inline void prro_ll_set_aonp_sap_aon_apb_ckmn_nsec(uint32_t v) {
	prro_aonp_sap_t *r = (prro_aonp_sap_t*)(SOC_PRRO_REG_BASE + (0x6 << 2));
	r->aon_apb_ckmn_nsec = v;
}

static inline uint32_t prro_ll_get_aonp_sap_aon_apb_ckmn_nsec(void) {
	prro_aonp_sap_t *r = (prro_aonp_sap_t*)(SOC_PRRO_REG_BASE + (0x6 << 2));
	return r->aon_apb_ckmn_nsec;
}

static inline void prro_ll_set_aonp_sap_aon_apb_wdt_ap(uint32_t v) {
	prro_aonp_sap_t *r = (prro_aonp_sap_t*)(SOC_PRRO_REG_BASE + (0x6 << 2));
	r->aon_apb_wdt_ap = v;
}

static inline uint32_t prro_ll_get_aonp_sap_aon_apb_wdt_ap(void) {
	prro_aonp_sap_t *r = (prro_aonp_sap_t*)(SOC_PRRO_REG_BASE + (0x6 << 2));
	return r->aon_apb_wdt_ap;
}

static inline void prro_ll_set_aonp_sap_aon_apb_timr0_ap(uint32_t v) {
	prro_aonp_sap_t *r = (prro_aonp_sap_t*)(SOC_PRRO_REG_BASE + (0x6 << 2));
	r->aon_apb_timr0_ap = v;
}

static inline uint32_t prro_ll_get_aonp_sap_aon_apb_timr0_ap(void) {
	prro_aonp_sap_t *r = (prro_aonp_sap_t*)(SOC_PRRO_REG_BASE + (0x6 << 2));
	return r->aon_apb_timr0_ap;
}

static inline void prro_ll_set_aonp_sap_aon_apb_uart0_ap(uint32_t v) {
	prro_aonp_sap_t *r = (prro_aonp_sap_t*)(SOC_PRRO_REG_BASE + (0x6 << 2));
	r->aon_apb_uart0_ap = v;
}

static inline uint32_t prro_ll_get_aonp_sap_aon_apb_uart0_ap(void) {
	prro_aonp_sap_t *r = (prro_aonp_sap_t*)(SOC_PRRO_REG_BASE + (0x6 << 2));
	return r->aon_apb_uart0_ap;
}

static inline void prro_ll_set_aonp_sap_aon_apb_spi0_ap(uint32_t v) {
	prro_aonp_sap_t *r = (prro_aonp_sap_t*)(SOC_PRRO_REG_BASE + (0x6 << 2));
	r->aon_apb_spi0_ap = v;
}

static inline uint32_t prro_ll_get_aonp_sap_aon_apb_spi0_ap(void) {
	prro_aonp_sap_t *r = (prro_aonp_sap_t*)(SOC_PRRO_REG_BASE + (0x6 << 2));
	return r->aon_apb_spi0_ap;
}

static inline void prro_ll_set_aonp_sap_aon_apb_efs_ap(uint32_t v) {
	prro_aonp_sap_t *r = (prro_aonp_sap_t*)(SOC_PRRO_REG_BASE + (0x6 << 2));
	r->aon_apb_efs_ap = v;
}

static inline uint32_t prro_ll_get_aonp_sap_aon_apb_efs_ap(void) {
	prro_aonp_sap_t *r = (prro_aonp_sap_t*)(SOC_PRRO_REG_BASE + (0x6 << 2));
	return r->aon_apb_efs_ap;
}

static inline void prro_ll_set_aonp_sap_aon_apb_mcheck_ap(uint32_t v) {
	prro_aonp_sap_t *r = (prro_aonp_sap_t*)(SOC_PRRO_REG_BASE + (0x6 << 2));
	r->aon_apb_mcheck_ap = v;
}

static inline uint32_t prro_ll_get_aonp_sap_aon_apb_mcheck_ap(void) {
	prro_aonp_sap_t *r = (prro_aonp_sap_t*)(SOC_PRRO_REG_BASE + (0x6 << 2));
	return r->aon_apb_mcheck_ap;
}

static inline void prro_ll_set_aonp_sap_aon_apb_ckmn_ap(uint32_t v) {
	prro_aonp_sap_t *r = (prro_aonp_sap_t*)(SOC_PRRO_REG_BASE + (0x6 << 2));
	r->aon_apb_ckmn_ap = v;
}

static inline uint32_t prro_ll_get_aonp_sap_aon_apb_ckmn_ap(void) {
	prro_aonp_sap_t *r = (prro_aonp_sap_t*)(SOC_PRRO_REG_BASE + (0x6 << 2));
	return r->aon_apb_ckmn_ap;
}

static inline void prro_ll_set_aonp_sap_aon_apb_sresp(uint32_t v) {
	prro_aonp_sap_t *r = (prro_aonp_sap_t*)(SOC_PRRO_REG_BASE + (0x6 << 2));
	r->aon_apb_sresp = v;
}

static inline uint32_t prro_ll_get_aonp_sap_aon_apb_sresp(void) {
	prro_aonp_sap_t *r = (prro_aonp_sap_t*)(SOC_PRRO_REG_BASE + (0x6 << 2));
	return r->aon_apb_sresp;
}

static inline void prro_ll_set_aonp_sap_reserved_bit_28_31(uint32_t v) {
	prro_aonp_sap_t *r = (prro_aonp_sap_t*)(SOC_PRRO_REG_BASE + (0x6 << 2));
	r->reserved_bit_28_31 = v;
}

static inline uint32_t prro_ll_get_aonp_sap_reserved_bit_28_31(void) {
	prro_aonp_sap_t *r = (prro_aonp_sap_t*)(SOC_PRRO_REG_BASE + (0x6 << 2));
	return r->reserved_bit_28_31;
}

//reg bakp_ahb_sap:

static inline void prro_ll_set_bakp_ahb_sap_value(uint32_t v) {
	prro_bakp_ahb_sap_t *r = (prro_bakp_ahb_sap_t*)(SOC_PRRO_REG_BASE + (0x7 << 2));
	r->v = v;
}

static inline uint32_t prro_ll_get_bakp_ahb_sap_value(void) {
	prro_bakp_ahb_sap_t *r = (prro_bakp_ahb_sap_t*)(SOC_PRRO_REG_BASE + (0x7 << 2));
	return r->v;
}

static inline void prro_ll_set_bakp_ahb_sap_bak_ahb_dma0_nsec(uint32_t v) {
	prro_bakp_ahb_sap_t *r = (prro_bakp_ahb_sap_t*)(SOC_PRRO_REG_BASE + (0x7 << 2));
	r->bak_ahb_dma0_nsec = v;
}

static inline uint32_t prro_ll_get_bakp_ahb_sap_bak_ahb_dma0_nsec(void) {
	prro_bakp_ahb_sap_t *r = (prro_bakp_ahb_sap_t*)(SOC_PRRO_REG_BASE + (0x7 << 2));
	return r->bak_ahb_dma0_nsec;
}

static inline void prro_ll_set_bakp_ahb_sap_bak_ahb_dma1_nsec(uint32_t v) {
	prro_bakp_ahb_sap_t *r = (prro_bakp_ahb_sap_t*)(SOC_PRRO_REG_BASE + (0x7 << 2));
	r->bak_ahb_dma1_nsec = v;
}

static inline uint32_t prro_ll_get_bakp_ahb_sap_bak_ahb_dma1_nsec(void) {
	prro_bakp_ahb_sap_t *r = (prro_bakp_ahb_sap_t*)(SOC_PRRO_REG_BASE + (0x7 << 2));
	return r->bak_ahb_dma1_nsec;
}

static inline void prro_ll_set_bakp_ahb_sap_bak_ahb_la_nsec(uint32_t v) {
	prro_bakp_ahb_sap_t *r = (prro_bakp_ahb_sap_t*)(SOC_PRRO_REG_BASE + (0x7 << 2));
	r->bak_ahb_la_nsec = v;
}

static inline uint32_t prro_ll_get_bakp_ahb_sap_bak_ahb_la_nsec(void) {
	prro_bakp_ahb_sap_t *r = (prro_bakp_ahb_sap_t*)(SOC_PRRO_REG_BASE + (0x7 << 2));
	return r->bak_ahb_la_nsec;
}

static inline void prro_ll_set_bakp_ahb_sap_bak_ahb_apb_nsec(uint32_t v) {
	prro_bakp_ahb_sap_t *r = (prro_bakp_ahb_sap_t*)(SOC_PRRO_REG_BASE + (0x7 << 2));
	r->bak_ahb_apb_nsec = v;
}

static inline uint32_t prro_ll_get_bakp_ahb_sap_bak_ahb_apb_nsec(void) {
	prro_bakp_ahb_sap_t *r = (prro_bakp_ahb_sap_t*)(SOC_PRRO_REG_BASE + (0x7 << 2));
	return r->bak_ahb_apb_nsec;
}

static inline void prro_ll_set_bakp_ahb_sap_bak_ahb_dma0_ap(uint32_t v) {
	prro_bakp_ahb_sap_t *r = (prro_bakp_ahb_sap_t*)(SOC_PRRO_REG_BASE + (0x7 << 2));
	r->bak_ahb_dma0_ap = v;
}

static inline uint32_t prro_ll_get_bakp_ahb_sap_bak_ahb_dma0_ap(void) {
	prro_bakp_ahb_sap_t *r = (prro_bakp_ahb_sap_t*)(SOC_PRRO_REG_BASE + (0x7 << 2));
	return r->bak_ahb_dma0_ap;
}

static inline void prro_ll_set_bakp_ahb_sap_bak_ahb_dma1_ap(uint32_t v) {
	prro_bakp_ahb_sap_t *r = (prro_bakp_ahb_sap_t*)(SOC_PRRO_REG_BASE + (0x7 << 2));
	r->bak_ahb_dma1_ap = v;
}

static inline uint32_t prro_ll_get_bakp_ahb_sap_bak_ahb_dma1_ap(void) {
	prro_bakp_ahb_sap_t *r = (prro_bakp_ahb_sap_t*)(SOC_PRRO_REG_BASE + (0x7 << 2));
	return r->bak_ahb_dma1_ap;
}

static inline void prro_ll_set_bakp_ahb_sap_bak_ahb_la_ap(uint32_t v) {
	prro_bakp_ahb_sap_t *r = (prro_bakp_ahb_sap_t*)(SOC_PRRO_REG_BASE + (0x7 << 2));
	r->bak_ahb_la_ap = v;
}

static inline uint32_t prro_ll_get_bakp_ahb_sap_bak_ahb_la_ap(void) {
	prro_bakp_ahb_sap_t *r = (prro_bakp_ahb_sap_t*)(SOC_PRRO_REG_BASE + (0x7 << 2));
	return r->bak_ahb_la_ap;
}

static inline void prro_ll_set_bakp_ahb_sap_bak_ahb_apb_ap(uint32_t v) {
	prro_bakp_ahb_sap_t *r = (prro_bakp_ahb_sap_t*)(SOC_PRRO_REG_BASE + (0x7 << 2));
	r->bak_ahb_apb_ap = v;
}

static inline uint32_t prro_ll_get_bakp_ahb_sap_bak_ahb_apb_ap(void) {
	prro_bakp_ahb_sap_t *r = (prro_bakp_ahb_sap_t*)(SOC_PRRO_REG_BASE + (0x7 << 2));
	return r->bak_ahb_apb_ap;
}

static inline void prro_ll_set_bakp_ahb_sap_bak_ahb_sresp(uint32_t v) {
	prro_bakp_ahb_sap_t *r = (prro_bakp_ahb_sap_t*)(SOC_PRRO_REG_BASE + (0x7 << 2));
	r->bak_ahb_sresp = v;
}

static inline uint32_t prro_ll_get_bakp_ahb_sap_bak_ahb_sresp(void) {
	prro_bakp_ahb_sap_t *r = (prro_bakp_ahb_sap_t*)(SOC_PRRO_REG_BASE + (0x7 << 2));
	return r->bak_ahb_sresp;
}

static inline void prro_ll_set_bakp_ahb_sap_reserved_bit_9_31(uint32_t v) {
	prro_bakp_ahb_sap_t *r = (prro_bakp_ahb_sap_t*)(SOC_PRRO_REG_BASE + (0x7 << 2));
	r->reserved_bit_9_31 = v;
}

static inline uint32_t prro_ll_get_bakp_ahb_sap_reserved_bit_9_31(void) {
	prro_bakp_ahb_sap_t *r = (prro_bakp_ahb_sap_t*)(SOC_PRRO_REG_BASE + (0x7 << 2));
	return r->reserved_bit_9_31;
}

//reg bakp_apb_sap:

static inline void prro_ll_set_bakp_apb_sap_value(uint32_t v) {
	prro_bakp_apb_sap_t *r = (prro_bakp_apb_sap_t*)(SOC_PRRO_REG_BASE + (0x8 << 2));
	r->v = v;
}

static inline uint32_t prro_ll_get_bakp_apb_sap_value(void) {
	prro_bakp_apb_sap_t *r = (prro_bakp_apb_sap_t*)(SOC_PRRO_REG_BASE + (0x8 << 2));
	return r->v;
}

static inline void prro_ll_set_bakp_apb_sap_bak_apb_timer1_nsec(uint32_t v) {
	prro_bakp_apb_sap_t *r = (prro_bakp_apb_sap_t*)(SOC_PRRO_REG_BASE + (0x8 << 2));
	r->bak_apb_timer1_nsec = v;
}

static inline uint32_t prro_ll_get_bakp_apb_sap_bak_apb_timer1_nsec(void) {
	prro_bakp_apb_sap_t *r = (prro_bakp_apb_sap_t*)(SOC_PRRO_REG_BASE + (0x8 << 2));
	return r->bak_apb_timer1_nsec;
}

static inline void prro_ll_set_bakp_apb_sap_bak_apb_sdmadc_nsec(uint32_t v) {
	prro_bakp_apb_sap_t *r = (prro_bakp_apb_sap_t*)(SOC_PRRO_REG_BASE + (0x8 << 2));
	r->bak_apb_sdmadc_nsec = v;
}

static inline uint32_t prro_ll_get_bakp_apb_sap_bak_apb_sdmadc_nsec(void) {
	prro_bakp_apb_sap_t *r = (prro_bakp_apb_sap_t*)(SOC_PRRO_REG_BASE + (0x8 << 2));
	return r->bak_apb_sdmadc_nsec;
}

static inline void prro_ll_set_bakp_apb_sap_bak_apb_uart1_nsec(uint32_t v) {
	prro_bakp_apb_sap_t *r = (prro_bakp_apb_sap_t*)(SOC_PRRO_REG_BASE + (0x8 << 2));
	r->bak_apb_uart1_nsec = v;
}

static inline uint32_t prro_ll_get_bakp_apb_sap_bak_apb_uart1_nsec(void) {
	prro_bakp_apb_sap_t *r = (prro_bakp_apb_sap_t*)(SOC_PRRO_REG_BASE + (0x8 << 2));
	return r->bak_apb_uart1_nsec;
}

static inline void prro_ll_set_bakp_apb_sap_bak_apb_uart2_nsec(uint32_t v) {
	prro_bakp_apb_sap_t *r = (prro_bakp_apb_sap_t*)(SOC_PRRO_REG_BASE + (0x8 << 2));
	r->bak_apb_uart2_nsec = v;
}

static inline uint32_t prro_ll_get_bakp_apb_sap_bak_apb_uart2_nsec(void) {
	prro_bakp_apb_sap_t *r = (prro_bakp_apb_sap_t*)(SOC_PRRO_REG_BASE + (0x8 << 2));
	return r->bak_apb_uart2_nsec;
}

static inline void prro_ll_set_bakp_apb_sap_bak_apb_i2c0_nsec(uint32_t v) {
	prro_bakp_apb_sap_t *r = (prro_bakp_apb_sap_t*)(SOC_PRRO_REG_BASE + (0x8 << 2));
	r->bak_apb_i2c0_nsec = v;
}

static inline uint32_t prro_ll_get_bakp_apb_sap_bak_apb_i2c0_nsec(void) {
	prro_bakp_apb_sap_t *r = (prro_bakp_apb_sap_t*)(SOC_PRRO_REG_BASE + (0x8 << 2));
	return r->bak_apb_i2c0_nsec;
}

static inline void prro_ll_set_bakp_apb_sap_bak_apb_i2c1_nsec(uint32_t v) {
	prro_bakp_apb_sap_t *r = (prro_bakp_apb_sap_t*)(SOC_PRRO_REG_BASE + (0x8 << 2));
	r->bak_apb_i2c1_nsec = v;
}

static inline uint32_t prro_ll_get_bakp_apb_sap_bak_apb_i2c1_nsec(void) {
	prro_bakp_apb_sap_t *r = (prro_bakp_apb_sap_t*)(SOC_PRRO_REG_BASE + (0x8 << 2));
	return r->bak_apb_i2c1_nsec;
}

static inline void prro_ll_set_bakp_apb_sap_bak_apb_spi1_nsec(uint32_t v) {
	prro_bakp_apb_sap_t *r = (prro_bakp_apb_sap_t*)(SOC_PRRO_REG_BASE + (0x8 << 2));
	r->bak_apb_spi1_nsec = v;
}

static inline uint32_t prro_ll_get_bakp_apb_sap_bak_apb_spi1_nsec(void) {
	prro_bakp_apb_sap_t *r = (prro_bakp_apb_sap_t*)(SOC_PRRO_REG_BASE + (0x8 << 2));
	return r->bak_apb_spi1_nsec;
}

static inline void prro_ll_set_bakp_apb_sap_bak_apb_sadc_nsec(uint32_t v) {
	prro_bakp_apb_sap_t *r = (prro_bakp_apb_sap_t*)(SOC_PRRO_REG_BASE + (0x8 << 2));
	r->bak_apb_sadc_nsec = v;
}

static inline uint32_t prro_ll_get_bakp_apb_sap_bak_apb_sadc_nsec(void) {
	prro_bakp_apb_sap_t *r = (prro_bakp_apb_sap_t*)(SOC_PRRO_REG_BASE + (0x8 << 2));
	return r->bak_apb_sadc_nsec;
}

static inline void prro_ll_set_bakp_apb_sap_bak_apb_pwm0_nsec(uint32_t v) {
	prro_bakp_apb_sap_t *r = (prro_bakp_apb_sap_t*)(SOC_PRRO_REG_BASE + (0x8 << 2));
	r->bak_apb_pwm0_nsec = v;
}

static inline uint32_t prro_ll_get_bakp_apb_sap_bak_apb_pwm0_nsec(void) {
	prro_bakp_apb_sap_t *r = (prro_bakp_apb_sap_t*)(SOC_PRRO_REG_BASE + (0x8 << 2));
	return r->bak_apb_pwm0_nsec;
}

static inline void prro_ll_set_bakp_apb_sap_bak_apb_irda_nsec(uint32_t v) {
	prro_bakp_apb_sap_t *r = (prro_bakp_apb_sap_t*)(SOC_PRRO_REG_BASE + (0x8 << 2));
	r->bak_apb_irda_nsec = v;
}

static inline uint32_t prro_ll_get_bakp_apb_sap_bak_apb_irda_nsec(void) {
	prro_bakp_apb_sap_t *r = (prro_bakp_apb_sap_t*)(SOC_PRRO_REG_BASE + (0x8 << 2));
	return r->bak_apb_irda_nsec;
}

static inline void prro_ll_set_bakp_apb_sap_bak_apb_trng_nsec(uint32_t v) {
	prro_bakp_apb_sap_t *r = (prro_bakp_apb_sap_t*)(SOC_PRRO_REG_BASE + (0x8 << 2));
	r->bak_apb_trng_nsec = v;
}

static inline uint32_t prro_ll_get_bakp_apb_sap_bak_apb_trng_nsec(void) {
	prro_bakp_apb_sap_t *r = (prro_bakp_apb_sap_t*)(SOC_PRRO_REG_BASE + (0x8 << 2));
	return r->bak_apb_trng_nsec;
}

static inline void prro_ll_set_bakp_apb_sap_bak_apb_sdio_nsec(uint32_t v) {
	prro_bakp_apb_sap_t *r = (prro_bakp_apb_sap_t*)(SOC_PRRO_REG_BASE + (0x8 << 2));
	r->bak_apb_sdio_nsec = v;
}

static inline uint32_t prro_ll_get_bakp_apb_sap_bak_apb_sdio_nsec(void) {
	prro_bakp_apb_sap_t *r = (prro_bakp_apb_sap_t*)(SOC_PRRO_REG_BASE + (0x8 << 2));
	return r->bak_apb_sdio_nsec;
}

static inline void prro_ll_set_bakp_apb_sap_bak_apb_scld_nsec(uint32_t v) {
	prro_bakp_apb_sap_t *r = (prro_bakp_apb_sap_t*)(SOC_PRRO_REG_BASE + (0x8 << 2));
	r->bak_apb_scld_nsec = v;
}

static inline uint32_t prro_ll_get_bakp_apb_sap_bak_apb_scld_nsec(void) {
	prro_bakp_apb_sap_t *r = (prro_bakp_apb_sap_t*)(SOC_PRRO_REG_BASE + (0x8 << 2));
	return r->bak_apb_scld_nsec;
}

static inline void prro_ll_set_bakp_apb_sap_bak_apb_pwm1_nsec(uint32_t v) {
	prro_bakp_apb_sap_t *r = (prro_bakp_apb_sap_t*)(SOC_PRRO_REG_BASE + (0x8 << 2));
	r->bak_apb_pwm1_nsec = v;
}

static inline uint32_t prro_ll_get_bakp_apb_sap_bak_apb_pwm1_nsec(void) {
	prro_bakp_apb_sap_t *r = (prro_bakp_apb_sap_t*)(SOC_PRRO_REG_BASE + (0x8 << 2));
	return r->bak_apb_pwm1_nsec;
}

static inline void prro_ll_set_bakp_apb_sap_bak_apb_timer1_ap(uint32_t v) {
	prro_bakp_apb_sap_t *r = (prro_bakp_apb_sap_t*)(SOC_PRRO_REG_BASE + (0x8 << 2));
	r->bak_apb_timer1_ap = v;
}

static inline uint32_t prro_ll_get_bakp_apb_sap_bak_apb_timer1_ap(void) {
	prro_bakp_apb_sap_t *r = (prro_bakp_apb_sap_t*)(SOC_PRRO_REG_BASE + (0x8 << 2));
	return r->bak_apb_timer1_ap;
}

static inline void prro_ll_set_bakp_apb_sap_bak_apb_sdmadc_ap(uint32_t v) {
	prro_bakp_apb_sap_t *r = (prro_bakp_apb_sap_t*)(SOC_PRRO_REG_BASE + (0x8 << 2));
	r->bak_apb_sdmadc_ap = v;
}

static inline uint32_t prro_ll_get_bakp_apb_sap_bak_apb_sdmadc_ap(void) {
	prro_bakp_apb_sap_t *r = (prro_bakp_apb_sap_t*)(SOC_PRRO_REG_BASE + (0x8 << 2));
	return r->bak_apb_sdmadc_ap;
}

static inline void prro_ll_set_bakp_apb_sap_bak_apb_uart1_ap(uint32_t v) {
	prro_bakp_apb_sap_t *r = (prro_bakp_apb_sap_t*)(SOC_PRRO_REG_BASE + (0x8 << 2));
	r->bak_apb_uart1_ap = v;
}

static inline uint32_t prro_ll_get_bakp_apb_sap_bak_apb_uart1_ap(void) {
	prro_bakp_apb_sap_t *r = (prro_bakp_apb_sap_t*)(SOC_PRRO_REG_BASE + (0x8 << 2));
	return r->bak_apb_uart1_ap;
}

static inline void prro_ll_set_bakp_apb_sap_bak_apb_uart2_ap(uint32_t v) {
	prro_bakp_apb_sap_t *r = (prro_bakp_apb_sap_t*)(SOC_PRRO_REG_BASE + (0x8 << 2));
	r->bak_apb_uart2_ap = v;
}

static inline uint32_t prro_ll_get_bakp_apb_sap_bak_apb_uart2_ap(void) {
	prro_bakp_apb_sap_t *r = (prro_bakp_apb_sap_t*)(SOC_PRRO_REG_BASE + (0x8 << 2));
	return r->bak_apb_uart2_ap;
}

static inline void prro_ll_set_bakp_apb_sap_bak_apb_i2c0_ap(uint32_t v) {
	prro_bakp_apb_sap_t *r = (prro_bakp_apb_sap_t*)(SOC_PRRO_REG_BASE + (0x8 << 2));
	r->bak_apb_i2c0_ap = v;
}

static inline uint32_t prro_ll_get_bakp_apb_sap_bak_apb_i2c0_ap(void) {
	prro_bakp_apb_sap_t *r = (prro_bakp_apb_sap_t*)(SOC_PRRO_REG_BASE + (0x8 << 2));
	return r->bak_apb_i2c0_ap;
}

static inline void prro_ll_set_bakp_apb_sap_bak_apb_i2c1_ap(uint32_t v) {
	prro_bakp_apb_sap_t *r = (prro_bakp_apb_sap_t*)(SOC_PRRO_REG_BASE + (0x8 << 2));
	r->bak_apb_i2c1_ap = v;
}

static inline uint32_t prro_ll_get_bakp_apb_sap_bak_apb_i2c1_ap(void) {
	prro_bakp_apb_sap_t *r = (prro_bakp_apb_sap_t*)(SOC_PRRO_REG_BASE + (0x8 << 2));
	return r->bak_apb_i2c1_ap;
}

static inline void prro_ll_set_bakp_apb_sap_bak_apb_spi1_ap(uint32_t v) {
	prro_bakp_apb_sap_t *r = (prro_bakp_apb_sap_t*)(SOC_PRRO_REG_BASE + (0x8 << 2));
	r->bak_apb_spi1_ap = v;
}

static inline uint32_t prro_ll_get_bakp_apb_sap_bak_apb_spi1_ap(void) {
	prro_bakp_apb_sap_t *r = (prro_bakp_apb_sap_t*)(SOC_PRRO_REG_BASE + (0x8 << 2));
	return r->bak_apb_spi1_ap;
}

static inline void prro_ll_set_bakp_apb_sap_bak_apb_sadc_ap(uint32_t v) {
	prro_bakp_apb_sap_t *r = (prro_bakp_apb_sap_t*)(SOC_PRRO_REG_BASE + (0x8 << 2));
	r->bak_apb_sadc_ap = v;
}

static inline uint32_t prro_ll_get_bakp_apb_sap_bak_apb_sadc_ap(void) {
	prro_bakp_apb_sap_t *r = (prro_bakp_apb_sap_t*)(SOC_PRRO_REG_BASE + (0x8 << 2));
	return r->bak_apb_sadc_ap;
}

static inline void prro_ll_set_bakp_apb_sap_bak_apb_pwm0_ap(uint32_t v) {
	prro_bakp_apb_sap_t *r = (prro_bakp_apb_sap_t*)(SOC_PRRO_REG_BASE + (0x8 << 2));
	r->bak_apb_pwm0_ap = v;
}

static inline uint32_t prro_ll_get_bakp_apb_sap_bak_apb_pwm0_ap(void) {
	prro_bakp_apb_sap_t *r = (prro_bakp_apb_sap_t*)(SOC_PRRO_REG_BASE + (0x8 << 2));
	return r->bak_apb_pwm0_ap;
}

static inline void prro_ll_set_bakp_apb_sap_bak_apb_irda_ap(uint32_t v) {
	prro_bakp_apb_sap_t *r = (prro_bakp_apb_sap_t*)(SOC_PRRO_REG_BASE + (0x8 << 2));
	r->bak_apb_irda_ap = v;
}

static inline uint32_t prro_ll_get_bakp_apb_sap_bak_apb_irda_ap(void) {
	prro_bakp_apb_sap_t *r = (prro_bakp_apb_sap_t*)(SOC_PRRO_REG_BASE + (0x8 << 2));
	return r->bak_apb_irda_ap;
}

static inline void prro_ll_set_bakp_apb_sap_bak_apb_trng_ap(uint32_t v) {
	prro_bakp_apb_sap_t *r = (prro_bakp_apb_sap_t*)(SOC_PRRO_REG_BASE + (0x8 << 2));
	r->bak_apb_trng_ap = v;
}

static inline uint32_t prro_ll_get_bakp_apb_sap_bak_apb_trng_ap(void) {
	prro_bakp_apb_sap_t *r = (prro_bakp_apb_sap_t*)(SOC_PRRO_REG_BASE + (0x8 << 2));
	return r->bak_apb_trng_ap;
}

static inline void prro_ll_set_bakp_apb_sap_bak_apb_sdio_ap(uint32_t v) {
	prro_bakp_apb_sap_t *r = (prro_bakp_apb_sap_t*)(SOC_PRRO_REG_BASE + (0x8 << 2));
	r->bak_apb_sdio_ap = v;
}

static inline uint32_t prro_ll_get_bakp_apb_sap_bak_apb_sdio_ap(void) {
	prro_bakp_apb_sap_t *r = (prro_bakp_apb_sap_t*)(SOC_PRRO_REG_BASE + (0x8 << 2));
	return r->bak_apb_sdio_ap;
}

static inline void prro_ll_set_bakp_apb_sap_bak_apb_scld_ap(uint32_t v) {
	prro_bakp_apb_sap_t *r = (prro_bakp_apb_sap_t*)(SOC_PRRO_REG_BASE + (0x8 << 2));
	r->bak_apb_scld_ap = v;
}

static inline uint32_t prro_ll_get_bakp_apb_sap_bak_apb_scld_ap(void) {
	prro_bakp_apb_sap_t *r = (prro_bakp_apb_sap_t*)(SOC_PRRO_REG_BASE + (0x8 << 2));
	return r->bak_apb_scld_ap;
}

static inline void prro_ll_set_bakp_apb_sap_bak_apb_pwm1_ap(uint32_t v) {
	prro_bakp_apb_sap_t *r = (prro_bakp_apb_sap_t*)(SOC_PRRO_REG_BASE + (0x8 << 2));
	r->bak_apb_pwm1_ap = v;
}

static inline uint32_t prro_ll_get_bakp_apb_sap_bak_apb_pwm1_ap(void) {
	prro_bakp_apb_sap_t *r = (prro_bakp_apb_sap_t*)(SOC_PRRO_REG_BASE + (0x8 << 2));
	return r->bak_apb_pwm1_ap;
}

static inline void prro_ll_set_bakp_apb_sap_bak_apb_sresp(uint32_t v) {
	prro_bakp_apb_sap_t *r = (prro_bakp_apb_sap_t*)(SOC_PRRO_REG_BASE + (0x8 << 2));
	r->bak_apb_sresp = v;
}

static inline uint32_t prro_ll_get_bakp_apb_sap_bak_apb_sresp(void) {
	prro_bakp_apb_sap_t *r = (prro_bakp_apb_sap_t*)(SOC_PRRO_REG_BASE + (0x8 << 2));
	return r->bak_apb_sresp;
}

static inline void prro_ll_set_bakp_apb_sap_reserved_bit_29_31(uint32_t v) {
	prro_bakp_apb_sap_t *r = (prro_bakp_apb_sap_t*)(SOC_PRRO_REG_BASE + (0x8 << 2));
	r->reserved_bit_29_31 = v;
}

static inline uint32_t prro_ll_get_bakp_apb_sap_reserved_bit_29_31(void) {
	prro_bakp_apb_sap_t *r = (prro_bakp_apb_sap_t*)(SOC_PRRO_REG_BASE + (0x8 << 2));
	return r->reserved_bit_29_31;
}

//reg ahbp_sap:

static inline void prro_ll_set_ahbp_sap_value(uint32_t v) {
	prro_ahbp_sap_t *r = (prro_ahbp_sap_t*)(SOC_PRRO_REG_BASE + (0x9 << 2));
	r->v = v;
}

static inline uint32_t prro_ll_get_ahbp_sap_value(void) {
	prro_ahbp_sap_t *r = (prro_ahbp_sap_t*)(SOC_PRRO_REG_BASE + (0x9 << 2));
	return r->v;
}

static inline void prro_ll_set_ahbp_sap_ahb_ahb_usb_nsec(uint32_t v) {
	prro_ahbp_sap_t *r = (prro_ahbp_sap_t*)(SOC_PRRO_REG_BASE + (0x9 << 2));
	r->ahb_ahb_usb_nsec = v;
}

static inline uint32_t prro_ll_get_ahbp_sap_ahb_ahb_usb_nsec(void) {
	prro_ahbp_sap_t *r = (prro_ahbp_sap_t*)(SOC_PRRO_REG_BASE + (0x9 << 2));
	return r->ahb_ahb_usb_nsec;
}

static inline void prro_ll_set_ahbp_sap_ahb_ahb_can_nsec(uint32_t v) {
	prro_ahbp_sap_t *r = (prro_ahbp_sap_t*)(SOC_PRRO_REG_BASE + (0x9 << 2));
	r->ahb_ahb_can_nsec = v;
}

static inline uint32_t prro_ll_get_ahbp_sap_ahb_ahb_can_nsec(void) {
	prro_ahbp_sap_t *r = (prro_ahbp_sap_t*)(SOC_PRRO_REG_BASE + (0x9 << 2));
	return r->ahb_ahb_can_nsec;
}

static inline void prro_ll_set_ahbp_sap_ahb_ahb_qspi0_nsec(uint32_t v) {
	prro_ahbp_sap_t *r = (prro_ahbp_sap_t*)(SOC_PRRO_REG_BASE + (0x9 << 2));
	r->ahb_ahb_qspi0_nsec = v;
}

static inline uint32_t prro_ll_get_ahbp_sap_ahb_ahb_qspi0_nsec(void) {
	prro_ahbp_sap_t *r = (prro_ahbp_sap_t*)(SOC_PRRO_REG_BASE + (0x9 << 2));
	return r->ahb_ahb_qspi0_nsec;
}

static inline void prro_ll_set_ahbp_sap_ahb_ahb_qspi1_nsec(uint32_t v) {
	prro_ahbp_sap_t *r = (prro_ahbp_sap_t*)(SOC_PRRO_REG_BASE + (0x9 << 2));
	r->ahb_ahb_qspi1_nsec = v;
}

static inline uint32_t prro_ll_get_ahbp_sap_ahb_ahb_qspi1_nsec(void) {
	prro_ahbp_sap_t *r = (prro_ahbp_sap_t*)(SOC_PRRO_REG_BASE + (0x9 << 2));
	return r->ahb_ahb_qspi1_nsec;
}

static inline void prro_ll_set_ahbp_sap_ahb_ahb_psram_nsec(uint32_t v) {
	prro_ahbp_sap_t *r = (prro_ahbp_sap_t*)(SOC_PRRO_REG_BASE + (0x9 << 2));
	r->ahb_ahb_psram_nsec = v;
}

static inline uint32_t prro_ll_get_ahbp_sap_ahb_ahb_psram_nsec(void) {
	prro_ahbp_sap_t *r = (prro_ahbp_sap_t*)(SOC_PRRO_REG_BASE + (0x9 << 2));
	return r->ahb_ahb_psram_nsec;
}

static inline void prro_ll_set_ahbp_sap_ahb_ahb_enet_nsec(uint32_t v) {
	prro_ahbp_sap_t *r = (prro_ahbp_sap_t*)(SOC_PRRO_REG_BASE + (0x9 << 2));
	r->ahb_ahb_enet_nsec = v;
}

static inline uint32_t prro_ll_get_ahbp_sap_ahb_ahb_enet_nsec(void) {
	prro_ahbp_sap_t *r = (prro_ahbp_sap_t*)(SOC_PRRO_REG_BASE + (0x9 << 2));
	return r->ahb_ahb_enet_nsec;
}

static inline void prro_ll_set_ahbp_sap_ahb_ahb_apb_nsec(uint32_t v) {
	prro_ahbp_sap_t *r = (prro_ahbp_sap_t*)(SOC_PRRO_REG_BASE + (0x9 << 2));
	r->ahb_ahb_apb_nsec = v;
}

static inline uint32_t prro_ll_get_ahbp_sap_ahb_ahb_apb_nsec(void) {
	prro_ahbp_sap_t *r = (prro_ahbp_sap_t*)(SOC_PRRO_REG_BASE + (0x9 << 2));
	return r->ahb_ahb_apb_nsec;
}

static inline void prro_ll_set_ahbp_sap_ahb_ahb_usb_ap(uint32_t v) {
	prro_ahbp_sap_t *r = (prro_ahbp_sap_t*)(SOC_PRRO_REG_BASE + (0x9 << 2));
	r->ahb_ahb_usb_ap = v;
}

static inline uint32_t prro_ll_get_ahbp_sap_ahb_ahb_usb_ap(void) {
	prro_ahbp_sap_t *r = (prro_ahbp_sap_t*)(SOC_PRRO_REG_BASE + (0x9 << 2));
	return r->ahb_ahb_usb_ap;
}

static inline void prro_ll_set_ahbp_sap_ahb_ahb_can_ap(uint32_t v) {
	prro_ahbp_sap_t *r = (prro_ahbp_sap_t*)(SOC_PRRO_REG_BASE + (0x9 << 2));
	r->ahb_ahb_can_ap = v;
}

static inline uint32_t prro_ll_get_ahbp_sap_ahb_ahb_can_ap(void) {
	prro_ahbp_sap_t *r = (prro_ahbp_sap_t*)(SOC_PRRO_REG_BASE + (0x9 << 2));
	return r->ahb_ahb_can_ap;
}

static inline void prro_ll_set_ahbp_sap_ahb_ahb_qspi0_ap(uint32_t v) {
	prro_ahbp_sap_t *r = (prro_ahbp_sap_t*)(SOC_PRRO_REG_BASE + (0x9 << 2));
	r->ahb_ahb_qspi0_ap = v;
}

static inline uint32_t prro_ll_get_ahbp_sap_ahb_ahb_qspi0_ap(void) {
	prro_ahbp_sap_t *r = (prro_ahbp_sap_t*)(SOC_PRRO_REG_BASE + (0x9 << 2));
	return r->ahb_ahb_qspi0_ap;
}

static inline void prro_ll_set_ahbp_sap_ahb_ahb_qspi1_ap(uint32_t v) {
	prro_ahbp_sap_t *r = (prro_ahbp_sap_t*)(SOC_PRRO_REG_BASE + (0x9 << 2));
	r->ahb_ahb_qspi1_ap = v;
}

static inline uint32_t prro_ll_get_ahbp_sap_ahb_ahb_qspi1_ap(void) {
	prro_ahbp_sap_t *r = (prro_ahbp_sap_t*)(SOC_PRRO_REG_BASE + (0x9 << 2));
	return r->ahb_ahb_qspi1_ap;
}

static inline void prro_ll_set_ahbp_sap_ahb_ahb_psram_ap(uint32_t v) {
	prro_ahbp_sap_t *r = (prro_ahbp_sap_t*)(SOC_PRRO_REG_BASE + (0x9 << 2));
	r->ahb_ahb_psram_ap = v;
}

static inline uint32_t prro_ll_get_ahbp_sap_ahb_ahb_psram_ap(void) {
	prro_ahbp_sap_t *r = (prro_ahbp_sap_t*)(SOC_PRRO_REG_BASE + (0x9 << 2));
	return r->ahb_ahb_psram_ap;
}

static inline void prro_ll_set_ahbp_sap_ahb_ahb_enet_ap(uint32_t v) {
	prro_ahbp_sap_t *r = (prro_ahbp_sap_t*)(SOC_PRRO_REG_BASE + (0x9 << 2));
	r->ahb_ahb_enet_ap = v;
}

static inline uint32_t prro_ll_get_ahbp_sap_ahb_ahb_enet_ap(void) {
	prro_ahbp_sap_t *r = (prro_ahbp_sap_t*)(SOC_PRRO_REG_BASE + (0x9 << 2));
	return r->ahb_ahb_enet_ap;
}

static inline void prro_ll_set_ahbp_sap_ahb_ahb_apb_ap(uint32_t v) {
	prro_ahbp_sap_t *r = (prro_ahbp_sap_t*)(SOC_PRRO_REG_BASE + (0x9 << 2));
	r->ahb_ahb_apb_ap = v;
}

static inline uint32_t prro_ll_get_ahbp_sap_ahb_ahb_apb_ap(void) {
	prro_ahbp_sap_t *r = (prro_ahbp_sap_t*)(SOC_PRRO_REG_BASE + (0x9 << 2));
	return r->ahb_ahb_apb_ap;
}

static inline void prro_ll_set_ahbp_sap_ahb_ahb_sresp(uint32_t v) {
	prro_ahbp_sap_t *r = (prro_ahbp_sap_t*)(SOC_PRRO_REG_BASE + (0x9 << 2));
	r->ahb_ahb_sresp = v;
}

static inline uint32_t prro_ll_get_ahbp_sap_ahb_ahb_sresp(void) {
	prro_ahbp_sap_t *r = (prro_ahbp_sap_t*)(SOC_PRRO_REG_BASE + (0x9 << 2));
	return r->ahb_ahb_sresp;
}

static inline void prro_ll_set_ahbp_sap_ahb_apb_scr_nsec(uint32_t v) {
	prro_ahbp_sap_t *r = (prro_ahbp_sap_t*)(SOC_PRRO_REG_BASE + (0x9 << 2));
	r->ahb_apb_scr_nsec = v;
}

static inline uint32_t prro_ll_get_ahbp_sap_ahb_apb_scr_nsec(void) {
	prro_ahbp_sap_t *r = (prro_ahbp_sap_t*)(SOC_PRRO_REG_BASE + (0x9 << 2));
	return r->ahb_apb_scr_nsec;
}

static inline void prro_ll_set_ahbp_sap_ahb_apb_lin_nsec(uint32_t v) {
	prro_ahbp_sap_t *r = (prro_ahbp_sap_t*)(SOC_PRRO_REG_BASE + (0x9 << 2));
	r->ahb_apb_lin_nsec = v;
}

static inline uint32_t prro_ll_get_ahbp_sap_ahb_apb_lin_nsec(void) {
	prro_ahbp_sap_t *r = (prro_ahbp_sap_t*)(SOC_PRRO_REG_BASE + (0x9 << 2));
	return r->ahb_apb_lin_nsec;
}

static inline void prro_ll_set_ahbp_sap_ahb_apb_scr_ap(uint32_t v) {
	prro_ahbp_sap_t *r = (prro_ahbp_sap_t*)(SOC_PRRO_REG_BASE + (0x9 << 2));
	r->ahb_apb_scr_ap = v;
}

static inline uint32_t prro_ll_get_ahbp_sap_ahb_apb_scr_ap(void) {
	prro_ahbp_sap_t *r = (prro_ahbp_sap_t*)(SOC_PRRO_REG_BASE + (0x9 << 2));
	return r->ahb_apb_scr_ap;
}

static inline void prro_ll_set_ahbp_sap_ahb_apb_lin_ap(uint32_t v) {
	prro_ahbp_sap_t *r = (prro_ahbp_sap_t*)(SOC_PRRO_REG_BASE + (0x9 << 2));
	r->ahb_apb_lin_ap = v;
}

static inline uint32_t prro_ll_get_ahbp_sap_ahb_apb_lin_ap(void) {
	prro_ahbp_sap_t *r = (prro_ahbp_sap_t*)(SOC_PRRO_REG_BASE + (0x9 << 2));
	return r->ahb_apb_lin_ap;
}

static inline void prro_ll_set_ahbp_sap_ahb_apb_sresp(uint32_t v) {
	prro_ahbp_sap_t *r = (prro_ahbp_sap_t*)(SOC_PRRO_REG_BASE + (0x9 << 2));
	r->ahb_apb_sresp = v;
}

static inline uint32_t prro_ll_get_ahbp_sap_ahb_apb_sresp(void) {
	prro_ahbp_sap_t *r = (prro_ahbp_sap_t*)(SOC_PRRO_REG_BASE + (0x9 << 2));
	return r->ahb_apb_sresp;
}

static inline void prro_ll_set_ahbp_sap_reserved_bit_20_31(uint32_t v) {
	prro_ahbp_sap_t *r = (prro_ahbp_sap_t*)(SOC_PRRO_REG_BASE + (0x9 << 2));
	r->reserved_bit_20_31 = v;
}

static inline uint32_t prro_ll_get_ahbp_sap_reserved_bit_20_31(void) {
	prro_ahbp_sap_t *r = (prro_ahbp_sap_t*)(SOC_PRRO_REG_BASE + (0x9 << 2));
	return r->reserved_bit_20_31;
}

//reg audp_sap:

static inline void prro_ll_set_audp_sap_value(uint32_t v) {
	prro_audp_sap_t *r = (prro_audp_sap_t*)(SOC_PRRO_REG_BASE + (0xa << 2));
	r->v = v;
}

static inline uint32_t prro_ll_get_audp_sap_value(void) {
	prro_audp_sap_t *r = (prro_audp_sap_t*)(SOC_PRRO_REG_BASE + (0xa << 2));
	return r->v;
}

static inline void prro_ll_set_audp_sap_aud_ahb_sbc_nsec(uint32_t v) {
	prro_audp_sap_t *r = (prro_audp_sap_t*)(SOC_PRRO_REG_BASE + (0xa << 2));
	r->aud_ahb_sbc_nsec = v;
}

static inline uint32_t prro_ll_get_audp_sap_aud_ahb_sbc_nsec(void) {
	prro_audp_sap_t *r = (prro_audp_sap_t*)(SOC_PRRO_REG_BASE + (0xa << 2));
	return r->aud_ahb_sbc_nsec;
}

static inline void prro_ll_set_audp_sap_aud_ahb_apb_nsec(uint32_t v) {
	prro_audp_sap_t *r = (prro_audp_sap_t*)(SOC_PRRO_REG_BASE + (0xa << 2));
	r->aud_ahb_apb_nsec = v;
}

static inline uint32_t prro_ll_get_audp_sap_aud_ahb_apb_nsec(void) {
	prro_audp_sap_t *r = (prro_audp_sap_t*)(SOC_PRRO_REG_BASE + (0xa << 2));
	return r->aud_ahb_apb_nsec;
}

static inline void prro_ll_set_audp_sap_aud_ahb_sbc_ap(uint32_t v) {
	prro_audp_sap_t *r = (prro_audp_sap_t*)(SOC_PRRO_REG_BASE + (0xa << 2));
	r->aud_ahb_sbc_ap = v;
}

static inline uint32_t prro_ll_get_audp_sap_aud_ahb_sbc_ap(void) {
	prro_audp_sap_t *r = (prro_audp_sap_t*)(SOC_PRRO_REG_BASE + (0xa << 2));
	return r->aud_ahb_sbc_ap;
}

static inline void prro_ll_set_audp_sap_aud_ahb_apb_ap(uint32_t v) {
	prro_audp_sap_t *r = (prro_audp_sap_t*)(SOC_PRRO_REG_BASE + (0xa << 2));
	r->aud_ahb_apb_ap = v;
}

static inline uint32_t prro_ll_get_audp_sap_aud_ahb_apb_ap(void) {
	prro_audp_sap_t *r = (prro_audp_sap_t*)(SOC_PRRO_REG_BASE + (0xa << 2));
	return r->aud_ahb_apb_ap;
}

static inline void prro_ll_set_audp_sap_aud_ahb_sresp(uint32_t v) {
	prro_audp_sap_t *r = (prro_audp_sap_t*)(SOC_PRRO_REG_BASE + (0xa << 2));
	r->aud_ahb_sresp = v;
}

static inline uint32_t prro_ll_get_audp_sap_aud_ahb_sresp(void) {
	prro_audp_sap_t *r = (prro_audp_sap_t*)(SOC_PRRO_REG_BASE + (0xa << 2));
	return r->aud_ahb_sresp;
}

static inline void prro_ll_set_audp_sap_aud_apb_aud_nses(uint32_t v) {
	prro_audp_sap_t *r = (prro_audp_sap_t*)(SOC_PRRO_REG_BASE + (0xa << 2));
	r->aud_apb_aud_nses = v;
}

static inline uint32_t prro_ll_get_audp_sap_aud_apb_aud_nses(void) {
	prro_audp_sap_t *r = (prro_audp_sap_t*)(SOC_PRRO_REG_BASE + (0xa << 2));
	return r->aud_apb_aud_nses;
}

static inline void prro_ll_set_audp_sap_aud_apb_i2s0_nses(uint32_t v) {
	prro_audp_sap_t *r = (prro_audp_sap_t*)(SOC_PRRO_REG_BASE + (0xa << 2));
	r->aud_apb_i2s0_nses = v;
}

static inline uint32_t prro_ll_get_audp_sap_aud_apb_i2s0_nses(void) {
	prro_audp_sap_t *r = (prro_audp_sap_t*)(SOC_PRRO_REG_BASE + (0xa << 2));
	return r->aud_apb_i2s0_nses;
}

static inline void prro_ll_set_audp_sap_aud_apb_i2s1_nses(uint32_t v) {
	prro_audp_sap_t *r = (prro_audp_sap_t*)(SOC_PRRO_REG_BASE + (0xa << 2));
	r->aud_apb_i2s1_nses = v;
}

static inline uint32_t prro_ll_get_audp_sap_aud_apb_i2s1_nses(void) {
	prro_audp_sap_t *r = (prro_audp_sap_t*)(SOC_PRRO_REG_BASE + (0xa << 2));
	return r->aud_apb_i2s1_nses;
}

static inline void prro_ll_set_audp_sap_aud_apb_i2s2_nses(uint32_t v) {
	prro_audp_sap_t *r = (prro_audp_sap_t*)(SOC_PRRO_REG_BASE + (0xa << 2));
	r->aud_apb_i2s2_nses = v;
}

static inline uint32_t prro_ll_get_audp_sap_aud_apb_i2s2_nses(void) {
	prro_audp_sap_t *r = (prro_audp_sap_t*)(SOC_PRRO_REG_BASE + (0xa << 2));
	return r->aud_apb_i2s2_nses;
}

static inline void prro_ll_set_audp_sap_aud_apb_aud_ap(uint32_t v) {
	prro_audp_sap_t *r = (prro_audp_sap_t*)(SOC_PRRO_REG_BASE + (0xa << 2));
	r->aud_apb_aud_ap = v;
}

static inline uint32_t prro_ll_get_audp_sap_aud_apb_aud_ap(void) {
	prro_audp_sap_t *r = (prro_audp_sap_t*)(SOC_PRRO_REG_BASE + (0xa << 2));
	return r->aud_apb_aud_ap;
}

static inline void prro_ll_set_audp_sap_aud_apb_i2s0_ap(uint32_t v) {
	prro_audp_sap_t *r = (prro_audp_sap_t*)(SOC_PRRO_REG_BASE + (0xa << 2));
	r->aud_apb_i2s0_ap = v;
}

static inline uint32_t prro_ll_get_audp_sap_aud_apb_i2s0_ap(void) {
	prro_audp_sap_t *r = (prro_audp_sap_t*)(SOC_PRRO_REG_BASE + (0xa << 2));
	return r->aud_apb_i2s0_ap;
}

static inline void prro_ll_set_audp_sap_aud_apb_i2s1_ap(uint32_t v) {
	prro_audp_sap_t *r = (prro_audp_sap_t*)(SOC_PRRO_REG_BASE + (0xa << 2));
	r->aud_apb_i2s1_ap = v;
}

static inline uint32_t prro_ll_get_audp_sap_aud_apb_i2s1_ap(void) {
	prro_audp_sap_t *r = (prro_audp_sap_t*)(SOC_PRRO_REG_BASE + (0xa << 2));
	return r->aud_apb_i2s1_ap;
}

static inline void prro_ll_set_audp_sap_aud_apb_i2s2_ap(uint32_t v) {
	prro_audp_sap_t *r = (prro_audp_sap_t*)(SOC_PRRO_REG_BASE + (0xa << 2));
	r->aud_apb_i2s2_ap = v;
}

static inline uint32_t prro_ll_get_audp_sap_aud_apb_i2s2_ap(void) {
	prro_audp_sap_t *r = (prro_audp_sap_t*)(SOC_PRRO_REG_BASE + (0xa << 2));
	return r->aud_apb_i2s2_ap;
}

static inline void prro_ll_set_audp_sap_aud_apb_sresp(uint32_t v) {
	prro_audp_sap_t *r = (prro_audp_sap_t*)(SOC_PRRO_REG_BASE + (0xa << 2));
	r->aud_apb_sresp = v;
}

static inline uint32_t prro_ll_get_audp_sap_aud_apb_sresp(void) {
	prro_audp_sap_t *r = (prro_audp_sap_t*)(SOC_PRRO_REG_BASE + (0xa << 2));
	return r->aud_apb_sresp;
}

static inline void prro_ll_set_audp_sap_reserved_bit_14_31(uint32_t v) {
	prro_audp_sap_t *r = (prro_audp_sap_t*)(SOC_PRRO_REG_BASE + (0xa << 2));
	r->reserved_bit_14_31 = v;
}

static inline uint32_t prro_ll_get_audp_sap_reserved_bit_14_31(void) {
	prro_audp_sap_t *r = (prro_audp_sap_t*)(SOC_PRRO_REG_BASE + (0xa << 2));
	return r->reserved_bit_14_31;
}

//reg vidp_sap:

static inline void prro_ll_set_vidp_sap_value(uint32_t v) {
	prro_vidp_sap_t *r = (prro_vidp_sap_t*)(SOC_PRRO_REG_BASE + (0xb << 2));
	r->v = v;
}

static inline uint32_t prro_ll_get_vidp_sap_value(void) {
	prro_vidp_sap_t *r = (prro_vidp_sap_t*)(SOC_PRRO_REG_BASE + (0xb << 2));
	return r->v;
}

static inline void prro_ll_set_vidp_sap_vid_ahb_jpge_nsec(uint32_t v) {
	prro_vidp_sap_t *r = (prro_vidp_sap_t*)(SOC_PRRO_REG_BASE + (0xb << 2));
	r->vid_ahb_jpge_nsec = v;
}

static inline uint32_t prro_ll_get_vidp_sap_vid_ahb_jpge_nsec(void) {
	prro_vidp_sap_t *r = (prro_vidp_sap_t*)(SOC_PRRO_REG_BASE + (0xb << 2));
	return r->vid_ahb_jpge_nsec;
}

static inline void prro_ll_set_vidp_sap_vid_ahb_yuv_nsec(uint32_t v) {
	prro_vidp_sap_t *r = (prro_vidp_sap_t*)(SOC_PRRO_REG_BASE + (0xb << 2));
	r->vid_ahb_yuv_nsec = v;
}

static inline uint32_t prro_ll_get_vidp_sap_vid_ahb_yuv_nsec(void) {
	prro_vidp_sap_t *r = (prro_vidp_sap_t*)(SOC_PRRO_REG_BASE + (0xb << 2));
	return r->vid_ahb_yuv_nsec;
}

static inline void prro_ll_set_vidp_sap_vid_ahb_jpgd_nsec(uint32_t v) {
	prro_vidp_sap_t *r = (prro_vidp_sap_t*)(SOC_PRRO_REG_BASE + (0xb << 2));
	r->vid_ahb_jpgd_nsec = v;
}

static inline uint32_t prro_ll_get_vidp_sap_vid_ahb_jpgd_nsec(void) {
	prro_vidp_sap_t *r = (prro_vidp_sap_t*)(SOC_PRRO_REG_BASE + (0xb << 2));
	return r->vid_ahb_jpgd_nsec;
}

static inline void prro_ll_set_vidp_sap_vid_ahb_disp_nsec(uint32_t v) {
	prro_vidp_sap_t *r = (prro_vidp_sap_t*)(SOC_PRRO_REG_BASE + (0xb << 2));
	r->vid_ahb_disp_nsec = v;
}

static inline uint32_t prro_ll_get_vidp_sap_vid_ahb_disp_nsec(void) {
	prro_vidp_sap_t *r = (prro_vidp_sap_t*)(SOC_PRRO_REG_BASE + (0xb << 2));
	return r->vid_ahb_disp_nsec;
}

static inline void prro_ll_set_vidp_sap_vid_ahb_dmad_nsec(uint32_t v) {
	prro_vidp_sap_t *r = (prro_vidp_sap_t*)(SOC_PRRO_REG_BASE + (0xb << 2));
	r->vid_ahb_dmad_nsec = v;
}

static inline uint32_t prro_ll_get_vidp_sap_vid_ahb_dmad_nsec(void) {
	prro_vidp_sap_t *r = (prro_vidp_sap_t*)(SOC_PRRO_REG_BASE + (0xb << 2));
	return r->vid_ahb_dmad_nsec;
}

static inline void prro_ll_set_vidp_sap_vid_ahb_rott_nsec(uint32_t v) {
	prro_vidp_sap_t *r = (prro_vidp_sap_t*)(SOC_PRRO_REG_BASE + (0xb << 2));
	r->vid_ahb_rott_nsec = v;
}

static inline uint32_t prro_ll_get_vidp_sap_vid_ahb_rott_nsec(void) {
	prro_vidp_sap_t *r = (prro_vidp_sap_t*)(SOC_PRRO_REG_BASE + (0xb << 2));
	return r->vid_ahb_rott_nsec;
}

static inline void prro_ll_set_vidp_sap_vid_ahb_scal0_nsec(uint32_t v) {
	prro_vidp_sap_t *r = (prro_vidp_sap_t*)(SOC_PRRO_REG_BASE + (0xb << 2));
	r->vid_ahb_scal0_nsec = v;
}

static inline uint32_t prro_ll_get_vidp_sap_vid_ahb_scal0_nsec(void) {
	prro_vidp_sap_t *r = (prro_vidp_sap_t*)(SOC_PRRO_REG_BASE + (0xb << 2));
	return r->vid_ahb_scal0_nsec;
}

static inline void prro_ll_set_vidp_sap_vid_ahb_scal1_nsec(uint32_t v) {
	prro_vidp_sap_t *r = (prro_vidp_sap_t*)(SOC_PRRO_REG_BASE + (0xb << 2));
	r->vid_ahb_scal1_nsec = v;
}

static inline uint32_t prro_ll_get_vidp_sap_vid_ahb_scal1_nsec(void) {
	prro_vidp_sap_t *r = (prro_vidp_sap_t*)(SOC_PRRO_REG_BASE + (0xb << 2));
	return r->vid_ahb_scal1_nsec;
}

static inline void prro_ll_set_vidp_sap_vid_ahb_apb_nsec(uint32_t v) {
	prro_vidp_sap_t *r = (prro_vidp_sap_t*)(SOC_PRRO_REG_BASE + (0xb << 2));
	r->vid_ahb_apb_nsec = v;
}

static inline uint32_t prro_ll_get_vidp_sap_vid_ahb_apb_nsec(void) {
	prro_vidp_sap_t *r = (prro_vidp_sap_t*)(SOC_PRRO_REG_BASE + (0xb << 2));
	return r->vid_ahb_apb_nsec;
}

static inline void prro_ll_set_vidp_sap_vid_ahb_jpge_ap(uint32_t v) {
	prro_vidp_sap_t *r = (prro_vidp_sap_t*)(SOC_PRRO_REG_BASE + (0xb << 2));
	r->vid_ahb_jpge_ap = v;
}

static inline uint32_t prro_ll_get_vidp_sap_vid_ahb_jpge_ap(void) {
	prro_vidp_sap_t *r = (prro_vidp_sap_t*)(SOC_PRRO_REG_BASE + (0xb << 2));
	return r->vid_ahb_jpge_ap;
}

static inline void prro_ll_set_vidp_sap_vid_ahb_yuv_ap(uint32_t v) {
	prro_vidp_sap_t *r = (prro_vidp_sap_t*)(SOC_PRRO_REG_BASE + (0xb << 2));
	r->vid_ahb_yuv_ap = v;
}

static inline uint32_t prro_ll_get_vidp_sap_vid_ahb_yuv_ap(void) {
	prro_vidp_sap_t *r = (prro_vidp_sap_t*)(SOC_PRRO_REG_BASE + (0xb << 2));
	return r->vid_ahb_yuv_ap;
}

static inline void prro_ll_set_vidp_sap_vid_ahb_jpgd_ap(uint32_t v) {
	prro_vidp_sap_t *r = (prro_vidp_sap_t*)(SOC_PRRO_REG_BASE + (0xb << 2));
	r->vid_ahb_jpgd_ap = v;
}

static inline uint32_t prro_ll_get_vidp_sap_vid_ahb_jpgd_ap(void) {
	prro_vidp_sap_t *r = (prro_vidp_sap_t*)(SOC_PRRO_REG_BASE + (0xb << 2));
	return r->vid_ahb_jpgd_ap;
}

static inline void prro_ll_set_vidp_sap_vid_ahb_disp_ap(uint32_t v) {
	prro_vidp_sap_t *r = (prro_vidp_sap_t*)(SOC_PRRO_REG_BASE + (0xb << 2));
	r->vid_ahb_disp_ap = v;
}

static inline uint32_t prro_ll_get_vidp_sap_vid_ahb_disp_ap(void) {
	prro_vidp_sap_t *r = (prro_vidp_sap_t*)(SOC_PRRO_REG_BASE + (0xb << 2));
	return r->vid_ahb_disp_ap;
}

static inline void prro_ll_set_vidp_sap_vid_ahb_dmad_ap(uint32_t v) {
	prro_vidp_sap_t *r = (prro_vidp_sap_t*)(SOC_PRRO_REG_BASE + (0xb << 2));
	r->vid_ahb_dmad_ap = v;
}

static inline uint32_t prro_ll_get_vidp_sap_vid_ahb_dmad_ap(void) {
	prro_vidp_sap_t *r = (prro_vidp_sap_t*)(SOC_PRRO_REG_BASE + (0xb << 2));
	return r->vid_ahb_dmad_ap;
}

static inline void prro_ll_set_vidp_sap_vid_ahb_rott_ap(uint32_t v) {
	prro_vidp_sap_t *r = (prro_vidp_sap_t*)(SOC_PRRO_REG_BASE + (0xb << 2));
	r->vid_ahb_rott_ap = v;
}

static inline uint32_t prro_ll_get_vidp_sap_vid_ahb_rott_ap(void) {
	prro_vidp_sap_t *r = (prro_vidp_sap_t*)(SOC_PRRO_REG_BASE + (0xb << 2));
	return r->vid_ahb_rott_ap;
}

static inline void prro_ll_set_vidp_sap_vid_ahb_scal0_ap(uint32_t v) {
	prro_vidp_sap_t *r = (prro_vidp_sap_t*)(SOC_PRRO_REG_BASE + (0xb << 2));
	r->vid_ahb_scal0_ap = v;
}

static inline uint32_t prro_ll_get_vidp_sap_vid_ahb_scal0_ap(void) {
	prro_vidp_sap_t *r = (prro_vidp_sap_t*)(SOC_PRRO_REG_BASE + (0xb << 2));
	return r->vid_ahb_scal0_ap;
}

static inline void prro_ll_set_vidp_sap_vid_ahb_scal1_ap(uint32_t v) {
	prro_vidp_sap_t *r = (prro_vidp_sap_t*)(SOC_PRRO_REG_BASE + (0xb << 2));
	r->vid_ahb_scal1_ap = v;
}

static inline uint32_t prro_ll_get_vidp_sap_vid_ahb_scal1_ap(void) {
	prro_vidp_sap_t *r = (prro_vidp_sap_t*)(SOC_PRRO_REG_BASE + (0xb << 2));
	return r->vid_ahb_scal1_ap;
}

static inline void prro_ll_set_vidp_sap_vid_ahb_apb_ap(uint32_t v) {
	prro_vidp_sap_t *r = (prro_vidp_sap_t*)(SOC_PRRO_REG_BASE + (0xb << 2));
	r->vid_ahb_apb_ap = v;
}

static inline uint32_t prro_ll_get_vidp_sap_vid_ahb_apb_ap(void) {
	prro_vidp_sap_t *r = (prro_vidp_sap_t*)(SOC_PRRO_REG_BASE + (0xb << 2));
	return r->vid_ahb_apb_ap;
}

static inline void prro_ll_set_vidp_sap_vid_ahb_sresp(uint32_t v) {
	prro_vidp_sap_t *r = (prro_vidp_sap_t*)(SOC_PRRO_REG_BASE + (0xb << 2));
	r->vid_ahb_sresp = v;
}

static inline uint32_t prro_ll_get_vidp_sap_vid_ahb_sresp(void) {
	prro_vidp_sap_t *r = (prro_vidp_sap_t*)(SOC_PRRO_REG_BASE + (0xb << 2));
	return r->vid_ahb_sresp;
}

static inline void prro_ll_set_vidp_sap_vid_apb_h264_nsec(uint32_t v) {
	prro_vidp_sap_t *r = (prro_vidp_sap_t*)(SOC_PRRO_REG_BASE + (0xb << 2));
	r->vid_apb_h264_nsec = v;
}

static inline uint32_t prro_ll_get_vidp_sap_vid_apb_h264_nsec(void) {
	prro_vidp_sap_t *r = (prro_vidp_sap_t*)(SOC_PRRO_REG_BASE + (0xb << 2));
	return r->vid_apb_h264_nsec;
}

static inline void prro_ll_set_vidp_sap_vid_apb_h264_ap(uint32_t v) {
	prro_vidp_sap_t *r = (prro_vidp_sap_t*)(SOC_PRRO_REG_BASE + (0xb << 2));
	r->vid_apb_h264_ap = v;
}

static inline uint32_t prro_ll_get_vidp_sap_vid_apb_h264_ap(void) {
	prro_vidp_sap_t *r = (prro_vidp_sap_t*)(SOC_PRRO_REG_BASE + (0xb << 2));
	return r->vid_apb_h264_ap;
}

static inline void prro_ll_set_vidp_sap_vid_apb_h264_sresp(uint32_t v) {
	prro_vidp_sap_t *r = (prro_vidp_sap_t*)(SOC_PRRO_REG_BASE + (0xb << 2));
	r->vid_apb_h264_sresp = v;
}

static inline uint32_t prro_ll_get_vidp_sap_vid_apb_h264_sresp(void) {
	prro_vidp_sap_t *r = (prro_vidp_sap_t*)(SOC_PRRO_REG_BASE + (0xb << 2));
	return r->vid_apb_h264_sresp;
}

static inline void prro_ll_set_vidp_sap_reserved_bit_22_31(uint32_t v) {
	prro_vidp_sap_t *r = (prro_vidp_sap_t*)(SOC_PRRO_REG_BASE + (0xb << 2));
	r->reserved_bit_22_31 = v;
}

static inline uint32_t prro_ll_get_vidp_sap_reserved_bit_22_31(void) {
	prro_vidp_sap_t *r = (prro_vidp_sap_t*)(SOC_PRRO_REG_BASE + (0xb << 2));
	return r->reserved_bit_22_31;
}

//reg wifp_sap:

static inline void prro_ll_set_wifp_sap_value(uint32_t v) {
	prro_wifp_sap_t *r = (prro_wifp_sap_t*)(SOC_PRRO_REG_BASE + (0xc << 2));
	r->v = v;
}

static inline uint32_t prro_ll_get_wifp_sap_value(void) {
	prro_wifp_sap_t *r = (prro_wifp_sap_t*)(SOC_PRRO_REG_BASE + (0xc << 2));
	return r->v;
}

static inline void prro_ll_set_wifp_sap_wifi_ahb_mac_nsec(uint32_t v) {
	prro_wifp_sap_t *r = (prro_wifp_sap_t*)(SOC_PRRO_REG_BASE + (0xc << 2));
	r->wifi_ahb_mac_nsec = v;
}

static inline uint32_t prro_ll_get_wifp_sap_wifi_ahb_mac_nsec(void) {
	prro_wifp_sap_t *r = (prro_wifp_sap_t*)(SOC_PRRO_REG_BASE + (0xc << 2));
	return r->wifi_ahb_mac_nsec;
}

static inline void prro_ll_set_wifp_sap_wifi_ahb_mod_nsec(uint32_t v) {
	prro_wifp_sap_t *r = (prro_wifp_sap_t*)(SOC_PRRO_REG_BASE + (0xc << 2));
	r->wifi_ahb_mod_nsec = v;
}

static inline uint32_t prro_ll_get_wifp_sap_wifi_ahb_mod_nsec(void) {
	prro_wifp_sap_t *r = (prro_wifp_sap_t*)(SOC_PRRO_REG_BASE + (0xc << 2));
	return r->wifi_ahb_mod_nsec;
}

static inline void prro_ll_set_wifp_sap_wifi_ahb_mac_ap(uint32_t v) {
	prro_wifp_sap_t *r = (prro_wifp_sap_t*)(SOC_PRRO_REG_BASE + (0xc << 2));
	r->wifi_ahb_mac_ap = v;
}

static inline uint32_t prro_ll_get_wifp_sap_wifi_ahb_mac_ap(void) {
	prro_wifp_sap_t *r = (prro_wifp_sap_t*)(SOC_PRRO_REG_BASE + (0xc << 2));
	return r->wifi_ahb_mac_ap;
}

static inline void prro_ll_set_wifp_sap_wifi_ahb_mod_ap(uint32_t v) {
	prro_wifp_sap_t *r = (prro_wifp_sap_t*)(SOC_PRRO_REG_BASE + (0xc << 2));
	r->wifi_ahb_mod_ap = v;
}

static inline uint32_t prro_ll_get_wifp_sap_wifi_ahb_mod_ap(void) {
	prro_wifp_sap_t *r = (prro_wifp_sap_t*)(SOC_PRRO_REG_BASE + (0xc << 2));
	return r->wifi_ahb_mod_ap;
}

static inline void prro_ll_set_wifp_sap_wifi_ahb_sresp(uint32_t v) {
	prro_wifp_sap_t *r = (prro_wifp_sap_t*)(SOC_PRRO_REG_BASE + (0xc << 2));
	r->wifi_ahb_sresp = v;
}

static inline uint32_t prro_ll_get_wifp_sap_wifi_ahb_sresp(void) {
	prro_wifp_sap_t *r = (prro_wifp_sap_t*)(SOC_PRRO_REG_BASE + (0xc << 2));
	return r->wifi_ahb_sresp;
}

static inline void prro_ll_set_wifp_sap_reserved_bit_5_31(uint32_t v) {
	prro_wifp_sap_t *r = (prro_wifp_sap_t*)(SOC_PRRO_REG_BASE + (0xc << 2));
	r->reserved_bit_5_31 = v;
}

static inline uint32_t prro_ll_get_wifp_sap_reserved_bit_5_31(void) {
	prro_wifp_sap_t *r = (prro_wifp_sap_t*)(SOC_PRRO_REG_BASE + (0xc << 2));
	return r->reserved_bit_5_31;
}

//reg btsp_sap:

static inline void prro_ll_set_btsp_sap_value(uint32_t v) {
	prro_btsp_sap_t *r = (prro_btsp_sap_t*)(SOC_PRRO_REG_BASE + (0xd << 2));
	r->v = v;
}

static inline uint32_t prro_ll_get_btsp_sap_value(void) {
	prro_btsp_sap_t *r = (prro_btsp_sap_t*)(SOC_PRRO_REG_BASE + (0xd << 2));
	return r->v;
}

static inline void prro_ll_set_btsp_sap_btdm_ahb_btdm_nsec(uint32_t v) {
	prro_btsp_sap_t *r = (prro_btsp_sap_t*)(SOC_PRRO_REG_BASE + (0xd << 2));
	r->btdm_ahb_btdm_nsec = v;
}

static inline uint32_t prro_ll_get_btsp_sap_btdm_ahb_btdm_nsec(void) {
	prro_btsp_sap_t *r = (prro_btsp_sap_t*)(SOC_PRRO_REG_BASE + (0xd << 2));
	return r->btdm_ahb_btdm_nsec;
}

static inline void prro_ll_set_btsp_sap_btdm_ahb_apb_nsec(uint32_t v) {
	prro_btsp_sap_t *r = (prro_btsp_sap_t*)(SOC_PRRO_REG_BASE + (0xd << 2));
	r->btdm_ahb_apb_nsec = v;
}

static inline uint32_t prro_ll_get_btsp_sap_btdm_ahb_apb_nsec(void) {
	prro_btsp_sap_t *r = (prro_btsp_sap_t*)(SOC_PRRO_REG_BASE + (0xd << 2));
	return r->btdm_ahb_apb_nsec;
}

static inline void prro_ll_set_btsp_sap_btdm_ahb_btdm_ap(uint32_t v) {
	prro_btsp_sap_t *r = (prro_btsp_sap_t*)(SOC_PRRO_REG_BASE + (0xd << 2));
	r->btdm_ahb_btdm_ap = v;
}

static inline uint32_t prro_ll_get_btsp_sap_btdm_ahb_btdm_ap(void) {
	prro_btsp_sap_t *r = (prro_btsp_sap_t*)(SOC_PRRO_REG_BASE + (0xd << 2));
	return r->btdm_ahb_btdm_ap;
}

static inline void prro_ll_set_btsp_sap_btdm_ahb_apb_ap(uint32_t v) {
	prro_btsp_sap_t *r = (prro_btsp_sap_t*)(SOC_PRRO_REG_BASE + (0xd << 2));
	r->btdm_ahb_apb_ap = v;
}

static inline uint32_t prro_ll_get_btsp_sap_btdm_ahb_apb_ap(void) {
	prro_btsp_sap_t *r = (prro_btsp_sap_t*)(SOC_PRRO_REG_BASE + (0xd << 2));
	return r->btdm_ahb_apb_ap;
}

static inline void prro_ll_set_btsp_sap_btdm_ahb_sresp(uint32_t v) {
	prro_btsp_sap_t *r = (prro_btsp_sap_t*)(SOC_PRRO_REG_BASE + (0xd << 2));
	r->btdm_ahb_sresp = v;
}

static inline uint32_t prro_ll_get_btsp_sap_btdm_ahb_sresp(void) {
	prro_btsp_sap_t *r = (prro_btsp_sap_t*)(SOC_PRRO_REG_BASE + (0xd << 2));
	return r->btdm_ahb_sresp;
}

static inline void prro_ll_set_btsp_sap_btdm_apb_xvr_nsec(uint32_t v) {
	prro_btsp_sap_t *r = (prro_btsp_sap_t*)(SOC_PRRO_REG_BASE + (0xd << 2));
	r->btdm_apb_xvr_nsec = v;
}

static inline uint32_t prro_ll_get_btsp_sap_btdm_apb_xvr_nsec(void) {
	prro_btsp_sap_t *r = (prro_btsp_sap_t*)(SOC_PRRO_REG_BASE + (0xd << 2));
	return r->btdm_apb_xvr_nsec;
}

static inline void prro_ll_set_btsp_sap_btdm_apb_xvr_ap(uint32_t v) {
	prro_btsp_sap_t *r = (prro_btsp_sap_t*)(SOC_PRRO_REG_BASE + (0xd << 2));
	r->btdm_apb_xvr_ap = v;
}

static inline uint32_t prro_ll_get_btsp_sap_btdm_apb_xvr_ap(void) {
	prro_btsp_sap_t *r = (prro_btsp_sap_t*)(SOC_PRRO_REG_BASE + (0xd << 2));
	return r->btdm_apb_xvr_ap;
}

static inline void prro_ll_set_btsp_sap_btdm_apb_sresp(uint32_t v) {
	prro_btsp_sap_t *r = (prro_btsp_sap_t*)(SOC_PRRO_REG_BASE + (0xd << 2));
	r->btdm_apb_sresp = v;
}

static inline uint32_t prro_ll_get_btsp_sap_btdm_apb_sresp(void) {
	prro_btsp_sap_t *r = (prro_btsp_sap_t*)(SOC_PRRO_REG_BASE + (0xd << 2));
	return r->btdm_apb_sresp;
}

static inline void prro_ll_set_btsp_sap_reserved_bit_8_31(uint32_t v) {
	prro_btsp_sap_t *r = (prro_btsp_sap_t*)(SOC_PRRO_REG_BASE + (0xd << 2));
	r->reserved_bit_8_31 = v;
}

static inline uint32_t prro_ll_get_btsp_sap_reserved_bit_8_31(void) {
	prro_btsp_sap_t *r = (prro_btsp_sap_t*)(SOC_PRRO_REG_BASE + (0xd << 2));
	return r->reserved_bit_8_31;
}

//reg mbox_sap:

static inline void prro_ll_set_mbox_sap_value(uint32_t v) {
	prro_mbox_sap_t *r = (prro_mbox_sap_t*)(SOC_PRRO_REG_BASE + (0xe << 2));
	r->v = v;
}

static inline uint32_t prro_ll_get_mbox_sap_value(void) {
	prro_mbox_sap_t *r = (prro_mbox_sap_t*)(SOC_PRRO_REG_BASE + (0xe << 2));
	return r->v;
}

static inline void prro_ll_set_mbox_sap_mbox_ahb_nsec(uint32_t v) {
	prro_mbox_sap_t *r = (prro_mbox_sap_t*)(SOC_PRRO_REG_BASE + (0xe << 2));
	r->mbox_ahb_nsec = v;
}

static inline uint32_t prro_ll_get_mbox_sap_mbox_ahb_nsec(void) {
	prro_mbox_sap_t *r = (prro_mbox_sap_t*)(SOC_PRRO_REG_BASE + (0xe << 2));
	return r->mbox_ahb_nsec;
}

static inline void prro_ll_set_mbox_sap_mbox_ahb_ap(uint32_t v) {
	prro_mbox_sap_t *r = (prro_mbox_sap_t*)(SOC_PRRO_REG_BASE + (0xe << 2));
	r->mbox_ahb_ap = v;
}

static inline uint32_t prro_ll_get_mbox_sap_mbox_ahb_ap(void) {
	prro_mbox_sap_t *r = (prro_mbox_sap_t*)(SOC_PRRO_REG_BASE + (0xe << 2));
	return r->mbox_ahb_ap;
}

static inline void prro_ll_set_mbox_sap_ppro_mpc_ap(uint32_t v) {
	prro_mbox_sap_t *r = (prro_mbox_sap_t*)(SOC_PRRO_REG_BASE + (0xe << 2));
	r->ppro_mpc_ap = v;
}

static inline uint32_t prro_ll_get_mbox_sap_ppro_mpc_ap(void) {
	prro_mbox_sap_t *r = (prro_mbox_sap_t*)(SOC_PRRO_REG_BASE + (0xe << 2));
	return r->ppro_mpc_ap;
}

static inline void prro_ll_set_mbox_sap_reserved_bit_3_31(uint32_t v) {
	prro_mbox_sap_t *r = (prro_mbox_sap_t*)(SOC_PRRO_REG_BASE + (0xe << 2));
	r->reserved_bit_3_31 = v;
}

static inline uint32_t prro_ll_get_mbox_sap_reserved_bit_3_31(void) {
	prro_mbox_sap_t *r = (prro_mbox_sap_t*)(SOC_PRRO_REG_BASE + (0xe << 2));
	return r->reserved_bit_3_31;
}

//reg master_hnsec:

static inline void prro_ll_set_master_hnsec_value(uint32_t v) {
	prro_master_hnsec_t *r = (prro_master_hnsec_t*)(SOC_PRRO_REG_BASE + (0xf << 2));
	r->v = v;
}

static inline uint32_t prro_ll_get_master_hnsec_value(void) {
	prro_master_hnsec_t *r = (prro_master_hnsec_t*)(SOC_PRRO_REG_BASE + (0xf << 2));
	return r->v;
}

static inline void prro_ll_set_master_hnsec_jpenc_m_hnsec(uint32_t v) {
	prro_master_hnsec_t *r = (prro_master_hnsec_t*)(SOC_PRRO_REG_BASE + (0xf << 2));
	r->jpenc_m_hnsec = v;
}

static inline uint32_t prro_ll_get_master_hnsec_jpenc_m_hnsec(void) {
	prro_master_hnsec_t *r = (prro_master_hnsec_t*)(SOC_PRRO_REG_BASE + (0xf << 2));
	return r->jpenc_m_hnsec;
}

static inline void prro_ll_set_master_hnsec_jpdec_m_hnsec(uint32_t v) {
	prro_master_hnsec_t *r = (prro_master_hnsec_t*)(SOC_PRRO_REG_BASE + (0xf << 2));
	r->jpdec_m_hnsec = v;
}

static inline uint32_t prro_ll_get_master_hnsec_jpdec_m_hnsec(void) {
	prro_master_hnsec_t *r = (prro_master_hnsec_t*)(SOC_PRRO_REG_BASE + (0xf << 2));
	return r->jpdec_m_hnsec;
}

static inline void prro_ll_set_master_hnsec_dmad_m_hnsec(uint32_t v) {
	prro_master_hnsec_t *r = (prro_master_hnsec_t*)(SOC_PRRO_REG_BASE + (0xf << 2));
	r->dmad_m_hnsec = v;
}

static inline uint32_t prro_ll_get_master_hnsec_dmad_m_hnsec(void) {
	prro_master_hnsec_t *r = (prro_master_hnsec_t*)(SOC_PRRO_REG_BASE + (0xf << 2));
	return r->dmad_m_hnsec;
}

static inline void prro_ll_set_master_hnsec_mac_m_hnsec(uint32_t v) {
	prro_master_hnsec_t *r = (prro_master_hnsec_t*)(SOC_PRRO_REG_BASE + (0xf << 2));
	r->mac_m_hnsec = v;
}

static inline uint32_t prro_ll_get_master_hnsec_mac_m_hnsec(void) {
	prro_master_hnsec_t *r = (prro_master_hnsec_t*)(SOC_PRRO_REG_BASE + (0xf << 2));
	return r->mac_m_hnsec;
}

static inline void prro_ll_set_master_hnsec_hsu_m_hnsec(uint32_t v) {
	prro_master_hnsec_t *r = (prro_master_hnsec_t*)(SOC_PRRO_REG_BASE + (0xf << 2));
	r->hsu_m_hnsec = v;
}

static inline uint32_t prro_ll_get_master_hnsec_hsu_m_hnsec(void) {
	prro_master_hnsec_t *r = (prro_master_hnsec_t*)(SOC_PRRO_REG_BASE + (0xf << 2));
	return r->hsu_m_hnsec;
}

static inline void prro_ll_set_master_hnsec_btdm_m_hnsec(uint32_t v) {
	prro_master_hnsec_t *r = (prro_master_hnsec_t*)(SOC_PRRO_REG_BASE + (0xf << 2));
	r->btdm_m_hnsec = v;
}

static inline uint32_t prro_ll_get_master_hnsec_btdm_m_hnsec(void) {
	prro_master_hnsec_t *r = (prro_master_hnsec_t*)(SOC_PRRO_REG_BASE + (0xf << 2));
	return r->btdm_m_hnsec;
}

static inline void prro_ll_set_master_hnsec_la_m_hnsec(uint32_t v) {
	prro_master_hnsec_t *r = (prro_master_hnsec_t*)(SOC_PRRO_REG_BASE + (0xf << 2));
	r->la_m_hnsec = v;
}

static inline uint32_t prro_ll_get_master_hnsec_la_m_hnsec(void) {
	prro_master_hnsec_t *r = (prro_master_hnsec_t*)(SOC_PRRO_REG_BASE + (0xf << 2));
	return r->la_m_hnsec;
}

static inline void prro_ll_set_master_hnsec_disp_m_hnsec(uint32_t v) {
	prro_master_hnsec_t *r = (prro_master_hnsec_t*)(SOC_PRRO_REG_BASE + (0xf << 2));
	r->disp_m_hnsec = v;
}

static inline uint32_t prro_ll_get_master_hnsec_disp_m_hnsec(void) {
	prro_master_hnsec_t *r = (prro_master_hnsec_t*)(SOC_PRRO_REG_BASE + (0xf << 2));
	return r->disp_m_hnsec;
}

static inline void prro_ll_set_master_hnsec_rott_m_hnsec(uint32_t v) {
	prro_master_hnsec_t *r = (prro_master_hnsec_t*)(SOC_PRRO_REG_BASE + (0xf << 2));
	r->rott_m_hnsec = v;
}

static inline uint32_t prro_ll_get_master_hnsec_rott_m_hnsec(void) {
	prro_master_hnsec_t *r = (prro_master_hnsec_t*)(SOC_PRRO_REG_BASE + (0xf << 2));
	return r->rott_m_hnsec;
}

static inline void prro_ll_set_master_hnsec_usb_m_hnsec(uint32_t v) {
	prro_master_hnsec_t *r = (prro_master_hnsec_t*)(SOC_PRRO_REG_BASE + (0xf << 2));
	r->usb_m_hnsec = v;
}

static inline uint32_t prro_ll_get_master_hnsec_usb_m_hnsec(void) {
	prro_master_hnsec_t *r = (prro_master_hnsec_t*)(SOC_PRRO_REG_BASE + (0xf << 2));
	return r->usb_m_hnsec;
}

static inline void prro_ll_set_master_hnsec_enet_m_hnsec(uint32_t v) {
	prro_master_hnsec_t *r = (prro_master_hnsec_t*)(SOC_PRRO_REG_BASE + (0xf << 2));
	r->enet_m_hnsec = v;
}

static inline uint32_t prro_ll_get_master_hnsec_enet_m_hnsec(void) {
	prro_master_hnsec_t *r = (prro_master_hnsec_t*)(SOC_PRRO_REG_BASE + (0xf << 2));
	return r->enet_m_hnsec;
}

static inline void prro_ll_set_master_hnsec_scal0_m_hnsec(uint32_t v) {
	prro_master_hnsec_t *r = (prro_master_hnsec_t*)(SOC_PRRO_REG_BASE + (0xf << 2));
	r->scal0_m_hnsec = v;
}

static inline uint32_t prro_ll_get_master_hnsec_scal0_m_hnsec(void) {
	prro_master_hnsec_t *r = (prro_master_hnsec_t*)(SOC_PRRO_REG_BASE + (0xf << 2));
	return r->scal0_m_hnsec;
}

static inline void prro_ll_set_master_hnsec_scal1_m_hnsec(uint32_t v) {
	prro_master_hnsec_t *r = (prro_master_hnsec_t*)(SOC_PRRO_REG_BASE + (0xf << 2));
	r->scal1_m_hnsec = v;
}

static inline uint32_t prro_ll_get_master_hnsec_scal1_m_hnsec(void) {
	prro_master_hnsec_t *r = (prro_master_hnsec_t*)(SOC_PRRO_REG_BASE + (0xf << 2));
	return r->scal1_m_hnsec;
}

static inline void prro_ll_set_master_hnsec_reserved_bit_13_31(uint32_t v) {
	prro_master_hnsec_t *r = (prro_master_hnsec_t*)(SOC_PRRO_REG_BASE + (0xf << 2));
	r->reserved_bit_13_31 = v;
}

static inline uint32_t prro_ll_get_master_hnsec_reserved_bit_13_31(void) {
	prro_master_hnsec_t *r = (prro_master_hnsec_t*)(SOC_PRRO_REG_BASE + (0xf << 2));
	return r->reserved_bit_13_31;
}

//reg cmp0_addr_start:

static inline void prro_ll_set_cmp0_addr_start_value(uint32_t v) {
	prro_cmp0_addr_start_t *r = (prro_cmp0_addr_start_t*)(SOC_PRRO_REG_BASE + (0x17 << 2));
	r->v = v;
}

static inline uint32_t prro_ll_get_cmp0_addr_start_value(void) {
	prro_cmp0_addr_start_t *r = (prro_cmp0_addr_start_t*)(SOC_PRRO_REG_BASE + (0x17 << 2));
	return r->v;
}

static inline void prro_ll_set_cmp0_addr_start_cmp0_addr_start(uint32_t v) {
	prro_cmp0_addr_start_t *r = (prro_cmp0_addr_start_t*)(SOC_PRRO_REG_BASE + (0x17 << 2));
	r->cmp0_addr_start = v;
}

static inline uint32_t prro_ll_get_cmp0_addr_start_cmp0_addr_start(void) {
	prro_cmp0_addr_start_t *r = (prro_cmp0_addr_start_t*)(SOC_PRRO_REG_BASE + (0x17 << 2));
	return r->cmp0_addr_start;
}

//reg cmp0_end_start:

static inline void prro_ll_set_cmp0_end_start_value(uint32_t v) {
	prro_cmp0_end_start_t *r = (prro_cmp0_end_start_t*)(SOC_PRRO_REG_BASE + (0x18 << 2));
	r->v = v;
}

static inline uint32_t prro_ll_get_cmp0_end_start_value(void) {
	prro_cmp0_end_start_t *r = (prro_cmp0_end_start_t*)(SOC_PRRO_REG_BASE + (0x18 << 2));
	return r->v;
}

static inline void prro_ll_set_cmp0_end_start_cmp0_addr_end(uint32_t v) {
	prro_cmp0_end_start_t *r = (prro_cmp0_end_start_t*)(SOC_PRRO_REG_BASE + (0x18 << 2));
	r->cmp0_addr_end = v;
}

static inline uint32_t prro_ll_get_cmp0_end_start_cmp0_addr_end(void) {
	prro_cmp0_end_start_t *r = (prro_cmp0_end_start_t*)(SOC_PRRO_REG_BASE + (0x18 << 2));
	return r->cmp0_addr_end;
}

//reg cmp0_data_rega:

static inline void prro_ll_set_cmp0_data_rega_value(uint32_t v) {
	prro_cmp0_data_rega_t *r = (prro_cmp0_data_rega_t*)(SOC_PRRO_REG_BASE + (0x19 << 2));
	r->v = v;
}

static inline uint32_t prro_ll_get_cmp0_data_rega_value(void) {
	prro_cmp0_data_rega_t *r = (prro_cmp0_data_rega_t*)(SOC_PRRO_REG_BASE + (0x19 << 2));
	return r->v;
}

static inline void prro_ll_set_cmp0_data_rega_cmp0_data_src(uint32_t v) {
	prro_cmp0_data_rega_t *r = (prro_cmp0_data_rega_t*)(SOC_PRRO_REG_BASE + (0x19 << 2));
	r->cmp0_data_src = v;
}

static inline uint32_t prro_ll_get_cmp0_data_rega_cmp0_data_src(void) {
	prro_cmp0_data_rega_t *r = (prro_cmp0_data_rega_t*)(SOC_PRRO_REG_BASE + (0x19 << 2));
	return r->cmp0_data_src;
}

//reg cmp0_data_regb:

static inline void prro_ll_set_cmp0_data_regb_value(uint32_t v) {
	prro_cmp0_data_regb_t *r = (prro_cmp0_data_regb_t*)(SOC_PRRO_REG_BASE + (0x1a << 2));
	r->v = v;
}

static inline uint32_t prro_ll_get_cmp0_data_regb_value(void) {
	prro_cmp0_data_regb_t *r = (prro_cmp0_data_regb_t*)(SOC_PRRO_REG_BASE + (0x1a << 2));
	return r->v;
}

static inline void prro_ll_set_cmp0_data_regb_cmp0_data_dst(uint32_t v) {
	prro_cmp0_data_regb_t *r = (prro_cmp0_data_regb_t*)(SOC_PRRO_REG_BASE + (0x1a << 2));
	r->cmp0_data_dst = v;
}

static inline uint32_t prro_ll_get_cmp0_data_regb_cmp0_data_dst(void) {
	prro_cmp0_data_regb_t *r = (prro_cmp0_data_regb_t*)(SOC_PRRO_REG_BASE + (0x1a << 2));
	return r->cmp0_data_dst;
}

//reg cmp1_addr_start:

static inline void prro_ll_set_cmp1_addr_start_value(uint32_t v) {
	prro_cmp1_addr_start_t *r = (prro_cmp1_addr_start_t*)(SOC_PRRO_REG_BASE + (0x1b << 2));
	r->v = v;
}

static inline uint32_t prro_ll_get_cmp1_addr_start_value(void) {
	prro_cmp1_addr_start_t *r = (prro_cmp1_addr_start_t*)(SOC_PRRO_REG_BASE + (0x1b << 2));
	return r->v;
}

static inline void prro_ll_set_cmp1_addr_start_cmp1_addr_start(uint32_t v) {
	prro_cmp1_addr_start_t *r = (prro_cmp1_addr_start_t*)(SOC_PRRO_REG_BASE + (0x1b << 2));
	r->cmp1_addr_start = v;
}

static inline uint32_t prro_ll_get_cmp1_addr_start_cmp1_addr_start(void) {
	prro_cmp1_addr_start_t *r = (prro_cmp1_addr_start_t*)(SOC_PRRO_REG_BASE + (0x1b << 2));
	return r->cmp1_addr_start;
}

//reg cmp1_end_start:

static inline void prro_ll_set_cmp1_end_start_value(uint32_t v) {
	prro_cmp1_end_start_t *r = (prro_cmp1_end_start_t*)(SOC_PRRO_REG_BASE + (0x1c << 2));
	r->v = v;
}

static inline uint32_t prro_ll_get_cmp1_end_start_value(void) {
	prro_cmp1_end_start_t *r = (prro_cmp1_end_start_t*)(SOC_PRRO_REG_BASE + (0x1c << 2));
	return r->v;
}

static inline void prro_ll_set_cmp1_end_start_cmp1_addr_end(uint32_t v) {
	prro_cmp1_end_start_t *r = (prro_cmp1_end_start_t*)(SOC_PRRO_REG_BASE + (0x1c << 2));
	r->cmp1_addr_end = v;
}

static inline uint32_t prro_ll_get_cmp1_end_start_cmp1_addr_end(void) {
	prro_cmp1_end_start_t *r = (prro_cmp1_end_start_t*)(SOC_PRRO_REG_BASE + (0x1c << 2));
	return r->cmp1_addr_end;
}

//reg cmp1_data_rega:

static inline void prro_ll_set_cmp1_data_rega_value(uint32_t v) {
	prro_cmp1_data_rega_t *r = (prro_cmp1_data_rega_t*)(SOC_PRRO_REG_BASE + (0x1d << 2));
	r->v = v;
}

static inline uint32_t prro_ll_get_cmp1_data_rega_value(void) {
	prro_cmp1_data_rega_t *r = (prro_cmp1_data_rega_t*)(SOC_PRRO_REG_BASE + (0x1d << 2));
	return r->v;
}

static inline void prro_ll_set_cmp1_data_rega_cmp1_data_src(uint32_t v) {
	prro_cmp1_data_rega_t *r = (prro_cmp1_data_rega_t*)(SOC_PRRO_REG_BASE + (0x1d << 2));
	r->cmp1_data_src = v;
}

static inline uint32_t prro_ll_get_cmp1_data_rega_cmp1_data_src(void) {
	prro_cmp1_data_rega_t *r = (prro_cmp1_data_rega_t*)(SOC_PRRO_REG_BASE + (0x1d << 2));
	return r->cmp1_data_src;
}

//reg cmp1_data_regb:

static inline void prro_ll_set_cmp1_data_regb_value(uint32_t v) {
	prro_cmp1_data_regb_t *r = (prro_cmp1_data_regb_t*)(SOC_PRRO_REG_BASE + (0x1e << 2));
	r->v = v;
}

static inline uint32_t prro_ll_get_cmp1_data_regb_value(void) {
	prro_cmp1_data_regb_t *r = (prro_cmp1_data_regb_t*)(SOC_PRRO_REG_BASE + (0x1e << 2));
	return r->v;
}

static inline void prro_ll_set_cmp1_data_regb_cmp1_data_dst(uint32_t v) {
	prro_cmp1_data_regb_t *r = (prro_cmp1_data_regb_t*)(SOC_PRRO_REG_BASE + (0x1e << 2));
	r->cmp1_data_dst = v;
}

static inline uint32_t prro_ll_get_cmp1_data_regb_cmp1_data_dst(void) {
	prro_cmp1_data_regb_t *r = (prro_cmp1_data_regb_t*)(SOC_PRRO_REG_BASE + (0x1e << 2));
	return r->cmp1_data_dst;
}

//reg cmp2_addr_start:

static inline void prro_ll_set_cmp2_addr_start_value(uint32_t v) {
	prro_cmp2_addr_start_t *r = (prro_cmp2_addr_start_t*)(SOC_PRRO_REG_BASE + (0x1f << 2));
	r->v = v;
}

static inline uint32_t prro_ll_get_cmp2_addr_start_value(void) {
	prro_cmp2_addr_start_t *r = (prro_cmp2_addr_start_t*)(SOC_PRRO_REG_BASE + (0x1f << 2));
	return r->v;
}

static inline void prro_ll_set_cmp2_addr_start_cmp2_addr_start(uint32_t v) {
	prro_cmp2_addr_start_t *r = (prro_cmp2_addr_start_t*)(SOC_PRRO_REG_BASE + (0x1f << 2));
	r->cmp2_addr_start = v;
}

static inline uint32_t prro_ll_get_cmp2_addr_start_cmp2_addr_start(void) {
	prro_cmp2_addr_start_t *r = (prro_cmp2_addr_start_t*)(SOC_PRRO_REG_BASE + (0x1f << 2));
	return r->cmp2_addr_start;
}

//reg cmp2_end_start:

static inline void prro_ll_set_cmp2_end_start_value(uint32_t v) {
	prro_cmp2_end_start_t *r = (prro_cmp2_end_start_t*)(SOC_PRRO_REG_BASE + (0x20 << 2));
	r->v = v;
}

static inline uint32_t prro_ll_get_cmp2_end_start_value(void) {
	prro_cmp2_end_start_t *r = (prro_cmp2_end_start_t*)(SOC_PRRO_REG_BASE + (0x20 << 2));
	return r->v;
}

static inline void prro_ll_set_cmp2_end_start_cmp2_addr_end(uint32_t v) {
	prro_cmp2_end_start_t *r = (prro_cmp2_end_start_t*)(SOC_PRRO_REG_BASE + (0x20 << 2));
	r->cmp2_addr_end = v;
}

static inline uint32_t prro_ll_get_cmp2_end_start_cmp2_addr_end(void) {
	prro_cmp2_end_start_t *r = (prro_cmp2_end_start_t*)(SOC_PRRO_REG_BASE + (0x20 << 2));
	return r->cmp2_addr_end;
}

//reg cmp2_data_rega:

static inline void prro_ll_set_cmp2_data_rega_value(uint32_t v) {
	prro_cmp2_data_rega_t *r = (prro_cmp2_data_rega_t*)(SOC_PRRO_REG_BASE + (0x21 << 2));
	r->v = v;
}

static inline uint32_t prro_ll_get_cmp2_data_rega_value(void) {
	prro_cmp2_data_rega_t *r = (prro_cmp2_data_rega_t*)(SOC_PRRO_REG_BASE + (0x21 << 2));
	return r->v;
}

static inline void prro_ll_set_cmp2_data_rega_cmp2_data_src(uint32_t v) {
	prro_cmp2_data_rega_t *r = (prro_cmp2_data_rega_t*)(SOC_PRRO_REG_BASE + (0x21 << 2));
	r->cmp2_data_src = v;
}

static inline uint32_t prro_ll_get_cmp2_data_rega_cmp2_data_src(void) {
	prro_cmp2_data_rega_t *r = (prro_cmp2_data_rega_t*)(SOC_PRRO_REG_BASE + (0x21 << 2));
	return r->cmp2_data_src;
}

//reg cmp2_data_regb:

static inline void prro_ll_set_cmp2_data_regb_value(uint32_t v) {
	prro_cmp2_data_regb_t *r = (prro_cmp2_data_regb_t*)(SOC_PRRO_REG_BASE + (0x22 << 2));
	r->v = v;
}

static inline uint32_t prro_ll_get_cmp2_data_regb_value(void) {
	prro_cmp2_data_regb_t *r = (prro_cmp2_data_regb_t*)(SOC_PRRO_REG_BASE + (0x22 << 2));
	return r->v;
}

static inline void prro_ll_set_cmp2_data_regb_cmp2_data_dst(uint32_t v) {
	prro_cmp2_data_regb_t *r = (prro_cmp2_data_regb_t*)(SOC_PRRO_REG_BASE + (0x22 << 2));
	r->cmp2_data_dst = v;
}

static inline uint32_t prro_ll_get_cmp2_data_regb_cmp2_data_dst(void) {
	prro_cmp2_data_regb_t *r = (prro_cmp2_data_regb_t*)(SOC_PRRO_REG_BASE + (0x22 << 2));
	return r->cmp2_data_dst;
}

//reg cmp_int_status:

static inline void prro_ll_set_cmp_int_status_value(uint32_t v) {
	prro_cmp_int_status_t *r = (prro_cmp_int_status_t*)(SOC_PRRO_REG_BASE + (0x23 << 2));
	r->v = v;
}

static inline uint32_t prro_ll_get_cmp_int_status_value(void) {
	prro_cmp_int_status_t *r = (prro_cmp_int_status_t*)(SOC_PRRO_REG_BASE + (0x23 << 2));
	return r->v;
}

static inline void prro_ll_set_cmp_int_status_cmp0_mismatch_int(uint32_t v) {
	prro_cmp_int_status_t *r = (prro_cmp_int_status_t*)(SOC_PRRO_REG_BASE + (0x23 << 2));
	r->cmp0_mismatch_int = v;
}

static inline uint32_t prro_ll_get_cmp_int_status_cmp0_mismatch_int(void) {
	prro_cmp_int_status_t *r = (prro_cmp_int_status_t*)(SOC_PRRO_REG_BASE + (0x23 << 2));
	return r->cmp0_mismatch_int;
}

static inline void prro_ll_set_cmp_int_status_cmp1_mismatch_int(uint32_t v) {
	prro_cmp_int_status_t *r = (prro_cmp_int_status_t*)(SOC_PRRO_REG_BASE + (0x23 << 2));
	r->cmp1_mismatch_int = v;
}

static inline uint32_t prro_ll_get_cmp_int_status_cmp1_mismatch_int(void) {
	prro_cmp_int_status_t *r = (prro_cmp_int_status_t*)(SOC_PRRO_REG_BASE + (0x23 << 2));
	return r->cmp1_mismatch_int;
}

static inline void prro_ll_set_cmp_int_status_cmp2_mismatch_int(uint32_t v) {
	prro_cmp_int_status_t *r = (prro_cmp_int_status_t*)(SOC_PRRO_REG_BASE + (0x23 << 2));
	r->cmp2_mismatch_int = v;
}

static inline uint32_t prro_ll_get_cmp_int_status_cmp2_mismatch_int(void) {
	prro_cmp_int_status_t *r = (prro_cmp_int_status_t*)(SOC_PRRO_REG_BASE + (0x23 << 2));
	return r->cmp2_mismatch_int;
}

static inline void prro_ll_set_cmp_int_status_reserved_bit_3_31(uint32_t v) {
	prro_cmp_int_status_t *r = (prro_cmp_int_status_t*)(SOC_PRRO_REG_BASE + (0x23 << 2));
	r->reserved_bit_3_31 = v;
}

static inline uint32_t prro_ll_get_cmp_int_status_reserved_bit_3_31(void) {
	prro_cmp_int_status_t *r = (prro_cmp_int_status_t*)(SOC_PRRO_REG_BASE + (0x23 << 2));
	return r->reserved_bit_3_31;
}

static inline void prro_ll_set_reg_bit(uint32_t reg_offset, uint32_t bit, uint32_t value)
{
	uint32_t reg = PRRO_LL_REG_BASE + (reg_offset << 2);

	if (value) {
		*(volatile uint32_t*)reg |= BIT(bit);
	} else {
		*(volatile uint32_t*)reg &= ~(BIT(bit));
	}
}

static inline uint32_t prro_ll_get_reg_bit(uint32_t reg_offset, uint32_t bit)
{
	uint32_t reg = PRRO_LL_REG_BASE + (reg_offset << 2);
	uint32_t bit_val = ((*(volatile uint32_t*)reg) & BIT(bit)) >> bit;

	return bit_val;
}

#ifdef __cplusplus
}
#endif
